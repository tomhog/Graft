/*  -*-c++-*-
 *  Copyright (C) 2015 Thomas Hogarth <tom@hogbox.com>, adapted from osgconv
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/

#include <hbx/PluginRegistry.h>
#include <osg/NodeVisitor>
#include <osg/Texture2D>
#include <osg/Texture3D>
#include <osgViewer/GraphicsWindow>

#include <osgDB/FileNameUtils>


class MyGraphicsContext {
    public:
        MyGraphicsContext()
        {
            osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
            traits->x = 0;
            traits->y = 0;
            traits->width = 1;
            traits->height = 1;
            traits->windowDecoration = false;
            traits->doubleBuffer = false;
            traits->sharedContext = 0;
            traits->pbuffer = true;

            _gc = osg::GraphicsContext::createGraphicsContext(traits.get());

            if (!_gc)
            {
                osg::notify(osg::NOTICE)<<"Failed to create pbuffer, failing back to normal graphics window."<<std::endl;

                traits->pbuffer = false;
                _gc = osg::GraphicsContext::createGraphicsContext(traits.get());
            }

            if (_gc.valid())
            {
                _gc->realize();
                _gc->makeCurrent();
                if (dynamic_cast<osgViewer::GraphicsWindow*>(_gc.get()))
                {
                    std::cout<<"Realized graphics window for OpenGL operations."<<std::endl;
                }
                else
                {
                    std::cout<<"Realized pbuffer for OpenGL operations."<<std::endl;
                }
            }
        }

        bool valid() const { return _gc.valid() && _gc->isRealized(); }

    private:
        osg::ref_ptr<osg::GraphicsContext> _gc;
        osg::ref_ptr<osg::GraphicsContext> _restoreContext;
};

class CompressTexturesVisitor : public osg::NodeVisitor
{
public:

    CompressTexturesVisitor(osg::Texture::InternalFormatMode internalFormatMode):
        osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
        _internalFormatMode(internalFormatMode) {}

    virtual void apply(osg::Node& node)
    {
        if (node.getStateSet()) apply(*node.getStateSet());
        traverse(node);
    }

    virtual void apply(osg::Geode& node)
    {
        if (node.getStateSet()) apply(*node.getStateSet());

        for(unsigned int i=0;i<node.getNumDrawables();++i)
        {
            osg::Drawable* drawable = node.getDrawable(i);
            if (drawable && drawable->getStateSet()) apply(*drawable->getStateSet());
        }

        traverse(node);
    }

    virtual void apply(osg::StateSet& stateset)
    {
        // search for the existence of any texture object attributes
        for(unsigned int i=0;i<stateset.getTextureAttributeList().size();++i)
        {
            osg::Texture* texture = dynamic_cast<osg::Texture*>(stateset.getTextureAttribute(i,osg::StateAttribute::TEXTURE));
            if (texture)
            {
                _textureSet.insert(texture);
            }
        }
    }

    void compress()
    {
        /*MyGraphicsContext context;
        if (!context.valid())
        {
            osg::notify(osg::NOTICE)<<"Error: Unable to create graphis context, problem with running osgViewer, cannot run compression."<<std::endl;
            return;
        }*/


        osg::ref_ptr<osg::State> state = new osg::State;
        state->initializeExtensionProcs();

        for(TextureSet::iterator itr=_textureSet.begin();
            itr!=_textureSet.end();
            ++itr)
        {
            osg::Texture* texture = const_cast<osg::Texture*>(itr->get());

            osg::Texture2D* texture2D = dynamic_cast<osg::Texture2D*>(texture);
            osg::Texture3D* texture3D = dynamic_cast<osg::Texture3D*>(texture);

            osg::ref_ptr<osg::Image> image = texture2D ? texture2D->getImage() : (texture3D ? texture3D->getImage() : 0);
            if (image.valid() &&
                (image->getPixelFormat()==GL_RGB || image->getPixelFormat()==GL_RGBA) &&
                (image->s()>=32 && image->t()>=32))
            {
                texture->setInternalFormatMode(_internalFormatMode);

                // need to disable the unref after apply, otherwise the image could go out of scope.
                bool unrefImageDataAfterApply = texture->getUnRefImageDataAfterApply();
                texture->setUnRefImageDataAfterApply(false);

                // get OpenGL driver to create texture from image.
                texture->apply(*state);

                // restore the original setting
                texture->setUnRefImageDataAfterApply(unrefImageDataAfterApply);

                image->readImageFromCurrentTexture(0,true);

                texture->setInternalFormatMode(osg::Texture::USE_IMAGE_DATA_FORMAT);

                image->setFileName(osgDB::getNameLessExtension(image->getFileName()) + ".dds");
            }
        }
    }

    void write(const std::string &dir)
    {
        for(TextureSet::iterator itr=_textureSet.begin();
            itr!=_textureSet.end();
            ++itr)
        {
            osg::Texture* texture = const_cast<osg::Texture*>(itr->get());

            osg::Texture2D* texture2D = dynamic_cast<osg::Texture2D*>(texture);
            osg::Texture3D* texture3D = dynamic_cast<osg::Texture3D*>(texture);

            osg::ref_ptr<osg::Image> image = texture2D ? texture2D->getImage() : (texture3D ? texture3D->getImage() : 0);
            if (image.valid())
            {
                std::string name = osgDB::getStrippedName(image->getFileName());
                name += ".dds";
                image->setFileName(name);
                std::string path = dir.empty() ? name : osgDB::concatPaths(dir, name);
                osgDB::writeImageFile(*image, path);
                osg::notify(osg::NOTICE) << "Image written to '" << path << "'." << std::endl;
            }
        }
    }

    typedef std::set< osg::ref_ptr<osg::Texture> > TextureSet;
    TextureSet                          _textureSet;
    osg::Texture::InternalFormatMode    _internalFormatMode;

};

