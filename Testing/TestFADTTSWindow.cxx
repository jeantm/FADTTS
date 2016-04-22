#include "TestFADTTSWindow.h"

//#include <QDebug>


TestFADTTSWindow::TestFADTTSWindow()
{
}


/**********************************************************************/
/*************************** Test Functions ***************************/
/**********************************************************************/

/********** Configuration && Events **********/
bool TestFADTTSWindow::Test_UpdateCurrentDir()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QString currentFileInputDir = fadttsWindow->m_currentInputFileDir;
    QString currentSubjectListInputDir = fadttsWindow->m_currentSubjectFileDir;
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


    bool testUpdateCurrentDir_Passed = testUpdateCurrentDirOK1 && testUpdateCurrentDirKO1 && testUpdateCurrentDirOK2 && testUpdateCurrentDirKO2;
    if( !testUpdateCurrentDir_Passed )
    {
        std::cerr << "/!\\/!\\ Test_UpdateCurrentDir() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with UpdateCurrentDir( const QString newfilePath, QString& currentDir )" << std::endl;
//        if( !testUpdateCurrentDirOK1 || !testUpdateCurrentDirOK2 )
//        {
//            std::cerr << "\t  m_currentInputFileDir and/or m_currentSubjectListInputDir not correctly updated when they should have" << std::endl;
//        }
//        if( !testUpdateCurrentDirKO1 || !testUpdateCurrentDirKO2 )
//        {
//            std::cerr << "\t  m_currentInputFileDir and/or m_currentSubjectListInputDir updated when they should not have" << std::endl;
//        }
    }
    else
    {
        std::cerr << "Test_UpdateCurrentDir() PASSED";
    }

    return testUpdateCurrentDir_Passed;
}

bool TestFADTTSWindow::Test_SetDir(QString paraFilePath, QString dataDir, QString tempoDir )
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );


    fadttsWindow->m_currentInputFileDir = dataDir;
    bool testUpdateCurrentDirOK1 = fadttsWindow->SetDir( paraFilePath, fadttsWindow->m_currentInputFileDir ) == dataDir;
    bool testUpdateCurrentDirKO1 = fadttsWindow->SetDir( "/wrong/path", fadttsWindow->m_currentInputFileDir ).absolutePath() == dataDir;

    fadttsWindow->m_currentInputFileDir = tempoDir;
    bool testUpdateCurrentDirOK2 = fadttsWindow->SetDir( tempoDir + "/file.csv", fadttsWindow->m_currentInputFileDir ).absolutePath() == tempoDir;
    bool testUpdateCurrentDirKO2 = fadttsWindow->SetDir( "", fadttsWindow->m_currentInputFileDir ).absolutePath() == tempoDir;


    bool testSetDir_Passed = testUpdateCurrentDirOK1 && testUpdateCurrentDirKO1 && testUpdateCurrentDirOK2 && testUpdateCurrentDirKO2;
    if( !testSetDir_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetDir() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with SetDir( QString filePath, QString currentDir )" << std::endl;
//        if( !testUpdateCurrentDirOK1 || !testUpdateCurrentDirOK2 )
//        {
//            std::cerr << "\t  m_currentInputFileDir and/or m_currentSubjectListInputDir not correctly updated when they should have" << std::endl;
//        }
//        if( !testUpdateCurrentDirKO1 || !testUpdateCurrentDirKO2 )
//        {
//            std::cerr << "\t  m_currentInputFileDir and/or m_currentSubjectListInputDir updated when they should not have" << std::endl;
//        }
    }
    else
    {
        std::cerr << "Test_SetDir() PASSED";
    }

    return testSetDir_Passed;
}

bool TestFADTTSWindow::Test_DisplayIcon( QString dataDir )
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QPixmap okPixmap = dataDir + "/okIconOut.xpm";
    QPixmap koPixmap = dataDir + "/koIconOut.xpm";


    AddIcon( fadttsWindow, dataDir );

    fadttsWindow->DisplayIcon( fadttsWindow->m_inputTabIconLabelMap.value( 0 ), okPixmap );
    bool testOKPixmap = fadttsWindow->m_inputTabIconLabelMap.value( 0 )->pixmap()->size() == QSize( 12, 12 );
    testOKPixmap = testOKPixmap && !fadttsWindow->m_inputTabIconLabelMap.value( 0 )->pixmap()->isNull();

    fadttsWindow->DisplayIcon( fadttsWindow->subjectTab_iconLoadSubjectFile_label, koPixmap );
    bool testKOPixmap = fadttsWindow->subjectTab_iconLoadSubjectFile_label->pixmap()->size() == QSize( 12, 12 );
    testKOPixmap = testKOPixmap && !fadttsWindow->subjectTab_iconLoadSubjectFile_label->pixmap()->isNull();


    bool testDisplayIcon_Passed = testOKPixmap && testKOPixmap;
    if( !testDisplayIcon_Passed )
    {
        std::cerr << "/!\\/!\\ Test_DisplayIcon() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with DisplayIcon( QLabel *label , const QPixmap icon )" << std::endl;
//        if( !testOKPixmap )
//        {
//            std::cerr << "\t  okIconOut.xpm not correctly added to label" << std::endl;
//        }
//        if( !testKOPixmap )
//        {
//            std::cerr << "\t  koIconOut.xpm not correctly added to label" << std::endl;
//        }
    }
    else
    {
        std::cerr << "Test_DisplayIcon() PASSED";
    }

    return testDisplayIcon_Passed;
}



bool TestFADTTSWindow::Test_OnLoadSaveParaSettings( QString paraFilePath, QString noParaFilePath, QString dataDir, QString tempoDir )
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QString testDir = tempoDir + "/TestFADTTSWindow/Test_OnLoadSaveSoftSettings";
    QDir().mkpath( testDir );
    bool testAllPara = true;
    bool testNoPara = true;
    bool testParaAfterSave = true;

    AddIcon( fadttsWindow, dataDir );

    testAllPara == testAllPara && fadttsWindow->para_inputTab_adFile_lineEdit->text().isEmpty();
    testAllPara == testAllPara && ( fadttsWindow->para_inputTab_rdFile_lineEdit->text() == "/NIRAL/work/jeantm/Project/FADTTS-build/data/test_RD_RawData.csv" );
    testAllPara == testAllPara && fadttsWindow->para_inputTab_mdFile_lineEdit->text().isEmpty();
    testAllPara == testAllPara && ( fadttsWindow->para_inputTab_faFile_lineEdit->text() == "/NIRAL/work/jeantm/Project/FADTTS-build/data/test_FA_RawData.csv" );
    testAllPara == testAllPara && ( fadttsWindow->para_inputTab_subMatrixFile_lineEdit->text() == "/NIRAL/work/jeantm/Project/FADTTS-build/data/test_SUBMATRIX_RawData.csv" );
    testAllPara == testAllPara && !fadttsWindow->para_subjectTab_adFile_checkBox->isChecked();
    testAllPara == testAllPara && fadttsWindow->para_subjectTab_rdFile_checkBox->isChecked();
    testAllPara == testAllPara && !fadttsWindow->para_subjectTab_mdFile_checkBox->isChecked();
    testAllPara == testAllPara && !fadttsWindow->para_subjectTab_faFile_checkBox->isChecked();
    testAllPara == testAllPara && fadttsWindow->para_subjectTab_subMatrixFile_checkBox->isChecked();
    testAllPara == testAllPara && !fadttsWindow->para_subjectTab_adFile_checkBox->isEnabled();
    testAllPara == testAllPara && fadttsWindow->para_subjectTab_rdFile_checkBox->isEnabled();
    testAllPara == testAllPara && !fadttsWindow->para_subjectTab_mdFile_checkBox->isEnabled();
    testAllPara == testAllPara && fadttsWindow->para_subjectTab_faFile_checkBox->isEnabled();
    testAllPara == testAllPara && fadttsWindow->para_subjectTab_subMatrixFile_checkBox->isEnabled();
    testAllPara == testAllPara && ( fadttsWindow->para_inputTab_covariates_listWidget->count() == 4 );
    testAllPara == testAllPara && ( fadttsWindow->para_inputTab_covariates_listWidget->item( 0 )->text() == "Intercept" );
    testAllPara == testAllPara && ( fadttsWindow->para_inputTab_covariates_listWidget->item( 0 )->checkState() == Qt::Checked );
    testAllPara == testAllPara && ( fadttsWindow->para_inputTab_covariates_listWidget->item( 1 )->text() == "ngroup" );
    testAllPara == testAllPara && ( fadttsWindow->para_inputTab_covariates_listWidget->item( 1 )->checkState() == Qt::Unchecked );
    testAllPara == testAllPara && ( fadttsWindow->para_inputTab_covariates_listWidget->item( 2 )->text() == "GENDER" );
    testAllPara == testAllPara && ( fadttsWindow->para_inputTab_covariates_listWidget->item( 2 )->checkState() == Qt::Checked );
    testAllPara == testAllPara && ( fadttsWindow->para_inputTab_covariates_listWidget->item( 3 )->text() == "GestAgeAtBirth" );
    testAllPara == testAllPara && ( fadttsWindow->para_inputTab_covariates_listWidget->item( 3 )->checkState() == Qt::Unchecked );
    testAllPara == testAllPara && ( fadttsWindow->para_executionTab_fiberName_lineEdit->text() == "testLoadSaveSettings" );
    testAllPara == testAllPara && ( fadttsWindow->para_executionTab_nbrPermutations_spinBox->value() == 500 );
    testAllPara == testAllPara && ( fadttsWindow->para_executionTab_confidenceBandsThreshold_doubleSpinBox->value() == 0.10 );
    testAllPara == testAllPara && ( fadttsWindow->para_executionTab_pvalueThreshold_doubleSpinBox->value() == 0.04 );
    testAllPara == testAllPara && fadttsWindow->para_executionTab_omnibus_checkBox->isChecked();
    testAllPara == testAllPara && !fadttsWindow->para_executionTab_postHoc_checkBox->isChecked();
    testAllPara == testAllPara && ( fadttsWindow->para_executionTab_outputDir_lineEdit->text() == "/NIRAL/work/jeantm/Project/FADTTS-build/data/" );
//    testAllPara == testAllPara && fadttsWindow->para_executionTab_matlabExe_checkBox->isChecked();


    testNoPara == testNoPara && fadttsWindow->para_inputTab_adFile_lineEdit->text().isEmpty();
    testNoPara == testNoPara && fadttsWindow->para_inputTab_rdFile_lineEdit->text().isEmpty();
    testNoPara == testNoPara && fadttsWindow->para_inputTab_mdFile_lineEdit->text().isEmpty();
    testNoPara == testNoPara && fadttsWindow->para_inputTab_faFile_lineEdit->text().isEmpty();
    testNoPara == testNoPara && fadttsWindow->para_inputTab_subMatrixFile_lineEdit->text().isEmpty();
    testNoPara == testNoPara && !fadttsWindow->para_subjectTab_adFile_checkBox->isChecked();
    testNoPara == testNoPara && !fadttsWindow->para_subjectTab_rdFile_checkBox->isChecked();
    testNoPara == testNoPara && !fadttsWindow->para_subjectTab_mdFile_checkBox->isChecked();
    testNoPara == testNoPara && !fadttsWindow->para_subjectTab_faFile_checkBox->isChecked();
    testNoPara == testNoPara && !fadttsWindow->para_subjectTab_subMatrixFile_checkBox->isChecked();
    testNoPara == testNoPara && !fadttsWindow->para_subjectTab_adFile_checkBox->isEnabled();
    testNoPara == testNoPara && !fadttsWindow->para_subjectTab_rdFile_checkBox->isEnabled();
    testNoPara == testNoPara && !fadttsWindow->para_subjectTab_mdFile_checkBox->isEnabled();
    testNoPara == testNoPara && !fadttsWindow->para_subjectTab_faFile_checkBox->isEnabled();
    testNoPara == testNoPara && !fadttsWindow->para_subjectTab_subMatrixFile_checkBox->isEnabled();
    testNoPara == testNoPara && ( fadttsWindow->para_inputTab_covariates_listWidget->count() == 0 );
    testNoPara == testNoPara && fadttsWindow->para_executionTab_fiberName_lineEdit->text().isEmpty();
    testNoPara == testNoPara && ( fadttsWindow->para_executionTab_nbrPermutations_spinBox->value() == 100 );
    testNoPara == testNoPara && ( fadttsWindow->para_executionTab_confidenceBandsThreshold_doubleSpinBox->value() == 0.05 );
    testNoPara == testNoPara && ( fadttsWindow->para_executionTab_pvalueThreshold_doubleSpinBox->value() == 0.05 );
    testNoPara == testNoPara && !fadttsWindow->para_executionTab_omnibus_checkBox->isChecked();
    testNoPara == testNoPara && !fadttsWindow->para_executionTab_postHoc_checkBox->isChecked();
    testNoPara == testNoPara && fadttsWindow->para_executionTab_outputDir_lineEdit->text().isEmpty();
//    testNoPara == testNoPara && !fadttsWindow->para_executionTab_matlabExe_checkBox->isChecked();

    testParaAfterSave == testParaAfterSave && fadttsWindow->para_inputTab_adFile_lineEdit->text().isEmpty();
    testParaAfterSave == testParaAfterSave && ( fadttsWindow->para_inputTab_rdFile_lineEdit->text() == "/NIRAL/work/jeantm/Project/FADTTS-build/data/test_RD_RawData.csv" );
    testParaAfterSave == testParaAfterSave && fadttsWindow->para_inputTab_mdFile_lineEdit->text().isEmpty();
    testParaAfterSave == testParaAfterSave && ( fadttsWindow->para_inputTab_faFile_lineEdit->text() == "/NIRAL/work/jeantm/Project/FADTTS-build/data/test_FA_RawData.csv" );
    testParaAfterSave == testParaAfterSave && ( fadttsWindow->para_inputTab_subMatrixFile_lineEdit->text() == "/NIRAL/work/jeantm/Project/FADTTS-build/data/test_SUBMATRIX_RawData.csv" );
    testParaAfterSave == testParaAfterSave && !fadttsWindow->para_subjectTab_adFile_checkBox->isChecked();
    testParaAfterSave == testParaAfterSave && fadttsWindow->para_subjectTab_rdFile_checkBox->isChecked();
    testParaAfterSave == testParaAfterSave && !fadttsWindow->para_subjectTab_mdFile_checkBox->isChecked();
    testParaAfterSave == testParaAfterSave && !fadttsWindow->para_subjectTab_faFile_checkBox->isChecked();
    testParaAfterSave == testParaAfterSave && fadttsWindow->para_subjectTab_subMatrixFile_checkBox->isChecked();
    testParaAfterSave == testParaAfterSave && !fadttsWindow->para_subjectTab_adFile_checkBox->isEnabled();
    testParaAfterSave == testParaAfterSave && fadttsWindow->para_subjectTab_rdFile_checkBox->isEnabled();
    testParaAfterSave == testParaAfterSave && !fadttsWindow->para_subjectTab_mdFile_checkBox->isEnabled();
    testParaAfterSave == testParaAfterSave && fadttsWindow->para_subjectTab_faFile_checkBox->isEnabled();
    testParaAfterSave == testParaAfterSave && fadttsWindow->para_subjectTab_subMatrixFile_checkBox->isEnabled();
    testParaAfterSave == testParaAfterSave && ( fadttsWindow->para_inputTab_covariates_listWidget->count() == 4 );
    testParaAfterSave == testParaAfterSave && ( fadttsWindow->para_inputTab_covariates_listWidget->item( 0 )->text() == "Intercept" );
    testParaAfterSave == testParaAfterSave && ( fadttsWindow->para_inputTab_covariates_listWidget->item( 0 )->checkState() == Qt::Checked );
    testParaAfterSave == testParaAfterSave && ( fadttsWindow->para_inputTab_covariates_listWidget->item( 1 )->text() == "ngroup" );
    testParaAfterSave == testParaAfterSave && ( fadttsWindow->para_inputTab_covariates_listWidget->item( 1 )->checkState() == Qt::Unchecked );
    testParaAfterSave == testParaAfterSave && ( fadttsWindow->para_inputTab_covariates_listWidget->item( 2 )->text() == "GENDER" );
    testParaAfterSave == testParaAfterSave && ( fadttsWindow->para_inputTab_covariates_listWidget->item( 2 )->checkState() == Qt::Checked );
    testParaAfterSave == testParaAfterSave && ( fadttsWindow->para_inputTab_covariates_listWidget->item( 3 )->text() == "GestAgeAtBirth" );
    testParaAfterSave == testParaAfterSave && ( fadttsWindow->para_inputTab_covariates_listWidget->item( 3 )->checkState() == Qt::Unchecked );
    testParaAfterSave == testParaAfterSave && ( fadttsWindow->para_executionTab_fiberName_lineEdit->text() == "testLoadSaveSettings" );
    testParaAfterSave == testParaAfterSave && ( fadttsWindow->para_executionTab_nbrPermutations_spinBox->value() == 500 );
    testParaAfterSave == testParaAfterSave && ( fadttsWindow->para_executionTab_confidenceBandsThreshold_doubleSpinBox->value() == 0.10 );
    testParaAfterSave == testParaAfterSave && ( fadttsWindow->para_executionTab_pvalueThreshold_doubleSpinBox->value() == 0.04 );
    testParaAfterSave == testParaAfterSave && fadttsWindow->para_executionTab_omnibus_checkBox->isChecked();
    testParaAfterSave == testParaAfterSave && !fadttsWindow->para_executionTab_postHoc_checkBox->isChecked();
    testParaAfterSave == testParaAfterSave && ( fadttsWindow->para_executionTab_outputDir_lineEdit->text() == "/NIRAL/work/jeantm/Project/FADTTS-build/data/" );
//    testParaAfterSave == testParaAfterSave && fadttsWindow->para_executionTab_matlabExe_checkBox->isChecked();


    bool testOnLoadSaveParaSettings_Passed = testAllPara && testNoPara && testParaAfterSave;
    if( !testOnLoadSaveParaSettings_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnLoadSaveParaSettings() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnLoadParaSettings() and/or OnSaveParaSettings()" << std::endl;
    }
    else
    {
        std::cerr << "Test_OnLoadSaveParaSettings() PASSED";
    }

    return testOnLoadSaveParaSettings_Passed;
}

bool TestFADTTSWindow::Test_OnLoadSaveSoftSettings( QString softFilePath, QString noSoftFilePath, QString dataDir, QString tempoDir )
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QString testDir = tempoDir + "/TestFADTTSWindow/Test_OnLoadSaveSoftSettings";
    QDir().mkpath( testDir );
    bool testAllSoft = true;
    bool testNoSoft = true;
    bool testSoftAfterSave = true;


//    testAllSoft = testAllSoft && ( fadttsWindow->soft_executionTab_mvcm_lineEdit->text() == "/devel/linux/FADTTS/FADTTS_V3.01_NIRAL/FADTTS/FADTTS" );
    testAllSoft = testAllSoft && ( fadttsWindow->soft_executionTab_matlabExe_lineEdit->text() == "/NIRAL/tools/matlab7p4_linux64/bin/matlab" );
//    testAllSoft = testAllSoft && !fadttsWindow->soft_executionTab_runMatlabSystem_radioButton->isChecked();
//    testAllSoft = testAllSoft && fadttsWindow->soft_executionTab_runMatlabKD_radioButton->isChecked();
    testAllSoft = testAllSoft && ( fadttsWindow->soft_executionTab_nbrCompThreads_spinBox->value() == 4 );
//    testAllSoft = testAllSoft && ( fadttsWindow->soft_executionTab_killDevilQueue_comboBox->currentText() == "Week (7 days)" );
//    testAllSoft = testAllSoft && ( fadttsWindow->soft_executionTab_killDevilAllocatedMemory_spinBox->value() == 9 );


//    testNoSoft = testNoSoft && fadttsWindow->soft_executionTab_mvcm_lineEdit->text().isEmpty();
    testNoSoft = testNoSoft && fadttsWindow->soft_executionTab_matlabExe_lineEdit->text().isEmpty();
//    testNoSoft = testNoSoft && fadttsWindow->soft_executionTab_runMatlabSystem_radioButton->isChecked();
//    testNoSoft = testNoSoft && !fadttsWindow->soft_executionTab_runMatlabKD_radioButton->isChecked();
    testNoSoft = testNoSoft && ( fadttsWindow->soft_executionTab_nbrCompThreads_spinBox->value() == 1 );
//    testNoSoft = testNoSoft && ( fadttsWindow->soft_executionTab_killDevilQueue_comboBox->currentText() == "Hour (60 mins)" );
//    testNoSoft = testNoSoft && ( fadttsWindow->soft_executionTab_killDevilAllocatedMemory_spinBox->value() == 4 );

//    testSoftAfterSave = testSoftAfterSave && ( fadttsWindow->soft_executionTab_mvcm_lineEdit->text() == "/devel/linux/FADTTS/FADTTS_V3.01_NIRAL/FADTTS/FADTTS" );
    testSoftAfterSave = testSoftAfterSave && ( fadttsWindow->soft_executionTab_matlabExe_lineEdit->text() == "/NIRAL/tools/matlab7p4_linux64/bin/matlab" );
//    testSoftAfterSave = testSoftAfterSave && !fadttsWindow->soft_executionTab_runMatlabSystem_radioButton->isChecked();
//    testSoftAfterSave = testSoftAfterSave && fadttsWindow->soft_executionTab_runMatlabKD_radioButton->isChecked();
    testSoftAfterSave = testSoftAfterSave && ( fadttsWindow->soft_executionTab_nbrCompThreads_spinBox->value() == 4 );
//    testSoftAfterSave = testSoftAfterSave && ( fadttsWindow->soft_executionTab_killDevilQueue_comboBox->currentText() == "Week (7 days)" );
//    testSoftAfterSave = testSoftAfterSave && ( fadttsWindow->soft_executionTab_killDevilAllocatedMemory_spinBox->value() == 9 );


    bool testOnLoadSaveSoftSettings_Passed = testAllSoft && testNoSoft && testSoftAfterSave;
    if( !testOnLoadSaveSoftSettings_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnLoadSaveSoftSettings() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnLoadSoftSettings() and/or OnSaveSoftSettings()" << std::endl;
    }
    else
    {
        std::cerr << "Test_OnLoadSaveSoftSettings() PASSED";
    }

    return testOnLoadSaveSoftSettings_Passed;
}



/************** Subjects  Tab ***************/
bool TestFADTTSWindow::Test_OnApplyingQCThreshold()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList matchedSubjectList = QStringList() << "Marcus Paige" << "Brice Johnson" << "Joel Berry II" << "Kennedy Meeks" << "Justin Jackson";
    QStringList subjectsCorrelated = QStringList() << "Marcus Paige" << "Brice Johnson";
    QStringList subjectsNotCorrelated = QStringList() << "Joel Berry II" << "Kennedy Meeks" << "Justin Jackson";
    QStringList checkedMatchedSubjects;
    double qcThreshold = 0.74;


    fadttsWindow->DisplaySortedSubjects( matchedSubjectList, QMap< QString, QList< int > >() );
    fadttsWindow->OnApplyingQCThreshold( subjectsCorrelated, subjectsNotCorrelated, qcThreshold );

    bool testFailedQCThresholdSubjects = fadttsWindow->m_failedQCThresholdSubjects == subjectsNotCorrelated;

    for( int i = 0; i < fadttsWindow->m_matchedSubjectListWidget->count(); i++ )
    {
        QListWidgetItem *currentItem = fadttsWindow->m_matchedSubjectListWidget->item( i );
        if( currentItem->checkState() == Qt::Checked )
        {
            checkedMatchedSubjects.append( currentItem->text() );
        }
    }
    bool testCheckedMatchedSubjects = checkedMatchedSubjects == subjectsCorrelated;

    bool testQCThreshold = fadttsWindow->m_qcThreshold == qcThreshold;


    bool testOnApplyingQCThreshold_Passed = testFailedQCThresholdSubjects && testCheckedMatchedSubjects && testQCThreshold;
    if( !testOnApplyingQCThreshold_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnApplyingQCThreshold() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnApplyingQCThreshold( const QStringList& subjectsCorrelated, const QStringList& subjectsNotCorrelated, double qcThreshold )" << std::endl;
//        if( testFailedQCThresholdSubjects )
//        {
//            std::cerr << "\t  wrong m_failedQCThresholdSubjects" << std::endl;
//            DisplayError_QStringList( subjectsNotCorrelated, fadttsWindow->m_failedQCThresholdSubjects, "subjects who failed the qc threshold" );
//        }
//        if( testCheckedMatchedSubjects )
//        {
//            std::cerr << "\t  wrong checked subjects" << std::endl;
//            DisplayError_QStringList( subjectsCorrelated, checkedMatchedSubjects, "checked subjects" );
//        }
//        if( testQCThreshold )
//        {
//            std::cerr << "\t  wrong m_qcThreshold" << std::endl;
//        }
    }
    else
    {
        std::cerr << "Test_OnApplyingQCThreshold() PASSED";
    }

    return testOnApplyingQCThreshold_Passed;
}

