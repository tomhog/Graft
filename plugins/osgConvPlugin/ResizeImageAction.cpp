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

namespace hbx {

class ResizeImageAction : public hbx::Action
{
public:
    ResizeImageAction()
        : hbx::Action(),
        _width(512),
        _height(512)
    {}

    ResizeImageAction(const ResizeImageAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : hbx::Action(op, copyop),
       _width(op._width),
       _height(op._height)
    {
    }

    META_Object(hbx,ResizeImageAction)

    virtual std::string friendlyName(){ return "Resize image"; }
    virtual std::string description(){ return "Resize an image to exact size";}
    virtual std::string category(){ return "Image"; }

    virtual void process(ActionData* aData) {
        if(aData->asImage() == NULL) return;
        osg::Image* img = aData->asImage();

        img->scaleImage(_width, _height, 1);
    }

    void setWidth(unsigned int aWidth){ _width = aWidth; }
    unsigned int getWidth() const { return _width; }

    void setHeight(unsigned int aHeight){ _height = aHeight; }
    unsigned int getHeight() const { return _height; }

protected:
    unsigned int _width;
    unsigned int _height;
};

} //end hbx

REGISTER_OBJECT_WRAPPER( hbx_ResizeImageAction,
                         new hbx::ResizeImageAction,
                         hbx::ResizeImageAction,
                         "osg::Object hbx::Action hbx::ResizeImageAction" )
{
    ADD_UINT_SERIALIZER(Width, 512);
    ADD_UINT_SERIALIZER(Height, 512);
}
