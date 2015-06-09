#-------------------------------------------------
#
# Project created by QtCreator 2015-05-19T23:46:33
#
#-------------------------------------------------

QT       -= core gui

TARGET = hbx
TEMPLATE = lib

DEFINES += HBX_LIBRARY

SOURCES += Formats.cpp \
        PluginRegistry.cpp \
        PropertyEdit.cpp \
        Config.cpp \
        FileInputAction.cpp \
        FileOutputAction.cpp \
        Action.cpp \
        ActionQueue.cpp \
        BatchConvertor.cpp \
    ChangeFileExtensionAction.cpp \
    ChangeFileNameAction.cpp

HEADERS += Export.h \
        Formats.h \
        PluginRegistry.h \
        PropertyEdit.h \
        Config.h \
        FileInputAction.h \
        FileOutputAction.h \
        Action.h \
        ActionQueue.h \
        BatchConvertor.h \
    ChangeFileExtensionAction.h \
    ChangeFileNameAction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    DEFINES += WIN32
    INCLUDEPATH += C:\Work\VS2013_x64\osg\include \
                ../

    CONFIG(debug, debug|release) {
        LIBS += -L"C:\Work\VS2013_x64\osg\lib" -lOpenThreadsd -losgd -losgDBd -losgUtild
    }

    CONFIG(release, debug|release) {
        LIBS += -L"C:\Work\VS2013_x64\osg\lib" -lOpenThreads -losg -losgDB -losgUtil
    }
}

macx {
    INCLUDEPATH += /Users/thomashogarth/Documents/libs/osg/include \
                ../
    QMAKE_LFLAGS += -F/Users/thomashogarth/Documents/libs/osg/lib

    LIBS += -framework OpenThreads -framework osg -framework osgDB -framework osgUtil
}


release:DESTDIR = ../release
debug:DESTDIR = ../debug
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui
