/*
# **************************************************************************************
# *
# * Project Line Counter Plugin (https://sourceforge.net/p/projectlinecounterplugin)
# * This plugin adds info string to the Qt Creator Editor toolbar that counts lines
# * in the current file and whole project if pro-file opened.
# *
# * Copyright (C) 2014 Sergey Levin <liteswamp@gmail.com>
# *
# * This program is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with this program. If not, see <http://www.gnu.org/licenses/>.
# *
# **************************************************************************************
*/

#include "functions.h"
#include "clinecounteraction.h"
#include "projectlinecounterplugin.h"
#include "projectlinecounterconstants.h"

#include <projectexplorer/projecttree.h>
#include <projectexplorer/project.h>
#include <coreplugin/icore.h>
#include <coreplugin/idocument.h>
#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/editormanager/editormanager.h>

#include <QTranslator>
#include <QtConcurrentMap>
#include <QFutureWatcher>
#include <QPointer>
#include <QTimer>



QFutureWatcher<unsigned long> linesCountFutureWatcher;
QPointer<ProjectExplorer::Project> currentProject = 0;



using namespace ProjectLineCounter::Internal;



ProjectLineCounterPlugin::ProjectLineCounterPlugin()
{
    // Create your members
}



ProjectLineCounterPlugin::~ProjectLineCounterPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}



CLineCounterAction *ProjectLineCounterPlugin::getCounterLabel() const
{
    if (!Core::EditorManager::instance())
        return 0;

    Core::IEditor *editor = Core::EditorManager::instance()->currentEditor();
    return (editor && editor->toolBar()) ?
                CLineCounterAction::findInstance(editor->toolBar()->actions()) :
                0;
}



void ProjectLineCounterPlugin::setLinesCount() const
{    
    QPointer<CLineCounterAction> counterLabel = getCounterLabel();

    if (!counterLabel.isNull())
        counterLabel->setCounter(linesCountFutureWatcher.result());
}



bool ProjectLineCounterPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    const QString& language = Core::ICore::userInterfaceLanguage();
    if (!language.isEmpty()) {
        QStringList paths;
        paths << Core::ICore::resourcePath () << Core::ICore::userResourcePath();
        const QString& trFile = QLatin1String ("ProjectLineCounter_") + language;
        QTranslator* translator = new QTranslator(this);
        foreach (const QString& path, paths)
            if (translator->load(trFile, path + QLatin1String ("/translations"))) {
                qApp->installTranslator(translator);
                break;
            }
    }

    connect(&linesCountFutureWatcher, SIGNAL(finished()), SLOT(setLinesCount()));
    connect(Core::EditorManager::instance(), SIGNAL(editorCreated(Core::IEditor*, const QString&)), SLOT(onEditorCreated(Core::IEditor*, const QString&)));
    connect(Core::EditorManager::instance(), SIGNAL(currentEditorChanged(Core::IEditor*)), SLOT(onDocumentChanged()));
    connect(Core::EditorManager::instance(), SIGNAL(currentDocumentStateChanged()), SLOT(onDocumentChanged()));        
    connect(ProjectExplorer::ProjectTree::instance(), SIGNAL(currentProjectChanged(ProjectExplorer::Project*)), SLOT(onProjectChanged(ProjectExplorer::Project*)));

    return true;
}



void ProjectLineCounterPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}



ExtensionSystem::IPlugin::ShutdownFlag ProjectLineCounterPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}



bool ProjectLineCounterPlugin::isCurrentProject(const Utils::FileName &fileName) const
{        
    return (!currentProject.isNull() && fileName == currentProject->projectFilePath());
}



void ProjectLineCounterPlugin::linesCount(const Utils::FileName &fileName) const
{  
    QStringList sourceFiles = isCurrentProject(fileName) ?                
                currentProject->files(ProjectExplorer::Project::ExcludeGeneratedFiles) : //Находим все файлы текущего проекта
                QStringList(fileName.toString()); //Только текущий файл

    linesCountFutureWatcher.cancel();
    linesCountFutureWatcher.waitForFinished();

    QFuture<unsigned long> future = QtConcurrent::mappedReduced(sourceFiles, ::linesCount, addLines, QtConcurrent::UnorderedReduce);
    linesCountFutureWatcher.setFuture(future);
}



void ProjectLineCounterPlugin::onProjectChanged(ProjectExplorer::Project *project) const
{
    currentProject = project;

    if (!currentProject.isNull())
        connect(currentProject.data(), SIGNAL(fileListChanged()), SLOT(onDocumentChanged()));
}



void ProjectLineCounterPlugin::onDocumentChanged() const
{    
    QPointer<CLineCounterAction> counterLabel = getCounterLabel();

    if (!counterLabel.isNull()) {
        QPointer<Core::IDocument> document = Core::EditorManager::instance()->currentDocument();
        if (document) {
            counterLabel->setProjectSelected(isCurrentProject(document->filePath()));
            linesCount(document->filePath());
        }
    }
}



void ProjectLineCounterPlugin::onEditorCreated(Core::IEditor *editor, const QString  &fileName) const
{
    if (editor->toolBar()) {
        Utils::FileName file = Utils::FileName::fromString(fileName);
        CLineCounterAction *counterLabel = new CLineCounterAction(isCurrentProject(file), editor);
        editor->toolBar()->addAction(counterLabel);        
        linesCount(file);
    }
}
