#include "FADTTSWindow.h"

/****************************************************************/
/******************** Configuration & Events ********************/
/****************************************************************/
const QString FADTTSWindow::m_csvSeparator = QLocale().groupSeparator(); /** Retrieve platform separator ("," ";" ...) **/

const QColor FADTTSWindow::m_green = QColor( 0,255,0,127 );
const QColor FADTTSWindow::m_red = QColor( 255,0,0,127 );
const QColor FADTTSWindow::m_grey = QColor( 220,220,220,255 );
const QColor FADTTSWindow::m_yellow = QColor( 255,255,0,127 );
const QColor FADTTSWindow::m_lightBlack = QColor( 0,0,0,191 );

const int FADTTSWindow::m_iconSize = 12;


/********************** Public  functions ***********************/
FADTTSWindow::FADTTSWindow()
{
    m_okPixmap = QPixmap( ":/Icons/Resources/Icons/okIconOut.xpm" );
    m_koPixmap = QPixmap( ":/Icons/Resources/Icons/koIconOut.xpm" );
    m_warningPixmap = QPixmap( ":/Icons/Resources/Icons/warningIconOut.xpm" );

    InitFADTTSWindow();
}

FADTTSWindow::~FADTTSWindow()
{
    m_editInputDialog.clear();
    m_infoDialog.clear();
    delete m_sortedSubjectListWidget;
    delete m_covariateListWidget;
}


/*********************** Private slots ***********************/
void FADTTSWindow::SaveConfiguration()
{
    QString dir;
    QString filename = QFileDialog::getSaveFileName( this , tr( "Save Configuration" ) ,  dir , tr( ".xml( *.xml ) ;; .*( * )" ) );
    if( !filename.isEmpty() )
    {
        QFileInfo fi( filename ) ;
        dir = fi.dir().absolutePath() ;
        Save_Parameter_Configuration( filename.toStdString() );
    }
}

void FADTTSWindow::LoadConfiguration()
{
    QString dir;
    QString filename = QFileDialog::getOpenFileName( this , tr( "Load Configuration" ) , dir , tr( ".xml( *.xml ) ;; .*( * )" ) );
    if( !filename.isEmpty() )
    {
        QFileInfo fi( filename ) ;
        dir = fi.dir().absolutePath() ;
        Load_Parameter_Configuration( filename.toStdString() );
    }
}

void FADTTSWindow::DisplayAbout()
{
    QString messageBoxTitle = "About " + QString( FADTTS_TITLE );
    QString aboutFADTTS;
    std::cout<<FADTTS_DESCRIPTION<<std::endl;
    aboutFADTTS = "<b>Version:</b> " + QString( FADTTS_VERSION ) + "<br>"
            "<b>Description:</b> " + QString( FADTTS_DESCRIPTION ) + "<br>"
            "<b>Contributors:</b> " + QString( FADTTS_CONTRIBUTORS );
    QMessageBox::information( this, tr( qPrintable( messageBoxTitle ) ), tr( qPrintable( aboutFADTTS ) ), QMessageBox::Ok );
}


void FADTTSWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton closeMBox =
            QMessageBox::question( this, tr( "FADTTS" ), tr( "You are about to kill the running process.<br>Are you sure you want to continue?" ),
                                   QMessageBox::No | QMessageBox::Yes, QMessageBox::No );

    switch( closeMBox )
    {
    case QMessageBox::No:
        event->ignore();
        break;
    case QMessageBox::Yes:
    {
        /*******************************************/
        /*************** STOP THREAD ***************/
        /*******************************************/
        //        if( m_thread->isRunning() )
        //        {
        //            m_thread->terminate();
        //        }
        /*******************************************/
        /*************** STOP THREAD ***************/
        /*******************************************/
        event->accept();
        break;
    }
    default:
        event->accept();
        break;
    }
}


/*********************** Private function **********************/
void FADTTSWindow::InitFADTTSWindow()
{
    int nbrDuplicates = m_data.InitData();
    if( nbrDuplicates != 0 )  /** Check if a prefix has been entered twice **/
    {
        QString criticalMessage = "Be careful, you have " + QString::number( nbrDuplicates ) + " duplicates<br>" +
                "in your file prefix list.<br>The application may not work properly.";
        CriticalPopUp( criticalMessage );
    }

    InitMenuBar();
    InitInputTab();
    InitSubjectTab();
    InitParameterTab();
    InitRunTab();

    DisplayCovariates( m_data.GetCovariates() );

    UpdateAvailableFileParamTab();
}

void FADTTSWindow::InitMenuBar()
{
    connect( this->actionLoad_Settings, SIGNAL( triggered() ), SLOT( LoadConfiguration() ) );
    connect( this->actionSave_Settings, SIGNAL( triggered() ), SLOT( SaveConfiguration() ) );
    connect( this->actionAbout, SIGNAL( triggered() ), SLOT( DisplayAbout() ) );

}

