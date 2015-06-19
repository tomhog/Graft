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
			./Graft.app/Contents/Frameworks/$1.framework/Versions/122/$1
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

mkdir ./Graft.app/Contents/PlugIns
mkdir ./Graft.app/Contents/PlugIns/hbxPlugins
mkdir ./Graft.app/Contents/PlugIns/platforms

# copy osg frame works and plugins
cp -r /Users/thomashogarth/Documents/libs/osg/Frameworks ./Graft.app/Contents/
cp -r /Users/thomashogarth/Documents/libs/osg/osgPlugins-3.3.8 ./Graft.app/Contents/PlugIns/
cp -r /Users/thomashogarth/Documents/libs/osg/osgPlugins-3.3.8 ./Graft.app/Contents/PlugIns/

# set ids of osg libs
install_name_tool -id @executable_path/../Frameworks/OpenThreads.framework/Versions/20/OpenThreads \
			./Graft.app/Contents/Frameworks/OpenThreads.framework/Versions/20/OpenThreads
id_osg_libs osg osgDB osgUtil osgText osgViewer osgWidget osgGA osgManipulator osgQt osgAnimation osgSim osgShadow osgTerrain osgParticle osgFX osgUI osgVolume osgPresentation

# copy hbx lib
cp -r ./libhbx.1.dylib ./Graft.app/Contents/PlugIns/

# change where app looks for hbx lib
install_name_tool -change libhbx.1.dylib \
			@executable_path/../PlugIns/libhbx.1.dylib \
			./Graft.app/Contents/MacOs/Graft


# change where app looks for osg frameworks
change_ot_lib ./Graft.app/Contents/MacOs/Graft
change_osg_libs ./Graft.app/Contents/MacOs/Graft osg osgDB osgUtil osgGA osgManipulator osgViewer osgQt

# change where hbx lib looks for osg frameworks
change_ot_lib ./Graft.app/Contents/PlugIns/libhbx.1.dylib
change_osg_libs ./Graft.app/Contents/PlugIns/libhbx.1.dylib osg osgDB osgUtil

# change where osg libs look for eachother
change_ot_lib ./Graft.app/Contents/Frameworks/osg.framework/Versions/122/osg

change_ot_lib ./Graft.app/Contents/Frameworks/osgUtil.framework/Versions/122/osgUtil
change_osg_libs ./Graft.app/Contents/Frameworks/osgUtil.framework/Versions/122/osgUtil osg

change_ot_lib ./Graft.app/Contents/Frameworks/osgDB.framework/Versions/122/osgDB
change_osg_libs ./Graft.app/Contents/Frameworks/osgDB.framework/Versions/122/osgDB osg osgUtil

change_ot_lib ./Graft.app/Contents/Frameworks/osgGA.framework/Versions/122/osgGA
change_osg_libs ./Graft.app/Contents/Frameworks/osgGA.framework/Versions/122/osgGA osg osgDB osgUtil

change_ot_lib ./Graft.app/Contents/Frameworks/osgText.framework/Versions/122/osgText
change_osg_libs ./Graft.app/Contents/Frameworks/osgText.framework/Versions/122/osgText osg osgDB osgUtil

change_ot_lib ./Graft.app/Contents/Frameworks/osgViewer.framework/Versions/122/osgViewer
change_osg_libs ./Graft.app/Contents/Frameworks/osgViewer.framework/Versions/122/osgViewer osg osgDB osgUtil osgGA osgText

change_ot_lib ./Graft.app/Contents/Frameworks/osgManipulator.framework/Versions/122/osgManipulator
change_osg_libs ./Graft.app/Contents/Frameworks/osgManipulator.framework/Versions/122/osgManipulator osg osgDB osgUtil osgGA osgText osgViewer

change_ot_lib ./Graft.app/Contents/Frameworks/osgWidget.framework/Versions/122/osgWidget
change_osg_libs ./Graft.app/Contents/Frameworks/osgWidget.framework/Versions/122/osgWidget osg osgDB osgUtil osgGA osgText osgViewer

change_ot_lib ./Graft.app/Contents/Frameworks/osgAnimation.framework/Versions/122/osgAnimation
change_osg_libs ./Graft.app/Contents/Frameworks/osgAnimation.framework/Versions/122/osgAnimation osg osgDB osgUtil osgGA osgText osgViewer

