#include "TestProcessing.h"

//#include <QDebug>

TestProcessing::TestProcessing()
{
}

/**********************************************************************/
/*************************** Tests ************************************/
/**********************************************************************/
bool TestProcessing::Test_GetDataFromFile( QString file_N_Path, QString file_R_Path, QString file_RN_Path )
{
    Processing processing;
    QStringList dataReference_row = QStringList() << "a" << "b" << "c" << "d" << "e" << "f";
    QList< QStringList > dataReference = QList< QStringList >() << dataReference_row << dataReference_row <<
                                                                   dataReference_row << dataReference_row;


    bool testGetDataFromFile_N = processing.GetDataFromFile( file_N_Path ) == dataReference;
    bool testGetDataFromfile_R_Path = processing.GetDataFromFile( file_R_Path ) == dataReference;
    bool testGetDataFromFile_RN = processing.GetDataFromFile( file_RN_Path ) == dataReference;


    bool testGetDataFromFile_Passed = testGetDataFromFile_N && testGetDataFromfile_R_Path && testGetDataFromFile_RN;
    if( !testGetDataFromFile_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetDataFromFile() FAILED /!\\/!\\";
        //        std::cerr << "\t+ pb with GetDataFromFile( QString filePath )" << std::endl;
        //        if( !testGetDataFromFile_N )
        //        {
        //            std::cerr << "\t  not reading line feed character '\\n' (Used as a new line character in Unix/Mac OS X)" << std::endl;
        //        }
        //        if( !testGetDataFromfile_R_Path )
        //        {
        //            std::cerr << "\t  not reading carriage return character '\\r' (Used as a new line character in Mac OS before X)" << std::endl;
        //        }
        //        if( !testGetDataFromFile_RN )
        //        {
        //            std::cerr << "\t  not reading carriage return line feed character '\\r\\n' (Used as a new line character in Windows)" << std::endl;
        //        }
        //        std::cerr << std::endl;
        //        std::cerr << "/!\\ WARNING /!\\ As many functions from many classes heavly rely on GetDataFromFile( QString filePath ),\n"
        //                     "/!\\ WARNING /!\\ tests from every classes may not be accurate as long as Test_GetDataFromFile() remains failed" << std::endl;
        //        std::cerr << std::endl;
    }
    else
    {
        std::cerr << "Test_GetDataFromFile() PASSED";
    }

    return testGetDataFromFile_Passed;
}


bool TestProcessing::Test_IsMatrixDimensionOK( QString adFilePath )
{
    Processing processing;
    QList< QStringList > randomData;
    QStringList randomDataRow1 = QStringList() << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "123" << "0"
                                               << "276" << "61" << "19" << "0" << "0" << "0" << "632795";
    QStringList randomDataRow2 = QStringList() << "123" << "0" << "276" << "61" << "19" << "0" << "0" << "0" << "632795";
    randomData.append( QList< QStringList >() << randomDataRow1 << randomDataRow2 );


    bool testMatrixDimensionOK = processing.IsMatrixDimensionOK( processing.GetDataFromFile( adFilePath ) );
    bool testMatrixDimensionKO = !processing.IsMatrixDimensionOK( randomData );


    bool testIsMatrixDimensionOK_Passed = testMatrixDimensionOK && testMatrixDimensionKO;
    if( !testIsMatrixDimensionOK_Passed )
    {
        std::cerr << "/!\\/!\\ Test_IsMatrixDimensionOK() FAILED /!\\/!\\";
        //        std::cerr << "\t+ pb with IsMatrixDimensionOK( const QList< QStringList > data )" << std::endl;
        //        if( !testMatrixDimensionOK )
        //        {
        //            std::cerr << "\t  data do not have the same number of columns for each row when it should" << std::endl;
        //        }
        //        if( !testMatrixDimensionKO )
        //        {
        //            std::cerr << "\t  data have the same number of columns for each row when it should not" << std::endl;
        //        }
    }
    else
    {
        std::cerr << "Test_IsMatrixDimensionOK() PASSED";
    }

    return testIsMatrixDimensionOK_Passed;
}

