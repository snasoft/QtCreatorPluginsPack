#include "simianplugin.h"
#include "simianconstants.h"
#include "simianoutputpane.h"
#include "optionspage.h"
#include "settings.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>

#include <texteditor/texteditor.h>

#include <coreplugin/editormanager/editormanager.h> //получение редактора
#include <coreplugin/editormanager/ieditor.h> //селекция текста (выделение)

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>
#include <QFileInfo>
#include <QPlainTextEdit>
#include <QtPlugin>
#include <QScrollBar>

using namespace Simian::Internal;

SimianPlugin::SimianPlugin() : m_simianOutputPane(0), m_optionsPage(0)
{
    settings = new Settings();
}

SimianPlugin::~SimianPlugin()
{
    //TODO: Unregister objects from the plugin manager's object pool
    //TODO: Delete members
}

bool SimianPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    QAction *action = new QAction(tr("Find similarities"), this);
    action->setIcon(QIcon(QString::fromUtf8(":/icons/icons/find.png")));
    Core::Command *cmd = Core::ActionManager::registerAction(action, Constants::ACTION_ID,
                                                             Core::Context(Core::Constants::C_GLOBAL));
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+S+F")));
    connect(action, SIGNAL(triggered()), this, SLOT(triggerAction()));
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addAction(cmd);

    createSimianOutputPane();
    createOptionsPage();

    return true;
}
void SimianPlugin::createOptionsPage()
{
    m_optionsPage = new OptionsPage(this->settings); //m_settings
    addAutoReleasedObject(m_optionsPage);
}

void SimianPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag SimianPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}
void SimianPlugin::triggerAction()
{
    m_simianOutputPane->showPage(1);
    doSimian();
}

void SimianPlugin::doSimian()
{
    m_simianOutputPane->doSimian();
}

void SimianPlugin::openFileAndHighlightDuplicateText(QString filePath, int startLine, int endLine)
{
    if (!QFileInfo(filePath).exists()) //если файла не существует
        return;//то уходим

    //Core::EditorManager *em = Core::EditorManager::instance();

    Core::IEditor *editor = Core::EditorManager::openEditor(filePath); //открываем файл

    if (!editor)
        return;

    editor->gotoLine(startLine);

    TextEditor::TextEditorWidget *ed = qobject_cast<TextEditor::TextEditorWidget *>(editor->widget());
    if(!ed)
        return;

    QTextCursor cursor =  ed->textCursor();

    QList<QTextEdit::ExtraSelection> selections; //список селекторов текста
    QTextEdit::ExtraSelection selection; //текущий селектор текста

    selection.format.setBackground(QBrush(QColor(255,0,0, 50))); //фон селекта (бледно-красный)
    selection.format.setProperty(QTextFormat::FullWidthSelection, true); //выделяем только текст, а не всю строку

    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 0); //устанавливаем курсор на первую строку документа
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, startLine-1); //переносим на первую строку фрагмента
    cursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, endLine - startLine + 1);//затем на последнюю строку фрагмента
    selection.cursor = cursor;

    selections.append(selection);
    ed->setExtraSelections(TextEditor::TextEditorWidget::DebuggerExceptionSelection, selections );
}

void SimianPlugin::onClear()
{
    //Core::EditorManager *em = Core::EditorManager::currentEditor();
    Core::IEditor *editor = Core::EditorManager::currentEditor();
    if(!editor)
        return;

    TextEditor::TextEditorWidget *ed = qobject_cast<TextEditor::TextEditorWidget *>(editor->widget());
    if(!ed)
        return;

    QList<QTextEdit::ExtraSelection> clearSelections; //пустой список

    ed->setExtraSelections(TextEditor::TextEditorWidget::DebuggerExceptionSelection, clearSelections );
    //plainTextEdit->setExtraSelections();
}

void SimianPlugin::createSimianOutputPane()
{
    m_simianOutputPane = new SimianOutputPane(this->settings);

    connect(m_simianOutputPane, SIGNAL(simianItemTriggered(QString,int,int)), this, SLOT(openFileAndHighlightDuplicateText(QString,int,int)));
    connect(m_simianOutputPane, SIGNAL(outputPanelContentCleared()), this, SLOT(onClear()));
    addAutoReleasedObject(m_simianOutputPane);
}