void FADTTSWindow::InitInputTab()
{
    m_editInputDialog = QSharedPointer<EditInputDialog>( new EditInputDialog );
    m_editInputDialog->SetData( &m_data );

    m_infoDialog = QSharedPointer<InfoDialog>( new InfoDialog );
    m_infoDialog->SetData( &m_data );


    connect( this->inputTab_addMultipleInputFiles_pushButton, SIGNAL( clicked() ), this, SLOT( AddMultipleInputFiles() ) );

    /** Map of PushButtons to add each file separetely and
     *  SignalMapper to link them to the slot AddFile() **/
    m_inputTabAddInputFilePushButtonMap.insert( m_data.GetAxialDiffusivityPrefix(), this->inputTab_addADFile_pushButton );
    m_inputTabAddInputFilePushButtonMap.insert( m_data.GetRadialDiffusivityPrefix(), this->inputTab_addRDFile_pushButton );
    m_inputTabAddInputFilePushButtonMap.insert( m_data.GetMeanDiffusivityPrefix(), this->inputTab_addMDFile_pushButton );
    m_inputTabAddInputFilePushButtonMap.insert( m_data.GetFractionalAnisotropyPrefix(), this->inputTab_addFAFile_pushButton );
    m_inputTabAddInputFilePushButtonMap.insert( m_data.GetCovariatePrefix(), this->inputTab_addCovariateFile_pushButton );
    QSignalMapper *signalMapperAddFile = new QSignalMapper( this );
    connect( signalMapperAddFile, SIGNAL( mapped( const QString& ) ), this, SLOT( AddInputFile( const QString& ) ) );
    for( int i = 0; i < m_data.GetPrefixList().size(); ++i )
    {
        connect( m_inputTabAddInputFilePushButtonMap[ m_data.GetPrefixList().at( i ) ], SIGNAL( clicked() ), signalMapperAddFile,SLOT(map() ) );
        signalMapperAddFile->setMapping( m_inputTabAddInputFilePushButtonMap[ m_data.GetPrefixList().at( i ) ], m_data.GetPrefixList().at( i ) );
    }

    /** Map of LineEdits where the file path of each file is set and
     *  SignalMapper to link them to the slot UpdateInputLineEdit() **/
    m_inputTabInputFileLineEditMap.insert( m_data.GetAxialDiffusivityPrefix(), this->para_inputTab_adFile_lineEdit );
    m_inputTabInputFileLineEditMap.insert( m_data.GetRadialDiffusivityPrefix(), this->para_inputTab_rdFile_lineEdit );
    m_inputTabInputFileLineEditMap.insert( m_data.GetMeanDiffusivityPrefix(), this->para_inputTab_mdFile_lineEdit );
    m_inputTabInputFileLineEditMap.insert( m_data.GetFractionalAnisotropyPrefix(), this->para_inputTab_faFile_lineEdit );
    m_inputTabInputFileLineEditMap.insert( m_data.GetCovariatePrefix(), this->para_inputTab_covariateFile_lineEdit );
    QSignalMapper *signalMapperUpdateLineEdit = new QSignalMapper( this );
    connect( signalMapperUpdateLineEdit, SIGNAL( mapped( const QString& ) ), this, SLOT( UpdateInputLineEdit( const QString& ) ) );
    for( int i = 0; i < m_data.GetPrefixList().size(); ++i )
    {
        connect( m_inputTabInputFileLineEditMap[ m_data.GetPrefixList().at( i ) ], SIGNAL( textChanged( const QString& ) ), signalMapperUpdateLineEdit,SLOT(map() ) );
        signalMapperUpdateLineEdit->setMapping( m_inputTabInputFileLineEditMap[ m_data.GetPrefixList().at( i ) ], m_data.GetPrefixList().at( i ) );
    }

    /** Map of Labels to set the icon information of each file entered in a LineEdit **/
    m_inputTabIconLabelMap.insert( m_data.GetAxialDiffusivityPrefix(), this->inputTab_iconADFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetRadialDiffusivityPrefix(), this->inputTab_iconRDFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetMeanDiffusivityPrefix(), this->inputTab_iconMDFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetFractionalAnisotropyPrefix(), this->inputTab_iconFAFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetCovariatePrefix(), this->inputTab_iconCovariateFile_label );

    /** Map of PushButtons to edit the files and
     *  SignalMapper to link them to the slot EditFile() **/
    m_inputTabEditInputFilePushButtonMap.insert( m_data.GetAxialDiffusivityPrefix(), this->inputTab_editADFile_pushButton );
    m_inputTabEditInputFilePushButtonMap.insert( m_data.GetRadialDiffusivityPrefix(), this->inputTab_editRDFile_pushButton );
    m_inputTabEditInputFilePushButtonMap.insert( m_data.GetMeanDiffusivityPrefix(), this->inputTab_editMDFile_pushButton );
    m_inputTabEditInputFilePushButtonMap.insert( m_data.GetFractionalAnisotropyPrefix(), this->inputTab_editFAFile_pushButton );
    m_inputTabEditInputFilePushButtonMap.insert( m_data.GetCovariatePrefix(), this->inputTab_editCovariateFile_pushButton );
    QSignalMapper *signalMapperEditFile = new QSignalMapper( this );
    connect( signalMapperEditFile, SIGNAL( mapped( const QString& ) ), this, SLOT( EditInputFile( const QString& ) ) );
    for( int i = 0; i < m_data.GetPrefixList().size(); ++i )
    {
        connect( m_inputTabEditInputFilePushButtonMap[ m_data.GetPrefixList().at( i ) ], SIGNAL( clicked() ), signalMapperEditFile, SLOT(map() ) );
        signalMapperEditFile->setMapping( m_inputTabEditInputFilePushButtonMap[ m_data.GetPrefixList().at( i ) ], m_data.GetPrefixList().at( i ) );
    }

    connect( this->inputTab_fileInformation_pushButton, SIGNAL( clicked() ), this, SLOT( DisplayInputFileInformation() ) );

    /** Signal/Slot connection to receive updates from m_editInputDialog **/
    connect( m_editInputDialog.data(), SIGNAL( FilePathChanged( const QString&, const QString& ) ), this, SLOT( UpdateInputLineEditAfterFileEdition( const QString& , const QString& ) ) );
    connect( m_editInputDialog.data(), SIGNAL( CovariateMapChanged( const QMap<int, QString>& ) ), this, SLOT( UpdateCovariateMapAfterFileEdition( const QMap<int, QString>& ) ) );
    connect( m_editInputDialog.data(), SIGNAL( CovariateFileSubjectColumnIDChanged( const int& ) ), this, SLOT( UpdateCovariateFileSubjectColumnIDAfterFileEdition( int ) ) );
}

