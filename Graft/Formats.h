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

#ifndef GRAFT_FORMATS_H
#define GRAFT_FORMATS_H 1

#include <Graft/Export.h>

#include <osgDB/PluginQuery>

namespace graft {


// returns all files with a supported extension in the passed folder
extern GRAFT_EXPORT void getAllSupportedFilesInFolder(const std::string& aFolder, osgDB::FilePathList& allFiles, bool recurseSubFolders = false);


//
// Helper class to get information about formats supported by available osgPlugins
//
class GRAFT_EXPORT Formats : public osg::Referenced
{
public:

    static Formats* instance(bool erase = false);

    // query all avaliable osg plugins and build our _infos list
    // is called by the contructor but user can call to refresh osg plugin info
    void buildPluginInfo();

    // returns true if one of the _infos list supports anExtension
    bool supported(std::string anExtension);

    // returns true if anExtension is avaliable and if supports the passed featyres
    bool supportsFeatures(const std::string& anExtension, const osgDB::ReaderWriter::Features& aFeaturesMask);

    // returns via anInfoList reference all the ReaderWriterInfos which support the requested features
    void getAllPluginsSupportingFeatures(const osgDB::ReaderWriter::Features& aFeaturesMask, osgDB::ReaderWriterInfoList& anInfoList);

    // convenience method returns via anInfoList reference all the ReaderWriterInfos that support reading nodes
    void getAllPluginsSupportingReadNode(osgDB::ReaderWriterInfoList& anInfoList);

    // convenience method returns via anInfoList reference all the ReaderWriterInfos that support writing nodes
    void getAllPluginsSupportingWriteNode(osgDB::ReaderWriterInfoList& anInfoList);

    // convenience method returns via anInfoList reference all the ReaderWriterInfos that support reading images
    void getAllPluginsSupportingReadImage(osgDB::ReaderWriterInfoList& anInfoList);

    // convenience method returns via anInfoList reference all the ReaderWriterInfos that support writing images
    void getAllPluginsSupportingWriteImage(osgDB::ReaderWriterInfoList& anInfoList);

    // convenience method returns result of buildExtensionsString being passed all available read node plugins
    std::string getReadNodeExtensionsString(const std::string& aSeperator = " ", const std::string& aPrefix = "*.", const std::string& aPostfix = "");

    // convenience method returns result of buildExtensionsString being passed all available write node plugins
    std::string getWriteNodeExtensionsString(const std::string& aSeperator = " ", const std::string& aPrefix = "*.", const std::string& aPostfix = "");

    // convenience method returns result of buildExtensionsString being passed all available read image plugins
    std::string getReadImageExtensionsString(const std::string& aSeperator = " ", const std::string& aPrefix = "*.", const std::string& aPostfix = "");

    // convenience method returns result of buildExtensionsString being passed all available write image plugins
    std::string getWriteImageExtensionsString(const std::string& aSeperator = " ", const std::string& aPrefix = "*.", const std::string& aPostfix = "");

    // helper fuction to get all the FormatDescriptionMaps from the passed ReaderWriterInfoList
    static void getFormatDescriptionsForPlugins(const osgDB::ReaderWriterInfoList& anInfoList, osgDB::ReaderWriter::FormatDescriptionMap& aFormatDescriptionMap);

    // helper fuction to build a string with all the extensions in aFormatDescriptionMap
    // string is built like aPrefix + {ext} + aPostfix + aSeperator
    static std::string buildExtensionsString(const osgDB::ReaderWriter::FormatDescriptionMap& aFormatDescriptionMap, const std::string& aSeperator = " ", const std::string& aPrefix = "*.", const std::string& aPostfix = "");

protected:

    Formats();
    virtual ~Formats(void);

    //list of all osg plugin infos
    osgDB::ReaderWriterInfoList _infos;
};

} // end graft namesapce

#endif // GRAFT_FORMATS_H
