#ifndef OPTIONSPAGE_H
#define OPTIONSPAGE_H

#include <coreplugin/dialogs/ioptionspage.h>
#include <QPointer>

class OptionsDialog;
class QSettings;
class QSpinBox;
class QLineEdit;

namespace HabraStata {
namespace Internal {

class OptionsPage : public Core::IOptionsPage
{
    Q_OBJECT

public:
    OptionsPage(QSettings *settings, QObject *parent = 0);
    QWidget *createPage(QWidget *parent); //для QtCreator 3.0.0
    QWidget *widget(); //для QtCreator более 3.0.0
    void apply();
    void finish();

private:
    QWidget *m_widget;
    void initUiFromSettings();

    QSettings *settings;

    QSpinBox *updateIntervalInMinutes;
    QLineEdit *username;
signals:
    void settingsApplyed();
};

} // namespace Internal
} // namespace HabraStata

#endif // OPTIONSPAGE_H
