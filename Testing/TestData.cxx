#include "TestData.h"

TestData::TestData()
{
}


/**********************************************************************/
/*************************** Tests ************************************/
/**********************************************************************/
bool TestData::Test_InitData()
{
    Data mData;
    mData.InitData();
    bool resultTest1 = true, resultTest2 = true, resultTest3 = true, resultTest4 = true,
            resultTest5 = true, resultTest6 = true, resultTest7 = true;


    foreach ( int index, mData.GetDiffusionPropertiesIndices() )
    {
        resultTest1 = resultTest1 & mData.GetFilename( index ).isEmpty();
        resultTest2 = resultTest2 & mData.GetFileData( index ).isEmpty();
        resultTest3 = resultTest3 & mData.GetNbrRows( index ) == 0;
        resultTest4 = resultTest4 & mData.GetNbrColumns( index ) == 0;
        resultTest5 = resultTest5 & mData.GetSubjects().value( index ).isEmpty();
        resultTest6 = resultTest6 & mData.GetNbrSubjects( index ) == 0;
    }
    resultTest7 = mData.GetCovariateColumnID() == 0;


    bool testInitData_Passed = resultTest1 && resultTest2 && resultTest3
            && resultTest4 && resultTest5 && resultTest6 && resultTest7;
    if( !testInitData_Passed )
    {
        std::cerr << "/!\\ Test_InitData() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with InitData()" << std::endl;
        std::cerr << "/!\\ WARNING /!\\" << std::endl;
        std::cerr << "/!\\ WARNING /!\\ As many classes rely on the Data class, other tests from the Data class and\n" << std::endl;
        std::cerr << "/!\\ WARNING /!\\ tests from other classes may not be accurate as long as Test_InitData() remains failed" << std::endl;
        std::cerr << "/!\\ WARNING /!\\" << std::endl;
    }
    else
    {
        std::cerr << "Test_InitData() PASSED" << std::endl;
    }

    return testInitData_Passed;
}


bool TestData::Test_GetDiffusionPropertyIndices()
{
    Data mData;


    bool resultGetAxialDiffusivityIndex = mData.GetAxialDiffusivityIndex() == 0;
    bool resultGetRadialDiffusivityIndex = mData.GetRadialDiffusivityIndex() == 1;
    bool resultGetMeanDiffusivityIndex = mData.GetMeanDiffusivityIndex() == 2;
    bool resultGetFractionalAnisotropyIndex = mData.GetFractionalAnisotropyIndex() == 3;
    bool resultGetCovariateIndex = mData.GetSubMatrixIndex() == 4;
    bool resultGetAllIndices = mData.GetDiffusionPropertiesIndices() == QList<int>() << 0 << 1 << 2 << 3 << 4;


    bool testGetDiffusionPropertyIndices_Passed = resultGetAxialDiffusivityIndex && resultGetRadialDiffusivityIndex &&
            resultGetMeanDiffusivityIndex && resultGetFractionalAnisotropyIndex && resultGetCovariateIndex &&
            resultGetAllIndices;
    if( !testGetDiffusionPropertyIndices_Passed )
    {
        std::cerr << "/!\\ Test_GetDiffusionPropertyIndices() FAILED /!\\" << std::endl;
        if( !resultGetAxialDiffusivityIndex )
        {
            std::cerr << "\t+ pb with GetAxialDiffusivityIndex()" << std::endl;
        }
        if( !resultGetRadialDiffusivityIndex )
        {
            std::cerr << "\t+ pb with GetRadialDiffusivityIndex()" << std::endl;
        }
        if( !resultGetMeanDiffusivityIndex )
        {
            std::cerr << "\t+ pb with GetMeanDiffusivityIndex()" << std::endl;
        }
        if( !resultGetFractionalAnisotropyIndex )
        {
            std::cerr << "\t+ pb with GetFractionalAnisotropyIndex()" << std::endl;
        }
        if( !resultGetCovariateIndex )
        {
            std::cerr << "\t+ pb with GetCovariateIndex()" << std::endl;
        }
        if( !resultGetAllIndices )
        {
            std::cerr << "\t+ pb with GetDiffusionPropertiesIndices()" << std::endl;
        }
    }
    else
    {
        std::cerr << "Test_GetDiffusionPropertyIndices() PASSED" << std::endl;
    }

    return testGetDiffusionPropertyIndices_Passed;
}

