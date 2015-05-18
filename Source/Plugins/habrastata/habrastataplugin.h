#ifndef HABRASTATA_H
#define HABRASTATA_H

#include "habrastata_global.h"
#include <extensionsystem/iplugin.h>

class QLabel;
class QTimer;
class QNetworkAccessManager;
class QNetworkReply;
class QSettings;

namespace HabraStata {
namespace Internal {

class OptionsPage;

class HabraStataPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "HabraStata.json")

public:
    HabraStataPlugin();
    ~HabraStataPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private slots:
    void updateKarmaText(const QString &karmaValue);
    void updateReitingText(const QString &reitingValue);
    void setUserName(const QString &name);
    void setUpdateInterval(const int &minutes);

    void requestStat();
    void requestStatFinished(QNetworkReply* reply);

    void updateFromSettings();

private:
    QLabel *karma;
    QLabel *rating;

    int updateIntervalInMinutes;
    QString username;
    QTimer *updateDataTimer;

    QNetworkAccessManager *www;

    QSettings *settings;

    OptionsPage *m_optionsPage;

    void addWidget();
    void createOptionsPage();
};

} // namespace Internal
} // namespace HabraStata

#endif // HABRASTATA_H

