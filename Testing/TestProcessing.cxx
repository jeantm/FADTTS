#include "TestProcessing.h"

TestProcessing::TestProcessing()
{
}

/**********************************************************************/
/*************************** Tests ************************************/
/**********************************************************************/
bool TestProcessing::Test_IsMatrixDimensionOK( QString inputADFile )
{
    Processing processing;

    QList<QStringList> ADFileData = processing.GetDataFromFile( inputADFile );
    QList<QStringList> randomData;
    QStringList randomDataRow1;
    randomDataRow1 << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "123" << "0" << "276" << "61" << "19" << "0" << "0" << "0" << "632795";
    QStringList randomDataRow2;
    randomDataRow2 << "123" << "0" << "276" << "61" << "19" << "0" << "0" << "0" << "632795";
    randomData.insert( 0, randomDataRow1 );
    randomData.insert( 0, randomDataRow2 );

    bool testMatrixDimensionOK = processing.IsMatrixDimensionOK( ADFileData );
    bool testMatrixDimensionKO = !processing.IsMatrixDimensionOK( randomData );


    if( !testMatrixDimensionOK || !testMatrixDimensionKO )
    {
        std::cerr << std::endl << "Test_IsMatrixDimensionOK() FAILED:" << std::endl;
        if( !testMatrixDimensionOK )
        {
            std::cerr << "\t+ File Data is not detected as square matrix when it should" << std::endl;
        }
        if( !testMatrixDimensionKO )
        {
            std::cerr << "\t+ File Data is detected as square matrix when it should not" << std::endl;
        }
    }
    else
    {
        std::cout << std::endl << "Test_IsMatrixDimensionOK() PASSED" << std::endl;
    }

    return ( testMatrixDimensionOK & testMatrixDimensionKO );
}

bool TestProcessing::Test_IsCovariateFile()
{
    Processing processing;
    bool testIsNotCovariate;
    bool testIsCovariate = true;

    QStringList isNotCovariate;
    isNotCovariate << "-48.0514" << "0.0314882" << "0.0913085" << "0.073043" << "0.13709" << "0.121289" << "0.0957538" << "0.0314882" << "0.0913085" << "0.073043" << "0.13709" << "0.121289" << "0.0957538";
    testIsNotCovariate = !processing.IsCovariateFile( isNotCovariate );

    QStringList isCovariate_0, isCovariate_1, isCovariate_2, isCovariate_3, isCovariate_4, isCovariate_5, isCovariate_6, isCovariate_7, isCovariate_8, isCovariate_9;
    isCovariate_0 << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "123" << "0" << "276" << "61" << "19" << "0" << "0" << "0" << "632795";
    isCovariate_1 << "123" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "0" << "276" << "61" << "19" << "0" << "0" << "0" << "632795";
    isCovariate_2 << "123" << "0" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "276" << "61" << "19" << "0" << "0" << "0" << "632795";
    isCovariate_3 << "123" << "0" << "276" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "61" << "19" << "0" << "0" << "0" << "632795";
    isCovariate_4 << "123" << "0" << "276" << "61" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "19" << "0" << "0" << "0" << "632795";
    isCovariate_5 << "123" << "0" << "276" << "61" << "19" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "0" << "0" << "0" << "632795";
    isCovariate_6 << "123" << "0" << "276" << "61" << "19" << "0" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "0" << "0" << "632795";
    isCovariate_7 << "123" << "0" << "276" << "61" << "19" << "0" << "0" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "0" << "632795";
    isCovariate_8 << "123" << "0" << "276" << "61" << "19" << "0" << "0" << "0" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "632795";
    isCovariate_9 << "123" << "0" << "276" << "61" << "19" << "0" << "0" << "0" << "632795" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed";

    QList<QStringList> isCovariate;
    isCovariate << isCovariate_0 << isCovariate_1 << isCovariate_2 << isCovariate_3 << isCovariate_4 << isCovariate_5 << isCovariate_6 << isCovariate_7 << isCovariate_8 << isCovariate_9;
    foreach( QStringList isNC, isCovariate )
    {
        testIsCovariate = testIsCovariate & processing.IsCovariateFile( isNC );
    }


    if( !testIsNotCovariate || !testIsCovariate )
    {
        std::cerr << std::endl << "Test_IsCovariateFile() FAILED:" << std::endl;
        if( !testIsNotCovariate )
        {
            std::cerr << "\t+ Covariate file is detected when it should not" << std::endl;
        }
        if( !testIsCovariate )
        {
            std::cerr << "\t+ Covariate file is not detected when it should" << std::endl;
        }
    }
    else
    {
        std::cout << std::endl << "Test_IsCovariateFile() PASSED" << std::endl;
    }

    return ( testIsNotCovariate & testIsCovariate );
}

