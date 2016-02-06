#include "TestEditInputDialog.h"

TestEditInputDialog::TestEditInputDialog()
{
}

/**********************************************************************/
/*************************** Tests ************************************/
/**********************************************************************/
bool TestEditInputDialog::Test_UploadFileData( QString adInputFile )
{
    Processing processing;

    Data data;
    data.InitData();
    QString ad = "ad";
    QList<QStringList> dataInadInputFile = processing.GetDataFromFile( adInputFile );
    data.SetFileData( ad ) = dataInadInputFile;

    QSharedPointer<EditInputDialog> editInputDialog = QSharedPointer<EditInputDialog>( new EditInputDialog );
    editInputDialog->SetData( &data );
    editInputDialog->DisplayDataEdition( ad, adInputFile );

    QList<QStringList> expectedFileData = processing.GetDataFromFile( adInputFile );

    bool testResult = TestQTableWidget( expectedFileData, editInputDialog );


    if( !testResult )
    {
        std::cerr << std::endl << "Test_UploadFileData() FAILED:" << std::endl;
        std::cerr << "\t+ Incorrect data uploaded in QTableWidget" << std::endl;
        std::cerr << std::endl;
    }
    else
    {
        std::cout << std::endl << "Test_UploadFileData() PASSED" << std::endl;
    }

    return testResult;
}


bool TestEditInputDialog::Test_Delete( QString adInputFile )
{
    Processing processing;

    Data data;
    data.InitData();
    QString ad = "ad";
    QList<QStringList> dataInadInputFile = processing.GetDataFromFile( adInputFile );
    data.SetFileData( ad ) = dataInadInputFile;

    QSharedPointer<EditInputDialog> editInputDialog = QSharedPointer<EditInputDialog>( new EditInputDialog );
    editInputDialog->SetData( &data );
    editInputDialog->DisplayDataEdition( ad, adInputFile );

    QList<QStringList> expectedFileData = processing.GetDataFromFile( adInputFile );

    int IDRow = 1;
    int IDColumn = 5;
    /************************************************/
    /**************** Test Delete Row ***************/
    /************************************************/
    editInputDialog->m_dataTableWidget->selectRow( IDRow );
    editInputDialog->OnDeleteRows();
    expectedFileData.removeAt( IDRow );

    bool testDeleteRow = TestQTableWidget( expectedFileData, editInputDialog );


    /************************************************/
    /************** Test Delete Column **************/
    /************************************************/
    editInputDialog->m_dataTableWidget->selectColumn( IDColumn );
    editInputDialog->OnDeleteColumns();
    for( int i = 0; i < expectedFileData.size(); i++ )
    {
        expectedFileData[ i ].removeAt( IDColumn );
    }

    bool testDeleteColumn = TestQTableWidget( expectedFileData, editInputDialog );


    if( !testDeleteRow || !testDeleteColumn )
    {
        std::cerr << std::endl << "Test_Delete() FAILED:" << std::endl;
        if( !testDeleteRow )
        {
            std::cerr << "\t+ Row not deleted correctly" << std::endl;

        }
        if( !testDeleteColumn )
        {
            std::cerr << "\t+ Column not deleted correctly" << std::endl;
        }
    }
    else
    {
        std::cout << std::endl << "Test_Delete() PASSED" << std::endl;
    }

    return ( testDeleteRow & testDeleteColumn );
}