bool TestFADTTSWindow::Test_SetSelectedInputFiles()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QMap< int, QString > expectedPropertySelected;
    expectedPropertySelected.insert( 0, "ad" );
    expectedPropertySelected.insert( 2, "md" );
    expectedPropertySelected.insert( 4, "subMatrix" );
    QMap< int, QString > expectedSelectedFiles;
    expectedSelectedFiles.insert( 0, "/path/adFile.csv" );
    expectedSelectedFiles.insert( 2, "/path/mdFile.csv" );
    expectedSelectedFiles.insert( 4, "/path/subMatrixFile.csv" );


    fadttsWindow->m_data.SetFilename( 0 ) = "/path/adFile.csv";
    fadttsWindow->m_data.SetFilename( 1 ) = "/path/rdFile.csv";
    fadttsWindow->m_data.SetFilename( 2 ) = "/path/mdFile.csv";
    fadttsWindow->m_data.SetFilename( 4 ) = "/path/subMatrixFile.csv";
    fadttsWindow->m_paramTabFileCheckBoxMap.value( 0 )->setChecked( true );
    fadttsWindow->m_paramTabFileCheckBoxMap.value( 1 )->setChecked( false );
    fadttsWindow->m_paramTabFileCheckBoxMap.value( 2 )->setChecked( true );
    fadttsWindow->m_paramTabFileCheckBoxMap.value( 4 )->setChecked( true );

    fadttsWindow->SetSelectedInputFiles();

    bool testPropertySelected = fadttsWindow->m_propertySelected == expectedPropertySelected;
    bool testSelectedFiles = fadttsWindow->m_selectedFiles == expectedSelectedFiles;


    bool testSetSelectedInputFiles_Passed = testPropertySelected && testSelectedFiles;
    if( !testSetSelectedInputFiles_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetSelectedInputFiles() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with SetSelectedInputFiles()" << std::endl;
//        if( !testPropertySelected )
//        {
//            std::cerr << "\t    Wrong properties selected" << std::endl;
//            DisplayError_QMapIntQString( expectedPropertySelected, fadttsWindow->m_propertySelected, "m_propertySelected" );
//        }
//        if( !testSelectedFiles )
//        {
//            std::cerr << "\t    Wrong properties selected" << std::endl;
//            DisplayError_QMapIntQString( expectedSelectedFiles, fadttsWindow->m_selectedFiles, "m_selectedFiles" );
//        }
    }
    else
    {
        std::cerr << "Test_SetSelectedInputFiles() PASSED";
    }

    return testSetSelectedInputFiles_Passed;
}

bool TestFADTTSWindow::Test_UpdateAvailableDiffusionProperties()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );


    fadttsWindow->m_data.SetNbrRows( 0 ) = 50;
    fadttsWindow->m_data.SetNbrColumns( 0 ) = 99;
    fadttsWindow->m_data.SetNbrSubjects( 0 ) = 99;
    fadttsWindow->m_data.SetNbrRows( 1 ) = 0;
    fadttsWindow->m_data.SetNbrColumns( 1 ) = 50;
    fadttsWindow->m_data.SetNbrSubjects( 1 ) = 0;
    fadttsWindow->m_data.SetNbrRows( 2 ) = 50;
    fadttsWindow->m_data.SetNbrColumns( 2 ) = 0;
    fadttsWindow->m_data.SetNbrSubjects( 2 ) = 0;
    fadttsWindow->m_data.SetNbrRows( 3 ) = 0;
    fadttsWindow->m_data.SetNbrColumns( 3 ) = 0;
    fadttsWindow->m_data.SetNbrSubjects( 3 ) = 0;
    fadttsWindow->InitAvailableDiffusionProperties();

    bool testADFileCheckState = fadttsWindow->m_paramTabFileCheckBoxMap.value( 0 )->isChecked();
    bool testRDFileCheckState = !fadttsWindow->m_paramTabFileCheckBoxMap.value( 1 )->isChecked();
    bool testMDFileCheckState = !fadttsWindow->m_paramTabFileCheckBoxMap.value( 2 )->isChecked();
    bool testFAFileCheckState = !fadttsWindow->m_paramTabFileCheckBoxMap.value( 3 )->isChecked();
    bool testSubMatrixFileCheckState = !fadttsWindow->m_paramTabFileCheckBoxMap.value( 4 )->isChecked();


    bool testUpdateAvailableDiffusionProperties_Passed = testADFileCheckState && testRDFileCheckState && testMDFileCheckState
            && testFAFileCheckState && testSubMatrixFileCheckState;
    if( !testUpdateAvailableDiffusionProperties_Passed )
    {
        std::cerr << "/!\\/!\\ Test_UpdateAvailableDiffusionProperties() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with UpdateAvailableDiffusionProperties()" << std::endl;
//        if( !testADFileLabel || !testADFileCheckState )
//        {
//            std::cerr << "\t  Test with nbrRows > 0 and nbrColumns > 0" << std::endl;
//            if( !testADFileLabel )
//            {
//                std::cerr << "\t    Wrong info" << std::endl;
//                DisplayError_QString( adFileAvailable, fadttsWindow->m_paramTabFileDataSizeLabelMap.value( 0 )->text(), "info" );
//            }
//            if( !testADFileCheckState )
//            {
//                std::cerr << "\t    Wrong checkState" << std::endl;
//                DisplayError_QString( "TRUE", fadttsWindow->m_paramTabFileCheckBoxMap.value( 0 )->isChecked() ? "TRUE" : "FALSE", "checkState" );
//            }
//        }
//        if( !testRDFileLabel || !testRDFileCheckState )
//        {
//            std::cerr << "\n\t  Test with nbrRows = 0 and nbrColumns > 0" << std::endl;
//            if( !testRDFileLabel )
//            {
//                std::cerr << "\t    Wrong info" << std::endl;
//                DisplayError_QString( fileNotAvailable, fadttsWindow->m_paramTabFileDataSizeLabelMap.value( 1 )->text(), "info" );
//            }
//            if( !testRDFileCheckState )
//            {
//                std::cerr << "\t    Wrong checkState" << std::endl;
//                DisplayError_QString( "FALSE", fadttsWindow->m_paramTabFileCheckBoxMap.value( 1 )->isChecked() ? "TRUE" : "FALSE", "checkState" );
//            }
//        }
//        if( !testMDFileLabel || !testMDFileCheckState )
//        {
//            std::cerr << "\n\t  Test with nbrRows > 0 and nbrColumns = 0" << std::endl;
//            if( !testMDFileLabel )
//            {
//                std::cerr << "\t    Wrong info" << std::endl;
//                DisplayError_QString( fileNotAvailable, fadttsWindow->m_paramTabFileDataSizeLabelMap.value( 2 )->text(), "info" );
//            }
//            if( !testMDFileCheckState )
//            {
//                std::cerr << "\t    Wrong checkState" << std::endl;
//                DisplayError_QString( "FALSE", fadttsWindow->m_paramTabFileCheckBoxMap.value( 2 )->isChecked() ? "TRUE" : "FALSE", "checkState" );
//            }
//        }
//        if( !testFAFileLabel || !testFAFileCheckState )
//        {
//            std::cerr << "\n\t  Test with nbrRows = 0 and nbrColumns = 0" << std::endl;
//            if( !testFAFileLabel )
//            {
//                std::cerr << "\t    Wrong info" << std::endl;
//                DisplayError_QString( fileNotAvailable, fadttsWindow->m_paramTabFileDataSizeLabelMap.value( 3 )->text(), "info" );
//            }
//            if( !testFAFileCheckState )
//            {
//                std::cerr << "\t    Wrong checkState" << std::endl;
//                DisplayError_QString( "FALSE", fadttsWindow->m_paramTabFileCheckBoxMap.value( 3 )->isChecked() ? "TRUE" : "FALSE", "checkState" );
//            }
//        }
//        if( !testSubMatrixFileLabel || !testSubMatrixFileCheckState )
//        {
//            std::cerr << "\n\t  Test with no file" << std::endl;
//            if( !testSubMatrixFileLabel )
//            {
//                std::cerr << "\t    Wrong info" << std::endl;
//                DisplayError_QString( fileNotAvailable, fadttsWindow->m_paramTabFileDataSizeLabelMap.value( 4 )->text(), "info" );
//            }
//            if( !testSubMatrixFileCheckState )
//            {
//                std::cerr << "\t    Wrong checkState" << std::endl;
//                DisplayError_QString( "FALSE", fadttsWindow->m_paramTabFileCheckBoxMap.value( 4 )->isChecked() ? "TRUE" : "FALSE", "checkState" );
//            }
//        }
    }
    else
    {
        std::cerr << "Test_UpdateAvailableDiffusionProperties() PASSED";
    }

    return testUpdateAvailableDiffusionProperties_Passed;
}

bool TestFADTTSWindow::Test_GetDiffusionPropertiesCheckState()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QMap< int, bool > expectedDiffusionPropertyCheckStatus;
    expectedDiffusionPropertyCheckStatus.insert( 0, true );
    expectedDiffusionPropertyCheckStatus.insert( 1, false );
    expectedDiffusionPropertyCheckStatus.insert( 2, false );
    expectedDiffusionPropertyCheckStatus.insert( 3, true );
    expectedDiffusionPropertyCheckStatus.insert( 4, false );


    fadttsWindow->m_paramTabFileCheckBoxMap.value( 0 )->setChecked( true );
    fadttsWindow->m_paramTabFileCheckBoxMap.value( 1 )->setChecked( false );
    fadttsWindow->m_paramTabFileCheckBoxMap.value( 2 )->setChecked( false );
    fadttsWindow->m_paramTabFileCheckBoxMap.value( 3 )->setChecked( true );
    fadttsWindow->m_paramTabFileCheckBoxMap.value( 4 )->setChecked( false );

    bool testGetDiffusionPropertiesCheckState_Passed = fadttsWindow->GetDiffusionPropertiesCheckState() == expectedDiffusionPropertyCheckStatus;


    if( !testGetDiffusionPropertiesCheckState_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetDiffusionPropertiesCheckState() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with GetDiffusionPropertiesCheckState()" << std::endl;
    }
    else
    {
        std::cerr << "Test_GetDiffusionPropertiesCheckState() PASSED";
    }

    return testGetDiffusionPropertiesCheckState_Passed;
}

bool TestFADTTSWindow::Test_DisplaySubjectInformation()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList matchedSubjects = QStringList() << "Stan" << "Kyle" << "Kenny" << "Cartman" << "Chef" << "Butters" << "Wendy" << "Craig" << "Pip";
    QStringList unMatchedSubjects = QStringList() << "Yoda" << "Luke" << "Darth Vader" << "Han Solo" << "Chewbacca" << "R2D2" << "C3PO" << "Obi-Wan" << "Leila";


    foreach( QString subject, matchedSubjects )
    {
        QListWidgetItem *item = new QListWidgetItem( subject, fadttsWindow->m_matchedSubjectListWidget );
        fadttsWindow->m_matchedSubjectListWidget->addItem( item );
    }
    foreach( QString subject, unMatchedSubjects )
    {
        QListWidgetItem *item = new QListWidgetItem( subject, fadttsWindow->m_unmatchedSubjectListWidget );
        fadttsWindow->m_matchedSubjectListWidget->addItem( item );
    }
    fadttsWindow->DisplaySubjectInformation();
    bool testMatchedSubjectsUnMatchedSubjects = fadttsWindow->subjectTab_matchedSubjectsInformation_label->text() == "9/18 matched"
            && fadttsWindow->subjectTab_unmatchedSubjectsInformation_label->text() == "9/18 unmatched";

    fadttsWindow->m_matchedSubjectListWidget->clear();
    fadttsWindow->DisplaySubjectInformation();
    bool testUnMatchedSubjects = fadttsWindow->subjectTab_matchedSubjectsInformation_label->text() == "WARNING No matched! 0/9"
            && fadttsWindow->subjectTab_unmatchedSubjectsInformation_label->text() == "9/9 unmatched";

    fadttsWindow->m_unmatchedSubjectListWidget->clear();
    fadttsWindow->DisplaySubjectInformation();
    bool testNoSubject = fadttsWindow->subjectTab_matchedSubjectsInformation_label->text().isEmpty()
            && fadttsWindow->subjectTab_unmatchedSubjectsInformation_label->text().isEmpty();

    foreach( QString subject, matchedSubjects )
    {
        QListWidgetItem *item = new QListWidgetItem( subject, fadttsWindow->m_matchedSubjectListWidget );
        fadttsWindow->m_matchedSubjectListWidget->addItem( item );
    }
    fadttsWindow->DisplaySubjectInformation();
    bool testMatchedSubjects = fadttsWindow->subjectTab_matchedSubjectsInformation_label->text() == "All subjects matched 9/9"
            && fadttsWindow->subjectTab_unmatchedSubjectsInformation_label->text() == "";


    bool testDisplaySubjectInformation_Passed = testMatchedSubjectsUnMatchedSubjects && testUnMatchedSubjects && testMatchedSubjects && testNoSubject;
    if( !testDisplaySubjectInformation_Passed )
    {
//        std::cerr << "/!\\/!\\ Test_DisplaySubjectInformation() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with DisplaySubjectInformation()" << std::endl;
//        if( !testMatchedSubjectsUnMatchedSubjects )
//        {
//            std::cerr << "\t    When matched and unmatched subjects" << std::endl;
//        }
//        if( !testUnMatchedSubjects )
//        {
//            std::cerr << "\t    When unmatched subjects only" << std::endl;
//        }
//        if( !testMatchedSubjects )
//        {
//            std::cerr << "\t    When matched subjects only" << std::endl;
//        }
//        if( !testNoSubject )
//        {
//            std::cerr << "\t    When no subject" << std::endl;
//        }
    }
    else
    {
        std::cerr << "Test_DisplaySubjectInformation() PASSED";
    }

    return testDisplaySubjectInformation_Passed;
}

bool TestFADTTSWindow::Test_DisplayNbrSubjectSelected()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList matchedSubjects = QStringList() << "Stan" << "Kyle" << "Kenny" << "Cartman" << "Chef" << "Butters" << "Wendy" << "Craig" << "Pip";


    foreach( QString subject, matchedSubjects )
    {
        QListWidgetItem *item = new QListWidgetItem( subject, fadttsWindow->m_matchedSubjectListWidget );
        item->setCheckState( Qt::Checked );
        fadttsWindow->m_matchedSubjectListWidget->addItem( item );
    }
    fadttsWindow->DisplayNbrSubjectSelected();
    bool testAllSubjects = fadttsWindow->subjectTab_checkedSubjectsInformation_label->text() == "9 subjects selected";

    fadttsWindow->m_matchedSubjectListWidget->item( 1 )->setCheckState( Qt::Unchecked );
    bool testSubjects = fadttsWindow->subjectTab_checkedSubjectsInformation_label->text() == "8 subjects selected";

    fadttsWindow->m_matchedSubjectListWidget->clear();
    fadttsWindow->DisplayNbrSubjectSelected();
    bool testNoSubjects = fadttsWindow->subjectTab_checkedSubjectsInformation_label->text().isEmpty();


    bool testDisplayNbrSubjectSelected_Passed = testAllSubjects && testAllSubjects && testNoSubjects;
    if( !testDisplayNbrSubjectSelected_Passed )
    {
        std::cerr << "/!\\/!\\ Test_DisplayNbrSubjectSelected() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with DisplayNbrSubjectSelected()" << std::endl;
    }
    else
    {
        std::cerr << "Test_DisplayNbrSubjectSelected() PASSED";
    }

    return testDisplayNbrSubjectSelected_Passed;
}

