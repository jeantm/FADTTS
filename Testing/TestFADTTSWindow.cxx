#include "TestFADTTSWindow.h"


TestFADTTSWindow::TestFADTTSWindow()
{
}


/**********************************************************************/
/*************************** Test Functions ***************************/
/**********************************************************************/
bool TestFADTTSWindow::Test_UpdateCurrentDir()
{
    int argc = 0;
    char **argv = 0;
    QApplication *app = new QApplication( argc, argv );
    QSharedPointer<FADTTSWindow> fadttsWindow = QSharedPointer<FADTTSWindow>( new FADTTSWindow );

    QString currentFileInputDir = fadttsWindow->m_currentFileInputDir;
    QString currentSubjectListInputDir = fadttsWindow->m_currentSubjectListInputDir;

    QString filePathOK1 = "/home/file.txt";
    QString filePathOK2 = "/file.txt";
    QString filePathKO1 = "this/path/doesnt/exist/file.txt";
    QString filePathKO2 = "/home/folder/file.txt";

    fadttsWindow->UpdateCurrentDir( filePathOK1, currentFileInputDir );
    fadttsWindow->UpdateCurrentDir( filePathOK1, currentSubjectListInputDir );

    bool testUpdateCurrentDirOK1 = ( currentFileInputDir == "/home" ) && ( currentSubjectListInputDir == "/home" );


    fadttsWindow->UpdateCurrentDir( filePathKO1, currentFileInputDir );
    fadttsWindow->UpdateCurrentDir( filePathKO1, currentSubjectListInputDir );

    bool testUpdateCurrentDirKO1 = ( currentFileInputDir == "/home" ) && ( currentSubjectListInputDir == "/home" );


    fadttsWindow->UpdateCurrentDir( filePathOK2, currentFileInputDir );
    fadttsWindow->UpdateCurrentDir( filePathOK2, currentSubjectListInputDir );

    bool testUpdateCurrentDirOK2 = ( currentFileInputDir == "/" ) && ( currentSubjectListInputDir == "/" );


    fadttsWindow->UpdateCurrentDir( filePathKO2, currentFileInputDir );
    fadttsWindow->UpdateCurrentDir( filePathKO2, currentSubjectListInputDir );

    bool testUpdateCurrentDirKO2 = ( currentFileInputDir == "/" ) && ( currentSubjectListInputDir == "/" );


    if( !testUpdateCurrentDirOK1 || !testUpdateCurrentDirKO1 || !testUpdateCurrentDirOK2 || !testUpdateCurrentDirKO2 )
    {
        std::cerr << std::endl << "Test_UpdateCurrentDir() FAILED:" << std::endl;
        if( !testUpdateCurrentDirOK1 )
        {
            std::cerr << "\tm_currentFileInputDir and m_currentSubjectListInputDir not correctly updated when they should have. filePath = \"/home/file.txt\"" << std::endl;
        }
        if( !testUpdateCurrentDirKO1 )
        {
            std::cerr << "\tm_currentFileInputDir and m_currentSubjectListInputDir updated when they should not have. filePath = \"this/path/doesnt/exist/file.txt\"" << std::endl;
        }
        if( !testUpdateCurrentDirOK2 )
        {
            std::cerr << "\tm_currentFileInputDir and m_currentSubjectListInputDir not correctly updated when they should have. filePath = \"/file.txt\"" << std::endl;
        }
        if( !testUpdateCurrentDirKO2 )
        {
            std::cerr << "\tm_currentFileInputDir and m_currentSubjectListInputDir updated when they should not have. filePath = \"/home/folder/file.txt\"" << std::endl;
        }
    }
    else
    {
        std::cout << std::endl << "Test_UpdateCurrentDir() PASSED" << std::endl;
    }

    app->exit();
    return ( testUpdateCurrentDirOK1 & testUpdateCurrentDirKO1 & testUpdateCurrentDirOK2 & testUpdateCurrentDirKO2 );
}

