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

class PruneStateSetVisitor : public osg::NodeVisitor
{
public:

    PruneStateSetVisitor():
        osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
        _numStateSetRemoved(0)
    {
        std::cout<<"Running PruneStateSet..."<<std::endl;
    }

    ~PruneStateSetVisitor()
    {
        std::cout<<"  Number of StateState removed "<<_numStateSetRemoved<<std::endl;
    }

    virtual void apply(osg::Node& node)
    {
        if (node.getStateSet())
        {
            node.setStateSet(0);
            ++_numStateSetRemoved;
        }
        traverse(node);
    }

    virtual void apply(osg::Geode& node)
    {
        if (node.getStateSet())
        {
            node.setStateSet(0);
            ++_numStateSetRemoved;
        }

        traverse(node);
    }

    unsigned int _numStateSetRemoved;
};

namespace graft {

class PruneStateSetsVisitorAction : public graft::VisitorAction
{
public:
    PruneStateSetsVisitorAction()
        : graft::VisitorAction()
    {}

    PruneStateSetsVisitorAction(const PruneStateSetsVisitorAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : graft::VisitorAction(op, copyop)
    {
    }

    META_Object(graft,PruneStateSetsVisitorAction)

    virtual std::string friendlyName(){ return "Remove StateSets"; }
    virtual std::string description() { return "Clears all state sets found in the graph"; }
    virtual std::string category(){ return "StateSet"; }

    virtual osg::NodeVisitor* allocateVisitor(){ return new PruneStateSetVisitor(); }

protected:

};

} // end graft

REGISTER_OBJECT_WRAPPER( graft_PruneStateSetsVisitorAction,
                         new graft::PruneStateSetsVisitorAction,
                         graft::PruneStateSetsVisitorAction,
                         "osg::Object graft::Action graft::PruneStateSetsVisitorAction" )
{
}
