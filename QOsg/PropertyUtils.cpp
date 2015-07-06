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
T to_type_int(const std::string aString)
{
    T value;
    std::vector<std::string> elements;
    osgDB::split(aString, elements, ',');
    for(unsigned int i=0; i<elements.size(); i++)
    {
        value[i] = std::atoi(elements[i].c_str());
    }
    return T;
}

template <typename T>
T to_type_float(const std::string aString)
{
    T value;
    std::vector<std::string> elements;
    osgDB::split(aString, elements, ',');
    for(unsigned int i=0; i<elements.size(); i++)
    {
        value[i] = osg::asciiToFloat(elements[i].c_str());
    }
    return T;
}

template <typename T>
T to_type_double(const std::string aString)
{
    T value;
    std::vector<std::string> elements;
    osgDB::split(aString, elements, ',');
    for(unsigned int i=0; i<elements.size(); i++)
    {
        value[i] = osg::asciiToDouble(elements[i].c_str());
    }
    return T;
}

//
// returns the number of elements for a type (int=1, vec2=2, vec4=4 etc)
//
int osgDB::getNumElementsForType(osgDB::BaseSerializer::Type aType)
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
bool osgDB::getPropertyAsString(osg::Object* anObject, const std::string& aPropertyName, std::string& aValueString)
{
    osgDB::ClassInterface ci;
    osgDB::BaseSerializer::Type type;
    if(!ci.getPropertyType(anObject, aPropertyName, type))
        return false;

    int precision = DECIMAL_PLACES;//graft::Config::instance()->get()->getPrecision();

    switch(type)
    {
        case osgDB::BaseSerializer::RW_STRING:
            ci.getProperty<std::string>(anObject, aPropertyName, aValueString);
            break;
        case osgDB::BaseSerializer::RW_INT:
            int ivalue;
            ci.getProperty<int>(anObject, aPropertyName, ivalue);
            aValueString = to_string_with_precision(ivalue, precision);
            break;
        case osgDB::BaseSerializer::RW_UINT:
            unsigned int uivalue;
            ci.getProperty<unsigned int>(anObject, aPropertyName, uivalue);
            aValueString = to_string_with_precision(uivalue, precision);
            break;
        case osgDB::BaseSerializer::RW_FLOAT:
            float fvalue;
            ci.getProperty<float>(anObject, aPropertyName, fvalue);
            aValueString = to_string_with_precision(fvalue, precision);
            break;
        case osgDB::BaseSerializer::RW_DOUBLE:
            double dvalue;
            ci.getProperty<double>(anObject, aPropertyName, dvalue);
            aValueString = to_string_with_precision(dvalue, precision);
            break;
        case osgDB::BaseSerializer::RW_VEC2F:
        {
            osg::Vec2f v2fvalue;
            ci.getProperty<osg::Vec2f>(anObject, aPropertyName, v2fvalue);
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                aValueString += to_string_with_precision(v2fvalue[i], precision) + ", ";
            aValueString = aValueString.substr(0, aValueString.size()-2);
            break;
        }
        case osgDB::BaseSerializer::RW_VEC2D:
        {
            osg::Vec2d v2dvalue;
            ci.getProperty<osg::Vec2d>(anObject, aPropertyName, v2dvalue);
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                aValueString += to_string_with_precision(v2dvalue[i], precision) + ", ";
            aValueString = aValueString.substr(0, aValueString.size()-2);
            break;
        }
        case osgDB::BaseSerializer::RW_VEC3F:
        {
            osg::Vec3f v3fvalue;
            ci.getProperty<osg::Vec3f>(anObject, aPropertyName, v3fvalue);
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                aValueString += to_string_with_precision(v3fvalue[i], precision) + ", ";
            aValueString = aValueString.substr(0, aValueString.size()-2);
            break;
        }
        case osgDB::BaseSerializer::RW_VEC3D:
        {
            osg::Vec3d v3dvalue;
            ci.getProperty<osg::Vec3d>(anObject, aPropertyName, v3dvalue);
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                aValueString += to_string_with_precision(v3dvalue[i], precision) + ", ";
            aValueString = aValueString.substr(0, aValueString.size()-2);
            break;
        }
        case osgDB::BaseSerializer::RW_VEC4F:
        {
            osg::Vec4f v4fvalue;
            ci.getProperty<osg::Vec4f>(anObject, aPropertyName, v4fvalue);
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                aValueString += to_string_with_precision(v4fvalue[i], precision) + ", ";
            aValueString = aValueString.substr(0, aValueString.size()-2);
            break;
        }
        case osgDB::BaseSerializer::RW_VEC4D:
        {
            osg::Vec4d v4dvalue;
            ci.getProperty<osg::Vec4d>(anObject, aPropertyName, v4dvalue);
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                aValueString += to_string_with_precision(v4dvalue[i], precision) + ", ";
            aValueString = aValueString.substr(0, aValueString.size()-2);
            break;
        }
        case osgDB::BaseSerializer::RW_MATRIXF:
        {
            osg::Matrixf mfvalue;
            ci.getProperty<osg::Matrixf>(anObject, aPropertyName, mfvalue);
            for(int r=0; r<4; r++)
                for(int c=0; c<4; c++)
                    aValueString += to_string_with_precision(mfvalue(r, c), precision) + ", ";
            aValueString = aValueString.substr(0, aValueString.size()-2);
            break;
        }
        case osgDB::BaseSerializer::RW_MATRIXD:
        {
            osg::Matrixd mdvalue;
            ci.getProperty<osg::Matrixd>(anObject, aPropertyName, mdvalue);
            for(int r=0; r<4; r++)
                for(int c=0; c<4; c++)
                    aValueString += to_string_with_precision(mdvalue(r, c), precision) + ", ";
            aValueString = aValueString.substr(0, aValueString.size()-2);
            break;
        }
        default: break;
    }
    return true;
}

