
include(../plugins_qtc_dir.pri)

QT       += network xml gui

DEFINES += HABRASTATA_LIBRARY

# HabraStata files

SOURCES += habrastataplugin.cpp \
           optionspage.cpp

HEADERS += habrastataplugin.h \
        habrastata_global.h \
        habrastataconstants.h \
        optionspage.h

PROVIDER = QtCreatorPluginsPack

QTC_PLUGIN_NAME = HabraStata

QTC_PLUGIN_DEPENDS += \
    coreplugin

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

include(../plugins_dest_dir.pri)
message("$$TARGET destination dir: $$DESTDIR");
