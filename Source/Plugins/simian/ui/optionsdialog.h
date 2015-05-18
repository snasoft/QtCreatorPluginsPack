#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QWidget>

namespace Simian {
namespace Internal {

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QWidget
{
    Q_OBJECT

public:
    explicit OptionsDialog(QWidget *parent = 0);
    ~OptionsDialog();

    Ui::OptionsDialog *ui;

private:

public slots:
    void onWorkingDirectoryTypeChanged();

private slots:
    void onBrowseSimianPath();
    void onBrowseCustomDirectoryPath();


signals:
    void customDirectoryPathChanged(const QString &customDirectoryPath);
    void workingDirectoryChangedToProjectDir();
};

} // Internal
} // Simian
#endif // OPTIONSDIALOG_H
