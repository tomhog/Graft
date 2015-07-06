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

#ifndef GRAFT_PLUGINREGISTRY_H
#define GRAFT_PLUGINREGISTRY_H 1

#include <Graft/Export.h>

#include <osgDB/Registry>
#include <osgDB/DynamicLibrary>

#include <Graft/Action.h>

extern "C"
{
    typedef void (* CPluginFunction) (void);
}

namespace graft {

class GRAFT_EXPORT PluginRegistry : public osg::Referenced
{
public:

    static PluginRegistry* instance(bool erase = false);

    typedef std::vector< osg::ref_ptr<osgDB::DynamicLibrary> > DynamicLibraryList;

    // loads all the libraries in ./graftPlugins
    void loadAllAvaliablePlugins();

    // register a new operation type
    void addActionToRegistry(Action* anAction);

    // get a registered operation by its class name
    Action* getActionByClassName(const std::string& aClassName);

    // return the list of registered operation types
    const ActionList& getActionsList();

    // return lists of actions mapped to their category
    std::map<std::string, ActionList> getActionsByCategory();

    // load a library, which should register a plugin of some sort
    osgDB::Registry::LoadStatus loadLibrary(const std::string& aFileName);

    // return the iterator into _dlList of a library loaded with aFileName
    DynamicLibraryList::iterator getLibraryItr(const std::string& aFileName);

    // prints infomation on all loaded plugins via osgNotify
    void printLoadedPlugins();

protected:

    // private constructor for singleton
    PluginRegistry(void);
    virtual ~PluginRegistry(void);

    // list of avaliable operations registered via addActionToRegistry when plugin libraries are loaded
    ActionList _operations;

    // list of loaded libraries
    DynamicLibraryList _dlList;
};

//
//register proxy
//
class ActionsPluginRegistryProxy
{
public:
    ActionsPluginRegistryProxy(Action* proto)
    {
        //check the registry instance
        PluginRegistry* registry = PluginRegistry::instance();
        if(registry)
        {
            _operation = proto;
            registry->addActionToRegistry(proto);
        }
    }

    virtual ~ActionsPluginRegistryProxy(void)
    {
        _operation = NULL;
    }

protected:

    osg::ref_ptr<Action> _operation;

};


struct PluginFunctionProxy
{
    PluginFunctionProxy(CPluginFunction function) { (function)(); }
};

#define USE_ACTIONS_PLUGIN(classname) \
extern "C" void graftplugin_##classname(void); \
static graft::PluginFunctionProxy proxy_##classname(graftplugin_##classname);

//
//Register a new operation type plugin
//
#define REGISTER_ACTIONS_PLUGIN(classname) \
    extern "C" void graftplugin_##classname(void) {} \
    static graft::ActionsPluginRegistryProxy g_proxy_##classname(new classname);

} // end graft namespace

#endif // GRAFT_PLUGINREGISTRY_H
