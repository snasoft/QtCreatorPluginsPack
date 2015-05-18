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
 * Filename: configtool.h
 * Created : 9 Feb 2013
 **/


#ifndef CONFIGTOOL_H
#define CONFIGTOOL_H

#include <QStringList>
#include <QHash>

class QSettings;



/*!
 * \brief The ConfigTool struct keeps all information necessary to run a tool
 *
 *  For each tool there will be a entry in a config file, so the application can start that tool
 *
 */
struct ConfigToolData
{
public:
    enum ToolFilesInput
    {
        Stdin,
        CommandLineArgs,
        FileContentAfterSpace,
        FileContentNoSpace
    };

    enum ToolResultOutput
    {
        Stdout,
        Stderr
    };

    QString           toolName;     //!< name: cppcheck, krazy2, ..
    bool              useSources;   //!< almost all tools use Sources, but sometimes we want only Headers
    bool              useHeaders;   //!< tool accepts sources and headers / only sources
    ToolFilesInput    input;        //!< from the args (sources and or headers) comes from
    ToolResultOutput  output;       //!< where the tool prints its results
    QString           cmdLine;      //!< the command line
    QStringList  ignoreLinesWith;   //!< specify lines to be ignored, each word represents a line,
                                     //!< use " " to represent expression with more than one word
    QString      minimalVersion;
    bool         addProjectIncludes; //!< -I dir1 -I dir2 ...
    bool         addQtIncludes;      //!< -I $QTDIR/includes -I$QTDIR/includes/Core ...
    bool         addProjectDefines;  //!< -DDEF1[=] -DDEF2=  
};



class ConfigTool
{
public:   
    virtual ~ConfigTool();
    static   ConfigTool * getInstance(const QString &userConfig = QLatin1String(0));
private:
    explicit ConfigTool(const QString &userConfig  = QLatin1String(0));
    explicit ConfigTool(ConfigTool & ) {}  // not used, just to avoid cppcheck warning
public:
    QString                              configFilename();
    QStringList                          toolsList();
    QHash<QString, ConfigToolData*>      toolsDict();
    QList<ConfigToolData *>              toolsData();
private:
    enum KeyIndex
    {
        IdxUseSources=0,
        IdxUseHeaders,
        IdxInput,
        IdxOuptupt,
        IdxCommandLine,
        IdxIgnoreLinesWith,
        IdxMinimalVersion,
        IdxAddProjectIncludes,
        IdxAddQtIncludes,
        IdxAddProjectDefines
    };

    bool               createConfigFile(const struct ConfigToolData& conf);
    int                findItemIndex(const QString& key);

private:
    QSettings *          m_settings;
    QList<QByteArray>    m_keyItems;
    static ConfigTool *  m_configToolInstance;
};



#endif // CONFIGTOOL_H
