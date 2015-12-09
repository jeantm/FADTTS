#ifndef MATLABTHREAD_H
#define MATLABTHREAD_H

#include "Processing.h"
#include "MatlabScript.h"

#include <QThread>


class MatlabThread : public QThread
{
    Q_OBJECT

public:
    explicit MatlabThread( QObject *parent = 0 );

    void SetProcessing( Processing *processing );

    void SetMatlabScript( MatlabScript *matlabScript );

    void SetMatlabScriptPath( QString matlabScriptPath );

    void SetMatlabExe( QString matlabExe );

    void SetLogFile( QFile *logFile );


    void terminate();


signals:

public slots:

private:
    void run();


    void RedirectOutput();

    void RunScript();


    QString m_outputDir, m_fiberName, m_matlabExe, m_matlabScriptPath;

    Processing *m_processing;

    MatlabScript *m_matlabScript;

    QFile *m_logFile;

    QProcess *m_process;

};

#endif // MATLABTHREAD_H
