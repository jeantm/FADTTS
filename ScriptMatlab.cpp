#include "ScriptMatlab.h"

const QString ScriptMatlab::m_csvSeparator = QLocale().groupSeparator();

ScriptMatlab::ScriptMatlab()
{
}

void ScriptMatlab::GenerateMatlabScript( QString fiberName, QStringList selectedPrefixes, QMap<QString, bool> matlabInputFiles,
                                         QMap<int,QString> selectedCovariates, int nbrPermutation, QString outputDir )
{
    InitScriptMatlab();

    SetFiberName( fiberName );

    SetDiffusionProperties( selectedPrefixes );

    SetNbrPermutation( nbrPermutation );

    SetCovariatesList( selectedCovariates );

    SetInputFiles( matlabInputFiles );

    QFile matlabScript( outputDir + "/matlabScriptTest.m" );
    if( matlabScript.open( QIODevice::WriteOnly ) )
    {
        QTextStream ts( &matlabScript );
        ts << m_script;
        matlabScript.flush();
        matlabScript.close();
    }
}


void ScriptMatlab::InitScriptMatlab()
{
    QFile matlabScriptRef( "scriptRefTest.m" );
    matlabScriptRef.open( QIODevice::ReadOnly );
    QTextStream ts( &matlabScriptRef );
    m_script = ts.readAll();
    matlabScriptRef.close();
}

void ScriptMatlab::SetFiberName( QString fiberName )
{
    m_script.replace( "$fiberName$", fiberName );
}

void ScriptMatlab::SetDiffusionProperties( QStringList selectedPrefixes )
{
    QString diffusionProperties;
    QString allProperties;
    int i = 1;
    foreach (QString prefix, selectedPrefixes)
    {
        diffusionProperties.append( "Dnames{" + QString::number( i ) + "}='" + prefix + "';\n" );
        allProperties.append( prefix.toUpper() );
        i++;
    }
    m_script.replace( "$diffusionProperties$", diffusionProperties );
    m_script.replace( "$allProperties$", allProperties );
}

void ScriptMatlab::SetNbrPermutation( int nbrPermutation )
{
    m_script.replace( "$nbrPermutation$", QString::number( nbrPermutation ) );
}

void ScriptMatlab::SetCovariatesList( QMap<int, QString> selectedCovariates )
{
    m_script.replace( "$nbrCovariates$", QString::number( selectedCovariates.count() ) );
    QString str;
    int i = 1;
    QMap<int, QString>::ConstIterator iterCovariate = selectedCovariates.begin();
    while( iterCovariate != selectedCovariates.constEnd() )
    {
        str.append( "Pnames{" + QString::number( i ) + "}='" + iterCovariate.value() + "';\n" );
        ++iterCovariate;
        i++;
    }
    m_script.replace( "$covariates$", str );
}

void ScriptMatlab::SetInputFiles( QMap<QString, bool> matlabInputFiles )
{
    QString diffusionFiles;
    int i = 1;
    QMap<QString, bool>::ConstIterator iterMatlabInputFile = matlabInputFiles.begin();
    while( iterMatlabInputFile != matlabInputFiles.constEnd() )
    {
        if( iterMatlabInputFile.value() == false )
        {
            diffusionFiles.append( "\tdataFiber" + QString::number( i ) + "All=dlmread('" + QFileInfo( QFile( iterMatlabInputFile.key() ) ).fileName() +"','" + m_csvSeparator + "',1,0);\n" );
            diffusionFiles.append( "\tdiffusionFiles{" + QString::number( i ) + "}=dataFiber" + QString::number( i ) + "All(:,2:end);\n" );
            i++;
        }
        else
        {
            m_script.replace( "$matlabCOMPInputFile$", "data2=dlmread('" + QFileInfo( QFile( iterMatlabInputFile.key() ) ).fileName() + "','" + m_csvSeparator + "',0,1);\n" );
        }
        ++iterMatlabInputFile;
    }
    m_script.replace( "$diffusionFiles$", diffusionFiles );
}
