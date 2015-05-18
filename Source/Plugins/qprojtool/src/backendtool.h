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
 * Filename: backendtool.h
 * Created : 24 Jan 2013
 **/


#ifndef BACKENDTOOL_H
#define BACKENDTOOL_H

#include <QStringList>
#include <QProcess>

#include "tool.h"


class QFile;
struct ConfigToolData;
class QMake;
struct Parameters;

/**
 *   \class BackEndTool
 *
 *  \brief Excecutes the command and parses its output
 *
 *  The creator receives the \ qtcPlugin parameter which indicates
 *  if the QtCreator plugin should be activated, if do
 *  .tasks file is generated with the issues and qtcreator is called
 *  opening this file, that is the way qpt acts as a qtcreator plugin.
 */
class BackEndTool : public QObject
{
    Q_OBJECT
public:
    enum Error
    {
       ErrorCouldNotWriteFile=1,
       ErrorCouldNotStartTool,
       ErrorExecutionError
    };

public:
    explicit BackEndTool(const ConfigToolData *config, const Tool::ToolInfo *toolInfo, Parameters const *param, QMake *qmake);
    virtual ~BackEndTool();
    void run();

private:
    struct StackFileParsed
    {
    public:
        StackFileParsed() {  clear(); }
        void set(const QString& f, int l=0) {file = f; line = l; }
        void clear() { file.clear();  line = 0; }
        bool isEmpty()   { return file.isEmpty();   }
    public:
        QString   file;
        int       line;
    };

private slots:
    void                  finishQtcPluginInvoke();   //remove after some time
    void                  slotProcessExecFinished(int retCode);
    void                  slotProcessExecStarted();
    void                  slotProcessHasStdout();
    void                  slotProcessHasStderr();
    void                  slotProcessIsReadingStdin();
    void                  slotProcessError(QProcess::ProcessError);

public:
    inline const QString& errorMessage()     { return m_errorMsg; }
    const QString         taskFile();
    void                  saveTaskInformation(const QString& filepath,
                                              const QStringList &messageLines,
                                              bool  error=false,
                                              int   lineNumber=0);
signals:
    void                  error(int);
    void                  finished(int);

private:
    void                  print(const QByteArray& );
    bool                  parse(const QByteArray& data);
    void                  saveStackedFileInformationInTaskFile(StackFileParsed&,
                                                               QStringList&);
    void                  invokeQtcPlugin();
    bool                  shouldIgnoreLine(const QByteArray& line);

private:
    QString               m_errorMsg;
    const ConfigToolData* m_configTool;   
    QMake          *      m_qmake;
    QFile          *      m_taskFile;        //!<  .tasks file used in Qt Creator to simulate a plugin
                                             //!<  after send it to QCreator remove it after some time
    QProcess *            m_process;         //!< process that runs the tool
    QByteArray            m_stdoutData;
    QByteArray            m_stderrData;
    int                   m_retCode;
    QString               m_toolFilePath;
    int                   m_issuesCounter;
    Parameters const    * m_params;
};

#endif // BACKENDTOOL_H
