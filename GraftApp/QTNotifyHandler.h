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

#pragma once

#include <osg/Notify>
#include <QDebug>

class QTNotifyHandler : public osg::NotifyHandler
{
public:
    QTNotifyHandler() {
	}
	void notify(osg::NotifySeverity severity, const char* message) {
        switch(severity) {
            case osg::NotifySeverity::WARN:
                qWarning() << message;
                break;
            case osg::NotifySeverity::FATAL:
                qWarning() << message;
                break;
            default:
                qDebug() << message;
        }
	}
protected:
    ~QTNotifyHandler(void) {

	}
};
