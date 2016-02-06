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


    void SetMatlabScript( MatlabScript *matlabScript );

    void SetMatlabScriptPath( QString matlabScriptPath );

    void SetMatlabExe( QString matlabExe );

    void SetLogFile( QFile *logFile );

    void SetRunMatlab( bool runMatlab );

    void SetRunMatlabOnKD( bool runMatlabOnKD );

    void SetQueueKD( QString queueKD );

    void SetAllocatedMemoryKD( int allocatedMemoryKD );


    void terminate();


private:
    void RedirectOutput();

    void RunScript();


    void run();


    MatlabScript *m_matlabScript;

    QFile *m_logFile;

    QProcess *m_process;

    QString m_matlabExe, m_matlabScriptPath, m_queueKD;

    int m_allocatedMemoryKD;

    bool m_runMatlab, m_runMatlabOnKD;
};

#endif // MATLABTHREAD_H