bool TestProcessing::Test_GetSelectedSubjects( QString selectedSubjectsFileTest )
{
    Processing processing;

    QStringList selectedSubjects = processing.GetSelectedSubjects( selectedSubjectsFileTest );
    QStringList expectedselectedSubjects;
    expectedselectedSubjects << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                   "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed";

    bool selectedSubjectsMatched = CompareQStringList( selectedSubjects, expectedselectedSubjects );


    if( !selectedSubjectsMatched )
    {
        std::cerr << std::endl << "Test_GetSelectedSubjects() FAILED:" << std::endl;
        std::cerr << "\t+ Incorrect selected subjects from file" << std::endl;
        DisplayErrorSubjects( expectedselectedSubjects, selectedSubjects );
    }
    else
    {
        std::cout << std::endl << "Test_GetSelectedSubjects() PASSED" << std::endl;
    }

    return selectedSubjectsMatched;
}

bool TestProcessing::Test_GenerateMatlabInputFiles( QString outputDir, QString inputADFile, QString inputCovariateFileTest1, QString inputCovariateFileTest2,
                                                    QString matlabInputADFile, QString matlabInputCovariateFile, QString selectedSubjectsFileTest )
{
    Processing processing;
    int i = 0;
    bool ADFilesMatched = true;
    bool CovariateFileMatchedTest1 = true;
    bool CovariateFileMatchedTest2 = true;

    QMap< QPair< int, QString >, bool > expectedOutput;
    QPair< int, QString > adMatlabFilePair;
    adMatlabFilePair.first = 0;
    adMatlabFilePair.second = matlabInputADFile;
    QPair< int, QString > compMatlabFilePair;
    compMatlabFilePair.first = 4;
    compMatlabFilePair.second = matlabInputCovariateFile;
    expectedOutput.insert( adMatlabFilePair, false );
    expectedOutput.insert( compMatlabFilePair, true );
    QMap< QPair< int, QString >, bool >::ConstIterator iterExpectedOutput1 = expectedOutput.begin();
    QMap< QPair< int, QString >, bool >::ConstIterator iterExpectedOutput2 = expectedOutput.begin();

    /************************************************/
    /******************** Test 1 ********************/
    /************************************************/
    // AD, RA, MD and FA file
    // Covariates file: subjects on 1st column
    QMap< QPair< int, QString >, bool > selectedInputFilesTest1;
    QPair< int, QString > adFilePair;
    adFilePair.first = 0;
    adFilePair.second = inputADFile;
    QPair< int, QString > compFilePairTest1;
    compFilePairTest1.first = 4;
    compFilePairTest1.second = inputCovariateFileTest1;
    selectedInputFilesTest1.insert( adFilePair, false );
    selectedInputFilesTest1.insert( compFilePairTest1, true );
    int subjectCovariatesColumnIdTest1 = 0;
    QMap<int, QString> selectedCovariatesTest1;
    selectedCovariatesTest1.insert( -1, "Intercept" );
    selectedCovariatesTest1.insert( 1, "COMP" );
    selectedCovariatesTest1.insert( 2, "Gender" );
    selectedCovariatesTest1.insert( 3, "GestAgeBirth" );
    QString fiberNameTest1 = "Test1";

    QMap< QPair< int, QString >, bool > matlabInputFilesTest1 =
            processing.GenerateMatlabInputFiles( selectedInputFilesTest1, selectedSubjectsFileTest,
                                                 subjectCovariatesColumnIdTest1, selectedCovariatesTest1,
                                                 outputDir, fiberNameTest1 );

    QMap< QPair< int, QString >, bool >::ConstIterator iterTest1 = matlabInputFilesTest1.begin();
    while( iterTest1 != matlabInputFilesTest1.end() )
    {
        if( !CompareFile( iterExpectedOutput1.key().second, iterTest1.key().second ) && ( i%2 == 0 ) )
        {
            ADFilesMatched = false;
        }
        if( !CompareFile( iterExpectedOutput1.key().second, iterTest1.key().second ) && ( i%2 != 0 ) )
        {
            CovariateFileMatchedTest1 = false;
        }
        ++iterExpectedOutput1;
        ++iterTest1;
        i++;
    }


    /************************************************/
    /******************** Test 2 ********************/
    /************************************************/
    // Covariates file: subjects not on 1st column
    QMap< QPair< int, QString >, bool > selectedInputFilesTest2;
    QPair< int, QString > compFilePairTest2;
    compFilePairTest2.first = 4;
    compFilePairTest2.second = inputCovariateFileTest2;
    selectedInputFilesTest2.insert( adFilePair, false );
    selectedInputFilesTest2.insert( compFilePairTest2, true );
    int subjectCovariatesColumnIdTest2 = 3;
    QMap<int, QString> selectedCovariatesTest2;
    selectedCovariatesTest2.insert( -1, "Intercept" );
    selectedCovariatesTest2.insert( 0, "COMP" );
    selectedCovariatesTest2.insert( 1, "Gender" );
    selectedCovariatesTest2.insert( 2, "GestAgeBirth" );
    QString fiberNameTest2 = "Test2";

    QMap< QPair< int, QString >, bool > matlabInputFilesTest2 =
            processing.GenerateMatlabInputFiles( selectedInputFilesTest2, selectedSubjectsFileTest,
                                                 subjectCovariatesColumnIdTest2, selectedCovariatesTest2,
                                                 outputDir, fiberNameTest2 );

    QMap< QPair< int, QString >, bool >::ConstIterator iterTest2 = matlabInputFilesTest2.begin();
    while( iterTest2 != matlabInputFilesTest2.end() )
    {
        if( !CompareFile( iterExpectedOutput2.key().second, iterTest2.key().second ) && ( i%2 != 0 ) )
        {
            CovariateFileMatchedTest2 = false;
        }
        ++iterExpectedOutput2;
        ++iterTest2;
        i++;
    }


    if( !ADFilesMatched || !CovariateFileMatchedTest1 || !CovariateFileMatchedTest2 )
    {
        std::cerr << std::endl << "Test_GenerateMatlabInputFiles() FAILED:" << std::endl;
        if( !ADFilesMatched )
        {
            std::cerr << "\t+ Matlab Script for AD, RD, MD or FA file not generated correctly" << std::endl;
        }
        if( !CovariateFileMatchedTest1 )
        {
            std::cerr << "\t+ Matlab Script for covariates file not generated correctly when subjects on 1st column" << std::endl;
        }
        if( !CovariateFileMatchedTest2 )
        {
            std::cerr << "\t+ Matlab Script for covariates file not generated correctly when subjects not on 1st column" << std::endl;
        }
    }
    else
    {
        std::cout << std::endl << "Test_GenerateMatlabInputFiles() PASSED" << std::endl;
    }

    return ( ADFilesMatched & CovariateFileMatchedTest1 & CovariateFileMatchedTest2 );
}