bool TestData::Test_GetDiffusionPropertyName()
{
    Data mData;


    bool resultTest1 = mData.GetDiffusionPropertyName( 0 ) == "ad";
    bool resultTest2 = mData.GetDiffusionPropertyName( 1 ) == "rd";
    bool resultTest3 = mData.GetDiffusionPropertyName( 2 ) == "md";
    bool resultTest4 = mData.GetDiffusionPropertyName( 3 ) == "fa";
    bool resultTest5 = mData.GetDiffusionPropertyName( 4 ) == "subMatrix";
    bool resultTest6 = mData.GetDiffusionPropertyName( 5 ) == "No valid diffussion property";


    bool testGetDiffusionPropertyName_Passed = resultTest1 && resultTest2 && resultTest3
            && resultTest4 && resultTest5 && resultTest6;
    if( !testGetDiffusionPropertyName_Passed )
    {
        std::cerr << "/!\\ Test_GetDiffusionPropertyName() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with GetDiffusionPropertyName( int diffusionPropertyIndex )" << std::endl;
    }
    else
    {
        std::cerr << "Test_GetDiffusionPropertyName() PASSED" << std::endl;
    }

    return testGetDiffusionPropertyName_Passed;
}


bool TestData::Test_GetSetFilename()
{
    Data mData;
    mData.InitData();
    QString filename1 = "./path1/filename1";
    QString filename2 = "./path2/filename2";
    QString filename3 = "./path3/filename2";


    mData.SetFilename( 0 ) = filename1;
    bool resultTest1 = mData.GetFilename( 0 ) == filename1;

    mData.SetFilename( 0 ) = filename2;
    bool resultTest2 = mData.GetFilename( 0 ) == filename2;

    mData.SetFilename( 0 ).clear();
    bool resultTest3 = mData.GetFilename( 0 ).isEmpty();

    mData.SetFilename( 1 ) = filename1;
    bool resultTest4 = mData.GetFilename( 1 ) == filename1;

    mData.SetFilename( 2 ) = filename2;
    bool resultTest5 = mData.GetFilename( 2 ) == filename2;

    mData.SetFilename( 3 ) = filename3;
    bool resultTest6 = mData.GetFilename( 3 ) == filename3;


    bool testGetSetFilename_Passed = resultTest1 && resultTest2 && resultTest3 &&
            resultTest4 && resultTest5 && resultTest6;
    if( !testGetSetFilename_Passed )
    {
        std::cerr << "/!\\ Test_GetSetFilename() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with GetFilename( int diffusionPropertyIndex ) and/or SetFilename( int diffusionPropertyIndex )" << std::endl;
    }
    else
    {
        std::cerr << "Test_GetSetFilename() PASSED" << std::endl;
    }

    return testGetSetFilename_Passed;
}

