#-------------------------------------------------
#
# Project created by QtCreator 2015-05-03T04:48:06
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Graft
TEMPLATE = app

SOURCES += main.cpp \
        MainWindow.cpp


HEADERS += MainWindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += ../

unix {

    INCLUDEPATH += /home/hogbox/Documents/libs/osg/include

    CONFIG(debug, debug|release) {
        LIBS += -L"/home/hogbox/Documents/libs/osg/lib" -lOpenThreadsd -losgd -losgDBd -losgUtild -losgGAd -losgManipulatord -losgViewerd -losgQtd
        LIBS += -L"/home/hogbox/Documents/hbx/bin/debug" -lGraft -lQOsg
    }

    CONFIG(release, debug|release) {
        LIBS += -L"/home/hogbox/Documents/libs/osg/lib" -lOpenThreads -losg -losgDB -losgUtil -losgGA -losgManipulator -losgViewer -losgQt
        LIBS += -L"/home/hogbox/Documents/hbx/bin/debug" -lGraft -lQOsg
    }

}

win32 {
    DEFINES += WIN32
    INCLUDEPATH += C:\Work\VS2013_x64\osg\include

    CONFIG(debug, debug|release) {
        LIBS += -L"C:\Work\VS2013_x64\osg\lib" -lOpenThreadsd -losgd -losgDBd -losgUtild -losgGAd -losgManipulatord -losgViewerd -losgQTd
        LIBS += -L"C:/Work/hbx/bin/debug" -lGraft -lQOsg
    }

    CONFIG(release, debug|release) {
        LIBS += -L"C:\Work\VS2013_x64\osg\lib" -lOpenThreads -losg -losgDB -losgUtil -losgGA -losgManipulator -losgViewer -losgQT
        LIBS += -L"C:/Work/hbx/bin/release" -lGraft -lQOsg
    }

    RC_FILE = graftapp.rc
}

macx {
    DEFINE += OSX
    INCLUDEPATH += /Users/thomashogarth/Documents/libs/osg/include

    QMAKE_LFLAGS += -F/Users/thomashogarth/Documents/libs/osg/release/Frameworks
    LIBS += -framework OpenThreads -framework osg -framework osgDB -framework osgUtil -framework osgGA -framework osgManipulator -framework osgViewer -framework osgQT
    CONFIG(debug, debug|release) {
        LIBS += -L"/Users/thomashogarth/Documents/hbx/bin/debug" -lGraft -lQOsg
    }
    CONFIG(release, debug|release) {
        LIBS += -L"/Users/thomashogarth/Documents/hbx/bin/debug" -lGraft -lQOsg
    }

    ICON = hbx-icon.icns
}


release:DESTDIR = ../release
debug:DESTDIR = ../debug
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

