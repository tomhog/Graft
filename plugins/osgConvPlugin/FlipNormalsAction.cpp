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
#include <osgUtil/TangentSpaceGenerator>

class FlipNormalsVisitor
    : public osg::NodeVisitor
{
public:

    FlipNormalsVisitor()
        : osg::NodeVisitor( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN )
    {
    }

    virtual void apply( osg::Geode & geode )
    {
        for( unsigned int ii = 0; ii < geode.getNumDrawables(); ++ii )
        {
            osg::ref_ptr< osg::Geometry > geometry = dynamic_cast< osg::Geometry * >( geode.getDrawable( ii ) );
            if( geometry.valid() )
            {
                const osg::Vec3Array* normals = (const osg::Vec3Array*)geometry->getNormalArray();
                osg::ref_ptr<osg::Vec3Array> flipped = new osg::Vec3Array();
                for(unsigned int i=0; i<normals->size(); i++) {
                    flipped->push_back(-normals->at(i));
                }

                geometry->setNormalArray(flipped, normals->getBinding());
            }
        }
    }

    virtual void apply( osg::Node & node )
    {
        traverse( node );
    }

protected:

};

namespace graft {

class FlipNormalsVisitorAction : public graft::VisitorAction
{
public:
    FlipNormalsVisitorAction()
        : graft::VisitorAction()
    {}

    FlipNormalsVisitorAction(const FlipNormalsVisitorAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : graft::VisitorAction(op, copyop)
    {
    }

    META_Object(graft,FlipNormalsVisitorAction)

    virtual std::string friendlyName(){ return "Flip Normals"; }
    virtual std::string description() { return "Flip the normals of all geometries"; }
    virtual std::string category(){ return "Model"; }

    virtual osg::NodeVisitor* allocateVisitor(){ return new FlipNormalsVisitor(); }

protected:
};

} // end graft

REGISTER_OBJECT_WRAPPER( graft_FlipNormalsVisitorAction,
                         new graft::FlipNormalsVisitorAction,
                         graft::FlipNormalsVisitorAction,
                         "osg::Object graft::Action graft::FlipNormalsVisitorAction" )
{
}