bool TestProcessing::Test_GetSubjectsFromInputFile( QString inputADFile, QString inputCovariateFileTest1, QString inputCovariateFileTest2 )
{
    Processing processing;

    /************************************************/
    /******************** Test 1 ********************/
    /************************************************/
    // AD, RA, MD and FA file
    QList<QStringList> dataInInputADFile = processing.GetDataFromFile( inputADFile );
    QStringList subjectsTest1 = processing.GetSubjectsFromInputFile( dataInInputADFile, 2 );
    QStringList expectedSubjectsTest1;
    expectedSubjectsTest1 << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_ad";

    bool correctSubjectsTest1 = CompareQStringList( subjectsTest1, expectedSubjectsTest1 );


    /************************************************/
    /******************** Test 2 ********************/
    /************************************************/
    // Covariates file: subjects on 1st column
    QList<QStringList> dataInInputCovariateFileTest1 = processing.GetDataFromFile( inputCovariateFileTest1 );
    QStringList subjectsTest2 = processing.GetSubjectsFromInputFile( dataInInputCovariateFileTest1, 0 );
    QStringList expectedSubjectsTest2;
    expectedSubjectsTest2 << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_COMP_0";

    bool correctSubjectsTest2 = CompareQStringList( subjectsTest2, expectedSubjectsTest2 );


    /************************************************/
    /******************** Test 3 ********************/
    /************************************************/
    // Covariates file: subjects not on 1st column
    QList<QStringList> dataInInputCovariateFileTest2 = processing.GetDataFromFile( inputCovariateFileTest2 );
    QStringList subjectsTest3 = processing.GetSubjectsFromInputFile( dataInInputCovariateFileTest2, 3 );
    QStringList expectedSubjectsTest3;
    expectedSubjectsTest3 << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_COMP_3";

    bool correctSubjectsTest3 = CompareQStringList( subjectsTest3, expectedSubjectsTest3 );


    if( !correctSubjectsTest1 || !correctSubjectsTest2 || !correctSubjectsTest3 )
    {
        std::cerr << std::endl << "Test_GetSubjectsFromInputFile() FAILED:" << std::endl;
        if( !correctSubjectsTest1 )
        {
            std::cerr << "\t+ Incorrect subjects generated from AD, RD, MD or FA file" << std::endl;
            DisplayErrorSubjects( expectedSubjectsTest1, subjectsTest1 );
        }
        if( !correctSubjectsTest2 )
        {
            std::cerr << "\t+ Incorrect subjects generated from covariates file when subjects on 1st column" << std::endl;
            DisplayErrorSubjects( expectedSubjectsTest2, subjectsTest2 );
        }
        if( !correctSubjectsTest3 )
        {
            std::cerr << "\t+ Incorrect subjects generated from covariates file when subjects not on 1st column" << std::endl;
            DisplayErrorSubjects( expectedSubjectsTest3, subjectsTest3 );
        }
    }
    else
    {
        std::cout << std::endl << "Test_GetSubjectsFromInputFile() PASSED" << std::endl;
    }

    return ( correctSubjectsTest1 & correctSubjectsTest2 & correctSubjectsTest3 );
}

