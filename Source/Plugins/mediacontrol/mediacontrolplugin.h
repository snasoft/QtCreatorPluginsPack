#ifndef MEDIACONTROL_H
#define MEDIACONTROL_H

#include "mediacontrol_global.h"

#include <extensionsystem/iplugin.h>


namespace MediaControl {
namespace Internal {

class MediaControlWidget;

class MediaControlPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "MediaControl.json")

public:
    MediaControlPlugin();
    ~MediaControlPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private:
    MediaControlWidget* mediaControl;

private slots:

    void playPauseToogle();
    void playNext();
    void playPrevious();

    void sendKeyEventToSystem(Qt::Key key);
};

} // namespace Internal
} // namespace MediaControl

#endif // MEDIACONTROL_H