//
// set the property from a string, multi elements should be seperated by ,
//
bool osgDB::setPropertyFromString(osg::Object* anObject, const std::string& aPropertyName, const std::string& aValueString)
{
    osgDB::ClassInterface ci;
    osgDB::BaseSerializer::Type type;
    if(!ci.getPropertyType(anObject, aPropertyName, type))
        return false;

    osgDB::StringList elements;

    switch(type)
    {
        case osgDB::BaseSerializer::RW_STRING:
            ci.setProperty<std::string>(anObject, aPropertyName, aValueString);
            break;
        case osgDB::BaseSerializer::RW_INT:
            ci.setProperty<int>(anObject, aPropertyName, atoi(aValueString.c_str()));
            break;
        case osgDB::BaseSerializer::RW_UINT:
            ci.setProperty<unsigned int>(anObject, aPropertyName, atoi(aValueString.c_str()));
            break;
        case osgDB::BaseSerializer::RW_FLOAT:
            ci.setProperty<float>(anObject, aPropertyName, osg::asciiToFloat(aValueString.c_str()));
            break;
        case osgDB::BaseSerializer::RW_DOUBLE:
            ci.setProperty<double>(anObject, aPropertyName, osg::asciiToDouble(aValueString.c_str()));
            break;
        case osgDB::BaseSerializer::RW_VEC2F:
        {
            osgDB::split(aValueString, elements, ',');
            osg::Vec2f v2fvalue;
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                v2fvalue[i] = osg::asciiToFloat(elements[i].c_str());
            ci.setProperty<osg::Vec2f>(anObject, aPropertyName, v2fvalue);
            break;
        }
        case osgDB::BaseSerializer::RW_VEC2D:
        {
            osgDB::split(aValueString, elements, ',');
            osg::Vec2d v2dvalue;
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                v2dvalue[i] = osg::asciiToDouble(elements[i].c_str());
            ci.setProperty<osg::Vec2d>(anObject, aPropertyName, v2dvalue);
            break;
        }
        case osgDB::BaseSerializer::RW_VEC3F:
        {
            osgDB::split(aValueString, elements, ',');
            osg::Vec3f v3fvalue;
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                v3fvalue[i] = osg::asciiToFloat(elements[i].c_str());
            ci.setProperty<osg::Vec3f>(anObject, aPropertyName, v3fvalue);
            break;
        }
        case osgDB::BaseSerializer::RW_VEC3D:
        {
            osgDB::split(aValueString, elements, ',');
            osg::Vec3d v3dvalue;
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                v3dvalue[i] = osg::asciiToDouble(elements[i].c_str());
            ci.setProperty<osg::Vec3d>(anObject, aPropertyName, v3dvalue);
            break;
        }
        case osgDB::BaseSerializer::RW_MATRIXF:
        {
            osgDB::split(aValueString, elements, ',');
            osg::Matrixf mfvalue;
            for(int r=0; r<4; r++)
                for(int c=0; c<4; c++)
                    mfvalue(r, c) = osg::asciiToFloat(elements[(r*c) + c].c_str());
            ci.setProperty<osg::Matrixf>(anObject, aPropertyName, mfvalue);
            break;
        }
        case osgDB::BaseSerializer::RW_MATRIXD:
        {
            osgDB::split(aValueString, elements, ',');
            osg::Matrixd mdvalue;
            for(int r=0; r<4; r++)
                for(int c=0; c<4; c++)
                    mdvalue(r, c) = osg::asciiToDouble(elements[(r*c) + c].c_str());
            ci.setProperty<osg::Matrixd>(anObject, aPropertyName, mdvalue);
            break;
        }
        default: break;
    }
    return true;
}

