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

#ifndef INSPECTORWIDGET_H
#define INSPECTORWIDGET_H

#include <QScrollArea>
#include <QFormLayout>

#include <osg/Object>

class InspectorWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit InspectorWidget(QWidget *parent = 0);

    void setTargetObject(osg::Object* anObject);

signals:

public slots:

protected:
    void clearLayout(QLayout* layout, bool deleteWidgets = true);

    QWidget* _scrollAreaContents;
    QFormLayout* _layout;
};

#endif // INSPECTORWIDGET_H
