#include "MatlabThread.h"
#include <iostream>

MatlabThread::MatlabThread(QObject *parent) :
    QThread(parent)
{
}


/*****************************************************/
/***************** Public  Functions *****************/
/*****************************************************/
void MatlabThread::SetProcessing( Processing *processing )
{
    m_processing = processing;
}

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


void MatlabThread::terminate()
{
    m_process->terminate();
    QThread::terminate();
}


/*****************************************************/
/***************** Private Functions *****************/
/*****************************************************/
void MatlabThread::run()
{
    QString matlabScriptPath = m_matlabScript->GenerateMatlabFiles();
    SetMatlabScriptPath( matlabScriptPath );
    RunScript();
}

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
//    QString mScript = "run('" + m_matlabScript + "')";
    QString mScript = "run('/work/jeantm/Project/FADTTS_Test/test.m')";
    std::cout << mScript.toStdString() << std::endl;
    arguments << "-nosplash" << "-nodesktop" << QString( "-r \"try, " + mScript + "; catch, disp('failed'), end, quit\"" );

//    m_process->start( m_matlabExe, arguments );
    m_process->start( "/opt/matlab/bin/matlab", arguments );
    m_process->waitForFinished();
}