//
//
//
bool osgDB::getPropertyAsStringVector(osg::Object* anObject, const std::string& aPropertyName, std::vector<std::string>& aStringVector)
{
    osgDB::ClassInterface ci;
    osgDB::BaseSerializer::Type type;
    if(!ci.getPropertyType(anObject, aPropertyName, type))
        return false;

    int precision = DECIMAL_PLACES;//graft::Config::instance()->get()->getPrecision();

    switch(type)
    {
        case osgDB::BaseSerializer::RW_STRING:
        {
            std::string svalue;
            ci.getProperty<std::string>(anObject, aPropertyName, svalue);
            aStringVector.push_back(svalue);
            break;
        }
        case osgDB::BaseSerializer::RW_INT:
            int ivalue;
            ci.getProperty<int>(anObject, aPropertyName, ivalue);
            aStringVector.push_back(to_string_with_precision(ivalue, precision));
            break;
        case osgDB::BaseSerializer::RW_UINT:
            unsigned int uivalue;
            ci.getProperty<unsigned int>(anObject, aPropertyName, uivalue);
            aStringVector.push_back(to_string_with_precision(uivalue, precision));
            break;
        case osgDB::BaseSerializer::RW_FLOAT:
            float fvalue;
            ci.getProperty<float>(anObject, aPropertyName, fvalue);
            aStringVector.push_back(to_string_with_precision(fvalue, precision));
            break;
        case osgDB::BaseSerializer::RW_DOUBLE:
            double dvalue;
            ci.getProperty<double>(anObject, aPropertyName, dvalue);
            aStringVector.push_back(to_string_with_precision(dvalue, precision));
            break;
        case osgDB::BaseSerializer::RW_VEC2F:
        {
            osg::Vec2f v2fvalue;
            ci.getProperty<osg::Vec2f>(anObject, aPropertyName, v2fvalue);
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                aStringVector.push_back(to_string_with_precision(v2fvalue[i], precision));
            break;
        }
        case osgDB::BaseSerializer::RW_VEC2D:
        {
            osg::Vec2d v2dvalue;
            ci.getProperty<osg::Vec2d>(anObject, aPropertyName, v2dvalue);
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                aStringVector.push_back(to_string_with_precision(v2dvalue[i], precision));
            break;
        }
        case osgDB::BaseSerializer::RW_VEC3F:
        {
            osg::Vec3f v3fvalue;
            ci.getProperty<osg::Vec3f>(anObject, aPropertyName, v3fvalue);
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                aStringVector.push_back(to_string_with_precision(v3fvalue[i], precision));
            break;
        }
        case osgDB::BaseSerializer::RW_VEC3D:
        {
            osg::Vec3d v3dvalue;
            ci.getProperty<osg::Vec3d>(anObject, aPropertyName, v3dvalue);
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                aStringVector.push_back(to_string_with_precision(v3dvalue[i], precision));
            break;
        }
        case osgDB::BaseSerializer::RW_VEC4F:
        {
            osg::Vec4f v4fvalue;
            ci.getProperty<osg::Vec4f>(anObject, aPropertyName, v4fvalue);
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                aStringVector.push_back(to_string_with_precision(v4fvalue[i], precision));
            break;
        }
        case osgDB::BaseSerializer::RW_VEC4D:
        {
            osg::Vec4d v4dvalue;
            ci.getProperty<osg::Vec4d>(anObject, aPropertyName, v4dvalue);
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                aStringVector.push_back(to_string_with_precision(v4dvalue[i], precision));
            break;
        }
        case osgDB::BaseSerializer::RW_MATRIXF:
        {
            osg::Matrixf mfvalue;
            ci.getProperty<osg::Matrixf>(anObject, aPropertyName, mfvalue);
            for(int r=0; r<4; r++)
                for(int c=0; c<4; c++)
                    aStringVector.push_back(to_string_with_precision(mfvalue(r, c), precision));
            break;
        }
        case osgDB::BaseSerializer::RW_MATRIXD:
        {
            osg::Matrixd mdvalue;
            ci.getProperty<osg::Matrixd>(anObject, aPropertyName, mdvalue);
            for(int r=0; r<4; r++)
                for(int c=0; c<4; c++)
                    aStringVector.push_back(to_string_with_precision(mdvalue(r, c), precision));
            break;
        }
        default: break;
    }
    return true;
}

