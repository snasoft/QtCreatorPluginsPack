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
 * Filename: main.cpp
 * Created : 24 Jan 2013
 **/
 
 
#include "backendtool.h"
#include "configtool.h"
#include "qmake.h"
#include "util.h"
#include "qprojcommandline.h"

#include <QTemporaryFile>
#include <QProcess>
#include <QDebug>
#include <QCoreApplication>
#include <QTimer>
#include <QDir>
#include <QTime>
#include <sys/types.h>


BackEndTool::BackEndTool(const ConfigToolData *config,
                         const Tool::ToolInfo *toolInfo,
                         const Parameters *param,
                         QMake *qmake) :
    QObject()
  ,m_configTool(config) 
  ,m_qmake(qmake)
  ,m_taskFile(0)
  ,m_process(0)
  ,m_retCode(0)
  ,m_issuesCounter(0)
  ,m_params(param)
{
    m_toolFilePath = toolInfo->filePath;
}


BackEndTool::~BackEndTool()
{
    if (m_taskFile)    { m_taskFile->remove(); delete m_taskFile; }
    if (m_process)
    {
        if (m_process->state() == QProcess::Running)  {
            m_process->terminate();
            m_process->kill();
        }
        delete m_process;
    }
}


const QString  BackEndTool::taskFile()
{
    return (m_taskFile) ? Util::quoteWhenContainSpaces(m_taskFile->fileName())
                        : QString();
}


void BackEndTool::finishQtcPluginInvoke()
{
    if (m_taskFile)
    {
        m_taskFile->remove();
        delete m_taskFile;
        m_taskFile = 0;
    }
    if (m_params->m_returnIssues) {
        emit finished(m_issuesCounter);
    }
    else {
        emit finished(m_retCode);
    }
}

//TODO check write error
void BackEndTool::saveTaskInformation(const QString& filepath, const QStringList& messageLines,  bool error, int lineNumber)
{
    Q_UNUSED(error); // always warn
    QString line(filepath);
    line += QLatin1Char('\t') + QString::number(lineNumber);
    line += QLatin1Char('\t') + QLatin1String("warn");
    if (messageLines.count() > 0)
    {
        line += QLatin1Char('\t');
        if (messageLines.count() > 1)
        {
            line += messageLines.join("\n");
        }
        else
        {
            line += messageLines.at(0);
        }
    }
    line += QLatin1Char(0x0a); // always use LF (QtCreator misinterprets CR LF and displays a '♪' (eighth note / quaver symbol))
    m_taskFile->write(line.toLatin1());
    ++m_issuesCounter;
}

void BackEndTool::run()
{
    QString cmd(m_toolFilePath);

#if defined(Q_OS_UNIX) || defined(Q_OS_WIN)
    if (m_toolFilePath.contains(QLatin1String("cppcheck"), Qt::CaseInsensitive))
    {
         cmd += QLatin1String(" --platform=");
#if defined(Q_OS_WIN)
         cmd += QLatin1String("win");
#else
         cmd += QLatin1String("unix");
#endif
         cmd += QString::number(QT_POINTER_SIZE * 8);
#if defined(Q_OS_WIN)
         if(QSysInfo::WindowsVersion & QSysInfo::WV_NT_based)
             cmd += QLatin1Char('W');   // NT-Based Win
         else
             cmd += QLatin1Char('A');   // DOS-based Win
#endif
    }
#endif

    if (m_configTool->addQtIncludes)
    {
        QStringList inc = m_qmake->qtIncludesDir();
        for (int counter=0; counter < inc.count(); counter++)
        {
            cmd += QLatin1String(" -I ") + inc.at(counter);
        }
    }
    if (m_configTool->addProjectIncludes)
    {
        QStringList inc = m_qmake->includesDir();
        for (int counter=0; counter < inc.count(); counter++)
        {
            cmd += QLatin1String(" -I ") + inc.at(counter);
        }
    }
    if (m_configTool->addProjectDefines)
    {
        QStringList def = m_qmake->defines();
        for (int counter=0; counter < def.count(); counter++)
        {
            cmd += QLatin1String(" -D") + def.at(counter);
        }
    }
    cmd += QLatin1Char(' ') + m_configTool->cmdLine;
    if (m_configTool->input != ConfigToolData::CommandLineArgs)
    {
        QString temp = m_qmake->tempSourcesListFilename(m_configTool->useSources,
                                                        m_configTool->useHeaders);
        QString fileContent = Util::quoteWhenContainSpaces(temp);

        //Stdin will be used later when the tool starts reading stdin
        if (m_configTool->input != ConfigToolData::Stdin)
        {
            if (m_configTool->input == ConfigToolData::FileContentAfterSpace)
            {
                cmd += QLatin1Char(' ');
            }
            cmd += fileContent;
        }
    }
    else // build input files as arguments
    {
        QStringList files = m_qmake->makeList(m_configTool->useSources,
                                              m_configTool->useHeaders);
        for (int counter=0; counter < files.count(); counter++)
        {
            cmd += QLatin1Char(' ');
            cmd += files.at(counter);
        }
    }

    Util::info(cmd, QLatin1String("running"));

    m_process = new QProcess();
    connect(m_process, SIGNAL(started()),                  this, SLOT(slotProcessExecStarted()));
    connect(m_process, SIGNAL(readyReadStandardOutput()),  this, SLOT(slotProcessHasStdout()));
    connect(m_process, SIGNAL(readyReadStandardError()),   this, SLOT(slotProcessHasStderr()));
    //connect(m_process, SIGNAL(readyRead()),                this, SLOT(slotProcessIsReadingStdin()));
    connect(m_process, SIGNAL(finished(int)),              this, SLOT(slotProcessExecFinished(int)));
    //connect(m_process, SIGNAL(error(QProcess::ProcessError)),
    //        this, SLOT(slotProcessError(QProcess::ProcessError)));                                                                            )));

    m_process->start(cmd);
}



