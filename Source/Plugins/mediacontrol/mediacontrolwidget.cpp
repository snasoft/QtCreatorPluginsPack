#include "mediacontrolwidget.h"

#include <QToolButton>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>

using namespace MediaControl::Internal;

MediaControlWidget::MediaControlWidget(QWidget *parent) :
    QWidget(parent)
{
    playPause = new QToolButton();
    forward = new QToolButton();
    backward = new QToolButton();

    playPause->setMaximumWidth(22);
    forward->setMaximumWidth(20);
    backward->setMaximumWidth(20);

    playPause->setMaximumHeight(20);
    forward->setMaximumHeight(20);
    backward->setMaximumHeight(20);

    playPause->setObjectName(QString::fromUtf8("playPause"));
    forward->setObjectName(QString::fromUtf8("forward"));
    backward->setObjectName(QString::fromUtf8("backward"));

    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setSpacing(0);
    controlLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(controlLayout);

    controlLayout->addWidget(backward);
    controlLayout->addWidget(playPause);
    controlLayout->addWidget(forward);

    connect(backward, SIGNAL(clicked()), this, SIGNAL(backwardClicked()));
    connect(playPause, SIGNAL(clicked()), this, SIGNAL(playPauseClicked()));
    connect(forward, SIGNAL(clicked()), this, SIGNAL(forwardClicked()));

    QString qssStyle;
    QFile styleFile(QString::fromUtf8(":/styles/styles/toolButtonStyles.qss"));
    styleFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream out(&styleFile);
    qssStyle = out.readAll();
    this->setStyleSheet( qssStyle );
}
