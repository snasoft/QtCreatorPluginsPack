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
 * Filename: tool.cpp
 * Created : 26 Jan 2013
 **/


#include "tool.h"
#include "util.h"

#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QStringList>
#include <QDirIterator>
#include <QDebug>

#define TIME_TO_WAIT_FOR_STARTING_APP 150
#define TIME_TO_WAIT_FOR_KILLING_APP  100


#if defined(Q_OS_MSDOS) || defined(Q_OS_WIN)
QStringList  Tool::m_qtBinDirs;
#endif

Tool::ToolInfo::ToolInfo(const QString& binary) :
    filePath(binary), version(QLatin1String("0.0"))
{

}


Tool::Tool(const QString &paramfullpathOrName, bool checkVersion) : m_checkVersion(checkVersion)
{
    QString fullpathOrName(paramfullpathOrName);
    QFileInfo pathInfo(fullpathOrName);
    if (pathInfo.isAbsolute())
    {
        m_name = pathInfo.fileName();
        createToolInfo(fullpathOrName); //full path name of the executable
    }
    else
    {   // name only, search in the PATH variable
        m_name = fullpathOrName;
        searchInPathVariable();
#if defined(Q_OS_MSDOS) || defined(Q_OS_WIN)      
        QByteArray programFiles = qgetenv("PROGRAMFILES");
        if (!programFiles.isEmpty())
        {
            QString executableFilter(m_name  + QChar('*'));
            QString pfpath(programFiles);
            QDir pfDir (pfpath, executableFilter,
                        QDir::DirsFirst | QDir::IgnoreCase,
                        QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
            int counter = pfDir.count();
            QStringList extraPaths;
            while (counter--) // using reverse order should bring last versions first
            {
                extraPaths.append(pfpath + QDir::separator() + pfDir[counter]);
            }
            searchExtraPaths(extraPaths);
        }
#endif
    }

#if DEBUG
    qDebug() << "Tool::Tool(): tools items=" << m_tools.count();
    for(int i=0; i < m_tools.count(); i++)
    {
        const ToolInfo *qmakeInfo = m_tools.at(i);
        qDebug() << "Tool::Tool(): version" << qmakeInfo->version << "path" << qmakeInfo->filePath;
    }
#endif
}


Tool::Tool(const QString& name, const QStringList& extraPaths, bool checkVersion): m_name(name),
                                                                                   m_checkVersion(checkVersion)
{
     searchInPathVariable();
     searchExtraPaths(extraPaths);
}


Tool::~Tool()
{
    qDeleteAll(m_tools);
    m_tools.clear();
}


void Tool::searchInPathVariable()
{
    QByteArray  path = qgetenv("PATH");
    if (path.size() > 0 )
    {
#if defined(Q_OS_MSDOS) || defined(Q_OS_WIN)
        char separator = ';';
#else
        char separator = ':';  // Q_OS_UNIX
#endif
        QList<QByteArray>  dirs(path.split(separator));
        for (int counter = 0; counter < dirs.count(); counter++)
        {
            createToolInfo(m_name, dirs[counter]);
        }
    }
}


/*!
 * \brief Tool::createToolInfo()   creates ToolInfo if it not exist yet for the same qmake tool
 * \param path  path where qmake can live
 * \return pointer to an existent or created ToolInfo, a null pointer indicates that no qmake tool exists in \a path
 */
Tool::ToolInfo * Tool::createToolInfo(const QString&name, const QString &path)
{
    Tool::ToolInfo *ret = 0 ;
    QString fileName;    
    if (!path.isNull() && !path.isEmpty())
    {
        fileName = path + QDir::separator();
    }
    fileName += name;
    QFileInfo tInfo(fileName);
#if defined(Q_OS_MSDOS) || defined(Q_OS_WIN)
    if (!tInfo.exists() || !tInfo.isExecutable())
    {
        QLatin1String exe(".exe");
        if (!fileName.endsWith(exe, Qt::CaseInsensitive))
        {
            fileName += exe;
            tInfo.setFile(fileName);
        }
    }
#endif
    QString executable;
    if (tInfo.exists() && tInfo.isExecutable())
    {
        executable = Util::quoteWhenContainSpaces(tInfo.absoluteFilePath());
        int tools = m_tools.count();
        Tool::ToolInfo *exists = 0;
        while( tools--)
        {
            exists = m_tools.at(tools);
            if (exists->filePath == executable)
            {
                ret = exists;
                break;
            }
        }
        if (!ret) // it does not exist yet
        {
            ret = new Tool::ToolInfo(executable);
            m_tools.append(ret);           
            extractVersion(ret);
        }// it does not exist yet
    }
    return ret;
}


const Tool::ToolInfo * Tool::get(int index) const
{
    Tool::ToolInfo * ret = 0;
    if (index >= 0 && index < m_tools.size())
    {
       ret = m_tools.at(index);
    }
    return ret;
}


const Tool::ToolInfo * Tool::getGreaterVersion() const
{
    Tool::ToolInfo *ret   = 0;
    Tool::ToolInfo *info  = 0;
    QString version("0.0");
    int counter = m_tools.count();
    while (counter--)
    {
         info = m_tools.at(counter);
         if (info->version >= version)
         {
             version = info->version;
             ret     = info;
         }
    }
    return ret;
}

/*!
 * \brief Tool::geMinimalVersion()
 * \param version
 * \return a ToolInfo from a tool that matches the verion (greater or equal) or null otherwise
 */
const Tool::ToolInfo * Tool::getMinimalVersion(const QString& version) const
{
   const Tool::ToolInfo *ret = this->getGreaterVersion();
   if (ret && !ret->version.isEmpty() && ret->version < version)
   {
       ret = 0;
   }
   return ret;
}

/*!
 * \brief Tool::searchByEnvVar() verifies if a executable asigned by environment variable
 *
 *  \a m_name already contains a name of the executable to search
 *
 * \param envVAR        the variable
 * \param dirBehindVAR  any if exist behind this variable
 * \return a ToolInfo or Null
 *
 * Example:
 * \code
 *     m_name = "qmake"
 *     Tool::ToolInfo * qmakeInfo = Tool::searchByEnvVar("QTDIR", "bin");
 *     //returns the information if QTDIR is defined and exist $QTDIR/bin/qmake
 * \endcode
 */
const Tool::ToolInfo * Tool::searchByEnvVar(const char *envVAR, const char *dirBehindVAR)
{
     Tool::ToolInfo *tool = 0;
     QByteArray  vardir = qgetenv(envVAR);
     if (vardir.size() > 0 )
     {
         if (dirBehindVAR)
         {
             vardir += QDir::separator().toLatin1() + QByteArray(dirBehindVAR);
         }
         tool =  createToolInfo(m_name, vardir);
     }
     return tool;
}


void Tool::searchExtraPaths(const QStringList &extraPaths)
{
    for (int counter = 0; counter < extraPaths.count(); counter++)
    {
        createToolInfo(m_name, extraPaths[counter]);
    }
}

void Tool::extractVersion(Tool::ToolInfo *toolInfo) const
{
    QString version("0.0");
    if (m_checkVersion)
    {
        QProcess proc;
        proc.setProcessChannelMode(QProcess::MergedChannels);
        proc.start(toolInfo->filePath + " --version");
        proc.waitForStarted(TIME_TO_WAIT_FOR_STARTING_APP);
        bool hasVersion = proc.waitForFinished(TIME_TO_WAIT_FOR_KILLING_APP);
        if (!hasVersion)
        {
            if (proc.state() == QProcess::Running)
            {
                proc.terminate();
                if (!proc.waitForFinished(TIME_TO_WAIT_FOR_KILLING_APP))
                {
                    proc.kill();
                    proc.waitForFinished(TIME_TO_WAIT_FOR_KILLING_APP);
                }
            }
            proc.start(toolInfo->filePath + " -version");
            proc.waitForStarted(TIME_TO_WAIT_FOR_STARTING_APP);
            hasVersion = proc.waitForFinished(TIME_TO_WAIT_FOR_KILLING_APP);
            if (proc.state() == QProcess::Running)
            {
                proc.terminate();
                if (!proc.waitForFinished(TIME_TO_WAIT_FOR_KILLING_APP))
                {
                    proc.kill();
                    proc.waitForFinished(TIME_TO_WAIT_FOR_KILLING_APP);
                }
            }
        }
        if (hasVersion)
        {
            QByteArray line(proc.readLine().trimmed());
            //avoid one or two empty lines
            for (int counter = 0; line.size() == 0 && counter < 4; ++counter)
            {
                line = proc.readLine().trimmed();
            }
            if ( line.size() > 0 )
            {
                QList<QByteArray> words = line.split(' ');
                for (int counter = 0 ; counter < words.count(); counter++)
                {
                    QByteArray w = words.at(counter);
                    if (w.at(0) >= '0' && w.at(0) <= '9')
                    {
                        for (int c=0; c < w.size(); c++)
                        {
                            char letter = w.at(c);
                            if ( letter != '.' && (letter < '0' || letter > '9') )
                            {
                                w.truncate(c);
                                break;
                            }
                        }
                        version = w;
                        break; // words loop
                    }
                }
            }
        }//proc.waitForFinished())
    }
    toolInfo->version = version;
}


void  Tool::searchFromDir(const QString& startDir, const QString& dir )
{   
    searchExtraPaths(findDir(startDir,dir));
}


QStringList Tool::findDir(const QString& startDir, const QString& dir )

{
    QStringList filters(dir);
    QStringList extraPaths;
    QDirIterator * iterator = (dir.isNull() || dir.isEmpty()) ?
                new QDirIterator(startDir, QDir::Dirs | QDir::NoDotAndDotDot,QDirIterator::Subdirectories ) :
                new QDirIterator(startDir, filters, QDir::Dirs | QDir::NoDotAndDotDot,QDirIterator::Subdirectories );

    while (iterator->hasNext())
    {
       extraPaths.append( iterator->next() );
    }
    delete iterator;
    return extraPaths;
}


#if defined(Q_OS_MSDOS) || defined(Q_OS_WIN)
void  Tool::searchQtForWinExtraPaths()
{
   if (m_qtBinDirs.isEmpty())
   {
      QFileInfoList drivesList = QDir::drives();
      int counter = drivesList.count();
      QString qtAscii("Qt");
      QString qtTranslated(QObject::tr("Qt"));
      while (counter--)
      {
          QFileInfo qtdir(drivesList.at(counter).absoluteFilePath() +
                          QDir::separator() +
                          qtAscii
                         );
          if (!qtdir.exists() || !qtdir.isDir())
          {
             qtdir.setFile( drivesList.at(counter).absoluteFilePath() +
                            QDir::separator() +
                            qtTranslated
                          );
          }
          if (qtdir.exists() && qtdir.isDir())
          {
              m_qtBinDirs += findDir(qtdir.absoluteFilePath(), QLatin1String("bin"));
          }
      }
   }
   searchExtraPaths(m_qtBinDirs);
}
#endif
