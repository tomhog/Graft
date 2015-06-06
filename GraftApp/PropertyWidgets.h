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

#ifndef PROPERTYWIDGETS_H
#define PROPERTYWIDGETS_H

#include <osgDB/ClassInterface>

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QValidator>

class PropertyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent = 0);

    static PropertyWidget* Create(osg::Object* anObject, const std::string& aPropertyName, const osgDB::BaseSerializer::Type& aType);

signals:

public slots:

protected:
     QHBoxLayout* _hLayout;

     osg::ref_ptr<osg::Object> _object;
     std::string _propertyName;
     osgDB::BaseSerializer::Type _type;
     osgDB::ObjectWrapper* _wrapper;

     mutable osgDB::ClassInterface _ci;
};

//
// Basic toggle for bool values
//
class BoolPropertyWidget : public PropertyWidget
{
    Q_OBJECT
public:
    explicit BoolPropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent = 0);
signals:
public slots:
    void onStateChanged(int value);
protected:
    QCheckBox* _checkbox;
};

//
// Basic combo box for enums
//
class EnumPropertyWidget : public PropertyWidget
{
    Q_OBJECT
public:
    explicit EnumPropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent = 0);
signals:
public slots:
    void onCurrentTextChanged(const QString& value);
protected:
    QComboBox* _combobox;
};

//
// Any property type that can be represented as an array of strings
// string, int, float, double, vec2, vec3, matrix etc
//
class TextPropertyWidget : public PropertyWidget
{
    Q_OBJECT
public:
    explicit TextPropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent = 0);
signals:
public slots:
    void onEditingFinished();
protected:
    std::vector<QLineEdit*> _lineEdits;
};

#endif // PROPERTYWIDGETS_H