void FADTTSWindow::InitSubjectTab()
{
    m_sortedSubjectListWidget = new QListWidget();
    m_sortedSubjectListWidget = this->subjectTab_sortedSubjects_listWidget;

    m_subjectFileLineEdit = new QLineEdit();
    m_subjectFileLineEdit = this->para_subjectTab_subjectFile_lineEdit;

    /** Map of CheckBoxes to select the files we want to work on and
     *  SignalMapper to link them to the slot SortSubjects() **/
    m_paramTabFileCheckBoxMap.insert( m_data.GetAxialDiffusivityPrefix(), this->para_subjectTab_adFile_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetRadialDiffusivityPrefix(), this->para_subjectTab_rdFile_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetMeanDiffusivityPrefix(), this->para_subjectTab_mdFile_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetFractionalAnisotropyPrefix(), this->para_subjectTab_faFile_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetCovariatePrefix(), this->para_subjectTab_covariateFile_checkBox );
    QSignalMapper *signalMapperSelectFile = new QSignalMapper( this );
    connect( signalMapperSelectFile, SIGNAL( mapped( const QString& ) ), this, SLOT( SortAndDisplaySubjects() ) );
    for ( int i = 0; i < m_data.GetPrefixList().size(); ++i )
    {
        connect( m_paramTabFileCheckBoxMap[ m_data.GetPrefixList().at( i ) ], SIGNAL( toggled( const bool& ) ), signalMapperSelectFile,SLOT( map() ) );
        signalMapperSelectFile->setMapping( m_paramTabFileCheckBoxMap[ m_data.GetPrefixList().at( i ) ], m_data.GetPrefixList().at( i ) );
    }

    /** Map of Labels displaying the matrix data size of the files that have been chosen **/
    m_paramTabFileDataSizeLabelMap.insert( m_data.GetAxialDiffusivityPrefix(), this->subjectTab_adFileInfo_label );
    m_paramTabFileDataSizeLabelMap.insert( m_data.GetRadialDiffusivityPrefix(),this->subjectTab_rdFileInfo_label );
    m_paramTabFileDataSizeLabelMap.insert( m_data.GetMeanDiffusivityPrefix(), this->subjectTab_mdFileInfo_label );
    m_paramTabFileDataSizeLabelMap.insert( m_data.GetFractionalAnisotropyPrefix(), this->subjectTab_faFileInfo_label );
    m_paramTabFileDataSizeLabelMap.insert( m_data.GetCovariatePrefix(), this->subjectTab_covariateFileInfo_label );


    connect( this->subjectTab_loadSubjectFile_PushButton, SIGNAL( clicked() ), this, SLOT( LoadSubjectFile() ) );
    connect( this->subjectTab_resetSubjectFile_pushButton, SIGNAL( clicked() ), this, SLOT( ResetSubjectFile() ) );
    connect( m_subjectFileLineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( UpdateSubjectFileLineEdit( const QString&  ) ) );
    connect( this->subjectTab_saveCheckedSubjects_pushButton, SIGNAL( clicked() ), this, SLOT( SaveCheckedSubjects() ) );

    connect( this->subjectTab_checkAllVisible_pushButton, SIGNAL( clicked() ), this, SLOT( CheckAllSubjectsVisible() ) );
    connect( this->subjectTab_unCheckAllVisible_pushButton, SIGNAL( clicked() ), this, SLOT( UnCheckAllSubjectsVisible() ) );
    connect( this->subjectTab_sortedSubjects_listWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( SelectSubject( QListWidgetItem * ) ) );
    connect( this->subjectTab_search_lineEdit, SIGNAL( textEdited( const QString& ) ), this, SLOT( SearchSubjects() ) );
    connect( this->subjectTab_caseSensitive_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( SetCaseSensitivity( bool ) ) );
}

void FADTTSWindow::InitParameterTab()
{
    m_covariateListWidget = new QListWidget();
    m_covariateListWidget = this->para_parameterTab_covariates_listWidget;

    connect( this->para_parameterTab_covariates_listWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( SelectCovariate( QListWidgetItem * ) ) );
    connect( this->parameterTab_covariatesCheckAll_pushButton, SIGNAL( clicked() ), this, SLOT( CheckAllCovariates() ) );
    connect( this->parameterTab_covariatesUncheckAll_pushButton, SIGNAL( clicked() ), this, SLOT( UnCheckAllCovariates() ) );

    this->para_parameterTab_nbrPermutations_spinBox->setMaximum( 2000 );
}

void FADTTSWindow::InitRunTab()
{
    connect( this->runTab_outputDir_pushButton, SIGNAL( clicked() ), this, SLOT( SetOutputDir() ) );
    connect( this->para_runTab_outputDir_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( UpdateOutputDirLineEdit( const QString& ) ) );

    connect( this->runTab_matlab_pushButton, SIGNAL( clicked() ), this, SLOT( SetMatlabExe() ) );
    connect( this->para_runTab_matlab_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( UpdateMatlabExeLineEdit( const QString& ) ) );

    connect( this->runTab_run_pushButton, SIGNAL( clicked() ), this, SLOT( RunFADTTS() ) );
}


void FADTTSWindow::UpdateEditInputDialogCurrentDir( const QString newfilePath )
{

    QDir dir = UpdateCurrentDir( newfilePath, m_currentInputFileDir );
    if( dir.exists() )
    {
        m_editInputDialog->SetCurrentInputDir( dir.absolutePath() );
    }
}

QDir FADTTSWindow::UpdateCurrentDir( const QString newfilePath, QString& currentDir )
{
    QDir dir = QFileInfo( QFile( newfilePath ) ).absolutePath();
    if( dir.exists() )
    {
        currentDir = dir.absolutePath();
    }

    return dir;
}

void FADTTSWindow::SetDir( QDir& dir, QString filePath, QString currentDir )
{
    if( !filePath.isEmpty() )
    {
        dir = QFileInfo( QFile( filePath ) ).absolutePath();
        if( !dir.exists() )
        {
            dir = currentDir;
        }
    }
    else
    {
        dir = currentDir;
    }
}


void FADTTSWindow::WarningPopUp( const QString warningMessage )
{
    QMessageBox::warning( this, tr( "WARNING" ), tr( qPrintable( warningMessage ) ), QMessageBox::Ok );
}

void FADTTSWindow::CriticalPopUp( const QString criticalMessage )
{
    QMessageBox::critical( this, tr( "CRITICAL ERROR" ), tr( qPrintable( criticalMessage ) ), QMessageBox::Ok );
}

void FADTTSWindow::DisplayIcon( QLabel *label , const QPixmap icon )
{
    label->setPixmap( icon.scaled( QSize( m_iconSize, m_iconSize ), Qt::IgnoreAspectRatio ) );
}


/***************************************************************/
/************************** Input tab **************************/
/***************************************************************/

/***********************  Private slots  ***********************/
void FADTTSWindow::AddMultipleInputFiles()
{
    QString dir = m_currentInputFileDir;
    QStringList fileList;
    fileList = QFileDialog::getOpenFileNames( this, tr( "Choose Input Files" ), dir, tr( ".csv( *.csv ) ;; .*( * )" ) );

    UpdateLineEditsAfterAddingMultipleFiles( fileList );
}

