#include "TestMatlabScript.h"


TestMatlabScript::TestMatlabScript()
{
}


/**********************************************************************/
/*************************** Test Functions ***************************/
/**********************************************************************/
bool TestMatlabScript::Test_GenerateMatlabScript( QString outputDir, QString matlabInputADFile, QString matlabInputCOMPFile, QString expectedMatlabScript )
{
    MatlabScript mScript;

    QString fiberName = "TestFADTTS";
    bool omnibus = true;
    bool postHoc = true;
    int nbrPermutations = 259;
    QMap<int,QString> selectedCovariates;
    selectedCovariates.insert( -1, "Intercept" );
    selectedCovariates.insert( 1, "COMP" );
    selectedCovariates.insert( 2, "Gender" );
    selectedCovariates.insert( 3, "GestAgeBirth" );
    QStringList selectedPrefixes;
    selectedPrefixes.append( "ad" );
    QMap<QString, bool> matlabInputFiles;
    matlabInputFiles.insert( "00?" + matlabInputADFile, false );
    matlabInputFiles.insert( "04?" + matlabInputCOMPFile, true );

    QString matlabScript = mScript.GenerateMatlabScriptForTest( outputDir, fiberName, selectedPrefixes, matlabInputFiles,
                                                         selectedCovariates, nbrPermutations, omnibus, postHoc );

    bool scriptMatched = CompareFile( matlabScript, expectedMatlabScript );


    if( !scriptMatched )
    {
        std::cerr << std::endl << "Test_GenerateMatlabInputFiles() FAILED:" << std::endl;
        std::cerr << "\t+ Matlab Script not generated correctly" << std::endl;
    }
    else
    {
        std::cout << std::endl << "Test_GenerateMatlabInputFiles() PASSED" << std::endl;
    }

    return scriptMatched;
}



/**********************************************************************/
/********************** Functions Used For Testing ********************/
/**********************************************************************/
QByteArray TestMatlabScript::GetHashFile( QString filename )
{
    QCryptographicHash hash( QCryptographicHash::Sha1 );
    QFile file( filename );

    if( file.open( QIODevice::ReadOnly ) )
    {
        hash.addData( file.readAll() );
        file.close();
    }
    else
    {
        std::cerr << "Cannot open file: " << filename.toStdString() << std::endl;
    }

    return hash.result().toHex();
}

bool TestMatlabScript::CompareFile( QString fileName1, QString fileName2 )
{
    QByteArray sig1 = GetHashFile( fileName1 );
    QByteArray sig2 = GetHashFile( fileName2 );

    if( sig1 == sig2 )
    {
        return true;
    }
    else
    {
        return false;
    }
}