bool TestProcessing::Test_GetRefSubjectsFromSelectedInputFiles( QString inputADFile, QString inputCovariateFileTest1, QString inputCovariateFileTest2 )
{
    Processing processing;

    QStringList expectedRefSubjectsTest;
    expectedRefSubjectsTest << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                  "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                  "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" <<
                                  "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_ad";

    QList<QStringList> dataInInputADFile = processing.GetDataFromFile( inputADFile );
    QList<QStringList> dataInInputCovariateFileTest1 = processing.GetDataFromFile( inputCovariateFileTest1 );
    QList<QStringList> dataInInputCovariateFileTest2 = processing.GetDataFromFile( inputCovariateFileTest2 );

    /************************************************/
    /******************** Test 1 ********************/
    /************************************************/
    expectedRefSubjectsTest << "randomSubject_COMP_0";
    QMap< QPair< int, QString >, QList<QStringList> > dataInSelectedInputFilesTest1;
    QPair< int, QString > adFilePair;
    adFilePair.first = 0;
    adFilePair.second = inputADFile;
    QPair< int, QString > compFilePairTest1;
    compFilePairTest1.first = 4;
    compFilePairTest1.second = inputCovariateFileTest1;
    dataInSelectedInputFilesTest1.insert( adFilePair, dataInInputADFile );
    dataInSelectedInputFilesTest1.insert( compFilePairTest1, dataInInputCovariateFileTest1 );
    QStringList refSubjectsTest1 = processing.GetRefSubjectsFromSelectedInputFiles( dataInSelectedInputFilesTest1, 0 );

    bool refListMatchedTest1 = CompareQStringList( refSubjectsTest1, expectedRefSubjectsTest );


    /************************************************/
    /******************** Test 2 ********************/
    /************************************************/
    expectedRefSubjectsTest.removeLast();
    expectedRefSubjectsTest << "randomSubject_COMP_3";
    QMap< QPair< int, QString >, QList<QStringList> > dataInSelectedInputFilesTest2;
    QPair< int, QString > compFilePairTest2;
    compFilePairTest2.first = 4;
    compFilePairTest2.second = inputCovariateFileTest1;
    dataInSelectedInputFilesTest2.insert( adFilePair, dataInInputADFile );
    dataInSelectedInputFilesTest2.insert( compFilePairTest2, dataInInputCovariateFileTest2 );
    QStringList refSubjectsTest2 = processing.GetRefSubjectsFromSelectedInputFiles( dataInSelectedInputFilesTest2, 3 );

    bool refListMatchedTest2 = CompareQStringList( refSubjectsTest2, expectedRefSubjectsTest );


    if( !refListMatchedTest1 || !refListMatchedTest2 )
    {
        std::cerr << std::endl << "Test_GetRefSubjectsFromSelectedInputFiles() FAILED:" << std::endl;
        if( !refListMatchedTest1 )
        {
            std::cerr << "\t+ Incorrect reference subjects generated from AD, RD, MD, FA and Covariate file when subjects are on 1st column" << std::endl;
            DisplayErrorSubjects( expectedRefSubjectsTest, refSubjectsTest1 );
        }
        if( !refListMatchedTest2 )
        {
            std::cerr << "\t+ Incorrect reference subjects generated from AD, RD, MD, FA and Covariate file when subjects are not on 1st column" << std::endl;
            DisplayErrorSubjects( expectedRefSubjectsTest, refSubjectsTest2 );
        }
    }
    else
    {
        std::cout << std::endl << "Test_GetRefSubjectsFromSelectedInputFiles() PASSED" << std::endl;
    }

    return ( refListMatchedTest1 & refListMatchedTest2 );
}

bool TestProcessing::Test_GetRefSubjects( QString inputADFile, QString inputCovariateFile, QString subjectsFilePath )
{
    Processing processing;

    QList<QStringList> dataInInputADFile = processing.GetDataFromFile( inputADFile );
    QList<QStringList> dataInInputCovariateFile = processing.GetDataFromFile( inputCovariateFile );

    QMap< QPair< int, QString >, QList<QStringList> > dataInSelectedInputFiles;
    QPair< int, QString > adFilePair;
    adFilePair.first = 0;
    adFilePair.second = inputADFile;
    QPair< int, QString > compFilePair;
    compFilePair.first = 4;
    compFilePair.second = inputCovariateFile;
    dataInSelectedInputFiles.insert( adFilePair, dataInInputADFile );
    dataInSelectedInputFiles.insert( compFilePair, dataInInputCovariateFile );
    QStringList expectedRefSubjects;
    expectedRefSubjects << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                              "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed";


    /************************************************/
    /******************** Test 1 ********************/
    /************************************************/
    // With subjects file
    QStringList refSubjectsTest1 = processing.GetRefSubjects( subjectsFilePath, dataInSelectedInputFiles, 3 );

    bool refSubjectsMatchedTest1 = CompareQStringList( expectedRefSubjects, refSubjectsTest1 );


    /************************************************/
    /******************** Test 2 ********************/
    /************************************************/
    // Without subjects file
    expectedRefSubjects << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" <<
                              "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_ad" << "randomSubject_COMP_3";
    QStringList refSubjectsTest2 = processing.GetRefSubjects( "", dataInSelectedInputFiles, 3 );

    bool refSubjectsMatchedTest2 = CompareQStringList( expectedRefSubjects, refSubjectsTest2 );


    if( !refSubjectsMatchedTest1 || !refSubjectsMatchedTest2 )
    {
        std::cerr << std::endl << "Test_GetRefSubjects() FAILED:" << std::endl;
        if( !refSubjectsMatchedTest1 )
        {
            std::cerr << "\t+ Incorrect reference subjects generated when subjects file provided" << std::endl;
            DisplayErrorSubjects( expectedRefSubjects, refSubjectsTest1 );
        }
        if( !refSubjectsMatchedTest2 )
        {
            std::cerr << "\t+ Incorrect reference subjects generated when subjects file not provided" << std::endl;
            DisplayErrorSubjects( expectedRefSubjects, refSubjectsTest2 );
        }
    }
    else
    {
        std::cout << std::endl << "Test_GetRefSubjects() PASSED" << std::endl;
    }

    return ( refSubjectsMatchedTest1 & refSubjectsMatchedTest2 );
}


