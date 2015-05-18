/*
 *  Copyright (c) 2013 Carlos J. Mazieri <carlos.mazieri@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Filename: manager.cpp
 * Created : 2 Feb 2013
 **/

#include "manager.h"
#include "qmake.h"
#include "tool.h"
#include "configtool.h"
#include "backendtool.h"
#include "util.h"

#include <stdio.h>
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>


Manager::Manager(Parameters &params): m_params(params)
                                     ,m_qmake(0)
                                     ,m_backEndTool(0)

{
}

Manager::~Manager()
{
    if (m_qmake)         { delete m_qmake; }
    if (m_backEndTool)   { delete m_backEndTool; }
    qDeleteAll(m_configData);
    m_configData.clear();
}

void Manager::start()
{   
    m_qmake  = new QMake(m_params.m_projectFile, m_params.m_qmakePath);
    if (!m_qmake->isOK())
    {
        if (m_qmake->qmakeFilePath().isEmpty())
        {
            showError(tr("qmake tool not found or it is not an executable"));
        }
        else
        {
           showError(tr("Could not execute qmake in the project directory, check the project file and directory permissions"));
        }
        return;
    }

    if (m_params.m_anyListSet)
    {
        processLists();
        emit done();
    }
    else if (m_params.m_toolSet)
    {
        processTool();
    }   
}


void  Manager::showError(const QString& msg)
{
    fprintf(stderr, "\n\tERROR: %s\n", msg.toLatin1().constData());
    emit error();
}


void Manager::processLists()
{
    if (m_params.m_listAllSources)      { printList(m_qmake->allSources()); }
    else
    {
        if (m_params.m_listSources)     { printList(m_qmake->sources()); }
        if (m_params.m_listHeaders)     { printList(m_qmake->headers()); }
    }
    if (m_params.m_listDefines)         { printList(m_qmake->defines()); }
    if (m_params.m_listIncludes)        { printList(m_qmake->includesDir()); }
    if (m_params.m_listQtIncludes)      { printList(m_qmake->qtIncludesDir()); }
    fprintf(stdout, "\n");
}



void Manager::printList(const QStringList &l)
{
    static bool alreadyPrintedAnyList = false;
    if (alreadyPrintedAnyList)
    {
      fprintf(stdout, "\n");
    }
    for (int counter=0; counter < l.count(); counter++)
    {
        fprintf(stdout, "\n%s", l.at(counter).toLatin1().constData() );
    }
    alreadyPrintedAnyList = true;
}


void Manager::processTool()
{
    Tool toolInfo(m_params.m_tool);
    if (toolInfo.count() == 0) {
        QString msg(tr("Tool ") + m_params.m_tool + QString(tr(" not found")));
        showError(msg);
        return;
    }
    //discover which tool we are talking about
    QString tname = QFileInfo(toolInfo.get(0)->filePath).fileName();
    ConfigTool *configTool = ConfigTool::getInstance();
    m_configData   = configTool->toolsData();
    ConfigToolData *currentToolConfig = 0;
    for (int counter=0; counter < m_configData.count(); counter++)
    {
        ConfigToolData *t = m_configData.at(counter);
        //example people can use another name as: cppheck-new, cppcheck_12, etc, but it has cppcheck
        if (tname.contains(t->toolName, Qt::CaseInsensitive))
        {
              currentToolConfig = t;
              break;
        }
    }

    if (!currentToolConfig)
    {
        QString msg (tr("Tool ") + m_params.m_tool + QString(tr(" is not configured to run, see config file")));
        showError(msg);
        return;
    }

    const Tool::ToolInfo * rightToolInfo = 0;
    if (!currentToolConfig->minimalVersion.isEmpty())
    {       
        rightToolInfo = toolInfo.getMinimalVersion(currentToolConfig->minimalVersion.trimmed());
        if (!rightToolInfo)
        {
            QString msg (tr("Tool ") + m_params.m_tool +
                         QString(tr(" requires a minimal version ") + currentToolConfig->minimalVersion ));
            showError(msg);
            return;
        }

    }
    if (!rightToolInfo)
    {
         rightToolInfo = toolInfo.getGreaterVersion();
    }


    m_backEndTool = new BackEndTool(currentToolConfig, rightToolInfo, &m_params, m_qmake);
    connect(m_backEndTool, SIGNAL(error(int)),   this, SLOT(toolFailed(int)) );
    connect(m_backEndTool, SIGNAL(finished(int)),this, SLOT(toolFinished(int)) );
    m_backEndTool->run();
}


void Manager::toolFailed(int)
{
    QString msg (tr("Tool ") + m_params.m_tool +
                 tr(" has failed"));
    showError(msg);
}

void Manager::toolFinished(int retCode)
{
    QString msg = QString(tr("finished with return code %1")).arg(retCode);
    Util::info(msg);
    QCoreApplication::exit(retCode);
}