namespace hbx {

class CompressTexturesVisitorAction : public hbx::VisitorAction
{
public:

    enum Mode
    {
        USE_ARB_COMPRESSION = 0,
        USE_S3TC_DXT1_COMPRESSION,
        USE_S3TC_DXT3_COMPRESSION,
        USE_S3TC_DXT5_COMPRESSION
    };

    CompressTexturesVisitorAction()
        : hbx::VisitorAction(),
          _mode(USE_ARB_COMPRESSION)
    {}

    CompressTexturesVisitorAction(const CompressTexturesVisitorAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : hbx::VisitorAction(op, copyop),
       _mode(op._mode)
    {
    }

    META_Object(hbx,CompressTexturesVisitorAction)

    virtual std::string friendlyName(){ return "Compress Model Textures"; }
    virtual std::string description() { return "Compress a models textures to ARB or DXT format"; }
    virtual std::string category(){ return "Model"; }

    virtual osg::NodeVisitor* allocateVisitor(){ return new CompressTexturesVisitor(getTextureMode(_mode)); }

    virtual void postProcess(ActionData* aData, osg::NodeVisitor* aVisitor)
    {
        aData->_context->makeCurrent();
        CompressTexturesVisitor* compressor = dynamic_cast<CompressTexturesVisitor*>(aVisitor);
        compressor->compress();
    }

    //must be a better way of doing the enums
    osg::Texture::InternalFormatMode getTextureMode(Mode aMode){
        switch(aMode){
            case USE_ARB_COMPRESSION:
                return osg::Texture::USE_ARB_COMPRESSION;
            case USE_S3TC_DXT1_COMPRESSION:
                return osg::Texture::USE_S3TC_DXT1_COMPRESSION;
            case USE_S3TC_DXT3_COMPRESSION:
                return osg::Texture::USE_S3TC_DXT3_COMPRESSION;
            case USE_S3TC_DXT5_COMPRESSION:
                return osg::Texture::USE_S3TC_DXT5_COMPRESSION;
            default:
                return osg::Texture::USE_ARB_COMPRESSION;
        }
        return osg::Texture::USE_ARB_COMPRESSION;
    }

    //
    // serializable properties
    //
    inline void setMode(Mode aMode) { _mode = aMode; }
    inline Mode getMode() const { return _mode; }

protected:
    Mode _mode;
};

} // end hbx

REGISTER_OBJECT_WRAPPER( hbx_CompressTexturesVisitorAction,
                         new hbx::CompressTexturesVisitorAction,
                         hbx::CompressTexturesVisitorAction,
                         "osg::Object hbx::Action hbx::CompressTexturesVisitorAction" )
{
    BEGIN_ENUM_SERIALIZER( Mode, USE_ARB_COMPRESSION );
        ADD_ENUM_VALUE( USE_ARB_COMPRESSION );
        ADD_ENUM_VALUE( USE_S3TC_DXT1_COMPRESSION );
        ADD_ENUM_VALUE( USE_S3TC_DXT3_COMPRESSION );
        ADD_ENUM_VALUE( USE_S3TC_DXT5_COMPRESSION );
    END_ENUM_SERIALIZER();  // _mode
}
