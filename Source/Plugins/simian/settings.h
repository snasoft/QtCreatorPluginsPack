#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QString>
#include <QMap>

namespace Simian {
namespace Internal {

class Settings {
public:
    explicit Settings();

    QString simianExecutablePath; //путь к исполняемому файлу

    bool useCustomWorkingDir; //использовать директорию иную, чем директория текущего проекта
    QString customWorkingDir; //директория

    //настройки для запуска командной строки
    QMap<QString, bool> flags; //булевые флаги
    QString excludes; //исключения
    int threshold;

private:
    QMap<QString, bool> defaultFlags; //булевые флаги по-умолчанию
    QSettings *globalScopeSettings; //глобальные настройки
    QSettings *projectScopeSettings;//настройки, касающиеся отдельного задания simian

    void initDefaults();

public slots:
    void setToDefaults();

    void load();
    void save();

    void setSettingsToUseCustomDir(const bool &useCustomDir);
    void setCustomWorkingDir(const QString &customDirPath);
};

} // Internal
} // Simian
#endif // SETTINGS_H
