#-------------------------------------------------
#
# Project created by QtCreator 2015-05-19T23:46:33
#
#-------------------------------------------------

QT       -= core gui

TARGET = Graft
TEMPLATE = lib

DEFINES += GRAFT_LIBRARY

SOURCES += Formats.cpp \
        PluginRegistry.cpp \
        Config.cpp \
        FileInputAction.cpp \
        FileOutputAction.cpp \
        Action.cpp \
        ActionQueue.cpp \
        BatchConvertor.cpp \
        ChangeFileExtensionAction.cpp \
        ChangeFileNameAction.cpp \
        ChangeDirectoryAction.cpp \
    NamedNodesAction.cpp \
    Scene.cpp

HEADERS += Export.h \
        Formats.h \
        PluginRegistry.h \
        Config.h \
        FileInputAction.h \
        FileOutputAction.h \
        Action.h \
        ActionQueue.h \
        BatchConvertor.h \
        ChangeFileExtensionAction.h \
        ChangeFileNameAction.h \
        ChangeDirectoryAction.h \
    NamedNodesAction.h \
    Visitors.h \
    Scene.h

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
    DEFINES += OSX
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
