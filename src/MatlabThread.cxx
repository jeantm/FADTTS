#include "MatlabThread.h"

//#include <QDebug>

const QString MatlabThread::m_csvSeparator = QLocale().groupSeparator();

MatlabThread::MatlabThread(QObject *parent) :
    QThread(parent)
{
}


/***************************************************************/
/*************************** Script ****************************/
/***************************************************************/

/************ Public Functions ************/
void MatlabThread::InitMatlabScript( QString outputDir, QString matlabScriptName )
{
    m_outputDir = outputDir;
    m_matlabScriptName = matlabScriptName;
    m_matlabScript.clear();
    QResource resource( ":/MatlabFiles/Resources/MatlabFiles/matlabScriptWithPlotting.m" );
    QFile matlabScriptRef( resource.absoluteFilePath() );
    if ( !matlabScriptRef.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        QString criticalError = "Unable to open resource file: " + matlabScriptRef.fileName() +
                " because of error \"" + matlabScriptRef.errorString() + "\"";
        std::cerr << criticalError.toStdString() << std::endl;
    }
    else
    {
        QTextStream ts( &matlabScriptRef );
        m_matlabScript = ts.readAll();
        matlabScriptRef.close();
    }
}


void MatlabThread::SetHeader()
{
    m_matlabScript.replace( "$version$", QString( FADTTS_VERSION ).prepend( "V" ) );
    m_matlabScript.replace( "$date$", QDate::currentDate().toString( "MM/dd/yyyy" ) );
    m_matlabScript.replace( "$time$", QTime::currentTime().toString( "hh:mm ap" ) );
}


void MatlabThread::SetMVCMPath( QString mvcmPath )
{
    m_matlabScript.replace( "$addMVCMPath$", mvcmPath );
}


void MatlabThread::SetFiberName( QString fiberName )
{
    m_matlabScript.replace( "$fiberName$", "fiberName = \'" + fiberName + "\';\n" );
}

void MatlabThread::SetDiffusionProperties( QStringList selectedPrefixes )
{
    QString diffusionProperties;
    QString listDiffusionProperties;
    listDiffusionProperties.append( "Dnames = cell( " + QString::number( selectedPrefixes.size() ) + ", 1 );\n" );

    int i = 1;
    foreach ( QString prefID, selectedPrefixes )
    {
        if( !prefID.contains( "submatrix", Qt::CaseInsensitive ) )
        {
            diffusionProperties.append( prefID.toUpper() + " = \'" + prefID.toUpper() + "\';\n" );

            listDiffusionProperties.append( "Dnames{ " + QString::number( i ) + " } = " + prefID.toUpper() + ";\n" );
            i++;
        }
    }

    m_matlabScript.replace( "$diffusionProperties$", diffusionProperties );

    m_matlabScript.replace( "$listDiffusionProperties$", listDiffusionProperties );
}

void MatlabThread::SetInputFiles( const QMap< int, QString >& csvInputFiles )
{
    QString diffusionFiles;
    QString diffusionData;
    diffusionData.append("diffusionFiles = cell( " + QString::number( csvInputFiles.size() - 1 ) + ", 1 );\n");

    int i = 1;
    QMap< int, QString >::ConstIterator iterMatlabInputFile = csvInputFiles.cbegin();
    while( iterMatlabInputFile != csvInputFiles.cend() )
    {
        QString filename = QFileInfo( QFile( iterMatlabInputFile.value() ) ).fileName();
        if( !( filename.contains( "_subMatrix_", Qt::CaseInsensitive ) || filename.contains( "_subMatrix.csv", Qt::CaseInsensitive ) ) )
        {
            diffusionFiles.append( filename.split( "." ).first() + " = strcat( loadingFolder, \'/" + filename + "\' );\n" );
            diffusionData.append( "dataFiber" + QString::number( i ) + " = dlmread( " + filename.split( "." ).first() +
                                   ", \'" + m_csvSeparator + "\', 1, 0 );\n" );
            diffusionData.append( "diffusionFiles{ " + QString::number( i ) + " } = dataFiber" + QString::number( i ) + "( :, 2:end );\n" );
            i++;
        }
        else
        {
            m_matlabScript.replace( "$subMatrixFile$", filename.split( "." ).first() + " = strcat( loadingFolder, \'/" + filename + "\' );" );
            m_matlabScript.replace( "$subMatrixData$", "dataSubmatrix = dlmread( " + filename.split( "." ).first() + ", \'" + m_csvSeparator + "\', 1, 1);" );
        }
        ++iterMatlabInputFile;
    }
    m_matlabScript.replace( "$diffusionFiles$", diffusionFiles );
    m_matlabScript.replace( "$diffusionData$", diffusionData );
}

void MatlabThread::SetCovariates( const QMap< int, QString > &selectedCovariates )
{
    m_matlabScript.replace( "$nbrCovariates$", "nbrCovariates = " + QString::number( selectedCovariates.count() ) + ";" );
    QString covariates;
    QString listCovariates;
    int i = 1;
    QMap< int, QString >::ConstIterator iterCovariate = selectedCovariates.cbegin();
    while( iterCovariate != selectedCovariates.cend() )
    {
        covariates.append( iterCovariate.value() + " = \'" + iterCovariate.value() + "\';\n" );
        listCovariates.append( "Cnames{ " + QString::number( i ) + " } = " + iterCovariate.value() + ";\n" );
        ++iterCovariate;
        i++;
    }
    m_matlabScript.replace( "$covariates$", covariates );
    m_matlabScript.replace( "$listCovariates$", listCovariates );
}