bool TestFADTTSWindow::Test_UpdateCurrentDirEditInputDialog()
{
    int argc = 0;
    char **argv = 0;
    QApplication *app = new QApplication( argc, argv );
    QSharedPointer<FADTTSWindow> fadttsWindow = QSharedPointer<FADTTSWindow>( new FADTTSWindow );

    QString currentFileInputDir = fadttsWindow->m_currentFileInputDir;

    QString filePathOK1 = "/home/file.txt";
    QString filePathOK2 = "/file.txt";
    QString filePathKO1 = "this/path/doesnt/exist/file.txt";
    QString filePathKO2 = "/home/folder/file.txt";

    fadttsWindow->UpdateCurrentDirEditInputDialog( filePathOK1, currentFileInputDir );

    bool testUpdateCurrentDirOK1 = fadttsWindow->m_editInputDialog->m_currentInputDir == "/home";


    fadttsWindow->UpdateCurrentDirEditInputDialog( filePathKO1, currentFileInputDir );

    bool testUpdateCurrentDirKO1 = fadttsWindow->m_editInputDialog->m_currentInputDir == "/home";


    fadttsWindow->UpdateCurrentDirEditInputDialog( filePathOK2, currentFileInputDir );

    bool testUpdateCurrentDirOK2 = fadttsWindow->m_editInputDialog->m_currentInputDir == "/";


    fadttsWindow->UpdateCurrentDirEditInputDialog( filePathKO2, currentFileInputDir );

    bool testUpdateCurrentDirKO2 = fadttsWindow->m_editInputDialog->m_currentInputDir == "/";


    if( !testUpdateCurrentDirOK1 || !testUpdateCurrentDirKO1 || !testUpdateCurrentDirOK2 || !testUpdateCurrentDirKO2 )
    {
        std::cerr << std::endl << "Test_UpdateCurrentDirEditInputDialog() FAILED:" << std::endl;
        if( !testUpdateCurrentDirOK1 )
        {
            std::cerr << "\tEditInputDialog m_currentInputDir not correctly updated when it should have. filePath = \"/home/file.txt\"" << std::endl;
        }
        if( !testUpdateCurrentDirKO1 )
        {
            std::cerr << "\tEditInputDialog m_currentInputDir updated when it should not have. filePath = \"this/path/doesnt/exist/file.txt\"" << std::endl;
        }
        if( !testUpdateCurrentDirOK2 )
        {
            std::cerr << "\tEditInputDialog m_currentInputDir not correctly updated when it should have. filePath = \"/file.txt\"" << std::endl;
        }
        if( !testUpdateCurrentDirKO2 )
        {
            std::cerr << "\tEditInputDialog m_currentInputDir updated when it should not have. filePath = \"/home/folder/file.txt\"" << std::endl;
        }
    }
    else
    {
        std::cout << std::endl << "Test_UpdateCurrentDirEditInputDialog() PASSED" << std::endl;
    }

    app->exit();
    return ( testUpdateCurrentDirOK1 & testUpdateCurrentDirKO1 & testUpdateCurrentDirOK2 & testUpdateCurrentDirKO2 );
}