bool TestData::Test_GetSetFileData()
{
    Data mData;
    mData.InitData();
    QStringList row1 = QStringList() << "La" << "lune" << "était" << "sereine" << "et" << "jouait" << "sur" << "les" << "flots.";
    QStringList row2 = QStringList() << "La" << "fenêtre" << "enfin" << "libre" << "est" << "ouverte" << "à" << "la" << "brise,";
    QStringList row3 = QStringList() << "La" << "sultane" << "regarde," << "et" << "la" << "mer" << "qui" << "se" << "brise,";
    QStringList row4 = QStringList() << "Là-bas," << "d'un" << "flot" << "d'argent" << "brode" << "les" << "noirs" << "îlots.";

    QStringList row5 = QStringList() << "De" << "ses" << "doigts" << "en" << "vibrant" << "s'échappe" << "la" << "guitare.";
    QStringList row6 = QStringList() << "Elle" << "écoute..." << "Un" << "bruit" << "sourd" << "frappe" << "les" << "sourds" << "échos.";
    QStringList row7 = QStringList() << "Est-ce" << "un" << "lourd" << "vaisseau" << "turc" << "qui" << "vient" << "des" << "eaux" << "de" << "Cos,";
    QStringList row8 = QStringList() << "Battant" << "l'archipel" << "grec" << "de" << "sa" << "rame" << "tartare?";

    QStringList row9 = QStringList() << "Sont-ce" << "des" << "cormorans" << "qui" << "plongent" << "tour" << "à" << "tour,";
    QStringList row10 = QStringList() << "Et" << "coupent" << "l'eau," << "qui" << "roule" << "en" << "perles" << "sur" << "leur" << "aile?";
    QStringList row11 = QStringList() << "Est-ce" << "un" << "djinn" << "qui" << "là-haut" << "siffle" << "d'une" << "voix" << "grêle,";
    QStringList row12 = QStringList() << "Et" << "jette" << "dans" << "la" << "mer" << "les" << "créneaux" << "de" << "la" << "tour?";

    QStringList row13 = QStringList() << "Qui" << "trouble" << "ainsi" << "les" << "flots" << "près" << "du" << "sérail" << "des" << "femmes?";
    QStringList row14 = QStringList() << "Ni" << "le" << "noir" << "cormoran," << "sur" << "la" << "vague" << "bercé,";
    QStringList row15 = QStringList() << "Ni" << "les" << "pierres" << "du" << "mur," << "ni" << "le" << "bruit" << "cadencé";
    QStringList row16 = QStringList() << "Du" << "lourd" << "vaisseau," << "rampant" << "sur" << "l'onde" << "avec" << "des" << "rames.";

    QStringList row17 = QStringList() << "Ce" << "sont" << "des" << "sacs" << "pesants," << "d'où" << "partent" << "des" << "sanglots.";
    QStringList row18 = QStringList() << "On" << "verrait," << "en" << "sondant" << "la" << "mer" << "qui" << "les" << "promène,";
    QStringList row19 = QStringList() << "Se" << "mouvoir" << "dans" << "leurs" << "flancs" << "comme" << "une" << "forme" << "humaine...";
    QStringList row20 = QStringList() << "La" << "lune" << "était" << "sereine" << "et" << "jouait" << "sur" << "les" << "flots.";

    QList<QStringList> fileData1 = QList<QStringList>() << row1 << row2 << row3 << row4;
    QList<QStringList> fileData2 = QList<QStringList>() << row5 << row6 << row7 << row8;
    QList<QStringList> fileData3 = QList<QStringList>() << row9 << row10 << row11 << row12;
    QList<QStringList> fileData4 = QList<QStringList>() << row13 << row14 << row15 << row16;
    QList<QStringList> fileData5 = QList<QStringList>() << row17 << row18 << row19 << row20;


    mData.SetFileData( 0 ) = fileData1;
    bool resultTest1 = mData.GetFileData( 0 ) == fileData1;

    mData.SetFileData( 0 ) = fileData2;
    bool resultTest2 = mData.GetFileData( 0 ) == fileData2;

    mData.SetFileData( 0 ).clear();
    bool resultTest3 = mData.GetFileData( 0 ).isEmpty();

    mData.SetFileData( 1 ) = fileData3;
    bool resultTest4 = mData.GetFileData( 1 ) == fileData3;

    mData.SetFileData( 2 ) = fileData4;
    bool resultTest5 = mData.GetFileData( 2 ) == fileData4;

    mData.SetFileData( 3 ) = fileData5;
    bool resultTest6 = mData.GetFileData( 3 ) == fileData5;


    bool testGetSetFileData_Passed = resultTest1 && resultTest2 && resultTest3 &&
            resultTest4 && resultTest5 && resultTest6;
    if( !testGetSetFileData_Passed )
    {
        std::cerr << "/!\\ Test_GetSetFileData() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with GetFileData( int diffusionPropertyIndex ) and/or SetFileData( int diffusionPropertyIndex )" << std::endl;
    }
    else
    {
        std::cerr << "Test_GetSetFileData() PASSED" << std::endl;
    }

    return testGetSetFileData_Passed;
}

