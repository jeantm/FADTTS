#include "TestEditInputDialog.h"

TestEditInputDialog::TestEditInputDialog()
{
}

/**********************************************************************/
/*************************** Tests ************************************/
/**********************************************************************/
bool TestEditInputDialog::Test_SetCurrentDir()
{
    QSharedPointer<EditInputDialog> editInputDialog = QSharedPointer<EditInputDialog>( new EditInputDialog );
    QString outputDir1 = "./path/dir1";
    QString outputDir2 = "./path/dir2";


    editInputDialog->SetCurrentInputDir() = outputDir1;
    bool resultTest1 = editInputDialog->m_currentDir == outputDir1;

    editInputDialog->SetCurrentInputDir() = outputDir2;
    bool resultTest2 = editInputDialog->m_currentDir == outputDir2;

    editInputDialog->SetCurrentInputDir().clear();
    bool resultTest3 = editInputDialog->m_currentDir.isEmpty();


    bool testSetCurrentDir_Passed = resultTest1 && resultTest2 && resultTest3;
    if( !testSetCurrentDir_Passed )
    {
        std::cerr << "/!\\ Test_SetCurrentDir() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetCurrentInputDir()" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetCurrentDir() PASSED" << std::endl;
    }

    return ( testSetCurrentDir_Passed );
}

bool TestEditInputDialog::Test_CovariateColumnID()
{
    QSharedPointer<EditInputDialog> editInputDialog = QSharedPointer<EditInputDialog>( new EditInputDialog );
    int covariateColumnID = 10;
    int emitedCovariateColumnID;

    editInputDialog->m_covariateColumnIDSpinBox->setValue( covariateColumnID );
    editInputDialog->OnCovariateColumnIDChanged( editInputDialog->m_covariateColumnIDSpinBox->value() );
    emitedCovariateColumnID = editInputDialog->m_covariateColumnIDSpinBox->value() - 1;
    bool testOnCovariateColumnIDChanged = editInputDialog->m_covariateColumnID == covariateColumnID - 1;

    editInputDialog->ResetCovariateColumnID();
    bool testResetCovariateColumnID = editInputDialog->m_covariateColumnIDSpinBox->value() == 1;


    bool testCovariateColumnID_Passed = testOnCovariateColumnIDChanged && testResetCovariateColumnID;
    if( !testCovariateColumnID_Passed )
    {
        std::cerr << "/!\\ Test_CovariateColumnID() FAILED /!\\" << std::endl;
        if( !testOnCovariateColumnIDChanged )
        {
            std::cerr << "\t+ pb with OnCovariateColumnIDChanged( int columnID )" << std::endl;
            std::cerr << "\t   expected covariate column ID to be emited: " << covariateColumnID - 1
                      << " | covariate column ID emited: " << emitedCovariateColumnID << std::endl;

        }
        if( !testResetCovariateColumnID )
        {
            std::cerr << "\t+ pb with ResetCovariateColumnID()" << std::endl;
            std::cerr << "\t   expected m_covariateColumnIDSpinBox value: " << editInputDialog->m_covariateColumnID + 1
                      << " | covariate column ID emited: " << 0 << std::endl;
        }
    }
    else
    {
        std::cerr << "Test_CovariateColumnID() PASSED" << std::endl;
    }

    return ( testCovariateColumnID_Passed );
}


bool TestEditInputDialog::Test_OnDelete( QString adFilePath )
{
    QSharedPointer<EditInputDialog> editInputDialog = QSharedPointer<EditInputDialog>( new EditInputDialog );
    Data data;
    data.InitData();
    editInputDialog->SetData( &data );
    Processing processing;

    int indexAD = data.GetAxialDiffusivityIndex();
    data.SetFileData( indexAD ) = processing.GetDataFromFile( adFilePath );
    QList<QStringList> expectedFileData = data.GetFileData( indexAD );

    int rowID = 1;
    int columnID = 5;


    editInputDialog->DisplayDataEdition( indexAD );

    /**************** Test Delete Row ***************/
    editInputDialog->m_dataTableWidget->selectRow( rowID );
    editInputDialog->OnDeleteRows();
    expectedFileData.removeAt( rowID );

    bool testDeleteRow = TestQTableWidget( expectedFileData, editInputDialog );

    /************** Test Delete Column **************/
    editInputDialog->m_dataTableWidget->selectColumn( columnID );
    editInputDialog->OnDeleteColumns();
    for( int i = 0; i < expectedFileData.size(); i++ )
    {
        expectedFileData[ i ].removeAt( columnID );
    }

    bool testDeleteColumn = TestQTableWidget( expectedFileData, editInputDialog );


    bool testOnDelete_Passed = testDeleteRow && testDeleteColumn;
    if( !testOnDelete_Passed )
    {
        std::cerr << "/!\\ Test_OnDelete() FAILED /!\\" << std::endl;
        if( !testDeleteRow )
        {
            std::cerr << "\t+ pb with OnDeleteRows()" << std::endl;
        }
        if( !testDeleteColumn )
        {
            std::cerr << "\t+ pb with OnDeleteColumns()" << std::endl;
        }
        std::cerr << "/!\\ WARNING /!\\" << std::endl;
        std::cerr << "/!\\ WARNING /!\\ Next EditInputDialog tests heavly rely on OnDeleteRows() and OnDeleteColumns().\n"
                     "/!\\ WARNING /!\\ Therefore, results might not be accurate as long as Test_OnDelete() remains failed." << std::endl;
        std::cerr << "/!\\ WARNING /!\\" << std::endl;
    }
    else
    {
        std::cerr << "Test_OnDelete() PASSED" << std::endl;
    }

    return ( testOnDelete_Passed );
}


