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
 * Filename: qmake.cpp
 * Created : 26 Jan 2013
 **/

#include "qmake.h"
#include "util.h"
#include "tool.h"

#include <QTemporaryFile>
#include <QFileInfo>
#include <QHash>
#include <QStringRef>
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QMap>



/*!
 * \brief The StringDict is a simple class to store unique strings
 *
 *  Filenames are always stored using full path using \ref insertFile() even just the name was passed.
 *
 *  No delete is provided, so once an item is taken out by using QMap<T>::take() or Qmap<T>::remove(),
 *    the index from others are lost.
 */
class StringDict:  public QMap<QString, int>
{
public:
    StringDict(): m_index(0) {}
    QStringList list();
    void insert(const QString& key);
    void insertFile(const QString& key, const QString &curDir=QLatin1String(0));
    int  indexOf(const QString& key);
private:
    int   m_index;
};


/*!
 * \brief StringDict::insert() inserts a common string if still does not exist in the list
 * \param key the string itself
 */
void StringDict::insert(const QString &key)
{
    if(!key.isEmpty() && !contains(key))
    {
        QMap<QString, int>::insert(key, ++m_index);
    }
}

/*!
 * \brief StringDict::indexOf() index of the an item pointed by \a key
 * \param key the string itself
 * \return the index or -1 if the item pointed by \a key does not exist
 */
int  StringDict::indexOf(const QString& key)
{
    int ret = -1;
    QMap<QString, int>::const_iterator i = find(key);
    if (i != end())
    {
        ret =  i.value();
    }
    return ret;
}

/*!
 * \brief StringDict::insertFile() the same as \ref insert() but the key is file.
 *
 *  It assures the full pathname is stored.
 *
 * \param key the filename full path or just the name
 */
void StringDict::insertFile(const QString &key, const QString& curDir)
{
    QFileInfo   file(key);
    QString     fullname;
    if (file.isFile() || file.isDir())
    {
        fullname = file.absoluteFilePath();
    }
    else   if (!curDir.isNull() && !curDir.isEmpty())
    {
        QFileInfo full(curDir + QDir::separator() + key);
        if (full.isFile() || full.isDir())
        {
            fullname = full.absoluteFilePath();
        }
    }
    this->insert( Util::quoteWhenContainSpaces(fullname));
}


QStringList StringDict::list()
{
#if 1
    return QStringList(keys());
#else
    QStringList ret;
    const_iterator iterator = begin();
    while (iterator != end())
    {
        ret.append(iterator.key());
        iterator++;
    }
    return ret;
#endif
}

/*!
 * \brief QMake::QMake()
 * \param projectFile    [in]       Qt project file
 * \param qmakeFilePath  [optional] the full path of a preferred "qmake tool".
 *
 *  If no preferred "qmake tool" in \a qmakeFilePath is passed,  $QTDIR/bin/qmake  is used if QTDIR environment variable is set
 *  If QTDIR environment variable is not set, it searches in the PATH variable and uses the higer version found.
 */
QMake::QMake(const QString& projectFile, const QString& qmakeFilePath):
    m_listSources(new StringDict())
   ,m_listHeaders(new StringDict())
   ,m_listIncludes(new StringDict())
   ,m_listDefines(new StringDict())
   ,m_qmakeFilePath(qmakeFilePath)
   ,m_projectFilePath(projectFile)
   ,m_temporaryProjectFile(0)
   ,m_sourcesListFile(0)
   ,m_done(false)
{
    //make sure qmake parameter exists and it is executable
    if (!qmakeFilePath.isEmpty())
    {
        Tool *tool = new Tool(qmakeFilePath, false);
        const Tool::ToolInfo *qmakeInfo = tool->get(0);
        if (qmakeInfo)
        {
            m_qmakeFilePath = qmakeInfo->filePath;
        }
        else
        {
           m_qmakeFilePath.clear();
        }
    }
    if (m_qmakeFilePath.isEmpty())
    {
        m_qmakeFilePath = searchQmakePath();
    }
    if (!m_qmakeFilePath.isEmpty())
    {
        Util::info(m_qmakeFilePath, QLatin1String("using"));
        buildQtIncludeDirs();
        if (handleProjectFile())
        {
            m_done = execQmake();
        }
    }
}


QMake::~QMake()
{
    if (m_temporaryProjectFile)
    {
        delete m_temporaryProjectFile;
    }
    if ( m_sourcesListFile )
    {
        delete m_sourcesListFile;
    }
    delete   m_listSources;
    delete   m_listHeaders;
    delete   m_listIncludes;
    delete   m_listDefines;
}


//=============================================================
/*!
 * \brief QMake::sources returns the Project sources list
 * \return the Project sources list
 */
QStringList  QMake::sources()
{
    return m_listSources->list();
}


//=============================================================
/*!
 * \brief QMake::defines returns the Project defines list
 * \return the Project defines list
 */
QStringList  QMake::defines()
{
    return m_listDefines->list();
}


//=============================================================
/*!
 * \brief QMake::headers returns the Project header files
 * \return the Project header files
 */
QStringList  QMake::headers()
{
    return m_listHeaders->list();
}

//=============================================================
/*!
 * \brief  QMake::allSources return the Sources plus Headers list
 * \return the Sources plus Headers list
 *  \sa \ref sources() \ref headers()
 */
QStringList  QMake::allSources()
{
    QStringList tmp(m_listSources->list());
    tmp += m_listHeaders->list();
    StringDict  all;
    for(int counter=0;counter < tmp.count(); counter++)
    {
        all.insert(tmp.at(counter));
    }
    return all.list();
}

//=============================================================
/*!
 * \brief QMake::includesDir returns the Project include directories
 * \return Project include directories
 */
QStringList  QMake::includesDir()
{
    QStringList ret = m_listIncludes->list();
    if (ret.isEmpty() && m_done )
    {
        ret.append(".");
    }
    return ret;
}

//=============================================================
/*!
 * \brief QMake::qtIncludesDir returns the Qt include directories
 * \return the Qt include directories
 */
QStringList  QMake::qtIncludesDir()
{
    return m_listQtIncludes;
}


//=============================================================
/*!
 * \brief QMake::searchQmakePath searches for qmake tool in the path and using QTDIR environment variable
 *    The QTDIR variable is preferred against PATH variable
 *
 * \return the filepath of the qmake executable or an empty string if not found
 * \sa QMakeTool class
 */
QString QMake::searchQmakePath()
{
    QString  qmakePath;
    Tool *tool = new Tool("qmake");
    const Tool::ToolInfo *qmakeInfo = tool->searchByEnvVar("QTDIR", "bin");
#if defined(Q_OS_MSDOS) || defined(Q_OS_WIN)
     if (!qmakeInfo)
     {
         tool->searchQtForWinExtraPaths();
     }
#endif
    if (!qmakeInfo)
    {
        qmakeInfo = tool->getGreaterVersion();
    }
    if (qmakeInfo)
    {
        qmakePath = qmakeInfo->filePath;
    }
    delete tool;
    return qmakePath;
}



/*!
 * \brief QMake::buildQtIncludeDirs create a list of Qt Includes Dirs
 *
 *   The list is stores in \a m_listQtIncludes
 */
void QMake::buildQtIncludeDirs()
{
    QFileInfo qmake(m_qmakeFilePath);
    QDir qtdir(qmake.absoluteDir());
    if (qtdir.cd("../include"))
    {
        QStringList list = qtdir.entryList(QDir::AllDirs |
                                           QDir::NoSymLinks |
                                           QDir::NoDotAndDotDot);
        for (int counter=0; counter < list.count(); counter++)
        {
            m_listQtIncludes.append(qtdir.absolutePath() + QDir::separator() + list[counter]);
        }
        m_listQtIncludes.append(qtdir.absolutePath());
    }
}


/*!
 * \brief QMake::handleProjectFile  creates another project File and appends some extra stuff
 * \return true if could create another one, false otherwise
 */