void MatlabThread::SetNbrPermutation( int nbrPermutation )
{
    m_matlabScript.replace( "$nbrPermutations$", "nbrPermutations = " + QString::number( nbrPermutation ) + ";" );
}

void MatlabThread::SetOmnibus( bool omnibus )
{
    m_matlabScript.replace( "$omnibus$", "omnibus = " + QString::number( omnibus ) + ";" );
}

void MatlabThread::SetPostHoc( bool postHoc )
{
    m_matlabScript.replace( "$postHoc$", "postHoc = " + QString::number( postHoc ) + ";" );
}

void MatlabThread::SetConfidenceBandsThreshold( double confidenceBandsThreshold )
{
    m_matlabScript.replace( "$confidenceBandsThreshold$", "confidenceBandsThreshold = " + QString::number( confidenceBandsThreshold ) + ";" );
}

void MatlabThread::SetPvalueThreshold( double pvalueThreshold )
{
    m_matlabScript.replace( "$pvalueThreshold$", "pvalueThreshold = " + QString::number( pvalueThreshold ) + ";" );
}


/*********** Private  Functions ***********/
void MatlabThread::GenerateMyFDR()
{
    QResource resource( ":/MatlabFiles/Resources/MatlabFiles/myFDR.m" );
    QFile matlabFunctionResource( resource.absoluteFilePath() );
    QFile matlabFunction( m_outputDir + "/myFDR.m" );

    if ( !matlabFunctionResource.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        QString criticalError = "Unable to open resource file: " + matlabFunctionResource.fileName() +
                " because of error \"" + matlabFunctionResource.errorString() + "\"";
        std::cerr << criticalError.toStdString() << std::endl;
    }
    else
    {
        if( matlabFunction.open( QIODevice::WriteOnly | QIODevice::Text ) )
        {
            QTextStream tsMatlabFunctionResource( &matlabFunctionResource );
            QTextStream tsMatlabFunction( &matlabFunction );
            tsMatlabFunction << tsMatlabFunctionResource.readAll();
            matlabFunction.flush();
            matlabFunction.close();
            matlabFunctionResource.close();
        }
    }
}

QString MatlabThread::GenerateMatlabFiles()
{
    m_matlabScriptPath = m_outputDir + "/" + m_matlabScriptName;

    QFile matlabScript( m_matlabScriptPath );
    if( matlabScript.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
        QTextStream ts( &matlabScript );
        ts << m_matlabScript;
        matlabScript.flush();
        matlabScript.close();
    }

    GenerateMyFDR();

    return m_matlabScriptPath;
}


/***************************************************************/
/*************************** Thread ****************************/
/***************************************************************/

/************ Public Functions ************/
void MatlabThread::SetLogFile( QFile *logFile )
{
    m_logFile = logFile;
}


QString& MatlabThread::SetMatlabExe()
{
    return m_matlabExe;
}

bool& MatlabThread::SetRunMatlab()
{
    return m_runMatlab;
}


void MatlabThread::terminate()
{
    m_process->kill();
}


/*********** Private  Functions ***********/
void MatlabThread::RedirectOutput()
{
    m_process = new QProcess();
    m_process->setProcessChannelMode( QProcess::MergedChannels );
    m_process->setStandardOutputFile( m_logFile->fileName(), QProcess::Append );
}

bool MatlabThread::TestVersion()
{
    QProcess *processTest;
    processTest = new QProcess();
    QStringList arguments;
    QString getVersion = "['Release=R' version('-release')], quit";
    QString version;
    QString extension;
    int v;

    arguments << "-nosplash" << "-nodesktop" << QString( "-r \"try, " + getVersion + "; catch, disp('failed'), end, quit\"" );

    processTest->start( m_matlabExe, arguments );
    processTest->waitForFinished( -1 );

    version = processTest->readAllStandardOutput();
    version.chop( 2 );
    extension = version.at( version.length() - 1 ).toLower();
    version.indexOf( "Release=", 0 );
    v = version.mid( version.lastIndexOf( "R" ) + 1, 4 ).toInt();

    if( v > 2013 || ( v == 2013 && extension == "b" ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void MatlabThread::RunScript()
{
    bool isVersionOK = TestVersion();

    if( isVersionOK )
    {
        QStringList arguments;
        QString mScript = "run('" + m_matlabScriptPath + "')";

        arguments << "-nosplash" << "-nodesktop" << "-noFigureWindows" << QString( "-r \"try, " + mScript + "; catch, disp('failed'), end, quit\"" ) << "-logfile matlabLog.out";

        m_process->start( m_matlabExe, arguments );
        m_process->waitForFinished( -1 );
    }
    else
    {
        emit WrongMatlabVersion();
    }
}


void MatlabThread::run()
{
    m_matlabScriptPath.clear();
    GenerateMatlabFiles();

    if( m_runMatlab )
    {
        RedirectOutput();
        RunScript();
    }
}
