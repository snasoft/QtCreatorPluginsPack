DEFINES += MEDIACONTROL_LIBRARY

include(../plugins_qtc_dir.pri)

# MediaControl files

SOURCES += mediacontrolplugin.cpp \
    mediacontrolwidget.cpp
macx:OBJECTIVE_SOURCES += macx.mm

HEADERS += mediacontrolplugin.h \
        mediacontrol_global.h \
        mediacontrolconstants.h \
        mediacontrolwidget.h
macx:HEADERS  += macx.h

RESOURCES += \
    resources/res.qrc

#libs
unix:!macx:LIBS += -lXtst -lX11 # sudo apt-get install libxtst-dev
macx:LIBS += -framework ApplicationServices -framework IOKit
macx:LIBS += -framework IOKit

win32:LIBS += -lUser32 #For Send Input

PROVIDER = QtCreatorPluginsPack

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.

QTC_PLUGIN_NAME = MediaControl
QTC_LIB_DEPENDS += \
    # nothing here at this time

QTC_PLUGIN_DEPENDS += \
    coreplugin

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

include(../plugins_dest_dir.pri)
message("$$TARGET destination dir: $$DESTDIR");
