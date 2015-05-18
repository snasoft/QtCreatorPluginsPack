#include "simianoutputpane.h"

#include <QIcon>
#include <QHeaderView>
#include <QToolButton>
#include <QTreeWidget>
#include <QStringList>
#include <QLabel>

#include <coreplugin/icore.h>

#include <projectexplorer/projectexplorer.h> //получение текущего проекта
#include <projectexplorer/project.h> //получение директории текущего проекта
#include <projectexplorer/projecttree.h>

#include <QProcess> //запуск simian.exe
#include <QtXml> //парсинг XML - отчета simian.exe

namespace Simian {
namespace Internal {

SimianOutputPane::SimianOutputPane(Settings *settings, QObject *parent) :
    IOutputPane(parent)
{

    m_treeWidget = new QTreeWidget();
    m_treeWidget->setHeaderHidden(true);
    connect(m_treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(onDuplicateItemDoubleClicked(QTreeWidgetItem*,int)));

    m_simianStatisticsLabel = new QLabel();

    m_doSimian = new QToolButton();
    m_doSimian->setIcon(QIcon(QString::fromUtf8(":/icons/icons/find.png")));
    connect(m_doSimian, SIGNAL(clicked()), this, SLOT(doSimian()));

    this->settings = settings;
}

SimianOutputPane::~SimianOutputPane()
{
    //TODO: free resouces
}

QWidget *SimianOutputPane::outputWidget(QWidget *parent)
{
    Q_UNUSED(parent)
    return m_treeWidget;
}

QList<QWidget*> SimianOutputPane::toolBarWidgets() const
{
    return QList<QWidget*>()
            << m_doSimian
            << m_simianStatisticsLabel;
}

QString SimianOutputPane::displayName() const
{
    return tr("Simian");
}

int SimianOutputPane::priorityInStatusBar() const
{
    return 0;
}

void SimianOutputPane::clearContents()
{

    clearOutput();
    emit outputPanelContentCleared();
}


void SimianOutputPane::visibilityChanged(bool visible)
{
    Q_UNUSED(visible)
}

void SimianOutputPane::setFocus()
{
    m_treeWidget->setFocus();
}

bool SimianOutputPane::hasFocus() const
{
    return m_treeWidget->hasFocus();
}

bool SimianOutputPane::canFocus() const
{
    return true;
}

bool SimianOutputPane::canNavigate() const
{
    return false;
    //@ TODO: navigation on list
}

bool SimianOutputPane::canNext() const
{

    return true;
    //TODO: check is item last
}

bool SimianOutputPane::canPrevious() const
{
    return true;
    //TODO: check is item first
}

void SimianOutputPane::goToNext()
{
    //TODO: do next simian item (QTreeWidgetItem) current
}

void SimianOutputPane::goToPrev()
{
    //TODO: do prev simian item (QTreeWidgetItem) current
}

void SimianOutputPane::onDuplicateItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(!item->parent())
        return;
    QString filePath = item->data(0, Qt::UserRole).toString();
    int startLine = item->data(1, Qt::UserRole).toInt();
    int endLine = item->data(2, Qt::UserRole).toInt();