bool TestProcessing::Test_GetAllSubjectsFromSelectedInputFiles()
{
    Processing processing;
    bool testResult = true;

    QMap<QString, QCheckBox*> checkBoxMap;
    QCheckBox *checkedBox = new QCheckBox();
    checkedBox->setChecked( true );
    QCheckBox *uncheckedBox = new QCheckBox();
    uncheckedBox->setChecked( false );
    checkBoxMap.insert( "ad", checkedBox );
    checkBoxMap.insert( "rd", uncheckedBox );
    checkBoxMap.insert( "md", uncheckedBox );
    checkBoxMap.insert( "fa", uncheckedBox );
    checkBoxMap.insert( "COMP", checkedBox );
    QStringList subjectsADFile;
    subjectsADFile << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                      "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" <<
                      "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" <<
                      "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" <<
                      "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" <<
                      "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" <<
                      "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_ad";
    QStringList subjectsCovariateFile;
    subjectsCovariateFile << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                             "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" <<
                             "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" <<
                             "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" <<
                             "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" <<
                             "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" <<
                             "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_COMP_3";
    QMap<QString, QStringList > allSubjects;
    allSubjects.insert( "ad", subjectsADFile );
    allSubjects.insert( "rd", QStringList() );
    allSubjects.insert( "md", QStringList() );
    allSubjects.insert( "fa", QStringList() );
    allSubjects.insert( "COMP", subjectsCovariateFile );
    QMap<QString, QStringList> selectedSubjects = processing.GetAllSubjectsFromSelectedInputFiles( checkBoxMap, allSubjects );
    QMap<QString, QStringList> expectedSubjects;
    expectedSubjects.insert( "ad", subjectsADFile );
    expectedSubjects.insert( "COMP", subjectsCovariateFile );

    QMap<QString, QStringList >::ConstIterator iterExpectedSubjects = expectedSubjects.begin();
    QMap<QString, QStringList >::ConstIterator iterSelectedSubjects = selectedSubjects.begin();
    while( iterExpectedSubjects != expectedSubjects.constEnd() )
    {
        if( iterExpectedSubjects.key() != iterSelectedSubjects.key() || !CompareQStringList( iterExpectedSubjects.value(), iterSelectedSubjects.value() ) )
        {
            testResult = false;
        }
        ++iterExpectedSubjects;
        ++iterSelectedSubjects;
    }


    if( !testResult )
    {
        std::cerr << std::endl << "Test_GetAllSubjectsFromSelectedInputFiles() FAILED:" << std::endl;
        std::cerr << "\t+ Incorrect subject list from selected input files" << std::endl;
        DisplayErrorAllSubjects( expectedSubjects, selectedSubjects );
    }
    else
    {
        std::cout << std::endl << "Test_GetAllSubjectsFromSelectedInputFiles() PASSED" << std::endl;
    }

    delete checkedBox;
    return testResult;
}