void FADTTSWindow::AddInputFile( const QString& prefID )
{
    QLineEdit *lineEdit = m_inputTabInputFileLineEditMap[ prefID ];
    QString filePath = lineEdit->text();
    QDir dir;
    SetDir( dir, filePath, m_currentInputFileDir );

    QString file = QFileDialog::getOpenFileName( this, tr( qPrintable( "Choose " + prefID.toUpper() + " File" ) ), dir.absolutePath(), tr( ".csv( *.csv ) ;; .*( * )" ) );
    if( !file.isEmpty() )
    {
        lineEdit->setText( file );
    }
}

void FADTTSWindow::UpdateInputLineEdit( const QString& prefID )
{
    QLineEdit *lineEdit = m_inputTabInputFileLineEditMap[ prefID ];
    QString filePath = lineEdit->text();
    QFile file( filePath );
    if( filePath.isEmpty() )
    {
        m_inputTabIconLabelMap[ prefID ]->clear();
        m_data.ClearFileInformation( prefID );
    }
    else
    {
        UpdateEditInputDialogCurrentDir( filePath );

        if( !file.open( QIODevice::ReadOnly ) )
        {
            DisplayInputLineEditIcon( prefID, m_koPixmap );
            m_data.ClearFileInformation( prefID );
        }
        else
        {
            file.close();
            QList<QStringList> fileData = m_processing.GetDataFromFile( filePath );
            if( m_processing.IsMatrixDimensionOK( fileData ) )
            {
                DisplayInputLineEditIcon( prefID, m_okPixmap );
                m_data.SetFilename( prefID ) = filePath;
                m_data.SetFileData( prefID ) = fileData;
            }
            else
            {
                DisplayInputLineEditIcon( prefID, m_koPixmap );

                QString criticalMessage = prefID.toUpper() + " data file corrupted:<br><i>" + m_inputTabInputFileLineEditMap[ prefID ]->text() + "</i><br>"
                        "For each row, the number of columns is not constant.<br>"
                        "We advise that you to check your data file.";
                CriticalPopUp( criticalMessage );

                m_data.ClearFileInformation( prefID );
            }
        }
    }

    UpdateInputFileInformation( prefID );

    if( prefID == m_data.GetCovariatePrefix() )
    {
        m_editInputDialog->ResetCovariateFileSubjectColumnID(); /** By default Subjects are on the 1st column. **/
        SetInfoCovariateFileSubjectColumnID();
    }

    SortAndDisplaySubjects();
}

void FADTTSWindow::EditInputFile( const QString& prefID )
{
    if( m_data.GetFilename( prefID ).isEmpty() )
    {
        QString warningMessage = "<center><b>File edition unable</b></center>";
        if( m_inputTabInputFileLineEditMap[ prefID ]->text().isEmpty() )
        {
            warningMessage.append( "No file specified" );
        }
        else
        {
            warningMessage.append( "Could not open the file:<br><i>" + m_inputTabInputFileLineEditMap[ prefID ]->text() + "</i>" );
        }
        WarningPopUp( warningMessage );
    }
    else
    {
        LaunchEditInputDialog( prefID );
    }
}


void FADTTSWindow::DisplayInputFileInformation()
{
    m_infoDialog->setModal( true );
    m_infoDialog->setWindowTitle( tr( "Files Information" ) );
    m_infoDialog->DisplayFileInformation();
    m_infoDialog->exec();
}


void FADTTSWindow::UpdateInputLineEditAfterFileEdition( const QString& newFilePathAfterFileEdition, const QString& prefID )
{
    m_inputTabInputFileLineEditMap[ prefID ]->setText( newFilePathAfterFileEdition );
}

void FADTTSWindow::UpdateCovariateMapAfterFileEdition( const QMap<int, QString>& newCovariateMapAfterFileEdition )
{
    m_data.ClearCovariates();
    m_data.SetCovariates() = newCovariateMapAfterFileEdition;
    DisplayCovariates( m_data.GetCovariates() );
}

void FADTTSWindow::UpdateCovariateFileSubjectColumnIDAfterFileEdition( const int&  newCovariateFileSubjectColumnIDAfterFileEdition )
{
    /** Subjects are not on the 1st column anymore. **/
    m_data.SetCovariateFileSubjectColumnID() = newCovariateFileSubjectColumnIDAfterFileEdition;
    UpdateInputFileInformation( m_data.GetCovariatePrefix() );
    SetInfoCovariateFileSubjectColumnID();

    SortAndDisplaySubjects();
}


/*********************** Private function ***********************/
void FADTTSWindow::UpdateLineEditsAfterAddingMultipleFiles( const QStringList fileList )
{
    /** This function only works with filename that start with ad_, rd_, md_, fa_ and COMP_.
     *  If a prefix is detected more than once, the reated files will be ignored. **/

    QMap<QString, QStringList > map;
    foreach( QString file, fileList )
    {
        QString p = QFileInfo( QFile ( file ) ).fileName().split( '_' ).first();
        if( m_data.GetPrefixList().contains( p ) )
        {
            ( map[ p ] ).append( file );
        }
    }

    QMap<QString, QStringList>::ConstIterator iter = map.begin();
    while( iter != map.constEnd() )
    {
        QString prefix = iter.key();
        QString file;
        if( iter.value().size() == 1 )
        {
            file = iter.value().first();
        }
        else
        {
            file.clear();
        }

        m_inputTabInputFileLineEditMap[ prefix ]->setText( file );
        ++iter;
    }
}


void  FADTTSWindow::LaunchEditInputDialog( QString prefID )
{
    m_editInputDialog->SetLineEditPrefix( prefID );
    m_editInputDialog->SetInputFile( m_inputTabInputFileLineEditMap[ prefID ]->text() );
    m_editInputDialog->setModal( true );
    m_editInputDialog->setWindowTitle( tr( qPrintable( "Edit " + prefID.toUpper() + " File" ) ) );
    m_editInputDialog->DisplayFileData();
    m_editInputDialog->exec();
}


