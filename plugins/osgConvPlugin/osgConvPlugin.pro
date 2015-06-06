#-------------------------------------------------
#
# Project created by QtCreator 2015-05-19T23:46:33
#
#-------------------------------------------------

QT       -= core gui

TARGET = osgConvPlugin
TEMPLATE = lib

DEFINES += OSGCONVPLUGIN_LIBRARY

SOURCES += \
    SmoothingAction.cpp \
    SimplifyAction.cpp \
    AddMissingColourArraysAction.cpp \
    OptimizeAction.cpp \
    FixTransparencyAction.cpp \
    PruneStateSetsAction.cpp \
    DefaultNormalsAction.cpp \
    GenerateTangentsAction.cpp \
    OrientationConvertorAction.cpp \
    CompressTexturesAction.cpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += C:\Work\VS2013_x64\osg\include \
                ../../

CONFIG(debug, debug|release) {
LIBS += -L"C:\Work\VS2013_x64\osg\lib" -lOpenThreadsd -losgd -losgDBd -losgUtild
LIBS += -L"C:/Work/Graft/bin/debug" -lhbx
}

CONFIG(release, debug|release) {
LIBS += -L"C:\Work\VS2013_x64\osg\lib" -lOpenThreads -losg -losgDB -losgUtil
LIBS += -L"C:/Work/Graft/bin/release" -lhbx
}

release:DESTDIR = ../../release/hbxPlugins
debug:DESTDIR = ../../debug/hbxPlugins
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui