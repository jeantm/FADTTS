#include "TestFADTTSWindow.h"

TestFADTTSWindow::TestFADTTSWindow()
{
}


///**********************************************************************/
///*************************** Test Functions ***************************/
///**********************************************************************/
//bool TestFADTTSWindow::Test_UpdateCurrentDir()
//{
//    QSharedPointer<FADTTSWindow> fadttsWindow = QSharedPointer<FADTTSWindow>( new FADTTSWindow );

//    QString currentFileInputDir = fadttsWindow->m_currentInputFileDir;
//    QString currentSubjectListInputDir = fadttsWindow->m_currentSubjectFileDir;

//    QString filePathOK1 = "/home/file.txt";
//    QString filePathOK2 = "/file.txt";
//    QString filePathKO1 = "this/path/doesnt/exist/file.txt";
//    QString filePathKO2 = "/home/folder/file.txt";

//    fadttsWindow->UpdateCurrentDir( filePathOK1, currentFileInputDir );
//    fadttsWindow->UpdateCurrentDir( filePathOK1, currentSubjectListInputDir );

//    bool testUpdateCurrentDirOK1 = ( currentFileInputDir == "/home" ) && ( currentSubjectListInputDir == "/home" );


//    fadttsWindow->UpdateCurrentDir( filePathKO1, currentFileInputDir );
//    fadttsWindow->UpdateCurrentDir( filePathKO1, currentSubjectListInputDir );

//    bool testUpdateCurrentDirKO1 = ( currentFileInputDir == "/home" ) && ( currentSubjectListInputDir == "/home" );


//    fadttsWindow->UpdateCurrentDir( filePathOK2, currentFileInputDir );
//    fadttsWindow->UpdateCurrentDir( filePathOK2, currentSubjectListInputDir );

//    bool testUpdateCurrentDirOK2 = ( currentFileInputDir == "/" ) && ( currentSubjectListInputDir == "/" );


//    fadttsWindow->UpdateCurrentDir( filePathKO2, currentFileInputDir );
//    fadttsWindow->UpdateCurrentDir( filePathKO2, currentSubjectListInputDir );

//    bool testUpdateCurrentDirKO2 = ( currentFileInputDir == "/" ) && ( currentSubjectListInputDir == "/" );


//    if( !testUpdateCurrentDirOK1 || !testUpdateCurrentDirKO1 || !testUpdateCurrentDirOK2 || !testUpdateCurrentDirKO2 )
//    {
//        std::cerr << std::endl << "Test_UpdateCurrentDir() FAILED:" << std::endl;
//        if( !testUpdateCurrentDirOK1 )
//        {
//            std::cerr << "\tm_currentInputFileDir and m_currentSubjectListInputDir not correctly updated when they should have. filePath = \"/home/file.txt\"" << std::endl;
//        }
//        if( !testUpdateCurrentDirKO1 )
//        {
//            std::cerr << "\tm_currentInputFileDir and m_currentSubjectListInputDir updated when they should not have. filePath = \"this/path/doesnt/exist/file.txt\"" << std::endl;
//        }
//        if( !testUpdateCurrentDirOK2 )
//        {
//            std::cerr << "\tm_currentInputFileDir and m_currentSubjectListInputDir not correctly updated when they should have. filePath = \"/file.txt\"" << std::endl;
//        }
//        if( !testUpdateCurrentDirKO2 )
//        {
//            std::cerr << "\tm_currentInputFileDir and m_currentSubjectListInputDir updated when they should not have. filePath = \"/home/folder/file.txt\"" << std::endl;
//        }
//    }
//    else
//    {
//        std::cout << std::endl << "Test_UpdateCurrentDir() PASSED" << std::endl;
//    }

//    return ( testUpdateCurrentDirOK1 & testUpdateCurrentDirKO1 & testUpdateCurrentDirOK2 & testUpdateCurrentDirKO2 );
//}

//bool TestFADTTSWindow::Test_UpdateEditInputDialogCurrentDir()
//{
//    QSharedPointer<FADTTSWindow> fadttsWindow = QSharedPointer<FADTTSWindow>( new FADTTSWindow );

//    QString filePathOK1 = "/home/file.txt";
//    QString filePathOK2 = "/file.txt";
//    QString filePathKO1 = "this/path/doesnt/exist/file.txt";
//    QString filePathKO2 = "/home/folder/file.txt";

//    fadttsWindow->UpdateEditInputDialogCurrentDir( filePathOK1 );

//    bool testUpdateCurrentDirOK1 = fadttsWindow->m_editInputDialog->m_currentInputDir == "/home";


//    fadttsWindow->UpdateEditInputDialogCurrentDir( filePathKO1 );

//    bool testUpdateCurrentDirKO1 = fadttsWindow->m_editInputDialog->m_currentInputDir == "/home";


//    fadttsWindow->UpdateEditInputDialogCurrentDir( filePathOK2 );

//    bool testUpdateCurrentDirOK2 = fadttsWindow->m_editInputDialog->m_currentInputDir == "/";


//    fadttsWindow->UpdateEditInputDialogCurrentDir( filePathKO2 );

//    bool testUpdateCurrentDirKO2 = fadttsWindow->m_editInputDialog->m_currentInputDir == "/";


//    if( !testUpdateCurrentDirOK1 || !testUpdateCurrentDirKO1 || !testUpdateCurrentDirOK2 || !testUpdateCurrentDirKO2 )
//    {
//        std::cerr << std::endl << "Test_UpdateEditInputDialogCurrentDir() FAILED:" << std::endl;
//        if( !testUpdateCurrentDirOK1 )
//        {
//            std::cerr << "\tEditInputDialog m_currentInputDir not correctly updated when it should have. filePath = \"/home/file.txt\"" << std::endl;
//        }
//        if( !testUpdateCurrentDirKO1 )
//        {
//            std::cerr << "\tEditInputDialog m_currentInputDir updated when it should not have. filePath = \"this/path/doesnt/exist/file.txt\"" << std::endl;
//        }
//        if( !testUpdateCurrentDirOK2 )
//        {
//            std::cerr << "\tEditInputDialog m_currentInputDir not correctly updated when it should have. filePath = \"/file.txt\"" << std::endl;
//        }
//        if( !testUpdateCurrentDirKO2 )
//        {
//            std::cerr << "\tEditInputDialog m_currentInputDir updated when it should not have. filePath = \"/home/folder/file.txt\"" << std::endl;
//        }
//    }
//    else
//    {
//        std::cout << std::endl << "Test_UpdateEditInputDialogCurrentDir() PASSED" << std::endl;
//    }

//    return ( testUpdateCurrentDirOK1 & testUpdateCurrentDirKO1 & testUpdateCurrentDirOK2 & testUpdateCurrentDirKO2 );
//}



//bool TestFADTTSWindow::Test_DisplayCovariates()
//{
////    QSharedPointer<FADTTSWindow> fadttsWindow = QSharedPointer<FADTTSWindow>( new FADTTSWindow );

////    bool testTextCovariatesOK1 = true;
////    bool testCheckStateCovariatesOK1 = true;
////    bool testTextCovariatesOK2 = true;
////    bool testCheckStateCovariatesOK2 = true;

////    QMap<int, QString> covariateMap1;
////    covariateMap1.insert( -1, "Intercept" );
////    covariateMap1.insert( 1, "COMP" );
////    covariateMap1.insert( 2, "Gender" );
////    covariateMap1.insert( 3, "GestAgeBirth" );
////    covariateMap1.insert( 4, "DaysSinceBirth" );
////    covariateMap1.insert( 5, "MEDUC" );
////    covariateMap1.insert( 6, "Scanner" );
////    QMap<int, QString> covariateMap2;
////    covariateMap2.insert( 7, "Directions" );
////    covariateMap2.insert( 8, "TwinStatus" );
////    covariateMap2.insert( 9, "ICV" );
////    QMap<int, QString> covariateMap3;

