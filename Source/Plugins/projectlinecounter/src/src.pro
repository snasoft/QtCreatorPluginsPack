# ***************************************************************************************
# *
# * Project Line Counter Plugin (https://sourceforge.net/p/projectlinecounterplugin)
# * This plugin adds info string to the Qt Creator Editor toolbar that counts lines
# * in the current file and whole project if pro-file opened.
# *
# * Copyright (C) 2014 Sergey Levin <liteswamp@gmail.com>
# *
# * This program is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with this program. If not, see <http://www.gnu.org/licenses/>.
# *
# ***************************************************************************************

DEFINES += PROJECTLINECOUNTER_LIBRARY

PROVIDER = QtCreatorPluginsPack

include(../../plugins_qtc_dir.pri)

SOURCES += \
        projectlinecounterplugin.cpp \
        clinecounteraction.cpp

HEADERS += \
        projectlinecounterplugin.h \
        projectlinecounter_global.h \
        projectlinecounterconstants.h \
        clinecounteraction.h \
        functions.h

TRANSLATIONS += translation/ProjectLineCounter_ru.ts

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

include("../../plugins_dest_dir.pri")
message("$$TARGET destination dir: $$DESTDIR");
