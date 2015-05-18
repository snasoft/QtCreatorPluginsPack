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
 * Filename: qprojcommandline.cpp
 * Created : 1 Feb 2013
 **/

#include "qprojcommandline.h"
#include "configtool.h"
#include "util.h"

#include <QDebug>
#include <QVariant>
#include <QStringList>
#include <QCoreApplication>
#include <QFileInfo>


static QString sListSources("list-sources");
static QString sListHeaders("list-headers");
static QString sListAll("list-allSources");
static QString sListDefines("list-defines");
static QString sListIncludes("list-includes");
static QString sListQtIncludes("list-qtincludes");
static QString sQtcPlugin("qtc-plugin");

static QString oTool("tool");
static QString oQmake("qmake-pathname");
static QString oConfig("config-file");
static QString oQtCreatorPath("qtcreator-pathname");
static QString mQmake = QObject::tr("QMake full pathname, default: search qmake executable in ($QTDIR/bin, $PATH, C:\\Qt\\*.* under MS-Windows)");
static QString mCreator = QObject::tr("QtCreator full pathname, default:  search qtcreator executable in ($PATH, C:\\Qt\\*.* under MS-Windows)");
static QString mConfig = QObject::tr("use another config file, default: $HOME/$USER_PROFILES/QtProjectTool/qpt.ini");

static QString sToolReturn("return-issues");

static QString pProject("qtProject");

static QCommandLineConfigEntry configEntry =
{ QCommandLine::Option, 'f', oConfig, mConfig, QCommandLine::Optional };

static QCommandLineConfigEntry qmakeEntry =
        { QCommandLine::Option, 'm', oQmake, mQmake , QCommandLine::Optional };

static QCommandLineConfigEntry creatorPathEntry =
{ QCommandLine::Option, 'c', oQtCreatorPath, mCreator , QCommandLine::Optional };

static QCommandLineConfigEntry qtProject =
{ QCommandLine::Param, 'p',  pProject , QObject::tr("The Qt Project File (.pro)"),  QCommandLine::Mandatory};


static QCommandLineConfigEntry oAvailableTools =
{ QCommandLine::Option, 't', oTool,
  QObject::tr("tool to use (name or full pathname, for name: search executable in $PATH and \"C:\\Program Files\" under MS-Windows), choices are:["),
   QCommandLine::Mandatory
};

/*
 *  INDEX_OF_TOOLS  must point to oAvailableTools index
 */
#define INDEX_OF_TOOLS   2
static  struct QCommandLineConfigEntry confTool[] =
   {
     configEntry,
     qmakeEntry,
     oAvailableTools,
     creatorPathEntry,
     { QCommandLine::Switch, 'q', sQtcPlugin,
           QObject::tr("It must be set when using inside QtCreator as an External Tool"),
           QCommandLine::Optional },
    { QCommandLine::Switch, 'r', sToolReturn,  QObject::tr("return the number of reported issues, default: return tool's return code"),
      QCommandLine::Optional },
    qtProject,
     QCOMMANDLINE_CONFIG_ENTRY_END
   };





static const struct QCommandLineConfigEntry confList[] =
   {
     configEntry,
     qmakeEntry,
    { QCommandLine::Switch, 'S', sListSources,  QObject::tr("List the source files of the project"),
       QCommandLine::Optional },
     { QCommandLine::Switch, 'H', sListHeaders, QObject::tr("List the header files of the project"),
       QCommandLine::Optional },
    { QCommandLine::Switch, 'A', sListAll,  QObject::tr("List both sources and headers of the project"),
      QCommandLine::Optional },
     { QCommandLine::Switch, 'D', sListDefines, QObject::tr("List the defines set in the project"),
       QCommandLine::Optional },
     { QCommandLine::Switch, 'I', sListIncludes,  QObject::tr("List the include path of the project"),
       QCommandLine::Optional },
     { QCommandLine::Switch, 'Q', sListQtIncludes, QObject::tr("List Qt include path"),
       QCommandLine::Optional },
     qtProject,

     QCOMMANDLINE_CONFIG_ENTRY_END
   };



