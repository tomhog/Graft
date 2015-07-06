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

class FlipImageAction : public graft::Action
{
public:

    enum Mode
    {
        HORIZONTAL = 0,
        VERTICAL
    };

    FlipImageAction()
        : graft::Action(),
          _mode(HORIZONTAL)
    {}

    FlipImageAction(const FlipImageAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : graft::Action(op, copyop),
       _mode(op._mode)
    {
    }

    META_Object(graft,FlipImageAction)

    virtual std::string friendlyName(){ return "Flip Image"; }
    virtual std::string description(){ return "Flip image horizontally or vertically";}
    virtual std::string category(){ return "Image"; }

    virtual void process(ActionData* aData) {
        if(aData->asImage() == NULL) return;
        osg::Image* img = aData->asImage();

        if(_mode == HORIZONTAL)
            img->flipHorizontal();
        else if(_mode == VERTICAL)
            img->flipVertical();
    }

    //
    // serializable properties
    //
    inline void setMode(Mode aMode) { _mode = aMode; }
    inline Mode getMode() const { return _mode; }

protected:

    Mode _mode;

};

} //end graft

REGISTER_OBJECT_WRAPPER( graft_FlipImageAction,
                         new graft::FlipImageAction,
                         graft::FlipImageAction,
                         "osg::Object graft::Action graft::FlipImageAction" )
{
    BEGIN_ENUM_SERIALIZER( Mode, HORIZONTAL );
        ADD_ENUM_VALUE( HORIZONTAL );
        ADD_ENUM_VALUE( VERTICAL );
    END_ENUM_SERIALIZER();  // _mode
}
