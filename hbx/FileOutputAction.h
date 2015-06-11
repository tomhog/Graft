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

#ifndef HBX_FILEOUTPUTACTION_H
#define HBX_FILEOUTPUTACTION_H 1

#include <hbx/Action.h>
#include <osgDB/WriteFile>
#include <osgDB/FileNameUtils>

namespace hbx {

//
// Write the node or image to file
//
class HBX_EXPORT FileOutputAction : public OutputAction
{
public:

    enum Mode {
        OVERWRITE = 0,
        APPEND,
        SET_EXTENSION
    };

    FileOutputAction()
        : OutputAction(),
          _mode(APPEND),
          _string("_conv")
    {}

    FileOutputAction(const FileOutputAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : OutputAction(op, copyop)
    {
    }
    META_Object(hbx,FileOutputAction)

    virtual std::string friendlyName(){ return "Save File"; }
    virtual std::string description(){ return "Write task data to file";}

    inline void setString(const std::string& aString) { _string = aString; }
    inline const std::string& getString() const { return _string; }

    inline void setMode(Mode aMode) { _mode = aMode; }
    inline Mode getMode() const { return _mode; }

    virtual void process(ActionData* aData)
    {
        if(aData->asNode() != NULL)
            osgDB::writeNodeFile(*aData->asNode(), aData->_filePath);
        else if(aData->asImage() != NULL)
            osgDB::writeImageFile(*aData->asImage(), aData->_filePath);
    }

protected:
    std::string _string;
    Mode _mode;
};

} // end hbx

#endif // HBX_FILEOUTPUTACTION_H
