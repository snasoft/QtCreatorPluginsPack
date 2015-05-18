#include "optionspage.h"
#include "settings.h"

#include "ui/optionsdialog.h" //доступ к диалогу настроек (виджету)
#include "ui_optionsdialog.h" //прямой доступ к интерфейсу диалога

#include <coreplugin/icore.h>

Simian::Internal::OptionsDialog *some = 0;

namespace Simian {
namespace Internal {

OptionsPage::OptionsPage(Settings *settings, QObject *parent) : //const Settings &settings,
    IOptionsPage(parent),
    m_widget(0)
{
    setId("SimianSettings"); //уникальный идентификатор (по нему обращаться к странице)
    setDisplayName(tr("Similarity Analyser settings")); //наименование - отображается на вкладке
    setCategory("Simian");
    setDisplayCategory(tr("Simian")); //название в левом списке
    setCategoryIcon(QString::fromUtf8(":/icons/icons/simian.png"));
    m_settings = settings;

}

QWidget *OptionsPage::widget()
{
    if (!m_widget) {
        m_widget = new OptionsDialog;
        connect(m_widget, SIGNAL(workingDirectoryChangedToProjectDir()), this, SLOT(updateSettingsToCurrentProject()));
        connect(m_widget, SIGNAL(customDirectoryPathChanged(QString)), this, SLOT(updateSettingsToCustomDir(QString)));
    }
    m_settings->load();
    initUiFromSettings();

    return m_widget;
}

void OptionsPage::apply()
{
    m_settings->flags[QString::fromUtf8("ignoreCharacterCase")] = m_widget->ui->ignoreCharacterCase->isChecked();
    m_settings->flags[QString::fromUtf8("ignoreCharacters")] = m_widget->ui->ignoreCharacters->isChecked();
    m_settings->flags[QString::fromUtf8("ignoreStringCase")] = m_widget->ui->ignoreStringCase->isChecked();
    m_settings->flags[QString::fromUtf8("ignoreStrings")] = m_widget->ui->ignoreStrings->isChecked();
    m_settings->flags[QString::fromUtf8("ignoreNumbers")] = m_widget->ui->ignoreNumbers->isChecked();
    m_settings->flags[QString::fromUtf8("ignoreLiterals")] = m_widget->ui->ignoreLiterals->isChecked();
    m_settings->flags[QString::fromUtf8("ignoreIdentifierCase")] = m_widget->ui->ignoreIdentifierCase->isChecked();
    m_settings->flags[QString::fromUtf8("ignoreIdentifiers")] = m_widget->ui->ignoreIdentifiers->isChecked();
    m_settings->flags[QString::fromUtf8("ignoreModifiers")] = m_widget->ui->ignoreModifiers->isChecked();
    m_settings->flags[QString::fromUtf8("ignoreCurlyBraces")] = m_widget->ui->ignoreCurlyBraces->isChecked();
    m_settings->flags[QString::fromUtf8("ignoreOverlappingBlocks")] = m_widget->ui->ignoreOverlappingBlocks->isChecked();
    m_settings->flags[QString::fromUtf8("balanceParentheses")] = m_widget->ui->balanceParentheses->isChecked();
    m_settings->flags[QString::fromUtf8("balanceSquareBrackets")] = m_widget->ui->balanceSquareBrackets->isChecked();

    m_settings->threshold = m_widget->ui->threshold->value();
    m_settings->excludes = m_widget->ui->lineEditExcludes->text();

    m_settings->useCustomWorkingDir = m_widget->ui->radioButtonUseCustomWorkingDir->isChecked();
    m_settings->customWorkingDir = m_widget->ui->lineEditCustomDirectoryPath->text();

    m_settings->simianExecutablePath = m_widget->ui->lineEditCustomSimianPath->text();

    m_settings->save();
}

void OptionsPage::finish()
{
    delete m_widget;
}

void OptionsPage::initUiFromSettings()
{
    m_widget->ui->radioButtonUseCustomWorkingDir->setChecked(m_settings->useCustomWorkingDir);

    m_widget->ui->lineEditCustomDirectoryPath->setEnabled(m_settings->useCustomWorkingDir);
    m_widget->ui->pushButtonSelectWorkingDirectory->setEnabled(m_settings->useCustomWorkingDir);

    m_widget->ui->lineEditCustomDirectoryPath->setText(m_settings->customWorkingDir);

    m_widget->ui->lineEditCustomSimianPath->setText(m_settings->simianExecutablePath);

    m_widget->ui->threshold->setValue(m_settings->threshold);
    m_widget->ui->lineEditExcludes->setText(m_settings->excludes);

    m_widget->ui->ignoreCharacterCase->setChecked(m_settings->flags[QString::fromUtf8("ignoreCharacterCase")]);
    m_widget->ui->ignoreCharacters->setChecked(m_settings->flags[QString::fromUtf8("ignoreCharacters")]);
    m_widget->ui->ignoreStringCase->setChecked(m_settings->flags[QString::fromUtf8("ignoreStringCase")]);
    m_widget->ui->ignoreStrings->setChecked(m_settings->flags[QString::fromUtf8("ignoreStrings")]);
    m_widget->ui->ignoreNumbers->setChecked(m_settings->flags[QString::fromUtf8("ignoreNumbers")]);
    m_widget->ui->ignoreLiterals->setChecked(m_settings->flags[QString::fromUtf8("ignoreLiterals")]);
    m_widget->ui->ignoreIdentifierCase->setChecked(m_settings->flags[QString::fromUtf8("ignoreIdentifierCase")]);
    m_widget->ui->ignoreIdentifiers->setChecked(m_settings->flags[QString::fromUtf8("ignoreIdentifiers")]);
    m_widget->ui->ignoreModifiers->setChecked(m_settings->flags[QString::fromUtf8("ignoreModifiers")]);
    m_widget->ui->ignoreCurlyBraces->setChecked(m_settings->flags[QString::fromUtf8("ignoreCurlyBraces")]);
    m_widget->ui->ignoreOverlappingBlocks->setChecked(m_settings->flags[QString::fromUtf8("ignoreOverlappingBlocks")]);
    m_widget->ui->balanceParentheses->setChecked(m_settings->flags[QString::fromUtf8("balanceParentheses")]);
    m_widget->ui->balanceSquareBrackets->setChecked(m_settings->flags[QString::fromUtf8("balanceSquareBrackets")]);

}

void OptionsPage::updateSettingsToCurrentProject()
{
    m_settings->setSettingsToUseCustomDir(false);
    m_settings->load();

    initUiFromSettings();
}

void OptionsPage::updateSettingsToCustomDir(const QString &customDirectoryPath)
{
    m_settings->setSettingsToUseCustomDir(true);
    m_settings->setCustomWorkingDir(customDirectoryPath);
    m_settings->load();
    initUiFromSettings();
}

} // namespace Internal
} // namespace Todo

