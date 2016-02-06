#include "MatlabThread.h"
#include <iostream>

MatlabThread::MatlabThread(QObject *parent) :
    QThread(parent)
{
}


/*****************************************************/
/***************** Public  Functions *****************/
/*****************************************************/
void MatlabThread::SetMatlabScript( MatlabScript *matlabScript )
{
    m_matlabScript = matlabScript;
}

void MatlabThread::SetMatlabScriptPath( QString matlabScriptPath )
{
    m_matlabScriptPath = matlabScriptPath;
}

void MatlabThread::SetMatlabExe( QString matlabExe )
{
    m_matlabExe = matlabExe;
}

void MatlabThread::SetLogFile( QFile *logFile )
{
    m_logFile = logFile;
}

void MatlabThread::SetRunMatlab( bool runMatlab )
{
    m_runMatlab = runMatlab;
}

void MatlabThread::SetRunMatlabOnKD( bool runMatlabOnKD )
{
    m_runMatlabOnKD = runMatlabOnKD;
}

void MatlabThread::SetQueueKD( QString queueKD )
{
    m_queueKD = queueKD;
}

void MatlabThread::SetAllocatedMemoryKD( int allocatedMemoryKD )
{
    m_allocatedMemoryKD = allocatedMemoryKD;
}


void MatlabThread::terminate()
{
    m_process->terminate();
    QThread::terminate();
}


/*****************************************************/
/***************** Private Functions *****************/
/*****************************************************/
void MatlabThread::RedirectOutput()
{
    m_process = new QProcess();
    m_process->setProcessChannelMode( QProcess::MergedChannels );
    m_process->setStandardOutputFile( m_logFile->fileName(), QProcess::Append );
}

void MatlabThread::RunScript()
{
    RedirectOutput();

    QStringList arguments;
    QString mScript = "run('" + m_matlabScriptPath + "')";
    std::cout << mScript.toStdString() << std::endl;
    if( m_runMatlabOnKD )
    {
        arguments << ( QString( "bsub -q " + m_queueKD + "-M " + m_allocatedMemoryKD ) );
    }
    arguments << "-nosplash" << "-nodesktop" << QString( "-r \"try, " + mScript + "; catch, disp('failed'), end, quit\"" ) << "-logfile matlabLog.out";

    m_process->start( m_matlabExe, arguments );
    m_process->waitForFinished();
}


void MatlabThread::run()
{
    QString matlabScriptPath = m_matlabScript->GenerateMatlabFiles();
    if( m_runMatlab )
    {
        SetMatlabScriptPath( matlabScriptPath );
        RunScript();
    }
}
