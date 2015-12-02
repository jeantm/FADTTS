#include "MatlabThread.h"
#include <iostream>

MatlabThread::MatlabThread(QObject *parent) :
    QThread(parent)
{
}

void MatlabThread::SetProcessing( Processing *newProcessing )
{
    m_processing = newProcessing;
}

void MatlabThread::SetMatlabScript( MatlabScript *newMatlabScript )
{
    m_matlabScript = newMatlabScript;
}

void MatlabThread::run()
{
    QString matlabScript = m_matlabScript->GenerateMatlabFiles();
    m_processing->SetMatlabScript( matlabScript );
    m_processing->RunScript();
}
