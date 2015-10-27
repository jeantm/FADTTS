#include "MatlabScript.h"

const QString MatlabScript::m_csvSeparator = QLocale().groupSeparator();
//const QFile MatlabScript::m_matlabReferenceScript = QFile( ":/FADTTS_MatlabReferenceScript/MatlabRefScript.m" );

MatlabScript::MatlabScript()
{
}

/***************************************************************/
/********************** Public functions ***********************/
/***************************************************************/
QString MatlabScript::GenerateMatlabScript( bool release, QString outputDir, QString fiberName, QStringList selectedPrefixes,
                                            QMap<QString, bool> matlabInputFiles, QMap<int,QString> selectedCovariates,
                                            int nbrPermutations, bool omnibus, bool postHoc )
{
    InitMatlabScript();

    if( release )
    {
        SetHeader();
    }

    SetFiberName( fiberName );

    SetDiffusionProperties( selectedPrefixes );

    SetNbrPermutations( nbrPermutations );

    SetCovariatesList( selectedCovariates );

    SetInputFiles( matlabInputFiles );

    SetOmnibus( omnibus );

    SetPostHoc( postHoc );

    m_scriptPath = outputDir + "/" + fiberName + "_FADTTSAnalysis_MatlabScript.m";
    QFile matlabScript( m_scriptPath );

    if( matlabScript.open( QIODevice::WriteOnly ) )
    {
        QTextStream ts( &matlabScript );
        ts << m_script;
        matlabScript.flush();
        matlabScript.close();
    }

    return m_scriptPath;
}

void MatlabScript::ResetScript()
{
    m_script.clear();
    m_scriptPath.clear();
}



/***************************************************************/
/********************** Private functions **********************/
/***************************************************************/
void MatlabScript::InitMatlabScript()
{
    QResource resource( ":/MatlabRefScript.m" );

    qDebug() << resource.absoluteFilePath() << endl;
    QFile matlabScriptRef( resource.absoluteFilePath() );

    if ( !matlabScriptRef.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug() << "Unable to open file: " << matlabScriptRef.fileName() << " because of error " << matlabScriptRef.errorString() << endl;
        return; /** Display error **/
    }
    else
    {
        QTextStream ts( &matlabScriptRef );
        m_script = ts.readAll();
        matlabScriptRef.close();
    }
}

void MatlabScript::SetHeader()
{
    m_script.replace( "$version$", QString( FADTTS_VERSION ).prepend( "V" ) );
    m_script.replace( "$date$", QDate::currentDate().toString( "MM/dd/yyyy" ) );
    m_script.replace( "$time$", QTime::currentTime().toString( "hh:mm ap" ) );
}

void MatlabScript::SetFiberName( QString fiberName )
{
    m_script.replace( "$inputFiberName$", "fiberName = '" + fiberName + "';\n" );
}

void MatlabScript::SetDiffusionProperties( QStringList selectedPrefixes )
{
    if( !selectedPrefixes.isEmpty() )
    {
        QString inputDiffusionProperties;
        QString inputAllProperties;
        QString diffusionProperties;

        int i = 1;
        foreach (QString prefix, selectedPrefixes)
        {
            inputDiffusionProperties.append( prefix.toUpper() + " = '" + prefix.toUpper() + "';\n" );
            inputAllProperties.append( prefix.toUpper() );

            diffusionProperties.append( "Dnames{ " + QString::number( i ) + " } = " + prefix.toUpper() + ";\n" );

            i++;
        }

        m_script.replace( "$inputDiffusionProperties$", inputDiffusionProperties );
        m_script.replace( "$inputAllProperties$", "allProperties = '" + inputAllProperties + "';" );

        m_script.replace( "$diffusionProperties$", diffusionProperties );
        m_script.replace( "$allProperties$", "params{ 1 } = allProperties;" );
    }
    else
    {
        m_script.replace( "$inputDiffusionProperties$", "" );
        m_script.replace( "$inputAllProperties$", "" );

        m_script.replace( "$diffusionProperties$", "" );
        m_script.replace( "$allProperties$", "" );
    }

}

void MatlabScript::SetNbrPermutations( int nbrPermutations )
{
    m_script.replace( "$inputNbrPermutations$", "nbrPermutations = " + QString::number( nbrPermutations ) + ";" );
}

void MatlabScript::SetCovariatesList( QMap<int, QString> selectedCovariates )
{
    m_script.replace( "$inputNbrCovariates$", "nbrCovariates = " + QString::number( selectedCovariates.count() ) + ";" );
    QString strInputCovar;
    QString strCovar;
    int i = 1;
    QMap<int, QString>::ConstIterator iterCovariate = selectedCovariates.begin();
    while( iterCovariate != selectedCovariates.constEnd() )
    {
        strInputCovar.append( iterCovariate.value() + " = '" + iterCovariate.value() + "';\n" );
        strCovar.append( "Pnames{ " + QString::number( i ) + " } = " + iterCovariate.value() + ";\n" );
        ++iterCovariate;
        i++;
    }
    m_script.replace( "$inputCovariates$", strInputCovar );
    m_script.replace( "$covariates$", strCovar );
}

void MatlabScript::SetInputFiles( QMap<QString, bool> matlabInputFiles )
{
    QString inputDiffusionFiles;
    QString diffusionFiles;

    int i = 1;
    QMap<QString, bool>::ConstIterator iterMatlabInputFile = matlabInputFiles.begin();
    while( iterMatlabInputFile != matlabInputFiles.constEnd() )
    {
        QString filename = QFileInfo( QFile( iterMatlabInputFile.key().split( "?" ).last() ) ).fileName();
        if( iterMatlabInputFile.value() == false )
        {
            inputDiffusionFiles.append( filename.split( "." ).first() + " = strcat( folder, '/" + filename + "' );\n" );
            diffusionFiles.append( "dataFiber" + QString::number( i ) + "All = dlmread( " + filename.split( "." ).first() +
                                   ", '" + m_csvSeparator + "', 1, 0 );\n" );
            diffusionFiles.append( "diffusionFiles{ " + QString::number( i ) + " } = dataFiber" + QString::number( i ) + "All( :, 2:end );\n" );
            i++;
        }
        else
        {
            m_script.replace( "$inputMatlabCOMPInputFile$", filename.split( "." ).first() + " = strcat( folder, '/" + filename + "' );" );
            m_script.replace( "$matlabCOMPInputFile$", "data2 = dlmread( " + filename.split( "." ).first() + ", '" + m_csvSeparator + "', 1, 1);" );
        }
        ++iterMatlabInputFile;
    }
    if( !inputDiffusionFiles.isEmpty() )
    {
        m_script.replace( "$inputDiffusionFiles$", inputDiffusionFiles );
        m_script.replace( "$diffusionFiles$", diffusionFiles );
    }
    else
    {
        m_script.replace( "$inputDiffusionFiles$", "" );
        m_script.replace( "$diffusionFiles$", "" );
    }
}

void MatlabScript::SetOmnibus( bool omnibus )
{
    m_script.replace( "$inputOmnibus$", "omnibus = " + QString::number( omnibus ) + ";" );
}

void MatlabScript::SetPostHoc( bool postHoc )
{
    m_script.replace( "$inputPostHoc$", "postHoc = " + QString::number( postHoc ) + ";" );
}