bool TestProcessing::Test_IsSubMatrix()
{
    Processing processing;
    bool testFileIsSubMatrix = true;
    QStringList notSubMatrixFile = QStringList() << "-48.0514" << "0.0314882" << "0.0913085" << "0.073043" << "0.13709" << "0.121289"
                                                 << "0.0957538" << "0.0314882" << "0.0913085" << "0.073043" << "0.13709" << "0.121289" << "0.0957538";
    QStringList subMatrixFile_0 = QStringList() << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "123" << "0" << "276" << "61" << "19" << "0" << "0" << "0" << "632795";
    QStringList subMatrixFile_1 = QStringList() << "123" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "0" << "276" << "61" << "19" << "0" << "0" << "0" << "632795";
    QStringList subMatrixFile_2 = QStringList() << "123" << "0" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "276" << "61" << "19" << "0" << "0" << "0" << "632795";
    QStringList subMatrixFile_3 = QStringList() << "123" << "0" << "276" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "61" << "19" << "0" << "0" << "0" << "632795";
    QStringList subMatrixFile_4 = QStringList() << "123" << "0" << "276" << "61" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "19" << "0" << "0" << "0" << "632795";
    QStringList subMatrixFile_5 = QStringList() << "123" << "0" << "276" << "61" << "19" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "0" << "0" << "0" << "632795";
    QStringList subMatrixFile_6 = QStringList() << "123" << "0" << "276" << "61" << "19" << "0" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "0" << "0" << "632795";
    QStringList subMatrixFile_7 = QStringList() << "123" << "0" << "276" << "61" << "19" << "0" << "0" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "0" << "632795";
    QStringList subMatrixFile_8 = QStringList() << "123" << "0" << "276" << "61" << "19" << "0" << "0" << "0" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "632795";
    QStringList subMatrixFile_9 = QStringList() << "123" << "0" << "276" << "61" << "19" << "0" << "0" << "0" << "632795" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed";


    /*bool testFileIsNotSubMatrix = !processing.IsSubMatrix( notSubMatrixFile );
    testFileIsSubMatrix = testFileIsSubMatrix & processing.IsSubMatrix( subMatrixFile_0 );
    testFileIsSubMatrix = testFileIsSubMatrix & processing.IsSubMatrix( subMatrixFile_1 );
    testFileIsSubMatrix = testFileIsSubMatrix & processing.IsSubMatrix( subMatrixFile_2 );
    testFileIsSubMatrix = testFileIsSubMatrix & processing.IsSubMatrix( subMatrixFile_3 );
    testFileIsSubMatrix = testFileIsSubMatrix & processing.IsSubMatrix( subMatrixFile_4 );
    testFileIsSubMatrix = testFileIsSubMatrix & processing.IsSubMatrix( subMatrixFile_5 );
    testFileIsSubMatrix = testFileIsSubMatrix & processing.IsSubMatrix( subMatrixFile_6 );
    testFileIsSubMatrix = testFileIsSubMatrix & processing.IsSubMatrix( subMatrixFile_7 );
    testFileIsSubMatrix = testFileIsSubMatrix & processing.IsSubMatrix( subMatrixFile_8 );
    testFileIsSubMatrix = testFileIsSubMatrix & processing.IsSubMatrix( subMatrixFile_9 );*/


    bool testIsSubMatrix_Passed /*= testFileIsNotSubMatrix && testFileIsSubMatrix*/;
    if( !testIsSubMatrix_Passed )
    {
        std::cerr << "/!\\/!\\ Test_IsSubMatrix() FAILED /!\\/!\\";
        //        std::cerr << "\t+ pb with IsSubMatrix( const QStringList dataSecondRow )" << std::endl;
        //        if( !testFileIsNotSubMatrix )
        //        {
        //            std::cerr << "\t  subMatrix file is detected when it should not be" << std::endl;
        //        }
        //        if( !testFileIsSubMatrix )
        //        {
        //            std::cerr << "\t  subMatrix file is not detected when it should be" << std::endl;
        //        }
    }
    else
    {
        std::cerr << "Test_IsSubMatrix() PASSED";
    }

    return testIsSubMatrix_Passed;
}


bool TestProcessing::Test_GetSubjectsFromFileList( QString subjectFilePath )
{
    Processing processing;
    QStringList subjectsExpected = QStringList() << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" <<  "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed"
                                                 << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed"
                                                 << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed";
    QStringList subjectsDisplayed = processing.GetSubjectsFromFileList( subjectFilePath );


    bool testGetSubjectsFromFile_Passed = subjectsExpected == subjectsDisplayed;
    if( !testGetSubjectsFromFile_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetSubjectsFromFileList() FAILED /!\\/!\\";
        //        std::cerr << "\t+ pb with GetSubjectsFromFile( QString filePath )" << std::endl;
        //        DisplayError_GetSubjects( subjectsExpected, subjectsDisplayed );
    }
    else
    {
        std::cerr << "Test_GetSubjectsFromFileList() PASSED";
    }

    return testGetSubjectsFromFile_Passed;
}

bool TestProcessing::Test_GetSubjectsFromData( QString adFilePath, QString subMatrix0FilePath, QString subMatrix3FilePath )
{
    Processing processing;
    // AD, RA, MD or FA file
    QList< QStringList > adData = processing.GetDataFromFile( adFilePath );
    QStringList adSubjects = processing.GetSubjectsFromData( adData, 2 );
    QStringList expectedADSubjects = QStringList() << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed"
                                                   << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed"
                                                   << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed"
                                                   << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed"
                                                   << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed"
                                                   << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed"
                                                   << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_ad";
    expectedADSubjects.sort();

    // SubMatrix file: subjects on 1st column
    QList< QStringList > subMatrix0Data = processing.GetDataFromFile( subMatrix0FilePath );
    QStringList subMatrix0Subjects = processing.GetSubjectsFromData( subMatrix0Data, 0 );
    QStringList expectedSubMatrix0Subjects = QStringList() << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed"
                                                           << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed"
                                                           << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed"
                                                           << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed"
                                                           << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed"
                                                           << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed"
                                                           << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_COMP_0";
    expectedSubMatrix0Subjects.sort();

    // SubMatrix file: subjects not on 1st column
    QList< QStringList > subMatrix3Data = processing.GetDataFromFile( subMatrix3FilePath );
    QStringList subMatrix3Subjects = processing.GetSubjectsFromData( subMatrix3Data, 3 );
    QStringList expectedSubMatrix3Subjects = QStringList() << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed"
                                                           << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed"
                                                           << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed"
                                                           << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed"
                                                           << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed"
                                                           << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed"
                                                           << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_COMP_3";
    expectedSubMatrix3Subjects.sort();


    bool testADSubjects = adSubjects == expectedADSubjects;
    bool testSubMatrix0Subjects = subMatrix0Subjects == expectedSubMatrix0Subjects;
    bool testSubMatrix3Subjects = subMatrix3Subjects == expectedSubMatrix3Subjects;


    bool testGetSubjectsFromData_Passed = testADSubjects && testSubMatrix0Subjects && testSubMatrix3Subjects;
    if( !testGetSubjectsFromData_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetSubjectsFromData() FAILED /!\\/!\\";
        //        std::cerr << "\t+ pb with GetSubjectsFromData( QList< QStringList > data, int subjectColumnID )" << std::endl;
        //        if( !testADSubjects )
        //        {
        //            std::cerr << "\t+ Incorrect subjects generated from AD, RD, MD or FA file" << std::endl;
        //            DisplayError_GetSubjects( expectedADSubjects, adSubjects );
        //        }
        //        if( !testSubMatrix0Subjects )
        //        {
        //            std::cerr << "\t+ Incorrect subjects generated from subMatrix file when subjects on 1st column" << std::endl;
        //            DisplayError_GetSubjects( expectedSubMatrix0Subjects, subMatrix0Subjects );
        //        }
        //        if( !testSubMatrix3Subjects )
        //        {
        //            std::cerr << "\t+ Incorrect subjects generated from subMatrix file when subjects not on 1st column" << std::endl;
        //            DisplayError_GetSubjects( expectedSubMatrix3Subjects, subMatrix3Subjects );
        //        }
    }
    else
    {
        std::cerr << "Test_GetSubjectsFromData() PASSED";
    }

    return testGetSubjectsFromData_Passed;
}


