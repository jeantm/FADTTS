#include "TestMatlabScript.h"

TestMatlabScript::TestMatlabScript()
{
}

/**********************************************************************/
/*************************** Test Functions ***************************/
/**********************************************************************/
bool TestMatlabScript::Test_GenerateMatlabScript( QString outputDir, QString matlabInputADFile, QString matlabInputCOMPFile, QString expectedMatlabScript )
{
//    MatlabScript mScript;
//    mScript.InitMatlabScript();

//    QString fiberName = "TestFADTTS";
//    mScript.SetFiberName( fiberName );

//    QStringList selectedPrefixes;
//    selectedPrefixes.append( "ad" );
//    mScript.SetDiffusionProperties( selectedPrefixes );

//    int nbrPermutations = 259;
//    mScript.SetNbrPermutation( nbrPermutations );

//    QMap<int,QString> selectedCovariates;
//    selectedCovariates.insert( -1, "Intercept" );
//    selectedCovariates.insert( 1, "COMP" );
//    selectedCovariates.insert( 2, "Gender" );
//    selectedCovariates.insert( 3, "GestAgeBirth" );
//    mScript.SetCovariates( selectedCovariates );

//    QMap< QPair< int, QString >, bool> matlabInputFiles;
//    QPair< int, QString > adFilePair;
//    adFilePair.first = 0;
//    adFilePair.second = matlabInputADFile;
//    QPair< int, QString > compFilePair;
//    compFilePair.first = 4;
//    compFilePair.second = matlabInputCOMPFile;
//    matlabInputFiles.insert( adFilePair, false );
//    matlabInputFiles.insert( compFilePair, true );
//    mScript.SetInputFiles( matlabInputFiles );

//    bool omnibus = true;
//    mScript.SetOmnibus( omnibus );

//    bool postHoc = true;
//    mScript.SetPostHoc( postHoc );

//    mScript.SetMatlabOutputDir( outputDir + "/MatlabOutputs" );
//    mScript.SetMatlabScriptName( "/FADTTSAnalysis_MatlabScript_" + fiberName + "_" + nbrPermutations + "perm.m" );
//    mScript.SetMVCMPath( "/NIRAL/devel/linux/FADTTS/FADTTS_V3.01_NIRAL/FADTTS/FADTTS/" );

//    QString matlabScript = mScript.GenerateMatlabFiles();

//    bool scriptMatched = CompareFile( matlabScript, expectedMatlabScript );


//    if( !scriptMatched )
//    {
//        std::cerr << std::endl << "Test_GenerateMatlabScript() FAILED:" << std::endl;
//        std::cerr << "\t+ Matlab Script not generated correctly" << std::endl;
//    }
//    else
//    {
//        std::cout << std::endl << "Test_GenerateMatlabScript() PASSED" << std::endl;
//    }

//    return scriptMatched;
    return false;
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
