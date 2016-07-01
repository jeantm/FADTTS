#include "TestEditInputDialog.h"

TestEditInputDialog::TestEditInputDialog()
{
}

/**********************************************************************/
/*************************** Tests ************************************/
/**********************************************************************/
bool TestEditInputDialog::Test_SubjectColumnID()
{
    QSharedPointer< EditInputDialog > editInputDialog = QSharedPointer< EditInputDialog >( new EditInputDialog );
    int subjectColumnID = 10;
    int emitedSubjectColumnID;


    editInputDialog->m_subjectColumnIDSpinBox->setValue( subjectColumnID );
    editInputDialog->OnSubjectColumnIDChanged( editInputDialog->m_subjectColumnIDSpinBox->value() );
    emitedSubjectColumnID = editInputDialog->m_subjectColumnIDSpinBox->value() - 1;
    bool testOnSubjectColumnIDChanged = editInputDialog->m_subjectColumnID == subjectColumnID - 1;

    editInputDialog->ResetSubjectColumnID();
    bool testResetSubjectColumnID = editInputDialog->m_subjectColumnIDSpinBox->value() == 1;


    bool testSubjectColumnID_Passed = testOnSubjectColumnIDChanged && testResetSubjectColumnID;
    if( !testSubjectColumnID_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SubjectColumnID() FAILED /!\\ /!\\";
//        std::cerr << std::endl;
//        if( !testOnSubjectColumnIDChanged )
//        {
//            std::cerr << "\t+ pb with OnSubjectColumnIDChanged( int columnID )" << std::endl;
//            std::cerr << "\t  expected subject column ID to be emited: " << subjectColumnID - 1
//                      << " | subject column ID emited: " << emitedSubjectColumnID << std::endl;

//        }
//        if( !testResetSubjectColumnID )
//        {
//            std::cerr << "\t+ pb with ResetSubjectColumnID()" << std::endl;
//            std::cerr << "\t  expected m_subjectColumnIDSpinBox value: " << editInputDialog->m_subjectColumnID
//                      << " | subject column ID emited: " << 0 << std::endl;
//        }
    }
    else
    {
        std::cerr << "Test_SubjectColumnID() PASSED";
    }

    return testSubjectColumnID_Passed;
}


bool TestEditInputDialog::Test_OnDelete( QString adFilePath )
{
    QSharedPointer< EditInputDialog > editInputDialog = QSharedPointer< EditInputDialog >( new EditInputDialog );
    Data data;
    Processing processing;
    QList< QStringList > expectedFileData;
    int indexAD;
    int rowID = 1;
    int columnID = 5;


    data.InitData();
    editInputDialog->SetData( &data );
    indexAD = data.GetAxialDiffusivityIndex();
    data.SetFileData( indexAD ) = processing.GetDataFromFile( adFilePath );
    expectedFileData = data.GetFileData( indexAD );

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
        std::cerr << "/!\\/!\\ Test_OnDelete() FAILED /!\\ /!\\";
//        std::cerr << std::endl;
//        if( !testDeleteRow )
//        {
//            std::cerr << "\t+ pb with OnDeleteRows()" << std::endl;
//        }
//        if( !testDeleteColumn )
//        {
//            std::cerr << "\t+ pb with OnDeleteColumns()" << std::endl;
//        }
//        std::cerr << std::endl;
//        std::cerr << "/!\\ WARNING /!\\ Next EditInputDialog tests heavly rely on OnDeleteRows() and OnDeleteColumns().\n"
//                     "/!\\ WARNING /!\\ Therefore, results might not be accurate as long as Test_OnDelete() remains failed." << std::endl;
//        std::cerr << std::endl;
    }
    else
    {
        std::cerr << "Test_OnDelete() PASSED";
    }

    return testOnDelete_Passed;
}