bool TestFADTTSWindow::Test_DisplayCovariatesList()
{
    int argc = 0;
    char **argv = 0;
    QApplication *app = new QApplication( argc, argv );
    QSharedPointer<FADTTSWindow> fadttsWindow = QSharedPointer<FADTTSWindow>( new FADTTSWindow );

    bool testTextCovariatesOK1 = true;
    bool testCheckStateCovariatesOK1 = true;
    bool testTextCovariatesOK2 = true;
    bool testCheckStateCovariatesOK2 = true;

    QMap<int, QString> covariateMap1;
    covariateMap1.insert( -1, "Intercept" );
    covariateMap1.insert( 1, "COMP" );
    covariateMap1.insert( 2, "Gender" );
    covariateMap1.insert( 3, "GestAgeBirth" );
    covariateMap1.insert( 4, "DaysSinceBirth" );
    covariateMap1.insert( 5, "MEDUC" );
    covariateMap1.insert( 6, "Scanner" );
    QMap<int, QString> covariateMap2;
    covariateMap2.insert( 7, "Directions" );
    covariateMap2.insert( 8, "TwinStatus" );
    covariateMap2.insert( 9, "ICV" );
    QMap<int, QString> covariateMap3;

    fadttsWindow->DisplayCovariatesList( covariateMap1 );

    bool testNbrCovariatesOK1 = fadttsWindow->m_covariatesListWidget->count() == covariateMap1.size();
    int nbrCovarTest1 = fadttsWindow->m_covariatesListWidget->count();

    QStringList covariatesDisplayed1;
    QList<Qt::CheckState> covariatesCheckState1;
    if( testNbrCovariatesOK1 )
    {
        for( int i = 0; i < fadttsWindow->m_covariatesListWidget->count(); i++ )
        {
            covariatesDisplayed1.append( fadttsWindow->m_covariatesListWidget->item( i )->text() );
            if( fadttsWindow->m_covariatesListWidget->item( i )->text() != covariateMap1.values().at( i ) )
            {
                testTextCovariatesOK1 = false;
            }
            covariatesCheckState1.append( fadttsWindow->m_covariatesListWidget->item( i )->checkState() );
            if( fadttsWindow->m_covariatesListWidget->item( i )->checkState() != Qt::Checked )
            {
                testCheckStateCovariatesOK1 = false;
            }
        }
    }

    fadttsWindow->DisplayCovariatesList( covariateMap2 );

    bool testNbrCovariatesOK2 = fadttsWindow->m_covariatesListWidget->count() == covariateMap2.size();
    int nbrCovarTest2 = fadttsWindow->m_covariatesListWidget->count();

    QStringList covariatesDisplayed2;
    QList<Qt::CheckState> covariatesCheckState2;
    if( testNbrCovariatesOK2 )
    {
        for( int i = 0; i < fadttsWindow->m_covariatesListWidget->count(); i++ )
        {
            covariatesDisplayed2.append( fadttsWindow->m_covariatesListWidget->item( i )->text() );
            if( fadttsWindow->m_covariatesListWidget->item( i )->text() != covariateMap2.values().at( i ) )
            {
                testTextCovariatesOK2 = false;
            }
            covariatesCheckState2.append( fadttsWindow->m_covariatesListWidget->item( i )->checkState() );
            if( fadttsWindow->m_covariatesListWidget->item( i )->checkState() != Qt::Checked )
            {
                testCheckStateCovariatesOK2 = false;
            }
        }
    }

    fadttsWindow->DisplayCovariatesList( covariateMap3 );

    bool testNbrCovariatesOK3 = fadttsWindow->m_covariatesListWidget->count() == covariateMap3.size();
    int nbrCovarTest3 = fadttsWindow->m_covariatesListWidget->count();

    //    testNbrCovariatesOK1 = true;testTextCovariatesOK1 = false;testCheckStateCovariatesOK1 = false;
    //    testNbrCovariatesOK2 = true;testTextCovariatesOK2 = false;testCheckStateCovariatesOK2 = false;
    //    testNbrCovariatesOK3 = true;

    if( !testNbrCovariatesOK1 || !testTextCovariatesOK1 || !testCheckStateCovariatesOK1 ||
            !testNbrCovariatesOK2 || !testTextCovariatesOK2 || !testCheckStateCovariatesOK2 ||
            !testNbrCovariatesOK3 )
    {
        std::cerr << std::endl << "Test_DisplayCovariatesList() FAILED:" << std::endl;
        if( !testNbrCovariatesOK1 || !testTextCovariatesOK1 || !testCheckStateCovariatesOK1 )
        {
            std::cerr << "\t+ Test covariateMap with 7 covariates:" << std::endl;
            if( !testNbrCovariatesOK1 )
            {
                DisplayErrorNbrCovariates( nbrCovarTest1, covariateMap1.size() );
            }
            else
            {
                if( !testTextCovariatesOK1 )
                {
                    DisplayErrorTextCovariates( covariatesDisplayed1, covariateMap1 );
                }
                if( !testCheckStateCovariatesOK1 )
                {
                    DisplayErrorCheckStateCovariates( covariatesDisplayed1, covariatesCheckState1 );
                }
            }
        }
        if( !testNbrCovariatesOK2 || !testTextCovariatesOK2 || !testCheckStateCovariatesOK2 )
        {
            std::cerr << "\t+ Test covariateMap with 3 covariates:" << std::endl;
            if( !testNbrCovariatesOK2 )
            {
                DisplayErrorNbrCovariates( nbrCovarTest2, covariateMap2.size() );
            }
            else
            {
                if( !testTextCovariatesOK2 )
                {
                    DisplayErrorTextCovariates( covariatesDisplayed2, covariateMap2 );
                }
                if( !testCheckStateCovariatesOK2 )
                {
                    DisplayErrorCheckStateCovariates( covariatesDisplayed2, covariatesCheckState2 );
                }
            }
        }
        if( !testNbrCovariatesOK3 )
        {
            std::cerr << "\t+ Test empty covariateMap:" << std::endl;
            DisplayErrorNbrCovariates( nbrCovarTest3, covariateMap3.size() );
        }
    }
    else
    {
        std::cout << std::endl << "Test_DisplayCovariatesList() PASSED" << std::endl;
    }

    app->exit();
    return ( testNbrCovariatesOK1 & testTextCovariatesOK1 & testCheckStateCovariatesOK1 &
             testNbrCovariatesOK2 & testTextCovariatesOK2 & testCheckStateCovariatesOK2 &
             testNbrCovariatesOK3 );
}



