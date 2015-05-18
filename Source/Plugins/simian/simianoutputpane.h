#ifndef SIMIANOUTPUTPANE_H
#define SIMIANOUTPUTPANE_H

#include "settings.h"
#include <coreplugin/ioutputpane.h>

QT_BEGIN_NAMESPACE
class QToolButton;
class QTreeWidget;
class QTreeWidgetItem;
class QLabel;
QT_END_NAMESPACE

namespace Simian {
namespace Internal {

class SimianOutputPane : public Core::IOutputPane
{
    Q_OBJECT

public:
    SimianOutputPane(Settings *settings,/*TodoItemsModel *todoItemsModel, */QObject *parent = 0);
    ~SimianOutputPane();

    QWidget *outputWidget(QWidget *parent);
    QList<QWidget*> toolBarWidgets() const;
    QString displayName() const;
    int priorityInStatusBar() const;
    void clearContents();
    void visibilityChanged(bool visible);
    void setFocus();
    bool hasFocus() const;
    bool canFocus() const;
    bool canNavigate() const;
    bool canNext() const;
    bool canPrevious() const;
    void goToNext();
    void goToPrev();

signals:
    void simianItemTriggered(QString filePath, int startLine, int endLine);
    void outputPanelContentCleared();

public slots:
    void doSimian();

private:
    QTreeWidget *m_treeWidget;
    QToolButton *m_doSimian;
    QLabel *m_simianStatisticsLabel;
    Settings *settings;

    QStringList getArguments();

private slots:
    void onDuplicateItemDoubleClicked(QTreeWidgetItem* item,int column);
    void postErrorMessage(const QString &msgText);
    void clearOutput();
};

} // namespace Internal
} // namespace Simian

#endif // SIMIANOUTPUTPANE_H
