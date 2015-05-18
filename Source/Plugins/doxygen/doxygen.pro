DEFINES += DOXYGEN_LIBRARY

PROVIDER = QtCreatorPluginsPack

include(../plugins_qtc_dir.pri)

HEADERS += doxygenplugin.h \
    doxygen_global.h \
    doxygenconstants.h \
    doxygen.h \
    doxygensettings.h \
    doxygensettingswidget.h \
    doxygensettingsstruct.h

SOURCES += doxygenplugin.cpp \
    doxygen.cpp \
    doxygensettings.cpp \
    doxygensettingswidget.cpp \
    doxygensettingsstruct.cpp

FORMS += doxygensettingswidget.ui

RESOURCES += doxygen.qrc

OTHER_FILES += Doxygen.json.in \
               doxygen_dependencies.pri \
               doxygen.png

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

DEFINES -= QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII

include(../plugins_dest_dir.pri)
message("$$TARGET destination dir: $$DESTDIR");