////    fadttsWindow->DisplayCovariates( covariateMap1 );

////    bool testNbrCovariatesOK1 = fadttsWindow->m_covariateListWidget->count() == covariateMap1.size();
////    int nbrCovarTest1 = fadttsWindow->m_covariateListWidget->count();

////    QStringList covariatesDisplayed1;
////    QList<Qt::CheckState> covariatesCheckState1;
////    if( testNbrCovariatesOK1 )
////    {
////        for( int i = 0; i < fadttsWindow->m_covariateListWidget->count(); i++ )
////        {
////            covariatesDisplayed1.append( fadttsWindow->m_covariateListWidget->item( i )->text() );
////            if( fadttsWindow->m_covariateListWidget->item( i )->text() != covariateMap1.values().at( i ) )
////            {
////                testTextCovariatesOK1 = false;
////            }
////            covariatesCheckState1.append( fadttsWindow->m_covariateListWidget->item( i )->checkState() );
////            if( fadttsWindow->m_covariateListWidget->item( i )->checkState() != Qt::Checked )
////            {
////                testCheckStateCovariatesOK1 = false;
////            }
////        }
////    }

////    fadttsWindow->DisplayCovariates( covariateMap2 );

////    bool testNbrCovariatesOK2 = fadttsWindow->m_covariateListWidget->count() == covariateMap2.size();
////    int nbrCovarTest2 = fadttsWindow->m_covariateListWidget->count();

////    QStringList covariatesDisplayed2;
////    QList<Qt::CheckState> covariatesCheckState2;
////    if( testNbrCovariatesOK2 )
////    {
////        for( int i = 0; i < fadttsWindow->m_covariateListWidget->count(); i++ )
////        {
////            covariatesDisplayed2.append( fadttsWindow->m_covariateListWidget->item( i )->text() );
////            if( fadttsWindow->m_covariateListWidget->item( i )->text() != covariateMap2.values().at( i ) )
////            {
////                testTextCovariatesOK2 = false;
////            }
////            covariatesCheckState2.append( fadttsWindow->m_covariateListWidget->item( i )->checkState() );
////            if( fadttsWindow->m_covariateListWidget->item( i )->checkState() != Qt::Checked )
////            {
////                testCheckStateCovariatesOK2 = false;
////            }
////        }
////    }

////    fadttsWindow->DisplayCovariates( covariateMap3 );

////    bool testNbrCovariatesOK3 = fadttsWindow->m_covariateListWidget->count() == covariateMap3.size();
////    int nbrCovarTest3 = fadttsWindow->m_covariateListWidget->count();


////    if( !testNbrCovariatesOK1 || !testTextCovariatesOK1 || !testCheckStateCovariatesOK1 ||
////            !testNbrCovariatesOK2 || !testTextCovariatesOK2 || !testCheckStateCovariatesOK2 ||
////            !testNbrCovariatesOK3 )
////    {
////        std::cerr << std::endl << "Test_DisplayCovariates() FAILED:" << std::endl;
////        if( !testNbrCovariatesOK1 || !testTextCovariatesOK1 || !testCheckStateCovariatesOK1 )
////        {
////            std::cerr << "\t+ Test covariateMap with 7 covariates:" << std::endl;
////            if( !testNbrCovariatesOK1 )
////            {
////                DisplayErrorNbrCovariates_DisplayCovariates( nbrCovarTest1, covariateMap1.size() );
////            }
////            else
////            {
////                if( !testTextCovariatesOK1 )
////                {
////                    DisplayErrorNameCovariates_DisplayCovariates( covariatesDisplayed1, covariateMap1 );
////                }
////                if( !testCheckStateCovariatesOK1 )
////                {
////                    DisplayErrorCheckStateCovariates_DisplayCovariates( covariatesDisplayed1, covariatesCheckState1 );
////                }
////            }
////        }
////        if( !testNbrCovariatesOK2 || !testTextCovariatesOK2 || !testCheckStateCovariatesOK2 )
////        {
////            std::cerr << "\t+ Test covariateMap with 3 covariates:" << std::endl;
////            if( !testNbrCovariatesOK2 )
////            {
////                DisplayErrorNbrCovariates_DisplayCovariates( nbrCovarTest2, covariateMap2.size() );
////            }
////            else
////            {
////                if( !testTextCovariatesOK2 )
////                {
////                    DisplayErrorNameCovariates_DisplayCovariates( covariatesDisplayed2, covariateMap2 );
////                }
////                if( !testCheckStateCovariatesOK2 )
////                {
////                    DisplayErrorCheckStateCovariates_DisplayCovariates( covariatesDisplayed2, covariatesCheckState2 );
////                }
////            }
////        }
////        if( !testNbrCovariatesOK3 )
////        {
////            std::cerr << "\t+ Test empty covariateMap:" << std::endl;
////            DisplayErrorNbrCovariates_DisplayCovariates( nbrCovarTest3, covariateMap3.size() );
////        }
////    }
////    else
////    {
////        std::cout << std::endl << "Test_DisplayCovariates() PASSED" << std::endl;
////    }

////    return ( testNbrCovariatesOK1 & testTextCovariatesOK1 & testCheckStateCovariatesOK1 &
////             testNbrCovariatesOK2 & testTextCovariatesOK2 & testCheckStateCovariatesOK2 &
////             testNbrCovariatesOK3 );
//    return false;
//}



//bool TestFADTTSWindow::Test_UpdateAvailableFileParamTab()
//{
//    QSharedPointer<FADTTSWindow> fadttsWindow = QSharedPointer<FADTTSWindow>( new FADTTSWindow );

//    para_Model_FADTTS para_m;
//    para_Save_FADTTS para_s;
//    para_Load_FADTTS para_l;
//    fadttsWindow->SetParaLoad( &para_l );
//    fadttsWindow->SetParaSave( &para_s );
//    fadttsWindow->SetParaModel( &para_m );

//    bool testADFileLabel = true;
//    bool testADFileCheckState = true;
//    bool testRDFileLabel = true;
//    bool testRDFileCheckState = true;
//    bool testMDFileLabel = true;
//    bool testMDFileCheckState = true;
//    bool testFAFileLabel = true;
//    bool testFAFileCheckState = true;
//    bool testCOMPFileLabel = true;
//    bool testCOMPFileCheckState = true;

//    fadttsWindow->m_data.SetNbrRows( "ad" ) = 50;
//    fadttsWindow->m_data.SetNbrColumns( "ad" ) = 99;

//    fadttsWindow->m_data.SetNbrRows( "rd" ) = 0;
//    fadttsWindow->m_data.SetNbrColumns( "rd" ) = 50;

//    fadttsWindow->m_data.SetNbrRows( "md" ) = 50;
//    fadttsWindow->m_data.SetNbrColumns( "md" ) = 0;

//    fadttsWindow->m_data.SetNbrRows( "fa" ) = 0;
//    fadttsWindow->m_data.SetNbrColumns( "fa" ) = 0;

//    QString fileNotAvailable = "N/A";
//    QString adFileAvailable = QString::number( fadttsWindow->m_data.GetNbrRows( "ad" ) ) + "x" + QString::number( fadttsWindow->m_data.GetNbrColumns( "ad" ) );