bool TestFADTTSWindow::Test_UpdateAvailableFileParamTab( QString adInputFile )
{
    int argc = 0;
    char **argv = 0;
    QApplication *app = new QApplication( argc, argv );
    QSharedPointer<FADTTSWindow> fadttsWindow = QSharedPointer<FADTTSWindow>( new FADTTSWindow );
    Processing process;

    bool testADFileLabel = true;
    bool testADFileCheckState = true;
    bool testRDFileLabel = true;
    bool testRDFileCheckState = true;
    bool testMDFileLabel = true;
    bool testMDFileCheckState = true;
    bool testFAFileLabel = true;
    bool testFAFileCheckState = true;
    bool testCOMPFileLabel = true;
    bool testCOMPFileCheckState = true;

    QList<QStringList> dataInadInputFile = process.GetDataFromFile( adInputFile );
    fadttsWindow->m_data.SetDataInFile( "ad" ) = dataInadInputFile;
    fadttsWindow->m_data.SetNbrRows( "ad" ) = 50;
    fadttsWindow->m_data.SetNbrColumns( "ad" ) = 99;

    fadttsWindow->m_data.SetNbrRows( "rd" ) = 0;
    fadttsWindow->m_data.SetNbrColumns( "rd" ) = 50;

    fadttsWindow->m_data.SetNbrRows( "md" ) = 50;
    fadttsWindow->m_data.SetNbrColumns( "md" ) = 0;

    fadttsWindow->m_data.SetNbrRows( "fa" ) = 0;
    fadttsWindow->m_data.SetNbrColumns( "fa" ) = 0;

    qDebug() << endl << "LabelMap Test" << endl << fadttsWindow->m_paramTabFileCheckBoxMap << endl;
    qDebug() << endl << "CheckBoxMap Test" << endl << fadttsWindow->m_paramTabFileSizeLabelMap << endl;


    QString fileNotAvailable = "N/A";
    QString adFileAvailable = QString::number( fadttsWindow->m_data.GetNbrRows( "ad" ) ) + "x" + QString::number( fadttsWindow->m_data.GetNbrColumns( "ad" ) );

//    qDebug() << "m_data:    " << fadttsWindow->m_data.GetDataInFileMap();

//    try
//    {
//        fadttsWindow->UpdateAvailableFileParamTab();
//    }
//    catch ( std::exception &e )
//    {
//        qFatal("Error %s sending event %s to object %s (%s)",
//               e.what(), typeid(*event).name(), qPrintable(receiver->objectName()),
//               typeid(*receiver).name());
//    }
//    catch (...)
//    {
//        qFatal("Error <unknown> sending event %s to object %s (%s)",
//               typeid(*event).name(), qPrintable(receiver->objectName()),
//               typeid(*receiver).name());
//    }



    //    QMap<QString, QCheckBox*>::ConstIterator iterCheckBox = fadttsWindow->m_paramTabFileCheckBoxMap.begin();
    //    while( iterCheckBox != fadttsWindow->m_paramTabFileCheckBoxMap.constEnd() )
    //    {
    //        if( iterCheckBox.key() == "ad" )
    //        {
    //            testADFileLabel = iterCheckBox.value()->text() == adFileAvailable;
    //            testADFileCheckState = iterCheckBox.value()->isChecked() == true;
    //        }
    //        if( iterCheckBox.key() == "rd" )
    //        {
    //            testRDFileLabel = iterCheckBox.value()->text() == fileNotAvailable;
    //            testRDFileCheckState = iterCheckBox.value()->isChecked() == false;
    //        }
    //        if( iterCheckBox.key() == "md" )
    //        {
    //            testMDFileLabel = iterCheckBox.value()->text() == fileNotAvailable;
    //            testMDFileCheckState = iterCheckBox.value()->isChecked() == false;
    //        }
    //        if( iterCheckBox.key() == "fa" )
    //        {
    //            testFAFileLabel = iterCheckBox.value()->text() == fileNotAvailable;
    //            testFAFileCheckState = iterCheckBox.value()->isChecked() == false;
    //        }
    //        if( iterCheckBox.key() == "COMP" )
    //        {
    //            testCOMPFileLabel = iterCheckBox.value()->text() == fileNotAvailable;
    //            testCOMPFileCheckState = iterCheckBox.value()->isChecked() == false;
    //        }
    //        ++iterCheckBox;
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
    //                DisplayErrorTextAvailableFile( adFileAvailable, fadttsWindow->m_paramTabFileCheckBoxMap.value( "ad" )->text() );
    //                DisplayErrorCheckStateAvailableFile( true, fadttsWindow->m_paramTabFileCheckBoxMap.value( "ad" )->isChecked() );
    //            }
    //            if( !testADFileCheckState )
    //            {

    //            }
    //        }
    //        if( !testRDFileLabel || !testRDFileCheckState )
    //        {
    //            std::cerr << "\t+ Test with nbrRows = 0 and  nbrColumns > 0:" << std::endl;
    //            if( !testRDFileLabel )
    //            {

    //            }
    //            if( !testRDFileCheckState )
    //            {

    //            }
    //        }
    //        if( !testMDFileLabel || !testMDFileCheckState )
    //        {
    //            std::cerr << "\t+ Test with nbrRows > 0 and  nbrColumns = 0:" << std::endl;
    //            if( !testMDFileLabel )
    //            {

    //            }
    //            if( !testMDFileCheckState )
    //            {

    //            }
    //        }
    //        if( !testFAFileLabel || !testFAFileCheckState )
    //        {
    //            std::cerr << "\t+ Test with nbrRows = 0 and  nbrColumns = 0:" << std::endl;
    //            if( !testFAFileLabel )
    //            {

    //            }
    //            if( !testFAFileCheckState )
    //            {

    //            }
    //        }
    //        if( !testCOMPFileLabel || !testCOMPFileCheckState )
    //        {
    //            std::cerr << "\t+ Test with no file:" << std::endl;
    //            if( !testCOMPFileLabel )
    //            {

    //            }
    //            if( !testCOMPFileCheckState )
    //            {

    //            }
    //        }
    //    }
    //    else
    //    {
    //        std::cout << std::endl << "Test_UpdateAvailableFileParamTab() PASSED" << std::endl;
    //    }

    app->exit();
    //    return( testADFileLabel & testADFileCheckState &
    //            testRDFileLabel & testRDFileCheckState &
    //            testMDFileLabel & testMDFileCheckState &
    //            testFAFileLabel & testFAFileCheckState &
    //            testCOMPFileLabel & testCOMPFileCheckState );
    return true;
}

