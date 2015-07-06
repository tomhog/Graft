#include <Graft/Formats.h>

#include <osg/ValueObject>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#define FORMATS_CONFIG_PATH "./config/format.osgx"

using namespace graft;

// returns all files with a supported extension in the passed folder
void graft::getAllSupportedFilesInFolder(const std::string& aFolder, osgDB::FilePathList& allFiles, bool recurseSubFolders)
{
    osgDB::DirectoryContents contents = osgDB::getDirectoryContents(aFolder);
    for(osgDB::DirectoryContents::iterator itr=contents.begin(); itr<contents.end(); ++itr)
    {
        std::string fullpath = aFolder + "/" + (*itr);
        if(osgDB::fileType(fullpath) == osgDB::DIRECTORY)
        {
            if(recurseSubFolders) getAllSupportedFilesInFolder(fullpath, allFiles, recurseSubFolders);
        }else {
            //
            if(Formats::instance()->supported(osgDB::getFileExtension(fullpath))) {
                allFiles.push_back(fullpath);
            }
        }
    }
}

Formats* Formats::instance(bool erase)
{
    static osg::ref_ptr<Formats> s_formats = new Formats;
    if (erase)
    {
        //s_formats->destruct();
        s_formats = 0;
    }
    return s_formats.get(); // will return NULL on erase
}

Formats::Formats()
 : osg::Referenced()
{
    buildPluginInfo();
}

Formats::~Formats()
{

}

//
// query all avaliable osg plugins and build our infos and formatDesctions
//
void Formats::buildPluginInfo()
{
    _infos.clear();

    osgDB::FileNameList plugins = osgDB::listAllAvailablePlugins();

    for(osgDB::FileNameList::iterator itr = plugins.begin();
        itr != plugins.end();
        ++itr)
    {
        osgDB::ReaderWriterInfoList osgPluginInfo;
        osgDB::queryPlugin(*itr, osgPluginInfo);
        for(osgDB::ReaderWriterInfoList::iterator pitr = osgPluginInfo.begin();
            pitr != osgPluginInfo.end();
            ++pitr)
        {
            osgDB::ReaderWriterInfo* info = (*pitr).get();
            _infos.push_back(*pitr);
        }
    }
}

//
// returns true if anExtension exists in the _formatDescriptions map
//
bool Formats::supported(std::string anExtension)
{
    for(osgDB::ReaderWriterInfoList::iterator infoItr = _infos.begin();
        infoItr != _infos.end();
        ++infoItr)
    {
        for(osgDB::ReaderWriter::FormatDescriptionMap::iterator formatItr = (*infoItr)->extensions.begin();
            formatItr != (*infoItr)->extensions.end();
            ++formatItr)
        {
            if((*formatItr).first == anExtension) {
                return true;
            }
        }
    }
    return false;
}

//
// returns true if anExtension is avaliable and if supports the passed featyres
//
bool Formats::supportsFeatures(const std::string& anExtension, const osgDB::ReaderWriter::Features& aFeaturesMask)
{
    for(osgDB::ReaderWriterInfoList::iterator infoItr = _infos.begin();
        infoItr != _infos.end();
        ++infoItr)
    {
        for(osgDB::ReaderWriter::FormatDescriptionMap::iterator formatItr = (*infoItr)->extensions.begin();
            formatItr != (*infoItr)->extensions.end();
            ++formatItr)
        {
            if((*formatItr).first == anExtension) {
                if (((*infoItr)->features & aFeaturesMask) == aFeaturesMask)
                    return true;
            }
        }
    }
    return false;
}

//
// returns via anInfoList reference all the ReaderWriterInfos which support the requested features
//
void Formats::getAllPluginsSupportingFeatures(const osgDB::ReaderWriter::Features& aFeaturesMask, osgDB::ReaderWriterInfoList& anInfoList)
{
    for(osgDB::ReaderWriterInfoList::iterator infoItr = _infos.begin();
        infoItr != _infos.end();
        ++infoItr)
    {
        if (((*infoItr)->features & aFeaturesMask) == aFeaturesMask)
            anInfoList.push_back(*infoItr);
    }
}

//
// convenience method returns via anInfoList reference all the ReaderWriterInfos that support reading nodes
//
void Formats::getAllPluginsSupportingReadNode(osgDB::ReaderWriterInfoList& anInfoList)
{
    getAllPluginsSupportingFeatures(osgDB::ReaderWriter::FEATURE_READ_NODE, anInfoList);
}

//
// convenience method returns via anInfoList reference all the ReaderWriterInfos that support writing nodes
//
void Formats::getAllPluginsSupportingWriteNode(osgDB::ReaderWriterInfoList& anInfoList)
{
    getAllPluginsSupportingFeatures(osgDB::ReaderWriter::FEATURE_WRITE_NODE, anInfoList);
}