    emit simianItemTriggered(filePath, startLine, endLine);
}
void SimianOutputPane::postErrorMessage(const QString &msgText)
{
    clearOutput();
    QTreeWidgetItem *errorItem;
    errorItem = new QTreeWidgetItem();
    errorItem->setData(0, Qt::DisplayRole, msgText);
    errorItem->setIcon(0, QIcon(QString::fromUtf8(":/icons/icons/error.png")));
    this->m_treeWidget->addTopLevelItem(errorItem);
}

void SimianOutputPane::clearOutput()
{
    m_treeWidget->clear();
    m_simianStatisticsLabel->setText(QString::fromUtf8(""));
    this->setBadgeNumber(0);
}

QStringList SimianOutputPane::getArguments()
{
    QStringList arguments;

    arguments << QString::fromUtf8("-failOnDuplication-");

#ifdef TEST
    arguments << QString::fromUtf8("-formatter=xml:D:/QtProjects/foo.xml");// + whereToSaveXML;// << "fusion";
#else
    arguments << QString::fromUtf8("-formatter=xml");
#endif

    arguments << QString::fromUtf8("-includes=**/*.cpp");
    arguments << QString::fromUtf8("-includes=*.cpp");
    arguments << QString::fromUtf8("-includes=**/*.h");
    arguments << QString::fromUtf8("-includes=*.h");

    if(!settings->excludes.isEmpty())
    {
        QStringList excludes = settings->excludes.split(QString::fromUtf8(";"));
        for (int i = 0 ; i < excludes.count() ; i++)
        {
            QString argument = QString::fromUtf8("-excludes=") + excludes.at(i);
            arguments << argument;
        }
    }
    QString threshold = QString::fromUtf8("-threshold=") + QString::number(settings->threshold);
    arguments << threshold;

    QMapIterator<QString, bool> i(settings->flags);
    while (i.hasNext()) {
        i.next();
        QString flag = QString::fromUtf8("-");
        if (i.value() == true)
        {
            flag = QString::fromUtf8("+");
        }
        QString argument = QString::fromUtf8("-") + i.key() + flag;
        arguments << argument;
    }


    return arguments;
}

void SimianOutputPane::doSimian()
{
    clearOutput();
    settings->load();

    //получаем текущую директорию
    ProjectExplorer::ProjectExplorerPlugin *explorer = ProjectExplorer::ProjectExplorerPlugin::instance();
    //connect(explorer, SIGNAL(currentProjectChanged(ProjectExplorer::Project*)), this, SLOT(onCurrentProjectChanged(ProjectExplorer::Project*)));

    if (ProjectExplorer::ProjectTree::currentProject() == NULL && !settings->useCustomWorkingDir)
    {
        postErrorMessage(QString::fromUtf8("There is no targets for simian. Open project or set custom directory in Options."));
        return; //текущего проекта нет и в настройках используется текущий проект
    }
    if (settings->useCustomWorkingDir && settings->customWorkingDir.isEmpty())
    {
        postErrorMessage(QString::fromUtf8("You must spicify custom directory path in Options."));
        return; //текущего проекта нет и в настройках используется текущий проект
    }

    QString workingDirectoryPath;
    if (settings->useCustomWorkingDir)
    {
        workingDirectoryPath = settings->customWorkingDir;
    }
    else
    {
        workingDirectoryPath = ProjectExplorer::ProjectTree::currentProject()->projectDirectory().toString();
    }
    qDebug() << QString::fromUtf8("Working DIR ") << workingDirectoryPath;

    QDir workingDirectory(workingDirectoryPath);
    if(!workingDirectory.exists())
    {
        postErrorMessage(QString::fromUtf8("Working directory does not exists."));
        return; //текущего проекта нет и в настройках используется текущий проект
    }

    if (settings->simianExecutablePath.isEmpty())
    {
        postErrorMessage(QString::fromUtf8("You must spicify simian path in Options."));
        return; //текущего проекта нет и в настройках используется текущий проект
    }

    QString program = settings->simianExecutablePath ; //Аргумент запуска - исполняемая программа

    QProcess *simianProcess = new QProcess();
    simianProcess->setProcessChannelMode(QProcess::MergedChannels);
    simianProcess->setWorkingDirectory(workingDirectoryPath);

    simianProcess->start(program, getArguments() );
    qDebug() << getArguments();

    if(!simianProcess->waitForStarted())
    {
        //return 0;
        QString errorText = QString::fromUtf8("Simian unable to start. Set executables path in Options. Error string: ") + simianProcess->errorString();
        postErrorMessage(errorText);
    }
    if(simianProcess->error() == QProcess::FailedToStart) //
    {
        QString errorText = QString::fromUtf8("Simian failed to start. Set executables path in Options. Error string: ") + simianProcess->errorString();
        postErrorMessage(errorText);

        return; //текущего проекта нет и в настройках используется текущий проект
        //return 0;
    }

    QByteArray data;
    while(simianProcess->waitForReadyRead())
        data.append(simianProcess->readAll());
    data.remove(0, data.indexOf("<"));

#ifdef TEST
    QFile xmlOutput(whereToSaveXML);
    if (!xmlOutput.open(QIODevice::WriteOnly))
        return;
    xmlOutput.write(data);
    xmlOutput.close();
#endif

    m_treeWidget->setColumnCount(3);
    m_treeWidget->hideColumn(1);
    m_treeWidget->hideColumn(2);

    QXmlStreamReader xml;

    xml.addData(data);
    QTreeWidgetItem *simianItem;
    QTreeWidgetItem *blockItem;
    while (!xml.atEnd()) {
        xml.readNextStartElement();
        if (xml.name() == QStringLiteral("set") && xml.tokenType() == QXmlStreamReader::StartElement)
        {
            simianItem = new QTreeWidgetItem();
            QString lineCount = xml.attributes().value(QString::fromUtf8("lineCount")).toString();
            QString simianItemText = tr("Found ") + lineCount + tr(" duplicate lines in the following files:");
            simianItem->setData(0, Qt::DisplayRole,  simianItemText );
            m_treeWidget->addTopLevelItem(simianItem);
        }
        if (xml.name() == QStringLiteral("block") && xml.tokenType() == QXmlStreamReader::StartElement )
        {
            blockItem = new QTreeWidgetItem();
            QString sourceFile = xml.attributes().value(QString::fromUtf8("sourceFile")).toString();
            QString startLineNumber = xml.attributes().value(QString::fromUtf8("startLineNumber")).toString();
            QString endLineNumber = xml.attributes().value(QString::fromUtf8("endLineNumber")).toString();
            QString blockItemText =  sourceFile + tr(" found duplicate lines beetwen ") + startLineNumber + tr(" and ") + endLineNumber;

            blockItem->setData(0, Qt::DisplayRole, blockItemText );
            blockItem->setData(0, Qt::UserRole, sourceFile );
            blockItem->setData(1, Qt::UserRole, startLineNumber.toInt());
            blockItem->setData(2, Qt::UserRole, endLineNumber.toInt());

            simianItem->addChild(blockItem);
        }
        if (xml.name() == QStringLiteral("summary") && xml.tokenType() == QXmlStreamReader::StartElement )
        {
            QString duplicateFileCount = xml.attributes().value(QString::fromUtf8("duplicateFileCount")).toString();
            QString duplicateLineCount = xml.attributes().value(QString::fromUtf8("duplicateLineCount")).toString();
            QString duplicateBlockCount = xml.attributes().value(QString::fromUtf8("duplicateBlockCount")).toString();
            QString totalFileCount = xml.attributes().value(QString::fromUtf8("totalFileCount")).toString();
            QString totalRawLineCount = xml.attributes().value(QString::fromUtf8("totalRawLineCount")).toString();
            QString totalSignificantLineCount = xml.attributes().value(QString::fromUtf8("totalSignificantLineCount")).toString();
            QString processingTime = xml.attributes().value(QString::fromUtf8("processingTime")).toString();

            QString simianStatusMessage = tr("Duplicate count: ") +
                    duplicateBlockCount + tr(" blocks, ") +
                    duplicateLineCount + tr(" lines in ") +
                    duplicateFileCount + tr(" files. ") +
                    tr("Total count: ") + totalSignificantLineCount +
                    tr(" lines (") + totalRawLineCount + tr(" raw) in") +
                    totalFileCount + tr(" files ") +
                    tr(" at ") + processingTime + tr(" ms.");



            m_simianStatisticsLabel->setText(simianStatusMessage);
            this->setBadgeNumber(duplicateBlockCount.toInt());
        }
    }
    m_treeWidget->expandAll();
}

} // namespace Internal
} // namespace Simian