bool TestData::Test_GetSetNbrRows()
{
    Data mData;
    mData.InitData();
    int nbrRows1 = 0;
    int nbrRows2 = 400;
    int nbrRows3 = 800;


    mData.SetNbrRows( 0 ) = nbrRows1;
    bool resultTest1 = mData.GetNbrRows( 0 ) == nbrRows1;

    mData.SetNbrRows( 0 ) = nbrRows2;
    bool resultTest2 = mData.GetNbrRows( 0 ) == nbrRows2;

    mData.SetNbrRows( 0 ) = 0;
    bool resultTest3 = mData.GetNbrRows( 0 ) == 0;

    mData.SetNbrRows( 1 ) = nbrRows1;
    bool resultTest4 = mData.GetNbrRows( 1 ) == nbrRows1;

    mData.SetNbrRows( 2 ) = nbrRows2;
    bool resultTest5 = mData.GetNbrRows( 2 ) == nbrRows2;

    mData.SetNbrRows( 3 ) = nbrRows3;
    bool resultTest6 = mData.GetNbrRows( 3 ) == nbrRows3;


    bool testGetSetNbrRows_Passed = resultTest1 && resultTest2 && resultTest3 &&
            resultTest4 && resultTest5 && resultTest6;
    if( !testGetSetNbrRows_Passed )
    {
        std::cerr << "/!\\ Test_GetSetNbrRows() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with GetNbrRows( int diffusionPropertyIndex ) and/or SetNbrRows( int diffusionPropertyIndex )" << std::endl;
    }
    else
    {
        std::cerr << "Test_GetSetNbrRows() PASSED" << std::endl;
    }

    return testGetSetNbrRows_Passed;
}

bool TestData::Test_GetSetNbrColumns()
{
    Data mData;
    mData.InitData();
    int nbrColumns1 = 0;
    int nbrColumns2 = 400;
    int nbrColumns3 = 800;


    mData.SetNbrColumns( 0 ) = nbrColumns1;
    bool resultTest1 = mData.GetNbrColumns( 0 ) == nbrColumns1;

    mData.SetNbrColumns( 0 ) = nbrColumns2;
    bool resultTest2 = mData.GetNbrColumns( 0 ) == nbrColumns2;

    mData.SetNbrColumns( 0 ) = 0;
    bool resultTest3 = mData.GetNbrColumns( 0 ) == 0;

    mData.SetNbrColumns( 1 ) = nbrColumns1;
    bool resultTest4 = mData.GetNbrColumns( 1 ) == nbrColumns1;

    mData.SetNbrColumns( 2 ) = nbrColumns2;
    bool resultTest5 = mData.GetNbrColumns( 2 ) == nbrColumns2;

    mData.SetNbrColumns( 3 ) = nbrColumns3;
    bool resultTest6 = mData.GetNbrColumns( 3 ) == nbrColumns3;


    bool testGetSetNbrColumns_Passed = resultTest1 && resultTest2 && resultTest3 &&
            resultTest4 && resultTest5 && resultTest6;
    if( !testGetSetNbrColumns_Passed )
    {
        std::cerr << "/!\\ Test_GetSetNbrColumns() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with GetNbrColumns( int diffusionPropertyIndex ) and/or SetNbrColumns( int diffusionPropertyIndex )" << std::endl;
    }
    else
    {
        std::cerr << "Test_GetSetNbrColumns() PASSED" << std::endl;
    }

    return testGetSetNbrColumns_Passed;
}

bool TestData::Test_GetSetSubjects()
{
    Data mData;
    mData.InitData();
    QStringList subjectsList1 = QStringList() << "Stan" << "Kyle" << "Kenny" << "Cartman" << "Chef" << "Butters" << "Wendy" << "Craig" << "Pip";
    QStringList subjectsList2 = QStringList() << "Yoda" << "Luke" << "Darth Vader" << "Han Solo" << "Chewbacca" << "R2D2" << "C3PO" << "Obi-Wan" << "Leila";
    QStringList subjectsList3 = QStringList() << "Frodo" << "Gandalf" << "Aragorn" << "Legolas" << "Gimli" << "Boromir" << "Sam" << "Merry" << "Pipino";
    QMap<int, QStringList> subjectsMap;


    mData.SetSubjects( 0 ) = subjectsList1;
    mData.SetSubjects( 1 ) = subjectsList2;
    mData.SetSubjects( 3 ) = subjectsList3;
    mData.SetSubjects( 4 ) = subjectsList1;
    subjectsMap = mData.GetSubjects();

    bool resultTest1 = subjectsMap.value( 0 ) == subjectsList1;
    bool resultTest2 = subjectsMap.value( 1 ) == subjectsList2;
    bool resultTest3 = subjectsMap.value( 2 ).isEmpty();
    bool resultTest4 = subjectsMap.value( 3 ) == subjectsList3;
    bool resultTest5 = subjectsMap.value( 4 ) == subjectsList1;


    bool testGetSetSubjects_Passed = resultTest1 && resultTest2 && resultTest3 &&
            resultTest4 && resultTest5;
    if( !testGetSetSubjects_Passed )
    {
        std::cerr << "/!\\ Test_GetSetSubjects() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with GetSubjects() and/or SetSubjects( int diffusionPropertyIndex )" << std::endl;
    }
    else
    {
        std::cerr << "Test_GetSetSubjects() PASSED" << std::endl;
    }

    return testGetSetSubjects_Passed;
}