bool TestProcessing::Test_SortSubjects()
{
    Processing processing;
    bool testResult = true;

    QStringList refSubjectsInAll;
    refSubjectsInAll << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                           "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" <<
                           "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" <<
                           "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed";
    QStringList refSubjectsInNone;
    refSubjectsInNone << "Stan" << "Kyle" << "Kenny" << "Cartman";
    QStringList refSubjects = refSubjectsInAll;
    refSubjects << refSubjectsInNone;
    refSubjects << "randomSubject_ad" << "randomSubject_COMP_3";
    QMap< QString, QMap<QString, bool> > expectedSortedSubjects;
    foreach (QString subj, refSubjectsInAll)
    {
        ( expectedSortedSubjects[subj] )["ad"] = true;
        ( expectedSortedSubjects[subj] )["COMP"] = true;
    }
    foreach (QString subj, refSubjectsInNone)
    {
        ( expectedSortedSubjects[subj] )["ad"] = false;
        ( expectedSortedSubjects[subj] )["COMP"] = false;
    }
    ( expectedSortedSubjects["randomSubject_ad"] )["ad"] = true;
    ( expectedSortedSubjects["randomSubject_ad"] )["COMP"] = false;
    ( expectedSortedSubjects["randomSubject_COMP_3"] )["ad"] = false;
    ( expectedSortedSubjects["randomSubject_COMP_3"] )["COMP"] = true;

    QStringList subjectsADFile;
    subjectsADFile << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                      "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" <<
                      "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" <<
                      "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_ad";

    QStringList subjectsCovariateFile;
    subjectsCovariateFile << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                             "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" <<
                             "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" <<
                             "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_COMP_3";
    QMap<QString, QStringList> selectedSubjectss;
    selectedSubjectss.insert( "ad", subjectsADFile );
    selectedSubjectss.insert( "COMP", subjectsCovariateFile );
    QMap< QString, QMap<QString, bool> > sortedSubjects = processing.SortSubjects( refSubjects, selectedSubjectss );

    QMap< QString, QMap<QString, bool> >::ConstIterator iterSubj = sortedSubjects.begin();
    QMap< QString, QMap<QString, bool> >::ConstIterator iterExpectedSubj = expectedSortedSubjects.begin();
    while( iterSubj != sortedSubjects.end() )
    {
        QMap<QString, bool>::ConstIterator iterFile = iterSubj.value().begin();
        QMap<QString, bool>::ConstIterator iterExpectedFile = iterExpectedSubj.value().begin();
        while( iterFile != iterSubj.value().end() )
        {
            if( ( iterSubj.key() != iterSubj.key() ) ||
                    ( expectedSortedSubjects[iterSubj.key()] != sortedSubjects[iterSubj.key()] ) ||
                    ( ( expectedSortedSubjects[iterSubj.key()] )[iterFile.key()] != ( sortedSubjects[iterSubj.key()] )[iterFile.key()] ) )
            {
                testResult = false;
            }
            ++iterFile;
            ++iterExpectedFile;
        }
        ++iterSubj;
        ++iterExpectedSubj;
    }


    if( !testResult )
    {
        std::cerr << std::endl << "Test_SortSubjects() FAILED:" << std::endl;
        std::cerr << "\t+ Incorrect sorted subject list generated" << std::endl;
        DisplayErrorSortedSubjects( expectedSortedSubjects, sortedSubjects );
    }
    else
    {
        std::cout << std::endl << "Test_SortSubjects() PASSED" << std::endl;
    }

    return testResult;
}

bool TestProcessing::Test_AssignSortedSubject()
{
    Processing processing;

    QStringList refSubjectsInAll;
    refSubjectsInAll << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                           "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" <<
                           "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" <<
                           "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed";
    QStringList refSubjectsInNone;
    refSubjectsInNone << "Stan" << "Kyle" << "Kenny" << "Cartman";
    QStringList refSubjects = refSubjectsInAll;
    refSubjects << refSubjectsInNone;
    refSubjects << "randomSubject_ad" << "randomSubject_COMP_3";
    QMap< QString, QMap<QString, bool> > sortedSubjects;
    foreach (QString subj, refSubjectsInAll)
    {
        ( sortedSubjects[subj] )["ad"] = true;
        ( sortedSubjects[subj] )["COMP"] = true;
    }
    foreach (QString subj, refSubjectsInNone)
    {
        ( sortedSubjects[subj] )["ad"] = false;
        ( sortedSubjects[subj] )["COMP"] = false;
    }
    ( sortedSubjects["randomSubject_ad"] )["ad"] = true;
    ( sortedSubjects["randomSubject_ad"] )["COMP"] = false;
    ( sortedSubjects["randomSubject_COMP_3"] )["ad"] = false;
    ( sortedSubjects["randomSubject_COMP_3"] )["COMP"] = true;

    QMap<QString, QStringList > expectedUnMatchedSubjects;
    foreach (QString unMatchedSubj, refSubjectsInNone)
    {
        expectedUnMatchedSubjects.insert( unMatchedSubj, QStringList( "refList" ) );
    }
    QStringList slist1;
    slist1 << "ad" << "refList";
    QStringList slist2;
    slist2 << "COMP" << "refList";
    expectedUnMatchedSubjects.insert( "randomSubject_ad", slist1 );
    expectedUnMatchedSubjects.insert( "randomSubject_COMP_3", slist2 );

    QStringList matchedSubjects;
    QMap<QString, QStringList > unMatchedSubjects;
    processing.AssignSortedSubject( sortedSubjects, matchedSubjects, unMatchedSubjects );

    bool testMatchedSubjects = CompareQStringList( matchedSubjects, refSubjectsInAll );


    bool testUnMatchedSubjects = true;
    QMap<QString, QStringList >::ConstIterator iterExpectedUnMatched = expectedUnMatchedSubjects.begin();
    QMap<QString, QStringList >::ConstIterator iterUnMatched = unMatchedSubjects.begin();
    while( iterUnMatched != unMatchedSubjects.end() )
    {
        testUnMatchedSubjects = testUnMatchedSubjects & ( iterUnMatched.key() == iterExpectedUnMatched.key() ) &
                CompareQStringList( iterUnMatched.value(), iterExpectedUnMatched.value() );
        ++iterUnMatched;
        ++iterExpectedUnMatched;
    }


    if( !testMatchedSubjects || !testUnMatchedSubjects )
    {
        std::cerr << std::endl << "Test_AssignSortedSubject() FAILED:" << std::endl;
        if( !testMatchedSubjects )
        {
            std::cerr << "\t+ Matched subjects not correctly assigned" << std::endl;
            DisplayErrorSubjects( refSubjectsInAll, matchedSubjects );
        }
        if( !testUnMatchedSubjects )
        {
            std::cerr << "\t+ UnMatched subjects not correctly assigned" << std::endl;
            DisplayErrorUnMatchedSubjects( expectedUnMatchedSubjects, unMatchedSubjects );
        }
    }
    else
    {
        std::cout << std::endl << "Test_AssignSortedSubject() PASSED" << std::endl;
    }

    return ( testMatchedSubjects & testUnMatchedSubjects );
}