bool TestFADTTSWindow::Test_DisplaySortedSubjects()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList matchedSubjects = QStringList() << "Stan" << "Kyle" << "Kenny" << "Cartman" << "Chef" << "Butters" << "Wendy" << "Craig" << "Pip";
    QList< int > inYoda = QList< int >() << -1 << 1 << 2 << 3 << 4;
    QList< int > inLuke = QList< int >() << 0 << 1 << 2 << 3;
    QList< int > inDarthVader = QList< int >() << -1 << 1 << 2;
    QList< int > inHanSolo = QList< int >() << 0 << 1;
    QList< int > inChewbacca = QList< int >() << 0;
    QMap< QString, QList< int > > unMatchedSubjects;
    unMatchedSubjects.insert( "Chewbacca", inChewbacca );
    unMatchedSubjects.insert( "Darth Vader", inDarthVader );
    unMatchedSubjects.insert( "Han Solo", inHanSolo );
    unMatchedSubjects.insert( "Luke", inLuke );
    unMatchedSubjects.insert( "Yoda", inYoda );
    QList< QStringList > expectedTextUnmatchedSubjects = QList< QStringList >()
            << ( QStringList() << "ad" )
            << ( QStringList() << "loaded subjects" << "rd" << "md" )
            << ( QStringList() << "ad" << "rd" )
            << ( QStringList() << "ad" << "rd" << "md" << "fa" )
            << ( QStringList() << "loaded subjects" << "rd" << "md" << "fa" << "subMatrix" );
    int expectedNbrUnmatchedSubjects = unMatchedSubjects.size();
    int expectedNbrMatchedSubjects = matchedSubjects.size();


    /************************************************/
    /************* All Subjects Matched *************/
    /************************************************/
    int nbrMatchedSubjects1;
    int nbrUnmatchedSubjects1;
    bool testCheckState1 = true;
    bool testBackgroundColorSubjectsMatched1 = true;
    bool testFlagsSubjectsMatched1 = true;


    fadttsWindow->DisplaySortedSubjects( matchedSubjects, QMap< QString, QList< int > >() );

    nbrMatchedSubjects1 = fadttsWindow->m_matchedSubjectListWidget->count();
    bool testSizeListWidgetMatchedSubjects1 = nbrMatchedSubjects1 == expectedNbrMatchedSubjects;

    nbrUnmatchedSubjects1 = fadttsWindow->m_unmatchedSubjectListWidget->count();
    bool testSizeListWidgetUnmatchedSubjects1 = nbrUnmatchedSubjects1 == 0;

    for( int i = 0; i < expectedNbrMatchedSubjects; i++ )
    {
        QListWidgetItem *item = fadttsWindow->m_matchedSubjectListWidget->item( i );
        if( item->checkState() != Qt::Checked )
        {
            testCheckState1 = false;
        }
        if( item->backgroundColor() != fadttsWindow->m_green )
        {
            testBackgroundColorSubjectsMatched1 = false;
        }
        if( item->flags() != Qt::ItemIsEnabled )
        {
            testFlagsSubjectsMatched1 = false;
        }
    }

    QString expectedMatchedSubjectsInformation1 = "All subjects matched 9/9";
    QString matchedSubjectsInformationDisplayed1 = fadttsWindow->subjectTab_matchedSubjectsInformation_label->text();
    bool testMatchedSubjectsInformation1 = expectedMatchedSubjectsInformation1 == matchedSubjectsInformationDisplayed1;

    QString expectedUnmatchedSubjectsInformation1 = "";
    QString unmatchedSubjectsInformationDisplayed1 = fadttsWindow->subjectTab_unmatchedSubjectsInformation_label->text();
    bool testUnmatchedSubjectsInformation1 = expectedUnmatchedSubjectsInformation1 == unmatchedSubjectsInformationDisplayed1;


    /************************************************/
    /******** Matched and Unmatched Subjects ********/
    /************************************************/
    int nbrMatchedSubjects2;
    int nbrUnmatchedSubjects2;
    bool testCheckState2 = true;
    bool testBackgroundColorSubjectsMatched2 = true;
    bool testFlagsSubjectsMatched2 = true;
    bool testTextUnmatchedSubjects2 = true;
    bool testBackgroundColorSubjectsUnmatched2 = true;
    bool testFlagsSubjectsUnmatched2 = true;


    fadttsWindow->DisplaySortedSubjects( matchedSubjects, unMatchedSubjects );

    nbrMatchedSubjects2 = fadttsWindow->m_matchedSubjectListWidget->count();
    bool testSizeListWidgetMatchedSubjects2 = nbrMatchedSubjects2 == expectedNbrMatchedSubjects;

    nbrUnmatchedSubjects2 = fadttsWindow->m_unmatchedSubjectListWidget->count();
    bool testSizeListWidgetUnmatchedSubjects2 = nbrUnmatchedSubjects2 == expectedNbrUnmatchedSubjects;

    for( int i = 0; i < expectedNbrMatchedSubjects; i++ )
    {
        QListWidgetItem *item = fadttsWindow->m_matchedSubjectListWidget->item( i );
        if( item->checkState() != Qt::Checked )
        {
            testCheckState2 = false;
        }
        if( item->backgroundColor() != fadttsWindow->m_green )
        {
            testBackgroundColorSubjectsMatched2 = false;
        }
        if( item->flags() != Qt::ItemIsEnabled )
        {
            testFlagsSubjectsMatched2 = false;
        }
    }

    QMap< QString, QList< int > >::ConstIterator iterUnmatchedSubjects = unMatchedSubjects.begin();
    for( int i = 0; i < expectedNbrUnmatchedSubjects; i++ )
    {
        QString expectedText = iterUnmatchedSubjects.key() + " --> " + expectedTextUnmatchedSubjects.at( i ).join( ", " );
        QListWidgetItem *item = fadttsWindow->m_unmatchedSubjectListWidget->item( i );

        if( item->text() != expectedText )
        {
            testTextUnmatchedSubjects2 = false;
        }
        if( item->backgroundColor() != fadttsWindow->m_red )
        {
            testBackgroundColorSubjectsUnmatched2 = false;
        }
        if( item->flags() == Qt::ItemIsEnabled )
        {
            testFlagsSubjectsUnmatched2 = false;
        }
        ++iterUnmatchedSubjects;
    }

    QString expectedMatchedSubjectsInformation2 = "9/14 matched";
    QString matchedSubjectsInformationDisplayed2 = fadttsWindow->subjectTab_matchedSubjectsInformation_label->text();
    bool testMatchedSubjectsInformation2 = expectedMatchedSubjectsInformation2 == matchedSubjectsInformationDisplayed2;

    QString expectedUnmatchedSubjectsInformation2 = "5/14 unmatched";
    QString unmatchedSubjectsInformationDisplayed2 = fadttsWindow->subjectTab_unmatchedSubjectsInformation_label->text();
    bool testUnmatchedSubjectsInformation2 = expectedUnmatchedSubjectsInformation2 == unmatchedSubjectsInformationDisplayed2;


    /************************************************/
    /************** No Subjects Matched *************/
    /************************************************/
    int nbrMatchedSubjects3;
    int nbrUnmatchedSubjects3;
    bool testTextUnmatchedSubjects3 = true;
    bool testBackgroundColorSubjectsUnmatched3 = true;
    bool testFlagsSubjectsUnmatched3 = true;


    fadttsWindow->DisplaySortedSubjects( QStringList(), unMatchedSubjects );

    nbrMatchedSubjects3 = fadttsWindow->m_matchedSubjectListWidget->count();
    bool testSizeListWidgetMatchedSubjects3 = nbrMatchedSubjects3 == 0;

    nbrUnmatchedSubjects3 = fadttsWindow->m_unmatchedSubjectListWidget->count();
    bool testSizeListWidgetUnmatchedSubjects3 = nbrUnmatchedSubjects3 == expectedNbrUnmatchedSubjects;

    QMap< QString, QList< int > >::ConstIterator iterUnmatchedSubjectsBis = unMatchedSubjects.begin();
    for( int i = 0; i < expectedNbrUnmatchedSubjects; i++ )
    {
        QString expectedText = iterUnmatchedSubjectsBis.key() + " --> " + expectedTextUnmatchedSubjects.at( i ).join( ", " );
        QListWidgetItem *item = fadttsWindow->m_unmatchedSubjectListWidget->item( i );

        if( item->text() != expectedText )
        {
            testTextUnmatchedSubjects3 = false;
        }
        if( item->backgroundColor() != fadttsWindow->m_red )
        {
            testBackgroundColorSubjectsUnmatched3 = false;
        }
        if( item->flags() == Qt::ItemIsEnabled )
        {
            testFlagsSubjectsUnmatched3 = false;
        }
        ++iterUnmatchedSubjectsBis;
    }

    QString expectedMatchedSubjectsInformation3 = "WARNING No matched! 0/5";
    QString matchedSubjectsInformationDisplayed3 = fadttsWindow->subjectTab_matchedSubjectsInformation_label->text();
    bool testMatchedSubjectsInformation3 = expectedMatchedSubjectsInformation3 == matchedSubjectsInformationDisplayed3;

    QString expectedUnmatchedSubjectsInformation3 = "5/5 unmatched";
    QString unmatchedSubjectsInformationDisplayed3 = fadttsWindow->subjectTab_unmatchedSubjectsInformation_label->text();
    bool testUnmatchedSubjectsInformation3 = expectedUnmatchedSubjectsInformation3 == unmatchedSubjectsInformationDisplayed3;


    /************************************************/
    /****************** No Subjects *****************/
    /************************************************/
    int nbrMatchedSubjects4;
    int nbrUnmatchedSubjects4;


    fadttsWindow->DisplaySortedSubjects( QStringList(), QMap< QString, QList< int > >() );

    nbrMatchedSubjects4 = fadttsWindow->m_matchedSubjectListWidget->count();
    bool testSizeListWidgetMatchedSubjects4 = nbrMatchedSubjects4 == 0;

    nbrUnmatchedSubjects4 = fadttsWindow->m_unmatchedSubjectListWidget->count();
    bool testSizeListWidgetUnmatchedSubjects4 = nbrUnmatchedSubjects4 == 0;

    QString matchedSubjectsInformationDisplayed4 = fadttsWindow->subjectTab_matchedSubjectsInformation_label->text();
    bool testMatchedSubjectsInformation4 = fadttsWindow->subjectTab_matchedSubjectsInformation_label->text().isEmpty();

    QString unmatchedSubjectsInformationDisplayed4 = fadttsWindow->subjectTab_unmatchedSubjectsInformation_label->text();
    bool testUnmatchedSubjectsInformation4 = fadttsWindow->subjectTab_unmatchedSubjectsInformation_label->text().isEmpty();


    bool testDisplaySortedSubjects_Passed = testSizeListWidgetMatchedSubjects1 && testSizeListWidgetUnmatchedSubjects1 && testCheckState1 && testBackgroundColorSubjectsMatched1 && testFlagsSubjectsMatched1
            && testMatchedSubjectsInformation1 && testUnmatchedSubjectsInformation1
            && testSizeListWidgetMatchedSubjects2 && testSizeListWidgetUnmatchedSubjects2 && testCheckState2 && testBackgroundColorSubjectsMatched2 && testFlagsSubjectsMatched2
            && testTextUnmatchedSubjects2 && testBackgroundColorSubjectsUnmatched2 && testFlagsSubjectsUnmatched2 && testMatchedSubjectsInformation2 && testUnmatchedSubjectsInformation2
            && testSizeListWidgetMatchedSubjects3 && testSizeListWidgetUnmatchedSubjects3 && testTextUnmatchedSubjects3 && testBackgroundColorSubjectsUnmatched3 && testFlagsSubjectsUnmatched3
            && testMatchedSubjectsInformation3 && testUnmatchedSubjectsInformation3
            && testSizeListWidgetMatchedSubjects4 && testSizeListWidgetUnmatchedSubjects4 && testMatchedSubjectsInformation4 && testUnmatchedSubjectsInformation4;
    if( !testDisplaySortedSubjects_Passed )
    {
        std::cerr << "/!\\/!\\ Test_DisplaySortedSubjects() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with DisplaySortedSubjects( const QStringList matchedSubjects, const QMap< QString, QList< int > > unMatchedSubjects )" << std::endl;
//        if( !testSizeListWidgetMatchedSubjects1 || !testSizeListWidgetUnmatchedSubjects1
//                || !testCheckState1 || !testBackgroundColorSubjectsMatched1 || !testFlagsSubjectsMatched1
//                || !testMatchedSubjectsInformation1 || !testUnmatchedSubjectsInformation1 )
//        {
//            std::cerr << "\tTest with only matched subjects:" << std::endl;
//            if( !testSizeListWidgetMatchedSubjects1 )
//            {
//                std::cerr << "\t  wrong number of matched subjects" << std::endl;
//                DisplayError_int( expectedNbrMatchedSubjects, nbrMatchedSubjects1 );
//            }
//            if( !testSizeListWidgetUnmatchedSubjects1 )
//            {
//                std::cerr << "\t  wrong number of unmatched subjects" << std::endl;
//                DisplayError_int( 0, nbrUnmatchedSubjects1 );
//            }
//            if( !testCheckState1 || !testBackgroundColorSubjectsMatched1 || !testFlagsSubjectsMatched1 )
//            {
//                DisplayError_matchedSubjects( testCheckState1, testBackgroundColorSubjectsMatched1, testFlagsSubjectsMatched1 );
//            }
//            if( !testMatchedSubjectsInformation1 )
//            {
//                std::cerr << "\t  wrong matched subjects information" << std::endl;
//                DisplayError_QString( expectedMatchedSubjectsInformation1, matchedSubjectsInformationDisplayed1, "information" );
//            }
//            if( !testUnmatchedSubjectsInformation1 )
//            {
//                std::cerr << "\t  wrong unmatched subjects information" << std::endl;
//                DisplayError_QString( expectedUnmatchedSubjectsInformation1, unmatchedSubjectsInformationDisplayed1, "information" );
//            }
//        }
//        if( !testSizeListWidgetMatchedSubjects3 || !testSizeListWidgetUnmatchedSubjects3
//                || !testTextUnmatchedSubjects3 || !testBackgroundColorSubjectsUnmatched3 || !testFlagsSubjectsUnmatched3
//                || !testMatchedSubjectsInformation3 || !testUnmatchedSubjectsInformation3 )
//        {
//            std::cerr << "\n\tTest with only unmatched subjects:" << std::endl;
//            if( !testSizeListWidgetMatchedSubjects3 )
//            {
//                std::cerr << "\t  wrong number of matched subjects" << std::endl;
//                DisplayError_int( 0, nbrMatchedSubjects3 );
//            }
//            if( !testSizeListWidgetUnmatchedSubjects3 )
//            {
//                std::cerr << "\t  wrong number of unmatched subjects" << std::endl;
//                DisplayError_int( expectedNbrUnmatchedSubjects, nbrUnmatchedSubjects3 );
//            }
//            if( !testTextUnmatchedSubjects3 || !testBackgroundColorSubjectsUnmatched3 || !testFlagsSubjectsUnmatched3 )
//            {
//                DisplayError_unmatchedSubjects( testTextUnmatchedSubjects3, testBackgroundColorSubjectsUnmatched3, testFlagsSubjectsUnmatched3 );
//            }
//            if( !testMatchedSubjectsInformation3 )
//            {
//                std::cerr << "\t  wrong matched subjects information" << std::endl;
//                DisplayError_QString( expectedMatchedSubjectsInformation3, matchedSubjectsInformationDisplayed3, "information" );
//            }
//            if( !testUnmatchedSubjectsInformation3 )
//            {
//                std::cerr << "\t  wrong unmatched subjects information" << std::endl;
//                DisplayError_QString( expectedUnmatchedSubjectsInformation3, unmatchedSubjectsInformationDisplayed3, "information" );
//            }
//        }
//        if( !testSizeListWidgetMatchedSubjects2 || !testSizeListWidgetUnmatchedSubjects2
//                || !testCheckState2 || !testBackgroundColorSubjectsMatched2 || !testFlagsSubjectsMatched2
//                || !testTextUnmatchedSubjects2 || !testBackgroundColorSubjectsUnmatched2 || !testFlagsSubjectsUnmatched2
//                || !testMatchedSubjectsInformation2 || !testUnmatchedSubjectsInformation2 )
//        {
//            std::cerr << "\n\tTest with both matched and unmatched subjects:" << std::endl;
//            if( !testSizeListWidgetMatchedSubjects2 )
//            {
//                std::cerr << "\t  wrong number of matched subjects" << std::endl;
//                DisplayError_int( expectedNbrMatchedSubjects, nbrMatchedSubjects2 );
//            }
//            if( !testSizeListWidgetUnmatchedSubjects2 )
//            {
//                std::cerr << "\t  wrong number of unmatched subjects" << std::endl;
//                DisplayError_int( expectedNbrUnmatchedSubjects, nbrUnmatchedSubjects2 );
//            }
//            if( !testCheckState2 || !testBackgroundColorSubjectsMatched2 || !testFlagsSubjectsMatched2 )
//            {
//                DisplayError_matchedSubjects( testCheckState2, testBackgroundColorSubjectsMatched2, testFlagsSubjectsMatched2 );
//            }
//            if( !testTextUnmatchedSubjects2 || !testBackgroundColorSubjectsUnmatched2 || !testFlagsSubjectsUnmatched2 )
//            {
//                DisplayError_unmatchedSubjects( testTextUnmatchedSubjects2, testBackgroundColorSubjectsUnmatched2, testFlagsSubjectsUnmatched2 );
//            }
//            if( !testMatchedSubjectsInformation2 )
//            {
//                std::cerr << "\t  wrong matched subjects information" << std::endl;
//                DisplayError_QString( expectedMatchedSubjectsInformation2, matchedSubjectsInformationDisplayed2, "information" );
//            }
//            if( !testUnmatchedSubjectsInformation2 )
//            {
//                std::cerr << "\t  wrong unmatched subjects information" << std::endl;
//                DisplayError_QString( expectedUnmatchedSubjectsInformation2, unmatchedSubjectsInformationDisplayed2, "information" );
//            }
//        }
//        if( !testSizeListWidgetMatchedSubjects4 || !testSizeListWidgetUnmatchedSubjects4
//                || !testMatchedSubjectsInformation4 || !testUnmatchedSubjectsInformation4 )
//        {
//            std::cerr << "\n\tTest with no subjects:" << std::endl;
//            if( !testSizeListWidgetMatchedSubjects4 )
//            {
//                std::cerr << "\t  wrong number of matched subjects" << std::endl;
//                DisplayError_int( 0, nbrMatchedSubjects4 );
//            }
//            if( !testSizeListWidgetUnmatchedSubjects4 )
//            {
//                std::cerr << "\t  wrong number of matched unsubjects" << std::endl;
//                DisplayError_int( 0, nbrUnmatchedSubjects4 );
//            }
//            if( !testMatchedSubjectsInformation4 )
//            {
//                std::cerr << "\t  wrong matched subjects information" << std::endl;
//                DisplayError_QString( "", matchedSubjectsInformationDisplayed4, "information" );
//            }
//            if( !testUnmatchedSubjectsInformation4 )
//            {
//                std::cerr << "\t  wrong unmatched subjects information" << std::endl;
//                DisplayError_QString( "", unmatchedSubjectsInformationDisplayed4, "information" );
//            }
//        }
    }
    else
    {
        std::cerr << "Test_DisplaySortedSubjects() PASSED";
    }

    return testDisplaySortedSubjects_Passed;
}

bool TestFADTTSWindow::Test_SetCheckStateAllVisibleSubjects()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList matchedSubjects = QStringList() << "Stan" << "Kyle" << "Kenny" << "stanley" << "Stanny" << "Butters" << "Wendy" << "Craig" << "Pip";
    QStringList expectedCheckedSubjects1 = QStringList() << "Kyle" << "Kenny" << "Butters" << "Wendy" << "Craig" << "Pip";
    QStringList expectedCheckedSubjects2 = QStringList() << "Stan" << "stanley" << "Stanny";
    QStringList expectedUncheckedSubjects1 = expectedCheckedSubjects2;
    QStringList expectedUncheckedSubjects2 = expectedCheckedSubjects1;
    QStringList checkedSubjects1;
    QStringList checkedSubjects2;
    QStringList uncheckedSubjects1;
    QStringList uncheckedSubjects2;


    fadttsWindow->DisplaySortedSubjects( matchedSubjects, QMap< QString, QList< int > >() );

    fadttsWindow->m_caseSensitivity = Qt::CaseInsensitive;
    fadttsWindow->subjectTab_search_lineEdit->setText( "tan" );
    fadttsWindow->OnSearch();
    fadttsWindow->SetCheckStateAllVisibleSubjects( Qt::Unchecked );
    fadttsWindow->subjectTab_search_lineEdit->setText( "" );
    fadttsWindow->OnSearch();
    for( int i = 0; i < fadttsWindow->m_matchedSubjectListWidget->count(); i++ )
    {
        QListWidgetItem *item = fadttsWindow->m_matchedSubjectListWidget->item( i );
        if( item->checkState() == Qt::Checked )
        {
            checkedSubjects1.append( item->text() );
        }
        else
        {
            uncheckedSubjects1.append( item->text() );
        }
    }

    fadttsWindow->SetCheckStateAllVisibleSubjects( Qt::Unchecked );
    fadttsWindow->subjectTab_search_lineEdit->setText( "tan" );
    fadttsWindow->OnSearch();
    fadttsWindow->SetCheckStateAllVisibleSubjects( Qt::Checked );
    fadttsWindow->subjectTab_search_lineEdit->setText( "" );
    fadttsWindow->OnSearch();
    for( int i = 0; i < fadttsWindow->m_matchedSubjectListWidget->count(); i++ )
    {
        QListWidgetItem *item = fadttsWindow->m_matchedSubjectListWidget->item( i );
        if( item->checkState() == Qt::Checked )
        {
            checkedSubjects2.append( item->text() );
        }
        else
        {
            uncheckedSubjects2.append( item->text() );
        }
    }

    bool testCheckedSubjects1 = checkedSubjects1 == expectedCheckedSubjects1;
    bool testUncheckedSubjects1 = uncheckedSubjects1 == expectedUncheckedSubjects1;
    bool testCheckedSubjects2 = checkedSubjects2 == expectedCheckedSubjects2;
    bool testUncheckedSubjects2 = uncheckedSubjects2 == expectedUncheckedSubjects2;


    bool testSetCheckStateAllVisibleSubjects_Passed = testCheckedSubjects1 && testCheckedSubjects2 && testUncheckedSubjects1 && testUncheckedSubjects2;
    if( !testSetCheckStateAllVisibleSubjects_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetCheckStateAllVisibleSubjects() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with SetCheckStateAllVisibleSubjects( Qt::CheckState checkState )" << std::endl;
//        if( !testCheckedSubjects1 || !testUncheckedSubjects1 )
//        {
//            std::cerr << "\tTest Qt::Unchecked:" << std::endl;
//            if( !testCheckedSubjects1 )
//            {
//                DisplayError_QStringList( expectedCheckedSubjects1, checkedSubjects1, "checked subjects" );
//            }
//            if( !testUncheckedSubjects1 )
//            {
//                DisplayError_QStringList( expectedUncheckedSubjects1, uncheckedSubjects1, "unchecked subjects" );
//            }
//        }
//        if( !testCheckedSubjects2 || !testUncheckedSubjects2 )
//        {
//            std::cerr << "\tTest Qt::Checked:" << std::endl;
//            if( !testCheckedSubjects2 )
//            {
//                DisplayError_QStringList( expectedCheckedSubjects2, checkedSubjects2, "checked subjects" );
//            }
//            if( !testUncheckedSubjects2 )
//            {
//                DisplayError_QStringList( expectedUncheckedSubjects2, uncheckedSubjects2, "unchecked subjects" );
//            }
//        }
    }
    else
    {
        std::cerr << "Test_SetCheckStateAllVisibleSubjects() PASSED";
    }

    return testSetCheckStateAllVisibleSubjects_Passed;
}

bool TestFADTTSWindow::Test_SearchSubjects()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList matchedSubjects = QStringList() << "Stan" << "Kyle" << "Kenny" << "stanley" << "Stanny" << "Butters" << "Wendy" << "Craig" << "Pip";
    QList< int > inStan = QList< int >() << -1 << 2 << 4;
    QList< int > inKyle = QList< int >() << 0 << 1 << 3;
    QList< int > inKenny = QList< int >() << -1 << 2 << 4;
    QList< int > inR2D2 = QList< int >() << -1 << 2 << 4;
    QMap< QString, QList< int > > unMatchedSubjects;
    unMatchedSubjects.insert( "Stan", inStan );
    unMatchedSubjects.insert( "Kyle", inKyle );
    unMatchedSubjects.insert( "Kenny", inKenny );
    unMatchedSubjects.insert( "R2D2", inR2D2 );


    fadttsWindow->DisplaySortedSubjects( matchedSubjects, unMatchedSubjects );

    fadttsWindow->m_caseSensitivity = Qt::CaseInsensitive;
    fadttsWindow->subjectTab_search_lineEdit->setText( "tan" );
    fadttsWindow->OnSearch();
    QString expectedInfo1 = "found 4";
    bool testInfo1 = fadttsWindow->subjectTab_nbrFound_label->text() == expectedInfo1;

    fadttsWindow->m_caseSensitivity = Qt::CaseSensitive;
    fadttsWindow->subjectTab_search_lineEdit->setText( "Stan" );
    fadttsWindow->OnSearch();
    QString expectedInfo2 = "found 3";
    bool testInfo2 = fadttsWindow->subjectTab_nbrFound_label->text() == expectedInfo2;

    fadttsWindow->m_caseSensitivity = Qt::CaseInsensitive;
    fadttsWindow->subjectTab_search_lineEdit->setText( "R2D2" );
    fadttsWindow->OnSearch();
    QString expectedInfo3 = "found 1";
    bool testInfo3 = fadttsWindow->subjectTab_nbrFound_label->text() == expectedInfo3;

    fadttsWindow->subjectTab_search_lineEdit->setText( "Pip" );
    fadttsWindow->OnSearch();
    QString expectedInfo4 = "found 1";
    bool testInfo4 = fadttsWindow->subjectTab_nbrFound_label->text() == expectedInfo4;

    fadttsWindow->m_caseSensitivity = Qt::CaseSensitive;
    fadttsWindow->subjectTab_search_lineEdit->setText( "pip" );
    fadttsWindow->OnSearch();
    QString expectedInfo5 = "found 0";
    bool testInfo5 = fadttsWindow->subjectTab_nbrFound_label->text() == expectedInfo5;


    bool testSearchSubjects_Passed = testInfo1 && testInfo2 && testInfo3 && testInfo4 && testInfo5;
    if( !testSearchSubjects_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SearchSubjects() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with SearchSubjects( QListWidget *list )" << std::endl;
    }
    else
    {
        std::cerr << "Test_SearchSubjects() PASSED";
    }

    return testSearchSubjects_Passed;
}

bool TestFADTTSWindow::Test_SaveCheckedSubjects( QString dataDir, QString tempoDir )
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QString dirTest = tempoDir + "/TestFADTTSWindow/Test_SaveCheckedSubjects";
    QDir().mkpath( dirTest );


    fadttsWindow->OnSettingSubjectList( dataDir + "/testSubjectList.txt" );
    fadttsWindow->SaveCheckedSubjects( dirTest + "/testSubjectList.txt" );


    bool testSaveCheckedSubjects_Passed = CompareFile( dataDir + "/testSubjectList.txt", dirTest + "/testSubjectList.txt" );
    if( !testSaveCheckedSubjects_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SaveCheckedSubjects() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with SaveCheckedSubjects( QString filePath )" << std::endl;
    }
    else
    {
        std::cerr << "Test_SaveCheckedSubjects() PASSED";
    }

    return testSaveCheckedSubjects_Passed;
}

bool TestFADTTSWindow::Test_GetCheckedMatchedSubjects()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList subjects;
    QStringList subjectsExpected = QStringList() << "Marcus Paige" << "Joel James" << "Nate Britt" << "Joel Berry II" << "Kennedy Meeks" << "Brice Johnson" << "Justin Jackson";


    fadttsWindow->DisplaySortedSubjects( subjectsExpected, QMap< QString, QList< int > >() );
    fadttsWindow->m_matchedSubjectListWidget->item( 0 )->setCheckState( Qt::Unchecked );
    subjectsExpected.removeFirst();
    subjects = fadttsWindow->GetCheckedMatchedSubjects();

    bool testGetCheckedMatchedSubjects_Passed = subjects == subjectsExpected;
    if( !testGetCheckedMatchedSubjects_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetCheckedMatchedSubjects() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with GetCheckedMatchedSubjects()" << std::endl;
    }
    else
    {
        std::cerr << "Test_GetCheckedMatchedSubjects() PASSED";
    }

    return testGetCheckedMatchedSubjects_Passed;
}

bool TestFADTTSWindow::Test_GetPropertyRawData()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QMap< QString, QList< QStringList > > fileDataExpected, fileData;
    fileDataExpected.insert( "AD", QList< QStringList >() << ( QStringList() << "AD1" << "AD2" << "AD3" << "AD4" ) );
    fileDataExpected.insert( "RD", QList< QStringList >() << ( QStringList() << "RD1" << "RD2" << "RD3" << "RD4" ) );
    fileDataExpected.insert( "MD", QList< QStringList >() << ( QStringList() << "MD1" << "MD2" << "MD3" << "MD4" ) );
    fileDataExpected.insert( "FA", QList< QStringList >() << ( QStringList() << "FA1" << "FA2" << "FA3" << "FA4" ) );
    fileDataExpected.insert( "submatrix", QList< QStringList >() << ( QStringList() << "submatrix1" << "submatrix2" << "submatrix3" << "submatrix4" ) );


    fadttsWindow->m_propertySelected.insert( 1, "RD" );
    fadttsWindow->m_propertySelected.insert( 2, "MD" );
    fadttsWindow->m_propertySelected.insert( 3, "FA" );
    fadttsWindow->m_data.SetFileData( 0 ) = fileDataExpected.value( "AD" );
    fadttsWindow->m_data.SetFileData( 1 ) = fileDataExpected.value( "RD" );
    fadttsWindow->m_data.SetFileData( 2 ) = fileDataExpected.value( "MD" );
    fadttsWindow->m_data.SetFileData( 3 ) = fileDataExpected.value( "FA" );
    fadttsWindow->m_data.SetFileData( 4 ) = fileDataExpected.value( "submatrix" );
    fileData = fadttsWindow->GetPropertyRawData();
    fileDataExpected.remove( "AD" );
    fileDataExpected.remove( "submatrix" );


    bool testGetPropertyRawData_Passed = fileData == fileDataExpected;
    if( !testGetPropertyRawData_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetPropertyRawData() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with GetPropertyRawData()" << std::endl;
    }
    else
    {
        std::cerr << "Test_GetPropertyRawData() PASSED";
    }

    return testGetPropertyRawData_Passed;
}



