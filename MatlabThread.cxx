#include "MatlabThread.h"
#include <iostream>
#include <QDebug>

MatlabThread::MatlabThread(QObject *parent) :
    QThread(parent)
{
}

void MatlabThread::SetProcess( Processing *newProcess )
{
    m_process = newProcess;
}

void MatlabThread::SetMatlabScript( MatlabScript *newMatlabScript )
{
    m_matlabScript = newMatlabScript;
}

void MatlabThread::run()
{
//    for( int i = 0; i < 100; i++ )
//    {
//        std::cout << "Perm: " << i << std::endl;
//        sleep( 1 );
//    }

    QString matlabScript = m_matlabScript->GenerateMatlabFiles();
    m_process->SetMatlabScript( matlabScript );
    m_process->RunScript();
}