//
// convenience method returns via anInfoList reference all the ReaderWriterInfos that support reading images
//
void Formats::getAllPluginsSupportingReadImage(osgDB::ReaderWriterInfoList& anInfoList)
{
    getAllPluginsSupportingFeatures(osgDB::ReaderWriter::FEATURE_READ_IMAGE, anInfoList);
}

//
// convenience method returns via anInfoList reference all the ReaderWriterInfos that support writing images
//
void Formats::getAllPluginsSupportingWriteImage(osgDB::ReaderWriterInfoList& anInfoList)
{
    getAllPluginsSupportingFeatures(osgDB::ReaderWriter::FEATURE_WRITE_IMAGE, anInfoList);
}

//
// convenience method returns result of buildExtensionsString being passed all available read node plugins
//
std::string Formats::getReadNodeExtensionsString(const std::string& aSeperator, const std::string& aPrefix, const std::string& aPostfix)
{
    osgDB::ReaderWriterInfoList infoList;
    getAllPluginsSupportingReadNode(infoList);
    osgDB::ReaderWriter::FormatDescriptionMap formats;
    getFormatDescriptionsForPlugins(infoList, formats);
    return buildExtensionsString(formats, aSeperator, aPrefix, aPostfix);
}

//
// convenience method returns result of buildExtensionsString being passed all available write node plugins
//
std::string Formats::getWriteNodeExtensionsString(const std::string& aSeperator, const std::string& aPrefix, const std::string& aPostfix)
{
    osgDB::ReaderWriterInfoList infoList;
    getAllPluginsSupportingWriteNode(infoList);
    osgDB::ReaderWriter::FormatDescriptionMap formats;
    getFormatDescriptionsForPlugins(infoList, formats);
    return buildExtensionsString(formats, aSeperator, aPrefix, aPostfix);
}

//
// convenience method returns result of buildExtensionsString being passed all available read image plugins
//
std::string Formats::getReadImageExtensionsString(const std::string& aSeperator, const std::string& aPrefix, const std::string& aPostfix)
{
    osgDB::ReaderWriterInfoList infoList;
    getAllPluginsSupportingReadImage(infoList);
    osgDB::ReaderWriter::FormatDescriptionMap formats;
    getFormatDescriptionsForPlugins(infoList, formats);
    return buildExtensionsString(formats, aSeperator, aPrefix, aPostfix);
}

//
// convenience method returns result of buildExtensionsString being passed all available write image plugins
//
std::string Formats::getWriteImageExtensionsString(const std::string& aSeperator, const std::string& aPrefix, const std::string& aPostfix)
{
    osgDB::ReaderWriterInfoList infoList;
    getAllPluginsSupportingWriteImage(infoList);
    osgDB::ReaderWriter::FormatDescriptionMap formats;
    getFormatDescriptionsForPlugins(infoList, formats);
    return buildExtensionsString(formats, aSeperator, aPrefix, aPostfix);
}

//
// helper fuction to get all the FormatDescriptionMaps from the passed ReaderWriterInfoList
//
void Formats::getFormatDescriptionsForPlugins(const osgDB::ReaderWriterInfoList& anInfoList, osgDB::ReaderWriter::FormatDescriptionMap& aFormatDescriptionMap)
{
    for(osgDB::ReaderWriterInfoList::const_iterator infoItr = anInfoList.begin();
        infoItr != anInfoList.end();
        ++infoItr)
    {
        for(osgDB::ReaderWriter::FormatDescriptionMap::iterator formatItr = (*infoItr)->extensions.begin();
            formatItr != (*infoItr)->extensions.end();
            ++formatItr)
        {
            aFormatDescriptionMap[(*formatItr).first] = (*formatItr).second;
        }
    }
}

//
// helper fuction to build a string with all the extensions in aFormatDescriptionMap
// string is built like aPrefix + {ext} + aPostfix + aSeperator
//
std::string Formats::buildExtensionsString(const osgDB::ReaderWriter::FormatDescriptionMap& aFormatDescriptionMap, const std::string& aSeperator, const std::string& aPrefix, const std::string& aPostfix)
{
    std::string ext = "";
    for(osgDB::ReaderWriter::FormatDescriptionMap::const_iterator formatItr = aFormatDescriptionMap.begin();
        formatItr != aFormatDescriptionMap.end();
        ++formatItr)
    {
        ext += aPrefix + (*formatItr).first + aPostfix + aSeperator;
    }
    //trim last aSeperator
    ext = ext.substr(0, ext.size()-1);
    return ext;
}




