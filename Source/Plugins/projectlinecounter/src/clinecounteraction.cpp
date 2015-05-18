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

#include "clinecounteraction.h"

#include <QCoreApplication>



const QString NAME = QString::fromUtf8("CLineCounterAction");



CLineCounterAction::CLineCounterAction(bool isProjectSelected, QObject *parent = 0):QWidgetAction(parent),
    m_label(),
    m_text(),
    mToolTip()
{
    setObjectName(NAME);
    setProjectSelected(isProjectSelected);
    m_label.setText(QChar::Nbsp + m_text + QString::fromUtf8(": ~"));
    setDefaultWidget(&m_label);    
}



void CLineCounterAction::setCounter(unsigned long counter)
{
    m_label.setText(QChar::Nbsp + m_text + QString::fromUtf8(": ") + QString::number(counter));
    m_label.setToolTip(mToolTip);
}



void CLineCounterAction::setProjectSelected(bool isProjectSelected)
{
    if (isProjectSelected) {
        m_text = QObject::trUtf8("Project lines");
        mToolTip = QObject::trUtf8("Total lines count in selected project");
    } else {
        m_text = QObject::trUtf8("Lines");
        mToolTip = QObject::trUtf8("Lines count in current file");
    }
}



CLineCounterAction* CLineCounterAction::findInstance(const QList<QAction*> &actions)
{
    foreach (QAction *act, actions)
        if (act->objectName() == NAME)
            return qobject_cast<CLineCounterAction*>(act);

    return 0;
}
