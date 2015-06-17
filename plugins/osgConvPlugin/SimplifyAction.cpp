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
#include <osgUtil/Simplifier>

namespace hbx {

class SimpilfyAction : public hbx::Action
{
public:
    SimpilfyAction()
        : hbx::Action(),
        _smooth(false),
        _simplify(1.0f){
    }

    SimpilfyAction(const SimpilfyAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : hbx::Action(op, copyop),
       _smooth(op._smooth),
       _simplify(op._simplify)
    {
    }

    META_Object(hbx,SimpilfyAction)

    virtual std::string friendlyName() {return "Simplify";}
    virtual std::string description() {return "Run the osgUtil Simplifier on the object";}
    virtual std::string category(){ return "Model"; }

    //
    // serializable properties
    //
    inline void setSmooth(bool smooth) { _smooth = smooth; }
    inline bool getSmooth() const { return _smooth; }

    inline void setSimplify(float simplify) { _simplify = simplify; }
    inline float getSimplify() const { return _simplify; }

    virtual void process(ActionData* aData) {
        if(aData->asNode() == NULL) return;
        osgUtil::Simplifier simple;
        simple.setSmoothing(getSmooth());
        osg::notify( osg::ALWAYS ) << " smoothing: " << getSmooth() << std::endl;
        simple.setSampleRatio( getSimplify() );
        aData->asNode()->accept( simple );
    }

protected:
    bool _smooth;
    float _simplify;

};

} //end hbx

REGISTER_OBJECT_WRAPPER( hbx_SimpilfyAction,
                         new hbx::SimpilfyAction,
                         hbx::SimpilfyAction,
                         "osg::Object hbx::Action hbx::SimpilfyAction" )
{
    ADD_BOOL_SERIALIZER( Smooth, false );  // _smooth
    ADD_FLOAT_SERIALIZER( Simplify, 1.0f );  // _simplify
}
