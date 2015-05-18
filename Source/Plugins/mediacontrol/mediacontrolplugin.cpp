#include "mediacontrolplugin.h"
#include "mediacontrolconstants.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>

#include <coreplugin/modemanager.h> //for adding widget
#include "mediacontrolwidget.h"

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>
#include <QHBoxLayout>
#include <QLabel>

#include <QtPlugin>

//platforms specific
#ifdef Q_OS_WIN32
    #define WINVER 0x0500
    #include <Windows.h>
#endif

#ifdef Q_OS_LINUX
    #include <X11/Xlib.h>
    #include <X11/extensions/XTest.h>

    #define XF86AudioPlay 0x1008ff14
    #define XF86AudioNext 0x1008ff17
    #define XF86AudioPrev 0x1008ff16
#endif

#ifdef Q_OS_MAC
    #include <ApplicationServices/ApplicationServices.h> //for UInt8
    #include <IOKit/hidsystem/ev_keymap.h> //key codes definitions
    #include "macx.h" //Objective-C functions call (from macx.mm)
#endif
//end platforms specific

using namespace MediaControl::Internal;

MediaControlPlugin::MediaControlPlugin()
{

}

MediaControlPlugin::~MediaControlPlugin()
{

}

bool MediaControlPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    mediaControl = new MediaControlWidget();

    connect(mediaControl, SIGNAL(backwardClicked()), this, SLOT(playPrevious()));
    connect(mediaControl, SIGNAL(playPauseClicked()), this, SLOT(playPauseToogle()));
    connect(mediaControl, SIGNAL(forwardClicked()), this, SLOT(playNext()));

    Core::ModeManager::addWidget(mediaControl);

    return true;
}

void MediaControlPlugin::extensionsInitialized()
{

}

ExtensionSystem::IPlugin::ShutdownFlag MediaControlPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

void MediaControlPlugin::playPauseToogle()
{
    sendKeyEventToSystem(Qt::Key_MediaTogglePlayPause);
}

void MediaControlPlugin::playNext()
{
    sendKeyEventToSystem(Qt::Key_MediaNext);
}

void MediaControlPlugin::playPrevious()
{
    sendKeyEventToSystem(Qt::Key_MediaPrevious);
}

void MediaControlPlugin::sendKeyEventToSystem(Qt::Key qtKey)
{
#ifdef Q_OS_WIN32
    INPUT ip;

    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    switch (qtKey) {
    case Qt::Key_MediaPrevious:
        ip.ki.wVk = VK_MEDIA_PREV_TRACK;
        break;
    case Qt::Key_MediaTogglePlayPause:
        ip.ki.wVk = VK_MEDIA_PLAY_PAUSE;
        break;
    case Qt::Key_MediaNext:
        ip.ki.wVk = VK_MEDIA_NEXT_TRACK;
        break;
    default:
        return;
        break;
    }

    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

#endif
    
#ifdef Q_OS_LINUX
    unsigned int key;
    unsigned int keycode;
    switch (qtKey) {
    case Qt::Key_MediaPrevious:
        key = XF86AudioPrev;
        break;
    case Qt::Key_MediaTogglePlayPause:
        key = XF86AudioPlay;
        break;
    case Qt::Key_MediaNext:
        key = XF86AudioNext;
        break;
    default:
        return;
        break;
    }
    // Connect to X
    Display *display;
    display = XOpenDisplay(NULL);

    // Get the keycode
    keycode = XKeysymToKeycode(display, key);
    // Simulate the keypress
    XTestFakeKeyEvent(display, keycode, 1, 0);
    // Release the key
    XTestFakeKeyEvent(display, keycode, 0, 0);

    // Clear the X buffer which actually sends the key press
    XFlush(display);

    // Disconnect from X
    XCloseDisplay(display);

#endif

#ifdef Q_OS_MAC
    switch (qtKey) {
    case Qt::Key_MediaPrevious:
        HIDPostAuxKey( NX_KEYTYPE_NEXT );
        break;
    case Qt::Key_MediaTogglePlayPause:
        HIDPostAuxKey( NX_KEYTYPE_NEXT );
        break;
    case Qt::Key_MediaNext:
        HIDPostAuxKey( NX_KEYTYPE_NEXT );
        break;
    default:
        return;
        break;
    }
#endif
}

