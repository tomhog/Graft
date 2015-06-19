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

#ifndef HBX_CHANGEDIRECTORYACTION_H
#define HBX_CHANGEDIRECTORYACTION_H 1

#include <hbx/Action.h>
#include <osgDB/WriteFile>
#include <osgDB/FileNameUtils>

namespace hbx {

//
// Write the node or image to file
//
class HBX_EXPORT ChangeDirectoryAction : public Action
{
public:

    ChangeDirectoryAction()
        : Action(),
          _directory("")
    {}

    ChangeDirectoryAction(const ChangeDirectoryAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : Action(op, copyop),
       _directory(op._directory)
    {
    }
    META_Object(hbx,ChangeDirectoryAction)

    virtual std::string friendlyName(){ return "Change Directory"; }
    virtual std::string description(){ return "Change the directory of input files";}
    virtual std::string category(){ return "File"; }

    inline void setDirectory(const std::string& aDirectory) { _directory = aDirectory; }
    inline const std::string& getDirectory() const { return _directory; }

    virtual void process(ActionData* aData) {
        std::string filename = osgDB::getSimpleFileName(aData->_filePath);
        aData->_filePath = _directory + "/" + filename;
    }

protected:
    std::string _directory;
};

} // end hbx

#endif // HBX_CHANGEDIRECTORYACTION_H
