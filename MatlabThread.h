#ifndef MATLABTHREAD_H
#define MATLABTHREAD_H

#include "Processing.h"

#include <iostream>

#include <QThread>
#include <QLocale>
#include <QResource>
#include <QTime>
#include <QMap>
#include <QDir>
#include <QTextStream>

#ifndef FADTTS_TITLE
#define FADTTS_TITLE "Unknown"
#endif

#ifndef FADTTS_VERSION
#define FADTTS_VERSION "unknown"
#endif

#ifndef FADTTS_CONTRIBUTORS
#define FADTTS_CONTRIBUTORS "unknown"
#endif

#ifndef FADTTS_DESCRIPTION
#define FADTTS_DESCRIPTION "No description available"
#endif

class MatlabThread : public QThread
{
    friend class TestMatlabThread; /** For unit tests **/
    Q_OBJECT

public:
    explicit MatlabThread( QObject *parent = 0 );


    /*************** Script ***************/
    void InitMatlabScript( QString outputDir, QString matlabScriptName ); // Tested


    void SetHeader(); // Tested


    void SetNbrCompThreads( bool isRunOnSystem , int nbrComp ); // Tested

    void SetMVCMPath( QString mvcmPath ); // Tested


    void SetFiberName( QString fiberName ); // Tested

    void SetDiffusionProperties( QStringList selectedPrefixes ); // Tested

    void SetInputFiles( QMap<int, QString> csvInputFiles ); // Tested

    void SetCovariates( QMap<int, QString> selectedCovariates ); // Tested


    void SetNbrPermutation( int nbrPermutation ); // Tested

    void SetOmnibus( bool omnibus ); // Tested

    void SetPostHoc( bool postHoc ); // Tested

    void SetConfidenceBandsThreshold( double confidenceBandsThreshold ); // Tested

    void SetPvalueThreshold( double pvalueThreshold ); // Tested


    /*************** Thread ***************/
    void SetLogFile( QFile *logFile );

    QString& SetMatlabExe();

    bool& SetRunMatlab();

    bool& SetRunMatlabOnKD();

    QString& SetQueueKD();

    int& SetAllocatedMemoryKD();


    void terminate();


private:
    static const QString m_csvSeparator;

    QFile *m_logFile;

    QProcess *m_process;

    QString m_matlabScript, m_outputDir, m_matlabScriptName,
    m_matlabExe, m_matlabScriptPath, m_queueKD;

    int m_allocatedMemoryKD;

    bool m_runMatlab, m_runMatlabOnKD;


    /*************** Script ***************/
    void GenerateMyFDR(); // Tested

    QString GenerateMatlabFiles(); // Tested


    /*************** Thread ***************/
    void RedirectOutput();

    void RunScript();


    void run();
};

#endif // MATLABTHREAD_H