//    fadttsWindow->UpdateAvailableFileParamTab();
//    QMap<QString, QCheckBox*>::ConstIterator iterCheckBox = fadttsWindow->m_paramTabFileCheckBoxMap.begin();
//    QMap<QString, QLabel*>::ConstIterator iterLabel = fadttsWindow->m_paramTabFileDataSizeLabelMap.begin();
//    while( iterCheckBox != fadttsWindow->m_paramTabFileCheckBoxMap.constEnd() )
//    {
//        if( iterCheckBox.key() == "ad" )
//        {
//            testADFileLabel = iterLabel.value()->text() == adFileAvailable;
//            testADFileCheckState = iterCheckBox.value()->isChecked() == true;
//        }
//        if( iterCheckBox.key() == "rd" )
//        {
//            testRDFileLabel = iterLabel.value()->text() == fileNotAvailable;
//            testRDFileCheckState = iterCheckBox.value()->isChecked() == false;
//        }
//        if( iterCheckBox.key() == "md" )
//        {
//            testMDFileLabel = iterLabel.value()->text() == fileNotAvailable;
//            testMDFileCheckState = iterCheckBox.value()->isChecked() == false;
//        }
//        if( iterCheckBox.key() == "fa" )
//        {
//            testFAFileLabel = iterLabel.value()->text() == fileNotAvailable;
//            testFAFileCheckState = iterCheckBox.value()->isChecked() == false;
//        }
//        if( iterCheckBox.key() == "COMP" )
//        {
//            testCOMPFileLabel = iterLabel.value()->text() == fileNotAvailable;
//            testCOMPFileCheckState = iterCheckBox.value()->isChecked() == false;
//        }
//        ++iterCheckBox;
//        ++iterLabel;
//    }


//    if( !testADFileLabel || !testADFileCheckState ||
//            !testRDFileLabel || !testRDFileCheckState ||
//            !testMDFileLabel || !testMDFileCheckState ||
//            !testFAFileLabel || !testFAFileCheckState ||
//            !testCOMPFileLabel || !testCOMPFileCheckState )
//    {
//        std::cerr << std::endl << "Test_UpdateAvailableFileParamTab() FAILED:" << std::endl;
//        if( !testADFileLabel || !testADFileCheckState )
//        {
//            std::cerr << "\t+ Test with nbrRows > 0 and nbrColumns > 0:" << std::endl;
//            if( !testADFileLabel )
//            {
//                DisplayErrorFileInfo_UpdateAvailableFileParamTab( adFileAvailable, fadttsWindow->m_paramTabFileDataSizeLabelMap.value( "ad" )->text() );
//            }
//            if( !testADFileCheckState )
//            {
//                DisplayErrorCheckStateAvailableFile_UpdateAvailableFileParamTab( true, fadttsWindow->m_paramTabFileCheckBoxMap.value( "ad" )->isChecked() );
//            }
//        }
//        if( !testRDFileLabel || !testRDFileCheckState )
//        {
//            std::cerr << "\t+ Test with nbrRows = 0 and nbrColumns > 0:" << std::endl;
//            if( !testRDFileLabel )
//            {
//                DisplayErrorFileInfo_UpdateAvailableFileParamTab( fileNotAvailable, fadttsWindow->m_paramTabFileDataSizeLabelMap.value( "rd" )->text() );
//            }
//            if( !testRDFileCheckState )
//            {
//                DisplayErrorCheckStateAvailableFile_UpdateAvailableFileParamTab( false, fadttsWindow->m_paramTabFileCheckBoxMap.value( "rd" )->isChecked() );
//            }
//        }
//        if( !testMDFileLabel || !testMDFileCheckState )
//        {
//            std::cerr << "\t+ Test with nbrRows > 0 and nbrColumns = 0:" << std::endl;
//            if( !testMDFileLabel )
//            {
//                DisplayErrorFileInfo_UpdateAvailableFileParamTab( fileNotAvailable, fadttsWindow->m_paramTabFileDataSizeLabelMap.value( "md" )->text() );
//            }
//            if( !testMDFileCheckState )
//            {
//                DisplayErrorCheckStateAvailableFile_UpdateAvailableFileParamTab( false, fadttsWindow->m_paramTabFileCheckBoxMap.value( "md" )->isChecked() );
//            }
//        }
//        if( !testFAFileLabel || !testFAFileCheckState )
//        {
//            std::cerr << "\t+ Test with nbrRows = 0 and nbrColumns = 0:" << std::endl;
//            if( !testFAFileLabel )
//            {
//                DisplayErrorFileInfo_UpdateAvailableFileParamTab( fileNotAvailable, fadttsWindow->m_paramTabFileDataSizeLabelMap.value( "fa" )->text() );
//            }
//            if( !testFAFileCheckState )
//            {
//                DisplayErrorCheckStateAvailableFile_UpdateAvailableFileParamTab( false, fadttsWindow->m_paramTabFileCheckBoxMap.value( "fa" )->isChecked() );
//            }
//        }
//        if( !testCOMPFileLabel || !testCOMPFileCheckState )
//        {
//            std::cerr << "\t+ Test with no file:" << std::endl;
//            if( !testCOMPFileLabel )
//            {
//                DisplayErrorFileInfo_UpdateAvailableFileParamTab( fileNotAvailable, fadttsWindow->m_paramTabFileDataSizeLabelMap.value( "COMP" )->text() );
//            }
//            if( !testCOMPFileCheckState )
//            {
//                DisplayErrorCheckStateAvailableFile_UpdateAvailableFileParamTab( false, fadttsWindow->m_paramTabFileCheckBoxMap.value( "COMP" )->isChecked() );
//            }
//        }
//    }
//    else
//    {
//        std::cout << std::endl << "Test_UpdateAvailableFileParamTab() PASSED" << std::endl;
//    }

//    return( testADFileLabel & testADFileCheckState &
//            testRDFileLabel & testRDFileCheckState &
//            testMDFileLabel & testMDFileCheckState &
//            testFAFileLabel & testFAFileCheckState &
//            testCOMPFileLabel & testCOMPFileCheckState );
//}

//bool TestFADTTSWindow::Test_SetInfoCovariateFileSubjectColumnID( QString inputCOMPFile )
//{
//    QSharedPointer<FADTTSWindow> fadttsWindow = QSharedPointer<FADTTSWindow>( new FADTTSWindow );
//    Processing processing;

//    para_Model_FADTTS para_m;
//    para_Save_FADTTS para_s;
//    para_Load_FADTTS para_l;
//    fadttsWindow->SetParaLoad( &para_l );
//    fadttsWindow->SetParaSave( &para_s );
//    fadttsWindow->SetParaModel( &para_m );

//    /************************************************/
//    /********************* Test1 ********************/
//    /************************************************/
//    /** New covariate file **/
//    fadttsWindow->m_data.SetFileData( "COMP" ) = processing.GetDataFromFile( inputCOMPFile );
//    fadttsWindow->SetInfoCovariateFileSubjectColumnID();

//    QString infoDisplayedExpectedTest1 = "<b><i><span style=font-size:7pt;>1</i></b></span>";
//    QString infoDisplayedDisplayedTest1 = fadttsWindow->inputTab_subjectColumnID_label->text();

//    bool testNewCovariateFile = infoDisplayedExpectedTest1 == infoDisplayedDisplayedTest1;


//    /************************************************/
//    /********************* Test2 ********************/
//    /************************************************/
//    /** Change subject column ID of covariate file **/
//    fadttsWindow->m_data.SetCovariateColumnID() = 5;
//    fadttsWindow->SetInfoCovariateFileSubjectColumnID();

//    QString infoDisplayedExpectedTest2 = "<b><i><span style=font-size:7pt;>6</i></b></span>";
//    QString infoDisplayedDisplayedTest2 = fadttsWindow->inputTab_subjectColumnID_label->text();

//    bool testCovariateFileSubjectColumnIDChanged = infoDisplayedExpectedTest2 == infoDisplayedDisplayedTest2;


//    /************************************************/
//    /********************* Test3 ********************/
//    /************************************************/
//    /** No covariate file **/
//    fadttsWindow->m_data.ClearFileInformation( "COMP" );
//    fadttsWindow->SetInfoCovariateFileSubjectColumnID();

//    QString infoDisplayedExpectedTest3 = "";
//    QString infoDisplayedDisplayedTest3 = fadttsWindow->inputTab_subjectColumnID_label->text();

//    bool testNoCovariateFile = infoDisplayedExpectedTest3 == infoDisplayedDisplayedTest3;


//    if( !testNewCovariateFile || !testCovariateFileSubjectColumnIDChanged || !testNoCovariateFile )
//    {
//        std::cerr << std::endl << "Test_SetInfoCovariateFileSubjectColumnID() FAILED:" << std::endl;
//        if( !testNewCovariateFile )
//        {
//            std::cerr << "\t+ Test with new covariate file:" << std::endl;
//            DisplayErrorID_SetInfoCovariateFileSubjectColumnID( infoDisplayedExpectedTest1, infoDisplayedDisplayedTest1 );
//        }
//        if( !testCovariateFileSubjectColumnIDChanged )
//        {
//            std::cerr << "\t+ Test when covariate file subject column ID changed:" << std::endl;
//            DisplayErrorID_SetInfoCovariateFileSubjectColumnID( infoDisplayedExpectedTest2, infoDisplayedDisplayedTest2 );
//        }
//        if( !testNoCovariateFile )
//        {
//            std::cerr << "\t+ Test with no covariate file:" << std::endl;
//            DisplayErrorID_SetInfoCovariateFileSubjectColumnID( infoDisplayedExpectedTest3, infoDisplayedDisplayedTest3 );
//        }
//    }
//    else
//    {
//        std::cout << std::endl << "Test_SetInfoCovariateFileSubjectColumnID() PASSED" << std::endl;
//    }

//    return ( testNewCovariateFile & testCovariateFileSubjectColumnIDChanged & testNoCovariateFile );
//}


//bool TestFADTTSWindow::Test_UpdateInputFileInformation( QString inputADFile, QString inputCOMPFile )
//{
//    QSharedPointer<FADTTSWindow> fadttsWindow = QSharedPointer<FADTTSWindow>( new FADTTSWindow );
//    Processing processing;

//    para_Model_FADTTS para_m;
//    para_Save_FADTTS para_s;
//    para_Load_FADTTS para_l;
//    fadttsWindow->SetParaLoad( &para_l );
//    fadttsWindow->SetParaSave( &para_s );
//    fadttsWindow->SetParaModel( &para_m );

//    /************************************************/
//    /******************** AD File *******************/
//    /************************************************/
//    QList<QStringList> ADFileData = processing.GetDataFromFile( inputADFile );
//    fadttsWindow->m_data.SetFilename( "AD" ) = inputADFile;
//    fadttsWindow->m_data.SetFileData( "AD" ) = ADFileData;

//    int ADFileNbrRowsExpected = ADFileData.count() - 1;
//    int ADFileNbrColumnsExpected = ADFileData.at( 0 ).count();
//    int ADFileNbrSubjectsExpected = ADFileData.at( 0 ).count() - 1;

//    fadttsWindow->UpdateInputFileInformation( "AD" );

//    bool NbrRowsADFile = ADFileNbrRowsExpected == fadttsWindow->m_data.GetNbrRows( "AD" );
//    bool NbrColumnsADFile = ADFileNbrColumnsExpected == fadttsWindow->m_data.GetNbrColumns( "AD" );
//    bool NbrSubjectsADFile = ADFileNbrSubjectsExpected == fadttsWindow->m_data.GetNbrSubjects( "AD" );


//    /************************************************/
//    /***************** Covariate File ***************/
//    /************************************************/
//    QList<QStringList> COMPFileData = processing.GetDataFromFile( inputCOMPFile );
//    fadttsWindow->m_data.SetFilename( "COMP" ) = inputCOMPFile;
//    fadttsWindow->m_data.SetFileData( "COMP" ) = COMPFileData;

//    int COMPFileNbrRowsExpected = COMPFileData.count() - 1;
//    int COMPFileNbrColumnsExpected = COMPFileData.at( 0 ).count() - 1;
//    int COMPFileNbrSubjectsExpected = COMPFileData.count() - 1;

//    QMap<int, QString> covariates = processing.GetCovariatesFromData( COMPFileData, 1 );
//    covariates.insert( -1, "Intercept" );
//    fadttsWindow->UpdateInputFileInformation( "COMP" );

//    bool NbrRowsCOMPFile = COMPFileNbrRowsExpected == fadttsWindow->m_data.GetNbrRows( "COMP" );
//    bool NbrColumnsCOMPFile = COMPFileNbrColumnsExpected == fadttsWindow->m_data.GetNbrColumns( "COMP" );
//    bool NbrSubjectsCOMPFile = COMPFileNbrSubjectsExpected == fadttsWindow->m_data.GetNbrSubjects( "COMP" );


//    if( !NbrRowsADFile || !NbrColumnsADFile || !NbrSubjectsADFile ||
//            !NbrRowsCOMPFile || !NbrColumnsCOMPFile || !NbrSubjectsCOMPFile )
//    {
//        std::cerr << std::endl << "Test_UpdateInputFileInformation() FAILED:" << std::endl;
//        if( !NbrRowsADFile || !NbrColumnsADFile || !NbrSubjectsADFile )
//        {
//            std::cerr << "\t+ AD file update:" << std::endl;
//            if( !NbrRowsADFile )
//            {
//                DisplayErrorNbrRows_UpdateInputFileInformation( ADFileNbrRowsExpected, fadttsWindow->m_data.GetNbrRows( "AD" ) );
//            }
//            if( !NbrColumnsADFile )
//            {
//                DisplayErrorNbrColumns_UpdateInputFileInformation( ADFileNbrColumnsExpected, fadttsWindow->m_data.GetNbrColumns( "AD" ) );
//            }
//            if( !NbrSubjectsADFile )
//            {
//                DisplayErrorNbrSubjects_UpdateInputFileInformation( ADFileNbrSubjectsExpected, fadttsWindow->m_data.GetNbrSubjects( "AD" ) );
//            }
//        }
//        if( !NbrRowsCOMPFile || !NbrColumnsCOMPFile || !NbrSubjectsCOMPFile )
//        {
//            std::cerr << "\t+ Covariate file update:" << std::endl;
//            if( !NbrRowsCOMPFile )
//            {
//                DisplayErrorNbrRows_UpdateInputFileInformation( COMPFileNbrRowsExpected, fadttsWindow->m_data.GetNbrRows( "COMP" ) );
//            }
//            if( !NbrColumnsCOMPFile )
//            {
//                DisplayErrorNbrColumns_UpdateInputFileInformation( COMPFileNbrColumnsExpected, fadttsWindow->m_data.GetNbrColumns( "COMP" ) );
//            }
//            if( !NbrSubjectsCOMPFile )
//            {
//                DisplayErrorNbrSubjects_UpdateInputFileInformation( COMPFileNbrSubjectsExpected, fadttsWindow->m_data.GetNbrSubjects( "COMP" ) );
//            }
//        }
//    }
//    else
//    {
//        std::cout << std::endl << "Test_UpdateInputFileInformation() PASSED" << std::endl;
//    }

//    return ( NbrRowsADFile & NbrColumnsADFile & NbrSubjectsADFile &
//             NbrRowsCOMPFile & NbrColumnsCOMPFile & NbrSubjectsCOMPFile );
//}