bool TestEditInputDialog::Test_LoadDisplayData( QString adFilePath, QString subMatrixFilePath )
{
    QSharedPointer< EditInputDialog > editInputDialog = QSharedPointer< EditInputDialog >( new EditInputDialog );
    Data data;
    Processing processing;
    QList< QStringList > adData;
    QList< QStringList > subMatrixData;
    int indexAD;
    int indexSubMatrix;
    int IDRow = 2;
    int IDColumn = 2;


    data.InitData();
    editInputDialog->SetData( &data );
    indexAD = data.GetAxialDiffusivityIndex();
    indexSubMatrix = data.GetSubMatrixIndex();
    data.SetFileData( indexAD ) = processing.GetDataFromFile( adFilePath );
    data.SetFileData( indexSubMatrix ) = processing.GetDataFromFile( subMatrixFilePath );
    adData = data.GetFileData( indexAD );
    subMatrixData = data.GetFileData( indexSubMatrix );

    /************************************************/
    /******************** Test 1 ********************/
    /************************************************/
    /** AD, RD, MD or FA file in right QLineEdit **/
    editInputDialog->DisplayDataEdition( indexAD );

    bool testHiddenState1 = editInputDialog->m_subjectColumnIDSpinBox->isHidden();

    /************************************************/
    /******************** Test 2 ********************/
    /************************************************/
    /** AD, RD, MD or FA file in right QLineEdit
     *  after deleting 1 row and 1 column **/
    editInputDialog->m_dataTableWidget->selectRow( IDRow );
    editInputDialog->OnDeleteRows();
    editInputDialog->m_dataTableWidget->selectColumn( IDColumn );
    editInputDialog->OnDeleteColumns();

    bool testHiddenState2 = editInputDialog->m_subjectColumnIDSpinBox->isHidden();

    /************************************************/
    /******************** Test 3 ********************/
    /************************************************/
    /** AD, RD, MD or FA file in wrong QLineEdit **/
    data.SetFileData( indexSubMatrix ) = adData;
    editInputDialog->ResetTableWidget();
    //    editInputDialog->SetData( &data );
    editInputDialog->DisplayDataEdition( indexSubMatrix );

    bool testHiddenState3 = !editInputDialog->m_subjectColumnIDSpinBox->isHidden();

    /************************************************/
    /******************** Test 4 ********************/
    /************************************************/
    /** subMatrix file in right QLineEdit **/
    data.SetFileData( indexSubMatrix ) = subMatrixData;
    editInputDialog->ResetTableWidget();
    editInputDialog->DisplayDataEdition( indexSubMatrix );

    bool testHiddenState4 = !editInputDialog->m_subjectColumnIDSpinBox->isHidden();

    /************************************************/
    /******************** Test 5 ********************/
    /************************************************/
    /** subMatrix file in right QLineEdit after deleting 1 row
     *  and 1 column and changing the subjectColumnID **/
    editInputDialog->m_dataTableWidget->selectRow( IDRow );
    editInputDialog->OnDeleteRows();
    editInputDialog->m_dataTableWidget->selectColumn( IDColumn );
    editInputDialog->OnDeleteColumns();
    editInputDialog->m_subjectColumnIDSpinBox->setValue( 3 );

    bool testHiddenState5 = !editInputDialog->m_subjectColumnIDSpinBox->isHidden();

    /************************************************/
    /******************** Test 6 ********************/
    /************************************************/
    /** indexSubMatrixfile in wrong QLineEdit **/
    data.SetFileData( indexAD )= subMatrixData;
    editInputDialog->ResetTableWidget();
    editInputDialog->DisplayDataEdition( indexAD );

    bool testHiddenState6 = editInputDialog->m_subjectColumnIDSpinBox->isHidden();


    bool testLoadDisplayData_Passed = testHiddenState1 && testHiddenState2 && testHiddenState3 &&
            testHiddenState4 && testHiddenState5 && testHiddenState6;
    if( !testLoadDisplayData_Passed )
    {
        std::cerr << "/!\\/!\\ Test_LoadDisplayData() FAILED /!\\ /!\\";
//        std::cerr << std::endl << "\t+ pb with DisplayDataEdition( const int &newDiffusionPropertyIndex ) and/or with ResetTableWidget()" << std::endl;
//        if( !testHiddenState1 )
//        {
//            std::cerr << "\t  - When AD, RD, MD or FA file in right QLineEdit" << std::endl;
//            std::cerr << "\t    EditInputDialog_subjectColumnID_spinBox and EditInputDialog_subjectColumn_label not hidden" << std::endl;
//        }
//        if( !testHiddenState2 )
//        {
//            std::cerr << "\n\t  - When AD, RD, MD or FA file in right QLineEdit after deleting 1 row and 1 column" << std::endl;
//            std::cerr << "\t    EditInputDialog_subjectColumnID_spinBox and EditInputDialog_subjectColumn_label not hidden" << std::endl;
//        }
//        if( !testHiddenState3 )
//        {
//            std::cerr << "\n\t  - When AD, RD, MD or FA file in wrong QLineEdit" << std::endl;
//            std::cerr << "\t    QSpinBox hidden" << std::endl;
//        }
//        if( !testHiddenState4 )
//        {
//            std::cerr << "\n\t  - When subMatrix file in right QLineEdit" << std::endl;
//            std::cerr << "\t    QSpinBox hidden" << std::endl;
//        }
//        if( !testHiddenState5 )
//        {
//            std::cerr << "\n\t  - When indexSubMatrixfile in right QLineEdit after deleting 1 row and 1 column and changing the subjectColumnID" << std::endl;
//            std::cerr << "\t    QSpinBox hidden" << std::endl;
//        }
//        if( !testHiddenState6 )
//        {
//            std::cerr << "\n\t  - When subMatrix file in wrong QLineEdit" << std::endl;
//            std::cerr << "\t    EditInputDialog_subjectColumnID_spinBox and EditInputDialog_subjectColumn_label not hidden" << std::endl;
//        }
    }
    else
    {
        std::cerr << "Test_LoadDisplayData() PASSED";
    }

    return testLoadDisplayData_Passed;
}