void BackEndTool::slotProcessExecStarted()
{
    slotProcessIsReadingStdin();
}

void BackEndTool::slotProcessIsReadingStdin()
{
     if (m_configTool->input == ConfigToolData::Stdin)
     {
         QFile input(m_qmake->tempSourcesListFilename());
         QByteArray line;       
         if (input.open(QFile::ReadOnly))
         {
             line = input.readLine();
             while (line.size() > 0)
             {
                 m_process->write(line);
                 m_process->waitForBytesWritten();
                 line = input.readLine();
             }
             input.close();
             m_process->closeWriteChannel();
         }

     }
}


void BackEndTool::slotProcessExecFinished(int retCode)
{
    m_retCode = retCode;   
    QString msg(m_toolFilePath);
    msg += QString(tr(" finished with return code %1 ")).arg(retCode);
    Util::info(msg);
    if (m_params->m_qtcPlugin || m_params->m_returnIssues)
    {
        bool ok = false;
        if (m_configTool->output == ConfigToolData::Stderr) {
            ok =  parse(m_stderrData);
        }
        else {
            ok =  parse(m_stdoutData);
        }
        if (ok)
        {           
            if (m_params->m_qtcPlugin) {
                invokeQtcPlugin();
            }
        }
        else
        {
            fprintf(stdout, "%s\n",
                    QString(tr("No issues found ")).toLatin1().constData());

        }
        QTimer::singleShot(800, this, SLOT(finishQtcPluginInvoke()));
    }
    else
    {
        emit finished(retCode);
    }
}

void BackEndTool::slotProcessHasStdout()
{
    QByteArray temp = m_process->readAllStandardOutput();
    print(temp);
    m_stdoutData += temp;
}

void BackEndTool::slotProcessHasStderr()
{
    QByteArray temp = m_process->readAllStandardError();
    print(temp);
    m_stderrData += temp;
}


void BackEndTool::slotProcessError(QProcess::ProcessError error)
{
    Q_UNUSED(error);
}

void BackEndTool::print(const QByteArray &p)
{
     fprintf(stdout, "%s" ,p.constData());
     fflush(stdout);
}


