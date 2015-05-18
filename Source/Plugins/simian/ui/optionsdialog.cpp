#include "optionsdialog.h"
#include "ui_optionsdialog.h"

#include <QFileDialog>

namespace Simian {
namespace Internal {

OptionsDialog::OptionsDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);

    connect(ui->pushButtonSelectCustomSimianExePath, SIGNAL(clicked()), this, SLOT(onBrowseSimianPath()));
    connect(ui->pushButtonSelectWorkingDirectory, SIGNAL(clicked()), this, SLOT(onBrowseCustomDirectoryPath()));

    connect(ui->radioButtonUseProjectWorkingDir, SIGNAL(clicked()), this, SLOT(onWorkingDirectoryTypeChanged()));
    connect(ui->radioButtonUseCustomWorkingDir, SIGNAL(clicked()), this, SLOT(onWorkingDirectoryTypeChanged()));
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::onBrowseSimianPath()
{
    QFileDialog fDialog(this, Qt::Dialog);
    fDialog.setFileMode(QFileDialog::ExistingFile);
    QString simianExeName = fDialog.getOpenFileName(this, tr("Select Simian Exe File"), QDir::rootPath() , tr("Exe Files (*.exe)"));
    ui->lineEditCustomSimianPath->setText(simianExeName);
}

void OptionsDialog::onBrowseCustomDirectoryPath()
{
    QFileDialog fDialog(this, Qt::Dialog);
    fDialog.setFileMode(QFileDialog::Directory);
    fDialog.setOption(QFileDialog::ShowDirsOnly, true);
    QString workingDirectoryPath = fDialog.getExistingDirectory(this, tr("Select Working Dierctory"), QDir::rootPath() );
    ui->lineEditCustomDirectoryPath->setText(workingDirectoryPath);
    emit customDirectoryPathChanged(ui->lineEditCustomDirectoryPath->text());
}

void OptionsDialog::onWorkingDirectoryTypeChanged()
{

    if (ui->radioButtonUseProjectWorkingDir->isChecked())
    {
        ui->lineEditCustomDirectoryPath->setEnabled(false);
        ui->pushButtonSelectWorkingDirectory->setEnabled(false);
        emit workingDirectoryChangedToProjectDir();
    }
    else
    {

        ui->lineEditCustomDirectoryPath->setEnabled(true);
        ui->pushButtonSelectWorkingDirectory->setEnabled(true);

        if (!ui->lineEditCustomDirectoryPath->text().isEmpty())
        {
            emit customDirectoryPathChanged(ui->lineEditCustomDirectoryPath->text());
            return;
        }

        onBrowseCustomDirectoryPath();
        if(ui->lineEditCustomDirectoryPath->text().isEmpty())
        {
            ui->radioButtonUseCustomWorkingDir->toggle();
        }
        else
        {
            emit customDirectoryPathChanged(ui->lineEditCustomDirectoryPath->text());
        }
    }
}


}// Internal
}// Simian
