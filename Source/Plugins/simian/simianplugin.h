#ifndef SIMIANPLUGIN_H
#define SIMIANPLUGIN_H

#include "simian_global.h"
#include "settings.h"

#include <extensionsystem/iplugin.h>


namespace Simian {
namespace Internal {

class SimianOutputPane;
class OptionsPage;

class SimianPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Simian.json")

public:
    SimianPlugin();
    ~SimianPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

    Settings *settings;
private slots:
    void triggerAction();
    void doSimian();
    void openFileAndHighlightDuplicateText(QString filePath, int startLine, int endLine);
    void onClear(); //очистка вывода output panel

private:
    void createSimianOutputPane();
    void createOptionsPage();
    SimianOutputPane *m_simianOutputPane;
    OptionsPage *m_optionsPage;


};

} // namespace Internal
} // namespace Simian

#endif // SIMILARITYANALYZERPLUGIN_H

