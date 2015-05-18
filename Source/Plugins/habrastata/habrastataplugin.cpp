#include "habrastataplugin.h"
#include "habrastataconstants.h"
#include "optionspage.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/modemanager.h> //for adding widget

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>

#include <QtPlugin>

using namespace HabraStata::Internal;

HabraStataPlugin::HabraStataPlugin()  : m_optionsPage(0)
{
    settings = new QSettings(QString::fromUtf8("snasoft.com"), QString::fromUtf8("habrastata"));
    updateDataTimer = new QTimer();
    www = new QNetworkAccessManager(this);

    this->updateFromSettings();

    connect(www, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestStatFinished(QNetworkReply*)));

    setUpdateInterval(1);
    connect(updateDataTimer, SIGNAL(timeout()), this, SLOT(requestStat()));
    requestStat();

    updateDataTimer->start();
}

HabraStataPlugin::~HabraStataPlugin()
{
}

bool HabraStataPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    createOptionsPage();

    addWidget();

    return true;
}
void HabraStataPlugin::createOptionsPage()
{
    m_optionsPage = new OptionsPage(this->settings);
    connect(m_optionsPage, SIGNAL(settingsApplyed()), this, SLOT(updateFromSettings()));
    addAutoReleasedObject(m_optionsPage);
}


void HabraStataPlugin::extensionsInitialized()
{
}

ExtensionSystem::IPlugin::ShutdownFlag HabraStataPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

void HabraStataPlugin::updateKarmaText(const QString &karmaValue)
{
    karma->setText(karmaValue);
    karma->setHidden(false);
}

void HabraStataPlugin::updateReitingText(const QString &reitingValue)
{
    rating->setText(reitingValue);
    rating->setHidden(false);
}

void HabraStataPlugin::setUserName(const QString &name)
{
    username = name;
}

void HabraStataPlugin::setUpdateInterval(const int &minutes)
{
    updateIntervalInMinutes = minutes;
    updateDataTimer->setInterval( updateIntervalInMinutes * 60 * 1000);
}

void HabraStataPlugin::requestStat()
{
    if (this->username.isEmpty())
    {
        return;
    }
    QString getProfileApiUrl = QString::fromUtf8("http://habrahabr.ru/api/profile/") + username + QString::fromUtf8("/");
    www->get(QNetworkRequest(QUrl(getProfileApiUrl)));
}

void HabraStataPlugin::requestStatFinished(QNetworkReply *reply)
{
    QDomDocument doc;
    doc.setContent(reply->readAll());
    QDomElement docElem = doc.documentElement();

    QString karmaValue = docElem.elementsByTagName(QString::fromUtf8("karma")).at(0).firstChild().nodeValue();
    QString ratingValue = docElem.elementsByTagName(QString::fromUtf8("rating")).at(0).firstChild().nodeValue();

    updateKarmaText( karmaValue );
    updateReitingText( ratingValue);
}

void HabraStataPlugin::updateFromSettings()
{
    QString userName = (settings->value(QString::fromUtf8("username"), QVariant())).toString();
    this->setUserName(userName);
    int updateInterval = (settings->value(QString::fromUtf8("update_interval"), 1)).toInt();
    this->setUpdateInterval(updateInterval);
    requestStat();
}

void HabraStataPlugin::addWidget()
{
    QWidget *mainWindow = Core::ICore::mainWindow();
    QWidget *habraStata = new QWidget(mainWindow);

    QFont habraStataFont = QFont(QString::fromUtf8("Arial"), 14, 10);
    habraStataFont.setPointSize(14);
    habraStataFont.setFamily(QString::fromUtf8("Arial"));
    habraStataFont.setBold(true);

    karma = new QLabel();
    karma->setToolTip(tr("Habrahabr.ru User Karma"));
    karma->setHidden(true);
    karma->setAlignment(Qt::AlignHCenter);
    karma->setMaximumHeight(19);
    karma->setMaximumWidth(60);
    karma->setFont(habraStataFont);
    karma->setText(QString::fromUtf8("0,0"));
    karma->setStyleSheet( QString::fromUtf8("QLabel {color: #66CC66; }" ));
    karma->setObjectName(QString::fromUtf8("karma"));

    rating = new QLabel();
    rating->setToolTip(tr("Habrahabr.ru User Rating"));
    rating->setHidden(true);
    rating->setAlignment(Qt::AlignHCenter);
    rating->setMaximumHeight(19);
    rating->setMaximumWidth(60);
    rating->setStyleSheet( QString::fromUtf8("QLabel {color: #CC66CC; }" ));
    rating->setFont(habraStataFont);
    rating->setObjectName(QString::fromUtf8("rating"));
    rating->setText(QString::fromUtf8("0,0"));

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,5);
    layout->addWidget(karma);
    layout->addWidget(rating);
    layout->setAlignment(Qt::AlignHCenter);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->setAlignment(Qt::AlignHCenter);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(layout);

    habraStata->setLayout(mainLayout);

    Core::ModeManager::addWidget(habraStata);
}
