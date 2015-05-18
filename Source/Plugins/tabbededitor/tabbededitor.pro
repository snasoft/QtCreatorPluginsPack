DEFINES += TABBEDEDITOR_LIBRARY

PROVIDER = QtCreatorPluginsPack

include(../plugins_qtc_dir.pri)

# TabbedEditor files

SOURCES += \
    plugin.cpp \
    tabbar.cpp

HEADERS += \
    constants.h \
    plugin.h \
    tabbar.h

RESOURCES += \
    resources/res.qrc

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

include(../plugins_dest_dir.pri)
message("$$TARGET destination dir: $$DESTDIR");
