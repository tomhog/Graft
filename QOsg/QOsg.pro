#-------------------------------------------------
#
# Project created by QtCreator 2015-05-19T23:46:33
#
#-------------------------------------------------

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QOsg
TEMPLATE = lib

DEFINES += QOSG_LIBRARY

SOURCES += QOsgPropertyWidgets.cpp \
            QOsgInspectorWidget.cpp \
            PropertyUtils.cpp

HEADERS += QOsgNotifyHandler.h \
        QOsgTreeWidget.h \
        QOsgObject.h \
        QOsgPropertyWidgets.h \
        QOsgInspectorWidget.h \
        PropertyUtils.h \
    Export.h

INCLUDEPATH += ../

unix:!macx {
    target.path = /usr/lib
    INSTALLS += target

    INCLUDEPATH += /home/hogbox/Documents/libs/osg/include

    CONFIG(debug, debug|release) {
        LIBS += -L"/home/hogbox/Documents/libs/osg/lib" -lOpenThreadsd -losgd -losgDBd -losgUtild
    }

    CONFIG(release, debug|release) {
        LIBS += -L"/home/hogbox/Documents/libs/osg/lib" -lOpenThreads -losg -losgDB -losgUtil
    }
}

win32 {
    DEFINES += WIN32
    INCLUDEPATH += C:\Work\VS2013_x64\osg\include

    CONFIG(debug, debug|release) {
        LIBS += -L"C:\Work\VS2013_x64\osg\lib" -lOpenThreadsd -losgd -losgDBd -losgUtild
    }

    CONFIG(release, debug|release) {
        LIBS += -L"C:\Work\VS2013_x64\osg\lib" -lOpenThreads -losg -losgDB -losgUtil
    }
}

macx {
    INCLUDEPATH += /Users/thomashogarth/Documents/libs/osg/include

    QMAKE_LFLAGS += -F/Users/thomashogarth/Documents/libs/osg/release/Frameworks

    LIBS += -framework OpenThreads -framework osg -framework osgDB -framework osgUtil
}


release:DESTDIR = ../release
debug:DESTDIR = ../debug
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui
