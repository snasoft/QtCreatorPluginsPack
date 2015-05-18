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
 * Filename: qmake.h
 * Created : 26 Jan 2013
 **/

#ifndef QMAKE_H
#define QMAKE_H

#include <QStringList>
#include <QObject>

class QTemporaryFile;
class QProccess;
class StringDict;




/*!
 * \brief The QMake class aims to act in a Qt project file extracting its information
 *
 *  It uses the qmake provided by Qt to extract the information.
 *
 *  It extracts the information in the \ref QMake() constructor doing:
 *  \li creates another project (in the same path) as a copy of the original file using \ref m_temporaryProjectFile
 *  \li add some configurations in order to drive "qmake tool" to generate the necessary information in \ref handleProjectFile()
 *  \li executes qmake over the this new project file in \ref execQmake()
 *  \li parses "qmake tool" output to get the list of sources, headers and defines from the project
 *
 *  The status can be checked by calling \ref isOK()
 */
class QMake : public QObject
{
    Q_OBJECT
public:
    explicit QMake(const QString& projectFile, const QString& qmakeFilePath = QLatin1String(0) );
    virtual ~QMake();
public:
    bool            isOK()    { return m_done; }
    QStringList     sources();
    QStringList     defines();
    QStringList     headers();
    QStringList     allSources();
    QStringList     includesDir();
    QStringList     qtIncludesDir();
    QStringList     makeList(bool listSources=true, bool listHeaders=true);
    QString         tempSourcesListFilename(bool listSources=true, bool listHeaders=true);
    QString         qmakeFilePath()   { return m_qmakeFilePath; }
private:
    QString         searchQmakePath();
    void            buildQtIncludeDirs();
    bool            handleProjectFile();
    bool            execQmake();
private:
    StringDict  *  m_listSources;
    StringDict  *  m_listHeaders;
    StringDict  *  m_listIncludes;
    StringDict  *  m_listDefines;
    QStringList    m_listQtIncludes;
private:
    QString         m_qmakeFilePath;
    QString         m_projectFilePath;
    QTemporaryFile* m_temporaryProjectFile;
    QTemporaryFile* m_sourcesListFile;
    bool            m_done;
};


#endif // QMAKE_H
