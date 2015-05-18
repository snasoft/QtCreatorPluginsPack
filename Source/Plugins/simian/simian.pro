QT += xml gui
DEFINES += SIMIAN_LIBRARY

include(../plugins_qtc_dir.pri)

# Simian files

SOURCES +=  simianplugin.cpp \
            simianoutputpane.cpp \
            optionspage.cpp \
            ui/optionsdialog.cpp \
            settings.cpp

HEADERS +=  simianplugin.h \
            simian_global.h \
            simianconstants.h \
            simianoutputpane.h \
            optionspage.h \
            ui/optionsdialog.h \
            settings.h

PROVIDER = QtCreatorPluginsPack

QTC_PLUGIN_NAME = Simian

QTC_PLUGIN_DEPENDS += \
    coreplugin \
    projectexplorer

RESOURCES += \
    resources/res.qrc

FORMS += \
    ui/optionsdialog.ui

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

DEFINES -= QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII

include(../plugins_dest_dir.pri)
message("$$TARGET destination dir: $$DESTDIR");