bool TestProcessing::Test_GetCovariatesFromFileData( QString subMatrix0FilePath, QString subMatrix3FilePath )
{
    Processing processing;

    QMap< int, QString > covariatesExpected0;
    covariatesExpected0.insert( 1, "COMP" );
    covariatesExpected0.insert( 2, "Gender" );
    covariatesExpected0.insert( 3, "GestAgeBirth" );
    QMap< int, QString > covariatesExpected3;
    covariatesExpected3.insert( 0, "COMP" );
    covariatesExpected3.insert( 1, "Gender" );
    covariatesExpected3.insert( 2, "GestAgeBirth" );
    QMap< int, QString > covariates0 = processing.GetCovariatesFromData( processing.GetDataFromFile( subMatrix0FilePath ), 0 );
    QMap< int, QString > covariates3 = processing.GetCovariatesFromData( processing.GetDataFromFile( subMatrix3FilePath ), 3 );


    bool resultTest1 = covariates0 == covariatesExpected0;
    bool resultTest2 = covariates3 == covariatesExpected3;


    bool testGetCovariatesFromFileData_Passed = resultTest1 && resultTest2;
    if( !testGetCovariatesFromFileData_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetCovariatesFromFileData() FAILED /!\\/!\\";
        //        std::cerr << "\t+ pb with GetCovariatesFromData( QList< QStringList > data, int subjectColumnID )" << std::endl;
        //        if( !resultTest1 )
        //        {
        //            std::cerr << "\t+ Covariates retrieved when subjects are in the 1st row are not the ones expected" <<std::endl;
        //            DisplayError_GetCovariates( covariatesExpected0, covariates0 );
        //        }
        //        if( !resultTest2 )
        //        {
        //            std::cerr << "\t+ Covariates retrieved when subjects are not in the 1st row are not the ones expected" <<std::endl;
        //            DisplayError_GetCovariates( covariatesExpected3, covariates3 );
        //        }
    }
    else
    {
        std::cerr << "Test_GetCovariatesFromFileData() PASSED";
    }

    return testGetCovariatesFromFileData_Passed;
}


bool TestProcessing::Test_GetAllSubjects( QString adFilePath, QString subMatrix0FilePath )
{
    Processing processing;
    QMap< int, QStringList > subjectsMap;
    subjectsMap.insert( -1, QStringList() << "Stan" << "Kyle" << "Kenny" << "Cartman" );
    subjectsMap.insert( 0, processing.GetSubjectsFromData( processing.GetDataFromFile( adFilePath ), 4 ) );
    subjectsMap.insert( 1, QStringList() );
    subjectsMap.insert( 2, QStringList() );
    subjectsMap.insert( 3, QStringList() );
    subjectsMap.insert( 4, processing.GetSubjectsFromData( processing.GetDataFromFile( subMatrix0FilePath ), 0 ) );
    QStringList extractedSubjectList = processing.GetAllSubjects( subjectsMap ) ;
    QStringList expectedSubjects = QStringList() << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed"
                                                 << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed"
                                                 << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed"
                                                 << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed"
                                                 << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed"
                                                 << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed"
                                                 << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_ad" << "randomSubject_COMP_0"
                                                 << "Stan" << "Kyle" << "Kenny" << "Cartman";
    expectedSubjects.removeDuplicates();
    expectedSubjects.sort();


    bool testGetAllSubjects_Passed = extractedSubjectList == expectedSubjects;
    if( !testGetAllSubjects_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetAllSubjects() FAILED /!\\/!\\";
        //        std::cerr << "\t+ pb with GetAllSubjects( QMap< int, QStringList > subjectsMap )" << std::endl;
        //        std::cerr << "\t+ Wrong list of subjects extracted:" << std::endl;
        //        DisplayError_GetSubjects( expectedSubjects, extractedSubjectList );
    }
    else
    {
        std::cerr << "Test_GetAllSubjects() PASSED";
    }

    return testGetAllSubjects_Passed;
}

bool TestProcessing::Test_GetSubjectsFromSelectedFiles()
{
    Processing processing;
    QMap< int, bool > diffusionPropertiesCheckState;
    diffusionPropertiesCheckState.insert( -1, true );
    diffusionPropertiesCheckState.insert( 0, false );
    diffusionPropertiesCheckState.insert( 1, false );
    diffusionPropertiesCheckState.insert( 2, false );
    diffusionPropertiesCheckState.insert( 3, false );
    diffusionPropertiesCheckState.insert( 4, true );
    QStringList loadedSubjects = QStringList() << "Stan" << "Kyle" << "Kenny" << "Cartman";
    loadedSubjects.removeDuplicates();
    loadedSubjects.sort();
    QStringList adSubjects = QStringList() << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed"
                                           << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed"
                                           << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed"
                                           << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed"
                                           << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed"
                                           << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed"
                                           << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_ad";
    adSubjects.removeDuplicates();
    adSubjects.sort();
    QStringList subMatrixSubjects = QStringList() << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed"
                                                  << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed"
                                                  << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed"
                                                  << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed"
                                                  << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed"
                                                  << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed"
                                                  << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_COMP_3";
    subMatrixSubjects.removeDuplicates();
    subMatrixSubjects.sort();
    QMap< int, QStringList > allSubjects;
    allSubjects.insert( -1, loadedSubjects );
    allSubjects.insert( 0, adSubjects );
    allSubjects.insert( 1, QStringList() );
    allSubjects.insert( 2, QStringList() );
    allSubjects.insert( 3, QStringList() );
    allSubjects.insert( 4, subMatrixSubjects );

    QMap< int, QStringList > selectedSubjects = processing.GetSubjectsFromSelectedFiles( diffusionPropertiesCheckState, allSubjects );
    QMap< int, QStringList > expectedSubjects;
    expectedSubjects.insert( -1, loadedSubjects );
    expectedSubjects.insert( 4, subMatrixSubjects );


    bool testGetSubjectsFromSelectedFiles_Passed = selectedSubjects == expectedSubjects;
    if( !testGetSubjectsFromSelectedFiles_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetSubjectsFromSelectedFiles() FAILED /!\\/!\\";
        //        std::cerr << "\t+ pb with GetSubjectsFromSelectedFiles( const QMap< int, bool > diffusionPropertiesCheckState,"
        //                     " const QMap< int, QStringList > subjectsMap )" << std::endl;
        //        std::cerr << "\t+ Incorrect subject list from selected input files" << std::endl;
        //        std::cerr << "\t  Diffusion property (index: ad <-> 0, rd <-> 1, md <-> 2, fa <-> 3, subMatrix <-> 4)" << std::endl;
        //        DisplayError_GetSubjectsFromSelectedFiles( expectedSubjects, selectedSubjects );
    }
    else
    {
        std::cerr << "Test_GetSubjectsFromSelectedFiles() PASSED";
    }

    return testGetSubjectsFromSelectedFiles_Passed;
}

bool TestProcessing::Test_SortSubjects()
{
    Processing processing;
    QStringList loadedSubjects = QStringList() << "Stan" << "Kyle" << "Kenny" << "Cartman";
    loadedSubjects.sort();
    QStringList subjectsInAll = QStringList() << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed"
                                              << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed"
                                              << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed"
                                              << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << loadedSubjects;
    subjectsInAll.sort();
    QStringList allSubjects = QStringList() << subjectsInAll << loadedSubjects << "randomSubject_ad" << "randomSubject_COMP_3";
    allSubjects.sort();
    QStringList adSubjects = QStringList() << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed"
                                           << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed"
                                           << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed"
                                           << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_ad";
    adSubjects.sort();
    QStringList subMatrixSubjects = QStringList() << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed"
                                                  << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed"
                                                  << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed"
                                                  << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_COMP_3";
    subMatrixSubjects.sort();
    QMap< int, QStringList > selectedSubjects;
    selectedSubjects.insert( -1, loadedSubjects );
    selectedSubjects.insert( 0, adSubjects );
    selectedSubjects.insert( 4, subMatrixSubjects );
    QMap< QString, QMap< int, bool > > expectedSortedSubjects;
    foreach( QString subj, allSubjects )
    {
        ( expectedSortedSubjects[subj] )[-1] = loadedSubjects.contains( subj ) ? true : false;
        ( expectedSortedSubjects[subj] )[0] = adSubjects.contains( subj ) ? true : false;
        ( expectedSortedSubjects[subj] )[4] = subMatrixSubjects.contains( subj ) ? true : false;
    }
    QMap< QString, QMap< int, bool > > sortedSubjects = processing.SortSubjects( allSubjects, selectedSubjects );


    bool testSortSubjects_Passed = sortedSubjects == expectedSortedSubjects;
    if( !testSortSubjects_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SortSubjects() FAILED /!\\/!\\";
        //        std::cerr << "\t+ pb with SortSubjects( const QStringList subjects, const QMap< int, QStringList > subjectsMap )" << std::endl;
        //        std::cerr << "\t+ Incorrect sorted subject list generated" << std::endl;
        //        std::cerr << "\t  Diffusion property (index: ad <-> 0, rd <-> 1, md <-> 2, fa <-> 3, subMatrix <-> 4)" << std::endl;
        //        DisplayError_SortedSubjects( expectedSortedSubjects, sortedSubjects );
    }
    else
    {
        std::cerr << "Test_SortSubjects() PASSED";
    }

    return testSortSubjects_Passed;
}

