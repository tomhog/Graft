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
    CompressTexturesAction.cpp \
    ResizeImageAction.cpp \
    ClampImageSizeAction.cpp \
    CropImageToSquareAction.cpp \
    FlipImageAction.cpp \
    ScaleImageAction.cpp \
    RenameNodesAction.cpp \
    RemoveNodesAction.cpp \
    AddTextureAction.cpp \
    AddTextureFromBaseNameAction.cpp \
    FlipNormalsAction.cpp \
    LocaliseTexturesAction.cpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}


win32 {
    INCLUDEPATH += C:\Work\VS2013_x64\osg\include \
                ../../

    CONFIG(debug, debug|release) {
        LIBS += -L"C:\Work\VS2013_x64\osg\lib" -lOpenThreadsd -losgd -losgDBd -losgUtild
        LIBS += -L"C:/Work/hbx/bin/debug" -lGraft
    }

    CONFIG(release, debug|release) {
        LIBS += -L"C:\Work\VS2013_x64\osg\lib" -lOpenThreads -losg -losgDB -losgUtil
        LIBS += -L"C:/Work/hbx/bin/release" -lGraft
    }
}

macx {
    INCLUDEPATH += /Users/thomashogarth/Documents/libs/osg/include \
                ../../

    QMAKE_LFLAGS += -F/Users/thomashogarth/Documents/libs/osg/release/Frameworks
    LIBS += -framework OpenThreads -framework osg -framework osgDB -framework osgUtil
    CONFIG(debug, debug|release) {
        LIBS += -L"/Users/thomashogarth/Documents/hbx/bin/debug" -lGraft
    }
    CONFIG(release, debug|release) {
        LIBS += -L"/Users/thomashogarth/Documents/hbx/bin/debug" -lGraft
    }
}

release:DESTDIR = ../../release/graftPlugins
debug:DESTDIR = ../../debug/graftPlugins
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui
