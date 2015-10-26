#include "TestProcessing.h"

TestProcessing::TestProcessing()
{
}

/**********************************************************************/
/*************************** Tests ************************************/
/**********************************************************************/
bool TestProcessing::Test_isCOMP()
{
    Processing process;
    bool testIsNotCOMP;
    bool testIsCOMP = true;

    QStringList isNotCOMP;
    isNotCOMP << "-48.0514" << "0.0314882" << "0.0913085" << "0.073043" << "0.13709" << "0.121289" << "0.0957538" << "0.0314882" << "0.0913085" << "0.073043" << "0.13709" << "0.121289" << "0.0957538";
    testIsNotCOMP = !process.IsCOMPFile( isNotCOMP );

    QStringList isCOMP_0, isCOMP_1, isCOMP_2, isCOMP_3, isCOMP_4, isCOMP_5, isCOMP_6, isCOMP_7, isCOMP_8, isCOMP_9;
    isCOMP_0 << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "123" << "0" << "276" << "61" << "19" << "0" << "0" << "0" << "632795";
    isCOMP_1 << "123" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "0" << "276" << "61" << "19" << "0" << "0" << "0" << "632795";
    isCOMP_2 << "123" << "0" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "276" << "61" << "19" << "0" << "0" << "0" << "632795";
    isCOMP_3 << "123" << "0" << "276" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "61" << "19" << "0" << "0" << "0" << "632795";
    isCOMP_4 << "123" << "0" << "276" << "61" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "19" << "0" << "0" << "0" << "632795";
    isCOMP_5 << "123" << "0" << "276" << "61" << "19" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "0" << "0" << "0" << "632795";
    isCOMP_6 << "123" << "0" << "276" << "61" << "19" << "0" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "0" << "0" << "632795";
    isCOMP_7 << "123" << "0" << "276" << "61" << "19" << "0" << "0" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "0" << "632795";
    isCOMP_8 << "123" << "0" << "276" << "61" << "19" << "0" << "0" << "0" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "632795";
    isCOMP_9 << "123" << "0" << "276" << "61" << "19" << "0" << "0" << "0" << "632795" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed";

    QList<QStringList> isCOMP;
    isCOMP << isCOMP_0 << isCOMP_1 << isCOMP_2 << isCOMP_3 << isCOMP_4 << isCOMP_5 << isCOMP_6 << isCOMP_7 << isCOMP_8 << isCOMP_9;
    foreach( QStringList isNC, isCOMP )
    {
        testIsCOMP = testIsCOMP & process.IsCOMPFile( isNC );
    }


    if( testIsNotCOMP && testIsCOMP )
    {
        std::cout << std::endl << "Test_isCOMP() PASSED" << std::endl;
    }
    else
    {
        std::cerr << std::endl << "Test_isCOMP() FAILED:" << std::endl;
        if( !testIsNotCOMP )
        {
            std::cerr << "\t+ COMP file detected when it should not" << std::endl;
        }
        if( !testIsCOMP )
        {
            std::cerr << "\t+ COMP file not detected when it should" << std::endl;
        }
    }

    return ( testIsNotCOMP & testIsCOMP );
}

bool TestProcessing::Test_GetSelectedSubjectList( QString selectedSubjectListFileTest )
{
    Processing process;

    QStringList selectedSubjectList = process.GetSelectedSubjectList( selectedSubjectListFileTest );
    QStringList expectedSelectedSubjectList;
    expectedSelectedSubjectList << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                   "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed";

    bool selectedSubjectListMatched = CompareQStringList( selectedSubjectList, expectedSelectedSubjectList );


    if( !selectedSubjectListMatched )
    {
        std::cerr << std::endl << "Test_GetSelectedSubjectList() FAILED:" << std::endl;
        std::cerr << "\t+ Incorrect selected subjectList from subjectList file" << std::endl;
        DisplayErrorSubjectList( expectedSelectedSubjectList, selectedSubjectList );
    }
    else
    {
        std::cout << std::endl << "Test_GetSelectedSubjectList() PASSED" << std::endl;
    }

    return selectedSubjectListMatched;
}

bool TestProcessing::Test_GenerateMatlabInputFiles( QString outputDir, QString inputADFile, QString inputCOMPFileTest1, QString inputCOMPFileTest2,
                                                    QString matlabInputADFile, QString matlabInputCOMPFile, QString selectedSubjectListFileTest )
{
    Processing process;
    int i = 0;
    bool ADFilesMatched = true;
    bool COMPFileMatchedTest1 = true;
    bool COMPFileMatchedTest2 = true;

    QMap<QString, bool> expectedOutput;
    expectedOutput.insert( "00?" + matlabInputADFile, false );
    expectedOutput.insert( "04?" + matlabInputCOMPFile, true );
    QMap<QString, bool>::ConstIterator iterExpectedOutput1 = expectedOutput.begin();
    QMap<QString, bool>::ConstIterator iterExpectedOutput2 = expectedOutput.begin();

    /************************************************/
    /******************** Test 1 ********************/
    /************************************************/
    // AD, RA, MD and FA file
    // Covariates file: subjects on 1st column
    QMap<QString, bool> selectedInputFilesTest1;
    selectedInputFilesTest1.insert( "00?" + inputADFile, false );
    selectedInputFilesTest1.insert( "04?" + inputCOMPFileTest1, true );
    int subjectCovariatesColumnIdTest1 = 0;
    QMap<int, QString> selectedCovariatesTest1;
    selectedCovariatesTest1.insert( -1, "Intercept" );
    selectedCovariatesTest1.insert( 1, "COMP" );
    selectedCovariatesTest1.insert( 2, "Gender" );
    selectedCovariatesTest1.insert( 3, "GestAgeBirth" );
    QString fiberNameTest1 = "Test1";

    QMap<QString, bool> matlabInputFilesTest1 =
            process.GenerateMatlabInputFiles( selectedInputFilesTest1, selectedSubjectListFileTest,
                                              subjectCovariatesColumnIdTest1, selectedCovariatesTest1,
                                              outputDir, fiberNameTest1 );

    QMap<QString, bool>::ConstIterator iterTest1 = matlabInputFilesTest1.begin();
    while( iterTest1 != matlabInputFilesTest1.end() )
    {
        if( !CompareFile( iterExpectedOutput1.key().split( "?" ).last(), iterTest1.key().split( "?" ).last() ) && ( i%2 == 0 ) )
        {
            ADFilesMatched = false;
        }
        if( !CompareFile( iterExpectedOutput1.key().split( "?" ).last(), iterTest1.key().split( "?" ).last() ) && ( i%2 != 0 ) )
        {
            COMPFileMatchedTest1 = false;
        }
        ++iterExpectedOutput1;
        ++iterTest1;
        i++;
    }


    /************************************************/
    /******************** Test 2 ********************/
    /************************************************/
    // Covariates file: subjects not on 1st column
    QMap<QString, bool> selectedInputFilesTest2;
    selectedInputFilesTest2.insert( "00?" + inputADFile, false );
    selectedInputFilesTest2.insert( "04?" + inputCOMPFileTest2, true );
    int subjectCovariatesColumnIdTest2 = 3;
    QMap<int, QString> selectedCovariatesTest2;
    selectedCovariatesTest2.insert( -1, "Intercept" );
    selectedCovariatesTest2.insert( 0, "COMP" );
    selectedCovariatesTest2.insert( 1, "Gender" );
    selectedCovariatesTest2.insert( 2, "GestAgeBirth" );
    QString fiberNameTest2 = "Test2";

    QMap<QString, bool> matlabInputFilesTest2 =
            process.GenerateMatlabInputFiles( selectedInputFilesTest2, selectedSubjectListFileTest,
                                              subjectCovariatesColumnIdTest2, selectedCovariatesTest2,
                                              outputDir, fiberNameTest2 );

    QMap<QString, bool>::ConstIterator iterTest2 = matlabInputFilesTest2.begin();
    while( iterTest2 != matlabInputFilesTest2.end() )
    {
        if( !CompareFile( iterExpectedOutput2.key().split( "?" ).last(), iterTest2.key().split( "?" ).last() ) && ( i%2 != 0 ) )
        {
            COMPFileMatchedTest2 = false;
        }
        ++iterExpectedOutput2;
        ++iterTest2;
        i++;
    }


    if( !ADFilesMatched || !COMPFileMatchedTest1 || !COMPFileMatchedTest2 )
    {
        std::cerr << std::endl << "Test_GenerateMatlabInputFiles() FAILED:" << std::endl;
        if( !ADFilesMatched )
        {
            std::cerr << "\t+ Matlab Script for AD, RD, MD or FA file not generated correctly" << std::endl;
        }
        if( !COMPFileMatchedTest1 )
        {
            std::cerr << "\t+ Matlab Script for covariates file not generated correctly when subjects on 1st column" << std::endl;
        }
        if( !COMPFileMatchedTest2 )
        {
            std::cerr << "\t+ Matlab Script for covariates file not generated correctly when subjects not on 1st column" << std::endl;
        }
    }
    else
    {
        std::cout << std::endl << "Test_GenerateMatlabInputFiles() PASSED" << std::endl;
    }

    return ( ADFilesMatched & COMPFileMatchedTest1 & COMPFileMatchedTest2 );
}