bool TestProcessing::Test_AssignSortedSubject()
{
    Processing processing;
    QStringList loadedSubjects = QStringList() << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed"
                                               << "Stan" << "Kyle" << "Kenny" << "Cartman";
    loadedSubjects.removeDuplicates();
    loadedSubjects.sort();
    QStringList allSubjects = QStringList() << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed"
                                            << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed"
                                            << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed"
                                            << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_ad" << "randomSubject_COMP_3" << loadedSubjects;
    allSubjects.removeDuplicates();
    allSubjects.sort();
    QStringList adSubjects = QStringList() << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed"
                                           << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed"
                                           << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed"
                                           << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_ad";
    adSubjects.removeDuplicates();
    adSubjects.sort();
    QStringList subMatrixSubjects = QStringList() << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed"
                                                  << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed" << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed"
                                                  << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed"
                                                  << "neo-0087-1_dwi_35_all_QCed_VC_DTI_embed" << "randomSubject_COMP_3";
    subMatrixSubjects.removeDuplicates();
    subMatrixSubjects.sort();
    QMap< int, QStringList > selectedSubjects;
    selectedSubjects.insert( -1, loadedSubjects );
    selectedSubjects.insert( 0, adSubjects );
    selectedSubjects.insert( 4, subMatrixSubjects );
    QMap< QString, QMap< int, bool > > sortedSubjects = processing.SortSubjects( allSubjects, selectedSubjects );
    QMap< QString, QList< int > > expectedUnMatchedSubjects;
    QStringList expectedMatchedSubjects;
    QStringList matchedSubjects;
    QMap< QString, QList< int > > unMatchedSubjects;


    foreach( QString subject, allSubjects )
    {
        if( !loadedSubjects.contains( subject ) || !adSubjects.contains( subject ) || !subMatrixSubjects.contains( subject ) )
        {
            QList< int > indices;
            if( loadedSubjects.contains( subject ) )
            {
                indices.append( -1 );
            }
            if( adSubjects.contains( subject ) )
            {
                indices.append( 0 );
            }
            if( subMatrixSubjects.contains( subject ) )
            {
                indices.append( 4 );
            }
            expectedUnMatchedSubjects.insert( subject, indices );
        }
        if( loadedSubjects.contains( subject ) && adSubjects.contains( subject ) && subMatrixSubjects.contains( subject ) )
        {
            expectedMatchedSubjects.append( subject );
        }
    }

    processing.AssignSortedSubject( sortedSubjects, matchedSubjects, unMatchedSubjects );


    bool testMatchedSubjects = matchedSubjects == expectedMatchedSubjects;
    bool testUnMatchedSubjects = unMatchedSubjects == expectedUnMatchedSubjects;


    bool testAssignSortedSubject_Passed = testMatchedSubjects && testUnMatchedSubjects;
    if( !testAssignSortedSubject_Passed )
    {
        std::cerr << "/!\\/!\\ Test_AssignSortedSubject() FAILED /!\\/!\\";
        //        std::cerr << "\t+ pb with AssignSortedSubject( const QMap< QString, QMap< int, bool > > sortedSubjects,"
        //                     " QStringList& matchedSubjects, QMap< QString, QList< int > >& unMatchedSubjects )" << std::endl;
        //        if( !testMatchedSubjects )
        //        {
        //            std::cerr << "\t+ Matched subjects not correctly assigned" << std::endl;
        //            DisplayError_GetSubjects( expectedMatchedSubjects, matchedSubjects );
        //        }
        //        if( !testUnMatchedSubjects )
        //        {
        //            std::cerr << "\t+ UnMatched subjects not correctly assigned" << std::endl;
        //            std::cerr << "\t  Diffusion property (index: ad <-> 0, rd <-> 1, md <-> 2, fa <-> 3, subMatrix <-> 4)" << std::endl;
        //            DisplayError_UnMatchedSubjects( expectedUnMatchedSubjects, unMatchedSubjects );
        //        }
    }
    else
    {
        std::cerr << "Test_AssignSortedSubject() PASSED";
    }

    return testAssignSortedSubject_Passed;
}


bool TestProcessing::Test_Transpose()
{
    Processing processing;
    QList< QStringList > data = QList< QStringList >() << ( QStringList() << "a" << "b" << "c" )
                                                       << ( QStringList() << "d" << "e" << "f" );
    QList< QStringList > expectedTransposeData = QList< QStringList >() << ( QStringList() << "a" << "d" )
                                                                        << ( QStringList() << "b" << "e" )
                                                                        << ( QStringList() << "c" << "f" );


    bool testTranspose_Passed = processing.Transpose( data ) == expectedTransposeData;
    if( !testTranspose_Passed )
    {
        std::cerr << "/!\\/!\\ Test_Transpose() FAILED /!\\/!\\";
        //        std::cerr << "\t+ pb with Transpose( const QList< QStringList >& rawData )" << std::endl;
    }
    else
    {
        std::cerr << "Test_Transpose() PASSED";
    }

    return testTranspose_Passed;
}

bool TestProcessing::Test_RemoveUnmatchedSubjects()
{
    Processing processing;
    QList< QStringList > data = QList< QStringList >() << ( QStringList() << "a" << "b" << "c" )
                                                       << ( QStringList() << "d" << "e" << "f" );
    QList< QStringList > transposeData = processing.Transpose( data );
    QList< QStringList > expectedData = QList< QStringList >() << ( QStringList() << "a" << "d" )
                                                               << ( QStringList() << "c" << "f" );
    QStringList matchedSubjects = QStringList() << "a" << "c";
    QStringList subjects = QStringList() << "a" << "b" << "c";


    bool testSubjects = processing.RemoveUnmatchedSubjects( transposeData, subjects, matchedSubjects );


    bool testRemoveUnmatchedSubjects_Passed = transposeData == expectedData && testSubjects;
    if( !testRemoveUnmatchedSubjects_Passed )
    {
        std::cerr << "/!\\/!\\ Test_RemoveUnmatchedSubjects() FAILED /!\\/!\\";
        //        std::cerr << "\t+ pb with RemoveUnmatchedSubjects( QList< QStringList >& rawDataTransposed, QStringList subjects )" << std::endl;
    }
    else
    {
        std::cerr << "Test_RemoveUnmatchedSubjects() PASSED";
    }

    return testRemoveUnmatchedSubjects_Passed;
}

bool TestProcessing::Test_QStringListToDouble()
{
    Processing processing;
    QStringList stringList = QStringList() << "1.11" << "2.22" << "3.33" << "4.44" << "5.55";
    QList< double > doubleList = QList< double >() << 1.11 << 2.22 << 3.33 << 4.44 << 5.55;


    bool testQStringListToDouble_Passed = doubleList == processing.QStringListToDouble( stringList );
    if( !testQStringListToDouble_Passed )
    {
        std::cerr << "/!\\/!\\ Test_QStringListToDouble() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with QStringListToDouble( QStringList rowData )" << std::endl;
        //        std::cerr << "\t  convertion from QStringList to QList < double > failed" << std::endl;
    }
    else
    {
        std::cerr << "Test_QStringListToDouble() PASSED";
    }

    return testQStringListToDouble_Passed;
}