bool TestData::Test_GetSetNbrSubjects()
{
    Data mData;
    mData.InitData();
    int nbrSubjects1 = 0;
    int nbrSubjects2 = 400;
    int nbrSubjects3 = 800;


    mData.SetNbrSubjects( 0 ) = nbrSubjects1;
    bool resultTest1 = mData.GetNbrSubjects( 0 ) == nbrSubjects1;

    mData.SetNbrSubjects( 0 ) = nbrSubjects2;
    bool resultTest2 = mData.GetNbrSubjects( 0 ) == nbrSubjects2;

    mData.SetNbrSubjects( 0 ) = 0;
    bool resultTest3 = mData.GetNbrSubjects( 0 ) == 0;

    mData.SetNbrSubjects( 1 ) = nbrSubjects1;
    bool resultTest4 = mData.GetNbrSubjects( 1 ) == nbrSubjects1;

    mData.SetNbrSubjects( 2 ) = nbrSubjects2;
    bool resultTest5 = mData.GetNbrSubjects( 2 ) == nbrSubjects2;

    mData.SetNbrSubjects( 3 ) = nbrSubjects3;
    bool resultTest6 = mData.GetNbrSubjects( 3 ) == nbrSubjects3;


    bool testGetSetNbrSubjects_Passed = resultTest1 && resultTest2 && resultTest3 &&
            resultTest4 && resultTest5 && resultTest6;
    if( !testGetSetNbrSubjects_Passed )
    {
        std::cerr << "/!\\ Test_GetSetNbrSubjects() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with GetNbrSubjects( int diffusionPropertyIndex ) and/or SetNbrSubjects( int diffusionPropertyIndex )" << std::endl;
    }
    else
    {
        std::cerr << "Test_GetSetNbrSubjects() PASSED" << std::endl;
    }

    return testGetSetNbrSubjects_Passed;
}


bool TestData::Test_GetSetCovariates()
{
    Data mData;
    QList< int > covariateIndices = QList< int >() << -1 << 0 << 1 << 2 << 3 << 4;
    QStringList covariateNames = QStringList() << "Intercept" << "Gender" << "Age" << "Group" << "Scanner" << "Direction";
    QMap<int, QString> covariatesMap1, covariatesMap2, covariatesMap3, covariatesMap4;


    for( int i = 0; i < covariateIndices.size(); i++ )
    {
        covariatesMap1.insert( covariateIndices.at( i ), covariateNames.at( i ) );
        if( i > 0 )
        {
            covariatesMap2.insert( covariateIndices.at( i ), covariateNames.at( i ) );
        }
        if( i < 3 )
        {
            covariatesMap3.insert( covariateIndices.at( i ), covariateNames.at( i ) );
        }
        if( i > 0 && i < 3 )
        {
            covariatesMap4.insert( covariateIndices.at( i ), covariateNames.at( i ) );
        }
    }

    mData.SetCovariates() = covariatesMap1;
    bool resultTest1 = mData.GetCovariates() == covariatesMap1;

    mData.SetCovariates() = covariatesMap2;
    bool resultTest2 = mData.GetCovariates() == covariatesMap2;

    mData.SetCovariates() = covariatesMap3;
    bool resultTest3 = mData.GetCovariates() == covariatesMap3;

    mData.SetCovariates() = covariatesMap4;
    bool resultTest4 = mData.GetCovariates() == covariatesMap4;

    mData.SetCovariates().clear();
    bool resultTest5 = mData.GetCovariates().isEmpty();


    bool testGetSetCovariates_Passed = resultTest1 && resultTest2 && resultTest3 && resultTest4 && resultTest5;
    if( !testGetSetCovariates_Passed )
    {
        std::cerr << "/!\\ Test_GetSetCovariates() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with GetCovariate() and/or SetCovariate()" << std::endl;
    }
    else
    {
        std::cerr << "Test_GetSetCovariates() PASSED" << std::endl;
    }

    return testGetSetCovariates_Passed;
}

