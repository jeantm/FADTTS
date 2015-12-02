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

    void SetProcessing( Processing *newProcessing );

    void SetMatlabScript( MatlabScript *newMatlabScript );

signals:
    
public slots:

private:
    void run();


    QString m_outputDir, m_fiberName;

    Processing *m_processing;

    MatlabScript *m_matlabScript;

    QFile *m_logFile;

};

#endif // MATLABTHREAD_H
