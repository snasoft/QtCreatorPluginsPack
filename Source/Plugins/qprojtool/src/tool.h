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
 * Filename: tool.h
 * Created : 26 Jan 2013
 **/


#ifndef TOOL_H
#define TOOL_H

#include <QString>
#include <QList>


/*!
 * \brief The Tool class represents an executable (either binary or a script) and its information to be used by an application
 *
 *  The Tool searches in the PATH variable and in other Paths by a tool name, then builds a list of the pathnames
 *  with their respective versions if available by using the command line "tool --version".
 *
 *  Once the list is built, the application can ask for example ask if the tool has a minimal version 1.0
 * \code
 *      Tool qmakeTool("qmake");
 *      // suppose it requires qmake version 4.0 or later
 *      const Tool::ToolInfo *qmakeInfo qmakeTool.geMinimalVersion(4);
 *      if (!qmakeInfo)
 *      {
 *          // try $QTDIR/bin/qmake if it is not in the PATH
 *          qmakeInfo = qmakeTool.searchByEnvVar("QTDIR", "bin");
 *      }
 *      if (!qmakeInfo) {
 *          qDebug() << "qmake not found";
 *      } else {
 *         qDebug() "using qmake in" << qmakeInfo->filePath << "version" << qmakeInfo->version;
 *      }
 * \endcode
 */
class Tool
{
public:
    explicit Tool(const QString& paramfullpathOrName, bool checkVersion = true);
    explicit Tool(const QString& name, const QStringList& extraPaths, bool checkVersion = true);
    virtual ~Tool();
    struct ToolInfo
    {
      public:
        ToolInfo(const QString& binary);
        QString  filePath;  //!< full pathname of the tool/application
        QString  version;   //!< the version by executing the tool with "--version" or 0.0
    };
    const ToolInfo * searchByEnvVar(const char *envVAR, const char *dirBehindVAR=0);
    void             searchFromDir(const QString& startDir, const QString& dir = QLatin1String(0));
#if defined(Q_OS_MSDOS) || defined(Q_OS_WIN)
    void              searchQtForWinExtraPaths();
#endif
    const ToolInfo * getGreaterVersion()  const;
    const ToolInfo * getMinimalVersion(const QString &version) const;
    inline int       count() const { return m_tools.count(); }
    const ToolInfo * get(int index) const;
private:
    ToolInfo *       createToolInfo(const QString &name, const QString& path = QLatin1String(0));
    void             searchInPathVariable();
    void             searchExtraPaths(const QStringList &paths);  
    void             extractVersion(Tool::ToolInfo *toolInfo) const;
    static QStringList findDir(const QString& startDir, const QString& dir );
private:
    QList<ToolInfo*>  m_tools;
    QString           m_name;
    bool              m_checkVersion;
#if defined(Q_OS_MSDOS) || defined(Q_OS_WIN)
    static QStringList  m_qtBinDirs;
#endif
};

#endif // TOOL_H