bool QMake::handleProjectFile()
{
    const QByteArray qmakeProjectInfo =
            "# this block is generated to get a list of sources \n"
            "QMakePrint_SOURCES {\n"
            "for(file, SOURCES): message(SOURCES:$${file})\n"
            "}\n\n"
            "# this block is generated to get a list of headers \n"
            "QMakePrint_HEADERS {\n"
            "for(file, HEADERS): message(HEADERS:$${file})\n"
            "}\n\n"
            "# this block is generated to get a list of defines \n"
            "QMakePrint_DEFINES {\n"
            "for(define, DEFINES): message(DEFINES:$${define})\n"
            "}\n\n"
            "# this block is generated to get a list of includes \n"
            "QMakePrint_INCPATH {\n"
            "for(path, INCLUDEPATH): message(INCPATH:$${path})\n"
            "}\n\n"
            ;
    bool ret = false;
    QFileInfo project(m_projectFilePath);
    if (project.exists() &&  project.isReadable() && project.size() > 0)
    {
        if (QFileInfo(project.absolutePath()).isWritable())
        {
            QFile in(m_projectFilePath);
            m_temporaryProjectFile = new QTemporaryFile(project.absolutePath() + QDir::separator());
            if (in.open(QFile::ReadOnly) && m_temporaryProjectFile->open())
            {
#if DEBUG
                qDebug() << "filename()" << m_temporaryProjectFile->fileName();
#endif
                if (m_temporaryProjectFile->write(in.readAll()) == project.size())
                {
                   m_temporaryProjectFile->write(qmakeProjectInfo);
                   ret = true;
                }
                m_temporaryProjectFile->close();
                in.close();
            }
        }
    }
    return ret;
}




bool QMake::execQmake()
{
    bool ret = false;
    QLatin1String config(" \"CONFIG += QMakePrint_SOURCES QMakePrint_HEADERS QMakePrint_DEFINES QMakePrint_INCPATH\" ");
    QString cmd(m_qmakeFilePath + config +  m_temporaryProjectFile->fileName() );
    QTemporaryFile output;
    output.open();
    output.close();
    cmd += QLatin1String(" -o ") + output.fileName();
    QProcess proc;
    proc.setWorkingDirectory(QFileInfo(m_projectFilePath).absolutePath());
#if DEBUG
    qDebug() << "cmd" << cmd;
#endif
    proc.start(cmd) ;
    if (proc.waitForFinished())
    {
       ret = true;
       QByteArray  output = proc.readAllStandardError();
       QList<QByteArray> lines = output.split('\n');

       for (int l=0; l < lines.count(); l++)
       {
#if DEBUG
           qDebug() << "execQmake(): line" << l << lines.at(l);
#endif
           const QByteArray & line = lines.at(l).trimmed();         
           int start = 0;
           if ((start=line.indexOf("SOURCES:")) != -1)
           {              
               m_listSources->insertFile(line.mid(start+8), proc.workingDirectory());
           }
           else  if ((start=line.indexOf("HEADERS:")) != -1)
           {               
                m_listHeaders->insertFile(line.mid(start+8), proc.workingDirectory());
           }
           else  if ((start=line.indexOf("INCPATH:")) != -1)
           {               
                m_listIncludes->insertFile(line.mid(start+8), proc.workingDirectory());
           }
           else  if ((start=line.indexOf("DEFINES:")) != -1)
           {
                m_listDefines->insert(line.mid(start+8));
           }
       }
    }
    return ret;
}


QString QMake::tempSourcesListFilename(bool listSources, bool listHeaders)
{
    if(!m_sourcesListFile)
    {
        m_sourcesListFile = new QTemporaryFile();
        m_sourcesListFile->open();
        QStringList sfiles = makeList(listSources, listHeaders);

        for (int counter=0; counter < sfiles.count(); counter++)
        {
            QByteArray line(sfiles[counter].toLatin1());
            line += '\n';
            if ( m_sourcesListFile->write(line) != (qint64) line.size() )
            {
                qDebug() << "Error saving temporary file" << m_sourcesListFile->fileName();
            }
        }
        m_sourcesListFile->close();
    }
    return m_sourcesListFile->fileName();
}


QStringList  QMake::makeList(bool listSources, bool listHeaders)
{
    QStringList list;
    if (listSources && listHeaders)
    {
        list = allSources();
    }
    else
    {
        list = listHeaders ? headers() : sources();
    }
    return list;
}
