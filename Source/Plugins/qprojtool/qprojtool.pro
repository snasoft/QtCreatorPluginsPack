#-------------------------------------------------
#
# Project created by QtCreator 2013-01-19T11:50:30
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = qpt

CONFIG += console

##deploy doc: http://qt-project.org/doc/qt-5.0/qtdoc/deployment-windows.html
## package also:
# get from: C:\Program Files\Microsoft Visual Studio 10.0\VC\redist\x86\Microsoft.VC100.CRT
#  MSVCR100.DLL
#  MSVCP100.DLL
#  Qt5Core.dll from C:\Qt\Qt5.0.2\5.0.2\msvc2010\bin
#'
# Windows:  Qtcreator  user settings C:\Users\<user>\AppData\Roaming => %APPDATA%\QtProject\qtcreator\externaltools
win32 {
   CONFIG += embed_manifest_exe static
}


VERSION = $$cat($$PWD/version.txt)

DEFINES += VERSION=\\\"$$VERSION\\\"


TEMPLATE = app
INCLUDEPATH += $$PWD/src

SOURCES += $$PWD/src/main.cpp \
     $$PWD/src/qmake.cpp \
     $$PWD/src/backendtool.cpp \
     $$PWD/src/tool.cpp \
     $$PWD/src/qcommandline.cpp \
     $$PWD/src/qprojcommandline.cpp \
     $$PWD/src/manager.cpp \
     $$PWD/src/configtool.cpp \
     $$PWD/src/util.cpp

HEADERS += \
     $$PWD/src/qmake.h \
     $$PWD/src/backendtool.h \
     $$PWD/src/tool.h \
     $$PWD/src/qcommandline.h \
     $$PWD/src/qprojcommandline.h \
     $$PWD/src/manager.h \
     $$PWD/src/configtool.h \
     $$PWD/src/util.h

OTHER_FILES += \
    TODO.txt

include(../plugins_dest_dir.pri)
message("$$TARGET destination dir: $$DESTDIR");