bool TestEditInputDialog::Test_SaveFile( QString adFilePath, QString newADFile, QString tempoDir )
{
    QSharedPointer< EditInputDialog > editInputDialog = QSharedPointer< EditInputDialog >( new EditInputDialog );
    QString dirTest = tempoDir + "/TestEditInputDialog/Test_OnSaveFile";
    QDir().mkpath( dirTest );
    Data data;
    Processing processing;
    int indexAD;
    int rowID = 1;
    int columnID = 5;
    QString defaultSavedFilePath;


    data.InitData();
    editInputDialog->SetData( &data );
    indexAD = data.GetAxialDiffusivityIndex();
    data.SetFileData( indexAD ) = processing.GetDataFromFile( adFilePath );
    defaultSavedFilePath = dirTest + "/new" + data.GetDiffusionPropertyName( indexAD ).toUpper() + "File.csv";

    editInputDialog->DisplayDataEdition( indexAD );
    editInputDialog->m_dataTableWidget->selectRow( rowID );
    editInputDialog->OnDeleteRows();
    editInputDialog->m_dataTableWidget->selectColumn( columnID );
    editInputDialog->OnDeleteColumns();

    editInputDialog->m_data->SetFilename( indexAD ) = dirTest + "/adFile.csv";
    editInputDialog->SaveFile( defaultSavedFilePath );


    bool testOnSaveFile_Passed = CompareFile( defaultSavedFilePath, newADFile );
    if( !testOnSaveFile_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnSaveFile() FAILED /!\\ /!\\";
//        std::cerr << std::endl << "\t+ pb with OnSaveFile()" << std::endl;
//        if( !testFileSaved )
//        {
//            std::cerr << "\t  Error during new file creation" << std::endl;
//        }
//        if( !testFileWellSaved )
//        {
//            std::cerr << "\t  Data not correctly saved" << std::endl;
//        }
    }
    else
    {
        std::cerr << "Test_OnSaveFile() PASSED";
    }

    return testOnSaveFile_Passed;
}


/**********************************************************************/
/********************** Functions Used For Testing ********************/
/**********************************************************************/
bool TestEditInputDialog::TestQTableWidget( QList< QStringList > expectedFileData, QSharedPointer< EditInputDialog > editInputDialog )
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
