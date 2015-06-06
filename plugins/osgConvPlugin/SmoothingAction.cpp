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
#include <osgUtil/SmoothingVisitor>

namespace hbx {

class SmoothingVisitorAction : public hbx::VisitorAction
{
public:
    SmoothingVisitorAction()
        : hbx::VisitorAction(),
          _creaseAngle(180.0f)
    {}

    SmoothingVisitorAction(const SmoothingVisitorAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : hbx::VisitorAction(op, copyop),
       _creaseAngle(op._creaseAngle)
    {
    }

    META_Object(hbx,SmoothingVisitorAction)

    virtual std::string friendlyName() { return "Smooth Normals"; }
    virtual std::string description() { return "Smooth the normals of any geometries"; }

    virtual osg::NodeVisitor* allocateVisitor()
    {
        osgUtil::SmoothingVisitor* smoother = new osgUtil::SmoothingVisitor();
        smoother->setCreaseAngle(osg::DegreesToRadians((double)_creaseAngle));
        return smoother;
    }

    //
    // serializable properties
    //
    inline void setCreaseAngle(float anAngle) { _creaseAngle = anAngle; }
    inline float getCreaseAngle() const { return _creaseAngle; }

protected:
    float _creaseAngle;
};

} //end hbx

REGISTER_OBJECT_WRAPPER( hbx_SmoothingVisitorAction,
                         new hbx::SmoothingVisitorAction,
                         hbx::SmoothingVisitorAction,
                         "osg::Object hbx::Action hbx::SmoothingVisitorAction" )
{
    ADD_FLOAT_SERIALIZER(CreaseAngle, 180.0f);
}