void FADTTSWindow::UpdateInputFileInformation( const QString prefID )
{
    QString filePath = m_data.GetFilename( prefID );
    if( !filePath.isEmpty() )
    {
        QList<QStringList> fileData = m_data.GetFileData( prefID );
        int nbrRows = fileData.count();
        int nbrColumns = fileData.at( 0 ).count();

        m_data.ClearSubjects( prefID );
        QStringList subjects = m_processing.GetSubjectsFromInputFile( fileData, m_data.GetCovariateFileSubjectColumnID() );
        m_data.SetSubjects( prefID, subjects );

        if( prefID == m_data.GetCovariatePrefix() )
        {
            m_data.SetNbrRows( prefID ) = nbrRows-1;
            m_data.SetNbrColumns( prefID ) = nbrColumns-1;
            m_data.SetNbrSubjects( prefID ) = subjects.count();

            m_data.ClearCovariates();
            m_data.SetCovariates() = m_processing.GetCovariatesFromFileData( fileData, m_data.GetCovariateFileSubjectColumnID() );
            /** Intercept representes everything that has not been classified in one of the previous
             *  covariates. It is important to add it as 1st element of m_covariatesList **/
            m_data.AddInterceptToCovariates();
        }
        else
        {
            m_data.SetNbrRows( prefID ) = nbrRows-1;
            m_data.SetNbrColumns( prefID ) = nbrColumns;
            m_data.SetNbrSubjects( prefID ) = subjects.count();
        }
    }

    if( prefID == m_data.GetCovariatePrefix() )
    {
        DisplayCovariates( m_data.GetCovariates() );
    }

    UpdateAvailableFileParamTab();
}


void FADTTSWindow::DisplayInputLineEditIcon( const QString prefID, const QPixmap icon )
{
    DisplayIcon( m_inputTabIconLabelMap[ prefID ], icon );
}



void FADTTSWindow::SetInfoCovariateFileSubjectColumnID()
{
    if( !m_data.GetFileData( m_data.GetCovariatePrefix() ).isEmpty() )
    {
        this->inputTab_subjectColumnID_label->setText( tr( qPrintable( "<b><i><span style=""font-size:7pt;"">" +
                                                                       QString::number( m_data.GetCovariateFileSubjectColumnID() + 1 ) + "</i></b></span>") ) );
    }
    else
    {
        this->inputTab_subjectColumnID_label->clear();
    }
}


/****************************************************************/
/************************* Subjects tab *************************/
/****************************************************************/

/*********************** Public  function ***********************/


/***********************  Private slots  ************************/
void FADTTSWindow::LoadSubjectFile()
{
    QLineEdit *lineEdit = m_subjectFileLineEdit;
    QString filePath = lineEdit->text();
    QDir dir;
    SetDir( dir, filePath, m_currentSubjectFileDir );

    QString file = QFileDialog::getOpenFileName( this, tr( "Choose SubjectList File" ), dir.absolutePath(), tr( ".txt ( *.txt ) ;; .*( * )" ) );
    if( !file.isEmpty() )
    {
        lineEdit->setText( file );
    }
}

void FADTTSWindow::ResetSubjectFile()
{
    m_subjectFileLineEdit->clear();
}

void FADTTSWindow::UpdateSubjectFileLineEdit( const QString& filePath )
{
    QFile subjectFile( filePath );
    QLabel *label = this->subjectTab_iconLoadSubjectFile_label;
    if( filePath.isEmpty() )
    {
        label->clear();
    }
    else
    {
        if( !subjectFile.open( QIODevice::ReadOnly ) )
        {
            DisplayIcon( label, m_koPixmap );
        }
        else
        {
            subjectFile.close();
            DisplayIcon( label, m_okPixmap );
        }

        UpdateCurrentDir( filePath, m_currentSubjectFileDir );
    }

    SortAndDisplaySubjects();
}

void FADTTSWindow::SaveCheckedSubjects()
{
    QString filePath = QFileDialog::getSaveFileName( this, tr( "Save subject list as ..." ), m_currentSubjectFileDir, tr( ".txt ( *.txt ) ;; .*( * )" ) );
    QFile exportedTXT( filePath );
    if( exportedTXT.open( QIODevice::WriteOnly ) )
    {
        QTextStream ts( &exportedTXT );
        for( int i = 0; i < m_sortedSubjectListWidget->count(); i++ )
        {
            if( m_sortedSubjectListWidget->item( i )->checkState() == Qt::Checked )
            {
                ts << m_sortedSubjectListWidget->item( i )->text() <<  endl;
            }
        }
        exportedTXT.flush();
        exportedTXT.close();

        QString previousFilePath = m_subjectFileLineEdit->text();
        m_subjectFileLineEdit->setText( filePath );
        if( previousFilePath == filePath )
        {
            /** If filePath does not change, an update is needed to display the right subject list **/
            SortAndDisplaySubjects();
        }
    }
}


void FADTTSWindow::CheckAllSubjectsVisible()
{
    QListWidget *list = m_sortedSubjectListWidget;
    for( int i = 0; i < list->count(); i++ )
    {
        QListWidgetItem* current = list->item( i );
        if( !current->isHidden() && current->flags() == Qt::ItemIsEnabled )
        {
            current->setCheckState( Qt::Checked );
        }
    }
}

void FADTTSWindow::UnCheckAllSubjectsVisible()
{
    QListWidget *list = m_sortedSubjectListWidget;
    for( int i = 0; i < list->count(); i++ )
    {
        QListWidgetItem* current = list->item( i );
        if( !current->isHidden() && current->flags() == Qt::ItemIsEnabled )
        {
            current->setCheckState( Qt::Unchecked );
        }
    }
}

void FADTTSWindow::SelectSubject( QListWidgetItem *item )
{
    if( item->flags() == Qt::ItemIsEnabled )
    {
        if( item->checkState() == Qt::Unchecked )
        {
            item->setCheckState( Qt::Checked );
            item->setBackgroundColor( m_green );
        }
        else
        {
            if( item->checkState() == Qt::Checked )
            {
                item->setCheckState( Qt::Unchecked );
                item->setBackgroundColor( m_grey );
            }
        }
    }

    DisplayNbrSubjectSelected();
}


void FADTTSWindow::SortAndDisplaySubjects()
{
    QString subjectFile = m_subjectFileLineEdit->text();
    QStringList refSubjectList = m_processing.GetRefSubjects( subjectFile, GetFileDataOfSelectedFiles(), m_data.GetCovariateFileSubjectColumnID() );
    QMap<QString, QStringList> allSubjects = m_processing.GetAllSubjectsFromSelectedInputFiles( m_paramTabFileCheckBoxMap, m_data.GetSubjects() );

    QMap< QString, QMap<QString, bool> > sortedSubjects = m_processing.SortSubjects( refSubjectList, allSubjects );

    QStringList matchedSubjects;
    QMap<QString, QStringList > unMatchedSubjects;
    m_processing.AssignSortedSubject( sortedSubjects, matchedSubjects, unMatchedSubjects );

    DisplaySortedSubjects( matchedSubjects, unMatchedSubjects );

    SearchSubjects();
}

