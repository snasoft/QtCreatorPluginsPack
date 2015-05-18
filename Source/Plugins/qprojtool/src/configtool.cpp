/*
 *
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
 * Filename: configtool.cpp
 * Created : 9 Feb 2013
 **/

#include "configtool.h"
#include "util.h"

#include <QSettings>
#include <QCoreApplication>
#include <QDebug>
#include <QHash>

ConfigTool * ConfigTool::m_configToolInstance = 0;

static const QStringList ToolFilesInputValues = QStringList()
   <<  "Stdin"
   << "CommandLineArgs"
   <<  "FileContentAfterSpace"
   <<  "FileContentNoSpace";


static const QStringList ToolResultOutputValues  = QStringList()
    << "Stdout"
    << "Stderr";

static const QStringList TrueOrFalse  = QStringList() <<  "false" << "true";



static QString helperValues(const QString& info, const QStringList& values)
{
    return info + QLatin1String("__VALUES__") + values.join(QLatin1String("_"));
}

QString helperToString(int index, const QStringList& list)
{
    QString ret("Unknow");
    if(index >=0 && index < list.count())
    {
        ret =  list.at(index);
    }
    else
    {
       qDebug() << "ERROR helperToString() index out of range" << index << list ;
    }
    return ret;
}


int helperToIndex(const QString& value, const QStringList& list)
{
    int ret=0;
    for(int l = 0; l < list.count(); l++)
    {
        if (QString::compare(value,list.at(l), Qt::CaseSensitive)==0)
        {
            ret=l;
            break;
        }
    }
    return ret;
}

static struct ConfigToolData  configCppCheck =
{
     QString("cppcheck")
    , true
    ,false
    ,ConfigToolData::FileContentNoSpace
    ,ConfigToolData::Stderr
    ,QString("-f -q --template '{file}:{line}:{id}:{message}' --inline-suppr --enable=style --enable=unusedFunction "
             "--suppress=missingInclude:* --inconclusive --file-list="
            )
    ,QStringList()
    ,QString("1.55")
    ,true
    ,false
    ,true 
};






static struct ConfigToolData  configKrazy2 =
{
     QString("krazy2")
    ,true
    ,true
    ,ConfigToolData::Stdin
    ,ConfigToolData::Stdout
    ,QString("--verbose --export textedit -")
    ,QStringList() << "=>c++"
    ,QString("2.93")
    ,false
    ,false
    ,false  
};


ConfigTool::ConfigTool(const QString& userConfig)
{
    m_keyItems.append("Sources_are_input_files");
    m_keyItems.append("Headers_are_input_files");
    m_keyItems.append("How_input_files_are_passed");
    m_keyItems.append("Results_are_written_into");
    m_keyItems.append("CommandLine");
    m_keyItems.append("Ignore_lines_that_contain_such_strings_AND_strings_are_separated_by_a_comma_plus_space");
    m_keyItems.append("Mininal_version_USE_only_digist_and_dot_it_will_be_converted_to_float");
    m_keyItems.append("Add_project_includes_using_dash_I");
    m_keyItems.append("Add_qt_includes_using_dash_I");
    m_keyItems.append("Add_project_Defines_using_dash_D");

    if (userConfig.isNull() || userConfig.isEmpty())
    {
        m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                   QCoreApplication::organizationName(),
                                   QCoreApplication::applicationName());

        //this inserts any tool configuration that is not yet in the user config file
        //it will be useful when further releases of the Qt Projet Tool appear with new tools
        QList<ConfigToolData*>  configs;
        configs.append(&configCppCheck);
        configs.append(&configKrazy2);
        //when new tool available, just append it here
        QHash<QString, ConfigToolData*>   currentTools = toolsDict();
        for (int counter=0; counter < configs.count(); counter++)
        {
            ConfigToolData * confTool = configs.at(counter);
            if (!currentTools.contains(confTool->toolName))
            {
                createConfigFile(*confTool);
            }
        }
        qDeleteAll(currentTools);
        currentTools.clear();
    }
    else
    {
        m_settings = new QSettings(userConfig, QSettings::IniFormat);
    }
    Util::info( m_settings->fileName(), QLatin1String("using"));

    m_settings->sync();
}


ConfigTool::~ConfigTool()
{
    delete m_settings;
    ConfigTool::m_configToolInstance = 0;
}


/*!
 * \brief ConfigTool::getInstance() returns a unique instance of the object
 * \return a unique instance of the object
 */
ConfigTool * ConfigTool::getInstance(const QString& userConfig)
{
    if (!ConfigTool::m_configToolInstance)
    {
        ConfigTool::m_configToolInstance = new ConfigTool(userConfig);
    }
    return ConfigTool::m_configToolInstance;
}



QStringList  ConfigTool::toolsList()
{
    return m_settings->childGroups();
}