bool TestData::Test_GetSetCovariateColumnID()
{
    Data mData;
    int covariateColumnID1 = 0;
    int covariateColumnID2 = 4;
    int covariateColumnID3 = 8;


    mData.SetCovariateColumnID() = covariateColumnID1;
    bool resultTest1 = mData.GetCovariateColumnID() == covariateColumnID1;

    mData.SetCovariateColumnID() = covariateColumnID2;
    bool resultTest2 = mData.GetCovariateColumnID() == covariateColumnID2;

    mData.SetCovariateColumnID() = covariateColumnID3;
    bool resultTest3 = mData.GetCovariateColumnID() == covariateColumnID3;

    mData.SetCovariateColumnID() = 0;
    bool resultTest4 = mData.GetCovariateColumnID() == 0;


    bool testGetSetCovariateColumnID_Passed = resultTest1 && resultTest2 && resultTest3 && resultTest4;
    if( !testGetSetCovariateColumnID_Passed )
    {
        std::cerr << "/!\\ Test_GetSetCovariateColumnID() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with GetCovariateColumnID() and/or SetCovariateColumnID()" << std::endl;
    }
    else
    {
        std::cerr << "Test_GetSetCovariateColumnID() PASSED" << std::endl;
    }

    return testGetSetCovariateColumnID_Passed;
}

bool TestData::Test_GetSetOutputDir()
{
    Data mData;
    QString outputDir1 = "./pathToDir1";
    QString outputDir2 = "./pathToDir2";
    QString outputDir3 = "./pathToDir3";


    mData.SetOutputDir() = outputDir1;
    bool resultTest1 = mData.GetOutputDir() == outputDir1;

    mData.SetOutputDir() = outputDir2;
    bool resultTest2 = mData.GetOutputDir() == outputDir2;

    mData.SetOutputDir() = outputDir3;
    bool resultTest3 = mData.GetOutputDir() == outputDir3;

    mData.SetOutputDir().clear();
    bool resultTest4 = mData.GetOutputDir().isEmpty();


    bool testGetSetOutputDir_Passed = resultTest1 && resultTest2 && resultTest3 && resultTest4;
    if( !testGetSetOutputDir_Passed )
    {
        std::cerr << "/!\\ Test_GetSetOutputDir() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with GetOutputDir() and/or SetOutputDir()" << std::endl;
    }
    else
    {
        std::cerr << "Test_GetSetOutputDir() PASSED" << std::endl;
    }

    return testGetSetOutputDir_Passed;
}


