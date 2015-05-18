## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES){
    win32:QTCREATOR_SOURCES=D:\Sources\QtCreator\qt-creator-opensource-src-3.4.0
    unix:QTCREATOR_SOURCES=../../../QtCreator/qt-creator-opensource-src-3.4.0-beta1
}

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE){
    win32:IDE_BUILD_TREE=D:\Sources\QtCreator\build-qtcreator-msvc2013_x86-Release
    unix:IDE_BUILD_TREE=../../../QtCreator/qt-creator-build-3.4.0-beta1
}

PLUGINS_PACK_VENDOR = "Plugins Pack"
