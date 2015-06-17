/*  -*-c++-*-
 *  Copyright (C) 2015 Thomas Hogarth <tom@hogbox.com>
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

#ifndef HBX_ACTIONS_H
#define HBX_ACTIONS_H 1

#include <hbx/Export.h>

#include <osg/Object>
#include <osg/ValueObject>
#include <osg/NodeVisitor>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

namespace hbx {

class HBX_EXPORT ActionData : public osg::Referenced
{
public:
    ActionData() : osg::Referenced() {}

    std::string _filePath;
    osg::ref_ptr<osg::Object> _object;

    osg::Node* asNode(){ return dynamic_cast<osg::Node*>(_object.get()); }
    osg::Image* asImage(){ return dynamic_cast<osg::Image*>(_object.get()); }

    osg::ref_ptr<osg::GraphicsContext> _context;

protected:
    virtual ~ActionData(void){
    }
};

//
// Base class for operations we can perform on loaded inputs (nodes and images)
//
class HBX_EXPORT Action : public osg::Object
{
public:
    Action()
     : osg::Object(),
        _enabled(true)
    {}

    Action(const Action& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : osg::Object(op, copyop),
       _enabled(op._enabled)
    {
    }

    virtual osg::Object* cloneType() const = 0;
    virtual osg::Object* clone(const osg::CopyOp&) const = 0;
    virtual bool isSameKindAs(const osg::Object* obj) const { return dynamic_cast<const Action*>(obj)!=NULL; }
    virtual const char* libraryName() const { return "hbx"; }
    virtual const char* className() const { return "Action"; }

    bool getEnabled() const { return _enabled; }
    void setEnabled(bool isEnabled){ _enabled = isEnabled; }

    // provide a readable name for the operation to display in UIs
    virtual std::string friendlyName() = 0;

    // provide a readable description of what the operation does
    virtual std::string description() = 0;

    virtual std::string category() = 0;

    // apply this operation to the passed data
    virtual void process(ActionData* aData) = 0;

protected:
    bool _enabled;
};
typedef std::vector< osg::ref_ptr<Action> > ActionList;

//
// Base input operation
//
class HBX_EXPORT InputAction : public Action
{
public:
    InputAction() : Action() {}

    InputAction(const InputAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : Action(op, copyop)
    {
    }

    virtual osg::Object* cloneType() const = 0;
    virtual osg::Object* clone(const osg::CopyOp&) const = 0;
    virtual bool isSameKindAs(const osg::Object* obj) const { return dynamic_cast<const InputAction*>(obj)!=NULL; }
    virtual const char* libraryName() const { return "hbx"; }
    virtual const char* className() const { return "InputAction"; }
};


//
// Base output operation
//
class HBX_EXPORT OutputAction : public Action
{
public:
    OutputAction() : Action() {}

    OutputAction(const OutputAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : Action(op, copyop)
    {
    }

    virtual osg::Object* cloneType() const = 0;
    virtual osg::Object* clone(const osg::CopyOp&) const = 0;
    virtual bool isSameKindAs(const osg::Object* obj) const { return dynamic_cast<const OutputAction*>(obj)!=NULL; }
    virtual const char* libraryName() const { return "hbx"; }
    virtual const char* className() const { return "OutputAction"; }
};


//
// Wrapper for operations that just apply a simple visitor
//
class HBX_EXPORT VisitorAction : public Action
{
public:
    VisitorAction() : Action() {}

    VisitorAction(const VisitorAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : Action(op, copyop)
    {
    }

    virtual osg::Object* cloneType() const = 0;
    virtual osg::Object* clone(const osg::CopyOp&) const = 0;
    virtual bool isSameKindAs(const osg::Object* obj) const { return dynamic_cast<const VisitorAction*>(obj)!=NULL; }
    virtual const char* libraryName() const { return "hbx"; }
    virtual const char* className() const { return "VisitorAction"; }

    virtual osg::NodeVisitor* allocateVisitor() = 0;

    virtual void postProcess(ActionData* aData, osg::NodeVisitor* aVisitor){}

    virtual void process(ActionData* aData) {
        if(aData->asNode() == NULL) return;
        osg::ref_ptr<osg::NodeVisitor> visitor = allocateVisitor();
        aData->asNode()->accept(*visitor);
        postProcess(aData, visitor);
    }
};

} //end hbx namespace

#endif // HBX_ACTIONS_H
