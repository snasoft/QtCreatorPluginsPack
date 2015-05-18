#ifndef CPPCHECKRUNNER_H
#define CPPCHECKRUNNER_H

#include <QProcess>
#include <QTimer>

namespace QtcGtest {
  namespace Internal {

    class Settings;

    /*!
     * \brief Cppcheck binary runner.
     *  Does not have ownership on settings_ (must be destroyed before settings).
     * Launches, finishes, reads result, passes start arguments, etc.
     */
    class CppcheckRunner : public QObject
    {
        Q_OBJECT
      public:
        explicit CppcheckRunner(Settings* settings, QObject *parent = 0);
        ~CppcheckRunner ();

        //! Add files to check queue. Terminate current check if forced is true.
        void checkFiles (const QStringList& fileNames, bool forced);

        //! Update data based on current settings_.
        void updateSettings ();

      signals:
        //! New task has been generated.
        void newTask (char type, const QString &description,
                      const QString& fileName, int line);

      private slots:
        //! Check files from queue.
        void checkQueuedFiles ();

        // QProcess handling.
        void readOutput ();
        void readError ();
        void started ();
        void error (QProcess::ProcessError error);
        void finished (int exitCode, QProcess::ExitStatus exitStatus);

      private:
        //! Timer to delay queue checking.
        QTimer queueTimer_;
        //! Binary runner.
        QProcess process_;
        //! Plugin's settings.
        Settings* settings_;
        //! Binary run arguments.
        QStringList runArguments_;
        //! Queued list of files to check.
        QStringList fileCheckQueue_;
        //! List of files currently being checked.
        QStringList currentlyCheckingFiles_;
        //! Should print process' output to MessageManager or not.
        bool showOutput_;
    };

  } // namespace Internal
} // namespace QtcCppcheck


#endif // CPPCHECKRUNNER_H
