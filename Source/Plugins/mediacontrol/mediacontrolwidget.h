#ifndef MEDIACONTROLWIDGET_H
#define MEDIACONTROLWIDGET_H

#include <QWidget>

class QLabel;
class QToolButton;

namespace MediaControl {
namespace Internal {

class MediaControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MediaControlWidget(QWidget *parent = 0);

private:
    QToolButton *playPause;
    QToolButton *forward;
    QToolButton *backward;

signals:
    void playPauseClicked();
    void forwardClicked();
    void backwardClicked();

private slots:
    //void playPausePressed();
    //void forwardPressed();
    //void backwardPressed();

};
} // namespace Internal
} // namespace MediaControl

#endif // MEDIACONTROLWIDGET_H