bool TestProcessing::Test_DataToDouble()
{
    Processing processing;
    QStringList stringList1 = QStringList() << "1.11" << "2.22" << "3.33" << "4.44" << "5.55";
    QStringList stringList2 = QStringList() << "6.66" << "7.77" << "8.88" << "9.99" << "10.1010";
    QList< QStringList > stringData = QList< QStringList >() << stringList1 << stringList2;
    QList< double > doubleList1 = QList< double >() << 1.11 << 2.22 << 3.33 << 4.44 << 5.55;
    QList< double > doubleList2 = QList< double >() << 6.66 << 7.77 << 8.88 << 9.99 << 10.1010;
    QList< QList< double > > doubleData = QList< QList< double > >() << doubleList1 << doubleList2;


    bool testDataToDouble_Passed = doubleData == processing.DataToDouble( stringData );
    if( !testDataToDouble_Passed )
    {
        std::cerr << "/!\\/!\\ Test_DataToDouble() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with DataToDouble( QList < QStringList > data )" << std::endl;
        //        std::cerr << "\t  convertion from QList < QStringList > to QList < QList < double > > failed" << std::endl;
    }
    else
    {
        std::cerr << "Test_DataToDouble() PASSED";
    }

    return testDataToDouble_Passed;
}

bool TestProcessing::Test_GetMean()
{
    Processing processing;
    QList< QStringList > data = QList< QStringList >() << ( QStringList() << "a" << "b" << "c" << "e" )
                                                       << ( QStringList() << "1" << "2" << "3" << "1" )
                                                       << ( QStringList() << "4" << "5" << "6" << "1" )
                                                       << ( QStringList() << "7" << "8" << "9" << "7" )
                                                       << ( QStringList() << "0" << "1" << "2" << "6" )
                                                       << ( QStringList() << "0" << "0" << "0" << "0" );
    QList< QStringList > transposeData = processing.Transpose( data );
    QList< double > expectedMean = QList< double >() << QList< double >() << 2 << 4 << 8 << 3 << 0;
    QStringList matchedSubjects = QStringList()  << "b" << "c" << "e";
    QStringList subjects = QStringList()  << "a" << "b" << "c" << "e";

    processing.RemoveUnmatchedSubjects( transposeData, subjects, matchedSubjects );

    QList< double > tete = processing.GetMean( processing.DataToDouble( transposeData ), 1 );

    bool testGetMean_Passed = tete == expectedMean;
    if( !testGetMean_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetMean() FAILED /!\\/!\\";
        //        std::cerr << "\t+ pb with GetMean( const QList< QList< double > >& rawDataDouble )" << std::endl;
    }
    else
    {
        std::cerr << "Test_GetMean() PASSED";
    }

    return testGetMean_Passed;
}

bool TestProcessing::Test_ApplyPearsonCorrelation()
{
    Processing processing;
    QList< QStringList > data = QList< QStringList >() << ( QStringList() << "a" << "b" << "c" << "d" << "e" << "f" )
                                                       << ( QStringList() << "1" << "2" << "3" << "4" << "5" << "6" )
                                                       << ( QStringList() << "-7" << "-8" << "-9" << "-10" << "-11" << "-12" )
                                                       << ( QStringList() << "-13" << "-14" << "-15" << "-16" << "-17" << "-18" )
                                                       << ( QStringList() << "-19" << "-20" << "-21" << "-22" << "-23" << "24" )
                                                       << ( QStringList() << "0" << "0" << "0" << "0" << "0" << "0" );
    QList< QStringList > transposeData = processing.Transpose( data );
    QList< QList< double > > dataDouble;
    QStringList subjects = QStringList()  << "a" << "b" << "c" << "d" << "e" << "f";
    QStringList matchedSubjects = QStringList()  << "b" << "c" << "e" << "f";
    QList< double > correlation = QList< double >() << 0.844389 << 0.862809 << 0.888108 << 0.357414;
    QList< double > expectedCorrelation;
    QList< double > mean;
    bool testCorrelation = true;


    processing.RemoveUnmatchedSubjects( transposeData, subjects, matchedSubjects );
    dataDouble = processing.DataToDouble( transposeData );
    mean = processing.GetMean( dataDouble, 1 );

    for( int i = 0; i < dataDouble.size(); i++ )
    {
        expectedCorrelation.append( processing.ApplyPearsonCorrelation( dataDouble.at( i ), mean, 1 ) );
    }
    for( int i = 0; i < correlation.size(); i++ )
    {
        testCorrelation = testCorrelation && ( ( correlation.at( i ) - expectedCorrelation.at( i ) ) < 0.0001 );
    }


    bool testApplyPearsonCorrelation_Passed = testCorrelation;
    if( !testApplyPearsonCorrelation_Passed )
    {
        std::cerr << "/!\\/!\\ Test_ApplyPearsonCorrelation() FAILED /!\\/!\\";
        //        std::cerr << "\t+ pb with ApplyPearsonCorrelation( int indexLine, const QList< QList< double > >& rawDataDouble, const QList< double >& mean )" << std::endl;
    }
    else
    {
        std::cerr << "Test_ApplyPearsonCorrelation() PASSED";
    }

    return testApplyPearsonCorrelation_Passed;
}



