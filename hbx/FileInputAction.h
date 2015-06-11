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

#ifndef HBX_FILEINPUTACTION_H
#define HBX_FILEINPUTACTION_H 1

#include <hbx/Action.h>
#include <osgDB/ReadFile>

namespace hbx {

//
// Loads an asset using the provided filepath
// file can be a node or image
//
class HBX_EXPORT FileInputAction : public InputAction
{
public:
    FileInputAction() : InputAction() {}

    FileInputAction(const FileInputAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : InputAction(op, copyop)
    {
    }
    META_Object(hbx,FileInputAction)

    virtual std::string friendlyName(){ return "Open File"; }
    virtual std::string description(){ return "Open a file and set as task data";}

    inline void setFilePath(const std::string& aFilePath) { _filePath = aFilePath; }
    inline const std::string& getFilePath() const { return _filePath; }

    virtual void process(ActionData* aData) {
        aData->_filePath = _filePath;
        // fbx plugin doesn't seem to support readObjectFile calls so we'll just try read node
        // if it fails then try read image
        aData->_object = osgDB::readNodeFile(_filePath);
        if(!aData->_object.valid())
            aData->_object = osgDB::readImageFile(_filePath);
    }

protected:
    std::string _filePath;
};

} // end hbx

#endif // HBX_FILEINPUTACTION_H