bool TestEditInputDialog::Test_LoadDisplayData( QString adFilePath, QString subMatrixFilePath )
{
    QSharedPointer<EditInputDialog> editInputDialog = QSharedPointer<EditInputDialog>( new EditInputDialog );
    Data data;
    data.InitData();
    editInputDialog->SetData( &data );
    Processing processing;

    int indexAD = data.GetAxialDiffusivityIndex();
    int indexSubMatrix = data.GetSubMatrixIndex();
    data.SetFileData( indexAD ) = processing.GetDataFromFile( adFilePath );
    data.SetFileData( indexSubMatrix ) = processing.GetDataFromFile( subMatrixFilePath );
    QList<QStringList> adData = data.GetFileData( indexAD );
    QList<QStringList> subMatrixData = data.GetFileData( indexSubMatrix );

    int IDRow = 2;
    int IDColumn = 2;


    /************************************************/
    /******************** Test 1 ********************/
    /************************************************/
    /** AD, RD, MD or FA file in right QLineEdit **/
    editInputDialog->DisplayDataEdition( indexAD );

    bool testHiddenState1 = editInputDialog->m_covariateColumnIDSpinBox->isHidden();

    /************************************************/
    /******************** Test 2 ********************/
    /************************************************/
    /** AD, RD, MD or FA file in right QLineEdit
     *  after deleting 1 row and 1 column **/
    editInputDialog->m_dataTableWidget->selectRow( IDRow );
    editInputDialog->OnDeleteRows();
    editInputDialog->m_dataTableWidget->selectColumn( IDColumn );
    editInputDialog->OnDeleteColumns();

    bool testHiddenState2 = editInputDialog->m_covariateColumnIDSpinBox->isHidden();

    /************************************************/
    /******************** Test 3 ********************/
    /************************************************/
    /** AD, RD, MD or FA file in wrong QLineEdit **/
    data.SetFileData( indexSubMatrix ) = adData;
    editInputDialog->ResetTableWidget();
    //    editInputDialog->SetData( &data );
    editInputDialog->DisplayDataEdition( indexSubMatrix );

    bool testHiddenState3 = !editInputDialog->m_covariateColumnIDSpinBox->isHidden();

    /************************************************/
    /******************** Test 4 ********************/
    /************************************************/
    /** subMatrix file in right QLineEdit **/
    data.SetFileData( indexSubMatrix ) = subMatrixData;
    editInputDialog->ResetTableWidget();
    editInputDialog->DisplayDataEdition( indexSubMatrix );

    bool testHiddenState4 = !editInputDialog->m_covariateColumnIDSpinBox->isHidden();

    /************************************************/
    /******************** Test 5 ********************/
    /************************************************/
    /** subMatrix file in right QLineEdit after deleting 1 row
     *  and 1 column and changing the subjectColumnID **/
    editInputDialog->m_dataTableWidget->selectRow( IDRow );
    editInputDialog->OnDeleteRows();
    editInputDialog->m_dataTableWidget->selectColumn( IDColumn );
    editInputDialog->OnDeleteColumns();
    editInputDialog->m_covariateColumnIDSpinBox->setValue( 3 );

    bool testHiddenState5 = !editInputDialog->m_covariateColumnIDSpinBox->isHidden();

    /************************************************/
    /******************** Test 6 ********************/
    /************************************************/
    /** indexSubMatrixfile in wrong QLineEdit **/
    data.SetFileData( indexAD )= subMatrixData;
    editInputDialog->ResetTableWidget();
    editInputDialog->DisplayDataEdition( indexAD );

    bool testHiddenState6 = editInputDialog->m_covariateColumnIDSpinBox->isHidden();


    bool testLoadDisplayData_Passed = testHiddenState1 && testHiddenState2 && testHiddenState3 &&
            testHiddenState4 && testHiddenState5 && testHiddenState6;
    if( !testLoadDisplayData_Passed )
    {
        std::cerr << "/!\\ Test_LoadDisplayData() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with DisplayDataEdition( const int &newDiffusionPropertyIndex ) and/or with ResetTableWidget()" << std::endl;
        if( !testHiddenState1 )
        {
            std::cerr << "\t+ When AD, RD, MD or FA file in right QLineEdit" << std::endl;
            std::cerr << "\t   EditInputDialog_subjectColumnID_spinBox and EditInputDialog_subjectColumn_label not hidden" << std::endl;
        }
        if( !testHiddenState2 )
        {
            std::cerr << "\t+ When AD, RD, MD or FA file in right QLineEdit after deleting 1 row and 1 column" << std::endl;
            std::cerr << "\t   EditInputDialog_subjectColumnID_spinBox and EditInputDialog_subjectColumn_label not hidden" << std::endl;
        }
        if( !testHiddenState3 )
        {
            std::cerr << "\t+ When AD, RD, MD or FA file in wrong QLineEdit" << std::endl;
            std::cerr << "\t   QSpinBox hidden" << std::endl;
        }
        if( !testHiddenState4 )
        {
            std::cerr << "\t+ When subMatrix file in right QLineEdit" << std::endl;
            std::cerr << "\t   QSpinBox hidden" << std::endl;
        }
        if( !testHiddenState5 )
        {
            std::cerr << "\t+ WhenindexSubMatrixfile in right QLineEdit after deleting 1 row and 1 column and changing the subjectColumnID" << std::endl;
            std::cerr << "\t   QSpinBox hidden" << std::endl;
        }
        if( !testHiddenState6 )
        {
            std::cerr << "\t+ When subMatrix file in wrong QLineEdit" << std::endl;
            std::cerr << "\t   EditInputDialog_subjectColumnID_spinBox and EditInputDialog_subjectColumn_label not hidden" << std::endl;
        }
    }
    else
    {
        std::cerr << "Test_LoadDisplayData() PASSED" << std::endl;
    }

    return ( testLoadDisplayData_Passed );
}