bool BackEndTool::parse(const QByteArray & data)
{
    m_taskFile       = new QFile(QDir::tempPath() + QDir::separator() +
                                 QCoreApplication::applicationName()  +
                                 QTime::currentTime().toString("hhmmsszzz") +
                                 QString::number(QCoreApplication::applicationPid()) +
                                 QLatin1String(".tasks"));
    StackFileParsed  file;  
    bool ret = m_taskFile->open(QFile::WriteOnly | QFile::Truncate /*| QFile::Text*/); // QFile::Text-flag removed: Creates a 'CR LF' under Windows which is misinterpreted by QtCreator
    if (ret)
    {
        QStringList message;        
        QList<QByteArray>  lines = data.split('\n');
        for(int counter=0; counter < lines.count(); counter++)
        {
            QByteArray l = lines.at(counter).simplified();           
            if (l.isEmpty() || (m_configTool->ignoreLinesWith.count() > 0 && shouldIgnoreLine(l)) )
            {
                continue;
            }
            if ((l.startsWith('\"') || l.startsWith('\'')) &&
                (l.endsWith('\"') || l.endsWith('\''))
               )
            {
                l.remove(l.size()-1, 1);
                l.remove(0,1);
            }

#if defined(Q_OS_UNIX) || defined(Q_OS_WIN)
            if( l[1]==':' && (l[2]=='\\' || l[2]=='/') ) // in case Windows-Filenames start with: "c:\" or "c:/" (Qt can handle both)
                l[1]='#'; // temporarely replace ':' with '#' (':' is the split-separator)
#endif
            l.replace("::", "@");       // some messages contain double colon: 'C:\test.h:56:Technically the member function 'Class::function' can be const.'
                                                                                                // (don't use :: to split)

            QList<QByteArray> fields = l.split(':');
            if (fields.count() == 3  || // --template '{file}:{line}:{message}'
                fields.count() == 4     // --template '{file}:{line}:{id}:{message}'
                )
            {
#if defined(Q_OS_UNIX) || defined(Q_OS_WIN)
                if( fields[0][1]=='#' )
                    fields[0][1]=':';
#endif
                fields[2].replace("@", "::");

                int startMessageField = fields.count() -1 ;
                bool ok=false;
                int lineNumber = fields[1].toInt(&ok);
                {
                    if (!ok)
                    {
                        lineNumber=0;
                        startMessageField = 1;
                    }
                }
                //if no line is found, the content is considered a comment
                if (lineNumber > 0 )
                {
                    saveStackedFileInformationInTaskFile(file, message);
                    file.file = fields[0];
                    file.line = lineNumber;
                }
                for(; startMessageField < fields.count(); startMessageField++)
                {
                    message.append(fields[startMessageField]);
                }
            }
            else
            {
                l.replace("@", "::");
                message.append(QString(l));
            }
        }//end for all lines
        saveStackedFileInformationInTaskFile(file, message);
        m_taskFile->close();
        ret = (m_taskFile->size() > 0);
    }
#if DEBUG
    qDebug() << taskFile() << "ret" << ret;
#endif
    return ret;
}


void
BackEndTool::saveStackedFileInformationInTaskFile(StackFileParsed& sfile,
                                                  QStringList& message)
{
    if (!sfile.isEmpty())
    {
        saveTaskInformation(sfile.file, message, false, sfile.line);
        sfile.clear();
        message.clear();
    }
}


void BackEndTool::invokeQtcPlugin()
{
    QString qtcreator;
    QString qsQtCreatorPath(m_params->m_qtCreatorPath);
#if defined(Q_OS_UNIX)
    // try to get the qtcreator real parent process
    if( qsQtCreatorPath.isEmpty() )
    {
        Q_PID   ppid = 0;     //parent process id
        ppid = ::getppid();
#if defined(Q_OS_LINUX)
        //try to get argv[0] from qtcreator
        QFile proc("/proc/" + QString::number(ppid) + QDir::separator() + QLatin1String("cmdline"));
        if (proc.open(QFile::ReadOnly))
        {
            QByteArray tmp_qtcreator(proc.readAll().constData());
            if (tmp_qtcreator.endsWith("qtcreator"))
            {
                qtcreator = tmp_qtcreator;
                qtcreator += QLatin1String(" -client ");
            }
        }
    }
#endif // linux
#endif // unix

    if (qtcreator.isEmpty())
    {
        if( qsQtCreatorPath.isEmpty() )
            qsQtCreatorPath = "qtcreator";

        Tool creatorExe(qsQtCreatorPath, false);
        const Tool::ToolInfo *creatorInfo = creatorExe.getGreaterVersion();
#if defined(Q_OS_MSDOS) || defined(Q_OS_WIN)
     if (!creatorInfo)
     {
         creatorExe.searchQtForWinExtraPaths();
         creatorInfo = creatorExe.get(0);
     }
#endif
        if (!creatorInfo || creatorInfo->filePath.isEmpty())
        {
            fprintf(stderr, "\nqtcreator executable not found\n");
            emit finished(-1);
            return;
        }
        Util::info(creatorInfo->filePath, QLatin1String("using"));
        qtcreator = creatorInfo->filePath + QLatin1String(" -client") ;
    }

    qtcreator += QChar(' ') + taskFile();

#if DEBUG
    qDebug() << "plugin" << qtcreator;
#endif

    if ( QProcess::startDetached(qtcreator) == false )
    {
        printf("\nError: Unable to start \"%s\" for output into Issues pane. Check path to QtCreator or command line options.\n", qPrintable(qtcreator));
    }
}


bool BackEndTool::shouldIgnoreLine(const QByteArray& line)
{
     bool ret = false;
     for(int counter=0; counter < m_configTool->ignoreLinesWith.count(); counter++)
     {
         QByteArray string = m_configTool->ignoreLinesWith.at(counter).toLatin1();       
         if (line.contains(string))
         {
             ret = true;            
             break;
         }
     }
     return ret;
}
