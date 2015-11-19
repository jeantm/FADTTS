#include "MatlabScript.h"

const QString MatlabScript::m_csvSeparator = QLocale().groupSeparator();

MatlabScript::MatlabScript()
{
}

/***************************************************************/
/********************** Public functions ***********************/
/***************************************************************/
void MatlabScript::SetMatlabOutputDir( QString matlabOutputDir )
{
    m_matlabOutputDir = matlabOutputDir;
}

void MatlabScript::SetMatlabScriptName( QString matlabScriptName )
{
    m_matlabScriptName = matlabScriptName;
}


void MatlabScript::InitMatlabScript()
{
    m_matlabScript.clear();
//    QResource resource( ":/MatlabFiles/Resources/MatlabFiles/MatlabScriptRef.m" );
    QResource resource( ":/MatlabFiles/Resources/MatlabFiles/MatlabScriptRefWithPlots.m" );
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

void MatlabScript::SetMVCMPath( QString mvcmPath )
{
    m_matlabScript.replace( "$addMVCMPath$", "addpath \'" + mvcmPath + "\';\n" );
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

void MatlabScript::SetInputFiles( QMap< QPair< int, QString >, bool > matlabInputFiles )
{
    QString inputDiffusionFiles;
    QString diffusionFiles;
    diffusionFiles.append("diffusionFiles = cell( " + QString::number( matlabInputFiles.size() - 1 ) + ", 1 );\n");

    int i = 1;
    QMap< QPair< int, QString >, bool >::ConstIterator iterMatlabInputFile = matlabInputFiles.begin();
    while( iterMatlabInputFile != matlabInputFiles.constEnd() )
    {
        QString filename = QFileInfo( QFile( iterMatlabInputFile.key().second ) ).fileName();
        if( iterMatlabInputFile.value() == false )
        {
            inputDiffusionFiles.append( filename.split( "." ).first() + " = strcat( loadingFolder, \'/" + filename + "\' );\n" );
            diffusionFiles.append( "dataFiber" + QString::number( i ) + "All = dlmread( " + filename.split( "." ).first() +
                                   ", \'" + m_csvSeparator + "\', 1, 0 );\n" );
            diffusionFiles.append( "diffusionFiles{ " + QString::number( i ) + " } = dataFiber" + QString::number( i ) + "All( :, 2:end );\n" );
            i++;
        }
        else
        {
            m_matlabScript.replace( "$inputMatlabCOMPInputFile$", filename.split( "." ).first() + " = strcat( loadingFolder, \'/" + filename + "\' );" );
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


void MatlabScript::GenerateMyFDR()
{
    QResource resource( ":/MatlabFiles/Resources/MatlabFiles/myFDR.m" );
    QFile matlabFunctionResource( resource.absoluteFilePath() );
    QFile matlabFunction( m_matlabOutputDir + "/myFRD.m" );

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

QString MatlabScript::GenerateMatlabFiles()
{
    QDir().mkpath( m_matlabOutputDir );
    QString matlabScriptPath = m_matlabOutputDir + m_matlabScriptName;
    QFile matlabScript( matlabScriptPath );
    if( matlabScript.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
        QTextStream ts( &matlabScript );
        ts << m_matlabScript;
        matlabScript.flush();
        matlabScript.close();
    }

    GenerateMyFDR();

    return matlabScriptPath;
}
