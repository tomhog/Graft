#!/bin/bash

cd /Users/thomashogarth/Documents/hbx/bin/debug

# pass just binary to change
change_ot_lib () {
	install_name_tool -change /Users/thomashogarth/Documents/libs/osg/lib/OpenThreads.framework/Versions/20/OpenThreads \
			@executable_path/../Frameworks/OpenThreads.framework/Versions/20/OpenThreads \
			$1
}

id_osg_lib () {
	install_name_tool -id @executable_path/../Frameworks/$1.framework/Versions/122/$1 \
			./GraftApp.app/Contents/Frameworks/$1.framework/Versions/122/$1
}

id_osg_libs () {
	for i in ${@:1} ; do id_osg_lib $i; done
}

# pass osg lib name and binary to change
change_osg_lib () {
	install_name_tool -change /Users/thomashogarth/Documents/libs/osg/lib/$1.framework/Versions/122/$1 \
			@executable_path/../Frameworks/$1.framework/Versions/122/$1 \
			$2
}

change_osg_libs () {
	for i in ${@:1} ; do change_osg_lib $i $1; done
}

change_qt_lib () {
	install_name_tool -change /Users/thomashogarth/Qt/5.4/clang_64/lib/$1.framework/Versions/5/$1 \
			@executable_path/../Frameworks/$1.framework/Versions/5/$1 \
			$2
}

change_qt_libs () {
	for i in ${@:1} ; do change_qt_lib $i $1; done
}

mkdir ./GraftApp.app/Contents/PlugIns
mkdir ./GraftApp.app/Contents/PlugIns/hbxPlugins
mkdir ./GraftApp.app/Contents/PlugIns/platforms

# copy osg frame works and plugins
cp -r /Users/thomashogarth/Documents/libs/osg/Frameworks ./GraftApp.app/Contents/
cp -r /Users/thomashogarth/Documents/libs/osg/osgPlugins-3.3.8 ./GraftApp.app/Contents/PlugIns/
cp -r /Users/thomashogarth/Documents/libs/osg/osgPlugins-3.3.8 ./GraftApp.app/Contents/PlugIns/

# set ids of osg libs
install_name_tool -id @executable_path/../Frameworks/OpenThreads.framework/Versions/20/OpenThreads \
			./GraftApp.app/Contents/Frameworks/OpenThreads.framework/Versions/20/OpenThreads
id_osg_libs osg osgDB osgUtil osgText osgViewer osgWidget osgGA osgManipulator osgQt

# copy hbx lib
cp -r ./libhbx.1.dylib ./GraftApp.app/Contents/PlugIns/

# change where app looks for hbx lib
install_name_tool -change libhbx.1.dylib \
			@executable_path/../PlugIns/libhbx.1.dylib \
			./GraftApp.app/Contents/MacOs/GraftApp


# change where app looks for osg frameworks
change_ot_lib ./GraftApp.app/Contents/MacOs/GraftApp
change_osg_libs ./GraftApp.app/Contents/MacOs/GraftApp osg osgDB osgUtil osgGA osgManipulator osgViewer osgQt

# change where hbx lib looks for osg frameworks
change_ot_lib ./GraftApp.app/Contents/PlugIns/libhbx.1.dylib
change_osg_libs ./GraftApp.app/Contents/PlugIns/libhbx.1.dylib osg osgDB osgUtil

# change where osg libs look for eachother
change_ot_lib ./GraftApp.app/Contents/Frameworks/osg.framework/Versions/122/osg

change_ot_lib ./GraftApp.app/Contents/Frameworks/osgUtil.framework/Versions/122/osgUtil
change_osg_libs ./GraftApp.app/Contents/Frameworks/osgUtil.framework/Versions/122/osgUtil osg

change_ot_lib ./GraftApp.app/Contents/Frameworks/osgDB.framework/Versions/122/osgDB
change_osg_libs ./GraftApp.app/Contents/Frameworks/osgDB.framework/Versions/122/osgDB osg osgUtil

change_ot_lib ./GraftApp.app/Contents/Frameworks/osgGA.framework/Versions/122/osgGA
change_osg_libs ./GraftApp.app/Contents/Frameworks/osgGA.framework/Versions/122/osgGA osg osgDB osgUtil

change_ot_lib ./GraftApp.app/Contents/Frameworks/osgText.framework/Versions/122/osgText
change_osg_libs ./GraftApp.app/Contents/Frameworks/osgText.framework/Versions/122/osgText osg osgDB osgUtil

change_ot_lib ./GraftApp.app/Contents/Frameworks/osgViewer.framework/Versions/122/osgViewer
change_osg_libs ./GraftApp.app/Contents/Frameworks/osgViewer.framework/Versions/122/osgViewer osg osgDB osgUtil osgGA osgText

change_ot_lib ./GraftApp.app/Contents/Frameworks/osgManipulator.framework/Versions/122/osgManipulator
change_osg_libs ./GraftApp.app/Contents/Frameworks/osgManipulator.framework/Versions/122/osgManipulator osg osgDB osgUtil osgGA osgText osgViewer

change_ot_lib ./GraftApp.app/Contents/Frameworks/osgWidget.framework/Versions/122/osgWidget
change_osg_libs ./GraftApp.app/Contents/Frameworks/osgWidget.framework/Versions/122/osgWidget osg osgDB osgUtil osgGA osgText osgViewer

change_ot_lib ./GraftApp.app/Contents/Frameworks/osgQt.framework/Versions/122/osgQt
change_osg_libs ./GraftApp.app/Contents/Frameworks/osgQt.framework/Versions/122/osgQt osg osgDB osgUtil osgGA osgText osgViewer osgWidget


