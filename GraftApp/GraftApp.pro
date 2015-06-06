#-------------------------------------------------
#
# Project created by QtCreator 2015-05-03T04:48:06
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraftApp
TEMPLATE = app

SOURCES += main.cpp \
        PropertyWidgets.cpp \
        InspectorWidget.cpp \
        MainWindow.cpp


HEADERS += MainWindow.h \
        QTNotifyHandler.h \
        PropertyWidgets.h \
        InspectorWidget.h \
    Ommatidium.h

FORMS    += mainwindow.ui

INCLUDEPATH += C:\Work\VS2013_x64\osg\include \
                ../

CONFIG(debug, debug|release) {
LIBS += -L"C:\Work\VS2013_x64\osg\lib" -lOpenThreadsd -losgd -losgDBd -losgUtild -losgGAd -losgManipulatord -losgViewerd -losgQTd
LIBS += -L"C:/Work/Graft/bin/debug" -lhbx
}

CONFIG(release, debug|release) {
LIBS += -L"C:\Work\VS2013_x64\osg\lib" -lOpenThreads -losg -losgDB -losgUtil -losgGA -losgManipulator -losgViewer -losgQT
LIBS += -L"C:/Work/Graft/bin/release" -lhbx
}

release:DESTDIR = ../release
debug:DESTDIR = ../debug
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

