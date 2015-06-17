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

#ifndef HBX_CHANGEFILENAMEACTION_H
#define HBX_CHANGEFILENAMEACTION_H 1

#include <hbx/Action.h>
#include <osgDB/WriteFile>
#include <osgDB/FileNameUtils>

namespace hbx {

//
// Write the node or image to file
//
class HBX_EXPORT ChangeFileNameAction : public Action
{
public:

    enum Mode {
        APPEND_STRING = 0,
        PREPEND_STRING
    };

    ChangeFileNameAction()
        : Action(),
          _mode(APPEND_STRING),
          _string("")
    {}

    ChangeFileNameAction(const ChangeFileNameAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : Action(op, copyop),
       _mode(op._mode),
       _string(op._string)
    {
    }
    META_Object(hbx,ChangeFileNameAction)

    virtual std::string friendlyName(){ return "Change File Name"; }
    virtual std::string description(){ return "Adjust the files name, prepend or append a string to the name (does not effect path or extension)";}
    virtual std::string category(){ return "File"; }

    inline void setString(const std::string& aString) { _string = aString; }
    inline const std::string& getString() const { return _string; }

    inline void setMode(Mode aMode) { _mode = aMode; }
    inline Mode getMode() const { return _mode; }

    virtual void process(ActionData* aData) {
        switch(_mode)
        {
            case APPEND_STRING:
                aData->_filePath = osgDB::getNameLessAllExtensions(aData->_filePath) + _string + osgDB::getFileExtensionIncludingDot(aData->_filePath);
            break;
            case PREPEND_STRING:
                aData->_filePath = osgDB::getFilePath(aData->_filePath) + _string + osgDB::getSimpleFileName(aData->_filePath);
            break;
        }
    }

protected:
    std::string _string;
    Mode _mode;
};

} // end hbx

#endif // HBX_CHANGEFILENAMEACTION_H