bool TestFADTTSWindow::Test_OnInputToggled()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList adSujects = QStringList() << "Stan" << "Kyle" << "Kenny" << "Cartman" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "Chef";
    QStringList rdSujects = QStringList() << "Stan" << "Kyle" << "Kenny" << "Cartman" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "Butters";
    QStringList mdSujects = QStringList() << "Stan" << "Kyle" << "Kenny" << "Cartman" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "Wendy";
    QStringList faSujects = QStringList() << "Stan" << "Kyle" << "Kenny" << "Cartman" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "Craig";
    QStringList subMatrixSujects = QStringList() << "Stan" << "Kyle" << "Kenny" << "Cartman" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "Pip";
    QStringList subjectList = QStringList() << "Stan" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "Chef" << "Butters" << "Wendy" << "Craig" << "Pip" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed";
    QStringList expectedMatchedSubjects = QStringList() << "Stan" << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed";
    QStringList expectedUnmatchedSubjects = QStringList() << "Butters --> loaded subjects, rd" << "Cartman --> rd, md, subMatrix" << "Chef --> loaded subjects" << "Craig --> loaded subjects"
                                                          << "Kenny --> rd, md, subMatrix" << "Kyle --> rd, md, subMatrix" << "Pip --> loaded subjects, subMatrix"
                                                          << "Wendy --> loaded subjects, md" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed --> loaded subjects";
    QStringList matchedSubjects;
    QStringList unmatchedSubjects;


    fadttsWindow->m_data.SetSubjects( 0 ) = adSujects;
    fadttsWindow->m_data.SetSubjects( 1 ) = rdSujects;
    fadttsWindow->m_data.SetSubjects( 2 ) = mdSujects;
    fadttsWindow->m_data.SetSubjects( 3 ) = faSujects;
    fadttsWindow->m_data.SetSubjects( 4 ) = subMatrixSujects;
    fadttsWindow->m_loadedSubjects = subjectList;
    fadttsWindow->m_areSubjectsLoaded = true;

    fadttsWindow->m_paramTabFileCheckBoxMap.value( 0 )->setChecked( false );
    fadttsWindow->m_paramTabFileCheckBoxMap.value( 1 )->setChecked( true );
    fadttsWindow->m_paramTabFileCheckBoxMap.value( 2 )->setChecked( true );
    fadttsWindow->m_paramTabFileCheckBoxMap.value( 3 )->setChecked( false );
    fadttsWindow->m_paramTabFileCheckBoxMap.value( 4 )->setChecked( true );

    fadttsWindow->OnInputToggled();

    for( int i = 0; i < fadttsWindow->m_matchedSubjectListWidget->count(); i++ )
    {
        matchedSubjects.append( fadttsWindow->m_matchedSubjectListWidget->item( i )->text() );
    }
    for( int i = 0; i < fadttsWindow->m_unmatchedSubjectListWidget->count(); i++ )
    {
        unmatchedSubjects.append( fadttsWindow->m_unmatchedSubjectListWidget->item( i )->text() );
    }

    bool testMatchedSubjects = matchedSubjects == expectedMatchedSubjects;
    bool testUnmatchedSubjects = unmatchedSubjects == expectedUnmatchedSubjects;


    bool testOnInputToggled_Passed = testMatchedSubjects && testUnmatchedSubjects;
    if( !testOnInputToggled_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnInputToggled() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnInputToggled()" << std::endl;
//        if( !testMatchedSubjects )
//        {
//            std::cerr << "\t  wrong m_matchedSubjectListWidget" << std::endl;
//            DisplayError_QStringList( expectedMatchedSubjects, matchedSubjects, "matched subjects" );
//        }
//        if( !testUnmatchedSubjects )
//        {
//            std::cerr << "\t  wrong m_unmatchedSubjectListWidget" << std::endl;
//            DisplayError_QStringList( expectedUnmatchedSubjects, unmatchedSubjects, "unmatched subjects" );
//        }
    }
    else
    {
        std::cerr << "Test_OnInputToggled() PASSED";
    }

    return testOnInputToggled_Passed;
}

bool TestFADTTSWindow::Test_OnSetCaseSensitivityToggled()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );


    fadttsWindow->OnSetCaseSensitivityToggled( true );
    bool testCaseSensitive = fadttsWindow->m_caseSensitivity == Qt::CaseSensitive;

    fadttsWindow->OnSetCaseSensitivityToggled( false );
    bool testNotCaseSensitive = fadttsWindow->m_caseSensitivity == Qt::CaseInsensitive;


    bool testOnSetCaseSensitivityToggled_Passed = testCaseSensitive && testNotCaseSensitive;
    if( !testOnSetCaseSensitivityToggled_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnSetCaseSensitivityToggled() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnSetCaseSensitivityToggled( const bool& checked )" << std::endl;
//        std::cerr << "\t  wrong m_caseSensitivity set" << std::endl;
    }
    else
    {
        std::cerr << "Test_OnSetCaseSensitivityToggled() PASSED";
    }

    return testOnSetCaseSensitivityToggled_Passed;
}

bool TestFADTTSWindow::Test_OnSubjectClicked()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList matchedSubjects = QStringList() << "Stan" << "Kyle" << "Kenny" << "Cartman" << "Chef";
    bool testCheckState = true;
    bool testColor = true;


    fadttsWindow->DisplaySortedSubjects( matchedSubjects, QMap< QString, QList< int > >() );

    fadttsWindow->OnSubjectClicked( fadttsWindow->m_matchedSubjectListWidget->item( 0 ) );
    fadttsWindow->OnSubjectClicked( fadttsWindow->m_matchedSubjectListWidget->item( 2 ) );
    fadttsWindow->OnSubjectClicked( fadttsWindow->m_matchedSubjectListWidget->item( 2 ) );
    fadttsWindow->OnSubjectClicked( fadttsWindow->m_matchedSubjectListWidget->item( 3 ) );

    testCheckState = testCheckState && fadttsWindow->m_matchedSubjectListWidget->item( 0 )->checkState() == Qt::Unchecked;
    testCheckState = testCheckState && fadttsWindow->m_matchedSubjectListWidget->item( 1 )->checkState() == Qt::Checked;
    testCheckState = testCheckState && fadttsWindow->m_matchedSubjectListWidget->item( 2 )->checkState() == Qt::Checked;
    testCheckState = testCheckState && fadttsWindow->m_matchedSubjectListWidget->item( 3 )->checkState() == Qt::Unchecked;
    testCheckState = testCheckState && fadttsWindow->m_matchedSubjectListWidget->item( 4 )->checkState() == Qt::Checked;

    testColor = testColor && fadttsWindow->m_matchedSubjectListWidget->item( 0 )->backgroundColor() == fadttsWindow->m_grey;
    testColor = testColor && fadttsWindow->m_matchedSubjectListWidget->item( 1 )->backgroundColor() == fadttsWindow->m_green;
    testColor = testColor && fadttsWindow->m_matchedSubjectListWidget->item( 2 )->backgroundColor() == fadttsWindow->m_green;
    testColor = testColor && fadttsWindow->m_matchedSubjectListWidget->item( 3 )->backgroundColor() == fadttsWindow->m_grey;
    testColor = testColor && fadttsWindow->m_matchedSubjectListWidget->item( 4 )->backgroundColor() == fadttsWindow->m_green;


    bool testOnSubjectClicked_Passed = testCheckState && testColor;
    if( !testOnSubjectClicked_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnSubjectClicked() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnSubjectClicked( QListWidgetItem *item )" << std::endl;
//        if( !testCheckState )
//        {
//            std::cerr << "\t  wrong m_matchedSubjectListWidget item check state" << std::endl;
//        }
//        if( !testColor )
//        {
//            std::cerr << "\t  wrong m_matchedSubjectListWidget item background color" << std::endl;
//        }
    }
    else
    {
        std::cerr << "Test_OnSubjectClicked() PASSED";
    }

    return testOnSubjectClicked_Passed;
}

bool TestFADTTSWindow::Test_OnSettingSubjectList( QString subjectListPath )
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList expectedMatchedSubjects = QStringList() << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed"
                                                        << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed"
                                                        << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed";
    QStringList matchedSubjects;
    QString wrongPath = "/path/subjectList.txt";


    fadttsWindow->OnSettingSubjectList( wrongPath );
    bool testMatchedSubjects1 = fadttsWindow->m_matchedSubjectListWidget->count() == 0;
    bool testLoadedSubjects1 = fadttsWindow->m_loadedSubjects.isEmpty();
    bool testSubjectsLoaded1 = !fadttsWindow->m_areSubjectsLoaded;

    fadttsWindow->OnSettingSubjectList( subjectListPath );
    for( int i = 0; i < fadttsWindow->m_matchedSubjectListWidget->count(); i++ )
    {
        matchedSubjects.append( fadttsWindow->m_matchedSubjectListWidget->item( i )->text() );
    }
    bool testMatchedSubjects2 = matchedSubjects == expectedMatchedSubjects;
    bool testLoadedSubjects2 = fadttsWindow->m_loadedSubjects == expectedMatchedSubjects;
    bool testSubjectsLoaded2 = fadttsWindow->m_areSubjectsLoaded;

    fadttsWindow->OnResetSubjectList();
    fadttsWindow->OnSettingSubjectList( fadttsWindow->m_subjectFileLineEdit->text() );
    bool testMatchedSubjects3 = fadttsWindow->m_matchedSubjectListWidget->count() == 0;
    bool testLoadedSubjects3 = fadttsWindow->m_loadedSubjects.isEmpty();
    bool testSubjectsLoaded3 = !fadttsWindow->m_areSubjectsLoaded;


    bool testOnSettingSubjectList_Passed = testMatchedSubjects1 && testLoadedSubjects1 && testSubjectsLoaded1
            && testMatchedSubjects2 && testLoadedSubjects2 && testSubjectsLoaded2
            && testMatchedSubjects3 && testLoadedSubjects3 && testSubjectsLoaded3;
    if( !testOnSettingSubjectList_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnSettingSubjectList() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnSettingSubjectList()" << std::endl;
//        if( !testMatchedSubjects2 || !testLoadedSubjects2 || !testSubjectsLoaded2 )
//        {
//            std::cerr << "\t  subject list not correctly loaded" << std::endl;
//        }
//        if( !testMatchedSubjects1 || !testLoadedSubjects1 || !testSubjectsLoaded1 )
//        {
//            std::cerr << "\t  when para_subjectTab_subjectFile_lineEdit not correctly set" << std::endl;
//        }
//        if( !testMatchedSubjects3 || !testLoadedSubjects3 || !testSubjectsLoaded3 )
//        {
//            std::cerr << "\t  when para_subjectTab_subjectFile_lineEdit is empty" << std::endl;
//        }
    }
    else
    {
        std::cerr << "Test_OnSettingSubjectList() PASSED";
    }

    return testOnSettingSubjectList_Passed;
}



/**************** Input  Tab ****************/
bool TestFADTTSWindow::Test_SetSelectedCovariates()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QMap< int, QString > covariates;
    covariates.insert( -1, "Intercept" );
    covariates.insert( 0, "ICV" );
    covariates.insert( 1, "ngroup" );
    covariates.insert( 2, "GENDER" );
    covariates.insert( 3, "Scanner" );
    covariates.insert( 4, "Twin" );
    covariates.insert( 5, "Direction" );
    QMap< int, QString > expectedSelectedCovariates;
    expectedSelectedCovariates.insert( -1, "Intercept" );
    expectedSelectedCovariates.insert( 0, "ICV" );
    expectedSelectedCovariates.insert( 3, "Scanner" );
    expectedSelectedCovariates.insert( 5, "Direction" );


    fadttsWindow->para_subjectTab_subMatrixFile_checkBox->setChecked( true );
    fadttsWindow->m_data.SetCovariates() = covariates;
    fadttsWindow->SetCovariateListWidget();
    fadttsWindow->m_covariateListWidget->item( 2 )->setCheckState( Qt::Unchecked );
    fadttsWindow->m_covariateListWidget->item( 3 )->setCheckState( Qt::Unchecked );
    fadttsWindow->m_covariateListWidget->item( 5 )->setCheckState( Qt::Unchecked );
    fadttsWindow->SetSelectedCovariates();


    bool testSetSelectedCovariates_Passed = expectedSelectedCovariates == fadttsWindow->m_selectedCovariates;
    if( !testSetSelectedCovariates_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetSelectedCovariates() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with SetSelectedCovariates()" << std::endl;
//        std::cerr << "\t    Wrong properties selected" << std::endl;
//        DisplayError_QMapIntQString( expectedSelectedCovariates, fadttsWindow->m_selectedCovariates, "m_selectedCovariates" );
    }
    else
    {
        std::cerr << "Test_SetSelectedCovariates() PASSED";
    }

    return testSetSelectedCovariates_Passed;
}

bool TestFADTTSWindow::Test_SetCheckStateAllCovariates()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QMap< int, QString > covariates;
    covariates.insert( -1, "Intercept" );
    covariates.insert( 0, "ICV" );
    covariates.insert( 1, "ngroup" );
    covariates.insert( 2, "GENDER" );
    covariates.insert( 3, "Scanner" );
    covariates.insert( 4, "Twin" );
    covariates.insert( 5, "Direction" );
    QMap< int, QString > expectedSelectedCovariates;
    expectedSelectedCovariates.insert( -1, "Intercept" );
    QMap< int, QString > selectedCovariatesCheckAll;
    QMap< int, QString > selectedCovariatesUncheckAll;


    fadttsWindow->para_subjectTab_subMatrixFile_checkBox->setChecked( true );
    fadttsWindow->m_data.SetCovariates() = covariates;
    fadttsWindow->SetCovariateListWidget();

    fadttsWindow->SetCheckStateAllCovariates( Qt::Unchecked );
    fadttsWindow->SetSelectedCovariates();
    selectedCovariatesUncheckAll = fadttsWindow->m_selectedCovariates;
    bool testCheckNone = selectedCovariatesUncheckAll == expectedSelectedCovariates;

    fadttsWindow->SetCheckStateAllCovariates( Qt::Checked );
    fadttsWindow->SetSelectedCovariates();
    selectedCovariatesCheckAll = fadttsWindow->m_selectedCovariates;
    bool testCheckAll = selectedCovariatesCheckAll == covariates;


    bool testSetCheckStateAllCovariates_Passed = testCheckNone && testCheckAll;
    if( !testSetCheckStateAllCovariates_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetCheckStateAllCovariates() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with SetCheckStateAllCovariates( Qt::CheckState checkState )" << std::endl;
//        if( !testCheckNone )
//        {
//            std::cerr << "\t    When uncheck all" << std::endl;
//            DisplayError_QMapIntQString( expectedSelectedCovariates, selectedCovariatesUncheckAll, "m_selectedCovariates" );
//        }
//        if( !testCheckAll )
//        {
//            std::cerr << "\t    When check all" << std::endl;
//            DisplayError_QMapIntQString( covariates, selectedCovariatesCheckAll, "m_selectedCovariates" );
//        }
    }
    else
    {
        std::cerr << "Test_SetCheckStateAllCovariates() PASSED";
    }

    return testSetCheckStateAllCovariates_Passed;
}

bool TestFADTTSWindow::Test_GetDisplayInputFileInformation()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QString adInformationExpected = "Filename: <i>adFile.csv</i><br>Number of subjects: <i>10</i><br>Data matrix: <i>94x10</i><br>";
    QString mdInformationExpected = "Filename: <i>mdFile.csv</i><br>Number of subjects: <i>100</i><br>Data matrix: <i>94x100</i><br>";
    QString subMatrixInformationExpected = "Filename: <i>subMatrixFile.csv</i><br>Number of subjects: <i>1000</i><br>Data matrix: <i>1000x7</i><br>Number of covariates: <i>3</i>";
    QString noInformationExpected = "<i>No File Information.<br>Please select a correct data file</i>";


    fadttsWindow->m_data.SetFilename( 0 ) = "/path/adFile.csv";
    fadttsWindow->m_data.SetFilename( 2 ) = "/path/mdFile.csv";
    fadttsWindow->m_data.SetFilename( 4 ) = "/path/subMatrixFile.csv";
    fadttsWindow->m_data.SetNbrSubjects( 0 ) = 10;
    fadttsWindow->m_data.SetNbrSubjects( 2 ) = 100;
    fadttsWindow->m_data.SetNbrSubjects( 4 ) = 1000;
    fadttsWindow->m_data.SetNbrRows( 0 ) = 94;
    fadttsWindow->m_data.SetNbrRows( 2 ) = 94;
    fadttsWindow->m_data.SetNbrRows( 4 ) = 1000;
    fadttsWindow->m_data.SetNbrColumns( 0 ) = 10;
    fadttsWindow->m_data.SetNbrColumns( 2 ) = 100;
    fadttsWindow->m_data.SetNbrColumns( 4 ) = 7;
    fadttsWindow->m_data.m_covariateMap.insert( 1, "GENDER");
    fadttsWindow->m_data.m_covariateMap.insert( 2, "Direction");
    fadttsWindow->m_data.m_covariateMap.insert( 3, "Scanner");
    fadttsWindow->m_data.AddInterceptToCovariates();

    fadttsWindow->DisplayFileInformation();

    bool testADInformation = fadttsWindow->m_inputFileInformationLabelMap.value( 0 )->text() == adInformationExpected;
    bool testMDInformation = fadttsWindow->m_inputFileInformationLabelMap.value( 2 )->text() == mdInformationExpected;
    bool testSUBMATRIXInformation = fadttsWindow->m_inputFileInformationLabelMap.value( 4 )->text() == subMatrixInformationExpected;
    bool testNoInformation =
            ( fadttsWindow->m_inputFileInformationLabelMap.value( 1 )->text() == noInformationExpected ) &&
            ( fadttsWindow->m_inputFileInformationLabelMap.value( 3 )->text() == noInformationExpected );


    bool testGetDisplayInputFileInformation_Passed = testADInformation && testMDInformation && testSUBMATRIXInformation && testNoInformation;
    if( !testGetDisplayInputFileInformation_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetDisplayInputFileInformation() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with GetInputFileInformation( const int diffusionPropertyID ) and/or DisplayFileInformation()" << std::endl;
//        if( !testADInformation )
//        {
//            std::cerr << "\t    Wrong AD information displayed" << std::endl;
//            DisplayError_QString( adInformationExpected, fadttsWindow->m_inputFileInformationLabelMap.value( 0 )->text(), "info" );
//        }
//        if( !testMDInformation )
//        {
//            std::cerr << "\n\t    Wrong MD information displayed" << std::endl;
//            DisplayError_QString( mdInformationExpected, fadttsWindow->m_inputFileInformationLabelMap.value( 2 )->text(), "info" );
//        }
//        if( !testSUBMATRIXInformation )
//        {
//            std::cerr << "\n\t    Wrong SUBMATRIX information displayed" << std::endl;
//            DisplayError_QString( subMatrixInformationExpected, fadttsWindow->m_inputFileInformationLabelMap.value( 4 )->text(), "info" );
//        }
//        if( !testNoInformation )
//        {
//            std::cerr << "\n\t    Wrong information displayed when no file" << std::endl;
//            std::cerr << "\t    RD File" << std::endl;
//            DisplayError_QString( noInformationExpected, fadttsWindow->m_inputFileInformationLabelMap.value( 1 )->text(), "info" );
//            std::cerr << "\t    FA File" << std::endl;
//            DisplayError_QString( noInformationExpected, fadttsWindow->m_inputFileInformationLabelMap.value( 3 )->text(), "info" );
//        }
    }
    else
    {
        std::cerr << "Test_GetDisplayInputFileInformation() PASSED";
    }

    return testGetDisplayInputFileInformation_Passed;
}

bool TestFADTTSWindow::Test_DisplayInputLineEditIcon( QString dataDir )
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QPixmap okPixmap = dataDir + "/okIconOut.xpm";
    QPixmap koPixmap = dataDir + "/koIconOut.xpm";


    AddIcon( fadttsWindow, dataDir );

    fadttsWindow->DisplayIcon( fadttsWindow->m_inputTabIconLabelMap.value( 0 ), okPixmap );
    fadttsWindow->DisplayIcon( fadttsWindow->m_inputTabIconLabelMap.value( 2 ), okPixmap );
    bool testOKPixmap = fadttsWindow->m_inputTabIconLabelMap.value( 0 )->pixmap()->size() == QSize( 12, 12 );
    testOKPixmap = testOKPixmap && fadttsWindow->m_inputTabIconLabelMap.value( 2 )->pixmap()->size() == QSize( 12, 12 );
    testOKPixmap = testOKPixmap && !fadttsWindow->m_inputTabIconLabelMap.value( 0 )->pixmap()->isNull();
    testOKPixmap = testOKPixmap && !fadttsWindow->m_inputTabIconLabelMap.value( 2 )->pixmap()->isNull();

    fadttsWindow->DisplayIcon( fadttsWindow->m_inputTabIconLabelMap.value( 1 ), koPixmap );
    fadttsWindow->DisplayIcon( fadttsWindow->m_inputTabIconLabelMap.value( 3 ), koPixmap );
    bool testKOPixmap = fadttsWindow->m_inputTabIconLabelMap.value( 1 )->pixmap()->size() == QSize( 12, 12 );
    testOKPixmap = testOKPixmap && fadttsWindow->m_inputTabIconLabelMap.value( 3 )->pixmap()->size() == QSize( 12, 12 );
    testKOPixmap = testKOPixmap && !fadttsWindow->m_inputTabIconLabelMap.value( 1 )->pixmap()->isNull();
    testKOPixmap = testKOPixmap && !fadttsWindow->m_inputTabIconLabelMap.value( 3 )->pixmap()->isNull();


    bool testDisplayInputLineEditIcon_Passed = testOKPixmap && testKOPixmap;
    if( !testDisplayInputLineEditIcon_Passed )
    {
        std::cerr << "/!\\/!\\ Test_DisplayInputLineEditIcon() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with DisplayInputLineEditIcon( const int diffusionPropertyID, const QPixmap icon )" << std::endl;
//        if( !testOKPixmap )
//        {
//            std::cerr << "\t  okIconOut.xpm not correctly added to label" << std::endl;
//        }
//        if( !testKOPixmap )
//        {
//            std::cerr << "\t  koIconOut.xpm not correctly added to label" << std::endl;
//        }
    }
    else
    {
        std::cerr << "Test_DisplayInputLineEditIcon() PASSED";
    }

    return testDisplayInputLineEditIcon_Passed;
}