bool TestFADTTSWindow::Test_SetInfoSubjectColumnID()
{

}

bool TestFADTTSWindow::Test_IsMatrixDimensionOK()
{

}

bool TestFADTTSWindow::Test_UpdateFileInformation()
{

}

bool TestFADTTSWindow::Test_DisplayNbrSubjectsSelected()
{

}

bool TestFADTTSWindow::Test_DisplaySortedSubjectList()
{

}

bool TestFADTTSWindow::Test_Search()
{

}

bool TestFADTTSWindow::Test_SortSubjects()
{

}

bool TestFADTTSWindow::Test_UpdateInputLineEdit()
{

}

bool TestFADTTSWindow::Test_AddFiles()
{

}


bool TestFADTTSWindow::Test_UpdateInputSubjectListLineEdit()
{

}


bool TestFADTTSWindow::Test_SelectSubject()
{

}


bool TestFADTTSWindow::Test_SelectCovariates()
{

}


bool TestFADTTSWindow::Test_UpdateOutputDirLineEdit()
{

}


bool TestFADTTSWindow::Test_GetSelectedPrefixes()
{

}

bool TestFADTTSWindow::Test_GetSelectedInputFiles()
{

}

bool TestFADTTSWindow::Test_GetSelectedCovariates()
{

}

