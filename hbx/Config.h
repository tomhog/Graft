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

#ifndef CONFIG_H
#define CONFIG_H

#include <hbx/Export.h>
#include <osg/Object>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>

#define HBX_CONFIG_PATH "./config"
#define HBX_CONFIG_FILE "config.osgt"

namespace hbx {

class HBX_EXPORT ConfigObject : public osg::Object
{
public:
    ConfigObject()
        : osg::Object(),
        _precision(3),
        _lastImportDirectory(""),
        _lastExportDirectory(""),
        _clearColor(osg::Vec4(77.0f/255.0f, 184.0f/255.0f, 219.0f/255.0f, 0.0f))
    {
    }
    ConfigObject(const ConfigObject& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : osg::Object(op, copyop),
       _precision(op._precision),
       _lastImportDirectory(op._lastImportDirectory),
       _lastExportDirectory(op._lastExportDirectory),
       _clearColor(op._clearColor)
    {
    }

    META_Object(hbx,ConfigObject)

    inline void setPrecision(int aPrecision) { _precision = aPrecision; }
    inline int getPrecision() const { return _precision; }

    inline void setLastImportDirectory(const std::string& aDirectory){ _lastImportDirectory = aDirectory; }
    inline const std::string& getLastImportDirectory() const { return _lastImportDirectory; }

    inline void setLastExportDirectory(const std::string& aDirectory){ _lastExportDirectory = aDirectory; }
    inline const std::string& getLastExportDirectory() const { return _lastExportDirectory; }

    inline void setClearColor(const osg::Vec4& aClearColor) { _clearColor = aClearColor; }
    inline const osg::Vec4& getClearColor() const { return _clearColor; }

protected:
    virtual ~ConfigObject(void){
    }

    // precision used when converting decimal numbers to string
    int _precision;
    std::string _lastImportDirectory;
    std::string _lastExportDirectory;

    osg::Vec4 _clearColor;

};

//
// Singleton to get hold of our Config object
// on construct it will read ConfigObject from file if one is avaliable
//
class HBX_EXPORT Config : public osg::Referenced
{
public:
    static Config* instance(bool erase = false)
    {
        static osg::ref_ptr<Config> s_config = new Config;
        if (erase)
        {
            //s_formats->destruct();
            s_config = 0;
        }
        return s_config.get(); // will return NULL on erase
    }

    ConfigObject* get() { return _object.get(); }

    bool read()
    {
        //check a config object file exisits
        std::string configFilePath = std::string(HBX_CONFIG_PATH) + "/" + std::string(HBX_CONFIG_FILE);
        if(!osgDB::fileExists(configFilePath)) {
            write();
        }

        _object = dynamic_cast<ConfigObject*>(osgDB::readObjectFile(configFilePath));
        if(_object.valid()) {
            OSG_ALWAYS << "Loaded config" << std::endl;
        } else {
            OSG_ALWAYS << "Failed loading config" << std::endl;
        }
        return _object != NULL;
    }
    bool write()
    {
        //check config directory exists
        std::string configPath = std::string(HBX_CONFIG_PATH);
        std::string configFilePath = std::string(HBX_CONFIG_PATH) + "/" + std::string(HBX_CONFIG_FILE);
        if(osgDB::fileType(configPath) != osgDB::DIRECTORY)
            osgDB::makeDirectory(configPath);
        return osgDB::writeObjectFile(*_object.get(), configFilePath);
    }


protected:
    Config()
        :  osg::Referenced(),
          _object(NULL)
    {
        if(!read())
            _object = new ConfigObject();
    }
    virtual ~Config(void)
    {
        write();
    }

    osg::ref_ptr<ConfigObject> _object;
};

} // end hbx

#endif // CONFIG_H