void FADTTSWindow::SearchSubjects()
{
    QListWidget *list = m_sortedSubjectListWidget;
    QLineEdit *lineEdit = this->subjectTab_search_lineEdit;
    QString search = lineEdit->text();
    QPalette palette;
    if( !search.isEmpty() )
    {
        if(  list->count() != 0  )
        {
            search.prepend( "*" );
            search.append( "*" );
            QRegExp regExp( search );
            regExp.setPatternSyntax( QRegExp::Wildcard );
            regExp.setCaseSensitivity( caseSensitivity );
            for( int i = 0; i < list->count(); i++ )
            {
                QListWidgetItem* current = list->item( i );
                if( regExp.exactMatch( current->text() ) )
                {
                    current->setHidden( false );
                }
                else
                {
                    current->setHidden( true );
                }
            }
        }
        palette.setColor( QPalette::Base, m_yellow );
        lineEdit->setPalette( palette );
    }
    else
    {
        if( list->count() != 0 )
        {
            for( int i = 0; i < list->count(); i++ )
            {
                list->item( i )->setHidden( false );
            }
        }
        palette.setColor( QPalette::Base, Qt::white );
        lineEdit->setPalette( palette );
    }
}

void FADTTSWindow::SetCaseSensitivity( bool checked )
{
    if( checked )
    {
        caseSensitivity = Qt::CaseSensitive;
    }
    else
    {
        caseSensitivity = Qt::CaseInsensitive;
    }

    SearchSubjects();
}


/*********************** Private function ***********************/
void FADTTSWindow::UpdateAvailableFileParamTab()
{
    QString text;
    labelMapType::ConstIterator iterLabel = m_paramTabFileDataSizeLabelMap.begin();
    checkBoxMapType::ConstIterator iterCheckBox = m_paramTabFileCheckBoxMap.begin();
    while( iterLabel != m_paramTabFileDataSizeLabelMap.constEnd() )
    {
        int nbrRows = m_data.GetNbrRows( iterCheckBox.key() );
        int nbrColumns = m_data.GetNbrColumns( iterCheckBox.key() );
        if( ( nbrRows == 0 ) || ( nbrColumns == 0 ) )
        {
            text = tr( "N/A" );
            iterLabel.value()->setEnabled( false );
            iterCheckBox.value()->setChecked( false );
            iterCheckBox.value()->setEnabled( false );
        }
        else
        {
            text = tr( qPrintable( QString::number( nbrRows ) + "x" + QString::number( nbrColumns ) ) );
            iterLabel.value()->setEnabled( true );
            iterCheckBox.value()->setChecked( true );
            iterCheckBox.value()->setEnabled( true );
        }
        iterLabel.value()->setText( text );
        ++iterLabel;
        ++iterCheckBox;
    }
}

void FADTTSWindow::DisplaySortedSubjects( const QStringList matchedSubjects, const QMap<QString, QStringList > unMatchedSubjectMap )
{
    m_sortedSubjectListWidget->clear();
    m_sortedSubjectListWidget->setUpdatesEnabled( false );
    m_sortedSubjectListWidget->setSelectionMode( QAbstractItemView::NoSelection );

    foreach( QString subject, matchedSubjects )
    {
        QListWidgetItem *item = new QListWidgetItem( subject, m_sortedSubjectListWidget );
        item->setCheckState( Qt::Checked );
        item->setBackgroundColor( m_green );
        item->setFlags( Qt::ItemIsEnabled );
        m_sortedSubjectListWidget->addItem( item );
    }

    QMap<QString, QStringList >::ConstIterator iterUnmatchedSubjects = unMatchedSubjectMap.begin();
    while( iterUnmatchedSubjects != unMatchedSubjectMap.constEnd() )
    {
        QStringList sortedText = iterUnmatchedSubjects.value();
        sortedText.sort();
        QString text = tr( qPrintable( iterUnmatchedSubjects.key() + " --> " + sortedText.join( ", " ) ) );
        QListWidgetItem *item = new QListWidgetItem( text, m_sortedSubjectListWidget );
        item->setBackgroundColor( m_red );
        item->setTextColor( m_lightBlack );
        m_sortedSubjectListWidget->addItem( item );
        ++iterUnmatchedSubjects;
    }
    m_sortedSubjectListWidget->setUpdatesEnabled( true );

    this->subjectTab_sortedSubjectsInformation_label->clear();
    QString text;
    int nbrSubjects = matchedSubjects.count() + unMatchedSubjectMap.size();
    if( ( unMatchedSubjectMap.isEmpty() ) && ( !matchedSubjects.isEmpty() ) )
    {
        text = tr( qPrintable( "All subjects matched ( " + QString::number( matchedSubjects.size() ) + " )" ) );
    }
    else if( ( !unMatchedSubjectMap.isEmpty() ) && ( matchedSubjects.isEmpty() ) )
    {
        text = tr( qPrintable( "Warning! No subject matched ( " + QString::number( unMatchedSubjectMap.size() ) + " )" ) );
    }
    else if( !unMatchedSubjectMap.isEmpty() && ( !matchedSubjects.isEmpty() ) )
    {
        text = tr( qPrintable( QString::number( matchedSubjects.size() ) + "/" + QString::number( nbrSubjects ) + " matched      " +
                               QString::number( unMatchedSubjectMap.size() ) + "/" + QString::number( nbrSubjects ) + " unmatched" ) );
    }
    this->subjectTab_sortedSubjectsInformation_label->setText( text );

    DisplayNbrSubjectSelected();
}

void FADTTSWindow::DisplayNbrSubjectSelected()
{
    this->subjectTab_checkedSubjectsInformation_label->clear();
    int nbrSubjectsSelected = 0;

    if( m_sortedSubjectListWidget->count() != 0 )
    {
        for( int i = 0; i < m_sortedSubjectListWidget->count(); i++ )
        {
            QListWidgetItem *item = m_sortedSubjectListWidget->item( i );

            if( item->checkState() == Qt::Checked )
            {
                nbrSubjectsSelected++;
            }
        }

        this->subjectTab_checkedSubjectsInformation_label->setText( tr( qPrintable( QString::number( nbrSubjectsSelected ) + " subjects selected") ) );
    }
}


