@echo off

set OSG_ROOT=C:\mew\dvl\osg283

set OSG_PATH=%OSG_ROOT%\bin
set PATH=%OSG_PATH%;%PATH%

set OSG_NOTIFY_LEVEL=WARN
 
osgviewer.exe x.osg xyz100.osg.100.scale.65000,65000,2000.trans