bool TestProcessing::Test_GenerateMatlabInputFiles( QString adFilePath, QString subMatrix0FilePath, QString subMatrix3FilePath, QString adMatlabFilePath,
                                                    QString subMatrix0MatlabFilePath, QString subjectsFilePath, QString outputDir )
{
    Processing processing;
    QString dirTest = outputDir + "/TestProcessing/Test_GenerateMatlabInputFiles";
    QDir().mkpath( dirTest );
    bool adFilesMatched = true;
    bool subMatrix0FileFilesMatched = true;
    bool subMatrix3FileFilesMatched = true;
    int i = 0;
    QStringList subjects = processing.GetSubjectsFromFileList( subjectsFilePath );
    QMap< int, QString > expectedOutput;
    expectedOutput.insert( 0, adMatlabFilePath );
    expectedOutput.insert( 4, subMatrix0MatlabFilePath );
    QMap< int, QString >::ConstIterator iterExpectedOutputTest1 = expectedOutput.cbegin();
    QMap< int, QString >::ConstIterator iterExpectedOutputTest2 = expectedOutput.cbegin();

    /************************************************/
    /******************** Test 1 ********************/
    /************************************************/
    // SubMatrix file: subjects on 1st column
    QString fiberNameTest1 = "GenerateMatlabInputsTest1";
    QMap< int, QString > filesTest1;
    filesTest1.insert( 0, adFilePath );
    filesTest1.insert( 4, subMatrix0FilePath );
    QMap< int, QString > propertiesTest1;
    propertiesTest1.insert( 0, "ad" );
    propertiesTest1.insert( 4, "subMatrix" );
    QMap< int, QString > covariatesTest1;
    covariatesTest1.insert( -1, "Intercept" );
    covariatesTest1.insert( 1, "Gender" );
    covariatesTest1.insert( 2, "DaysSinceBirth" );
    covariatesTest1.insert( 3, "Scanner" );
    int covariatesColumnIdTest1 = 0;

    QMap< int, QString > matlabInputFilesTest1 = processing.GenerateMatlabInputs( dirTest, fiberNameTest1, filesTest1, propertiesTest1,
                                                                                  covariatesTest1, covariatesColumnIdTest1, subjects, -1, -1 );

    /************************************************/
    /******************** Test 2 ********************/
    /************************************************/
    // SubMatrix file: subjects not on 1st column
    QString fiberNameTest2 = "GenerateMatlabInputsTest2";
    QMap< int, QString > filesTest2;
    filesTest2.insert( 0, adFilePath );
    filesTest2.insert( 4, subMatrix3FilePath );
    QMap< int, QString > propertiesTest2;
    propertiesTest2.insert( 0, "ad" );
    propertiesTest2.insert( 4, "subMatrix" );
    QMap< int, QString > covariatesTest2;
    covariatesTest2.insert( -1, "Intercept" );
    covariatesTest2.insert( 0, "subMatrix" );
    covariatesTest2.insert( 1, "Gender" );
    covariatesTest2.insert( 2, "GestAgeBirth" );
    int covariatesColumnIdTest2 = 3;

    QMap< int, QString > matlabInputFilesTest2 = processing.GenerateMatlabInputs( dirTest, fiberNameTest2, filesTest2, propertiesTest2,
                                                                                  covariatesTest2, covariatesColumnIdTest2, subjects, -1, -1 );


    QMap< int, QString >::ConstIterator iterMatlabInputTest1 = matlabInputFilesTest1.cbegin();
    while( iterMatlabInputTest1 != matlabInputFilesTest1.cend() )
    {
        if( !CompareFile( iterExpectedOutputTest1.value(), iterMatlabInputTest1.value() ) ||
                ( iterExpectedOutputTest1.key() != iterMatlabInputTest1.key() ) )
        {
            adFilesMatched = false;
        }
        if( !CompareFile( iterExpectedOutputTest1.value(), iterMatlabInputTest1.value() ) && ( i%2 != 0 ) )
        {
            subMatrix0FileFilesMatched = false;
        }
        ++iterExpectedOutputTest1;
        ++iterMatlabInputTest1;
        i++;
    }

    QMap< int, QString >::ConstIterator iterMatlabInputTest2 = matlabInputFilesTest2.cbegin();
    while( iterMatlabInputTest2 != matlabInputFilesTest2.cend() )
    {
        if( !CompareFile( iterExpectedOutputTest2.value(), iterMatlabInputTest2.value() ) ||
                ( iterExpectedOutputTest2.key() != iterMatlabInputTest2.key() ) )
        {
            subMatrix3FileFilesMatched = false;
        }
        ++iterExpectedOutputTest2;
        ++iterMatlabInputTest2;
        i++;
    }


    bool testGenerateMatlabInputs_Passed = adFilesMatched && subMatrix0FileFilesMatched && subMatrix3FileFilesMatched;
    if( !testGenerateMatlabInputs_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GenerateMatlabInputs() FAILED /!\\/!\\";
        //        std::cerr << "\t+ pb with GenerateMatlabInputs( QString outputDir, QString fiberName,"
        //                     " QMap< int, QString > inputs, QMap< int, QString > properties,"
        //                     " QMap< int, QString > covariates, int subjectColumnID, QStringList subjects);" << std::endl;
        //        if( !adFilesMatched )
        //        {
        //            std::cerr << "\t  matlab input for AD, RD, MD or FA file not generated correctly" << std::endl;
        //        }
        //        if( !subMatrix0FileFilesMatched )
        //        {
        //            std::cerr << "\t  matlab input for subMatrix file not generated correctly when subjects on 1st column" << std::endl;
        //        }
        //        if( !subMatrix3FileFilesMatched )
        //        {
        //            std::cerr << "\t  batlab input for subMatrix file not generated correctly when subjects not on 1st column" << std::endl;
        //        }
    }
    else
    {
        std::cerr << "Test_GenerateMatlabInputs() PASSED";
    }

    return testGenerateMatlabInputs_Passed;
}



/**********************************************************************/
/********************** Functions Used For Testing ********************/
/**********************************************************************/
void TestProcessing::DisplayError_GetSubjects( QStringList subjectsExpected, QStringList subjectsDisplayed )
{
    std::cerr << "\t  - subjects expected" << std::endl;
    for( int i = 0; i < subjectsExpected.size(); i++ )
    {
        std::cerr << "\t      " << subjectsExpected.at( i ).toStdString() << std::endl;
    }
    std::cerr << "\t  - subjects displayed" << std::endl;
    for( int i = 0; i < subjectsDisplayed.size(); i++ )
    {
        std::cerr << "\t      " << subjectsDisplayed.at( i ).toStdString() << std::endl;
    }
}