/****************************************************************/
/************************ Parameters tab ************************/
/****************************************************************/

/***********************  Private  slots  ***********************/
void FADTTSWindow::SelectCovariate( QListWidgetItem *item )
{
    if( item->flags() == Qt::ItemIsEnabled )
    {
        if( item->checkState() == Qt::Unchecked )
        {
            item->setCheckState( Qt::Checked );
        }
        else if( !item->text().contains( "Intercept" ) )
        {
            item->setCheckState( Qt::Unchecked );
        }
        else
        {
            QString warningMessage = "You are about to uncheck the Intercept. This action is not recommended.<br>Are you sure you want to do it?";
            int ignoreWarning = QMessageBox::warning( this, tr( "Uncheck Intercept" ), tr( qPrintable( warningMessage ) ),
                                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::No );

            if( ignoreWarning == QMessageBox::Yes )
            {
                item->setCheckState( Qt::Unchecked );
            }
        }
    }
}

void FADTTSWindow::CheckAllCovariates()
{
    QListWidget *covariateListWidget = m_covariateListWidget;
    for( int i = 0; i < covariateListWidget->count(); i++ )
    {
        covariateListWidget->item( i )->setCheckState( Qt::Checked );
    }
}

void FADTTSWindow::UnCheckAllCovariates()
{
    QListWidget *covariateListWidget = m_covariateListWidget;
    for( int i = 0; i < covariateListWidget->count(); i++ )
    {
        QListWidgetItem *currentItem = covariateListWidget->item( i );
        if( !currentItem->text().contains( "Intercept" ) )
        {
            currentItem->setCheckState( Qt::Unchecked );
        }
    }
}


/*********************** Private function ***********************/
void FADTTSWindow::DisplayCovariates( QMap<int, QString> covariateMap )
{
    m_covariateListWidget->clear();

    if( !( covariateMap.isEmpty() ) )
    {
        QMap<int, QString>::ConstIterator iterCovariate = covariateMap.begin();
        while( iterCovariate != covariateMap.end() )
        {
            QListWidgetItem *covariateItem = new QListWidgetItem( iterCovariate.value(), m_covariateListWidget );
            covariateItem->setCheckState( Qt::Checked );
            covariateItem->setFlags( Qt::ItemIsEnabled );
            m_covariateListWidget->addItem( covariateItem );
            ++iterCovariate;
        }
    }
}


/****************************************************************/
/*************************** Run  tab ***************************/
/****************************************************************/

/***********************  Private  slots  ***********************/
void FADTTSWindow::SetOutputDir()
{
    QLineEdit *lineEdit = this->para_runTab_outputDir_lineEdit;
    QString filePath = lineEdit->text();
    QDir dir = filePath;

    QString dirPath;
    if( dir.exists() && !filePath.isEmpty() )
    {
        dirPath = QFileDialog::getExistingDirectory( this, tr( "Choose Output Directory" ), dir.absolutePath(), QFileDialog::ShowDirsOnly );
    }
    else
    {
        dirPath = QFileDialog::getExistingDirectory( this, tr( "Choose Output Directory" ), m_data.GetOutputDir(), QFileDialog::ShowDirsOnly );
    }
    if( !dirPath.isEmpty() )
    {
        m_data.SetOutputDir() =  dirPath;
        lineEdit->setText( dirPath );
    }
}

void FADTTSWindow::UpdateOutputDirLineEdit( const QString&  path )
{
    QLabel *label = this->runTab_iconOutputDir_label;
    if( !path.isEmpty() )
    {
        if( QDir( path ).exists() )
        {
            DisplayIcon( label, m_okPixmap );
            m_data.SetOutputDir() = path;
        }
        else
        {
            DisplayIcon( label, m_koPixmap );
        }
    }
    else
    {
        label->clear();
    }
}

void FADTTSWindow::SetMatlabExe()
{
    /******************************************/
    /****** NOT SATISFIED WITH THIS CODE ******/
    /******************************************/
    QLineEdit *lineEdit = this->para_runTab_matlab_lineEdit;
    QString filePath = lineEdit->text();
    QString file;
    QDir dir;
    if( !filePath.isEmpty() )
    {
        dir = QFileInfo( QFile( filePath ) ).absolutePath();
        if( !dir.exists() )
        {
            dir = m_currentMatlabExeDir;
        }
    }
    else
    {
        dir = m_currentMatlabExeDir;
    }

    file = QFileDialog::getOpenFileName( this, tr( "Choose Matlab Executable" ), dir.absolutePath() );
    if( !file.isEmpty() )
    {
        lineEdit->setText( file );
    }
    /******************************************/
    /****** NOT SATISFIED WITH THIS CODE ******/
    /******************************************/
}

void FADTTSWindow::UpdateMatlabExeLineEdit( const QString& executable )
{
    QFile matlabExe( executable );
    QLabel *label = this->runTab_iconMatlab_label;
    if( executable.isEmpty() )
    {
        label->clear();
    }
    else
    {
        if( !matlabExe.open( QIODevice::ReadOnly ) )
        {
            DisplayIcon( label, m_koPixmap );
        }
        else
        {
            matlabExe.close();
            DisplayIcon( label, m_okPixmap );
        }

        UpdateCurrentDir( executable, m_currentMatlabExeDir );
    }
}