bool osgDB::setPropertyFromStringVector(osg::Object* anObject, const std::string& aPropertyName, const std::vector<std::string>& aStringVector)
{
    osgDB::ClassInterface ci;
    osgDB::BaseSerializer::Type type;
    if(!ci.getPropertyType(anObject, aPropertyName, type))
        return false;

    std::vector<std::string> elements;

    switch(type)
    {
        case osgDB::BaseSerializer::RW_STRING:
            ci.setProperty<std::string>(anObject, aPropertyName, aStringVector[0]);
            break;
        case osgDB::BaseSerializer::RW_INT:
            ci.setProperty<int>(anObject, aPropertyName, atoi(aStringVector[0].c_str()));
            break;
        case osgDB::BaseSerializer::RW_UINT:
            ci.setProperty<unsigned int>(anObject, aPropertyName, atoi(aStringVector[0].c_str()));
            break;
        case osgDB::BaseSerializer::RW_FLOAT:
            ci.setProperty<float>(anObject, aPropertyName, osg::asciiToFloat(aStringVector[0].c_str()));
            break;
        case osgDB::BaseSerializer::RW_DOUBLE:
            ci.setProperty<double>(anObject, aPropertyName, osg::asciiToDouble(aStringVector[0].c_str()));
            break;
        case osgDB::BaseSerializer::RW_VEC2F:
        {
            osg::Vec2f v2fvalue;
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                v2fvalue[i] = osg::asciiToFloat(aStringVector[i].c_str());
            ci.setProperty<osg::Vec2f>(anObject, aPropertyName, v2fvalue);
            break;
        }
        case osgDB::BaseSerializer::RW_VEC2D:
        {
            osg::Vec2d v2dvalue;
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                v2dvalue[i] = osg::asciiToDouble(aStringVector[i].c_str());
            ci.setProperty<osg::Vec2d>(anObject, aPropertyName, v2dvalue);
            break;
        }
        case osgDB::BaseSerializer::RW_VEC3F:
        {
            osg::Vec3f v3fvalue;
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                v3fvalue[i] = osg::asciiToFloat(aStringVector[i].c_str());
            ci.setProperty<osg::Vec3f>(anObject, aPropertyName, v3fvalue);
            break;
        }
        case osgDB::BaseSerializer::RW_VEC3D:
        {
            osg::Vec3d v3dvalue;
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                v3dvalue[i] = osg::asciiToDouble(aStringVector[i].c_str());
            ci.setProperty<osg::Vec3d>(anObject, aPropertyName, v3dvalue);
            break;
        }
        case osgDB::BaseSerializer::RW_VEC4F:
        {
            osg::Vec4f v4fvalue;
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                v4fvalue[i] = osg::asciiToFloat(aStringVector[i].c_str());
            ci.setProperty<osg::Vec4f>(anObject, aPropertyName, v4fvalue);
            break;
        }
        case osgDB::BaseSerializer::RW_VEC4D:
        {
            osg::Vec4d v4dvalue;
            for(int i=0; i<osgDB::getNumElementsForType(type); i++)
                v4dvalue[i] = osg::asciiToDouble(aStringVector[i].c_str());
            ci.setProperty<osg::Vec4d>(anObject, aPropertyName, v4dvalue);
            break;
        }
        case osgDB::BaseSerializer::RW_MATRIXF:
        {
            osg::Matrixf mfvalue;
            for(int r=0; r<4; r++)
                for(int c=0; c<4; c++)
                    mfvalue(r, c) = osg::asciiToFloat(aStringVector[(r*c) + c].c_str());
            ci.setProperty<osg::Matrixf>(anObject, aPropertyName, mfvalue);
            break;
        }
        case osgDB::BaseSerializer::RW_MATRIXD:
        {
            osg::Matrixd mdvalue;
            for(int r=0; r<4; r++)
                for(int c=0; c<4; c++)
                    mdvalue(r, c) = osg::asciiToDouble(aStringVector[(r*c) + c].c_str());
            ci.setProperty<osg::Matrixd>(anObject, aPropertyName, mdvalue);
            break;
        }
        default: break;
    }
    return true;
}