//bool TestFADTTSWindow::Test_DisplaySortedSubjects()
//{
////    QSharedPointer<FADTTSWindow> fadttsWindow = QSharedPointer<FADTTSWindow>( new FADTTSWindow );

////    para_Model_FADTTS para_m;
////    para_Save_FADTTS para_s;
////    para_Load_FADTTS para_l;
////    fadttsWindow->SetParaLoad( &para_l );
////    fadttsWindow->SetParaSave( &para_s );
////    fadttsWindow->SetParaModel( &para_m );

////    QStringList matchedSubjects;
////    QMap<QString, QStringList > unMatchedSubjectMap;
////    int nbrMatchedSubjects;
////    int nbrUnmatchedSubjects;

////    /************************************************/
////    /************* All Subjects Matched *************/
////    /************************************************/
////    bool checkStateAllSubjectsMatched = true;
////    bool backgroundColorAllSubjectsMatched = true;
////    bool flagsAllSubjectsMatched = true;

////    matchedSubjects << "Minou" << "Clemon" << "Marty" << "Duchesse" << "JB" << "Antheuny" << "PouletCoco" << "Bab" << "Thizy";
////    fadttsWindow->DisplaySortedSubjects( matchedSubjects, unMatchedSubjectMap );

////    nbrMatchedSubjects = matchedSubjects.size();
////    int nbrSubjectsAllSubjectsMatched = fadttsWindow->m_sortedSubjectListWidget->count();
////    bool sizeAllSubjectsMatchedOK = nbrMatchedSubjects == nbrSubjectsAllSubjectsMatched;
////    if( sizeAllSubjectsMatchedOK )
////    {
////        for( int i = 0; i < nbrMatchedSubjects; i++ )
////        {
////            QListWidgetItem *item = fadttsWindow->m_sortedSubjectListWidget->item( i );
////            if( item->checkState() != Qt::Checked )
////            {
////                checkStateAllSubjectsMatched = false;
////            }
////            if( item->backgroundColor() != fadttsWindow->m_green )
////            {
////                backgroundColorAllSubjectsMatched = false;
////            }
////            if( item->flags() != Qt::ItemIsEnabled )
////            {
////                flagsAllSubjectsMatched = false;
////            }
////        }
////    }

////    QString expectedSortedSubjectsInformationAllSubjectsMatched = "All subjects matched ( " + QString::number( matchedSubjects.size() ) + " )";
////    QString sortedSubjectsInformationDisplayedAllSubjectsMatched = fadttsWindow->subjectTab_sortedSubjectsInformation_label->text();

////    bool sortedSubjectsInformationAllSubjectsMatched = expectedSortedSubjectsInformationAllSubjectsMatched == sortedSubjectsInformationDisplayedAllSubjectsMatched;


////    /************************************************/
////    /******** Matched and Unmatched Subjects ********/
////    /************************************************/
////    bool checkStateMatchedAndUnmatchedSubjects_matchedSubjects = true;
////    bool backgroundColorMatchedAndUnmatchedSubjects_matchedSubjects = true;
////    bool flagsMatchedAndUnmatchedSubjects_matchedSubjects = true;

////    bool textMatchedAndUnmatchedSubjects_unmatchedSubjects = true;
////    bool backgroundColorMatchedAndUnmatchedSubjects_unmatchedSubjects = true;
////    bool flagsMatchedAndUnmatchedSubjects_unmatchedSubjects = true;

////    QStringList inStan;
////    inStan << "ad" << "rd" << "md" << "fa" << "COMP";
////    QStringList inKyle;
////    inKyle << "ad" << "rd" << "md" << "fa";
////    QStringList inKenny;
////    inKenny << "ad" << "rd" << "md";
////    QStringList inCartman;
////    inCartman << "ad" << "rd";
////    QStringList inButters;
////    inButters << "ad";
////    unMatchedSubjectMap.insert( "Stan", inStan );
////    unMatchedSubjectMap.insert( "Kyle", inKyle );
////    unMatchedSubjectMap.insert( "Kenny", inKenny );
////    unMatchedSubjectMap.insert( "Cartman", inCartman );
////    unMatchedSubjectMap.insert( "Butters", inButters );
////    nbrUnmatchedSubjects = unMatchedSubjectMap.size();
////    fadttsWindow->DisplaySortedSubjects( matchedSubjects, unMatchedSubjectMap );

////    int nbrSubjectsMatchedAndUnmatchedSubjects = fadttsWindow->m_sortedSubjectListWidget->count();
////    bool sizeMatchedAndUnmatchedSubjectsOK = ( nbrMatchedSubjects + nbrUnmatchedSubjects ) == nbrSubjectsMatchedAndUnmatchedSubjects;
////    if( sizeMatchedAndUnmatchedSubjectsOK )
////    {
////        for( int i = 0; i < nbrMatchedSubjects; i++ )
////        {
////            QListWidgetItem *item = fadttsWindow->m_sortedSubjectListWidget->item( i );
////            if( item->checkState() != Qt::Checked )
////            {
////                checkStateMatchedAndUnmatchedSubjects_matchedSubjects = false;
////            }
////            if( item->backgroundColor() != fadttsWindow->m_green )
////            {
////                backgroundColorMatchedAndUnmatchedSubjects_matchedSubjects = false;
////            }
////            if( item->flags() != Qt::ItemIsEnabled )
////            {
////                flagsMatchedAndUnmatchedSubjects_matchedSubjects = false;
////            }
////        }
////        QMap<QString, QStringList >::ConstIterator iterUnmatchedSubjects = unMatchedSubjectMap.begin();
////        for( int i = nbrMatchedSubjects; i < ( nbrMatchedSubjects + nbrUnmatchedSubjects ); i++ )
////        {
////            QStringList sortedText = iterUnmatchedSubjects.value();
////            sortedText.sort();
////            QString expectedText = iterUnmatchedSubjects.key() + " --> " + sortedText.join( ", " );

////            QListWidgetItem *item = fadttsWindow->m_sortedSubjectListWidget->item( i );
////            if( item->text() != expectedText )
////            {
////                textMatchedAndUnmatchedSubjects_unmatchedSubjects = false;
////            }
////            if( item->backgroundColor() != fadttsWindow->m_red )
////            {
////                backgroundColorMatchedAndUnmatchedSubjects_unmatchedSubjects = false;
////            }
////            if( item->flags() == Qt::ItemIsEnabled )
////            {
////                flagsMatchedAndUnmatchedSubjects_unmatchedSubjects = false;
////            }
////            ++iterUnmatchedSubjects;
////        }
////    }

////    QString expectedSortedSubjectsInformationMatchedAndUnmatchedSubjects = QString::number( matchedSubjects.size() ) + "/" + QString::number( nbrMatchedSubjects + nbrUnmatchedSubjects ) + " matched      " +
////            QString::number( unMatchedSubjectMap.size() ) + "/" + QString::number( nbrMatchedSubjects + nbrUnmatchedSubjects ) + " unmatched";
////    QString sortedSubjectsInformationDisplayedMatchedAndUnmatchedSubjects = fadttsWindow->subjectTab_sortedSubjectsInformation_label->text();

////    bool sortedSubjectsInformationMatchedAndUnmatchedSubjects = expectedSortedSubjectsInformationMatchedAndUnmatchedSubjects == sortedSubjectsInformationDisplayedMatchedAndUnmatchedSubjects;


////    /************************************************/
////    /************** No Subjects Matched *************/
////    /************************************************/
////    bool textNoSubjectsMatched = true;
////    bool backgroundColorNoSubjectsMatched = true;
////    bool flagsNoSubjectsMatched = true;

////    matchedSubjects.clear();
////    fadttsWindow->DisplaySortedSubjects( matchedSubjects, unMatchedSubjectMap );

////    int nbrSubjectsNoSubjectsMatched = fadttsWindow->m_sortedSubjectListWidget->count();
////    bool sizeNoSubjectsMatchedOK = nbrUnmatchedSubjects == nbrSubjectsNoSubjectsMatched;
////    if( sizeNoSubjectsMatchedOK )
////    {
////        QMap<QString, QStringList >::ConstIterator iterUnmatchedSubjects = unMatchedSubjectMap.begin();
////        for( int i = nbrMatchedSubjects; i < ( nbrUnmatchedSubjects ); i++ )
////        {
////            QStringList sortedText = iterUnmatchedSubjects.value();
////            QString expectedText = iterUnmatchedSubjects.key() + " --> " + sortedText.join( ", " );

////            QListWidgetItem *item = fadttsWindow->m_sortedSubjectListWidget->item( i );
////            if( item->text() != expectedText )
////            {
////                textNoSubjectsMatched = false;
////            }
////            if( item->backgroundColor() != fadttsWindow->m_red )
////            {
////                backgroundColorNoSubjectsMatched = false;
////            }
////            if( item->flags() = Qt::ItemIsEnabled )
////            {
////                flagsNoSubjectsMatched = false;
////            }
////        }
////    }

////    QString expectedSortedSubjectsInformationNoSubjectsMatched = "Warning! No subject matched ( " + QString::number( unMatchedSubjectMap.size() ) + " )";
////    QString sortedSubjectsInformationDisplayedNoSubjectsMatched = fadttsWindow->subjectTab_sortedSubjectsInformation_label->text();

////    bool sortedSubjectsInformationNoSubjectsMatched = expectedSortedSubjectsInformationNoSubjectsMatched == sortedSubjectsInformationDisplayedNoSubjectsMatched;


////    /************************************************/
////    /****************** No Subjects *****************/
////    /************************************************/
////    unMatchedSubjectMap.clear();

////    fadttsWindow->DisplaySortedSubjects( matchedSubjects, unMatchedSubjectMap );

////    QString sortedSubjectsInformationDisplayedNoSubjects = fadttsWindow->subjectTab_sortedSubjectsInformation_label->text();

////    int nbrSubjectsNoSubjects = fadttsWindow->m_sortedSubjectListWidget->count();
////    bool sizeNoSubjectsOK = nbrSubjectsNoSubjects == 0;
////    bool sortedSubjectsInformationNoSubjects = sortedSubjectsInformationDisplayedNoSubjects.isEmpty();


////    if( !sizeAllSubjectsMatchedOK || !checkStateAllSubjectsMatched || !backgroundColorAllSubjectsMatched || !flagsAllSubjectsMatched || !sortedSubjectsInformationAllSubjectsMatched ||
////            !sizeMatchedAndUnmatchedSubjectsOK || !checkStateMatchedAndUnmatchedSubjects_matchedSubjects || !backgroundColorMatchedAndUnmatchedSubjects_matchedSubjects ||
////            !flagsMatchedAndUnmatchedSubjects_matchedSubjects || !textMatchedAndUnmatchedSubjects_unmatchedSubjects || !backgroundColorMatchedAndUnmatchedSubjects_unmatchedSubjects ||
////            !flagsMatchedAndUnmatchedSubjects_unmatchedSubjects || !sortedSubjectsInformationMatchedAndUnmatchedSubjects ||
////            !sizeNoSubjectsMatchedOK || !textNoSubjectsMatched || !backgroundColorNoSubjectsMatched || !flagsNoSubjectsMatched || !sortedSubjectsInformationNoSubjectsMatched ||
////            !sizeNoSubjectsOK || !sortedSubjectsInformationNoSubjects )
////    {
////        std::cerr << std::endl << "Test_DisplaySortedSubjects() FAILED:" << std::endl;
////        if( !sizeAllSubjectsMatchedOK || !checkStateAllSubjectsMatched || !backgroundColorAllSubjectsMatched || !flagsAllSubjectsMatched || !sortedSubjectsInformationAllSubjectsMatched )
////        {
////            std::cerr << "\t+ Test with only matched subjects:" << std::endl;
////            if( !sizeAllSubjectsMatchedOK )
////            {
////                DisplayErrorNbrSubjects_DisplayNbrSubjectSelected( nbrMatchedSubjects, nbrSubjectsAllSubjectsMatched );
////            }
////            if( !checkStateAllSubjectsMatched || !backgroundColorAllSubjectsMatched || !flagsAllSubjectsMatched )
////            {
////                DisplayErrorMatchedSubjects_DisplayNbrSubjectSelected( checkStateAllSubjectsMatched, backgroundColorAllSubjectsMatched, flagsAllSubjectsMatched );
////            }
////            if( !sortedSubjectsInformationAllSubjectsMatched )
////            {
////                DisplayErrorSortedSubjectsInformation_DisplayNbrSubjectSelected( expectedSortedSubjectsInformationAllSubjectsMatched, sortedSubjectsInformationDisplayedAllSubjectsMatched );
////            }
////        }
////        if( !sizeNoSubjectsMatchedOK || !textNoSubjectsMatched || !backgroundColorNoSubjectsMatched || !flagsNoSubjectsMatched || !sortedSubjectsInformationNoSubjectsMatched )
////        {
////            std::cerr << "\t+ Test with only unmatched subjects:" << std::endl;
////            if( !sizeNoSubjectsMatchedOK )
////            {
////                DisplayErrorNbrSubjects_DisplayNbrSubjectSelected( nbrUnmatchedSubjects, nbrSubjectsNoSubjectsMatched );
////            }
////            if( !textNoSubjectsMatched || !backgroundColorNoSubjectsMatched || !flagsNoSubjectsMatched )
////            {
////                DisplayErrorUnMatchedSubjects_DisplayNbrSubjectSelected( textNoSubjectsMatched, backgroundColorNoSubjectsMatched, flagsNoSubjectsMatched );
////            }
////            if( !sortedSubjectsInformationNoSubjectsMatched )
////            {
////                DisplayErrorSortedSubjectsInformation_DisplayNbrSubjectSelected( expectedSortedSubjectsInformationNoSubjectsMatched, sortedSubjectsInformationDisplayedNoSubjectsMatched );
////            }
////        }
////        if( !sizeMatchedAndUnmatchedSubjectsOK || !checkStateMatchedAndUnmatchedSubjects_matchedSubjects || !backgroundColorMatchedAndUnmatchedSubjects_matchedSubjects ||
////                !flagsMatchedAndUnmatchedSubjects_matchedSubjects || !textMatchedAndUnmatchedSubjects_unmatchedSubjects || !backgroundColorMatchedAndUnmatchedSubjects_unmatchedSubjects ||
////                !flagsMatchedAndUnmatchedSubjects_unmatchedSubjects || !sortedSubjectsInformationMatchedAndUnmatchedSubjects )
////        {
////            std::cerr << "\t+ Test with both matched and unmatched subjects:" << std::endl;
////            if( !sizeMatchedAndUnmatchedSubjectsOK )
////            {
////                DisplayErrorNbrSubjects_DisplayNbrSubjectSelected( nbrMatchedSubjects + nbrUnmatchedSubjects, nbrSubjectsMatchedAndUnmatchedSubjects );
////            }
////            if( !checkStateMatchedAndUnmatchedSubjects_matchedSubjects || !backgroundColorMatchedAndUnmatchedSubjects_matchedSubjects || !flagsMatchedAndUnmatchedSubjects_matchedSubjects )
////            {
////                DisplayErrorMatchedSubjects_DisplayNbrSubjectSelected( checkStateMatchedAndUnmatchedSubjects_matchedSubjects, backgroundColorMatchedAndUnmatchedSubjects_matchedSubjects, flagsMatchedAndUnmatchedSubjects_matchedSubjects );
////            }
////            if( !textMatchedAndUnmatchedSubjects_unmatchedSubjects || !backgroundColorMatchedAndUnmatchedSubjects_unmatchedSubjects || !flagsMatchedAndUnmatchedSubjects_unmatchedSubjects )
////            {
////                DisplayErrorUnMatchedSubjects_DisplayNbrSubjectSelected( textMatchedAndUnmatchedSubjects_unmatchedSubjects, backgroundColorMatchedAndUnmatchedSubjects_unmatchedSubjects, flagsMatchedAndUnmatchedSubjects_unmatchedSubjects );
////            }
////            if( !sortedSubjectsInformationMatchedAndUnmatchedSubjects )
////            {
////                DisplayErrorSortedSubjectsInformation_DisplayNbrSubjectSelected( expectedSortedSubjectsInformationMatchedAndUnmatchedSubjects, sortedSubjectsInformationDisplayedMatchedAndUnmatchedSubjects );
////            }
////        }
////        if( !sizeNoSubjectsOK || !sortedSubjectsInformationNoSubjects )
////        {
////            std::cerr << "\t+ Test with no subjects:" << std::endl;
////            if( !sizeNoSubjectsOK )
////            {
////                DisplayErrorNbrSubjects_DisplayNbrSubjectSelected( 0, nbrSubjectsNoSubjects );
////            }
////            if( !sortedSubjectsInformationNoSubjects )
////            {
////                DisplayErrorSortedSubjectsInformation_DisplayNbrSubjectSelected( "", sortedSubjectsInformationDisplayedNoSubjects );
////            }
////        }
////    }
////    else
////    {
////        std::cout << std::endl << "Test_DisplaySortedSubjects() PASSED" << std::endl;
////    }

////    return ( sizeAllSubjectsMatchedOK & checkStateAllSubjectsMatched & backgroundColorAllSubjectsMatched & flagsAllSubjectsMatched & sortedSubjectsInformationAllSubjectsMatched &
////             sizeMatchedAndUnmatchedSubjectsOK & checkStateMatchedAndUnmatchedSubjects_matchedSubjects & backgroundColorMatchedAndUnmatchedSubjects_matchedSubjects &
////             flagsMatchedAndUnmatchedSubjects_matchedSubjects & textMatchedAndUnmatchedSubjects_unmatchedSubjects & backgroundColorMatchedAndUnmatchedSubjects_unmatchedSubjects &
////             flagsMatchedAndUnmatchedSubjects_unmatchedSubjects & sortedSubjectsInformationMatchedAndUnmatchedSubjects &
////             sizeNoSubjectsMatchedOK & textNoSubjectsMatched & backgroundColorNoSubjectsMatched & flagsNoSubjectsMatched & sortedSubjectsInformationNoSubjectsMatched &
////             sizeNoSubjectsOK & sortedSubjectsInformationNoSubjects );
//    return false;
//}

//bool TestFADTTSWindow::Test_DisplayNbrSubjectSelected()
//{
////    QSharedPointer<FADTTSWindow> fadttsWindow = QSharedPointer<FADTTSWindow>( new FADTTSWindow );

////    para_Model_FADTTS para_m;
////    para_Save_FADTTS para_s;
////    para_Load_FADTTS para_l;
////    fadttsWindow->SetParaLoad( &para_l );
////    fadttsWindow->SetParaSave( &para_s );
////    fadttsWindow->SetParaModel( &para_m );

////    QStringList subjects;
////    subjects << "Stan" << "Kyle" << "Kenny" << "Cartman" << "Chef" << "Butters" << "Pip" << "Tweek" << "Timmy" << "Wendy";

////    /************************************************/
////    /******************** Test 1 ********************/
////    /************************************************/
////    foreach( QString subject, subjects )
////    {
////        QListWidgetItem *item = new QListWidgetItem( subject, fadttsWindow->m_sortedSubjectListWidget );
////        item->setCheckState( Qt::Checked );
////        item->setFlags( Qt::ItemIsEnabled );
////        fadttsWindow->m_sortedSubjectListWidget->addItem( item );
////    }
////    fadttsWindow->DisplayNbrSubjectSelected();
////    QString checkedSubjectsInformationExpectedTest1 = QString::number( subjects.count() ) + " subjects selected";
////    QString checkedSubjectsInformationDisplayedTest1 = fadttsWindow->subjectTab_checkedSubjectsInformation_label->text();

////    bool resultTest1 = checkedSubjectsInformationExpectedTest1 == checkedSubjectsInformationDisplayedTest1;


////    /************************************************/
////    /******************** Test 2 ********************/
////    /************************************************/
////    for( int i = 0; i < fadttsWindow->m_sortedSubjectListWidget->count()-3; i++ )
////    {
////        QListWidgetItem *item = fadttsWindow->m_sortedSubjectListWidget->item( i );
////        item->setCheckState( Qt::Unchecked );
////    }
////    fadttsWindow->DisplayNbrSubjectSelected();
////    QString checkedSubjectsInformationExpectedTest2 = QString::number( 3 ) + " subjects selected";
////    QString checkedSubjectsInformationDisplayedTest2 = fadttsWindow->subjectTab_checkedSubjectsInformation_label->text();

////    bool resultTest2 = checkedSubjectsInformationExpectedTest2 == checkedSubjectsInformationDisplayedTest2;

////    /************************************************/
////    /******************** Test 3 ********************/
////    /************************************************/
////    fadttsWindow->subjectTab_checkedSubjectsInformation_label->clear();

////    QString checkedSubjectsInformationDisplayedTest3 = fadttsWindow->subjectTab_checkedSubjectsInformation_label->text();
////    bool resultTest3 = checkedSubjectsInformationDisplayedTest3.isEmpty();


////    if( !resultTest1 || !resultTest2 || !resultTest3 )
////    {
////        std::cerr << std::endl << "Test_DisplayNbrSubjectSelected() FAILED:" << std::endl;
////        if( !resultTest1 )
////        {
////            std::cerr << "\t+ All subjects in m_sortedSubjectListWidget are checked:" << std::endl;
////            DisplayErrorCheckedSubjectsInformation_DisplayNbrSubjectSelected( checkedSubjectsInformationExpectedTest1, checkedSubjectsInformationDisplayedTest1 );
////        }
////        if( !resultTest2 )
////        {
////            std::cerr << "\t+ Some subjects in m_sortedSubjectListWidget are unchecked:" << std::endl;
////            DisplayErrorCheckedSubjectsInformation_DisplayNbrSubjectSelected( checkedSubjectsInformationExpectedTest2, checkedSubjectsInformationDisplayedTest2 );
////        }
////        if( !resultTest3 )
////        {
////            std::cerr << "\t+ Test with no covariate file:" << std::endl;
////            DisplayErrorCheckedSubjectsInformation_DisplayNbrSubjectSelected( "", checkedSubjectsInformationDisplayedTest3 );
////        }
////    }
////    else
////    {
////        std::cout << std::endl << "Test_DisplayNbrSubjectSelected() PASSED" << std::endl;
////    }

////    return ( resultTest1 & resultTest2 & resultTest3 );
//    return false;
//}

//bool TestFADTTSWindow::Test_SearchSubjects()
//{

//}

//bool TestFADTTSWindow::Test_UpdateLineEditsAfterAddingMultipleFiles()
//{

//}


//bool TestFADTTSWindow::Test_SelectSubject()
//{

//}

//bool TestFADTTSWindow::Test_SelectCovariate()
//{

//}


//bool TestFADTTSWindow::Test_GetSelectedPrefixes()
//{