bool TestFADTTSWindow::Test_UpdateInputFileInformation( QString rdFilePath, QString faFilePath, QString subMatrixFilePath )
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList expectedSubjects = QStringList() << "F02-1_dwi_35_all_QCed_VC_DTI_embed" << "F05-1_dwi_35_all_QCed_VC_DTI_embed" << "F06-1_42_DWI_QCed_VC_DTI_embed"
                                                 << "F07-1_42_DWI_QCed_VC_DTI_embed" << "F09-1_42_DWI_QCed_VC_DTI_embed" << "F12-1_42_DWI_QCed_VC_DTI_embed"
                                                 << "F21-1_42_DWI_QCed_VC_DTI_embed" << "F22-1_42_DWI_QCed_VC_DTI_embed" << "F23-1_42_DWI_QCed_VC_DTI_embed"
                                                 << "F25-1_42_DWI_QCed_VC_DTI_embed";
    QMap< int, QString > expectedCovariates;
    expectedCovariates.insert( -1, "Intercept" );
    expectedCovariates.insert( 1, "ngroup" );
    expectedCovariates.insert( 2, "GENDER" );
    expectedCovariates.insert( 3, "GestAgeAtBirth" );


    fadttsWindow->m_data.SetFilename( 1 ) = rdFilePath;
    fadttsWindow->m_data.SetFileData( 1 ) = fadttsWindow->m_processing.GetDataFromFile( rdFilePath );
    fadttsWindow->UpdateInputFileInformation( 1 );

    fadttsWindow->m_data.SetFilename( 3 ) = faFilePath;
    fadttsWindow->m_data.SetFileData( 3 ) = fadttsWindow->m_processing.GetDataFromFile( faFilePath );
    fadttsWindow->UpdateInputFileInformation( 3 );

    fadttsWindow->m_data.SetFilename( 4 ) = subMatrixFilePath;
    fadttsWindow->m_data.SetFileData( 4 ) = fadttsWindow->m_processing.GetDataFromFile( subMatrixFilePath );
    fadttsWindow->UpdateInputFileInformation( 4 );

    bool testUpdateRD = QFileInfo( QFile( fadttsWindow->m_data.GetFilename( 1 ) ) ).fileName() == "test_RawData_RD.csv"
            && fadttsWindow->m_data.GetNbrRows( 1 ) == 80
            && fadttsWindow->m_data.GetNbrColumns( 1 ) == 11
            && fadttsWindow->m_data.GetSubjects().value( 1 ) == expectedSubjects
            && fadttsWindow->m_data.GetNbrSubjects( 1 ) == 10;

    bool testUpdateFA = QFileInfo( QFile( fadttsWindow->m_data.GetFilename( 3 ) ) ).fileName() == "test_RawData_FA.csv"
            && fadttsWindow->m_data.GetNbrRows( 3 ) == 80
            && fadttsWindow->m_data.GetNbrColumns( 3 ) == 11
            && fadttsWindow->m_data.GetSubjects().value( 3 ) == expectedSubjects
            && fadttsWindow->m_data.GetNbrSubjects( 3 ) == 10;

    bool testUpdateSUBMATRIX = QFileInfo( QFile( fadttsWindow->m_data.GetFilename( 4 ) ) ).fileName() == "test_RawData_SUBMATRIX.csv"
            && fadttsWindow->m_data.GetNbrRows( 4 ) == 11
            && fadttsWindow->m_data.GetNbrColumns( 4 ) == 4
            && fadttsWindow->m_data.GetSubjects().value( 4 ) == expectedSubjects
            && fadttsWindow->m_data.GetNbrSubjects( 4 ) == 10
            && fadttsWindow->m_data.GetCovariates() == expectedCovariates;


    bool testUpdateInputFileInformation_Passed = testUpdateRD && testUpdateFA && testUpdateSUBMATRIX ;
    if( !testUpdateInputFileInformation_Passed )
    {
        std::cerr << "/!\\/!\\ Test_UpdateInputFileInformation() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with UpdateInputFileInformation( const int diffusionPropertyID )" << std::endl;
//        if( !testUpdateRD )
//        {
//            std::cerr << "\t  rd data not correctly updated" << std::endl;
//        }
//        if( !testUpdateFA )
//        {
//            std::cerr << "\t  fa data not correctly updated" << std::endl;
//        }
//        if( !testUpdateSUBMATRIX )
//        {
//            std::cerr << "\t  subMatrix data not correctly updated" << std::endl;
//        }
    }
    else
    {
        std::cerr << "Test_UpdateInputFileInformation() PASSED";
    }

    return testUpdateInputFileInformation_Passed;
}

bool TestFADTTSWindow::Test_UpdateLineEditsAfterAddingMultipleFiles( QString dataDir )
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList MultipleFilesListOK = QStringList() << "/path/ad_file.csv" << "/path/file_md_file.csv" << "/path/fa_file.csv" << "/path/subMatrix_file.csv";
    QStringList MultipleFilesListKO = QStringList() << "/path/ad_file.csv" << "/path/file_ad_file.csv" << "/path/filemd_file.csv" << "/path/subMatrixFile.csv";
    bool testMultipleFilesListOK = true;
    bool testMultipleFilesListKO = true;


    AddIcon( fadttsWindow, dataDir );

    fadttsWindow->UpdateLineEditsAfterAddingMultipleFiles( MultipleFilesListOK );
    testMultipleFilesListOK = testMultipleFilesListOK && ( fadttsWindow->m_inputTabInputFileLineEditMap.value( 0 )->text() == "/path/ad_file.csv" );
    testMultipleFilesListOK = testMultipleFilesListOK && fadttsWindow->m_inputTabInputFileLineEditMap.value( 1 )->text().isEmpty();
    testMultipleFilesListOK = testMultipleFilesListOK && ( fadttsWindow->m_inputTabInputFileLineEditMap.value( 2 )->text() == "/path/file_md_file.csv" );
    testMultipleFilesListOK = testMultipleFilesListOK && ( fadttsWindow->m_inputTabInputFileLineEditMap.value( 3 )->text() == "/path/fa_file.csv" );
    testMultipleFilesListOK = testMultipleFilesListOK && ( fadttsWindow->m_inputTabInputFileLineEditMap.value( 4 )->text() == "/path/subMatrix_file.csv" );

    fadttsWindow->UpdateLineEditsAfterAddingMultipleFiles( MultipleFilesListKO );
    testMultipleFilesListKO = testMultipleFilesListKO && fadttsWindow->m_inputTabInputFileLineEditMap.value( 0 )->text().isEmpty();
    testMultipleFilesListKO = testMultipleFilesListKO && fadttsWindow->m_inputTabInputFileLineEditMap.value( 1 )->text().isEmpty();
    testMultipleFilesListKO = testMultipleFilesListKO && fadttsWindow->m_inputTabInputFileLineEditMap.value( 2 )->text().isEmpty();
    testMultipleFilesListKO = testMultipleFilesListKO && fadttsWindow->m_inputTabInputFileLineEditMap.value( 3 )->text().isEmpty();
    testMultipleFilesListKO = testMultipleFilesListKO && fadttsWindow->m_inputTabInputFileLineEditMap.value( 4 )->text().isEmpty();


    bool testUpdateLineEditsAfterAddingMultipleFiles_Passed = testMultipleFilesListOK && testMultipleFilesListKO;
    if( !testUpdateLineEditsAfterAddingMultipleFiles_Passed )
    {
        std::cerr << "/!\\/!\\ Test_UpdateLineEditsAfterAddingMultipleFiles() FAILED /!\\/!\\";
//        std::cerr << std::endl << std::endl << "\t+ pb with UpdateLineEditsAfterAddingMultipleFiles( const QStringList fileList )" << std::endl;
//        if( !testMultipleFilesListOK )
//        {
//            std::cerr << "\t  files not added when they should" << std::endl;
//        }
//        if( !testMultipleFilesListKO )
//        {
//            std::cerr << "\t  files added when they should not" << std::endl;
//        }
    }
    else
    {
        std::cerr << "Test_UpdateLineEditsAfterAddingMultipleFiles() PASSED";
    }

    return testUpdateLineEditsAfterAddingMultipleFiles_Passed;
}


bool TestFADTTSWindow::Test_SetInfoSubjectColumnID( QString subMatrixFilePath )
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );


    fadttsWindow->m_data.SetFilename( 4 ) = subMatrixFilePath;
    fadttsWindow->m_data.SetFileData( 4 ) = fadttsWindow->m_processing.GetDataFromFile( subMatrixFilePath );
    fadttsWindow->SetInfoSubjectColumnID();
    bool testID = fadttsWindow->inputTab_subjectColumnID_label->text() == "<b><i><span style=""font-size:7pt;"">1</span></i></b>";

    fadttsWindow->m_data.SetSubjectColumnID() = 3;
    fadttsWindow->SetInfoSubjectColumnID();
    testID = testID && fadttsWindow->inputTab_subjectColumnID_label->text() == "<b><i><span style=""font-size:7pt;"">4</span></i></b>";

    fadttsWindow->m_data.ClearData( 4 );
    fadttsWindow->SetInfoSubjectColumnID();
    bool testNoID = fadttsWindow->inputTab_subjectColumnID_label->text().isEmpty();


    bool testSetInfoSubjectColumnID_Passed = testID && testNoID;
    if( !testSetInfoSubjectColumnID_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetInfoSubjectColumnID() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with SetInfoSubjectColumnID()" << std::endl;
//        if( !testID )
//        {
//            std::cerr << "\t    wrong subject column ID" << std::endl;
//        }
//        if( !testNoID )
//        {
//            std::cerr << "\t    no subject column ID should be displayed when submatrix data is empty" << std::endl;
//        }
    }
    else
    {
        std::cerr << "Test_SetInfoSubjectColumnID() PASSED";
    }

    return testSetInfoSubjectColumnID_Passed;
}



bool TestFADTTSWindow::Test_OnSettingInputFile( QString rdFilePath, QString subMatrixFilePath )
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );


    /******************************************/
    /***************** Test 1 *****************/
    /******************************************/
    fadttsWindow->m_inputTabInputFileLineEditMap.value( 1 )->setText( rdFilePath );
    fadttsWindow->OnSettingInputFile( 1 );
    bool testRDFilename = fadttsWindow->m_data.GetFilename( 1 ) == QFile( rdFilePath ).fileName();
    bool testRDData = fadttsWindow->m_data.GetFileData( 1 ) == fadttsWindow->m_processing.GetDataFromFile( rdFilePath );
    bool testRDNbrRows = fadttsWindow->m_data.GetNbrRows( 1 ) == 80;
    bool testRDNbrColumns = fadttsWindow->m_data.GetNbrColumns( 1 ) == 11;
    bool testRDSubjects = fadttsWindow->m_data.GetSubjects().value( 1 ) ==
            QStringList() << "F02-1_dwi_35_all_QCed_VC_DTI_embed" << "F05-1_dwi_35_all_QCed_VC_DTI_embed" << "F06-1_42_DWI_QCed_VC_DTI_embed" << "F07-1_42_DWI_QCed_VC_DTI_embed" << "F09-1_42_DWI_QCed_VC_DTI_embed"
                          << "F12-1_42_DWI_QCed_VC_DTI_embed" << "F21-1_42_DWI_QCed_VC_DTI_embed" << "F22-1_42_DWI_QCed_VC_DTI_embed" << "F23-1_42_DWI_QCed_VC_DTI_embed" << "F25-1_42_DWI_QCed_VC_DTI_embed";
    bool testRDNbrSubjects = fadttsWindow->m_data.GetNbrSubjects( 1 ) == 10;
    bool testRDCurrentDir = fadttsWindow->m_currentInputFileDir == QFileInfo( QFile( rdFilePath ) ).absolutePath();

    bool testRDFile = testRDFilename && testRDData && testRDNbrRows && testRDNbrColumns && testRDSubjects && testRDNbrSubjects && testRDCurrentDir;


    /******************************************/
    /***************** Test 2 *****************/
    /******************************************/
    fadttsWindow->m_inputTabInputFileLineEditMap.value( 1 )->setText( "/path/rdFile.csv" );
    fadttsWindow->OnSettingInputFile( 1 );
    bool testWrongRDFilename = fadttsWindow->m_data.GetFilename( 1 ).isEmpty();
    bool testWrongRDData = fadttsWindow->m_data.GetFileData( 1 ).isEmpty();
    bool testWrongRDNbrRows = fadttsWindow->m_data.GetNbrRows( 1 ) == 0;
    bool testWrongRDNbrColumns = fadttsWindow->m_data.GetNbrColumns( 1 ) == 0;
    bool testWrongRDSubjects = fadttsWindow->m_data.GetSubjects().value( 1 ).isEmpty();
    bool testWrongRDNbrSubjects = fadttsWindow->m_data.GetNbrSubjects( 1 ) == 0;
    bool testWrongRDCurrentDir = fadttsWindow->m_currentInputFileDir == QFileInfo( QFile( rdFilePath ) ).absolutePath();

    bool testWrongRDFile = testWrongRDFilename && testWrongRDData && testWrongRDNbrRows && testWrongRDNbrColumns && testWrongRDSubjects && testWrongRDNbrSubjects && testWrongRDCurrentDir;


    /******************************************/
    /***************** Test 3 *****************/
    /******************************************/
    fadttsWindow->m_inputTabInputFileLineEditMap.value( 4 )->setText( subMatrixFilePath );
    fadttsWindow->OnSettingInputFile( 4 );
    bool testSubMatrixFilename = fadttsWindow->m_data.GetFilename( 4 ) == QFile( subMatrixFilePath ).fileName();
    bool testSubMatrixData = fadttsWindow->m_data.GetFileData( 4 ) == fadttsWindow->m_processing.GetDataFromFile( subMatrixFilePath );
    bool testSubMatrixNbrRows = fadttsWindow->m_data.GetNbrRows( 4 ) == 11;
    bool testSubMatrixNbrColumns = fadttsWindow->m_data.GetNbrColumns( 4 ) == 4;
    bool testSubMatrixSubjects = fadttsWindow->m_data.GetSubjects().value( 4 ) ==
            QStringList() << "F02-1_dwi_35_all_QCed_VC_DTI_embed" << "F05-1_dwi_35_all_QCed_VC_DTI_embed" << "F06-1_42_DWI_QCed_VC_DTI_embed" << "F07-1_42_DWI_QCed_VC_DTI_embed" << "F09-1_42_DWI_QCed_VC_DTI_embed"
                          << "F12-1_42_DWI_QCed_VC_DTI_embed" << "F21-1_42_DWI_QCed_VC_DTI_embed" << "F22-1_42_DWI_QCed_VC_DTI_embed" << "F23-1_42_DWI_QCed_VC_DTI_embed" << "F25-1_42_DWI_QCed_VC_DTI_embed";
    bool testSubMatrixNbrSubjects = fadttsWindow->m_data.GetNbrSubjects( 4 ) == 10;
    bool testSubMatrixCurrentDir = fadttsWindow->m_currentInputFileDir == QFileInfo( QFile( subMatrixFilePath ) ).absolutePath();

    bool testSubMatrixFile = testSubMatrixFilename && testSubMatrixData && testSubMatrixNbrRows && testSubMatrixNbrColumns && testSubMatrixSubjects && testSubMatrixNbrSubjects && testSubMatrixCurrentDir;


    /******************************************/
    /***************** Test 4 *****************/
    /******************************************/
    fadttsWindow->m_inputTabInputFileLineEditMap.value( 4 )->setText( "" );
    fadttsWindow->OnSettingInputFile( 4 );
    bool testNoSubMatrixFilename = fadttsWindow->m_data.GetFilename( 4 ).isEmpty();
    bool testNoSubMatrixData = fadttsWindow->m_data.GetFileData( 4 ).isEmpty();
    bool testNoSubMatrixNbrRows = fadttsWindow->m_data.GetNbrRows( 4 ) == 0;
    bool testNoSubMatrixNbrColumns = fadttsWindow->m_data.GetNbrColumns( 4 ) == 0;
    bool testNoSubMatrixSubjects = fadttsWindow->m_data.GetSubjects().value( 4 ).isEmpty();
    bool testNoSubMatrixNbrSubjects = fadttsWindow->m_data.GetNbrSubjects( 4 ) == 0;
    bool testNoSubMatrixCurrentDir = fadttsWindow->m_currentInputFileDir == QFileInfo( QFile( subMatrixFilePath ) ).absolutePath();

    bool testNoSubMatrixFile = testNoSubMatrixFilename && testNoSubMatrixData && testNoSubMatrixNbrRows && testNoSubMatrixNbrColumns && testNoSubMatrixSubjects && testNoSubMatrixNbrSubjects && testNoSubMatrixCurrentDir;


    bool testOnSettingInputFile_Passed = testRDFile && testWrongRDFile && testSubMatrixFile && testNoSubMatrixFile;
    if( !testOnSettingInputFile_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnSettingInputFile() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnSettingInputFile( const int &diffusionPropertyIndex )" << std::endl;
//        if( !testRDFile )
//        {
//            std::cerr << "\t    Wrong AD/RD/MD/FA setting" << std::endl;
//        }
//        if( !testWrongRDFile )
//        {
//            std::cerr << "\t    Wrong setting when wrong file set" << std::endl;
//        }
//        if( !testSubMatrixFile )
//        {
//            std::cerr << "\t    Wrong subMatrix setting" << std::endl;
//        }
//        if( !testNoSubMatrixFile )
//        {
//            std::cerr << "\t    Wrong setting no file set" << std::endl;
//        }
    }
    else
    {
        std::cerr << "Test_OnSettingInputFile() PASSED";
    }

    return testOnSettingInputFile_Passed;
}

bool TestFADTTSWindow::Test_OnUpdatingInputFile()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QString adFilePath = "/path/adFilePath.csv";
    QString subMatrixFilePath = "/path/subMatrixFilePath.csv";


    fadttsWindow->OnUpdatingInputFile( 0, adFilePath );
    fadttsWindow->OnUpdatingInputFile( 4, subMatrixFilePath );

    bool testOnUpdatingInputFile_Passed = fadttsWindow->m_inputTabInputFileLineEditMap.value( 0 )->text() == adFilePath;
    testOnUpdatingInputFile_Passed = testOnUpdatingInputFile_Passed && fadttsWindow->m_inputTabInputFileLineEditMap.value( 4 )->text() == subMatrixFilePath;


    if( !testOnUpdatingInputFile_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnUpdatingInputFile() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnUpdatingInputFile( const int &diffusionPropertyIndex, const QString& newFilePath )" << std::endl;
    }
    else
    {
        std::cerr << "Test_OnUpdatingInputFile() PASSED";
    }

    return testOnUpdatingInputFile_Passed;
}

bool TestFADTTSWindow::Test_OnUpdatingSubjectColumnID( QString subMatrixRawDataPath )
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList expectedSubjects = QStringList() << "0" << "1";
    QStringList expectedCovariates = QStringList() << "Intercept" << "Case_orig_Shaili" << "ngroup" << "GestAgeAtBirth";
    QStringList subjects;
    QStringList covariates;


    fadttsWindow->m_inputTabInputFileLineEditMap.value( 4 )->setText( subMatrixRawDataPath );
    fadttsWindow->OnUpdatingSubjectColumnID( 2 );

    for( int i = 0; i < fadttsWindow->m_matchedSubjectListWidget->count(); i++ )
    {
        subjects.append( fadttsWindow->m_matchedSubjectListWidget->item( i )->text() );
    }
    bool testSubjects = subjects == expectedSubjects;

    for( int i = 0; i < fadttsWindow->para_inputTab_covariates_listWidget->count(); i++ )
    {
        covariates.append( fadttsWindow->para_inputTab_covariates_listWidget->item( i )->text() );
    }
    bool testCovariates = covariates == expectedCovariates;

    bool testInfoSubjectColumnID = fadttsWindow->inputTab_subjectColumnID_label->text() == "<b><i><span style=""font-size:7pt;"">3</span></i></b>";


    bool testOnUpdatingSubjectColumnID_Passed = testSubjects && testCovariates && testInfoSubjectColumnID;
    if( !testOnUpdatingSubjectColumnID_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnUpdatingSubjectColumnID() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnUpdatingSubjectColumnID( const int& newSubjectColumnID )" << std::endl;
//        if( !testSubjects )
//        {
//            std::cerr << "\t    subjects not updated" << std::endl;
//        }
//        if( !testCovariates )
//        {
//            std::cerr << "\t    covariates not updated" << std::endl;
//        }
//        if( !testInfoSubjectColumnID )
//        {
//            std::cerr << "\t    inputTab_subjectColumnID_label not updated" << std::endl;
//        }
    }
    else
    {
        std::cerr << "Test_OnUpdatingSubjectColumnID() PASSED";
    }

    return testOnUpdatingSubjectColumnID_Passed;
}

bool TestFADTTSWindow::Test_OnCovariateClicked()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QMap< int, QString > covariates;
    covariates.insert( -1, "Intercept" );
    covariates.insert( 0, "ngroup" );
    covariates.insert( 1, "GENDER" );
    covariates.insert( 2, "Twin" );
    QMap< int, QString > expectedSelectedCovariates;
    expectedSelectedCovariates.insert( -1, "Intercept" );
    expectedSelectedCovariates.insert( 2, "Twin" );
    bool testCovariateCheckState= true;


    fadttsWindow->para_subjectTab_subMatrixFile_checkBox->setChecked( true );
    fadttsWindow->m_data.SetCovariates() = covariates;
    fadttsWindow->SetCovariateListWidget();
    fadttsWindow->OnCovariateClicked( fadttsWindow->m_covariateListWidget->item( 1 ) );
    fadttsWindow->OnCovariateClicked( fadttsWindow->m_covariateListWidget->item( 2 ) );
    fadttsWindow->OnCovariateClicked( fadttsWindow->m_covariateListWidget->item( 3 ) );
    fadttsWindow->OnCovariateClicked( fadttsWindow->m_covariateListWidget->item( 3 ) );

    testCovariateCheckState = testCovariateCheckState && ( fadttsWindow->m_covariateListWidget->item( 0 )->checkState() == Qt::Checked );
    testCovariateCheckState = testCovariateCheckState && ( fadttsWindow->m_covariateListWidget->item( 1 )->checkState() == Qt::Unchecked );
    testCovariateCheckState = testCovariateCheckState && ( fadttsWindow->m_covariateListWidget->item( 2 )->checkState() == Qt::Unchecked );
    testCovariateCheckState = testCovariateCheckState && ( fadttsWindow->m_covariateListWidget->item( 3 )->checkState() == Qt::Checked );

    fadttsWindow->SetSelectedCovariates();
    bool testSelectedCovariates = fadttsWindow->m_selectedCovariates == expectedSelectedCovariates;


    bool testOnCovariateClicked_Passed = testCovariateCheckState && testSelectedCovariates;
    if( !testOnCovariateClicked_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnCovariateClicked() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnCovariateClicked( QListWidgetItem *item )" << std::endl;
//        if( !testCovariateCheckState )
//        {
//            std::cerr << "\t  wrong m_covariateListWidget item check state" << std::endl;
//        }
//        if( !testSelectedCovariates )
//        {
//            std::cerr << "\t  wrong m_selectedCovariates" << std::endl;
//            DisplayError_QMapIntQString( expectedSelectedCovariates, fadttsWindow->m_selectedCovariates, "m_selectedCovariates" );
//        }
    }
    else
    {
        std::cerr << "Test_OnCovariateClicked() PASSED";
    }

    return testOnCovariateClicked_Passed;
}



/************** Plotting  Tab **************/
bool TestFADTTSWindow::Test_SetProperties()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList colors = QStringList() << "Red" << "Lime" << "Blue" << "Yellow" << "Cyan" << "Magenta"
                                       << "Olive" << "Teal" << "Purple" << "Rosy Brown" << "Dark Sea Green" << "Corn Flower Blue"
                                       << "Maroon" << "Green" << "Navy" << "Orange" << "Mint" << "Pink" << "Brown" << "Black";
    QStringList properties1 = QStringList() << "AD" << "MD";
    QStringList properties2 = QStringList() << "AD" << "RD" << "FA";
    int i = 0;
    bool testLabelIndex1 = true;
    bool testLabelName1 = true;
    bool testComboxBoxIndex1 = true;
    bool testComboxBoxName1 = true;
    bool testLabelIndex2 = true;
    bool testLabelName2 = true;
    bool testComboxBoxIndex2 = true;
    bool testComboxBoxName2 = true;


    fadttsWindow->SetPropertyEdition( properties1 );
    QMap< int, QPair< QString, QLabel*> >::ConstIterator iterLabel1 = fadttsWindow->m_propertiesNameLabelMap.cbegin();
    QMap< int, QPair< QString, QComboBox*> >::ConstIterator iterComboBox1 = fadttsWindow->m_propertiesColorsComboBoxMap.cbegin();
    while( iterComboBox1 != fadttsWindow->m_propertiesColorsComboBoxMap.cend() )
    {
        if( iterLabel1.value().first != properties1.at( i ) )
        {
            testLabelIndex1 = false;
        }
        if( iterLabel1.value().second->text() != properties1.at( i ) )
        {
            testLabelName1 = false;
        }
        if( iterComboBox1.value().first != properties1.at( i ) )
        {
            testComboxBoxIndex1 = false;
        }
        if( iterComboBox1.value().second->currentText() != colors.at( i ) )
        {
            testComboxBoxName1 = false;
        }

        i++;
        ++iterLabel1;
        ++iterComboBox1;
    }
    bool test1 = testLabelIndex1 && testLabelName1 && testComboxBoxIndex1 && testComboxBoxName1;

    fadttsWindow->ResetPropertyEdition();
    fadttsWindow->SetPropertyEdition( properties2 );
    i = 0;
    QMap< int, QPair< QString, QLabel*> >::ConstIterator iterLabel2 = fadttsWindow->m_propertiesNameLabelMap.cbegin();
    QMap< int, QPair< QString, QComboBox*> >::ConstIterator iterComboBox2 = fadttsWindow->m_propertiesColorsComboBoxMap.cbegin();
    while( iterComboBox2 != fadttsWindow->m_propertiesColorsComboBoxMap.cend() )
    {
        if( iterLabel2.value().first != properties2.at( i ) )
        {
            testLabelIndex2 = false;
        }
        if( iterLabel2.value().second->text() != properties2.at( i ) )
        {
            testLabelName2 = false;
        }
        if( iterComboBox2.value().first != properties2.at( i ) )
        {
            testComboxBoxIndex2 = false;
        }
        if( iterComboBox2.value().second->currentText() != colors.at( i ) )
        {
            testComboxBoxName2 = false;
        }

        i++;
        ++iterLabel2;
        ++iterComboBox2;
    }
    bool test2 = testLabelIndex2 && testLabelName2 && testComboxBoxIndex2 && testComboxBoxName2;


    bool testSetProperties_Passed = test1 && test2;
    if( !testSetProperties_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetProperties() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with SetColorsComboBox( QComboBox* &comboBox ) and/or ResetPropertyEdition() and/or SetPropertyEdition( const QStringList &propertiesAvailable )" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetProperties() PASSED";
    }

    return testSetProperties_Passed;
}

