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

#include <Graft/PluginRegistry.h>
#include <osg/NodeVisitor>
#include <osg/BlendFunc>

class FixTransparencyVisitor : public osg::NodeVisitor
{
public:

    enum FixTransparencyMode
    {
        NO_TRANSPARANCY_FIXING = 0,
        MAKE_OPAQUE_TEXTURE_STATESET_OPAQUE,
        MAKE_ALL_STATESET_OPAQUE
    };

    FixTransparencyVisitor(FixTransparencyMode mode=MAKE_OPAQUE_TEXTURE_STATESET_OPAQUE):
        osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
        _numTransparent(0),
        _numOpaque(0),
        _numTransparentMadeOpaque(0),
        _mode(mode)
    {}

    ~FixTransparencyVisitor()
    {}

    virtual void apply(osg::Node& node)
    {
        if (node.getStateSet()) isTransparent(*node.getStateSet());
        traverse(node);
    }

    virtual void apply(osg::Geode& node)
    {
        if (node.getStateSet()) isTransparent(*node.getStateSet());

        for(unsigned int i=0;i<node.getNumDrawables();++i)
        {
            osg::Drawable* drawable = node.getDrawable(i);
            if (drawable && drawable->getStateSet()) isTransparent(*drawable->getStateSet());
        }

        traverse(node);
    }

    virtual bool isTransparent(osg::StateSet& stateset)
    {
        bool hasTranslucentTexture = false;
        bool hasBlendFunc = dynamic_cast<osg::BlendFunc*>(stateset.getAttribute(osg::StateAttribute::BLENDFUNC))!=0;
        bool hasTransparentRenderingHint = stateset.getRenderingHint()==osg::StateSet::TRANSPARENT_BIN;
        bool hasDepthSortBin = (stateset.getRenderBinMode()==osg::StateSet::USE_RENDERBIN_DETAILS)?(stateset.getBinName()=="DepthSortedBin"):false;
        bool hasTexture = false;


        // search for the existence of any texture object attributes
        for(unsigned int i=0;i<stateset.getTextureAttributeList().size();++i)
        {
            osg::Texture* texture = dynamic_cast<osg::Texture*>(stateset.getTextureAttribute(i,osg::StateAttribute::TEXTURE));
            if (texture)
            {
                hasTexture = true;
                for (unsigned int im=0;im<texture->getNumImages();++im)
                {
                    osg::Image* image = texture->getImage(im);
                    if (image && image->isImageTranslucent()) hasTranslucentTexture = true;
                }
            }
        }

        if (hasTranslucentTexture || hasBlendFunc || hasTransparentRenderingHint || hasDepthSortBin)
        {
            ++_numTransparent;

            bool makeNonTransparent = false;

            switch(_mode)
            {
            case(MAKE_OPAQUE_TEXTURE_STATESET_OPAQUE):
                if (hasTexture && !hasTranslucentTexture)
                {
                    makeNonTransparent = true;
                }
                break;
            case(MAKE_ALL_STATESET_OPAQUE):
                makeNonTransparent = true;
                break;
            default:
                makeNonTransparent = false;
                break;
            }

            if (makeNonTransparent)
            {
                stateset.removeAttribute(osg::StateAttribute::BLENDFUNC);
                stateset.removeMode(GL_BLEND);
                stateset.setRenderingHint(osg::StateSet::DEFAULT_BIN);
                ++_numTransparentMadeOpaque;
            }


            return true;
        }
        else
        {
            ++_numOpaque;
            return false;
        }
    }

    unsigned int _numTransparent;
    unsigned int _numOpaque;
    unsigned int _numTransparentMadeOpaque;
    FixTransparencyMode _mode;
};

namespace graft {

class FixTransparencyVisitorAction : public graft::VisitorAction
{
public:
    enum Mode
    {
        NO_TRANSPARANCY_FIXING = 0,
        MAKE_OPAQUE_TEXTURE_STATESET_OPAQUE,
        MAKE_ALL_STATESET_OPAQUE
    };

    FixTransparencyVisitorAction()
        : graft::VisitorAction(),
         _mode(MAKE_OPAQUE_TEXTURE_STATESET_OPAQUE)
    {}

    FixTransparencyVisitorAction(const FixTransparencyVisitorAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : graft::VisitorAction(op, copyop),
       _mode(op._mode)
    {
    }

    META_Object(graft,FixTransparencyVisitorAction)

    virtual std::string friendlyName(){ return "Fix Transparency"; }
    virtual std::string description() { return "Sets blending modes and rendering hints to fix transparency issues"; }
    virtual std::string category(){ return "StateSet"; }

    virtual osg::NodeVisitor* allocateVisitor(){ return new FixTransparencyVisitor((FixTransparencyVisitor::FixTransparencyMode)_mode); }

    //
    // serializable properties
    //
    inline void setMode(Mode aMode) { _mode = aMode; }
    inline Mode getMode() const { return _mode; }

protected:
    Mode _mode;
};

} // end graft

REGISTER_OBJECT_WRAPPER( graft_FixTransparencyVisitorAction,
                         new graft::FixTransparencyVisitorAction,
                         graft::FixTransparencyVisitorAction,
                         "osg::Object graft::Action graft::FixTransparencyVisitorAction" )
{
    BEGIN_ENUM_SERIALIZER( Mode, MAKE_OPAQUE_TEXTURE_STATESET_OPAQUE );
        ADD_ENUM_VALUE( NO_TRANSPARANCY_FIXING );
        ADD_ENUM_VALUE( MAKE_OPAQUE_TEXTURE_STATESET_OPAQUE );
        ADD_ENUM_VALUE( MAKE_ALL_STATESET_OPAQUE );
    END_ENUM_SERIALIZER();  // _mode
}
