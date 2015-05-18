#include "settings.h"

//для получения текущей директории
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projecttree.h>

namespace Simian {
namespace Internal {

Settings::Settings()
{

    globalScopeSettings = new QSettings(QString::fromUtf8("snasoft.com"), QString::fromUtf8("simian"));
    //projectScopeSettings;

    simianExecutablePath = globalScopeSettings->value(QString::fromUtf8("simianExecutablePath")).toString();

    useCustomWorkingDir = globalScopeSettings->value(QString::fromUtf8("useCustomWorkingDir"), false).toBool();
    customWorkingDir = globalScopeSettings->value(QString::fromUtf8("customWorkingDir")).toString();


    this->initDefaults();
    this->setToDefaults();

}

void Settings::load()
{
    simianExecutablePath = globalScopeSettings->value(QString::fromUtf8("simianExecutablePath")).toString();

    useCustomWorkingDir = globalScopeSettings->value(QString::fromUtf8("useCustomWorkingDir"), false).toBool();
    customWorkingDir = globalScopeSettings->value(QString::fromUtf8("customWorkingDir")).toString();



    //
    QString settingsFilePath;
    if (useCustomWorkingDir && !customWorkingDir.isEmpty())
    {
        settingsFilePath = customWorkingDir;
    }
    else
    {
        //получаем текущую директорию проекта
        ProjectExplorer::ProjectExplorerPlugin *explorer = ProjectExplorer::ProjectExplorerPlugin::instance();

        if (ProjectExplorer::ProjectTree::currentProject() == NULL)
        {
            this->setToDefaults(); //загружаем настройки по-умолчанию
            return;
        }
        settingsFilePath = ProjectExplorer::ProjectTree::currentProject()->projectDirectory().toString();
    }


    settingsFilePath = settingsFilePath + QString::fromUtf8("/simian.ini");
    projectScopeSettings = new QSettings(settingsFilePath, QSettings::IniFormat);

    //установка значений
    flags[QString::fromUtf8("ignoreCharacterCase")] = projectScopeSettings->value(QString::fromUtf8("ignoreCharacterCase"), defaultFlags[QString::fromUtf8("ignoreCharacterCase")]).toBool();
    flags[QString::fromUtf8("ignoreCharacters")] = projectScopeSettings->value(QString::fromUtf8("ignoreCharacters"), defaultFlags[QString::fromUtf8("ignoreCharacters")]).toBool();
    flags[QString::fromUtf8("ignoreStringCase")] = projectScopeSettings->value(QString::fromUtf8("ignoreStringCase"), defaultFlags[QString::fromUtf8("ignoreStringCase")]).toBool();
    flags[QString::fromUtf8("ignoreStrings")] = projectScopeSettings->value(QString::fromUtf8("ignoreStrings"), defaultFlags[QString::fromUtf8("ignoreStrings")]).toBool();
    flags[QString::fromUtf8("ignoreNumbers")] = projectScopeSettings->value(QString::fromUtf8("ignoreNumbers"), defaultFlags[QString::fromUtf8("ignoreNumbers")]).toBool();
    flags[QString::fromUtf8("ignoreLiterals")] = projectScopeSettings->value(QString::fromUtf8("ignoreLiterals"), defaultFlags[QString::fromUtf8("ignoreLiterals")]).toBool();
    flags[QString::fromUtf8("ignoreIdentifierCase")] = projectScopeSettings->value(QString::fromUtf8("ignoreIdentifierCase"), defaultFlags[QString::fromUtf8("ignoreIdentifierCase")]).toBool();
    flags[QString::fromUtf8("ignoreIdentifiers")] = projectScopeSettings->value(QString::fromUtf8("ignoreIdentifiers"), defaultFlags[QString::fromUtf8("ignoreIdentifiers")]).toBool();
    flags[QString::fromUtf8("ignoreModifiers")] = projectScopeSettings->value(QString::fromUtf8("ignoreModifiers"), defaultFlags[QString::fromUtf8("ignoreModifiers")]).toBool();
    flags[QString::fromUtf8("ignoreCurlyBraces")] = projectScopeSettings->value(QString::fromUtf8("ignoreCurlyBraces"), defaultFlags[QString::fromUtf8("ignoreCurlyBraces")]).toBool();
    flags[QString::fromUtf8("ignoreOverlappingBlocks")] = projectScopeSettings->value(QString::fromUtf8("ignoreOverlappingBlocks"), defaultFlags[QString::fromUtf8("ignoreOverlappingBlocks")]).toBool();
    flags[QString::fromUtf8("balanceParentheses")] = projectScopeSettings->value(QString::fromUtf8("balanceParentheses"), defaultFlags[QString::fromUtf8("balanceParentheses")]).toBool();
    flags[QString::fromUtf8("balanceSquareBrackets")] = projectScopeSettings->value(QString::fromUtf8("balanceSquareBrackets"), defaultFlags[QString::fromUtf8("balanceSquareBrackets")]).toBool();

    threshold = projectScopeSettings->value(QString::fromUtf8("threshold"), 6).toInt();
    excludes = projectScopeSettings->value(QString::fromUtf8("excludes")).toString();

}

void Settings::save()
{
    //1. проверить, отличаются ли настройки от умолчальных
    //если да, то необходимо сохранить настройки
    //для этого проверяем текущую папку для настроек (проект или другая директория)
    //сохраняем глобальные настройки



    //сохраняем умолчальные настройки
    globalScopeSettings->setValue(QString::fromUtf8("simianExecutablePath"), simianExecutablePath);

    globalScopeSettings->setValue(QString::fromUtf8("useCustomWorkingDir"), useCustomWorkingDir);
    globalScopeSettings->setValue(QString::fromUtf8("customWorkingDir"), customWorkingDir);

    globalScopeSettings->sync();

    bool isDefaultSettings = true;
    //проверка флагов на умолчальность
    QMapIterator<QString, bool> i(this->flags);
    while (i.hasNext()) {
        i.next();
        if (defaultFlags[i.key()] != i.value())
        {
            isDefaultSettings = false;
            break;
        }
    }
    if (threshold != 6 || !excludes.isEmpty()) //проверка threshold и excludes на умолчальность
    {
        isDefaultSettings = false;
    }

    if (isDefaultSettings) //если настройки умолчальные, то ничего не нужно сохранять в файл
    {
        return;
    }

    //если настройки изменились, то их надо сохранить для конкретного проекта или директории

    QString settingsFilePath;
    if (useCustomWorkingDir)
    {
        settingsFilePath = customWorkingDir;
    }
    else
    {
        //получаем текущую директорию проекта
        ProjectExplorer::ProjectExplorerPlugin *explorer = ProjectExplorer::ProjectExplorerPlugin::instance();

        if (ProjectExplorer::ProjectTree::currentProject() == NULL)
        {
            return; //текущего проекта нет и в настройках используется текущий проект
        }
        settingsFilePath = ProjectExplorer::ProjectTree::currentProject()->projectDirectory().toString();
    }


    settingsFilePath = settingsFilePath + QString::fromUtf8("/simian.ini");

    //QMessageBox msgBox;
    //msgBox.setText(settingsFilePath);
    //msgBox.exec();

    projectScopeSettings = new QSettings(settingsFilePath, QSettings::IniFormat);

    //установка значений
    projectScopeSettings->setValue(QString::fromUtf8("ignoreCharacterCase"), flags[QString::fromUtf8("ignoreCharacterCase")]);
    projectScopeSettings->setValue(QString::fromUtf8("ignoreCharacters"), flags[QString::fromUtf8("ignoreCharacters")]);
    projectScopeSettings->setValue(QString::fromUtf8("ignoreStringCase"), flags[QString::fromUtf8("ignoreStringCase")]);
    projectScopeSettings->setValue(QString::fromUtf8("ignoreStrings"), flags[QString::fromUtf8("ignoreStrings")]);
    projectScopeSettings->setValue(QString::fromUtf8("ignoreNumbers"), flags[QString::fromUtf8("ignoreNumbers")]);
    projectScopeSettings->setValue(QString::fromUtf8("ignoreLiterals"), flags[QString::fromUtf8("ignoreLiterals")]);
    projectScopeSettings->setValue(QString::fromUtf8("ignoreIdentifierCase"), flags[QString::fromUtf8("ignoreIdentifierCase")]);
    projectScopeSettings->setValue(QString::fromUtf8("ignoreIdentifiers"), flags[QString::fromUtf8("ignoreIdentifiers")]);
    projectScopeSettings->setValue(QString::fromUtf8("ignoreModifiers"), flags[QString::fromUtf8("ignoreModifiers")]);
    projectScopeSettings->setValue(QString::fromUtf8("ignoreCurlyBraces"), flags[QString::fromUtf8("ignoreCurlyBraces")]);
    projectScopeSettings->setValue(QString::fromUtf8("ignoreOverlappingBlocks"), flags[QString::fromUtf8("ignoreOverlappingBlocks")]);
    projectScopeSettings->setValue(QString::fromUtf8("balanceParentheses"), flags[QString::fromUtf8("balanceParentheses")]);
    projectScopeSettings->setValue(QString::fromUtf8("balanceSquareBrackets"), flags[QString::fromUtf8("balanceSquareBrackets")]);

    projectScopeSettings->setValue(QString::fromUtf8("threshold"), threshold);
    projectScopeSettings->setValue(QString::fromUtf8("excludes"), excludes);

    projectScopeSettings->sync();



}

void Settings::initDefaults()
{
    defaultFlags[QString::fromUtf8("ignoreCharacterCase")] = true;
    defaultFlags[QString::fromUtf8("ignoreCharacters")] = false;
    defaultFlags[QString::fromUtf8("ignoreStringCase")] = true;
    defaultFlags[QString::fromUtf8("ignoreStrings")] = false;
    defaultFlags[QString::fromUtf8("ignoreNumbers")] = false;
    defaultFlags[QString::fromUtf8("ignoreLiterals")] = false;
    defaultFlags[QString::fromUtf8("ignoreIdentifierCase")] = true;
    defaultFlags[QString::fromUtf8("ignoreIdentifiers")] = false;
    defaultFlags[QString::fromUtf8("ignoreModifiers")] = true;
    defaultFlags[QString::fromUtf8("ignoreCurlyBraces")] = false;
    defaultFlags[QString::fromUtf8("ignoreOverlappingBlocks")] = false;
    defaultFlags[QString::fromUtf8("balanceParentheses")] = false;
    defaultFlags[QString::fromUtf8("balanceSquareBrackets")] = false;
}

void Settings::setToDefaults()
{
    excludes = QString::fromUtf8("");
    threshold = 6;

    flags[QString::fromUtf8("ignoreCharacterCase")] = defaultFlags[QString::fromUtf8("ignoreCharacterCase")];
    flags[QString::fromUtf8("ignoreCharacters")] = defaultFlags[QString::fromUtf8("ignoreCharacters")];
    flags[QString::fromUtf8("ignoreStringCase")] = defaultFlags[QString::fromUtf8("ignoreStringCase")];
    flags[QString::fromUtf8("ignoreStrings")] = defaultFlags[QString::fromUtf8("ignoreStrings")];
    flags[QString::fromUtf8("ignoreNumbers")] = defaultFlags[QString::fromUtf8("ignoreNumbers")];
    flags[QString::fromUtf8("ignoreLiterals")] = defaultFlags[QString::fromUtf8("ignoreLiterals")];
    flags[QString::fromUtf8("ignoreIdentifierCase")] = defaultFlags[QString::fromUtf8("ignoreIdentifierCase")];
    flags[QString::fromUtf8("ignoreIdentifiers")] = defaultFlags[QString::fromUtf8("ignoreIdentifiers")];
    flags[QString::fromUtf8("ignoreModifiers")] = defaultFlags[QString::fromUtf8("ignoreModifiers")];
    flags[QString::fromUtf8("ignoreCurlyBraces")] = defaultFlags[QString::fromUtf8("ignoreCurlyBraces")];
    flags[QString::fromUtf8("ignoreOverlappingBlocks")] = defaultFlags[QString::fromUtf8("ignoreOverlappingBlocks")];
    flags[QString::fromUtf8("balanceParentheses")] = defaultFlags[QString::fromUtf8("balanceParentheses")];
    flags[QString::fromUtf8("balanceSquareBrackets")] = defaultFlags[QString::fromUtf8("balanceSquareBrackets")];

}

void Settings::setSettingsToUseCustomDir(const bool &useCustomDir)
{
    globalScopeSettings->setValue(QString::fromUtf8("useCustomWorkingDir"), useCustomDir);
    globalScopeSettings->sync();
}

void Settings::setCustomWorkingDir(const QString &customDirPath)
{
    globalScopeSettings->setValue(QString::fromUtf8("customWorkingDir"), customDirPath);
    globalScopeSettings->sync();
}

}// Internal
}// Simian