bool ConfigTool::createConfigFile(const ConfigToolData &conf)
{
    bool ret = m_settings->isWritable();
    if (ret)
    {
        m_settings->beginGroup(conf.toolName);

        m_settings->setValue( helperValues(m_keyItems.at(IdxUseSources),TrueOrFalse), conf.useSources);
        m_settings->setValue( helperValues(m_keyItems.at(IdxUseHeaders),TrueOrFalse), conf.useHeaders);

        m_settings->setValue( helperValues(m_keyItems.at(IdxInput), ToolFilesInputValues),
                              helperToString(conf.input, ToolFilesInputValues)
                              );

        m_settings->setValue(helperValues(m_keyItems.at(IdxOuptupt), ToolResultOutputValues),
                             helperToString(conf.output, ToolResultOutputValues)
                             );

        m_settings->setValue(m_keyItems.at(IdxIgnoreLinesWith),  conf.ignoreLinesWith);

        m_settings->setValue(m_keyItems.at(IdxCommandLine),        conf.cmdLine);
        m_settings->setValue(m_keyItems.at(IdxMinimalVersion),     conf.minimalVersion);
        m_settings->setValue(helperValues(m_keyItems.at(IdxAddProjectIncludes),TrueOrFalse), conf.addProjectIncludes);
        m_settings->setValue(helperValues(m_keyItems.at(IdxAddQtIncludes),     TrueOrFalse), conf.addQtIncludes);
        m_settings->setValue(helperValues(m_keyItems.at(IdxAddProjectDefines), TrueOrFalse), conf.addProjectDefines);

        m_settings->endGroup();
    }
    return ret;
}

/*!
 * \brief ConfigTool::toolsData()
 *  It builds and returns a list of ConfigToolData objects with all tools that are configured to run.
 *  All the objects must be deleted after being used.
 *
 * \return a list of ConfigToolData objects with all tools that are configured to run
 */
QList<ConfigToolData *> ConfigTool::toolsData()
{
    QList<ConfigToolData *>  ret;
    QStringList tools = toolsList();
    if (tools.count() > 0)
    {
        QHash<QString, ConfigToolData *>  configToolTable;
        for(int t = 0; t < tools.count(); t++)
        {
            ConfigToolData *data = new ConfigToolData();
            data->toolName       = tools.at(t);
            configToolTable.insert(data->toolName, data);
            ret.append(data);
        }

        QList<QString> keys = m_settings->allKeys();       
        for(int k = 0; k < keys.count(); k++)
        {
            QStringList groupAndItem = keys.at(k).split(QChar('/'));
            ConfigToolData * data = configToolTable.value(groupAndItem[0]);
            QVariant  value = m_settings->value(keys.at(k));
            if (data)
            {
                switch(findItemIndex(groupAndItem[1]))
                {
                   case IdxUseSources:
                     data->useSources = value.toBool();
                   break;
                   case IdxUseHeaders:
                        data->useHeaders = value.toBool();
                   break;
                   case IdxInput:
                        data->input = (ConfigToolData::ToolFilesInput) helperToIndex(value.toString(), ToolFilesInputValues);
                   break;
                   case IdxOuptupt:
                        data->output = (ConfigToolData::ToolResultOutput) helperToIndex(value.toString(), ToolResultOutputValues);
                   break;
                   case IdxCommandLine:
                        data->cmdLine = value.toString();
                   break;
                   case IdxIgnoreLinesWith:
                        data->ignoreLinesWith = value.toStringList();
                   break;
                   case IdxMinimalVersion:
                        data->minimalVersion  = value.toString();
                   break;
                   case IdxAddProjectIncludes:
                        data->addProjectIncludes = value.toBool();
                   break;
                   case IdxAddQtIncludes:
                        data->addQtIncludes = value.toBool();
                   break;
                   case IdxAddProjectDefines:
                        data->addProjectDefines = value.toBool();
                   break;
                   default:
                             qDebug() << "Item out of range" << groupAndItem[1];
                   break;
                }
            }
        }
    }
    return ret;
}

/*!
 * \brief ConfigTool::toolsDict() returns all configured tools in a dictionary by tool name
 *
 *  Objects from hash must be deleted after being used
 *
 * \return returns all configured tools in a dictionary by tool name
 */
QHash<QString, ConfigToolData*>  ConfigTool::toolsDict()
{
    QHash<QString, ConfigToolData*>  ret;
    QList<ConfigToolData *> tools = toolsData();
    for (int counter=0; counter < tools.count(); counter++)
    {
        ret.insert(tools.at(counter)->toolName, tools.at(counter));
    }
    tools.clear();
    return ret;
}



int ConfigTool::findItemIndex(const QString &key)
{
    int index = m_keyItems.count();
    while (index--)
    {
        if (key.startsWith(m_keyItems.at(index)))
        {
            break;
        }
    }
    return index;
}