# copy qt frameworks
cp -R /Users/thomashogarth/Qt/5.4/clang_64/lib/QtCore.framework ./GraftApp.app/Contents/Frameworks/
cp -R /Users/thomashogarth/Qt/5.4/clang_64/lib/QtGui.framework ./GraftApp.app/Contents/Frameworks/
cp -R /Users/thomashogarth/Qt/5.4/clang_64/lib/QtWidgets.framework ./GraftApp.app/Contents/Frameworks/
cp -R /Users/thomashogarth/Qt/5.4/clang_64/lib/QtOpenGL.framework ./GraftApp.app/Contents/Frameworks/
cp -R /Users/thomashogarth/Qt/5.4/clang_64/lib/QtPrintSupport.framework ./GraftApp.app/Contents/Frameworks/

# set qt framework ids
install_name_tool -id @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore \
			./GraftApp.app/Contents/Frameworks/QtCore.framework/Versions/5/QtCore
install_name_tool -id @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui \
			./GraftApp.app/Contents/Frameworks/QtGui.framework/Versions/5/QtGui
install_name_tool -id @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets \
			./GraftApp.app/Contents/Frameworks/QtWidgets.framework/Versions/5/QtWidgets
install_name_tool -id @executable_path/../Frameworks/QtOpenGL.framework/Versions/5/QtOpenGL \
			./GraftApp.app/Contents/Frameworks/QtOpenGL.framework/Versions/5/QtOpenGL
install_name_tool -id @executable_path/../Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport \
			./GraftApp.app/Contents/Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport

# change where qt frameworks look for each other
change_qt_libs ./GraftApp.app/Contents/Frameworks/QtGui.framework/Versions/5/QtGui QtCore
change_qt_libs ./GraftApp.app/Contents/Frameworks/QtWidgets.framework/Versions/5/QtWidgets QtCore QtGui
change_qt_libs ./GraftApp.app/Contents/Frameworks/QtOpenGL.framework/Versions/5/QtOpenGL QtCore QtGui QtWidgets
change_qt_libs ./GraftApp.app/Contents/Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport QtCore QtGui QtWidgets

# change where app looks for qt frameworks
change_qt_libs ./GraftApp.app/Contents/MacOs/GraftApp QtCore QtGui QtWidgets QtOpenGL

# change where osgQT looks for qt frameworks
change_qt_libs ./GraftApp.app/Contents/Frameworks/osgQT.framework/Versions/122/osgQT QtCore QtGui QtWidgets QtOpenGL


# copy qt platform plugins 
cp -R /Users/thomashogarth/Qt/5.4/clang_64/plugins/platforms/libqcocoa_debug.dylib ./GraftApp.app/Contents/PlugIns/platforms/
cp -R /Users/thomashogarth/Qt/5.4/clang_64/plugins/platforms/libqcocoa.dylib ./GraftApp.app/Contents/PlugIns/platforms/
cp -R /Users/thomashogarth/Qt/5.4/clang_64/plugins/platforms/libqminimal_debug.dylib ./GraftApp.app/Contents/PlugIns/platforms/
cp -R /Users/thomashogarth/Qt/5.4/clang_64/plugins/platforms/libqminimal.dylib ./GraftApp.app/Contents/PlugIns/platforms/
cp -R /Users/thomashogarth/Qt/5.4/clang_64/plugins/platforms/libqoffscreen_debug.dylib ./GraftApp.app/Contents/PlugIns/platforms/
cp -R /Users/thomashogarth/Qt/5.4/clang_64/plugins/platforms/libqoffscreen.dylib ./GraftApp.app/Contents/PlugIns/platforms/

# change where qt platform plugins look for qt
change_qt_libs ./GraftApp.app/Contents/PlugIns/platforms/libqcocoa_debug.dylib QtCore QtGui QtWidgets QtPrintSupport
change_qt_libs ./GraftApp.app/Contents/PlugIns/platforms/libqcocoa.dylib QtCore QtGui QtWidgets QtPrintSupport
change_qt_libs ./GraftApp.app/Contents/PlugIns/platforms/libqminimal_debug.dylib QtCore QtGui
change_qt_libs ./GraftApp.app/Contents/PlugIns/platforms/libqminimal.dylib QtCore QtGui
change_qt_libs ./GraftApp.app/Contents/PlugIns/platforms/libqoffscreen_debug.dylib QtCore QtGui
change_qt_libs ./GraftApp.app/Contents/PlugIns/platforms/libqoffscreen.dylib QtCore QtGui

# hack to fix osg plugins, some of the osg libs won't be referenced but don't want to specify for individual plugins
OSG_PLUGINS=./GraftApp.app/Contents/PlugIns/osgPlugins-3.3.8/*
for f in $OSG_PLUGINS
do
	change_ot_lib $f
	change_osg_libs $f osg osgDB osgUtil osgGA osgText osgViewer osgWidget osgAnimation osgShadow osgSim osgManipulator osgQT
	echo $f
done

# copy and relink hbx plugins
CP_HBX_PLUGINS=./hbxPlugins/*
for f in $CP_HBX_PLUGINS
do
echo $f
	if [[ $f == *"dylib"* ]]
	then
		cp -r $f ./GraftApp.app/Contents/PlugIns/hbxPlugins/
	fi
done

HBX_PLUGINS=./GraftApp.app/Contents/PlugIns/hbxPlugins/*
for f in $HBX_PLUGINS
do
echo $f
	change_ot_lib $f
	change_osg_libs $f osg osgDB osgUtil osgGA osgText osgViewer osgWidget osgAnimation osgShadow osgSim osgManipulator osgQT
	install_name_tool -change libhbx.1.dylib \
			@executable_path/../PlugIns/libhbx.1.dylib \
			$f
	echo $f
done

