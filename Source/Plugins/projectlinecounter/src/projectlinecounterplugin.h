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

#ifndef PROJECTLINECOUNTER_H
#define PROJECTLINECOUNTER_H

#include "projectlinecounter_global.h"

#include <extensionsystem/iplugin.h>


class CLineCounterAction;

namespace Core {
class IEditor;
}

namespace Utils {
class FileName;
}

namespace ProjectExplorer {
class Project;
}



namespace ProjectLineCounter {
namespace Internal {

class ProjectLineCounterPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "ProjectLineCounter.json")

    bool isCurrentProject(const Utils::FileName &fileName) const;
    void linesCount(const Utils::FileName &fileName) const;
    CLineCounterAction *getCounterLabel() const;

public:
    ProjectLineCounterPlugin();
    ~ProjectLineCounterPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private slots:
    void onEditorCreated(Core::IEditor *editor, const QString &fileName) const;
    void onProjectChanged(ProjectExplorer::Project *project) const;
    void onDocumentChanged() const;        
    void setLinesCount() const;
};

} // namespace Internal
} // namespace ProjectLineCounter

#endif // PROJECTLINECOUNTER_H

