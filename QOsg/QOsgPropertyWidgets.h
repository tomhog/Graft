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

#ifndef QOSGPROPERTYWIDGETS_H
#define QOSGPROPERTYWIDGETS_H

#include <QOsg/Export.h>

#include <osgDB/ClassInterface>

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QToolButton>
#include <QLineEdit>
#include <QValidator>

class QOSG_EXPORT QOsgPropertyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QOsgPropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent = 0);

    static QOsgPropertyWidget* Create(osg::Object* anObject, const std::string& aPropertyName, const osgDB::BaseSerializer::Type& aType);

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
class QOSG_EXPORT QOsgBoolPropertyWidget : public QOsgPropertyWidget
{
    Q_OBJECT
public:
    explicit QOsgBoolPropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent = 0);
signals:
public slots:
    void onStateChanged(int value);
protected:
    QCheckBox* _checkbox;
};

//
// Basic combo box for enums
//
class QOSG_EXPORT QOsgEnumPropertyWidget : public QOsgPropertyWidget
{
    Q_OBJECT
public:
    explicit QOsgEnumPropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent = 0);
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
class QOSG_EXPORT QOsgTextPropertyWidget : public QOsgPropertyWidget
{
    Q_OBJECT
public:
    explicit QOsgTextPropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent = 0);
signals:
public slots:
    void onEditingFinished();
protected:
    std::vector<QLineEdit*> _lineEdits;
};

//
// A text box for a path, plus a button to open the choose file dialog
// Inpector will use for any String properties with a name cotaining the word Path or Directory
//
class QOSG_EXPORT QOsgPathPropertyWidget : public QOsgPropertyWidget
{
    Q_OBJECT
public:
    explicit QOsgPathPropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent = 0);
signals:
public slots:
    void onEditingFinished();
    void onSelectFileButtonClicked(bool checked);
protected:
    QLineEdit* _lineEdit;
    QToolButton* _button;
};

#endif // QOSGPROPERTYWIDGETS_H
