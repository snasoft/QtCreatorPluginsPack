#include "optionspage.h"

#include <coreplugin/icore.h>
#include <coreplugin/coreconstants.h>

#include <QLineEdit>
#include <QSpinBox>
#include <QFormLayout>
#include <QSettings>

using namespace HabraStata::Internal;

namespace HabraStata {
namespace Internal {

OptionsPage::OptionsPage(QSettings *settings, QObject *parent) : //const Settings &settings,
    IOptionsPage(parent),
    m_widget(0)
{
    this->settings = settings;
    setId("Habrahabr"); //уникальный идентификатор (по нему обращаться к странице)
    setDisplayName(tr("Habrahabr")); //наименование - отображается на вкладке
    setCategory(Core::Constants::SETTINGS_CATEGORY_CORE); //Определить категорию общую
}

void OptionsPage::apply()
{
    settings->setValue(QString::fromUtf8("username"), username->text());
    settings->setValue(QString::fromUtf8("update_interval"),updateIntervalInMinutes->value() );
    settings->sync();
    emit settingsApplyed();
}

void OptionsPage::finish()
{
    delete m_widget;
}

QWidget *OptionsPage::createPage(QWidget *parent)
{
    Q_UNUSED(parent);
    //TODO: fix this
    //if(!m_widget)
    //{
        m_widget = new QWidget();
        username = new QLineEdit();
        username->setMaximumWidth(200);

        updateIntervalInMinutes = new QSpinBox();
        updateIntervalInMinutes->setMaximumWidth(200);
        updateIntervalInMinutes->setMinimum(1);
        updateIntervalInMinutes->setMaximum(60);
        updateIntervalInMinutes->setValue(1);
        updateIntervalInMinutes->setSuffix(QString::fromUtf8(" min."));

        QFormLayout *inputLayout = new QFormLayout;
        inputLayout->addRow(tr("username:"), username);
        inputLayout->addRow(tr("update interval (minutes):"), updateIntervalInMinutes);

        m_widget->setLayout(inputLayout);

    //}
    initUiFromSettings();
    return m_widget;
}

QWidget *OptionsPage::widget()
{
    return createPage(0);
}



void OptionsPage::initUiFromSettings()
{
    updateIntervalInMinutes->setValue((settings->value(QString::fromUtf8("update_interval"), 1)).toInt());
    username->setText((settings->value(QString::fromUtf8("username"), QVariant())).toString());
}


} // namespace Internal
} // namespace HabraStata

