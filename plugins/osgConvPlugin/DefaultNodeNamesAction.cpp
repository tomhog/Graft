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

class DefaultNodeNamesVisitor : public osg::NodeVisitor
{
public:
    DefaultNodeNamesVisitor():
        osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
    {}

    virtual void apply(osg::Node& node)
    {
        if(_labels.find(node.className()) == _labels.end())
            _labels[node.className()] = 1;
        else
            _labels[node.className()]++;

        std::stringstream ss;
        ss << node.className() << _labels[node.className()];
        node.setName(ss.str().c_str());

        traverse(node);

    }
    std::map<std::string, int> _labels;
};

class DefaultNodeNamesAction : public graft::VisitorAction
{
public:

    DefaultNodeNamesAction()
        : graft::VisitorAction()
    {}

    DefaultNodeNamesAction(const DefaultNodeNamesAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : graft::VisitorAction(op, copyop)
    {
    }

    META_Object(graft,DefaultNodeNamesAction)

    virtual std::string friendlyName(){ return "Default Node Names"; }
    virtual std::string description(){ return "Give nodes without names a default name group01, transform03 etc";}
    virtual std::string category(){ return "Model"; }

    virtual osg::NodeVisitor* allocateVisitor(){ return new DefaultNodeNamesVisitor(); }

protected:

};

} //end graft

REGISTER_OBJECT_WRAPPER( graft_DefaultNodeNamesAction,
                         new graft::DefaultNodeNamesAction,
                         graft::DefaultNodeNamesAction,
                         "osg::Object graft::Action graft::DefaultNodeNamesAction" )
{
}