bool TestProcessing::Test_GetCovariatesFromFileData( QString inputCovariateFileTest1, QString inputCovariateFileTest2 )
{
    Processing processing;

    QMap<int, QString> covariatesExpectedTest1;
    covariatesExpectedTest1.insert( 1, "COMP" );
    covariatesExpectedTest1.insert( 2, "Gender" );
    covariatesExpectedTest1.insert( 3, "GestAgeBirth" );
    QMap<int, QString> covariatesExpectedTest2;
    covariatesExpectedTest2.insert( 0, "COMP" );
    covariatesExpectedTest2.insert( 1, "Gender" );
    covariatesExpectedTest2.insert( 2, "GestAgeBirth" );
    QMap<int, QString> covariatesTest1 = processing.GetCovariatesFromFileData( processing.GetDataFromFile( inputCovariateFileTest1 ), 0 );
    QMap<int, QString> covariatesTest2 = processing.GetCovariatesFromFileData( processing.GetDataFromFile( inputCovariateFileTest2 ), 3 );

    bool resultTest1 = covariatesTest1 == covariatesExpectedTest1;
    bool resultTest2 = covariatesTest2 == covariatesExpectedTest2;


    if( !resultTest1 || !resultTest2 )
    {
        std::cerr << std::endl << "Test_GetCovariatesFromFileData() FAILED:" << std::endl;
        if( !resultTest1 )
        {
            std::cerr << "\t+ Covariates retrieve from " << QFileInfo( QFile( inputCovariateFileTest1 ) ).fileName().toStdString() << " are not the ones expected" <<std::endl;
            DisplayErrorCovariates( covariatesExpectedTest1, covariatesTest1 );
        }
        if( !resultTest2 )
        {
            std::cerr << "\t+ Covariates retrieve from " << QFileInfo( QFile( inputCovariateFileTest2 ) ).fileName().toStdString() << " are not the ones expected" <<std::endl;
            DisplayErrorCovariates( covariatesExpectedTest2, covariatesTest2 );
        }
    }
    else
    {
        std::cout << std::endl << "Test_GetCovariatesFromFileData() PASSED" << std::endl;
    }

    return ( resultTest1 & resultTest2 );
}


/**********************************************************************/
/********************** Functions Used For Testing ********************/
/**********************************************************************/
bool TestProcessing::CompareFile( QString fileName1, QString fileName2 )
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

QByteArray TestProcessing::GetHashFile( QString filename )
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