bool TestData::Test_ClearFileInformation()
{
    Data mData;
    mData.InitData();
    bool resultTest1, resultTest2, resultTest3, resultTest4, resultTest5, resultTest6;
    QString filename = "./path/filename";

    QStringList row1 = QStringList() << "Demain," << "dès" << "l'aube," << "à" << "l'heure" << "où" << "blanchit" << "la" << "campagne,";
    QStringList row2 = QStringList() << "Je" << "partirai." << "Vois-tu," << "je" << "sais" << "que" << "tu" << "m'attends.";
    QStringList row3 = QStringList() << "J'irai" << "par" << "la" << "forêt," << "j'irai" << "par" << "la" << "montagne.";
    QStringList row4 = QStringList() << "Je" << "ne" << "puis" << "demeurer" << "loin" << "de" << "toi" << "plus" << "longtemps.";

    QStringList row5 = QStringList() << "Je" << "marcherai" << "les" << "yeux" << "fixés" << "sur" << "mes" << "pensées,";
    QStringList row6 = QStringList() << "Sans" << "rien" << "voir" << "au" << "dehors," << "sans" << "entendre" << "aucun" << "bruit,";
    QStringList row7 = QStringList() << "Seul," << "inconnu," << "le" << "dos" << "courbé," << "les" << "mains" << "croisées,";
    QStringList row8 = QStringList() << "Triste," << "et" << "le" << "jour" << "pour" << "moi" << "sera" << "comme" << "la" << "nuit.";

    QStringList row9 = QStringList() << "Je" << "ne" << "regarderai" << "ni" << "l'or" << "du" << "soir" << "qui" << "tombe,";
    QStringList row10 = QStringList() << "Ni" << "les" << "voiles" << "au" << "loin" << "descendant" << "vers" << "Harfleur,";
    QStringList row11 = QStringList() << "Et" << "quand" << "j'arriverai," << "je" << "mettrai" << "sur" << "ta" << "tombe";
    QStringList row12 = QStringList() << "Un" << "bouquet" << "de" << "houx" << "vert" << "et" << "de" << "bruyère" << "en" << "fleur.";

    QList<QStringList> fileData = QList<QStringList>() << row1 << row2 << row3 << row4 << row5 << row6 << row7 << row8 << row9 << row10 << row11 << row12;
    int nbrRows = fileData.size();
    int nbrColumns = fileData.first().size();
    QStringList subjects = QStringList() << "Rachel" << "Ross" << "Monica" << "Chandler" << "Phoebe" << "Gunther";
    int nbrSubjects = subjects.size();

    QList< int > covariateIndices = QList< int >() << -1 << 0 << 1 << 2 << 3 << 4;
    QStringList covariateNames = QStringList() << "Intercept" << "Gender" << "Age" << "Group" << "Scanner" << "Direction";
    QMap<int, QString> covariates;
    for( int i = 0; i < covariateIndices.size(); i++ )
    {
        covariates.insert( covariateIndices.at( i ), covariateNames.at( i ) );
    }


    foreach ( int index, mData.GetDiffusionPropertiesIndices() )
    {
        mData.SetFilename( index ) = filename;
        mData.SetFileData( index ) = fileData;
        mData.SetNbrRows( index ) = nbrRows;
        mData.SetNbrColumns( index ) = nbrColumns;
        mData.SetSubjects( index ) = subjects;
        mData.SetNbrSubjects( index ) = nbrSubjects;
    }

    foreach ( int index, mData.GetDiffusionPropertiesIndices() )
    {
        mData.ClearFileInformation( index );
    }

    foreach ( int index, mData.GetDiffusionPropertiesIndices() )
    {
        resultTest1 = mData.GetFilename( index ).isEmpty();
        resultTest2 = mData.GetFileData( index ).isEmpty();
        resultTest3 = mData.GetNbrRows( index ) == 0;
        resultTest4 = mData.GetNbrColumns( index ) == 0;
        resultTest5 = mData.GetNbrSubjects( index ) == 0;
        resultTest6 = mData.GetSubjects().value( index ).isEmpty();
    }


    bool testClearFileInformation_Passed = resultTest1 && resultTest2 && resultTest3 && resultTest4;
    if( !testClearFileInformation_Passed )
    {
        std::cerr << "/!\\ Test_ClearFileInformation() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with ClearFileInformation( int diffusionPropertyIndex )" << std::endl;
    }
    else
    {
        std::cerr << "Test_ClearFileInformation() PASSED" << std::endl;
    }

    return testClearFileInformation_Passed;
}

bool TestData::Test_AddInterceptToCovariates()
{
    Data mData;
    QList< int > covariateIndices = QList< int >() << 0 << 1 << 2 << 3 << 4;
    QStringList covariateNames = QStringList() << "Gender" << "Age" << "Group" << "Scanner" << "Direction";
    QMap<int, QString> covariates;
    for( int i = 0; i < covariateIndices.size(); i++ )
    {
        covariates.insert( covariateIndices.at( i ), covariateNames.at( i ) );
    }


    mData.AddInterceptToCovariates();
    bool testAddInterceptToCovariates_Passed = mData.GetCovariates().firstKey() == -1 && mData.GetCovariates().first() == "Intercept";


    if( !testAddInterceptToCovariates_Passed )
    {
        std::cerr << "/!\\ Test_AddInterceptToCovariates() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with AddInterceptToCovariates()" << std::endl;
    }
    else
    {
        std::cerr << "Test_AddInterceptToCovariates() PASSED" << std::endl;
    }

    return testAddInterceptToCovariates_Passed;
}
