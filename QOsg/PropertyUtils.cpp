#include <QOsg/PropertyUtils.h>

#include <iomanip>
#include <osg/io_utils>

using namespace osgDB;

// same as std::to_string but allows us to set precision for decimal numbers
template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(n) << a_value;
    return out.str();
}

template <typename T>
std::string property_to_string(osgDB::ClassInterface* ci, osg::Object* obj, std::string propname)
{
    T value;
    ci->getProperty<T>(obj, propname, value);
    int precision = DECIMAL_PLACES
    return to_string_with_precision<T>(value, precision);
}

template <typename T>
T string_to_type(const std::string aString)
{
    T value;
    std::istringstream in(aString);
    in >> value;
    return value;
}

template <typename T>
void string_to_property(osgDB::ClassInterface* ci, osg::Object* obj, std::string propname, std::string aValueString)
{
    T value = string_to_type<T>(aValueString);
    ci->setProperty<T>(obj, propname, value);
}

//
// returns the number of elements for a type (int=1, vec2=2, vec4=4 etc)
//
int getNumElementsForType(osgDB::BaseSerializer::Type aType)
{
    switch(aType)
    {
        case osgDB::BaseSerializer::RW_STRING:
        case osgDB::BaseSerializer::RW_INT:
        case osgDB::BaseSerializer::RW_UINT:
        case osgDB::BaseSerializer::RW_FLOAT:
        case osgDB::BaseSerializer::RW_DOUBLE:
            return 1;
        case osgDB::BaseSerializer::RW_VEC2F:
        case osgDB::BaseSerializer::RW_VEC2D:
            return 2;
        case osgDB::BaseSerializer::RW_VEC3F:
        case osgDB::BaseSerializer::RW_VEC3D:
            return 3;
        case osgDB::BaseSerializer::RW_VEC4F:
        case osgDB::BaseSerializer::RW_VEC4D:
            return 4;
        case osgDB::BaseSerializer::RW_MATRIXF:
        case osgDB::BaseSerializer::RW_MATRIXD:
            return 16;
        default: break;
    }
    return 1;
}

//
// convert property value to an std string, if value has multiple elements they are seperated by a ,
//
bool getPropertyAsString(osg::Object* anObject, const std::string& aPropertyName, std::string& aValueString)
{
    osgDB::ClassInterface ci;
    osgDB::BaseSerializer::Type type;
    if(!ci.getPropertyType(anObject, aPropertyName, type))
        return false;

    switch(type)
    {
        case osgDB::BaseSerializer::RW_STRING:
            aValueString = property_to_string<std::string>(&ci, anObject, aPropertyName);
            break;
        case osgDB::BaseSerializer::RW_INT:
            aValueString = property_to_string<int>(&ci, anObject, aPropertyName);
            break;
        case osgDB::BaseSerializer::RW_UINT:
            aValueString = property_to_string<unsigned int>(&ci, anObject, aPropertyName);
            break;
        case osgDB::BaseSerializer::RW_FLOAT:
            aValueString = property_to_string<float>(&ci, anObject, aPropertyName);
            break;
        case osgDB::BaseSerializer::RW_DOUBLE:
            aValueString = property_to_string<double>(&ci, anObject, aPropertyName);
            break;
        case osgDB::BaseSerializer::RW_VEC2F:
            aValueString = property_to_string<osg::Vec2f>(&ci, anObject, aPropertyName);
            break;
        case osgDB::BaseSerializer::RW_VEC2D:
            aValueString = property_to_string<osg::Vec2d>(&ci, anObject, aPropertyName);
            break;
        case osgDB::BaseSerializer::RW_VEC3F:
            aValueString = property_to_string<osg::Vec3f>(&ci, anObject, aPropertyName);
            break;
        case osgDB::BaseSerializer::RW_VEC3D:
            aValueString = property_to_string<osg::Vec3d>(&ci, anObject, aPropertyName);
            break;
        case osgDB::BaseSerializer::RW_VEC4F:
            aValueString = property_to_string<osg::Vec4f>(&ci, anObject, aPropertyName);
            break;
        case osgDB::BaseSerializer::RW_VEC4D:
            aValueString = property_to_string<osg::Vec4d>(&ci, anObject, aPropertyName);
            break;
        case osgDB::BaseSerializer::RW_MATRIXF:
            aValueString = property_to_string<osg::Matrixf>(&ci, anObject, aPropertyName);
            break;
        case osgDB::BaseSerializer::RW_MATRIXD:
            aValueString = property_to_string<osg::Matrixd>(&ci, anObject, aPropertyName);
            break;
        default: break;
    }
    return true;
}

//
// set the property from a string, multi elements should be seperated by ,
//
bool setPropertyFromString(osg::Object* anObject, const std::string& aPropertyName, const std::string& aValueString)
{
    osgDB::ClassInterface ci;
    osgDB::BaseSerializer::Type type;
    if(!ci.getPropertyType(anObject, aPropertyName, type))
        return false;

    osgDB::StringList elements;

    switch(type)
    {
        case osgDB::BaseSerializer::RW_STRING:
            string_to_property<std::string>(&ci, anObject, aPropertyName, aValueString);
            break;
        case osgDB::BaseSerializer::RW_INT:
            string_to_property<int>(&ci, anObject, aPropertyName, aValueString);
            break;
        case osgDB::BaseSerializer::RW_UINT:
            string_to_property<unsigned int>(&ci, anObject, aPropertyName, aValueString);
            break;
        case osgDB::BaseSerializer::RW_FLOAT:
            string_to_property<float>(&ci, anObject, aPropertyName, aValueString);
            break;
        case osgDB::BaseSerializer::RW_DOUBLE:
            string_to_property<double>(&ci, anObject, aPropertyName, aValueString);
            break;
        case osgDB::BaseSerializer::RW_VEC2F:
            string_to_property<osg::Vec2f>(&ci, anObject, aPropertyName, aValueString);
            break;
        case osgDB::BaseSerializer::RW_VEC2D:
            string_to_property<osg::Vec2d>(&ci, anObject, aPropertyName, aValueString);
            break;
        case osgDB::BaseSerializer::RW_VEC3F:
            string_to_property<osg::Vec3f>(&ci, anObject, aPropertyName, aValueString);
            break;
        case osgDB::BaseSerializer::RW_VEC3D:
            string_to_property<osg::Vec3d>(&ci, anObject, aPropertyName, aValueString);
            break;
        case osgDB::BaseSerializer::RW_MATRIXF:
            //string_to_property<osg::Matrix>(&ci, anObject, aPropertyName, aValueString);
            break;
        case osgDB::BaseSerializer::RW_MATRIXD:
            //string_to_property<osg::Matrixd>(&ci, anObject, aPropertyName, aValueString);
            break;
        default: break;
    }
    return true;
}

//
//
//
bool getPropertyAsStringVector(osg::Object* anObject, const std::string& aPropertyName, std::vector<std::string>& aStringVector)
{
    std::string vstring;
    getPropertyAsString(anObject, aPropertyName, vstring);
    osgDB::split(vstring, aStringVector, ' ');

    return true;
}

bool setPropertyFromStringVector(osg::Object* anObject, const std::string& aPropertyName, const std::vector<std::string>& aStringVector)
{
    std::string concat = "";
    for(unsigned int i=0; i<aStringVector.size(); i++)
    {
        concat += aStringVector[i] + " ";
    }
    concat = concat.substr(0, concat.size() - 1);
    setPropertyFromString(anObject, aPropertyName, concat);
    return true;
}