void TestProcessing::DisplayError_GetSubjectsFromSelectedFiles( QMap< int, QStringList > expectedSubjects, QMap< int, QStringList > selectedSubjects )
{
    QMap< int, QStringList >::ConstIterator iterExpectedSubjects = expectedSubjects.cbegin();
    std::cerr << "\t  - subjects expected" << std::endl;
    while( iterExpectedSubjects != expectedSubjects.cend() )
    {
        std::cerr << "\t      index: " << iterExpectedSubjects.key() << std::endl;
        foreach( QString subject, iterExpectedSubjects.value() )
        {
            std::cerr << "\t\t" << subject.toStdString() << std::endl;
        }
        ++iterExpectedSubjects;
    }

    QMap< int, QStringList >::ConstIterator iterSubjects = selectedSubjects.cbegin();
    std::cerr << "\t  - subjects retrieved" << std::endl;
    while( iterSubjects != selectedSubjects.cend() )
    {
        std::cerr << "\t      index: " << iterSubjects.key() << std::endl;
        foreach( QString subject, iterSubjects.value() )
        {
            std::cerr << "\t\t" << subject.toStdString() << std::endl;
        }
        ++iterSubjects;
    }
}

void TestProcessing::DisplayError_SortedSubjects( QMap< QString, QMap< int, bool > > expectedSortedSubjects, QMap< QString, QMap< int, bool > > sortedSubjects )
{
    QMap< QString, QMap< int, bool > >::ConstIterator iterExpectedSortedSubjects = expectedSortedSubjects.cbegin();
    std::cerr << "\t  - sorted subjects expected" << std::endl;
    while( iterExpectedSortedSubjects != expectedSortedSubjects.cend() )
    {
        std::cerr << "\t      " << iterExpectedSortedSubjects.key().toStdString() << " --> ";
        QMap< int, bool >::ConstIterator iterExpectedFile = iterExpectedSortedSubjects.value().cbegin();
        while( iterExpectedFile != iterExpectedSortedSubjects.value().cend() )
        {
            std::cerr << " index: " << iterExpectedFile.key() << " [" << ( iterExpectedFile.value() == 1 ? "True" : "False" ) << "]";
            ++iterExpectedFile;
        }
        std::cerr << std::endl;
        ++iterExpectedSortedSubjects;
    }

    QMap< QString, QMap< int, bool > >::ConstIterator iterSortedSubjects = sortedSubjects.cbegin();
    std::cerr << "\t  - sorted subjects displayed" << std::endl;
    while( iterSortedSubjects != sortedSubjects.cend() )
    {
        std::cerr << "\t      " << iterSortedSubjects.key().toStdString() << " --> ";
        QMap< int, bool >::ConstIterator iterFile = iterSortedSubjects.value().cbegin();
        while( iterFile != iterSortedSubjects.value().cend() )
        {
            std::cerr << " index: " << iterFile.key() << " [" << ( iterFile.value() == 1 ? "True" : "False" ) << "]";
            ++iterFile;
        }
        std::cerr << std::endl;
        ++iterSortedSubjects;
    }
}

void TestProcessing::DisplayError_UnMatchedSubjects( QMap< QString, QList< int > > unMatchedSubjectsExpected, QMap< QString, QList< int > > unMatchedSubjectsDisplayed )
{
    QMap< QString, QList< int > >::ConstIterator iterExpectedUnMatched = unMatchedSubjectsExpected.cbegin();
    std::cerr << "\t  - unMatched subjects expected" << std::endl;
    while( iterExpectedUnMatched != unMatchedSubjectsExpected.cend() )
    {
        std::cerr << "\t      " << iterExpectedUnMatched.key().toStdString() << " --> ";
        foreach( int index, iterExpectedUnMatched.value() )
        {
            std::cerr << index << " ";
        }
        std::cerr << std::endl;
        ++iterExpectedUnMatched;
    }

    QMap< QString, QList< int > >::ConstIterator iterUnMatched = unMatchedSubjectsDisplayed.cbegin();
    std::cerr << "\t  - unMatched subjects displayed" << std::endl;
    while( iterUnMatched != unMatchedSubjectsDisplayed.cend() )
    {
        std::cerr << "\t      " << iterUnMatched.key().toStdString() << " --> ";
        foreach( int index, iterUnMatched.value() )
        {
            std::cerr << index << " ";
        }
        std::cerr << std::endl;
        ++iterUnMatched;
    }
}

void TestProcessing::DisplayError_GetCovariates( QMap< int, QString > covariatesExpected, QMap< int, QString > covariatesDisplayed )
{
    std::cerr << "\t  - covariates expected" << std::endl;
    QMap< int, QString >::ConstIterator iterCovariatesExpected = covariatesExpected.cbegin();
    while( iterCovariatesExpected != covariatesExpected.cend() )
    {
        std::cerr << "\t      " << iterCovariatesExpected.value().toStdString() << " on column " << iterCovariatesExpected.key() << std::endl;
        ++iterCovariatesExpected;
    }
    std::cerr << "\t  - covariates displayed" << std::endl;
    QMap< int, QString >::ConstIterator iterCovariatesDisplayed = covariatesDisplayed.cbegin();
    while( iterCovariatesDisplayed != covariatesDisplayed.cend() )
    {
        std::cerr << "\t      " << iterCovariatesDisplayed.value().toStdString() << " on column " << iterCovariatesDisplayed.key() << std::endl;
        ++iterCovariatesDisplayed;
    }
}


QByteArray TestProcessing::GetHashFile( QString filePath )
{
    QCryptographicHash hash( QCryptographicHash::Sha1 );
    QFile file( filePath );

    if( file.open( QIODevice::ReadOnly ) )
    {
        hash.addData( file.readAll() );
        file.close();
    }
    else
    {
        std::cerr << "Cannot open file: " << filePath.toStdString() << std::endl;
    }

    return hash.result().toHex();
}

bool TestProcessing::CompareFile( QString filePath1, QString filePath2 )
{
    QByteArray sig1 = GetHashFile( filePath1 );
    QByteArray sig2 = GetHashFile( filePath2 );

    if( sig1 == sig2 )
    {
        return true;
    }
    else
    {
        return false;
    }
}
