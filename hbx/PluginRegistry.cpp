#include <hbx/PluginRegistry.h>

#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

using namespace hbx;

PluginRegistry* PluginRegistry::instance(bool erase)
{
    static osg::ref_ptr<PluginRegistry> s_registry = new PluginRegistry;
    if (erase)
    {
        //s_registry->destruct();
        s_registry = 0;
    }
    return s_registry.get(); // will return NULL on erase
}

//
//private constructor for singleton
//
PluginRegistry::PluginRegistry(void)
    : osg::Referenced()
{
    loadAllAvaliablePlugins();
}

PluginRegistry::~PluginRegistry(void)
{
    OSG_NOTICE << "Deallocating PluginRegistry:." << std::endl;
    _operations.clear();
    _dlList.clear();
}

//
// loads all the libraries in ./hbxPlugins
//
void PluginRegistry::loadAllAvaliablePlugins()
{
    osgDB::DirectoryContents pluginsContents = osgDB::getDirectoryContents("./hbxPlugins");

    for(osgDB::DirectoryContents::iterator pItr = pluginsContents.begin(); pItr != pluginsContents.end(); pItr++) {
        if(osgDB::getFileExtension(*pItr) == "dll") {

            std::string libraryName = "./hbxPlugins/" + (*pItr);
            osgDB::Registry::LoadStatus result = this->loadLibrary(libraryName);

            if(result == osgDB::Registry::LOADED)
            {
                OSG_INFO << "Plugin INFO: Plugin '" << libraryName << "', was loaded successfully." << std::endl;
            }else if(result == osgDB::Registry::PREVIOUSLY_LOADED) {
                OSG_WARN << "Plugin WARN: Library '" << libraryName << "' has already been loaded" << std::endl;
            }else{
                //failed to load the libray
                OSG_WARN << "Plugin WARN: Failed to load Library '" << libraryName << "'." << std::endl;
            }
        }
    }


    //build our operations list
    osgDB::ObjectWrapperManager* wrapperMng = osgDB::Registry::instance()->getObjectWrapperManager();
    osgDB::ObjectWrapperManager::WrapperMap wrapperMap = wrapperMng->getWrapperMap();

    for(osgDB::ObjectWrapperManager::WrapperMap::iterator wrapItr = wrapperMap.begin();
        wrapItr != wrapperMap.end();
        ++wrapItr)
    {
        osgDB::ObjectWrapper* wrapper = (*wrapItr).second.get();
        hbx::Action* asAction = dynamic_cast<hbx::Action*>(wrapper->createInstance());
        if(asAction != NULL)
            _operations.push_back(asAction);
    }
}

//
// register a new object with the singleton
//
void PluginRegistry::addActionToRegistry(Action* anAction)
{
    //already exist by operation class name
    Action* existing = getActionByClassName(anAction->className());
    if(existing){return;}
    _operations.push_back(anAction);
}

//
// get a registered operation by its class name
//
Action* PluginRegistry::getActionByClassName(const std::string& aClassName)
{
    for(ActionList::iterator itr = _operations.begin(); itr != _operations.end(); itr++)
        if((*itr)->className() == aClassName)
            return (*itr).get();
    return NULL;
}

//
// return the list of registered operation types
//
const ActionList& PluginRegistry::getActionsList()
{
    return _operations;
}

//
// load a library, which should register a plugin of some sort
//
osgDB::Registry::LoadStatus PluginRegistry::loadLibrary(const std::string& aFileName)
{
    DynamicLibraryList::iterator ditr = getLibraryItr(aFileName);
    if (ditr!=_dlList.end()) return osgDB::Registry::PREVIOUSLY_LOADED;

    osgDB::DynamicLibrary* dl = osgDB::DynamicLibrary::loadLibrary(aFileName);

    if (dl)
    {
        _dlList.push_back(dl);
        return osgDB::Registry::LOADED;
    }
    return osgDB::Registry::NOT_LOADED;
}

//
// return the iterator into _dlList of a library loaded with aFileName
//
PluginRegistry::DynamicLibraryList::iterator PluginRegistry::getLibraryItr(const std::string& aFileName)
{
    DynamicLibraryList::iterator ditr = _dlList.begin();
    for(;ditr!=_dlList.end();++ditr)
    {
        if ((*ditr)->getName()==aFileName) return ditr;
    }
    return _dlList.end();
}

//
// prints infomation on all loaded plugins via osgNotify
//
void PluginRegistry::printLoadedPlugins()
{
    OSG_ALWAYS << "Registered operations: " << _operations.size() << std::endl;

    for(ActionList::iterator itr = _operations.begin(); itr != _operations.end(); itr++)
        OSG_ALWAYS << "    Action classname: " << (*itr)->className() << std::endl;
}