change_ot_lib ./Graft.app/Contents/Frameworks/osgSim.framework/Versions/122/osgSim
change_osg_libs ./Graft.app/Contents/Frameworks/osgSim.framework/Versions/122/osgSim osg osgDB osgUtil osgText

change_ot_lib ./Graft.app/Contents/Frameworks/osgShadow.framework/Versions/122/osgShadow
change_osg_libs ./Graft.app/Contents/Frameworks/osgShadow.framework/Versions/122/osgShadow osg osgDB osgUtil

change_ot_lib ./Graft.app/Contents/Frameworks/osgTerrain.framework/Versions/122/osgTerrain
change_osg_libs ./Graft.app/Contents/Frameworks/osgTerrain.framework/Versions/122/osgTerrain osg osgDB osgUtil

change_ot_lib ./Graft.app/Contents/Frameworks/osgParticle.framework/Versions/122/osgParticle
change_osg_libs ./Graft.app/Contents/Frameworks/osgParticle.framework/Versions/122/osgParticle osg osgDB osgUtil

change_ot_lib ./Graft.app/Contents/Frameworks/osgFX.framework/Versions/122/osgFX
change_osg_libs ./Graft.app/Contents/Frameworks/osgFX.framework/Versions/122/osgFX osg osgDB osgUtil

change_ot_lib ./Graft.app/Contents/Frameworks/osgVolume.framework/Versions/122/osgVolume
change_osg_libs ./Graft.app/Contents/Frameworks/osgVolume.framework/Versions/122/osgVolume osg osgDB osgUtil osgGA

change_ot_lib ./Graft.app/Contents/Frameworks/osgUI.framework/Versions/122/osgUI
change_osg_libs ./Graft.app/Contents/Frameworks/osgUI.framework/Versions/122/osgUI osg osgDB osgUtil osgText osgGA osgViewer

change_ot_lib ./Graft.app/Contents/Frameworks/osgPresentation.framework/Versions/122/osgPresentation
change_osg_libs ./Graft.app/Contents/Frameworks/osgPresentation.framework/Versions/122/osgPresentation osg osgDB osgUtil osgText osgViewer osgGA osgFX osgVolume osgManipulator osgWidget osgUI

change_ot_lib ./Graft.app/Contents/Frameworks/osgQt.framework/Versions/122/osgQt
change_osg_libs ./Graft.app/Contents/Frameworks/osgQt.framework/Versions/122/osgQt osg osgDB osgUtil osgGA osgText osgViewer osgWidget


# copy qt frameworks
cp -R /Users/thomashogarth/Qt/5.4/clang_64/lib/QtCore.framework ./Graft.app/Contents/Frameworks/
cp -R /Users/thomashogarth/Qt/5.4/clang_64/lib/QtGui.framework ./Graft.app/Contents/Frameworks/
cp -R /Users/thomashogarth/Qt/5.4/clang_64/lib/QtWidgets.framework ./Graft.app/Contents/Frameworks/
cp -R /Users/thomashogarth/Qt/5.4/clang_64/lib/QtOpenGL.framework ./Graft.app/Contents/Frameworks/
cp -R /Users/thomashogarth/Qt/5.4/clang_64/lib/QtPrintSupport.framework ./Graft.app/Contents/Frameworks/

# set qt framework ids
install_name_tool -id @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore \
			./Graft.app/Contents/Frameworks/QtCore.framework/Versions/5/QtCore
install_name_tool -id @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui \
			./Graft.app/Contents/Frameworks/QtGui.framework/Versions/5/QtGui
install_name_tool -id @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets \
			./Graft.app/Contents/Frameworks/QtWidgets.framework/Versions/5/QtWidgets
install_name_tool -id @executable_path/../Frameworks/QtOpenGL.framework/Versions/5/QtOpenGL \
			./Graft.app/Contents/Frameworks/QtOpenGL.framework/Versions/5/QtOpenGL
install_name_tool -id @executable_path/../Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport \
			./Graft.app/Contents/Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport

# change where qt frameworks look for each other
change_qt_libs ./Graft.app/Contents/Frameworks/QtGui.framework/Versions/5/QtGui QtCore
change_qt_libs ./Graft.app/Contents/Frameworks/QtWidgets.framework/Versions/5/QtWidgets QtCore QtGui
change_qt_libs ./Graft.app/Contents/Frameworks/QtOpenGL.framework/Versions/5/QtOpenGL QtCore QtGui QtWidgets
change_qt_libs ./Graft.app/Contents/Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport QtCore QtGui QtWidgets

# change where app looks for qt frameworks
change_qt_libs ./Graft.app/Contents/MacOs/Graft QtCore QtGui QtWidgets QtOpenGL

# change where osgQT looks for qt frameworks
change_qt_libs ./Graft.app/Contents/Frameworks/osgQT.framework/Versions/122/osgQT QtCore QtGui QtWidgets QtOpenGL


# copy qt platform plugins 
cp -R /Users/thomashogarth/Qt/5.4/clang_64/plugins/platforms/libqcocoa_debug.dylib ./Graft.app/Contents/PlugIns/platforms/
cp -R /Users/thomashogarth/Qt/5.4/clang_64/plugins/platforms/libqcocoa.dylib ./Graft.app/Contents/PlugIns/platforms/
cp -R /Users/thomashogarth/Qt/5.4/clang_64/plugins/platforms/libqminimal_debug.dylib ./Graft.app/Contents/PlugIns/platforms/
cp -R /Users/thomashogarth/Qt/5.4/clang_64/plugins/platforms/libqminimal.dylib ./Graft.app/Contents/PlugIns/platforms/
cp -R /Users/thomashogarth/Qt/5.4/clang_64/plugins/platforms/libqoffscreen_debug.dylib ./Graft.app/Contents/PlugIns/platforms/
cp -R /Users/thomashogarth/Qt/5.4/clang_64/plugins/platforms/libqoffscreen.dylib ./Graft.app/Contents/PlugIns/platforms/

# change where qt platform plugins look for qt
change_qt_libs ./Graft.app/Contents/PlugIns/platforms/libqcocoa_debug.dylib QtCore QtGui QtWidgets QtPrintSupport
change_qt_libs ./Graft.app/Contents/PlugIns/platforms/libqcocoa.dylib QtCore QtGui QtWidgets QtPrintSupport
change_qt_libs ./Graft.app/Contents/PlugIns/platforms/libqminimal_debug.dylib QtCore QtGui
change_qt_libs ./Graft.app/Contents/PlugIns/platforms/libqminimal.dylib QtCore QtGui
change_qt_libs ./Graft.app/Contents/PlugIns/platforms/libqoffscreen_debug.dylib QtCore QtGui
change_qt_libs ./Graft.app/Contents/PlugIns/platforms/libqoffscreen.dylib QtCore QtGui

# hack to fix osg plugins, some of the osg libs won't be referenced but don't want to specify for individual plugins
OSG_PLUGINS=./Graft.app/Contents/PlugIns/osgPlugins-3.3.8/*
for f in $OSG_PLUGINS
do
	change_ot_lib $f
	change_osg_libs $f osg osgDB osgUtil osgGA osgText osgViewer osgWidget osgAnimation osgShadow osgSim osgManipulator osgQT osgTerrain osgParticle osgFX osgPresentation osgUI osgVolume
	echo $f
done

# copy and relink hbx plugins
CP_HBX_PLUGINS=./hbxPlugins/*
for f in $CP_HBX_PLUGINS
do
echo $f
	if [[ $f == *"dylib"* ]]
	then
		cp -r $f ./Graft.app/Contents/PlugIns/hbxPlugins/
	fi
done

HBX_PLUGINS=./Graft.app/Contents/PlugIns/hbxPlugins/*
for f in $HBX_PLUGINS
do
echo $f
	change_ot_lib $f
	change_osg_libs $f osg osgDB osgUtil osgGA osgText osgViewer osgWidget osgAnimation osgShadow osgSim osgManipulator osgQT osgTerrain osgParticle osgFX osgPresentation osgUI osgVolume
	install_name_tool -change libhbx.1.dylib \
			@executable_path/../PlugIns/libhbx.1.dylib \
			$f
	echo $f
done

