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

#ifndef OSGDB_PROPERTYEDIT_H
#define OSGDB_PROPERTYEDIT_H 1

#include <QOsg/Export.h>
#include <osgDB/ClassInterface>

#define DECIMAL_PLACES 2;

// returns the number of elements for a type (int=1, vec2=2, vec4=4 etc)
extern QOSG_EXPORT int getNumElementsForType(osgDB::BaseSerializer::Type aType);

// convert property to an std string, if property has multiple elements they are seperated by a ,
extern QOSG_EXPORT bool getPropertyAsString(osg::Object* anObject, const std::string& aPropertyName, std::string& aValueString);
// set the property from a string, multi elements should be seperated by ,
extern QOSG_EXPORT bool setPropertyFromString(osg::Object* anObject, const std::string& aPropertyName, const std::string& aValueString);

// convert property to a string and push to aStringVector if the property has multiple elements they are individually pushed onto the vector
extern QOSG_EXPORT bool getPropertyAsStringVector(osg::Object* anObject, const std::string& aPropertyName, std::vector<std::string>& aStringVector);
extern QOSG_EXPORT bool setPropertyFromStringVector(osg::Object* anObject, const std::string& aPropertyName, const std::vector<std::string>& aStringVector);

#endif // OSGDB_PROPERTYEDIT_H
