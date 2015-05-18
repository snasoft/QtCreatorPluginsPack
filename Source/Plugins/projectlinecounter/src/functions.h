/*
# **************************************************************************************
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
# **************************************************************************************
*/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QFile>
#include <QString>


unsigned long linesCount(const QString &name)
{
    const char END_LINE = '\n'; //Should work for all systems except classic Mac
    unsigned long result = 0;
    QFile file(name);
    if (file.open(QIODevice::ReadOnly)) {
        result = file.readAll().count(END_LINE) + 1;
        file.close();
    }

    return result;
}



void addLines(unsigned long &result, const unsigned long &lines)
{
    result += lines;
}

#endif // FUNCTIONS_H