//}

//bool TestFADTTSWindow::Test_GetSelectedInputFiles()
//{

//}

//bool TestFADTTSWindow::Test_GetSelectedCovariates()
//{

//}

//bool TestFADTTSWindow::Test_GenerateSelectedSubjectFile()
//{

//}


//bool TestFADTTSWindow::Test_SyncUiToModelStructure()
//{

//}



///**********************************************************************/
///********************** Functions Used For Testing ********************/
///**********************************************************************/
//void TestFADTTSWindow::DisplayErrorNbrCovariates_DisplayCovariates( int nbrCovariates, int nbrCovariatesExpected )
//{
//    std::cerr << "\t   m_covariateListWidget does not have the right number of covariates. " << nbrCovariates <<
//                 " covariates are displayed whereas " << nbrCovariatesExpected << " were expected." << std::endl;
//}

//void TestFADTTSWindow::DisplayErrorNameCovariates_DisplayCovariates( QStringList covariatesDisplayed, QMap<int, QString> covariateMap )
//{
//    std::cerr << "\t   m_covariateListWidget does not display the right covariates" << std::endl;
//    std::cerr << "\t\t- Covariates expected" << std::endl;
//    for( int i = 0; i < covariateMap.size(); i++ )
//    {
//        std::cerr << "\t\t  \"" << covariateMap.values().at( i ).toStdString() << "\"" << std::endl;
//    }
//    std::cerr << "\t\t- Covariates displayed" << std::endl;
//    for( int i = 0; i < covariatesDisplayed.size(); i++ )
//    {
//        std::cerr << "\t\t  \"" << covariatesDisplayed.at( i ).toStdString() << "\"" << std::endl;
//    }
//}

//void TestFADTTSWindow::DisplayErrorCheckStateCovariates_DisplayCovariates( QStringList covariatesDisplayed, QList<Qt::CheckState> covariatesCheckState )
//{
//    std::cerr << "\t   Items in m_covariateListWidget do not have the right checkState ( 0 == unChecked and 2 == checked )" << std::endl;
//    for( int i = 0; i < covariatesDisplayed.size(); i++ )
//    {
//        std::cerr << "\t\t- " << covariatesDisplayed.at( i ).toStdString() << " --> check state expected: 2 || check state displayed: " << covariatesCheckState.at( i ) << std::endl;
//    }
//}


//void TestFADTTSWindow::DisplayErrorFileInfo_UpdateAvailableFileParamTab( QString expectedFileLabel, QString fileLabelDisplayed )
//{
//    std::cerr << "\t   m_paramTabFileCheckBoxMap does not display the right file info" << std::endl;
//    std::cerr << "\t\t- Info expected: " << expectedFileLabel.toStdString() << " || Info displayed: " << fileLabelDisplayed.toStdString() << std::endl;
//}

//void TestFADTTSWindow::DisplayErrorCheckStateAvailableFile_UpdateAvailableFileParamTab( bool checkStateExpected, bool checkStateDisplayed )
//{
//    std::cerr << "\t   File in m_paramTabFileCheckBoxMap does not have the right checkState ( 0 == unChecked and 1 == checked )" << std::endl;
//    std::cerr << "\t\t- Check state expected: " << checkStateExpected << " || Check state displayed: " << checkStateDisplayed << std::endl;
//}


//void TestFADTTSWindow::DisplayErrorID_SetInfoCovariateFileSubjectColumnID( QString labelExpected, QString labelDisplayed )
//{
//    std::cerr << "\t   Wrong column ID displayed" << std::endl;
//    std::cerr << "\t\t- ID expected: " << labelExpected.toStdString() << " || ID displayed: " << labelDisplayed.toStdString() << std::endl;
//}


//void TestFADTTSWindow::DisplayErrorNbrRows_UpdateInputFileInformation( int nbrRowsExpected, int nbrRowsLoaded )
//{
//    std::cerr << "\t   Wrong nbr of rows" << std::endl;
//    std::cerr << "\t\t- Nbr of rows expected: " << nbrRowsExpected << " || Nbr of rows loaded: " << nbrRowsLoaded << std::endl;
//}

//void TestFADTTSWindow::DisplayErrorNbrColumns_UpdateInputFileInformation( int nbrColumnsExpected, int nbrColumnsLoaded )
//{
//    std::cerr << "\t   Wrong nbr of columns" << std::endl;
//    std::cerr << "\t\t- Nbr of columns expected: " << nbrColumnsExpected << " || Nbr of columns loaded: " << nbrColumnsLoaded << std::endl;
//}

//void TestFADTTSWindow::DisplayErrorNbrSubjects_UpdateInputFileInformation( int nbrSubjectsExpected, int nbrSubjectsLoaded )
//{
//    std::cerr << "\t   Wrong nbr of subjects" << std::endl;
//    std::cerr << "\t\t- Nbr of subjects expected: " << nbrSubjectsExpected << " || Nbr of subjects loaded: " << nbrSubjectsLoaded << std::endl;
//}


//void TestFADTTSWindow::DisplayErrorNbrSubjects_DisplayNbrSubjectSelected( int nbrSubjectsExpected, int nbrSubjectsDisplayed )
//{
//    std::cerr << "\t   Wrong nbr subjects displayed" << std::endl;
//    std::cerr << "\t\t- Nbr of subjects expected: " << nbrSubjectsExpected << " || Nbr of subjects displayed: " << nbrSubjectsDisplayed << std::endl;
//}

//void TestFADTTSWindow::DisplayErrorMatchedSubjects_DisplayNbrSubjectSelected( bool checkState, bool backgroundColor, bool flags )
//{
//    std::cerr << "\t   At least one of the matched subjects displayed in the QListWidget have the wrong" << std::endl;
//    if( !checkState )
//    {
//        std::cerr << "\t\t- Check state ( which should be Checked )" << std::endl;
//    }
//    if( !backgroundColor )
//    {
//        std::cerr << "\t\t- Background color ( which should be green )" << std::endl;
//    }
//    if( !flags )
//    {
//        std::cerr << "\t\t- Flag ( which should be Enable )" << std::endl;
//    }
//}

//void TestFADTTSWindow::DisplayErrorUnMatchedSubjects_DisplayNbrSubjectSelected( bool text, bool backgroundColor, bool flags )
//{
//    std::cerr << "\t   At least one of the unmatched subjects displayed in the QListWidget have the wrong" << std::endl;
//    if( !text )
//    {
//        std::cerr << "\t\t- Text" << std::endl;
//    }
//    if( !backgroundColor )
//    {
//        std::cerr << "\t\t- Background color ( which should be red )" << std::endl;
//    }
//    if( !flags )
//    {
//        std::cerr << "\t\t- Flag ( which should be Disable )" << std::endl;
//    }
//}

//void TestFADTTSWindow::DisplayErrorSortedSubjectsInformation_DisplayNbrSubjectSelected( QString expectedInformation, QString informationDisplayed )
//{
//    std::cerr << "\t   Wrong sorted subjects information displayed" << std::endl;
//    std::cerr << "\t\t- Info expected: " << expectedInformation.toStdString() << " || Info displayed: " << informationDisplayed.toStdString() << std::endl;
//}






//void TestFADTTSWindow::DisplayErrorCheckedSubjectsInformation_DisplayNbrSubjectSelected( QString checkedSubjectsInformationExpected, QString checkedSubjectsInformationDisplayed )
//{
//    std::cerr << "\t   Information about nbr of subjects selected is incorrect" << std::endl;
//    std::cerr << "\t\t- Information expected: " << checkedSubjectsInformationExpected.toStdString() << " || Information displayed: " << checkedSubjectsInformationDisplayed.toStdString() << std::endl;
//}