bool TestEditInputDialog::Test_OnSaveFile( QString outputDir, QString adFilePath, QString newADFile )
{
    QSharedPointer<EditInputDialog> editInputDialog = QSharedPointer<EditInputDialog>( new EditInputDialog );
    Data data;
    data.InitData();
    editInputDialog->SetData( &data );
    editInputDialog->SetCurrentInputDir() = outputDir;
    Processing processing;

    int indexAD = data.GetAxialDiffusivityIndex();
    data.SetFileData( indexAD ) = processing.GetDataFromFile( adFilePath );
    QString defaultSavedFilePath = outputDir + "/new" + data.GetDiffusionPropertyName( indexAD ).toUpper() + "File.csv";

    int rowID = 1;
    int columnID = 5;


    editInputDialog->DisplayDataEdition( indexAD );
    editInputDialog->m_dataTableWidget->selectRow( rowID );
    editInputDialog->OnDeleteRows();
    editInputDialog->m_dataTableWidget->selectColumn( columnID );
    editInputDialog->OnDeleteColumns();

    bool testFileSaved = editInputDialog->OnSaveFile();
    bool testFileWellSaved = CompareFile( defaultSavedFilePath, newADFile );


    bool testOnSaveFile_Passed = testFileSaved && testFileWellSaved;
    if( !testOnSaveFile_Passed )
    {
        std::cerr << "/!\\ Test_OnSaveFile() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with OnSaveFile()" << std::endl;
        if( !testFileSaved )
        {
            std::cerr << "\t+ Error during new file creation" << std::endl;
        }
        if( !testFileWellSaved )
        {
            std::cerr << "\t+ Data not correctly saved" << std::endl;
        }
    }
    else
    {
        std::cerr << "Test_OnSaveFile() PASSED" << std::endl;
    }

    return ( testOnSaveFile_Passed );
}


/**********************************************************************/
/********************** Functions Used For Testing ********************/
/**********************************************************************/
bool TestEditInputDialog::TestQTableWidget( QList<QStringList> expectedFileData, QSharedPointer<EditInputDialog> editInputDialog )
{
    for( int i = 0; i < editInputDialog->m_dataTableWidget->rowCount(); i++ )
    {
        for( int j = 0; j < editInputDialog->m_dataTableWidget->columnCount(); j++ )
        {
            QTableWidgetItem *currentItem = editInputDialog->m_dataTableWidget->item( i, j );
            if( currentItem->text() != expectedFileData.at( i ).at( j ) )
            {
                return false;
            }
        }
    }
    return true;
}


QByteArray TestEditInputDialog::GetHashFile( QString filePath )
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

bool TestEditInputDialog::CompareFile( QString filePath1, QString filePath2 )
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
