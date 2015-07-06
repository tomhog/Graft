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

/** Add missing colours to osg::Geometry.*/
class AddMissingColoursToGeometryVisitor : public osg::NodeVisitor
{
public:

    AddMissingColoursToGeometryVisitor(osg::Vec4 aColor)
        :osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
        _color(aColor)
    {
    }

    virtual void apply(osg::Geode& geode)
    {
        for(unsigned int i=0;i<geode.getNumDrawables();++i)
        {
            osg::Geometry* geometry = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
            if (geometry)
            {
                if (geometry->getColorArray()==0 || geometry->getColorArray()->getNumElements()==0)
                {
                    osg::Vec4Array* colors = new osg::Vec4Array(1);
                    (*colors)[0] = _color;
                    geometry->setColorArray(colors, osg::Array::BIND_OVERALL);
                }
            }
        }
    }

    virtual void apply(osg::Node& node) { traverse(node); }
protected:
    osg::Vec4 _color;
};

namespace graft {

class AddMissingColoursToGeometryVisitorAction : public graft::VisitorAction
{
public:
    AddMissingColoursToGeometryVisitorAction()
        : graft::VisitorAction(),
          _color(osg::Vec4(1.0f,1.0f,1.0f,1.0f))
    {}

    AddMissingColoursToGeometryVisitorAction(const AddMissingColoursToGeometryVisitorAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : graft::VisitorAction(op, copyop),
       _color(op._color)
    {
    }

    META_Object(graft,AddMissingColoursToGeometryVisitorAction)

    virtual std::string friendlyName(){ return "Add missing colours"; }
    virtual std::string description() { return "Adds a colour array to an geometry that doesn't already have one"; }
    virtual std::string category(){ return "Model"; }

    virtual osg::NodeVisitor* allocateVisitor(){ return new AddMissingColoursToGeometryVisitor(_color); }

    //
    // serializable properties
    //
    inline void setColor(const osg::Vec4& color) { _color = color; }
    inline const osg::Vec4& getColor() const { return _color; }

protected:
    osg::Vec4 _color;
};

} // end graft

REGISTER_OBJECT_WRAPPER( graft_AddMissingColoursToGeometryVisitorAction,
                         new graft::AddMissingColoursToGeometryVisitorAction,
                         graft::AddMissingColoursToGeometryVisitorAction,
                         "osg::Object graft::Action graft::AddMissingColoursToGeometryVisitorAction" )
{
    ADD_VEC4_SERIALIZER( Color, osg::Vec4() );  // _color
}