QProjCommandLine::QProjCommandLine(Parameters &param) : m_params(param)
                                                      , m_configFile(0)
{    
    m_cmdLineTool = new QCommandLine(this);
    m_cmdLineTool->setConfig(confTool);
    m_cmdLineTool->enableVersion(false);
    m_cmdLineTool->enableHelp(true);
    connectAll(m_cmdLineTool);

    m_cmdLineLists = new QCommandLine(this);
    m_cmdLineLists->setConfig(confList);
    m_cmdLineLists->enableVersion(false);
    m_cmdLineLists->enableHelp(true);
    connectAll(m_cmdLineLists);

    m_cmdLineTool->setDescription(tr("To use as a wrapper to any tool (for example static analyzers)"));
    m_cmdLineLists->setDescription(tr("To extract information from a Qt Projetc (useful in scripts)"));
}

QProjCommandLine::~QProjCommandLine()
{
  if (m_configFile)   { delete m_configFile; }
}

void QProjCommandLine::parse()
{
    m_params.m_toolSet = m_cmdLineTool->parse();
    QCoreApplication::processEvents();
    (void) m_cmdLineLists->parse();
    QCoreApplication::processEvents();

    if (!m_params.m_userConfigFile.isEmpty())
    {
        QFileInfo config(m_params.m_userConfigFile);
        if (!config.exists() || config.size() < 50)
        {
            QString msg = QObject::tr("specified config file does not exist or does not have a valid content: ");
            msg +=  m_params.m_userConfigFile;
            Util::error(msg);
            emit error();
            return;
        }
    }
    m_configFile = ConfigTool::getInstance(m_params.m_userConfigFile);
    confTool[INDEX_OF_TOOLS].descr += m_configFile->toolsList().join(",") + QLatin1Char(']');
    m_cmdLineTool->setConfig(confTool);  

    if (!m_params.m_toolSet && ! m_params.m_anyListSet)
    {
        m_cmdLineTool->showHelp(false);
        m_cmdLineLists->showHelp(false);
        emit error();
    }
    else
    {
        if ( !QFileInfo( m_params.m_projectFile ).exists() )
        {
            Util::error( QObject::tr("project file is not specified in the command line or it does not exist") );
            if (m_params.m_toolSet)   m_cmdLineTool->showHelp(false);
            else                      m_cmdLineLists->showHelp(false);
            emit error();
        }
        else
        {
            emit done();
        }
    }

}

void QProjCommandLine::connectAll(QCommandLine *cmdParser)
{
    connect(cmdParser , SIGNAL(optionFound(QString,QVariant)),
            this,       SLOT(optionFound(QString,QVariant)));
    connect(cmdParser,  SIGNAL(paramFound(QString,QVariant)),
            this,       SLOT(paramFound(QString,QVariant)));
    connect(cmdParser,  SIGNAL(switchFound(QString)),
            this,       SLOT(switchFound(QString)));

}

void QProjCommandLine::switchFound(const QString &name)
{
    if (name.startsWith(QLatin1String("list-")))
    {
        m_params.m_anyListSet = true;
        if       (name == sListSources)    m_params.m_listSources = true;
        else if  (name == sListHeaders)    m_params.m_listHeaders = true;
        else if  (name == sListDefines)    m_params.m_listDefines = true;
        else if  (name == sListIncludes)   m_params.m_listIncludes= true;
        else if  (name == sListQtIncludes) m_params.m_listQtIncludes=true;
        else if  (name == sListAll)        m_params.m_listAllSources=true;
    }
    else if (name == sQtcPlugin)           m_params.m_qtcPlugin = true;
    else if (name == sToolReturn)          m_params.m_returnIssues = true;

}

void QProjCommandLine::paramFound(const QString &name, const QVariant &value)
{
    if (name == pProject)
    {
        m_params.m_projectFile = value.toString();
    }   
}

void QProjCommandLine::optionFound(const QString &name, const QVariant &value)
{
    if (name == oQmake)
    {
        m_params.m_qmakePath = value.toString();
    }
    else if (name == oTool)
        m_params.m_tool = value.toString();
    else if (name == oQtCreatorPath)
        m_params.m_qtCreatorPath = value.toString();
    else  if (name == oConfig )
        m_params.m_userConfigFile = value.toString();
}