bool TestFADTTSWindow::Test_SetPropertiesForDisplay()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList properties1 = QStringList() << "AD" << "RD" << "FA";
    QStringList properties2 = QStringList() << "RD" << "MD";
    QMap< int, QPair< QString, QPair< bool, QString > > > expectedPropertiesForDisplay1;
    expectedPropertiesForDisplay1.insert( 0, QPair< QString, QPair< bool, QString > >( "AD", QPair< bool, QString >( true, "Red" ) ) );
    expectedPropertiesForDisplay1.insert( 1, QPair< QString, QPair< bool, QString > >( "RD", QPair< bool, QString >( true, "Lime" ) ) );
    expectedPropertiesForDisplay1.insert( 2, QPair< QString, QPair< bool, QString > >( "FA", QPair< bool, QString >( true, "Blue" ) ) );
    QMap< int, QPair< QString, QPair< bool, QString > > > expectedPropertiesForDisplay2;
    expectedPropertiesForDisplay2.insert( 0, QPair< QString, QPair< bool, QString > >( "RD", QPair< bool, QString >( true, "Red" ) ) );
    expectedPropertiesForDisplay2.insert( 1, QPair< QString, QPair< bool, QString > >( "MD", QPair< bool, QString >( true, "Lime" ) ) );


    fadttsWindow->SetPropertyEdition( properties1 );
    fadttsWindow->SetPropertiesForDisplay( properties1 );
    bool test1 = fadttsWindow->m_propertiesForDisplay == expectedPropertiesForDisplay1;

    fadttsWindow->SetPropertyEdition( properties2 );
    fadttsWindow->SetPropertiesForDisplay( properties2 );
    bool test2 = fadttsWindow->m_propertiesForDisplay == expectedPropertiesForDisplay2;


    bool testSetPropertiesForDisplay_Passed = test1 && test2;
    if( !testSetPropertiesForDisplay_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetPropertiesForDisplay() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with SetPropertiesForDisplay( const QStringList& propertiesForDisplay )" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetPropertiesForDisplay() PASSED";
    }

    return testSetPropertiesForDisplay_Passed;
}

bool TestFADTTSWindow::Test_SetCovariates()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList colors = QStringList() << "Red" << "Lime" << "Blue" << "Carolina Blue" << "Yellow" << "Cyan" << "Magenta"
                                       << "Olive" << "Teal" << "Purple" << "Rosy Brown" << "Dark Sea Green" << "Corn Flower Blue"
                                       << "Maroon" << "Green" << "Navy" << "Orange" << "Mint" << "Pink" << "Brown" << "Black";
    QMap< int, QString > covariates1;
    covariates1.insert( 0, "Intercept" );
    covariates1.insert( 1, "ngroup" );
    covariates1.insert( 2, "GENDER" );
    covariates1.insert( 3, "Scanner" );
    covariates1.insert( 4, "Direction" );
    QMap< int, QString > covariates2;
    covariates2.insert( 0, "Intercept" );
    covariates2.insert( 1, "Twin" );
    covariates2.insert( 2, "ICV" );
    int i = 0;
    bool testLabelIndex1 = true;
    bool testLabelName1 = true;
    bool testComboxBoxIndex1 = true;
    bool testComboxBoxName1 = true;
    bool testLineEditName1 = true;
    bool testLineEditExist1 = true;
    bool testLabelIndex2 = true;
    bool testLabelName2 = true;
    bool testComboxBoxIndex2 = true;
    bool testComboxBoxName2 = true;
    bool testLineEditName2 = true;
    bool testLineEditExist2 = true;


    fadttsWindow->SetCovariateEdition( covariates1 );
    QMap< int, QPair< QString, QLabel* > >::ConstIterator iterLabel1 = fadttsWindow->m_covariatesNameLabelMap.cbegin();
    QMap< int, QPair< QString, QComboBox*> >::ConstIterator iterComboBox1 = fadttsWindow->m_covariatesColorsComboBoxMap.cbegin();
    QMap< int, QPair< QString, QLineEdit* > >::ConstIterator iterLineEdit1 = fadttsWindow->m_covariatesNameLineEditMap.cbegin();
    while( iterComboBox1 != fadttsWindow->m_covariatesColorsComboBoxMap.cend() )
    {
        if( iterLabel1.value().first != covariates1.value( i ) )
        {
            testLabelIndex1 = false;
        }
        if( iterLabel1.value().second->text() != covariates1.value( i ) )
        {
            testLabelName1 = false;
        }
        if( iterComboBox1.value().first != covariates1.value( i ) )
        {
            testComboxBoxIndex1 = false;
        }
        if( iterComboBox1.value().second->currentText() != colors.at( i ) )
        {
            testComboxBoxName1 = false;
        }
        if( iterLineEdit1.value().first != covariates1.value( i ) )
        {
            testLineEditName1 = false;
        }
        if( !iterLineEdit1.value().second->text().isEmpty() )
        {
            testLineEditExist1 = false;
        }

        i++;
        ++iterLabel1;
        ++iterComboBox1;
        ++iterLineEdit1;
    }
    bool test1 = testLabelIndex1 && testLabelName1 && testComboxBoxIndex1 && testComboxBoxName1 && testLineEditName1 && testLineEditExist1;

    fadttsWindow->SetCovariateEdition( covariates2 );
    i = 0;
    QMap< int, QPair< QString, QLabel* > >::ConstIterator iterLabel2 = fadttsWindow->m_covariatesNameLabelMap.cbegin();
    QMap< int, QPair< QString, QComboBox*> >::ConstIterator iterComboBox2 = fadttsWindow->m_covariatesColorsComboBoxMap.cbegin();
    QMap< int, QPair< QString, QLineEdit* > >::ConstIterator iterLineEdit2 = fadttsWindow->m_covariatesNameLineEditMap.cbegin();
    while( iterComboBox2 != fadttsWindow->m_covariatesColorsComboBoxMap.cend() )
    {
        if( iterLabel2.value().first != covariates2.value( i ) )
        {
            testLabelIndex2 = false;
        }
        if( iterLabel2.value().second->text() != covariates2.value( i ) )
        {
            testLabelName2 = false;
        }
        if( iterComboBox2.value().first != covariates2.value( i ) )
        {
            testComboxBoxIndex2 = false;
        }
        if( iterComboBox2.value().second->currentText() != colors.at( i ) )
        {
            testComboxBoxName2 = false;
        }
        if( iterLineEdit2.value().first != covariates2.value( i ) )
        {
            testLineEditName2 = false;
        }
        if( !iterLineEdit2.value().second->text().isEmpty() )
        {
            testLineEditExist2 = false;
        }

        i++;
        ++iterLabel2;
        ++iterComboBox2;
        ++iterLineEdit2;
    }
    bool test2 = testLabelIndex2 && testLabelName2 && testComboxBoxIndex2 && testComboxBoxName2 && testLineEditName2 && testLineEditExist2;


    bool testSetCovariates_Passed = test1 && test2;
    if( !testSetCovariates_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetCovariates() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with SetColorsComboBox( QComboBox* &comboBox ) and/or ResetCovariateEdition() and/or SetCovariateEdition( QMap< int, QString > covariatesForDisplay )" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetCovariates() PASSED";
    }

    return testSetCovariates_Passed;
}

bool TestFADTTSWindow::Test_SetCovariatesForDisplay()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QMap< int, QString > covariates1;
    covariates1.insert( 0, "Intercept" );
    covariates1.insert( 1, "ICV" );
    covariates1.insert( 2, "Direction" );
    covariates1.insert( 3, "Scanner" );
    QMap< int, QString > covariates2;
    covariates2.insert( 0, "Intercept" );
    covariates2.insert( 1, "GENDER" );
    covariates2.insert( 2, "Sex" );
    QMap< int, QPair< QString, QPair< bool, QString > > > expectedCovariatesForDisplay1;
    expectedCovariatesForDisplay1.insert( 0, QPair< QString, QPair< bool, QString > >( "Intercept", QPair< bool, QString >( true, "Red" ) ) );
    expectedCovariatesForDisplay1.insert( 1, QPair< QString, QPair< bool, QString > >( "ICV", QPair< bool, QString >( true, "Lime" ) ) );
    expectedCovariatesForDisplay1.insert( 2, QPair< QString, QPair< bool, QString > >( "Direction", QPair< bool, QString >( true, "Blue" ) ) );
    expectedCovariatesForDisplay1.insert( 3, QPair< QString, QPair< bool, QString > >( "Scanner", QPair< bool, QString >( true, "Carolina Blue" ) ) );
    QMap< int, QPair< QString, QPair< bool, QString > > > expectedCovariatesForDisplay2;
    expectedCovariatesForDisplay2.insert( 0, QPair< QString, QPair< bool, QString > >( "Intercept", QPair< bool, QString >( true, "Red" ) ) );
    expectedCovariatesForDisplay2.insert( 1, QPair< QString, QPair< bool, QString > >( "GENDER", QPair< bool, QString >( true, "Lime" ) ) );
    expectedCovariatesForDisplay2.insert( 2, QPair< QString, QPair< bool, QString > >( "Sex", QPair< bool, QString >( true, "Blue" ) ) );


    fadttsWindow->SetCovariateEdition( covariates1 );
    fadttsWindow->SetCovariatesForDisplay( covariates1 );
    bool test1 = fadttsWindow->m_covariatesForDisplay == expectedCovariatesForDisplay1;

    fadttsWindow->SetCovariateEdition( covariates2 );
    fadttsWindow->SetCovariatesForDisplay( covariates2 );
    bool test2 = fadttsWindow->m_covariatesForDisplay == expectedCovariatesForDisplay2;


    bool testSetCovariatesForDisplay_Passed = test1 && test2;
    if( !testSetCovariatesForDisplay_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetCovariatesForDisplay() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with SetCovariatesForDisplay( const QMap< int, QString >& covariatesForDisplay )" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetCovariatesForDisplay() PASSED";
    }

    return testSetCovariatesForDisplay_Passed;
}

bool TestFADTTSWindow::Test_AddLinesForDisplay()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QMap< int, QPair< QString, QPair< bool, QString > > > propertiesForDisplay;
    propertiesForDisplay.insert( 0, QPair< QString, QPair< bool, QString > >( "AD", QPair< bool, QString >( true, "red" ) ) );
    propertiesForDisplay.insert( 1, QPair< QString, QPair< bool, QString > >( "RD", QPair< bool, QString >( false, "green" ) ) );
    propertiesForDisplay.insert( 2, QPair< QString, QPair< bool, QString > >( "FA", QPair< bool, QString >( true, "blue" ) ) );
    QMap< int, QPair< QString, QPair< bool, QString > > > covariatesForDisplay;
    covariatesForDisplay.insert( 0, QPair< QString, QPair< bool, QString > >( "Intercept", QPair< bool, QString >( true, "red" ) ) );
    covariatesForDisplay.insert( 1, QPair< QString, QPair< bool, QString > >( "GENDER", QPair< bool, QString >( false, "green" ) ) );
    covariatesForDisplay.insert( 2, QPair< QString, QPair< bool, QString > >( "Scanner", QPair< bool, QString >( true, "blue" ) ) );
    covariatesForDisplay.insert( 3, QPair< QString, QPair< bool, QString > >( "ICV", QPair< bool, QString >( false, "marron" ) ) );
    bool arePropertiesForDisplay = true;
    bool testItemName1 = true;
    bool testItemCheckState1 = true;
    bool testItemName2 = true;
    bool testItemCheckState2 = true;


    fadttsWindow->m_propertiesForDisplay = propertiesForDisplay;
    fadttsWindow->m_covariatesForDisplay = covariatesForDisplay;

    fadttsWindow->AddLinesForDisplay( arePropertiesForDisplay );
    bool testArePropertiesForDisplay1 = fadttsWindow->m_areLinesForDisplayProperties == arePropertiesForDisplay;
    bool testCurrentLinesForDisplay1 = fadttsWindow->m_currentLinesForDisplay == propertiesForDisplay;
    for( int i = 0; i < fadttsWindow->m_lineDisplayedListWidget->count(); i++ )
    {
        QListWidgetItem *currentItem = fadttsWindow->m_lineDisplayedListWidget->item( i );
        testItemName1 = testItemName1 && currentItem->text() == propertiesForDisplay.value( i ).first;
        testItemCheckState1 = testItemCheckState1 && currentItem->checkState() == ( propertiesForDisplay.value( i ).second.first ? Qt::Checked : Qt::Unchecked );
    }

    bool test1 = testArePropertiesForDisplay1 && testCurrentLinesForDisplay1 && testItemName1 && testItemCheckState1;

    fadttsWindow->m_plotComboBox->addItem( "Omnibus Local pvalues" );
    fadttsWindow->AddLinesForDisplay( !arePropertiesForDisplay );
    bool testArePropertiesForDisplay2 = fadttsWindow->m_areLinesForDisplayProperties == !arePropertiesForDisplay;
    bool testCurrentLinesForDisplay2 = fadttsWindow->m_currentLinesForDisplay == covariatesForDisplay;

    for( int i = 0; i < fadttsWindow->m_lineDisplayedListWidget->count(); i++ )
    {
        QListWidgetItem *currentItem = fadttsWindow->m_lineDisplayedListWidget->item( i );
        testItemName2 = testItemName2 && currentItem->text() == covariatesForDisplay.value( i ).first;
        testItemCheckState2 = testItemCheckState2 && currentItem->checkState() == ( covariatesForDisplay.value( i ).second.first ? Qt::Checked : Qt::Unchecked );
    }
    bool testInterceptHidden = fadttsWindow->m_lineDisplayedListWidget->item( 0 )->isHidden();

    bool test2 = testArePropertiesForDisplay2 && testCurrentLinesForDisplay2 && testItemName2 && testItemCheckState2 && testInterceptHidden;


    bool testAddLinesForDisplay_Passed = test1 && test2;
    if( !testAddLinesForDisplay_Passed )
    {
        std::cerr << "/!\\/!\\ Test_AddLinesForDisplay() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with AddLinesForDisplay( bool isSelectionProperties )" << std::endl;
    }
    else
    {
        std::cerr << "Test_AddLinesForDisplay() PASSED";
    }

    return testAddLinesForDisplay_Passed;
}

bool TestFADTTSWindow::Test_SetCheckStateLinesToDisplay()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QMap< int, QPair< QString, QPair< bool, QString > > > propertiesForDisplay;
    propertiesForDisplay.insert( 0, QPair< QString, QPair< bool, QString > >( "AD", QPair< bool, QString >( true, "red" ) ) );
    propertiesForDisplay.insert( 1, QPair< QString, QPair< bool, QString > >( "RD", QPair< bool, QString >( false, "green" ) ) );
    propertiesForDisplay.insert( 2, QPair< QString, QPair< bool, QString > >( "FA", QPair< bool, QString >( true, "blue" ) ) );
    bool testItemCheckState1 = true;
    bool testItemCheckState2 = true;


    fadttsWindow->m_propertiesForDisplay = propertiesForDisplay;
    fadttsWindow->AddLinesForDisplay( true );

    fadttsWindow->SetCheckStateLinesToDisplay( Qt::Checked );
    for( int i = 0; i < fadttsWindow->m_lineDisplayedListWidget->count(); i++ )
    {
        testItemCheckState1 = testItemCheckState1 && fadttsWindow->m_lineDisplayedListWidget->item( i )->checkState() == Qt::Checked;
    }

    fadttsWindow->SetCheckStateLinesToDisplay( Qt::Unchecked );
    for( int i = 0; i < fadttsWindow->m_lineDisplayedListWidget->count(); i++ )
    {
        testItemCheckState2 = testItemCheckState1 && fadttsWindow->m_lineDisplayedListWidget->item( i )->checkState() == Qt::Unchecked;
    }


    bool testSetCheckStateLinesToDisplay_Passed = testItemCheckState1 && testItemCheckState2;
    if( !testSetCheckStateLinesToDisplay_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetCheckStateLinesToDisplay() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with SetCheckStateLinesToDisplay( Qt::CheckState checkState )" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetCheckStateLinesToDisplay() PASSED";
    }

    return testSetCheckStateLinesToDisplay_Passed;
}

bool TestFADTTSWindow::Test_EditCovariatesNames()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QMap< int, QString > covariates;
    covariates.insert( 0, "Intercept" );
    covariates.insert( 1, "ngroup" );
    covariates.insert( 2, "GENDER" );
    covariates.insert( 3, "Scanner" );
    covariates.insert( 4, "Direction" );
    QMap< int, QString > newCovariates;
    newCovariates.insert( 0, "Intercept" );
    newCovariates.insert( 1, "Group" );
    newCovariates.insert( 2, "Sex" );
    newCovariates.insert( 3, "Scanner" );
    newCovariates.insert( 4, "Dir" );
    QMap< int, QPair< QString, QPair< bool, QString > > > covariatesForDisplay;
    covariatesForDisplay.insert( 0, QPair< QString, QPair< bool, QString > >( "Intercept", QPair< bool, QString >( true, "red" ) ) );
    covariatesForDisplay.insert( 1, QPair< QString, QPair< bool, QString > >( "ngroup", QPair< bool, QString >( false, "green" ) ) );
    covariatesForDisplay.insert( 2, QPair< QString, QPair< bool, QString > >( "GENDER", QPair< bool, QString >( true, "blue" ) ) );
    covariatesForDisplay.insert( 3, QPair< QString, QPair< bool, QString > >( "Scanner", QPair< bool, QString >( false, "marron" ) ) );
    covariatesForDisplay.insert( 3, QPair< QString, QPair< bool, QString > >( "Direction", QPair< bool, QString >( false, "black" ) ) );
    bool testListWidget = true;
    bool testCovariatesForDisplay = true;


    fadttsWindow->SetCovariateEdition( covariates );
    fadttsWindow->m_covariatesForDisplay = covariatesForDisplay;
    fadttsWindow->m_areLinesForDisplayProperties = false;
    fadttsWindow->m_covariateComboBox->addItems( covariates.values() );
    fadttsWindow->m_covariateComboBox->setCurrentIndex( 3 );
    fadttsWindow->m_plot->m_allCovariates = covariates;
    covariates.remove( 0 );
    fadttsWindow->m_plot->m_covariatesNoIntercept = covariates;
    covariates.remove( 1 );
    covariates.remove( 4 );
    fadttsWindow->m_plot->m_binaryCovariates = covariates;
    fadttsWindow->m_plot->m_plotSelected = "Raw Betas by Properties";

    fadttsWindow->m_covariatesNameLineEditMap.value( 1 ).second->setText( newCovariates.value( 1 ) );
    fadttsWindow->m_covariatesNameLineEditMap.value( 2 ).second->setText( newCovariates.value( 2 ) );
    fadttsWindow->m_covariatesNameLineEditMap.value( 4 ).second->setText( newCovariates.value( 4 ) );
    fadttsWindow->EditCovariatesNames();


    bool testCovariateComboBox = fadttsWindow->m_covariateComboBox->currentText() == newCovariates.value( 2 );
    for( int i = 0; i < fadttsWindow->m_lineDisplayedListWidget->count(); i++ )
    {
        testListWidget = testListWidget && fadttsWindow->m_lineDisplayedListWidget->item( i )->text() == newCovariates.value( i );
    }
    for( int i = 0; i < fadttsWindow->m_lineDisplayedListWidget->count(); i++ )
    {
        testCovariatesForDisplay = testCovariatesForDisplay && covariatesForDisplay.value( i ).first == newCovariates.value( i );
    }

    bool testPlotCovariates = fadttsWindow->m_plot->m_allCovariates == newCovariates;
    newCovariates.remove( 0 );
    testPlotCovariates = testPlotCovariates && fadttsWindow->m_plot->m_covariatesNoIntercept == newCovariates;
    newCovariates.remove( 1 );
    newCovariates.remove( 4 );
    testPlotCovariates = testPlotCovariates && fadttsWindow->m_plot->m_binaryCovariates == newCovariates;


    bool testEditCovariatesNames_Passed = testCovariateComboBox && testListWidget && testPlotCovariates;
    if( !testEditCovariatesNames_Passed )
    {
        std::cerr << "/!\\/!\\ Test_EditCovariatesNames() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with EditCovariatesNames()" << std::endl;
    }
    else
    {
        std::cerr << "Test_EditCovariatesNames() PASSED";
    }

    return testEditCovariatesNames_Passed;
}

