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

#ifndef CLINECOUNTERACTION_H
#define CLINECOUNTERACTION_H

#include <QWidgetAction>
#include <QLabel>



class CLineCounterAction: public QWidgetAction {
    Q_OBJECT

    QLabel m_label;
    QString m_text;
    QString mToolTip;

public:
    CLineCounterAction(bool isProjectSelected, QObject *parent);
    void setCounter(unsigned long counter);
    void setProjectSelected(bool isProjectSelected);
    static CLineCounterAction* findInstance(const QList<QAction*> &actions);
};

#endif // CLINECOUNTERACTION_H
