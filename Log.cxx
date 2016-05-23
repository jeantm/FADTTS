#include "Log.h"

//#include <QDebug>

Log::Log(QObject *parent) :
    QObject(parent)
{
}


/****************************************************************/
/*********************    Private slots   ***********************/
/****************************************************************/
void Log::OnLogFileChange()
{
    emit UpdateLogActivity( m_textStreamLog->readAll() );
}



/****************************************************************/
/********************** Public  functions ***********************/
/****************************************************************/
void Log::SetMatlabScript( MatlabThread *matlabThread )
{
    m_matlabThread = matlabThread;
}


void Log::SetLogFile( QString outputDir, QString fibername )
{
    m_logFile = new::QFile( outputDir + "/" + fibername + ".log" );
    m_logFile->open( QIODevice::ReadWrite );
    m_matlabThread->SetLogFile( m_logFile );
    m_textStreamLog = new QTextStream( m_logFile );
}

void Log::SetFileWatcher()
{
    m_logWatcher = new QFileSystemWatcher( this );
    m_logWatcher->addPath( m_logFile->fileName() );
    connect( m_logWatcher, SIGNAL( fileChanged( QString ) ), this, SLOT( OnLogFileChange() ) );
}

void Log::InitLog( QString outputDir, QString fibername, const QMap< int, QString >& matlabInputFiles, const QMap< int, QString >& selectedCovariates,
                         QStringList loadedSubjects, QString subjectFile, int nbrSelectedSubjects, QStringList failedQCThresholdSubjects, double qcThreshold,
                         int nbrPermutations, double confidenceBandsThreshold, double pvalueThreshold, bool omnibus, bool posthoc, QString mvcmDir,
                         bool runMatlab, QString matlabExe )
{
    *m_textStreamLog << QDate::currentDate().toString( "MM/dd/yyyy" ) <<
                        " " << QTime::currentTime().toString( "hh:mmap" ) << endl;
    *m_textStreamLog << "FADTTSter: " << QString( FADTTS_VERSION ).prepend( "V" ) << endl;
    *m_textStreamLog << "/****************************************************************/" << endl;
    *m_textStreamLog << "/********************** FADTTSter LOG FILE **********************/" << endl;
    *m_textStreamLog << "/****************************************************************/" << endl << endl;

    *m_textStreamLog << "/**********************       Inputs       **********************/" << endl;
    *m_textStreamLog << "- input files: ";
    QStringList files;
    QMap< int, QString >::ConstIterator iterMatlabInputFiles = matlabInputFiles.cbegin();
    while( iterMatlabInputFiles != matlabInputFiles.cend() )
    {
        files.append( QFileInfo( QFile( iterMatlabInputFiles.value() ) ).fileName() );
        ++iterMatlabInputFiles;
    }
    *m_textStreamLog << files.join( ", " ) << endl;
    *m_textStreamLog << "- nbr covariates: " << QString::number( selectedCovariates.size() ) << endl;
    *m_textStreamLog << "- covariates: ";
    QStringList covariates;
    QMap< int, QString >::ConstIterator iterCovariates = selectedCovariates.cbegin();
    while( iterCovariates != selectedCovariates.cend() )
    {
        covariates.append( iterCovariates.value() );
        ++iterCovariates;
    }
    *m_textStreamLog << covariates.join( ", " ) << endl;
    if( !loadedSubjects.isEmpty() )
    {
        *m_textStreamLog << "- subject file: " << subjectFile << endl;
    }
    *m_textStreamLog << "- nbr subjects: " << QString::number( nbrSelectedSubjects ) << endl;
    if( !failedQCThresholdSubjects.isEmpty() )
    {
        *m_textStreamLog << "/!\\ That number INCLUDE the " << QString::number( failedQCThresholdSubjects.size() ) << " subject(s) that was/were removed from the study after failing the QC Threshold set at " << QString::number( qcThreshold ) << endl;
    }

    *m_textStreamLog << endl << "/**********************      Settings      **********************/" << endl;
    *m_textStreamLog << "- fiber name: " << fibername << endl;
    *m_textStreamLog << "- nbr permutations: " << QString::number( nbrPermutations ) << endl;
    *m_textStreamLog << "- confidence band threshold: " << QString::number( confidenceBandsThreshold ) << endl;
    *m_textStreamLog << "- pvalue threshold: " << QString::number( pvalueThreshold ) << endl;
    *m_textStreamLog << QString( omnibus ? "- omnibus: true" : "- omnibus: false" ) << endl;
    *m_textStreamLog << QString( posthoc ? "- post hoc: true" : "- post hoc: false" ) << endl << endl;

    *m_textStreamLog << "/**********************       Output       **********************/" << endl;
    *m_textStreamLog << "- output directory: " << outputDir << endl << endl;

    *m_textStreamLog << "/********************* Matlab Specification *********************/" << endl;
    *m_textStreamLog << "- FADTTS directory: " << mvcmDir << endl << endl;
    if( runMatlab )
    {
        *m_textStreamLog << "Matlab will be run after file generation" << endl;
        *m_textStreamLog << "- matlab executable: " << matlabExe << endl;

        *m_textStreamLog << endl << endl << endl;
        *m_textStreamLog << "/**********************     Run Matlab     **********************/" << endl << endl;
    }
    else
    {
        *m_textStreamLog << "/!\\ Matlab will not be run after file generation (user choice)" << endl;
    }

    m_logFile->flush();
}


void Log::AddText( QString text )
{
    *m_textStreamLog << text;
}

QString Log::GetTextStream() const
{
    return m_textStreamLog->readAll();
}

void Log::CloseLogFile()
{
    disconnect( m_logWatcher, SIGNAL( fileChanged( QString ) ), this, SLOT( OnLogFileChange() ) );

    m_logFile->flush();
    m_logFile->close();
}