bool TestEditInputDialog::Test_RefreshFileInfo( QString adInputFile, QString inputCOMPFile )
{
//    Processing processing;

//    Data data;
//    data.InitData();
//    QString ad = "ad";
//    QList<QStringList> dataInadInputFile = processing.GetDataFromFile( adInputFile );
//    data.SetFileData( ad ) = dataInadInputFile;
//    QString COMP = "COMP";
//    QList<QStringList> dataInCOMPInputFile = processing.GetDataFromFile( inputCOMPFile );

//    QSharedPointer<EditInputDialog> editInputDialog = QSharedPointer<EditInputDialog>( new EditInputDialog );

//    int IDRow = 2;
//    int IDColumn = 2;
//    /************************************************/
//    /******************** Test 1 ********************/
//    /************************************************/
//    /** AD, RD, MD or FA file in right QLineEdit **/
//    editInputDialog->SetData( &data );
//    editInputDialog->SetLineEditPrefix( ad );
//    editInputDialog->SetInputFile( adInputFile );
//    editInputDialog->DisplayFileData();

//    bool hiddenStateSpinBoxTest1 = editInputDialog->m_covariateFileSubjectColumnIDSpinBox->isHidden();
//    bool hiddenStateSubjectColumnLabelTest1 = editInputDialog->m_covariateFileSubjectColumnIDLabel->isHidden();

//    QList<QStringList> ADexpectedFileData = processing.GetDataFromFile( adInputFile );
//    QString ADfileInfoOK;
//    ADfileInfoOK.append( "<br><br><b>Filename</b> " + QFileInfo( QFile( adInputFile ) ).fileName() + "<br>" );
//    ADfileInfoOK.append( "<br><b>Number of test subjects</b>  " + QString::number( ADexpectedFileData.at( 0 ).size()-1 ) + "<br>" );
//    ADfileInfoOK.append( "<br><b>Data matrix</b>  " + QString::number( ADexpectedFileData.size()-1 ) + "x" + QString::number( ADexpectedFileData.at( 0 ).size() ) );
//    QString fileInfoTest1 = editInputDialog->m_fileInformationLabel->text();

//    bool fileInfoLabelTest1 = fileInfoTest1 == ADfileInfoOK;


//    /************************************************/
//    /******************** Test 2 ********************/
//    /************************************************/
//    /** AD, RD, MD or FA file in right QLineEdit
//     *  after deleting 1 row and 1 column **/
//    editInputDialog->m_dataTableWidget->selectRow( IDRow );
//    editInputDialog->DeleteRows();
//    editInputDialog->m_dataTableWidget->selectColumn( IDColumn );
//    editInputDialog->DeleteColumns();

//    bool hiddenStateSpinBoxTest2 = editInputDialog->m_covariateFileSubjectColumnIDSpinBox->isHidden();
//    bool hiddenStateSubjectColumnLabelTest2 = editInputDialog->m_covariateFileSubjectColumnIDLabel->isHidden();

//    QString ADfileInfoOKDelete;
//    ADfileInfoOKDelete.append( "<br><br><b>Filename</b> " + QFileInfo( QFile( adInputFile ) ).fileName() + "<br>" );
//    ADfileInfoOKDelete.append( "<br><b>Number of test subjects</b>  " + QString::number( ADexpectedFileData.at( 0 ).size()-2 ) + "<br>" );
//    ADfileInfoOKDelete.append( "<br><b>Data matrix</b>  " + QString::number( ADexpectedFileData.size()-2 ) + "x" + QString::number( ADexpectedFileData.at( 0 ).size()-1 ) );
//    QString fileInfoTest2 = editInputDialog->m_fileInformationLabel->text();

//    bool fileInfoLabelTest2 = fileInfoTest2 == ADfileInfoOKDelete;


//    /************************************************/
//    /******************** Test 3 ********************/
//    /************************************************/
//    data.SetFileData( COMP ) = dataInadInputFile;
//    /** AD, RD, MD or FA file in wrong QLineEdit **/
//    editInputDialog->ResetTableWidget();
//    editInputDialog->SetData( &data );
//    editInputDialog->SetLineEditPrefix( COMP );
//    editInputDialog->SetInputFile( adInputFile );
//    editInputDialog->DisplayFileData();

//    bool hiddenStateSpinBoxTest3 = !editInputDialog->m_covariateFileSubjectColumnIDSpinBox->isHidden();
//    bool hiddenStateSubjectColumnLabelTest3 = !editInputDialog->m_covariateFileSubjectColumnIDLabel->isHidden();

//    QString ADfileInfoKO;
//    ADfileInfoKO.append( "<br><br><b>Filename</b> " + QFileInfo( QFile( adInputFile ) ).fileName() + "<br>" );
//    ADfileInfoKO.append( "<br><center><i>WARNING<br><br>Please make sure you have uploaded<br>a " + editInputDialog->m_lineEditPrefix.toUpper() + " file</i></center>" );
//    QString fileInfoTest3 = editInputDialog->m_fileInformationLabel->text();

//    bool fileInfoLabelTest3 = fileInfoTest3 == ADfileInfoKO;


//    /************************************************/
//    /******************** Test 4 ********************/
//    /************************************************/
//    /** COMP file in right QLineEdit **/
//    data.SetFileData( COMP ) = dataInCOMPInputFile;
//    editInputDialog->ResetTableWidget();
//    editInputDialog->SetData( &data );
//    editInputDialog->SetLineEditPrefix( COMP );
//    editInputDialog->SetInputFile( inputCOMPFile );
//    editInputDialog->DisplayFileData();

//    bool hiddenStateSpinBoxTest4 = !editInputDialog->m_covariateFileSubjectColumnIDSpinBox->isHidden();
//    bool hiddenStateSubjectColumnLabelTest4 = !editInputDialog->m_covariateFileSubjectColumnIDLabel->isHidden();

//    QList<QStringList> COMPexpectedFileData = processing.GetDataFromFile( inputCOMPFile );
//    QString COMPfileInfoOK;
//    COMPfileInfoOK.append( "<br><br><b>Filename</b> " + QFileInfo( QFile( inputCOMPFile ) ).fileName() + "<br>" );
//    COMPfileInfoOK.append( "<br><b>Number of test subjects</b>  " + QString::number( COMPexpectedFileData.size()-1 ) + "<br>" );
//    COMPfileInfoOK.append( "<br><b>Data matrix</b>  " + QString::number( COMPexpectedFileData.size()-1 ) + "x" + QString::number( COMPexpectedFileData.at( 0 ).size()-1 ) + "<br>" );
//    COMPfileInfoOK.append( "<br><b>Number of covariates</b>  " + QString::number( COMPexpectedFileData.at( 0 ).size()-1 ) );
//    for( int i = 0; i < COMPexpectedFileData.at( 0 ).size(); i++ )
//    {
//        if( i != editInputDialog->m_covariateFileSubjectColumnIDSpinBox->value()-1 )
//        {
//            COMPfileInfoOK.append( "<br>-  " + COMPexpectedFileData.at( 0 ).at( i ) );
//        }
//    }
//    QString fileInfoTest4 = editInputDialog->m_fileInformationLabel->text();

//    bool fileInfoLabelTest4 = fileInfoTest4 == COMPfileInfoOK;


//    /************************************************/
//    /******************** Test 5 ********************/
//    /************************************************/
//    /** COMP file in right QLineEdit after deleting 1 row
//     *  and 1 column and changing the subjectColumnID **/
//    editInputDialog->m_dataTableWidget->selectRow( IDRow );
//    editInputDialog->DeleteRows();
//    editInputDialog->m_dataTableWidget->selectColumn( IDColumn );
//    editInputDialog->DeleteColumns();
//    editInputDialog->m_covariateFileSubjectColumnIDSpinBox->setValue( 3 );

//    bool hiddenStateSpinBoxTest5 = !editInputDialog->m_covariateFileSubjectColumnIDSpinBox->isHidden();
//    bool hiddenStateSubjectColumnLabelTest5 = !editInputDialog->m_covariateFileSubjectColumnIDLabel->isHidden();

//    QString COMPfileInfoOKDelete;
//    COMPfileInfoOKDelete.append( "<br><br><b>Filename</b> " + QFileInfo( QFile( inputCOMPFile ) ).fileName() + "<br>" );
//    COMPfileInfoOKDelete.append( "<br><b>Number of test subjects</b>  " + QString::number( COMPexpectedFileData.size()-2 ) + "<br>" );
//    COMPfileInfoOKDelete.append( "<br><b>Data matrix</b>  " + QString::number( COMPexpectedFileData.size()-2 ) + "x" + QString::number( COMPexpectedFileData.at( 0 ).size()-2 ) + "<br>" );
//    COMPfileInfoOKDelete.append( "<br><b>Number of covariates</b>  " + QString::number( COMPexpectedFileData.at( 0 ).size()-2 ) );
//    for( int i = 0; i < COMPexpectedFileData.at( 0 ).size()-1; i++ )
//    {
//        if( ( i != editInputDialog->m_covariateFileSubjectColumnIDSpinBox->value()-1 ) && ( i != IDColumn ) )
//        {
//            COMPfileInfoOKDelete.append( "<br>-  " + COMPexpectedFileData.at( 0 ).at( i ) );
//        }
//    }
//    QString fileInfoTest5 = editInputDialog->m_fileInformationLabel->text();

//    bool fileInfoLabelTest5 = fileInfoTest5 == COMPfileInfoOKDelete;


//    /************************************************/
//    /******************** Test 6 ********************/
//    /************************************************/
//    /** COMP file in right QLineEdit after deleting all ROWS **/
//    editInputDialog->m_dataTableWidget->selectColumn( IDColumn );
//    editInputDialog->DeleteRows();

//    QString COMPfileInfoOKDeleteAll;
//    COMPfileInfoOKDeleteAll.append( "<br><br><b>Filename</b> " + QFileInfo( QFile( inputCOMPFile ) ).fileName() + "<br>" );
//    COMPfileInfoOKDeleteAll.append( "<br>No data to display" );
//    QString fileInfoTest6 = editInputDialog->m_fileInformationLabel->text();

//    bool fileInfoLabelTest6 = fileInfoTest6 == COMPfileInfoOKDeleteAll;


//    /************************************************/
//    /******************** Test 7 ********************/
//    /************************************************/
//    /**  COMP file in wrong QLineEdit **/
//    data.SetFileData( ad )= dataInCOMPInputFile;
//    editInputDialog->ResetTableWidget();
//    editInputDialog->SetData( &data );
//    editInputDialog->SetLineEditPrefix( ad );
//    editInputDialog->SetInputFile( inputCOMPFile );
//    editInputDialog->DisplayFileData();

//    bool hiddenStateSpinBoxTest7 = editInputDialog->m_covariateFileSubjectColumnIDSpinBox->isHidden();
//    bool hiddenStateSubjectColumnLabelTest7 = editInputDialog->m_covariateFileSubjectColumnIDLabel->isHidden();

//    QString COMPfileInfoKO;
//    COMPfileInfoKO.append( "<br><br><b>Filename</b> " + QFileInfo( QFile( inputCOMPFile ) ).fileName() + "<br>" );
//    COMPfileInfoKO.append( "<br><center><i>WARNING<br><br>Please make sure you have uploaded<br>a(n) " + editInputDialog->m_lineEditPrefix.toUpper() + " file</i></center>" );
//    QString fileInfoTest7 = editInputDialog->m_fileInformationLabel->text();

//    bool fileInfoLabelTest7 = fileInfoTest7 == COMPfileInfoKO;


//    if( !hiddenStateSpinBoxTest1 || !hiddenStateSubjectColumnLabelTest1 || !fileInfoLabelTest1 ||
//            !hiddenStateSpinBoxTest2 || !hiddenStateSubjectColumnLabelTest2 || !fileInfoLabelTest2 ||
//            !hiddenStateSpinBoxTest3 || !hiddenStateSubjectColumnLabelTest3 || !fileInfoLabelTest3 ||
//            !hiddenStateSpinBoxTest4 || !hiddenStateSubjectColumnLabelTest4 || !fileInfoLabelTest4 ||
//            !hiddenStateSpinBoxTest5 || !hiddenStateSubjectColumnLabelTest5 || !fileInfoLabelTest5 ||
//            !fileInfoLabelTest6 ||
//            !hiddenStateSpinBoxTest7 || !hiddenStateSubjectColumnLabelTest7 || !fileInfoLabelTest7 )
//    {
//        std::cerr << std::endl << "Test_RefreshFileInfo() FAILED:" << std::endl;
//        if( !hiddenStateSpinBoxTest1 || !hiddenStateSubjectColumnLabelTest1 || !fileInfoLabelTest1 )
//        {
//            std::cerr << "\t+ When AD, RD, MD or FA file in right QLineEdit" << std::endl;
//            if( !hiddenStateSpinBoxTest1 )
//            {
//                std::cerr << "\t   QSpinBox not hidden" << std::endl;
//            }
//            if( !hiddenStateSubjectColumnLabelTest1 )
//            {
//                std::cerr << "\t   subjectColumnLabel not hidden" << std::endl;
//            }
//            if( !fileInfoLabelTest1 )
//            {
//                DisplayErrorFileInfoDisplayed( ADfileInfoOK, fileInfoTest1 );
//            }
//        }
//        if( !hiddenStateSpinBoxTest2 || !hiddenStateSubjectColumnLabelTest2 || !fileInfoLabelTest2 )
//        {
//            std::cerr << "\t+ When AD, RD, MD or FA file in right QLineEdit after deleting 1 row and 1 column" << std::endl;
//            if( !hiddenStateSpinBoxTest2 )
//            {
//                std::cerr << "\t   QSpinBox not hidden" << std::endl;
//            }
//            if( !hiddenStateSubjectColumnLabelTest2 )
//            {
//                std::cerr << "\t   subjectColumnLabel not hidden" << std::endl;
//            }
//            if( !fileInfoLabelTest2 )
//            {
//                DisplayErrorFileInfoDisplayed( ADfileInfoOKDelete, fileInfoTest2 );
//            }
//        }
//        if( !hiddenStateSpinBoxTest3 || !hiddenStateSubjectColumnLabelTest3 || !fileInfoLabelTest3 )
//        {
//            std::cerr << "\t+ When AD, RD, MD or FA file in wrong QLineEdit" << std::endl;
//            if( !hiddenStateSpinBoxTest3 )
//            {
//                std::cerr << "\t   QSpinBox hidden" << std::endl;
//            }
//            if( !hiddenStateSubjectColumnLabelTest3 )
//            {
//                std::cerr << "\t   subjectColumnLabel hidden" << std::endl;
//            }
//            if( !fileInfoLabelTest3 )
//            {
//                DisplayErrorFileInfoDisplayed( ADfileInfoKO, fileInfoTest3 );
//            }
//        }
//        if( !hiddenStateSpinBoxTest4 || !hiddenStateSubjectColumnLabelTest4 || !fileInfoLabelTest4 )
//        {
//            std::cerr << "\t+ When COMP file in right QLineEdit" << std::endl;
//            if( !hiddenStateSpinBoxTest4 )
//            {
//                std::cerr << "\t   QSpinBox hidden" << std::endl;
//            }
//            if( !hiddenStateSubjectColumnLabelTest4 )
//            {
//                std::cerr << "\t   subjectColumnLabel hidden" << std::endl;
//            }
//            if( !fileInfoLabelTest4 )
//            {
//                DisplayErrorFileInfoDisplayed( COMPfileInfoOK, fileInfoTest4 );
//            }
//        }
//        if( !hiddenStateSpinBoxTest5 || !hiddenStateSubjectColumnLabelTest5 || !fileInfoLabelTest5 )
//        {
//            std::cerr << "\t+ When Comp file in right QLineEdit after deleting 1 row and 1 column and changing the subjectColumnID" << std::endl;
//            if( !hiddenStateSpinBoxTest5 )
//            {
//                std::cerr << "\t   QSpinBox hidden" << std::endl;
//            }
//            if( !hiddenStateSubjectColumnLabelTest5 )
//            {
//                std::cerr << "\t   subjectColumnLabel hidden" << std::endl;
//            }
//            if( !fileInfoLabelTest5 )
//            {
//                DisplayErrorFileInfoDisplayed( COMPfileInfoOKDelete, fileInfoTest5 );
//            }
//        }
//        if( !fileInfoLabelTest6 )
//        {
//            std::cerr << "\t+ When Comp file in right QLineEdit after deleting all ROWS" << std::endl;
//            DisplayErrorFileInfoDisplayed( COMPfileInfoOKDeleteAll, fileInfoTest6 );

//        }
//        if( !hiddenStateSpinBoxTest7 || !hiddenStateSubjectColumnLabelTest7 || !fileInfoLabelTest7 )
//        {
//            std::cerr << "\t+ When COMP file in wrong QLineEdit" << std::endl;
//            if( !hiddenStateSpinBoxTest7 )
//            {
//                std::cerr << "\t   QSpinBox not hidden" << std::endl;
//            }
//            if( !hiddenStateSubjectColumnLabelTest7 )
//            {
//                std::cerr << "\t   subjectColumnLabel not hidden" << std::endl;
//            }
//            if( !fileInfoLabelTest7 )
//            {
//                DisplayErrorFileInfoDisplayed( COMPfileInfoKO, fileInfoTest7 );
//            }
//        }
//    }
//    else
//    {
//        std::cout << std::endl << "Test_RefreshFileInfo() PASSED" << std::endl;
//    }

//    return ( hiddenStateSpinBoxTest1 & hiddenStateSubjectColumnLabelTest1 & fileInfoLabelTest1 &
//             hiddenStateSpinBoxTest2 & hiddenStateSubjectColumnLabelTest2 & fileInfoLabelTest2 &
//             hiddenStateSpinBoxTest3 & hiddenStateSubjectColumnLabelTest3 & fileInfoLabelTest3 &
//             hiddenStateSpinBoxTest4 & hiddenStateSubjectColumnLabelTest4 & fileInfoLabelTest4 &
//             hiddenStateSpinBoxTest5 & hiddenStateSubjectColumnLabelTest5 & fileInfoLabelTest5 &
//             fileInfoLabelTest6 &
//             hiddenStateSpinBoxTest7 & hiddenStateSubjectColumnLabelTest7 & fileInfoLabelTest7 );
    return false;
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


void TestEditInputDialog::DisplayErrorFileInfoDisplayed( QString infoExpected, QString infoDisplayed )
{
    std::cerr << "\t   Wrong file info displayed" << std::endl;
    std::cerr << "\t\t- Info expected" << std::endl;
    std::cerr << "\t\t  " << infoExpected.toStdString() << std::endl;
    std::cerr << "\t\t- Info displayed" << std::endl;
    std::cerr << "\t\t  " << infoDisplayed.toStdString() << std::endl;
}