void FADTTSWindow::RunFADTTS()
{
    SyncUiToModelStructure();

    QMap<int, QString> selectedCovariates = GetSelectedCovariates();

    bool fiberNameProvided = !this->para_inputTab_fiberName_lineEdit->text().isEmpty();
    bool atLeastOneDataFileSelected = ( this->para_subjectTab_adFile_checkBox->isChecked() | this->para_subjectTab_rdFile_checkBox->isChecked() |
                                        this->para_subjectTab_mdFile_checkBox->isChecked() | this->para_subjectTab_faFile_checkBox->isChecked() );
    bool covariateFileSelected = this->para_subjectTab_covariateFile_checkBox->isChecked();
    bool atLeastOneCovariateSelected = selectedCovariates.count() != 0;
    if( !fiberNameProvided || !atLeastOneDataFileSelected || !covariateFileSelected || !atLeastOneCovariateSelected  )
    {
        QString warningText = "<b>FADTTS will not be executed for the following reason(s):</b><br>";
        if( !fiberNameProvided )
        {
            warningText.append( "- No fiber name provided<br><i>Inputs Tab</i><br>" );
        }
        if( !atLeastOneDataFileSelected )
        {
            warningText.append( "- Provide and select at least 1 data file<br><i>Inputs Tab / Parameters Tab</i><br>" );
        }
        if( !covariateFileSelected )
        {
            warningText.append( "- No correct input covariate file provided and/or selected<br><i>Inputs Tab / Parameters Tab</i><br>" );
        }
        if( !atLeastOneCovariateSelected )
        {
            warningText.append( "- Select at least 1 covariate<br><i>Parameters Tab</i><br>" );
        }
        WarningPopUp( warningText );
    }
    else
    {
        QStringList selectedPrefixes = GetSelectedPrefixes();

        QMap< QPair< int, QString >, bool> selectedInputFiles = GetSelectedInputFiles();

        QString selectedSubjectListFilePath = GenerateSelectedSubjectFile();

        QMap< QPair< int, QString >, bool> matlabInputFiles =
                m_processing.GenerateMatlabInputFiles( selectedInputFiles, selectedSubjectListFilePath,
                                                       m_data.GetCovariateFileSubjectColumnID(), selectedCovariates, m_data.GetOutputDir(),
                                                       this->para_inputTab_fiberName_lineEdit->text() );

        m_matlabScript.InitMatlabScript();
        m_matlabScript.SetHeader();
        m_matlabScript.SetFiberName( this->para_inputTab_fiberName_lineEdit->text() );
        m_matlabScript.SetDiffusionProperties( selectedPrefixes );
        m_matlabScript.SetNbrPermutation( this->para_parameterTab_nbrPermutations_spinBox->value() );
        m_matlabScript.SetCovariates( selectedCovariates );
        m_matlabScript.SetInputFiles( matlabInputFiles );
        m_matlabScript.SetOmnibus( this->para_parameterTab_omnibus_checkBox->isChecked() );
        m_matlabScript.SetPostHoc( this->para_parameterTab_postHoc_checkBox->isChecked() );


        QString matlabScript = m_matlabScript.GenerateMatlabFiles( m_data.GetOutputDir(), this->para_inputTab_fiberName_lineEdit->text(), this->para_parameterTab_nbrPermutations_spinBox->value() );

//        QString program = "/opt/matlab/bin/matlab";
//        m_processing.RunScript( program,  matlabScript );
    }
}


/*********************** Private function ***********************/
QStringList FADTTSWindow::GetSelectedPrefixes()
{
    QStringList selectedPrefixes;
    foreach ( QString prefID, m_data.GetPrefixList() )
    {
        if( m_paramTabFileCheckBoxMap[ prefID ]->isChecked() &&  prefID != m_data.GetCovariatePrefix() )
        {
            selectedPrefixes.append( prefID );
        }
    }
    return selectedPrefixes;
}

QMap< QPair< int, QString >, bool > FADTTSWindow::GetSelectedInputFiles()
{
    QMap< QPair< int, QString >, bool > selectedInputFiles;
    int i = 0;
    foreach ( QString prefID, m_data.GetPrefixList() )
    {
        QPair< int, QString > currentPair;
        if( m_paramTabFileCheckBoxMap[ prefID ]->isChecked() && !m_data.GetFilename( prefID ).isEmpty() )
        {
            currentPair.first = i;
            currentPair.second = m_data.GetFilename( prefID );
            selectedInputFiles.insert( currentPair, prefID != m_data.GetCovariatePrefix() ? false : true );
        }
        i++;
    }

    return selectedInputFiles;
}

QMap< QPair< int, QString >, QList<QStringList> > FADTTSWindow::GetFileDataOfSelectedFiles()
{
    QMap< QPair< int, QString >, QList<QStringList> > dataInSelectedFiles;
    int i = 0;
    foreach ( QString prefID, m_data.GetPrefixList() )
    {
        QPair< int, QString > currentPair;
        currentPair.first = i;
        currentPair.second = m_data.GetFilename( prefID );
        if( m_paramTabFileCheckBoxMap[ prefID ]->isChecked() && !m_data.GetFilename( prefID ).isEmpty() )
        {
            dataInSelectedFiles.insert( currentPair, m_data.GetFileData( prefID ) );
        }
        i++;
    }

    return dataInSelectedFiles;
}

QMap<int, QString> FADTTSWindow::GetSelectedCovariates()
{
    QMap<int, QString> covariateMap = m_data.GetCovariates();
    QMap<int, QString> selectedCovariates;

    QListWidget *list = m_covariateListWidget;
    for( int i = 0; i < list->count(); i++ )
    {
        QListWidgetItem *currentItem = list->item( i );
        if( currentItem->checkState() == Qt::Checked )
        {
            if( i == 0 )
            {
                selectedCovariates.insert( -1, "Intercept" );
            }
            else
            {
                QMap<int, QString>::ConstIterator iterCovar = covariateMap.begin();
                while( iterCovar != covariateMap.end() )
                {
                    if( currentItem->text() == iterCovar.value() )
                    {
                        selectedCovariates.insert( iterCovar.key(), iterCovar.value() );
                    }
                    ++iterCovar;
                }
            }
        }
    }

    return selectedCovariates;
}

QString FADTTSWindow::GenerateSelectedSubjectFile()
{
    QFile selectedSubjects( m_data.GetOutputDir() + "/" + this->para_inputTab_fiberName_lineEdit->text() + "_subjectList.txt" );
    if( selectedSubjects.open( QIODevice::WriteOnly ) )
    {
        QTextStream tsSelectedSubjects( &selectedSubjects );
        for( int i = 0; i < m_sortedSubjectListWidget->count(); i++ )
        {
            if( m_sortedSubjectListWidget->item( i )->checkState() == Qt::Checked )
            {
                tsSelectedSubjects << m_sortedSubjectListWidget->item( i )->text() << endl;
            }
        }
        selectedSubjects.close();
    }

    return selectedSubjects.fileName();
}
