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

    void SetProcess( Processing *newProcess );

    void SetMatlabScript( MatlabScript *newMatlabScript );

signals:
    
public slots:

private:
    void run();

    bool done;

    Processing *m_process;

    MatlabScript *m_matlabScript;

};

#endif // MATLABTHREAD_H