bool TestProcessing::CompareQStringList( QStringList qStringList1, QStringList qStringList2 )
{
    QSet<QString> diff1 = qStringList1.toSet().subtract(qStringList2.toSet());
    QSet<QString> diff2 = qStringList2.toSet().subtract(qStringList1.toSet());

    if( ( diff1.size() == 0 ) && ( diff2.size() == 0 ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}


void TestProcessing::DisplayErrorSubjects( QStringList subjectsExpected, QStringList subjectsDisplayed )
{
    std::cerr << "\t   - subjects expected" << std::endl;
    for( int i = 0; i < subjectsExpected.size(); i++ )
    {
        std::cerr << "\t\t  \"" << subjectsExpected.at( i ).toStdString() << "\"" << std::endl;
    }
    std::cerr << "\t   - subjects displayed" << std::endl;
    for( int i = 0; i < subjectsDisplayed.size(); i++ )
    {
        std::cerr << "\t\t  \"" << subjectsDisplayed.at( i ).toStdString() << "\"" << std::endl;
    }
}

void TestProcessing::DisplayErrorAllSubjects( QMap<QString, QStringList> expectedSubjects, QMap<QString, QStringList> selectedSubjects )
{
    QMap<QString, QStringList>::ConstIterator iterExpectedSubjects = expectedSubjects.begin();
    std::cerr << "\t   - subjects expected" << std::endl;
    while( iterExpectedSubjects != expectedSubjects.end() )
    {
        std::cerr << "\t\t  " << iterExpectedSubjects.key().toStdString() << std::endl;
        foreach( QString subject, iterExpectedSubjects.value() )
        {
            std::cerr << "\t\t\t\"" << subject.toStdString() << "\"" << std::endl;
        }
        ++iterExpectedSubjects;
    }

    QMap<QString, QStringList>::ConstIterator iterSubjects = selectedSubjects.begin();
    std::cerr << "\t   - subjects retrieved" << std::endl;
    while( iterSubjects != selectedSubjects.end() )
    {
        std::cerr << "\t\t  " << iterSubjects.key().toStdString() << std::endl;
        foreach( QString subject, iterSubjects.value() )
        {
            std::cerr << "\t\t\t\"" << subject.toStdString() << "\"" << std::endl;
        }
        ++iterSubjects;
    }
}

void TestProcessing::DisplayErrorSortedSubjects( QMap< QString, QMap<QString, bool> > expectedSortedSubjects, QMap< QString, QMap<QString, bool> > sortedSubjects )
{
    QMap< QString, QMap<QString, bool> >::ConstIterator iterExpectedSortedSubjects = expectedSortedSubjects.begin();
    std::cerr << "\t   - sorted subjects expected" << std::endl;
    while( iterExpectedSortedSubjects != expectedSortedSubjects.end() )
    {
        std::cerr << "\t\t  \"" << iterExpectedSortedSubjects.key().toStdString() << "\" --> ";
        QMap<QString, bool>::ConstIterator iterExpectedFile = iterExpectedSortedSubjects.value().begin();
        while( iterExpectedFile != iterExpectedSortedSubjects.value().end() )
        {
            std::cerr << " " << iterExpectedFile.key().toStdString() << "[" << iterExpectedFile.value() << "]";
            ++iterExpectedFile;
        }
        std::cerr << std::endl;
        ++iterExpectedSortedSubjects;
    }

    QMap< QString, QMap<QString, bool> >::ConstIterator iterSortedSubjects = sortedSubjects.begin();
    std::cerr << "\t   - sorted subjects displayed" << std::endl;
    while( iterSortedSubjects != sortedSubjects.end() )
    {
        std::cerr << "\t\t  \"" << iterSortedSubjects.key().toStdString() << "\" --> ";
        QMap<QString, bool>::ConstIterator iterFile = iterSortedSubjects.value().begin();
        while( iterFile != iterSortedSubjects.value().end() )
        {
            std::cerr << " " << iterFile.key().toStdString() << "[" << iterFile.value() << "]";
            ++iterFile;
        }
        std::cerr << std::endl;
        ++iterSortedSubjects;
    }
}

void TestProcessing::DisplayErrorUnMatchedSubjects( QMap<QString, QStringList > unMatchedSubjectsExpected, QMap<QString, QStringList > unMatchedSubjectsDisplayed )
{
    QMap<QString, QStringList >::ConstIterator iterExpectedUnMatched = unMatchedSubjectsExpected.begin();
    std::cerr << "\t   - unMatched subjects expected" << std::endl;
    while( iterExpectedUnMatched != unMatchedSubjectsExpected.end() )
    {
        std::cerr << "\t\t  \"" << iterExpectedUnMatched.key().toStdString() << "\" --> ";
        foreach( QString str, iterExpectedUnMatched.value() )
        {
            std::cerr << str.toStdString() << " ";
        }
        std::cerr << std::endl;
        ++iterExpectedUnMatched;
    }

    QMap<QString, QStringList >::ConstIterator iterUnMatched = unMatchedSubjectsDisplayed.begin();
    std::cerr << "\t   - unMatched subjects displayed" << std::endl;
    while( iterUnMatched != unMatchedSubjectsDisplayed.end() )
    {
        std::cerr << "\t\t  \"" << iterUnMatched.key().toStdString() << "\" --> ";
        foreach( QString str, iterUnMatched.value() )
        {
            std::cerr << str.toStdString() << " ";
        }
        std::cerr << std::endl;
        ++iterUnMatched;
    }
}

void TestProcessing::DisplayErrorCovariates( QMap<int, QString> covariatesExpected, QMap<int, QString> covariatesDisplayed )
{
    std::cerr << "\t   - covariates expected" << std::endl;
    QMap<int, QString>::ConstIterator iterCovariatesExpected = covariatesExpected.begin();
    while( iterCovariatesExpected != covariatesExpected.end() )
    {
        std::cerr << "\t\t  \"" << iterCovariatesExpected.value().toStdString() << "\" on column " << iterCovariatesExpected.key() << std::endl;
        ++iterCovariatesExpected;
    }
    std::cerr << "\t   - covariates displayed" << std::endl;
    QMap<int, QString>::ConstIterator iterCovariatesDisplayed = covariatesDisplayed.begin();
    while( iterCovariatesDisplayed != covariatesDisplayed.end() )
    {
        std::cerr << "\t\t  \"" << iterCovariatesDisplayed.value().toStdString() << "\" on column " << iterCovariatesDisplayed.key() << std::endl;
        ++iterCovariatesDisplayed;
    }
}
