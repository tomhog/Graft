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

    FileOutputAction()
        : OutputAction()
    {}

    FileOutputAction(const FileOutputAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : OutputAction(op, copyop)
    {
    }
    META_Object(hbx,FileOutputAction)

    virtual std::string friendlyName(){ return "Save File"; }
    virtual std::string description(){ return "Write object data to file";}
    virtual std::string category(){ return "File"; }

    virtual void process(ActionData* aData)
    {
        if(aData->asNode() != NULL)
            osgDB::writeNodeFile(*aData->asNode(), aData->_filePath);
        else if(aData->asImage() != NULL)
            osgDB::writeImageFile(*aData->asImage(), aData->_filePath);
    }

protected:

};

} // end hbx

#endif // HBX_FILEOUTPUTACTION_H
