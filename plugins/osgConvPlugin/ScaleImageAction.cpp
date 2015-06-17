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

class ScaleImageAction : public hbx::Action
{
public:
    ScaleImageAction()
        : hbx::Action(),
        _size(512)
    {}

    ScaleImageAction(const ScaleImageAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : hbx::Action(op, copyop),
       _size(op._size)
    {
    }

    META_Object(hbx,ScaleImageAction)

    virtual std::string friendlyName(){ return "Scale image"; }
    virtual std::string description(){ return "Resize images largest dimension to size, maintaining aspect ratio";}
    virtual std::string category(){ return "Image"; }

    virtual void process(ActionData* aData) {
        if(aData->asImage() == NULL) return;
        osg::Image* img = aData->asImage();

        if(img->s() >= img->t()){
            float aspect = (float)img->t() / (float)img->s();
            int newHeight = (float)_size * aspect;
            img->scaleImage(_size, newHeight, 1);
        } else {
            float aspect = (float)img->s() / (float)img->t();
            int newWidth = _size * aspect;
            img->scaleImage(newWidth, _size, 1);
        }
    }

    void setSize(unsigned int aSize){ _size = aSize; }
    unsigned int getSize() const { return _size; }

protected:
    unsigned int _size;
};

} //end hbx

REGISTER_OBJECT_WRAPPER( hbx_ScaleImageAction,
                         new hbx::ScaleImageAction,
                         hbx::ScaleImageAction,
                         "osg::Object hbx::Action hbx::ScaleImageAction" )
{
    ADD_UINT_SERIALIZER(Size, 512);
}