bool TestProcessing::Test_GetSubjectListFromInputFile( QString inputADFile, QString inputCOMPFileTest1, QString inputCOMPFileTest2 )
{
    Processing process;

    /************************************************/
    /******************** Test 1 ********************/
    /************************************************/
    // AD, RA, MD and FA file
    QStringList subjectListTest1 = process.GetSubjectListFromInputFile( inputADFile, 2 );
    QStringList expectedSubjectListTest1;
    expectedSubjectListTest1 << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_ad";

    bool correctSubjectListTest1 = CompareQStringList( subjectListTest1, expectedSubjectListTest1 );


    /************************************************/
    /******************** Test 2 ********************/
    /************************************************/
    // Covariates file: subjects on 1st column
    QStringList subjectListTest2 = process.GetSubjectListFromInputFile( inputCOMPFileTest1, 0 );
    QStringList expectedSubjectListTest2;
    expectedSubjectListTest2 << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_COMP_0";

    bool correctSubjectListTest2 = CompareQStringList( subjectListTest2, expectedSubjectListTest2 );


    /************************************************/
    /******************** Test 3 ********************/
    /************************************************/
    // Covariates file: subjects not on 1st column
    QStringList subjectListTest3 = process.GetSubjectListFromInputFile( inputCOMPFileTest2, 3 );
    QStringList expectedSubjectListTest3;
    expectedSubjectListTest3 << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" <<
                                "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_COMP_3";

    bool correctSubjectListTest3 = CompareQStringList( subjectListTest3, expectedSubjectListTest3 );


    if( !correctSubjectListTest1 || !correctSubjectListTest2 || !correctSubjectListTest3 )
    {
        std::cerr << std::endl << "Test_GetSubjectListFromInputFile() FAILED:" << std::endl;
        if( !correctSubjectListTest1 )
        {
            std::cerr << "\t+ Incorrect subjectList generated from AD, RD, MD or FA file" << std::endl;
            DisplayErrorSubjectList( expectedSubjectListTest1, subjectListTest1 );
        }
        if( !correctSubjectListTest2 )
        {
            std::cerr << "\t+ Incorrect subjectList generated from covariates file when subjects on 1st column" << std::endl;
            DisplayErrorSubjectList( expectedSubjectListTest2, subjectListTest2 );
        }
        if( !correctSubjectListTest3 )
        {
            std::cerr << "\t+ Incorrect subjectList generated from covariates file when subjects not on 1st column" << std::endl;
            DisplayErrorSubjectList( expectedSubjectListTest3, subjectListTest3 );
        }
    }
    else
    {
        std::cout << std::endl << "Test_GetSubjectListFromInputFile() PASSED" << std::endl;
    }

    return ( correctSubjectListTest1 & correctSubjectListTest2 & correctSubjectListTest3 );
}

bool TestProcessing::Test_GetRefSubjectListFromSelectedInputFiles( QString inputADFile, QString inputCOMPFileTest1, QString inputCOMPFileTest2 )
{
    Processing process;

    QStringList expectedRefSubjectListTest;
    expectedRefSubjectListTest << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                  "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" <<
                                  "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" <<
                                  "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_ad";

    /************************************************/
    /******************** Test 1 ********************/
    /************************************************/
    expectedRefSubjectListTest << "randomSubject_COMP_0";
    QMap<QString, bool> selectedInputFilesTest1;
    selectedInputFilesTest1.insert( "00?" + inputADFile, false );
    selectedInputFilesTest1.insert( "04?" + inputCOMPFileTest1, true );
    QStringList refSubjectListTest1 = process.GetRefSubjectListFromSelectedInputFiles( selectedInputFilesTest1, 0 );

    bool refListMatchedTest1 = CompareQStringList( refSubjectListTest1, expectedRefSubjectListTest );


    /************************************************/
    /******************** Test 2 ********************/
    /************************************************/
    expectedRefSubjectListTest.removeLast();
    expectedRefSubjectListTest << "randomSubject_COMP_3";
    QMap<QString, bool> selectedInputFilesTest2;
    selectedInputFilesTest2.insert( "00?" + inputADFile, false );
    selectedInputFilesTest2.insert( "04?" + inputCOMPFileTest2, true );
    QStringList refSubjectListTest2 = process.GetRefSubjectListFromSelectedInputFiles( selectedInputFilesTest2, 3 );

    bool refListMatchedTest2 = CompareQStringList( refSubjectListTest2, expectedRefSubjectListTest );


    if( !refListMatchedTest1 || !refListMatchedTest2 )
    {
        std::cerr << std::endl << "Test_GetRefSubjectListFromSelectedInputFiles() FAILED:" << std::endl;
        if( !refListMatchedTest1 )
        {
            std::cerr << "\t+ Incorrect reference subjectList generated from AD, RD, MD, FA and COMP file when subjects on 1st column" << std::endl;
            DisplayErrorSubjectList( expectedRefSubjectListTest, refSubjectListTest1 );
        }
        if( !refListMatchedTest2 )
        {
            std::cerr << "\t+ Incorrect reference subjectList generated from AD, RD, MD, FA and COMP file when subjects not on 1st column" << std::endl;
            DisplayErrorSubjectList( expectedRefSubjectListTest, refSubjectListTest2 );
        }
    }
    else
    {
        std::cout << std::endl << "Test_GetRefSubjectListFromSelectedInputFiles() PASSED" << std::endl;
    }

    return ( refListMatchedTest1 & refListMatchedTest2 );
}

bool TestProcessing::Test_GetRefSubjectList( QString inputADFile, QString inputCOMPFile, QString subjectListFilePath )
{
    Processing process;

    QMap<QString, bool> selectedInputFiles;
    selectedInputFiles.insert( "00?" + inputADFile, false );
    selectedInputFiles.insert( "04?" + inputCOMPFile, true );
    QStringList expectedRefSubjectList;
    expectedRefSubjectList << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                              "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed";


    /************************************************/
    /******************** Test 1 ********************/
    /************************************************/
    // With subjectList file
    QStringList refSubjectListTest1 = process.GetRefSubjectList( subjectListFilePath, selectedInputFiles, 3 );

    bool refSubjectListMatchedTest1 = CompareQStringList( expectedRefSubjectList, refSubjectListTest1 );


    /************************************************/
    /******************** Test 2 ********************/
    /************************************************/
    // Without subjectList file
    expectedRefSubjectList << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" <<
                              "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_ad" << "randomSubject_COMP_3";
    QStringList refSubjectListTest2 = process.GetRefSubjectList( "", selectedInputFiles, 3 );

    bool refSubjectListMatchedTest2 = CompareQStringList( expectedRefSubjectList, refSubjectListTest2 );


    if( !refSubjectListMatchedTest1 || !refSubjectListMatchedTest2 )
    {
        std::cerr << std::endl << "Test_GetRefSubjectList() FAILED:" << std::endl;
        if( !refSubjectListMatchedTest1 )
        {
            std::cerr << "\t+ Incorrect reference subjectList generated when subjectList file provided" << std::endl;
            DisplayErrorSubjectList( expectedRefSubjectList, refSubjectListTest1 );
        }
        if( !refSubjectListMatchedTest2 )
        {
            std::cerr << "\t+ Incorrect reference subjectList generated when subjectList file not provided" << std::endl;
            DisplayErrorSubjectList( expectedRefSubjectList, refSubjectListTest2 );
        }
    }
    else
    {
        std::cout << std::endl << "Test_GetRefSubjectList() PASSED" << std::endl;
    }

    return ( refSubjectListMatchedTest1 & refSubjectListMatchedTest2 );
}


bool TestProcessing::Test_GetAllSubjectsFromSelectedInputFiles()
{
    int argc = 0;
    char **argv = 0;
    QApplication *app = new QApplication(argc, argv);
    Processing process;
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
    QStringList subjectsCOMPFile;
    subjectsCOMPFile << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                        "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" <<
                        "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" <<
                        "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" <<
                        "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" <<
                        "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" <<
                        "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_COMP_3";
    QMap<QString, QStringList > subjectsMap;
    subjectsMap.insert( "ad", subjectsADFile );
    subjectsMap.insert( "rd", QStringList() );
    subjectsMap.insert( "md", QStringList() );
    subjectsMap.insert( "fa", QStringList() );
    subjectsMap.insert( "COMP", subjectsCOMPFile );
    QMap<QString, QStringList> selectedSubjects = process.GetAllSubjectsFromSelectedInputFiles( checkBoxMap, subjectsMap );
    QMap<QString, QStringList> expectedSubjects;
    expectedSubjects.insert( "ad", subjectsADFile );
    expectedSubjects.insert( "COMP", subjectsCOMPFile );

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
    app->exit();
    return testResult;
}