bool TestFADTTSWindow::Test_SetResetPlotTab( QString dataDir, QString tempoDir )
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QString dirTest = tempoDir + "/TestFADTTSWindow/Test_SetResetPlotTab";


    /******************************************/
    /***************** Test 1 *****************/
    /******************************************/
    QString outputDir1 = dirTest + "/FADTTSter_testPlot1";
    QString matlabDir1 = outputDir1 + "/MatlabOutputs";
    QDir().mkpath( outputDir1 );
    QDir().mkpath( matlabDir1 );
    QFile::copy( dataDir + "/testPlot1_RawData_MD.csv", outputDir1 + "/testPlot1_RawData_MD.csv" );
    QFile::copy( dataDir + "/testPlot1_RawData_RD.csv", outputDir1 + "/testPlot1_RawData_RD.csv" );
    QFile::copy( dataDir + "/testPlot1_RawData_SUBMATRIX.csv", outputDir1 + "/testPlot1_RawData_SUBMATRIX.csv" );
    QFile::copy( dataDir + "/testPlot1_Betas_MD.csv", matlabDir1 + "/testPlot1_Betas_MD.csv" );
    QFile::copy( dataDir + "/testPlot1_Betas_RD.csv", matlabDir1 + "/testPlot1_Betas_RD.csv" );
    QFile::copy( dataDir + "/testPlot1_Omnibus_ConfidenceBands_MD.csv", matlabDir1 + "/testPlot1_Omnibus_ConfidenceBands_MD.csv" );
    QFile::copy( dataDir + "/testPlot1_Omnibus_ConfidenceBands_RD.csv", matlabDir1 + "/testPlot1_Omnibus_ConfidenceBands_RD.csv" );
    QFile::copy( dataDir + "/testPlot1_PostHoc_FDR_Local_pvalues_MD.csv", matlabDir1 + "/testPlot1_PostHoc_FDR_Local_pvalues_MD.csv" );
    QFile::copy( dataDir + "/testPlot1_PostHoc_FDR_Local_pvalues_RD.csv", matlabDir1 + "/testPlot1_PostHoc_FDR_Local_pvalues_RD.csv" );
    QMap< int, QString > expectedProperties1;
    expectedProperties1.insert( 1, "RD" );
    expectedProperties1.insert( 2, "MD" );
    QMap< int, QString > expectedAllCovariates1;
    expectedAllCovariates1.insert( 0, "Intercept" );
    expectedAllCovariates1.insert( 1, "ngroup" );
    expectedAllCovariates1.insert( 2, "GENDER" );
    expectedAllCovariates1.insert( 3, "DaysPostBirth" );
    expectedAllCovariates1.insert( 4, "ICV_y0" );
    QMap< int, QString > expectedCovariatesNoIntercept1;
    expectedCovariatesNoIntercept1.insert( 1, "ngroup" );
    expectedCovariatesNoIntercept1.insert( 2, "GENDER" );
    expectedCovariatesNoIntercept1.insert( 3, "DaysPostBirth" );
    expectedCovariatesNoIntercept1.insert( 4, "ICV_y0" );
    QMap< int, QString > expectedBinaryCovariates1;
    expectedBinaryCovariates1.insert( 1, "ngroup" );
    expectedBinaryCovariates1.insert( 2, "GENDER" );


    fadttsWindow->para_plottingTab_loadSetDataTab_browsePlotDirectory_lineEdit->setText( outputDir1 );
    fadttsWindow->SetPlotTab();

    bool testSet1 = fadttsWindow->m_plot->m_directory == outputDir1
            && fadttsWindow->m_plot->m_matlabDirectory == matlabDir1
            && fadttsWindow->m_plot->m_fibername == "testPlot1"
            && fadttsWindow->m_plot->m_csvRawDataFiles == ( QStringList() << "testPlot1_RawData_MD.csv" << "testPlot1_RawData_RD.csv" << "testPlot1_RawData_SUBMATRIX.csv" )
            && fadttsWindow->m_plot->m_csvBetaFiles == ( QStringList() << "testPlot1_Betas_MD.csv" << "testPlot1_Betas_RD.csv" )
            && fadttsWindow->m_plot->m_csvOmnibusLpvalueFiles.isEmpty()
            && fadttsWindow->m_plot->m_csvOmnibusFDRLpvalueFiles.isEmpty()
            && fadttsWindow->m_plot->m_csvConfidenceBandsFiles == ( QStringList() << "testPlot1_Omnibus_ConfidenceBands_MD.csv" << "testPlot1_Omnibus_ConfidenceBands_RD.csv" )
            && fadttsWindow->m_plot->m_csvPostHocFDRLpvalueFiles == ( QStringList() << "testPlot1_PostHoc_FDR_Local_pvalues_MD.csv" << "testPlot1_PostHoc_FDR_Local_pvalues_RD.csv" )
            && fadttsWindow->m_plot->m_plotsUsed == ( QStringList() << "Raw Data" << "Raw Stats" << "Raw Betas by Properties" << "Raw Betas by Covariates" << "Betas with Omnibus Confidence Bands"
                                                      << "Post-Hoc FDR Local pvalues by Covariates" << "Post-Hoc FDR Significant Betas by Properties" << "Post-Hoc FDR Significant Betas by Covariates" )
            && fadttsWindow->m_plot->m_properties == expectedProperties1
            && fadttsWindow->m_plot->m_subjects == ( QStringList() << "F02-1_dwi_35_all_QCed_VC_DTI_embed" << "F05-1_dwi_35_all_QCed_VC_DTI_embed" << "F06-1_42_DWI_QCed_VC_DTI_embed" << "F07-1_42_DWI_QCed_VC_DTI_embed" << "F09-1_42_DWI_QCed_VC_DTI_embed"
                                                     << "F12-1_42_DWI_QCed_VC_DTI_embed" << "F21-1_42_DWI_QCed_VC_DTI_embed" << "F22-1_42_DWI_QCed_VC_DTI_embed" << "F23-1_42_DWI_QCed_VC_DTI_embed" << "F25-1_42_DWI_QCed_VC_DTI_embed"
                                                     << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed"
                                                     << "neo-0093-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0096-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0113-2_42_DWI_QCed_VC_DTI_embed" << "neo-0129-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0130-1_dwi_35_all_QCed_VC_DTI_embed"
                                                     << "neo-0137-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0187-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0191-1-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0219-1-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0235-1-1_dwi_35_all_QCed_VC_DTI_embed"
                                                     << "neo-0261-1-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0266-1-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0318-2_42_DWI_QCed_VC_DTI_embed" << "neo-0387-1_42_DWI_QCed_VC_DTI_embed" << "neo-0389-1_42_DWI_QCed_VC_DTI_embed"
                                                     << "neo-0393-1_42_DWI_QCed_VC_DTI_embed" << "neo-0394-1_42_DWI_QCed_VC_DTI_embed" << "neo-0397-1_42_DWI_QCed_VC_DTI_embed" << "neo-0404-1_42_DWI_QCed_VC_DTI_embed" << "neo-0409-1_42_DWI_QCed_VC_DTI_embed"
                                                     << "neo-0414-1_42_DWI_QCed_VC_DTI_embed" << "neo-0436-1_42_DWI_QCed_VC_DTI_embed" << "neo-0444-1_42_DWI_QCed_VC_DTI_embed" << "neo-0446-1_42_DWI_QCed_VC_DTI_embed" << "neo-0460-1_42_DWI_QCed_VC_DTI_embed"
                                                     << "neo-0485-1_42_DTI_QCed_VC_DTI_embed" << "neo-0498-1_42_DTI_QCed_VC_DTI_embed" << "T0008-1-2-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0009-1-2-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0010-1-1-neo_dwi_35_all_QCed_VC_DTI_embed"
                                                     << "T0039-1-2-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0045-1-1-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0068-1-1-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0068-1-2-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0081-1-2-neo_dwi_35_all_QCed_VC_DTI_embed"
                                                     << "T0086-1-2-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0092-1-2-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0107-1-1-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0107-1-2-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0113-1-1-neo_dwi_35_all_QCed_VC_DTI_embed"
                                                     << "T0113-1-2-neo_dwi_28_all_QCed_VC_DTI_embed" << "T0114-1-1-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0126-1-1-neo_dwi_21_all_QCed_VC_DTI_embed" << "T0131-1-2-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0143-1-1-neo_dwi_35_all_QCed_VC_DTI_embed" )
            && fadttsWindow->m_plot->m_allCovariates == expectedAllCovariates1
            && fadttsWindow->m_plot->m_covariatesNoIntercept == expectedCovariatesNoIntercept1
            && fadttsWindow->m_plot->m_binaryCovariates == expectedBinaryCovariates1
            && fadttsWindow->m_plot->m_abscissa == ( QList< double >() << -39.1846 << -38.1846 << -37.1846 << -36.1846 << -35.1846 << -34.1846 << -33.1846 << -32.1846 << -31.1846 << -30.1846
                                                     << -29.1846 << -28.1846 << -27.1846 << -26.1846 << -25.1846 << -24.1846 << -23.1846 << -22.1846 << -21.1846 << -20.1846
                                                     << -19.1846 << -18.1846 << -17.1846 << -16.1846 << -15.1846 << -14.1846 << -13.1846 << -12.1846 << -11.1846 << -10.1846
                                                     << -9.18462 << -8.18462 << -7.18462 << -6.18462 << -5.18462 << -4.18462 << -3.18462 << -2.18462 << -1.18462 << -0.184619
                                                     << 0.815381 << 1.81538 << 2.81538 << 3.81538 << 4.81538 << 5.81538 << 6.81538 << 7.81538 << 8.81538 << 9.81538
                                                     << 10.8154 << 11.8154 << 12.8154 << 13.8154 << 14.8154 << 15.8154 << 16.8154 << 17.8154 << 18.8154 << 19.8154
                                                     << 20.8154 << 21.8154 << 22.8154 << 23.8154 << 24.8154 << 25.8154 << 26.8154 << 27.8154 << 28.8154 << 29.8154
                                                     << 30.8154 << 31.8154 << 32.8154 << 33.8154 << 34.8154 << 35.8154 << 36.8154 << 37.8154 << 38.8154 )
            && fadttsWindow->m_plotComboBox->currentText() == "No Plot"
            && fadttsWindow->plottingTab_loadSetDataTab_load_widget->isEnabled()
            && fadttsWindow->plottingTab_titleAxisLegendTab->isEnabled()
            && fadttsWindow->plottingTab_editionTab->isEnabled()
            && fadttsWindow->plottingTab_loadPlotSettings_pushButton->isEnabled()
            && fadttsWindow->plottingTab_savePlotSettings_pushButton->isEnabled();


    /**********************************************/
    /***************** Test Reset *****************/
    /**********************************************/
    fadttsWindow->para_plottingTab_loadSetDataTab_browsePlotDirectory_lineEdit->setText( "wrong/path" );

    bool testReset = fadttsWindow->m_plot->m_directory.isEmpty() && fadttsWindow->m_plot->m_matlabDirectory.isEmpty() && fadttsWindow->m_plot->m_fibername.isEmpty()
            && fadttsWindow->m_plot->m_csvRawDataFiles.isEmpty() && fadttsWindow->m_plot->m_csvBetaFiles.isEmpty() && fadttsWindow->m_plot->m_csvOmnibusLpvalueFiles.isEmpty()
            && fadttsWindow->m_plot->m_csvOmnibusFDRLpvalueFiles.isEmpty() && fadttsWindow->m_plot->m_csvConfidenceBandsFiles.isEmpty() && fadttsWindow->m_plot->m_csvPostHocFDRLpvalueFiles.isEmpty()
            && fadttsWindow->m_plot->m_plotsUsed.isEmpty() && fadttsWindow->m_plot->m_properties.isEmpty() && fadttsWindow->m_plot->m_subjects.isEmpty() && fadttsWindow->m_plot->m_allCovariates.isEmpty()
            && fadttsWindow->m_plot->m_covariatesNoIntercept.isEmpty() && fadttsWindow->m_plot->m_binaryCovariates.isEmpty() && fadttsWindow->m_plot->m_abscissa.isEmpty()
            && fadttsWindow->m_plotComboBox->currentText() == "No Plot" && !fadttsWindow->plottingTab_loadSetDataTab_load_widget->isEnabled() && !fadttsWindow->plottingTab_titleAxisLegendTab->isEnabled()
            && !fadttsWindow->plottingTab_editionTab->isEnabled() && !fadttsWindow->plottingTab_loadPlotSettings_pushButton->isEnabled() && !fadttsWindow->plottingTab_savePlotSettings_pushButton->isEnabled();


    /******************************************/
    /***************** Test 2 *****************/
    /******************************************/
    QString outputDir2 = dirTest + "/FADTTSter_testPlot2";
    QString matlabDir2 = outputDir2 + "/MatlabOutputs";
    QDir().mkpath( outputDir2 );
    QDir().mkpath( matlabDir2 );
    QFile::copy( dataDir + "/testPlot2_RawData_AD.csv", outputDir2 + "/testPlot2_RawData_AD.csv" );
    QFile::copy( dataDir + "/testPlot2_RawData_FA.csv", outputDir2 + "/testPlot2_RawData_FA.csv" );
    QFile::copy( dataDir + "/testPlot2_RawData_SUBMATRIX.csv", outputDir2 + "/testPlot2_RawData_SUBMATRIX.csv" );
    QFile::copy( dataDir + "/testPlot2_Betas_AD.csv", matlabDir2 + "/testPlot2_Betas_AD.csv" );
    QFile::copy( dataDir + "/testPlot2_Betas_FA.csv", matlabDir2 + "/testPlot2_Betas_FA.csv" );
    QFile::copy( dataDir + "/testPlot2_Omnibus_FDR_Local_pvalues.csv", matlabDir2 + "/testPlot2_Omnibus_FDR_Local_pvalues.csv" );
    QMap< int, QString > expectedProperties2;
    expectedProperties2.insert( 0, "AD" );
    expectedProperties2.insert( 3, "FA" );
    QMap< int, QString > expectedAllCovariates2;
    expectedAllCovariates2.insert( 0, "Intercept" );
    expectedAllCovariates2.insert( 1, "ngroup" );
    expectedAllCovariates2.insert( 2, "DaysPostBirth" );
    QMap< int, QString > expectedCovariatesNoIntercept2;
    expectedCovariatesNoIntercept2.insert( 1, "ngroup" );
    expectedCovariatesNoIntercept2.insert( 2, "DaysPostBirth" );
    QMap< int, QString > expectedBinaryCovariates2;
    expectedBinaryCovariates2.insert( 1, "ngroup" );


//    fadttsWindow->para_executionTab_fiberName_lineEdit->setText( "testPlot2" );
    fadttsWindow->para_plottingTab_loadSetDataTab_browsePlotDirectory_lineEdit->setText( outputDir2 );
    fadttsWindow->SetPlotTab();

    bool testSet2 = fadttsWindow->m_plot->m_directory == outputDir2
            && fadttsWindow->m_plot->m_matlabDirectory == matlabDir2
            && fadttsWindow->m_plot->m_fibername == "testPlot2"
            && fadttsWindow->m_plot->m_csvRawDataFiles == ( QStringList() << "testPlot2_RawData_AD.csv" << "testPlot2_RawData_FA.csv" << "testPlot2_RawData_SUBMATRIX.csv" )
            && fadttsWindow->m_plot->m_csvBetaFiles == ( QStringList() << "testPlot2_Betas_AD.csv" << "testPlot2_Betas_FA.csv" )
            && fadttsWindow->m_plot->m_csvOmnibusLpvalueFiles.isEmpty()
            && fadttsWindow->m_plot->m_csvOmnibusFDRLpvalueFiles == ( QStringList() << "testPlot2_Omnibus_FDR_Local_pvalues.csv" )
            && fadttsWindow->m_plot->m_csvConfidenceBandsFiles.isEmpty()
            && fadttsWindow->m_plot->m_csvPostHocFDRLpvalueFiles.isEmpty()
            && fadttsWindow->m_plot->m_plotsUsed == ( QStringList() << "Raw Data" << "Raw Stats" << "Raw Betas by Properties" << "Raw Betas by Covariates" << "Omnibus FDR Local pvalues"
                                                      << "Omnibus FDR Significant Betas by Properties" << "Omnibus FDR Significant Betas by Covariates" )
            && fadttsWindow->m_plot->m_properties == expectedProperties2
            && fadttsWindow->m_plot->m_subjects == ( QStringList() << "F02-1_dwi_35_all_QCed_VC_DTI_embed" << "F05-1_dwi_35_all_QCed_VC_DTI_embed" << "F06-1_42_DWI_QCed_VC_DTI_embed" << "F07-1_42_DWI_QCed_VC_DTI_embed" << "F09-1_42_DWI_QCed_VC_DTI_embed"
                                                     << "F12-1_42_DWI_QCed_VC_DTI_embed" << "F21-1_42_DWI_QCed_VC_DTI_embed" << "F22-1_42_DWI_QCed_VC_DTI_embed" << "F23-1_42_DWI_QCed_VC_DTI_embed" << "F25-1_42_DWI_QCed_VC_DTI_embed"
                                                     << "neo-0038-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0042-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0066-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0071-1_dwi_35_all_QCed_VC_DTI_embed"
                                                     << "neo-0093-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0096-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0113-2_42_DWI_QCed_VC_DTI_embed" << "neo-0129-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0130-1_dwi_35_all_QCed_VC_DTI_embed"
                                                     << "neo-0137-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0187-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0191-1-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0219-1-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0235-1-1_dwi_35_all_QCed_VC_DTI_embed"
                                                     << "neo-0261-1-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0266-1-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0318-2_42_DWI_QCed_VC_DTI_embed" << "neo-0387-1_42_DWI_QCed_VC_DTI_embed" << "neo-0389-1_42_DWI_QCed_VC_DTI_embed"
                                                     << "neo-0393-1_42_DWI_QCed_VC_DTI_embed" << "neo-0394-1_42_DWI_QCed_VC_DTI_embed" << "neo-0397-1_42_DWI_QCed_VC_DTI_embed" << "neo-0404-1_42_DWI_QCed_VC_DTI_embed" << "neo-0409-1_42_DWI_QCed_VC_DTI_embed"
                                                     << "neo-0414-1_42_DWI_QCed_VC_DTI_embed" << "neo-0436-1_42_DWI_QCed_VC_DTI_embed" << "neo-0444-1_42_DWI_QCed_VC_DTI_embed" << "neo-0446-1_42_DWI_QCed_VC_DTI_embed" << "neo-0460-1_42_DWI_QCed_VC_DTI_embed"
                                                     << "neo-0485-1_42_DTI_QCed_VC_DTI_embed" << "neo-0498-1_42_DTI_QCed_VC_DTI_embed" << "T0008-1-2-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0009-1-2-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0010-1-1-neo_dwi_35_all_QCed_VC_DTI_embed"
                                                     << "T0039-1-2-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0045-1-1-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0068-1-1-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0068-1-2-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0081-1-2-neo_dwi_35_all_QCed_VC_DTI_embed"
                                                     << "T0086-1-2-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0092-1-2-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0107-1-1-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0107-1-2-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0113-1-1-neo_dwi_35_all_QCed_VC_DTI_embed"
                                                     << "T0113-1-2-neo_dwi_28_all_QCed_VC_DTI_embed" << "T0114-1-1-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0126-1-1-neo_dwi_21_all_QCed_VC_DTI_embed" << "T0131-1-2-neo_dwi_35_all_QCed_VC_DTI_embed" << "T0143-1-1-neo_dwi_35_all_QCed_VC_DTI_embed" )
            && fadttsWindow->m_plot->m_allCovariates == expectedAllCovariates2
            && fadttsWindow->m_plot->m_covariatesNoIntercept == expectedCovariatesNoIntercept2
            && fadttsWindow->m_plot->m_binaryCovariates == expectedBinaryCovariates2
            && fadttsWindow->m_plot->m_abscissa == ( QList< double >() << -39.1846 << -38.1846 << -37.1846 << -36.1846 << -35.1846 << -34.1846 << -33.1846 << -32.1846 << -31.1846 << -30.1846
                                                     << -29.1846 << -28.1846 << -27.1846 << -26.1846 << -25.1846 << -24.1846 << -23.1846 << -22.1846 << -21.1846 << -20.1846
                                                     << -19.1846 << -18.1846 << -17.1846 << -16.1846 << -15.1846 << -14.1846 << -13.1846 << -12.1846 << -11.1846 << -10.1846
                                                     << -9.18462 << -8.18462 << -7.18462 << -6.18462 << -5.18462 << -4.18462 << -3.18462 << -2.18462 << -1.18462 << -0.184619
                                                     << 0.815381 << 1.81538 << 2.81538 << 3.81538 << 4.81538 << 5.81538 << 6.81538 << 7.81538 << 8.81538 << 9.81538
                                                     << 10.8154 << 11.8154 << 12.8154 << 13.8154 << 14.8154 << 15.8154 << 16.8154 << 17.8154 << 18.8154 << 19.8154
                                                     << 20.8154 << 21.8154 << 22.8154 << 23.8154 << 24.8154 << 25.8154 << 26.8154 << 27.8154 << 28.8154 << 29.8154
                                                     << 30.8154 << 31.8154 << 32.8154 << 33.8154 << 34.8154 << 35.8154 << 36.8154 << 37.8154 << 38.8154 )
            && fadttsWindow->m_plotComboBox->currentText() == "No Plot"
            && fadttsWindow->plottingTab_loadSetDataTab_load_widget->isEnabled()
            && fadttsWindow->plottingTab_titleAxisLegendTab->isEnabled()
            && fadttsWindow->plottingTab_editionTab->isEnabled()
            && fadttsWindow->plottingTab_loadPlotSettings_pushButton->isEnabled()
            && fadttsWindow->plottingTab_savePlotSettings_pushButton->isEnabled();


    bool testSetResetPlotTab_Passed = testSet1 /*&& testReset*/ && testSet2;
    if( !testSetResetPlotTab_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetResetPlotTab() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with SetPlotTab() and/or ResetPlotTab()" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetResetPlotTab() PASSED";
    }

    return testSetResetPlotTab_Passed;
}


bool TestFADTTSWindow::Test_LoadSavePlotSettings( QString dataDir, QString tempoDir )
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QString dirTest = tempoDir + "/TestFADTTSWindow/Test_LoadSavePlotSettings";
    QString outputDir = dirTest + "/FADTTSter_testPlotSettings";
    QString matlabDir = outputDir + "/MatlabOutputs";
    QDir().mkpath( outputDir );
    QDir().mkpath( matlabDir );
    QFile::copy( dataDir + "/testPlot1_RawData_MD.csv", outputDir + "/testPlot1_RawData_MD.csv" );
    QFile::copy( dataDir + "/testPlot1_RawData_RD.csv", outputDir + "/testPlot1_RawData_RD.csv" );
    QFile::copy( dataDir + "/testPlot1_RawData_SUBMATRIX.csv", outputDir + "/testPlot1_RawData_SUBMATRIX.csv" );
    QFile::copy( dataDir + "/testPlot1_Betas_MD.csv", matlabDir + "/testPlot1_Betas_MD.csv" );
    QFile::copy( dataDir + "/testPlot1_Betas_RD.csv", matlabDir + "/testPlot1_Betas_RD.csv" );
    QFile::copy( dataDir + "/testPlot1_Omnibus_ConfidenceBands_MD.csv", matlabDir + "/testPlot1_Omnibus_ConfidenceBands_MD.csv" );
    QFile::copy( dataDir + "/testPlot1_Omnibus_ConfidenceBands_RD.csv", matlabDir + "/testPlot1_Omnibus_ConfidenceBands_RD.csv" );
    QFile::copy( dataDir + "/testPlot1_PostHoc_FDR_Local_pvalues_MD.csv", matlabDir + "/testPlot1_PostHoc_FDR_Local_pvalues_MD.csv" );
    QFile::copy( dataDir + "/testPlot1_PostHoc_FDR_Local_pvalues_RD.csv", matlabDir + "/testPlot1_PostHoc_FDR_Local_pvalues_RD.csv" );
    QMap< int, QString > expectedProperties;
    expectedProperties.insert( 1, "RD" );
    expectedProperties.insert( 2, "MD" );
    QMap< int, QString > expectedAllCovariates;
    expectedAllCovariates.insert( 0, "Intercept" );
    expectedAllCovariates.insert( 1, "ngroup" );
    expectedAllCovariates.insert( 2, "GENDER" );
    expectedAllCovariates.insert( 3, "DaysPostBirth" );
    expectedAllCovariates.insert( 4, "ICV_y0" );
    QMap< int, QString > expectedCovariatesNoIntercept;
    expectedCovariatesNoIntercept.insert( 1, "ngroup" );
    expectedCovariatesNoIntercept.insert( 2, "GENDER" );
    expectedCovariatesNoIntercept.insert( 3, "DaysPostBirth" );
    expectedCovariatesNoIntercept.insert( 4, "ICV_y0" );
    QMap< int, QString > expectedBinaryCovariates;
    expectedBinaryCovariates.insert( 1, "ngroup" );
    expectedBinaryCovariates.insert( 2, "GENDER" );


    fadttsWindow->m_plot->InitPlot( outputDir, "testPlotSettings" );
    fadttsWindow->LoadPlotSettings( dataDir + "/plotSettings.csv" );
    fadttsWindow->SavePlotSettings( dirTest + "/plotSettings.csv" );


    bool testLoadSavePlotSettings_Passed = CompareFile( dataDir + "/plotSettings.csv", dirTest + "/plotSettings.csv" );
    if( !testLoadSavePlotSettings_Passed )
    {
        std::cerr << "/!\\/!\\ Test_LoadSavePlotSettings() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with LoadPlotSettings( QString filePath ) and/or SavePlotSettings( QString filePath )" << std::endl;
    }
    else
    {
        std::cerr << "Test_LoadSavePlotSettings() PASSED";
    }

    return testLoadSavePlotSettings_Passed;
}



