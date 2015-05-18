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
 * Filename: qprojcommandline.h
 * Created : 1 Feb 2013
 **/


#ifndef QPROJCOMMANDLINE_H
#define QPROJCOMMANDLINE_H

#include <QObject>
#include <qcommandline.h>

class ConfigTool;

/**
  * This is flags set by options in the command line
  */
struct Parameters
{
public:
    Parameters()
    {
        m_anyListSet  = m_toolSet         = m_listSources = m_listHeaders = m_listIncludes =
        m_listDefines =  m_listQtIncludes = m_qtcPlugin   = m_listAllSources = m_returnIssues = false;
    }

    bool          m_anyListSet     : 1;
    bool          m_toolSet        : 1;
    bool          m_listSources    : 1;
    bool          m_listHeaders    : 1;
    bool          m_listAllSources : 1;
    bool          m_listDefines    : 1;
    bool          m_listIncludes   : 1;
    bool          m_listQtIncludes : 1;
    bool          m_qtcPlugin      : 1;
    QString       m_qmakePath;
    QString       m_projectFile;
    QString       m_tool;
    QString       m_qtCreatorPath;
    QString       m_userConfigFile;
    bool          m_returnIssues;
};


/**
  * Build command line options
  * Parses command lines and set the Parameters structure
  */
class QProjCommandLine : public QObject
{
    Q_OBJECT
public:
    explicit QProjCommandLine( struct Parameters &param);
    ~QProjCommandLine();

signals:
    void            error();
    void            done();

public slots:
    void parse();

private slots:
    void switchFound(const QString & name);
    void optionFound(const QString & name, const QVariant & value);
    void paramFound(const QString & name, const QVariant & value);

private:
    void  connectAll(QCommandLine *cmdParser);

private:
    struct Parameters &  m_params;
    QCommandLine      *  m_cmdLineLists;
    QCommandLine      *  m_cmdLineTool;
    ConfigTool        *  m_configFile;
};

#endif // QPROJCOMMANDLINE_H
