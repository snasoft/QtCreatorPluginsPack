DEFINES += QTCDBVIEWER_LIBRARY

VERSION_SUFFIX = ""

include(../plugins_qtc_dir.pri)

# QtcDbViewer files

include (3rd-party/QtSqlView/src/src.pri)

SOURCES += \
    src/QtcDbViewerPlugin.cpp

HEADERS += \
    src/Constants.h \
    src/PluginGlobal.h \
    src/QtcDbViewerPlugin.h

TRANSLATIONS += \
    translation/QtcDbViewer_ru.ts

RESOURCES += \
    resources/resources.qrc

OTHER_FILES += \
    LICENSE.md \
    README.md \
    uncrustify.cfg \
    dist/README.md \
    dist/make_dist.bat \
    dist/make_dist.sh \
    resources/README.md

OTHER_FILES += $$TRANSLATIONS

PROVIDER = QtCreatorPluginsPack

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.

QTC_PLUGIN_NAME = QtcDbViewer
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
