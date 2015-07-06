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

namespace graft {

class ClampImageSizeAction : public graft::Action
{
public:
    ClampImageSizeAction()
        : graft::Action(),
        _maxWidth(512),
        _maxHeight(512)
    {}

    ClampImageSizeAction(const ClampImageSizeAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : graft::Action(op, copyop),
       _maxWidth(op._maxWidth),
       _maxHeight(op._maxHeight)
    {
    }

    META_Object(graft,ClampImageSizeAction)

    virtual std::string friendlyName(){ return "Clamp Image Size"; }
    virtual std::string description(){ return "Clamp Image dimensions to MaxWidth MaxHeight";}
    virtual std::string category(){ return "Image"; }

    virtual void process(ActionData* aData) {
        if(aData->asImage() == NULL) return;
        osg::Image* img = aData->asImage();

        unsigned int clampedWidth = osg::clampTo<unsigned int>(img->s(), 1, _maxWidth);
        unsigned int clampedHeight = osg::clampTo<unsigned int>(img->t(), 1, _maxHeight);

        if(clampedWidth != img->s() || clampedHeight != img->t())
            img->scaleImage(clampedWidth, clampedHeight, 1);
    }

    void setMaxWidth(unsigned int aMaxWidth){ _maxWidth = aMaxWidth; }
    unsigned int getMaxWidth() const { return _maxWidth; }

    void setMaxHeight(unsigned int aMaxHeight){ _maxHeight = aMaxHeight; }
    unsigned int getMaxHeight() const { return _maxHeight; }

protected:
    unsigned int _maxWidth;
    unsigned int _maxHeight;
};

} //end graft

REGISTER_OBJECT_WRAPPER( graft_ClampImageSizeAction,
                         new graft::ClampImageSizeAction,
                         graft::ClampImageSizeAction,
                         "osg::Object graft::Action graft::ClampImageSizeAction" )
{
    ADD_UINT_SERIALIZER(MaxWidth, 512);
    ADD_UINT_SERIALIZER(MaxHeight, 512);
}
