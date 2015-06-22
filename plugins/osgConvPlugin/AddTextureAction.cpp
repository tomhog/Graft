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
#include <hbx/NamedNodesAction.h>

#include <osg/Texture2D>
#include <osgDB/ReadFile>


namespace hbx {

class AddTextureAction : public hbx::NamedNodesAction
{
public:

    AddTextureAction()
        : hbx::NamedNodesAction(),
        _channel(0),
        _imagePath("")
    {}

    AddTextureAction(const AddTextureAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : hbx::NamedNodesAction(op, copyop),
       _channel(op._channel),
       _imagePath(op._imagePath)
    {
    }

    META_Object(hbx,AddTextureAction)

    virtual std::string friendlyName(){ return "Add Texture"; }
    virtual std::string description(){ return "Load an image and add as texture to specified channel for all nodes matching search";}
    virtual std::string category(){ return "StateSet"; }

    virtual void processNamedNode(ActionData* aData, osg::Node* aNode)
    {
        if(aNode == NULL || !_texture.valid())
            return;

        aNode->getOrCreateStateSet()->setTextureAttribute(_channel, _texture, osg::StateAttribute::ON);
    }

    // overload process to load the image
    virtual void process(ActionData* aData)
    {
        osg::Image* img = osgDB::readImageFile(_imagePath);
        if(img != NULL) {
            _texture = new osg::Texture2D();
            _texture->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR_MIPMAP_LINEAR);
            _texture->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR_MIPMAP_LINEAR);
            _texture->setResizeNonPowerOfTwoHint(false);
            _texture->setImage(img);
        }
        NamedNodesAction::process(aData);
    }



    //
    // Serializable
    //
    inline void setChannel(unsigned int aChannel) { _channel = aChannel; }
    inline unsigned int getChannel() const { return _channel; }

    inline void setImagePath(const std::string& aPath) { _imagePath = aPath; }
    inline const std::string& getImagePath() const { return _imagePath; }

protected:
    unsigned int _channel;
    std::string _imagePath;

    osg::ref_ptr<osg::Texture2D> _texture;
};

} //end hbx

REGISTER_OBJECT_WRAPPER( hbx_AddTextureAction,
                         new hbx::AddTextureAction,
                         hbx::AddTextureAction,
                         "osg::Object hbx::Action hbx::NamedNodesAction hbx::AddTextureAction" )
{
    ADD_UINT_SERIALIZER( Channel, 0);
    ADD_STRING_SERIALIZER( ImagePath, "" );  // _imagePath
}
