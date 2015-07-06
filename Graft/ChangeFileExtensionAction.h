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

#ifndef GRAFT_CHANGEFILEEXTENSIONACTION_H
#define GRAFT_CHANGEFILEEXTENSIONACTION_H 1

#include <Graft/Action.h>
#include <osgDB/WriteFile>
#include <osgDB/FileNameUtils>

namespace graft {

//
// Write the node or image to file
//
class GRAFT_EXPORT ChangeFileExtensionAction : public Action
{
public:

    enum Type {
        NODE = 0,
        IMAGE
    };

    ChangeFileExtensionAction()
        : Action(),
          _type(NODE),
          _extension("")
    {}

    ChangeFileExtensionAction(const ChangeFileExtensionAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : Action(op, copyop),
       _type(op._type),
       _extension(op._extension)
    {
    }
    META_Object(graft,ChangeFileExtensionAction)

    virtual std::string friendlyName(){ return "Change File Extension"; }
    virtual std::string description(){ return "Change all files of type (node or image) to extension";}
    virtual std::string category(){ return "File"; }

    inline void setExtension(const std::string& aString) { _extension = aString; }
    inline const std::string& getExtension() const { return _extension; }

    inline void setType(Type aType) { _type = aType; }
    inline Type getType() const { return _type; }

    virtual void process(ActionData* aData) {
        if(_extension.empty()) return;
        if(_type == NODE && aData->asNode() != NULL)
            aData->_filePath = osgDB::getNameLessAllExtensions(aData->_filePath) + "." + _extension;
    }

protected:
    std::string _extension;
    Type _type;
};

} // end graft

#endif // GRAFT_CHANGEFILEEXTENSIONACTION_H