bool TestFADTTSWindow::Test_GenerateSelectedSubjectList()
{

}


bool TestFADTTSWindow::Test_SyncUiToModelStructure()
{

}



/**********************************************************************/
/********************** Functions Used For Testing ********************/
/**********************************************************************/
void TestFADTTSWindow::DisplayErrorNbrCovariates( int nbrCovariates, int nbrCovariatesExpected )
{
    std::cerr << "\t   m_covariatesListWidget does not have the right number of covariates. " << nbrCovariates <<
                 " covariates are displayed whereas " << nbrCovariatesExpected << " were expected." << std::endl;
}

void TestFADTTSWindow::DisplayErrorTextCovariates( QStringList covariatesDisplayed, QMap<int, QString> covariateMap )
{
    std::cerr << "\t   m_covariatesListWidget does not display the right covariates" << std::endl;
    std::cerr << "\t\t- Covariates expected" << std::endl;
    for( int i = 0; i < covariateMap.size(); i++ )
    {
        std::cerr << "\t\t  \"" << covariateMap.values().at( i ).toStdString() << "\"" << std::endl;
    }
    std::cerr << "\t\t- Covariates displayed" << std::endl;
    for( int i = 0; i < covariatesDisplayed.size(); i++ )
    {
        std::cerr << "\t\t  \"" << covariatesDisplayed.at( i ).toStdString() << "\"" << std::endl;
    }
}

void TestFADTTSWindow::DisplayErrorCheckStateCovariates( QStringList covariatesDisplayed, QList<Qt::CheckState> covariatesCheckState )
{
    std::cerr << "\t   Items in m_covariatesListWidget do not have the right checkState ( 0 == unChecked and 2 == checked )" << std::endl;
    for( int i = 0; i < covariatesDisplayed.size(); i++ )
    {
        std::cerr << "\t\t- " << covariatesDisplayed.at( i ).toStdString() << " --> check state expected: 2 || check state displayed: " << covariatesCheckState.at( i ) << std::endl;
    }
}

void TestFADTTSWindow::DisplayErrorTextAvailableFile( QString expectedFileLabel, QString fileLabelDisplayed )
{
    std::cerr << "\t   m_paramTabFileCheckBoxMap does not display the right file label" << std::endl;
    std::cerr << "\t\t- label expected: " << expectedFileLabel.toStdString() << " || label displayed: " << fileLabelDisplayed.toStdString() << std::endl;
}

void TestFADTTSWindow::DisplayErrorCheckStateAvailableFile( bool checkStateExpected, bool checkStateDisplayed )
{
    std::cerr << "\t   File in m_paramTabFileCheckBoxMap does not have the right checkState ( 0 == unChecked and 2 == checked )" << std::endl;
    std::cerr << "\t\t- check state expected: " << checkStateExpected << " || check state displayed: " << checkStateDisplayed << std::endl;
}