bool TestProcessing::Test_SortSubjectInInputFile()
{
    Processing process;
    bool testResult = true;

    QStringList refSubjectListInAll;
    refSubjectListInAll << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                           "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" <<
                           "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" <<
                           "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed";
    QStringList refSubjectListInNone;
    refSubjectListInNone << "Stan" << "Kyle" << "Kenny" << "Cartman";
    QStringList refSubjectList = refSubjectListInAll;
    refSubjectList << refSubjectListInNone;
    refSubjectList << "randomSubject_ad" << "randomSubject_COMP_3";
    QMap< QString, QMap<QString, bool> > expectedSortedSubjects;
    foreach (QString subj, refSubjectListInAll)
    {
        ( expectedSortedSubjects[subj] )["ad"] = true;
        ( expectedSortedSubjects[subj] )["COMP"] = true;
    }
    foreach (QString subj, refSubjectListInNone)
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

    QStringList subjectsCOMPFile;
    subjectsCOMPFile << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                        "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" <<
                        "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" <<
                        "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_COMP_3";
    QMap<QString, QStringList> selectedSubjectList;
    selectedSubjectList.insert( "ad", subjectsADFile );
    selectedSubjectList.insert( "COMP", subjectsCOMPFile );
    QMap< QString, QMap<QString, bool> > sortedSubjects = process.SortSubjectInInputFile( refSubjectList, selectedSubjectList );

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
        std::cerr << std::endl << "Test_SortSubjectInInputFile() FAILED:" << std::endl;
        std::cerr << "\t+ Incorrect sorted subject list generated" << std::endl;
        DisplayErrorSortedSubjects( expectedSortedSubjects, sortedSubjects );
    }
    else
    {
        std::cout << std::endl << "Test_SortSubjectInInputFile() PASSED" << std::endl;
    }

    return testResult;
}

bool TestProcessing::Test_AssignSortedSubject()
{
    Processing process;

    QStringList refSubjectListInAll;
    refSubjectListInAll << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" <<
                           "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" <<
                           "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" <<
                           "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed";
    QStringList refSubjectListInNone;
    refSubjectListInNone << "Stan" << "Kyle" << "Kenny" << "Cartman";
    QStringList refSubjectList = refSubjectListInAll;
    refSubjectList << refSubjectListInNone;
    refSubjectList << "randomSubject_ad" << "randomSubject_COMP_3";
    QMap< QString, QMap<QString, bool> > sortedSubjects;
    foreach (QString subj, refSubjectListInAll)
    {
        ( sortedSubjects[subj] )["ad"] = true;
        ( sortedSubjects[subj] )["COMP"] = true;
    }
    foreach (QString subj, refSubjectListInNone)
    {
        ( sortedSubjects[subj] )["ad"] = false;
        ( sortedSubjects[subj] )["COMP"] = false;
    }
    ( sortedSubjects["randomSubject_ad"] )["ad"] = true;
    ( sortedSubjects["randomSubject_ad"] )["COMP"] = false;
    ( sortedSubjects["randomSubject_COMP_3"] )["ad"] = false;
    ( sortedSubjects["randomSubject_COMP_3"] )["COMP"] = true;

    QMap<QString, QStringList > expectedUnMatchedSubjects;
    foreach (QString unMatchedSubj, refSubjectListInNone)
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
    process.AssignSortedSubject( sortedSubjects, matchedSubjects, unMatchedSubjects );

    bool testMatchedSubjects = CompareQStringList( matchedSubjects, refSubjectListInAll );


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
            DisplayErrorSubjectList( refSubjectListInAll, matchedSubjects );
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


void TestProcessing::DisplayErrorSubjectList( QStringList subjectListExpected, QStringList subjectListDisplayed )
{
    std::cerr << "\t   - subjectList expected" << std::endl;
    for( int i = 0; i < subjectListExpected.size(); i++ )
    {
        std::cerr << "\t\t  \"" << subjectListExpected.at( i ).toStdString() << "\"" << std::endl;
    }
    std::cerr << "\t   - subjectList displayed" << std::endl;
    for( int i = 0; i < subjectListDisplayed.size(); i++ )
    {
        std::cerr << "\t\t  \"" << subjectListDisplayed.at( i ).toStdString() << "\"" << std::endl;
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
