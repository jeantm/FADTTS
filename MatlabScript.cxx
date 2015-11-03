#include "MatlabScript.h"

const QString MatlabScript::m_csvSeparator = QLocale().groupSeparator();
//const QFile MatlabScript::m_matlabReferenceScript = QFile( ":/FADTTS_MatlabReferenceScript/MatlabRefScript.m" );

MatlabScript::MatlabScript()
{
}

/***************************************************************/
/********************** Public functions ***********************/
/***************************************************************/
void MatlabScript::InitMatlabScript()
{
    m_matlabScript.clear();
    QResource resource( ":/MatlabFiles/Resources/MatlabFiles/MatlabScriptRef.m" );
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

void MatlabScript::SetHeader()
{
    m_matlabScript.replace( "$version$", QString( FADTTS_VERSION ).prepend( "V" ) );
    m_matlabScript.replace( "$date$", QDate::currentDate().toString( "MM/dd/yyyy" ) );
    m_matlabScript.replace( "$time$", QTime::currentTime().toString( "hh:mm ap" ) );
}

void MatlabScript::SetFiberName( QString fiberName )
{
    m_matlabScript.replace( "$inputFiberName$", "fiberName = \'" + fiberName + "\';\n" );
}

void MatlabScript::SetDiffusionProperties( QStringList selectedPrefixes )
{
    QString inputDiffusionProperties;
    QString inputAllProperties;
    QString diffusionProperties;
    diffusionProperties.append( "Dnames = cell( " + QString::number( selectedPrefixes.size() ) + ", 1 );\n" );

    int i = 1;
    foreach ( QString prefID, selectedPrefixes )
    {
        inputDiffusionProperties.append( prefID.toUpper() + " = \'" + prefID.toUpper() + "\';\n" );
        inputAllProperties.append( prefID.toUpper() );

        diffusionProperties.append( "Dnames{ " + QString::number( i ) + " } = " + prefID.toUpper() + ";\n" );
        i++;
    }

    m_matlabScript.replace( "$inputDiffusionProperties$", inputDiffusionProperties );
    m_matlabScript.replace( "$inputAllProperties$", "allProperties = \'" + inputAllProperties + "\';" );

    m_matlabScript.replace( "$diffusionProperties$", diffusionProperties );
    m_matlabScript.replace( "$allProperties$", "params{ 1 } = allProperties;" );
}

void MatlabScript::SetNbrPermutation( int nbrPermutation )
{
    m_matlabScript.replace( "$inputNbrPermutations$", "nbrPermutations = " + QString::number( nbrPermutation ) + ";" );
}

void MatlabScript::SetCovariates( QMap<int, QString> selectedCovariates )
{
    m_matlabScript.replace( "$inputNbrCovariates$", "nbrCovariates = " + QString::number( selectedCovariates.count() ) + ";" );
    QString inputCovarariates;
    QString covariates;
    int i = 1;
    QMap<int, QString>::ConstIterator iterCovariate = selectedCovariates.begin();
    while( iterCovariate != selectedCovariates.constEnd() )
    {
        inputCovarariates.append( iterCovariate.value() + " = \'" + iterCovariate.value() + "\';\n" );
        covariates.append( "Pnames{ " + QString::number( i ) + " } = " + iterCovariate.value() + ";\n" );
        ++iterCovariate;
        i++;
    }
    m_matlabScript.replace( "$inputCovariates$", inputCovarariates );
    m_matlabScript.replace( "$covariates$", covariates );
}

void MatlabScript::SetInputFiles( QMap<QString, bool> matlabInputFiles )
{
    QString inputDiffusionFiles;
    QString diffusionFiles;
    diffusionFiles.append("diffusionFiles = cell( " + QString::number( matlabInputFiles.size() - 1 ) + ", 1 );\n");

    int i = 1;
    QMap<QString, bool>::ConstIterator iterMatlabInputFile = matlabInputFiles.begin();
    while( iterMatlabInputFile != matlabInputFiles.constEnd() )
    {
        QString filename = QFileInfo( QFile( iterMatlabInputFile.key().split( "?" ).last() ) ).fileName();
        if( iterMatlabInputFile.value() == false )
        {
            inputDiffusionFiles.append( filename.split( "." ).first() + " = strcat( folder, \'/" + filename + "\' );\n" );
            diffusionFiles.append( "dataFiber" + QString::number( i ) + "All = dlmread( " + filename.split( "." ).first() +
                                   ", \'" + m_csvSeparator + "\', 1, 0 );\n" );
            diffusionFiles.append( "diffusionFiles{ " + QString::number( i ) + " } = dataFiber" + QString::number( i ) + "All( :, 2:end );\n" );
            i++;
        }
        else
        {
            m_matlabScript.replace( "$inputMatlabCOMPInputFile$", filename.split( "." ).first() + " = strcat( folder, \'/" + filename + "\' );" );
            m_matlabScript.replace( "$matlabCOMPInputFile$", "data2 = dlmread( " + filename.split( "." ).first() + ", \'" + m_csvSeparator + "\', 1, 1);" );
        }
        ++iterMatlabInputFile;
    }
    m_matlabScript.replace( "$inputDiffusionFiles$", inputDiffusionFiles );
    m_matlabScript.replace( "$diffusionFiles$", diffusionFiles );
}

void MatlabScript::SetOmnibus( bool omnibus )
{
    m_matlabScript.replace( "$inputOmnibus$", "omnibus = " + QString::number( omnibus ) + ";" );
}

void MatlabScript::SetPostHoc( bool postHoc )
{
    m_matlabScript.replace( "$inputPostHoc$", "postHoc = " + QString::number( postHoc ) + ";" );
}

QString MatlabScript::GenerateMatlabFiles( QString outputDir, QString fiberName, int nbrPermutations )
{
    QString matlabScriptPath = outputDir + "/FADTTSAnalysis_MatlabScript_" + fiberName + "_" + QString::number( nbrPermutations ) + "perm.m";
    QFile matlabScript( matlabScriptPath );
    if( matlabScript.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
        QTextStream ts( &matlabScript );
        ts << m_matlabScript;
        matlabScript.flush();
        matlabScript.close();
    }

    QResource resource( ":/MatlabFiles/Resources/MatlabFiles/myFDR.m" );
    QFile myFDR( resource.absoluteFilePath() );
    QFile myFDROutput( outputDir + "/myFDR.m" );
    if( myFDROutput.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
        if( !myFDR.open( QIODevice::ReadOnly | QIODevice::Text ) )
        {
            QString criticalError = "Unable to open resource file: " + myFDR.fileName() +
                    " because of error \"" + myFDR.errorString() + "\"";
            std::cerr << criticalError.toStdString() << std::endl;
        }
        else
        {
            QTextStream tsMyFDR( &myFDR );
            QTextStream tsMyFDROutput( &myFDROutput );
            tsMyFDROutput << tsMyFDR.readAll();
            myFDROutput.flush();
            myFDROutput.close();
            myFDR.close();
        }
    }

    return matlabScriptPath;
}
