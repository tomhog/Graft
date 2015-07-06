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

class DefaultNormalsGeometryVisitor
    : public osg::NodeVisitor
{
public:

    DefaultNormalsGeometryVisitor(osg::Vec3 aNormal = osg::Z_AXIS)
        : osg::NodeVisitor( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN ),
        _normal(aNormal)
    {
    }

    virtual void apply( osg::Geode & geode )
    {
        for( unsigned int ii = 0; ii < geode.getNumDrawables(); ++ii )
        {
            osg::ref_ptr< osg::Geometry > geometry = dynamic_cast< osg::Geometry * >( geode.getDrawable( ii ) );
            if( geometry.valid() )
            {
                osg::ref_ptr< osg::Vec3Array > newnormals = new osg::Vec3Array;
                newnormals->push_back( _normal );
                geometry->setNormalArray( newnormals.get(), osg::Array::BIND_OVERALL );
            }
        }
    }

    virtual void apply( osg::Node & node )
    {
        traverse( node );
    }

protected:
    osg::Vec3 _normal;

};

namespace graft {

class DefaultNormalsVisitorAction : public graft::VisitorAction
{
public:
    DefaultNormalsVisitorAction()
        : graft::VisitorAction(),
          _normal(osg::Z_AXIS)
    {}

    DefaultNormalsVisitorAction(const DefaultNormalsVisitorAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : graft::VisitorAction(op, copyop),
       _normal(op._normal)
    {
    }

    META_Object(graft,DefaultNormalsVisitorAction)

    virtual std::string friendlyName(){ return "Add default normals"; }
    virtual std::string description() { return "Adds the same normal across all verticies"; }
    virtual std::string category(){ return "Model"; }

    virtual osg::NodeVisitor* allocateVisitor(){ return new DefaultNormalsGeometryVisitor(_normal); }

    //
    // serializable properties
    //
    inline void setNormal(const osg::Vec3& aNormal) { _normal = aNormal; }
    inline const osg::Vec3& getNormal() const { return _normal; }

protected:
    osg::Vec3 _normal;
};

} // end graft

REGISTER_OBJECT_WRAPPER( graft_DefaultNormalsVisitorAction,
                         new graft::DefaultNormalsVisitorAction,
                         graft::DefaultNormalsVisitorAction,
                         "osg::Object graft::Action graft::DefaultNormalsVisitorAction" )
{
    ADD_VEC3_SERIALIZER( Normal,  osg::Z_AXIS);  // _normal
}
