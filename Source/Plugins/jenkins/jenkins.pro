
include(../plugins_qtc_dir.pri)

QT += xml network gui

DEFINES += JENKINSPLUGIN_LIBRARY

QTC_PLUGIN_NAME = Jenkins

QTC_PLUGIN_DEPENDS = coreplugin \
                     texteditor

SOURCES +=  src/jenkinssettingspage.cpp \
            src/displaywidget.cpp \
            src/projects.cpp \
            src/datafetcher.cpp \
            src/jenkinsplugin.cpp

HEADERS +=  src/jenkinsplugin.h \
            src/jenkinssettingspage.h \
            src/jenkinsconstants.h \
            src/displaywidget.h \
            src/projects.h \
            src/datafetcher.h

FORMS += src/jenkinssettingspage.ui

RESOURCES += \
    jenkins.qrc

OTHER_FILES += \
    Readme.md

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

DEFINES -= QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII

include(../plugins_dest_dir.pri)
message("$$TARGET destination dir: $$DESTDIR");
