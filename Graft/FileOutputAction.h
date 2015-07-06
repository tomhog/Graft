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

#ifndef GRAFT_FILEOUTPUTACTION_H
#define GRAFT_FILEOUTPUTACTION_H 1

#include <Graft/Action.h>
#include <Graft/Visitors.h>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

namespace graft {

//
// Write the node or image to file
//
class GRAFT_EXPORT FileOutputAction : public OutputAction
{
public:

    FileOutputAction()
        : OutputAction(),
          _outputTextures(false)
    {}

    FileOutputAction(const FileOutputAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : OutputAction(op, copyop),
       _outputTextures(op._outputTextures)
    {
    }
    META_Object(graft,FileOutputAction)

    virtual std::string friendlyName(){ return "Save File"; }
    virtual std::string description(){ return "Write object data to file";}
    virtual std::string category(){ return "File"; }

    virtual void process(ActionData* aData)
    {
        if(aData->asNode() != NULL) {
            osgDB::writeNodeFile(*aData->asNode(), aData->_filePath);

            //find textures and output if requested
            if(_outputTextures)
            {
                FindTexturesVisitor findTex;
                aData->asNode()->accept(findTex);
                for(TextureSet::iterator itr = findTex._textureSet.begin();
                    itr != findTex._textureSet.end();
                    ++itr)
                {
                    osg::Texture2D* asTex2d = dynamic_cast<osg::Texture2D*>((*itr).get());
                    if(asTex2d == NULL) continue;
                    osg::Image* img = asTex2d->getImage();
                    if(img == NULL) continue;
                    //
                    std::string wd = osgDB::getCurrentWorkingDirectory();
                    std::string imgpath = img->getFileName();
                    if(!osgDB::isAbsolutePath(imgpath))
                    {
                        // root for relative paths
                        std::string root = osgDB::getFilePath(aData->_filePath);
                        std::string imgdir = osgDB::getFilePath(imgpath);
                        std::vector<std::string> pe;
                        osgDB::getPathElements(imgdir, pe);
                        //make sure the path exisits
                        for(unsigned int i=0; i<pe.size(); i++)
                        {
                            std::string p = osgDB::concatPaths(root, pe[i]);
                            if(osgDB::fileType(p) != osgDB::DIRECTORY)
                                osgDB::makeDirectory(p);
                        }
                        imgpath = osgDB::concatPaths(root, imgpath);
                    }

                    //osgDB::setCurrentWorkingDirectory(dir);
                    osgDB::writeImageFile(*img, imgpath);
                    //osgDB::setCurrentWorkingDirectory(wd);
                }
            }

        } else if(aData->asImage() != NULL) {
            osgDB::writeImageFile(*aData->asImage(), aData->_filePath);
        }
    }

    //
    // Serializable
    //
    bool getSaveTextures() const { return _outputTextures; }
    void setSaveTextures(bool isEnabled){ _outputTextures = isEnabled; }

protected:

    bool _outputTextures;

};

} // end graft

#endif // GRAFT_FILEOUTPUTACTION_H
