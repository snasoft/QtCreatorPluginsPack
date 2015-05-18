DEFINES += GISTPLUGIN_LIBRARY

QT += network

include(../plugins_qtc_dir.pri)

# GistPlugin files

SOURCES += \
    src/gistplugin.cpp \
    src/gistmanager.cpp \
    src/optionspage.cpp \
    src/settings.cpp

HEADERS += \
    src/gistplugin.h \
    src/gistplugin_global.h \
    src/gistpluginconstants.h \
    src/gistmanager.h \
    src/optionspage.h \
    src/settings.h


QTC_PLUGIN_NAME = GistPlugin
QTC_LIB_DEPENDS += \
    extensionsystem \
    utils
    # nothing here at this time

QTC_PLUGIN_DEPENDS += \
    coreplugin \
    projectexplorer \
    texteditor

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

FORMS += \
    src/optionspage.ui

RESOURCES += \
    gistplugin.qrc

include(../plugins_dest_dir.pri)
message("$$TARGET destination dir: $$DESTDIR");
