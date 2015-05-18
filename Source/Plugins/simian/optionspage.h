#ifndef OPTIONSPAGE_H
#define OPTIONSPAGE_H

#include "settings.h"
#include <coreplugin/dialogs/ioptionspage.h>
#include <QPointer>

namespace Simian {
namespace Internal {

class OptionsDialog;
class Settings;

class OptionsPage : public Core::IOptionsPage
{
    Q_OBJECT

public:
    OptionsPage(Settings *settings, QObject *parent = 0);
    QWidget *widget();
    void apply();
    void finish();

private:
    QPointer<OptionsDialog> m_widget;
    Settings *m_settings;
    void initUiFromSettings();

private slots:
    void updateSettingsToCurrentProject();
    void updateSettingsToCustomDir(const QString &customDirectoryPath);
};

} // namespace Internal
} // namespace Simian

#endif // OPTIONSPAGE_H