bool TestFADTTSWindow::Test_OnSettingPlotsUsed()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList expectedPlots1 = QStringList() << "Plot1" << "Plot2" << "Plot2";
    QStringList expectedPlots2 = QStringList() << "plot1" << "plot2" << "plot2" << "plot3";
    QStringList plots1;
    QStringList plots2;


    fadttsWindow->OnSettingPlotsUsed( expectedPlots1 );
    for( int i = 0; i < fadttsWindow->m_plotComboBox->count(); i++ )
    {
        plots1.append( fadttsWindow->m_plotComboBox->itemText( i ) );
    }
    expectedPlots1.prepend( "No Plot" );
    bool test1 = plots1 == expectedPlots1;

    fadttsWindow->OnSettingPlotsUsed( expectedPlots2 );
    for( int i = 0; i < fadttsWindow->m_plotComboBox->count(); i++ )
    {
        plots2.append( fadttsWindow->m_plotComboBox->itemText( i ) );
    }
    expectedPlots2.prepend( "No Plot" );
    bool test2 = plots2 == expectedPlots2;


    bool testOnSettingPlotsUsed_Passed = test1 && test2;
    if( !testOnSettingPlotsUsed_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnSettingPlotsUsed() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnSettingPlotsUsed( const QStringList& plotsAvailable )" << std::endl;
    }
    else
    {
        std::cerr << "Test_OnSettingPlotsUsed() PASSED";
    }

    return testOnSettingPlotsUsed_Passed;
}

bool TestFADTTSWindow::Test_OnSettingAllPropertiesUsed()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QMap< int, QString > propertiesUsed1;
    propertiesUsed1.insert( 0, "AD" );
    propertiesUsed1.insert( 2, "MD" );
    propertiesUsed1.insert( 3, "FA" );
    QMap< int, QString > propertiesUsed2;
    propertiesUsed2.insert( 1, "RD" );
    propertiesUsed2.insert( 3, "FA" );
    QStringList expectedProperties1 = QStringList() << "" << "AD" << "MD" << "FA";
    QStringList expectedProperties2 = QStringList() << "" << "RD" << "FA";
    QStringList properties1;
    QStringList properties2;


    fadttsWindow->OnSettingAllPropertiesUsed( propertiesUsed1 );
    for( int i = 0; i < fadttsWindow->m_propertyComboBox->count(); i++ )
    {
        properties1.append( fadttsWindow->m_propertyComboBox->itemText( i ) );
    }
    bool test1 = properties1 == expectedProperties1;

    fadttsWindow->OnSettingAllPropertiesUsed( propertiesUsed2 );
    for( int i = 0; i < fadttsWindow->m_propertyComboBox->count(); i++ )
    {
        properties2.append( fadttsWindow->m_propertyComboBox->itemText( i ) );
    }
    bool test2 = properties2 == expectedProperties2;


    bool testOnSettingAllPropertiesUsed_Passed = test1 && test2;
    if( !testOnSettingAllPropertiesUsed_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnSettingAllPropertiesUsed() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnSettingAllPropertiesUsed( const QMap< int, QString >& allPropertiesUsed )" << std::endl;
    }
    else
    {
        std::cerr << "Test_OnSettingAllPropertiesUsed() PASSED";
    }

    return testOnSettingAllPropertiesUsed_Passed;
}

bool TestFADTTSWindow::Test_OnUpdatingCovariatesAvailable()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QMap< int, QString > covariatesUsed1;
    covariatesUsed1.insert( 0, "Intercept" );
    covariatesUsed1.insert( 1, "ngroup" );
    covariatesUsed1.insert( 2, "GENDER" );
    QMap< int, QString > covariatesUsed2;
    covariatesUsed2.insert( 0, "Intercept" );
    covariatesUsed2.insert( 1, "ngroup" );
    covariatesUsed2.insert( 2, "GENDER" );
    covariatesUsed2.insert( 3, "ICV" );
    covariatesUsed2.insert( 4, "Scanner" );
    QStringList expectedCovariates1 = QStringList() << "" << "Intercept" << "ngroup" << "GENDER";
    QStringList expectedCovariates2 = QStringList() << "" << "Intercept" << "ngroup" << "GENDER" << "ICV" << "Scanner";
    QStringList covariates1;
    QStringList covariates2;


    fadttsWindow->OnUpdatingCovariatesAvailable( covariatesUsed1 );
    for( int i = 0; i < fadttsWindow->m_covariateComboBox->count(); i++ )
    {
        covariates1.append( fadttsWindow->m_covariateComboBox->itemText( i ) );
    }
    bool test1 = covariates1 == expectedCovariates1;

    fadttsWindow->OnUpdatingCovariatesAvailable( covariatesUsed2 );
    for( int i = 0; i < fadttsWindow->m_covariateComboBox->count(); i++ )
    {
        covariates2.append( fadttsWindow->m_covariateComboBox->itemText( i ) );
    }
    bool test2 = covariates2 == expectedCovariates2;


    bool testOnUpdatingCovariatesAvailable_Passed = test1 && test2;
    if( !testOnUpdatingCovariatesAvailable_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnUpdatingCovariatesAvailable() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnUpdatingCovariatesAvailable( const QMap< int, QString >& covariateAvailable )" << std::endl;
    }
    else
    {
        std::cerr << "Test_OnUpdatingCovariatesAvailable() PASSED";
    }

    return testOnUpdatingCovariatesAvailable_Passed;
}

bool TestFADTTSWindow::Test_OnUpdatingPropertyPlotColor()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QMap< int, QString > properties;
    properties.insert( 0, "AD" );
    properties.insert( 1, "RD" );
    properties.insert( 3, "FA" );
    QMap< int, QPair< QString, QPair< bool, QString > > > expectedPropertyForDisplay;
    expectedPropertyForDisplay.insert( 0, QPair< QString, QPair< bool, QString > >( "AD", QPair< bool, QString >( true, "Yellow" ) ) );
    expectedPropertyForDisplay.insert( 1, QPair< QString, QPair< bool, QString > >( "RD", QPair< bool, QString >( true, "Red" ) ) );
    expectedPropertyForDisplay.insert( 2, QPair< QString, QPair< bool, QString > >( "FA", QPair< bool, QString >( true, "Lime" ) ) );


    fadttsWindow->OnSettingAllPropertiesUsed( properties );
    fadttsWindow->m_propertiesColorsComboBoxMap.value( 0 ).second->setCurrentText( "Yellow" );
    fadttsWindow->m_propertiesColorsComboBoxMap.value( 1 ).second->setCurrentText( "Red" );
    fadttsWindow->m_propertiesColorsComboBoxMap.value( 2 ).second->setCurrentText( "Lime" );


    bool testOnUpdatingPropertyPlotColor_Passed = fadttsWindow->m_propertiesForDisplay == expectedPropertyForDisplay;
    if( !testOnUpdatingPropertyPlotColor_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnUpdatingPropertyPlotColor() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnUpdatingPropertyPlotColor( const QString& property )" << std::endl;
    }
    else
    {
        std::cerr << "Test_OnUpdatingPropertyPlotColor() PASSED";
    }

    return testOnUpdatingPropertyPlotColor_Passed;
}

bool TestFADTTSWindow::Test_OnUpdatingCovariatePlotColor()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QMap< int, QString > covariates;
    covariates.insert( 0, "Intercept" );
    covariates.insert( 1, "ngroup" );
    covariates.insert( 2, "GENDER" );
    QMap< int, QPair< QString, QPair< bool, QString > > > expectedCovariateForDisplay;
    expectedCovariateForDisplay.insert( 0, QPair< QString, QPair< bool, QString > >( "Intercept", QPair< bool, QString >( true, "Yellow" ) ) );
    expectedCovariateForDisplay.insert( 1, QPair< QString, QPair< bool, QString > >( "ngroup", QPair< bool, QString >( true, "Red" ) ) );
    expectedCovariateForDisplay.insert( 2, QPair< QString, QPair< bool, QString > >( "GENDER", QPair< bool, QString >( true, "Lime" ) ) );

    fadttsWindow->m_areLinesForDisplayProperties = false;
    fadttsWindow->OnSettingAllCovariatesUsed( covariates );
    fadttsWindow->m_covariatesColorsComboBoxMap.value( 0 ).second->setCurrentText( "Yellow" );
    fadttsWindow->m_covariatesColorsComboBoxMap.value( 1 ).second->setCurrentText( "Red" );
    fadttsWindow->m_covariatesColorsComboBoxMap.value( 2 ).second->setCurrentText( "Lime" );


    bool testOnUpdatingCovariatePlotColor_Passed = fadttsWindow->m_covariatesForDisplay == expectedCovariateForDisplay;
    if( !testOnUpdatingCovariatePlotColor_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnUpdatingCovariatePlotColor() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnUpdatingCovariatePlotColor( const QString& covariate )" << std::endl;
    }
    else
    {
        std::cerr << "Test_OnUpdatingCovariatePlotColor() PASSED";
    }

    return testOnUpdatingCovariatePlotColor_Passed;
}

bool TestFADTTSWindow::Test_OnPropertySelection()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QString propertySelected = "AD";


    fadttsWindow->OnPropertySelection( propertySelected );


    bool testOnPropertySelection_Passed = fadttsWindow->m_plot->m_propertySelected == propertySelected;
    if( !testOnPropertySelection_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnPropertySelection() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnPropertySelection( const QString& propertySelected )" << std::endl;
    }
    else
    {
        std::cerr << "Test_OnPropertySelection() PASSED";
    }

    return testOnPropertySelection_Passed;
}

bool TestFADTTSWindow::Test_OnCovariateSelection()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QString covariateSelected = "GENDER";


    fadttsWindow->OnCovariateSelection( covariateSelected );


    bool testOnCovariateSelection_Passed = fadttsWindow->m_plot->m_covariateSelected == covariateSelected;
    if( !testOnCovariateSelection_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnCovariateSelection() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnCovariateSelection( const QString& covariateSelected )" << std::endl;
    }
    else
    {
        std::cerr << "Test_OnCovariateSelection() PASSED";
    }

    return testOnCovariateSelection_Passed;
}

bool TestFADTTSWindow::Test_OnSettingLinesSelected()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList linesSelected = QStringList() << "Line1" << "Line2" << "Line3" << "Line4" << "Line5" << "Line6";
    QStringList textExpected = QStringList() << QString( QString::number( linesSelected.size() ) + " line(s) selected:" ) << "- Line1" << "- Line2" << "- Line3" << "- Line4" << "- Line5" << "- Line6";
    QStringList text;


    fadttsWindow->OnSettingLinesSelected( linesSelected );
    QList< QListWidgetItem * > items = fadttsWindow->m_lineSelectedListWidget->findItems( "*", Qt::MatchWrap | Qt::MatchWildcard );
    foreach( QListWidgetItem *item, items )
    {
        text.append( item->text() );
    }
    bool test1 = text == textExpected;


    bool testOnSettingLinesSelected_Passed = test1;
    if( !testOnSettingLinesSelected_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnSettingLinesSelected() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnSettingLinesSelected( const QStringList& linesSelected )" << std::endl;
//        DisplayError_QStringList( textExpected, text, "text line selected" );
    }
    else
    {
        std::cerr << "Test_OnSettingLinesSelected() PASSED";
    }

    return testOnSettingLinesSelected_Passed;
}

bool TestFADTTSWindow::Test_OnLineForDisplayClicked()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QStringList properties = QStringList() << "AD" << "RD" << "FA";
    QMap< int, QPair< QString, QPair< bool, QString > > > expectedPropertyForDisplay;
    expectedPropertyForDisplay.insert( 0, QPair< QString, QPair< bool, QString > >( "AD", QPair< bool, QString >( true, "Red" ) ) );
    expectedPropertyForDisplay.insert( 1, QPair< QString, QPair< bool, QString > >( "RD", QPair< bool, QString >( false, "Lime" ) ) );
    expectedPropertyForDisplay.insert( 2, QPair< QString, QPair< bool, QString > >( "FA", QPair< bool, QString >( true, "Blue" ) ) );


    fadttsWindow->SetPropertyEdition( properties );
    fadttsWindow->SetPropertiesForDisplay( properties );
    fadttsWindow->AddLinesForDisplay( true );
    fadttsWindow->OnLineForDisplayClicked( fadttsWindow->m_lineDisplayedListWidget->item( 0 ) );
    fadttsWindow->OnLineForDisplayClicked( fadttsWindow->m_lineDisplayedListWidget->item( 0 ) );
    fadttsWindow->OnLineForDisplayClicked( fadttsWindow->m_lineDisplayedListWidget->item( 1 ) );

    bool testCheckState = fadttsWindow->m_lineDisplayedListWidget->item( 0 )->checkState()
            && !fadttsWindow->m_lineDisplayedListWidget->item( 1 )->checkState() && fadttsWindow->m_lineDisplayedListWidget->item( 2 )->checkState();
    bool testPropertyForDisplay = fadttsWindow->m_propertiesForDisplay == expectedPropertyForDisplay;


    bool testOnLineForDisplayClicked_Passed = testCheckState && testPropertyForDisplay;
    if( !testOnLineForDisplayClicked_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnLineForDisplayClicked() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnLineForDisplayClicked( QListWidgetItem *item )" << std::endl;
    }
    else
    {
        std::cerr << "Test_OnLineForDisplayClicked() PASSED";
    }

    return testOnLineForDisplayClicked_Passed;
}

bool TestFADTTSWindow::Test_OnYChanged()
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    double yMinValue = 0.05;
    double yMaxValue = 0.09;


    fadttsWindow->OnYMinValueChanged( yMinValue );
    fadttsWindow->OnYMaxValueChanged( yMaxValue );

    bool testYMinValueChanged = fadttsWindow->plottingTab_titleAxisLegendTab_yMax_doubleSpinBox->minimum() == yMinValue;
    bool testYMaxValueChanged = fadttsWindow->plottingTab_titleAxisLegendTab_yMin_doubleSpinBox->maximum() == yMaxValue;


    bool testOnYChanged_Passed = testYMinValueChanged && testYMaxValueChanged;
    if( !testOnYChanged_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnYChanged() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with OnYMinValueChanged( double yMinValue ) and/or OnYMaxValueChanged( double yMaxValue )" << std::endl;
    }
    else
    {
        std::cerr << "Test_OnYChanged() PASSED";
    }

    return testOnYChanged_Passed;
}



/************** Execution Tab **************/
bool TestFADTTSWindow::Test_GenerateSelectedSubjectFile( QString subjectListPath, QString tempoDir )
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QString testDir = tempoDir + "/TestFADTTSWindow/Test_GenerateSelectedSubjectFile";
    QDir().mkpath( testDir );
    QStringList subjects = QStringList() << "neo-0004-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0011-2_dwi_35_all_QCed_VC_DTI_embed" << "neo-0012-2_dwi_35_all_QCed_VC_DTI_embed"
                                         << "neo-0019-2-1_dwi_35_all_QCed_VC_DTI_embed" << "neo-0029-3_dwi_35_all_QCed_VC_DTI_embed";


    fadttsWindow->DisplaySortedSubjects( subjects, QMap< QString, QList< int > >() );
    fadttsWindow->para_executionTab_fiberName_lineEdit->setText( "testSubjects" );
    fadttsWindow->GenerateSelectedSubjectFile( testDir );


    bool testGenerateSelectedSubjectFile_Passed = CompareFile( testDir + "/testSubjects_subjectList.txt", subjectListPath );
    if( !testGenerateSelectedSubjectFile_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GenerateSelectedSubjectFile() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with GenerateSelectedSubjectFile( QString outputDir )" << std::endl;
//        std::cerr << "\t    wrong subject list saved" << std::endl;
    }
    else
    {
        std::cerr << "Test_GenerateSelectedSubjectFile() PASSED";
    }

    return testGenerateSelectedSubjectFile_Passed;
}

bool TestFADTTSWindow::Test_GenerateFailedQCThresholdSubjectFile( QString dataDir, QString tempoDir )
{
    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );
    QString testDir = tempoDir + "/TestFADTTSWindow/Test_GenerateFailedQCThresholdSubjectFile";
    QDir().mkpath( testDir );


    fadttsWindow->m_failedQCThresholdSubjects = QStringList() << "Marcus Paige" << "Joel James" << "Nate Britt" << "Joel Berry II" << "Kennedy Meeks" << "Brice Johnson" << "Justin Jackson";
    fadttsWindow->para_executionTab_fiberName_lineEdit->setText( "test" );
    fadttsWindow->m_qcThreshold = 0.11;
    fadttsWindow->GenerateFailedQCThresholdSubjectFile( testDir );


    bool testGenerateFailedQCThresholdSubjectFile_Passed = CompareFile( testDir + "/test_failed_QCThreshold_SubjectList.txt", dataDir + "/test_failed_QCThreshold_SubjectList.txt" );
    if( !testGenerateFailedQCThresholdSubjectFile_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GenerateFailedQCThresholdSubjectFile() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with GenerateFailedQCThresholdSubjectFile( QString outputDir )" << std::endl;
    }
    else
    {
        std::cerr << "Test_GenerateFailedQCThresholdSubjectFile() PASSED";
    }

    return testGenerateFailedQCThresholdSubjectFile_Passed;
}

//bool TestFADTTSWindow::Test_SetLogDisplay()
//{
//    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );

//    bool testSetLogDisplay_Passed = false;
//    if( !testSetLogDisplay_Passed )
//    {
//        std::cerr << "/!\\/!\\ Test_SetLogDisplay() FAILED /!\\/!\\";
////        std::cerr << std::endl << "\t+ pb with SetLogDisplay( QString outputDir, const QMap< int, QString >& matlabInputFiles, const QMap< int, QString >& selectedCovariates );" << std::endl;
//    }
//    else
//    {
//        std::cerr << "Test_SetLogDisplay() PASSED";
//    }

//    return testSetLogDisplay_Passed;
//}



//bool TestFADTTSWindow::Test_OnRun()
//{
//    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );

//    bool testOnRun_Passed = false;
//    if( !testOnRun_Passed )
//    {
//        std::cerr << "/!\\/!\\ Test_OnRun() FAILED /!\\/!\\";
////        std::cerr << std::endl << "\t+ pb with OnRun()" << std::endl;
//    }
//    else
//    {
//        std::cerr << "Test_OnRun() PASSED";
//    }

//    return testOnRun_Passed;
//}

//bool TestFADTTSWindow::Test_OnStop()
//{
//    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );

//    bool testOnStop_Passed = false;
//    if( !testOnStop_Passed )
//    {
//        std::cerr << "/!\\/!\\ Test_OnStop() FAILED /!\\/!\\";
////        std::cerr << std::endl << "\t+ pb with OnStop()" << std::endl;
//    }
//    else
//    {
//        std::cerr << "Test_OnStop() PASSED";
//    }

//    return testOnStop_Passed;
//}

//bool TestFADTTSWindow::Test_OnDisplayLog()
//{
//    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );

//    bool testOnDisplayLog_Passed = false;
//    if( !testOnDisplayLog_Passed )
//    {
//        std::cerr << "/!\\/!\\ Test_OnDisplayLog() FAILED /!\\/!\\";
////        std::cerr << std::endl << "\t+ pb with OnDisplayLog()" << std::endl;
//    }
//    else
//    {
//        std::cerr << "Test_OnDisplayLog() PASSED";
//    }

//    return testOnDisplayLog_Passed;
//}

//bool TestFADTTSWindow::Test_OnClearLog()
//{
//    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );

//    bool testOnClearLog_Passed = false;
//    if( !testOnClearLog_Passed )
//    {
//        std::cerr << "/!\\/!\\ Test_OnClearLog() FAILED /!\\/!\\";
////        std::cerr << std::endl << "\t+ pb with OnClearLog()" << std::endl;
//    }
//    else
//    {
//        std::cerr << "Test_OnClearLog() PASSED";
//    }

//    return testOnClearLog_Passed;
//}

//bool TestFADTTSWindow::Test_OnMatlabThreadFinished()
//{
//    QSharedPointer< FADTTSWindow > fadttsWindow = QSharedPointer< FADTTSWindow >( new FADTTSWindow );

//    bool testOnMatlabThreadFinished_Passed = false;
//    if( !testOnMatlabThreadFinished_Passed )
//    {
//        std::cerr << "/!\\/!\\ Test_OnMatlabThreadFinished() FAILED /!\\/!\\";
////        std::cerr << std::endl << "\t+ pb with OnMatlabThreadFinished()" << std::endl;
//    }
//    else
//    {
//        std::cerr << "Test_OnMatlabThreadFinished() PASSED";
//    }

//    return testOnMatlabThreadFinished_Passed;
//}



/**********************************************************************/
/********************** Functions Used For Testing ********************/
/**********************************************************************/
void TestFADTTSWindow::AddIcon( QSharedPointer< FADTTSWindow > fadttsWindow, QString dataDir )
{
    fadttsWindow->m_okPixmap = dataDir + "/okIconOut.xpm";
    fadttsWindow->m_koPixmap = dataDir + "/koIconOut.xpm";
}


void TestFADTTSWindow::DisplayError_int( int intExpected, int intDisplayed )
{
    std::cerr << "\t  - expected: " << intExpected << std::endl;
    std::cerr << "\t  - displayed: " << intDisplayed << std::endl;
}

void TestFADTTSWindow::DisplayError_QString( QString qStringExpected, QString qStringDisplayed , QString type )
{
    std::cerr << "\t  - " << type.toStdString() << " expected:" << std::endl;
    std::cerr << "\t       " << qStringExpected.toStdString() << std::endl;
    std::cerr << "\t  - " << type.toStdString() << " displayed:" << std::endl;
    std::cerr << "\t       " << qStringDisplayed.toStdString() << std::endl;
}

void TestFADTTSWindow::DisplayError_QStringList( QStringList qStringListExpected, QStringList qStringListDisplayed, QString type )
{
    std::cerr << "\t  - " << type.toStdString() << " expected:" << std::endl;
    for( int i = 0; i < qStringListExpected.size(); i++ )
    {
        std::cerr << "\t      " << qStringListExpected.at( i ).toStdString() << std::endl;
    }
    std::cerr << "\t  - " << type.toStdString() << " displayed:" << std::endl;
    for( int i = 0; i < qStringListDisplayed.size(); i++ )
    {
        std::cerr << "\t      " << qStringListDisplayed.at( i ).toStdString() << std::endl;
    }
}

void TestFADTTSWindow::DisplayError_QMapIntQString( QMap< int, QString > qMapExpected, QMap< int, QString > qMapDisplayed, QString type )
{
    std::cerr << "\t  - " << type.toStdString() << " expected:" << std::endl;
    QMap< int, QString >::ConstIterator iterQMapExpected = qMapExpected.cbegin();
    while( iterQMapExpected != qMapExpected.cend() )
    {
        std::cerr << "\t       " << iterQMapExpected.value().toStdString() << " at " << iterQMapExpected.key() << std::endl;
        ++iterQMapExpected;
    }
    std::cerr << "\t  - " << type.toStdString() << " displayed:" << std::endl;
    QMap< int, QString >::ConstIterator iterQMapDisplayed = qMapDisplayed.cbegin();
    while( iterQMapDisplayed != qMapDisplayed.cend() )
    {
        std::cerr << "\t       " << iterQMapDisplayed.value().toStdString() << " at " << iterQMapDisplayed.key() << std::endl;
        ++iterQMapDisplayed;
    }
}

void TestFADTTSWindow::DisplayError_matchedSubjects( bool checkState, bool backgroundColor, bool flags )
{
    std::cerr << "\t  at least one of the matched subjects displayed in the QListWidget have the wrong" << std::endl;
    if( !checkState )
    {
        std::cerr << "\t  - Check state ( should be Checked )" << std::endl;
    }
    if( !backgroundColor )
    {
        std::cerr << "\t  - Background color ( should be green )" << std::endl;
    }
    if( !flags )
    {
        std::cerr << "\t  - Flag ( should be Enable )" << std::endl;
    }
}

void TestFADTTSWindow::DisplayError_unmatchedSubjects( bool text, bool backgroundColor, bool flags )
{
    std::cerr << "\t  at least one of the unmatched subjects displayed in the QListWidget have the wrong" << std::endl;
    if( !text )
    {
        std::cerr << "\t  - Text" << std::endl;
    }
    if( !backgroundColor )
    {
        std::cerr << "\t  - Background color ( should be red )" << std::endl;
    }
    if( !flags )
    {
        std::cerr << "\t  - Flag ( should be Disable )" << std::endl;
    }
}


QByteArray TestFADTTSWindow::GetHashFile( QString filePath )
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

bool TestFADTTSWindow::CompareFile( QString filePath1, QString filePath2 )
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
