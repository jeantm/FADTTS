#include "FADTTSWindow.h"

/****************************************************************/
/******************** Configuration && Events ********************/
/****************************************************************/
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
}


/*********************** Private slots ***********************/
void FADTTSWindow::SaveParaSettings()
{
    SyncUiToModelStructure();
    QString dir;
    QString filename = QFileDialog::getSaveFileName( this , tr( "Save Param Settings" ) ,  dir , tr( ".xml( *.xml ) ;; .*( * )" ) );
    if( !filename.isEmpty() )
    {
        QFileInfo fi( filename ) ;
        dir = fi.dir().absolutePath() ;
        Save_Parameter_Configuration( filename.toStdString() );
    }
}

void FADTTSWindow::LoadParaSettings()
{
    SyncUiToModelStructure();
    QString dir;
    QString filename = QFileDialog::getOpenFileName( this , tr( "Load Param Settings" ) , dir , tr( ".xml( *.xml ) ;; .*( * )" ) );
    if( !filename.isEmpty() )
    {
        QFileInfo fi( filename ) ;
        dir = fi.dir().absolutePath() ;
        Load_Parameter_Configuration( filename.toStdString() );
    }
}

void FADTTSWindow::SaveSoftSettings()
{
    SyncUiToModelStructure();
    QString dir;
    QString filename = QFileDialog::getSaveFileName( this , tr( "Save Soft Settings" ) ,  dir , tr( ".xml( *.xml ) ;; .*( * )" ) );
    if( !filename.isEmpty() )
    {
        QFileInfo fi( filename ) ;
        dir = fi.dir().absolutePath() ;
        Save_Software_Configuration( filename.toStdString() );
    }
}

void FADTTSWindow::LoadSoftSettings()
{
    SyncUiToModelStructure();
    QString dir;
    QString filename = QFileDialog::getOpenFileName( this , tr( "Load Soft Settings" ) , dir , tr( ".xml( *.xml ) ;; .*( * )" ) );
    if( !filename.isEmpty() )
    {
        QFileInfo fi( filename ) ;
        dir = fi.dir().absolutePath() ;
        Load_Software_Configuration( filename.toStdString() );
    }
}

void FADTTSWindow::DisplayAbout()
{
    QString messageBoxTitle = "About " + QString( FADTTS_TITLE );
    QString aboutFADTTS;
    aboutFADTTS = "<b>Version:</b> " + QString( FADTTS_VERSION ) + "<br>"
            "<b>Description:</b> " + QString( FADTTS_DESCRIPTION ) + "<br>"
            "<b>Contributors:</b> " + QString( FADTTS_CONTRIBUTORS );
    QMessageBox::information( this, tr( qPrintable( messageBoxTitle ) ), tr( qPrintable( aboutFADTTS ) ), QMessageBox::Ok );
}


void FADTTSWindow::closeEvent(QCloseEvent *event)
{
    if( m_matlabThread->isRunning() )
    {
        QMessageBox::StandardButton closeBox =
                QMessageBox::question( this, tr( "FADTTSter" ), tr( "Data are still being processed.<br>Are you sure you want to exit FADTTSter?" ),
                                       QMessageBox::No | QMessageBox::Yes, QMessageBox::No );
        switch( closeBox )
        {
        case QMessageBox::No:
            event->ignore();
            break;
        case QMessageBox::Yes:
        {
            m_matlabThread->terminate();
            event->accept();
            break;
        }
        default:
            event->accept();
            break;
        }
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
    InitSubjectCovariateTab();
    InitExecutionTab();
    InitPlottingTab();

    DisplayCovariates();

    UpdateAvailableFileParamTab();
}

void FADTTSWindow::InitMenuBar()
{
    connect( this->actionLoad_Param_Settings, SIGNAL( triggered() ), SLOT( LoadParaSettings() ) );
    connect( this->actionSave_Param_Settings, SIGNAL( triggered() ), SLOT( SaveParaSettings() ) );
    connect( this->actionLoad_Soft_Settings, SIGNAL( triggered() ), SLOT( LoadSoftSettings() ) );
    connect( this->actionSave_Soft_Settings, SIGNAL( triggered() ), SLOT( SaveSoftSettings() ) );
    connect( this->actionAbout, SIGNAL( triggered() ), SLOT( DisplayAbout() ) );
}

void FADTTSWindow::InitInputTab()
{
    m_editInputDialog = QSharedPointer<EditInputDialog>( new EditInputDialog( this ) );
    m_editInputDialog->SetData( &m_data );

    m_infoDialog.SetData( &m_data );


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

    /** Map of Labels to edit the file information and
     *  SignalMapper to link them to the slot EditFile() **/
    m_inputFileInformationLabelMap.insert( m_data.GetAxialDiffusivityPrefix(), this->inputTab_adFileInfo_label );
    m_inputFileInformationLabelMap.insert( m_data.GetRadialDiffusivityPrefix(), this->inputTab_rdFileInfo_label );
    m_inputFileInformationLabelMap.insert( m_data.GetMeanDiffusivityPrefix(), this->inputTab_mdFileInfo_label );
    m_inputFileInformationLabelMap.insert( m_data.GetFractionalAnisotropyPrefix(), this->inputTab_faFileInfo_label );
    m_inputFileInformationLabelMap.insert( m_data.GetCovariatePrefix(), this->inputTab_covariateFileInfo_label );
    m_infoDialog.SetInformationLabelMap( m_inputFileInformationLabelMap );


    /** Signal/Slot connection to receive updates from m_editInputDialog **/
    connect( m_editInputDialog.data(), SIGNAL( FilePathChanged( const QString&, const QString& ) ), this, SLOT( UpdateInputLineEditAfterFileEdition( const QString& , const QString& ) ) );
    connect( m_editInputDialog.data(), SIGNAL( CovariateMapChanged( const QMap<int, QString>& ) ), this, SLOT( UpdateCovariateMapAfterFileEdition( const QMap<int, QString>& ) ) );
    connect( m_editInputDialog.data(), SIGNAL( CovariateFileSubjectColumnIDChanged( const int& ) ), this, SLOT( UpdateCovariateFileSubjectColumnIDAfterFileEdition( int ) ) );
}

void FADTTSWindow::InitSubjectCovariateTab()
{
    caseSensitivity = Qt::CaseInsensitive;

    m_matchedSubjectListWidget = new QListWidget();
    m_matchedSubjectListWidget = this->subjectCovariateTab_matchedSubjectsInformation_listWidget;
    m_unmatchedSubjectListWidget = new QListWidget();
    m_unmatchedSubjectListWidget =this->subjectCovariateTab_unmatchedSubjectsInformation_listWidget;

    m_subjectFileLineEdit = new QLineEdit();
    m_subjectFileLineEdit = this->para_subjectCovariateTab_subjectFile_lineEdit;

    m_covariateListWidget = new QListWidget();
    m_covariateListWidget = this->para_subjectCovariateTab_covariates_listWidget;

    /** Map of CheckBoxes to select the files we want to work on and
     *  SignalMapper to link them to the slot SortSubjects() **/
    m_paramTabFileCheckBoxMap.insert( m_data.GetAxialDiffusivityPrefix(), this->para_subjectCovariateTab_adFile_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetRadialDiffusivityPrefix(), this->para_subjectCovariateTab_rdFile_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetMeanDiffusivityPrefix(), this->para_subjectCovariateTab_mdFile_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetFractionalAnisotropyPrefix(), this->para_subjectCovariateTab_faFile_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetCovariatePrefix(), this->para_subjectCovariateTab_covariateFile_checkBox );
    QSignalMapper *signalMapperSelectFile = new QSignalMapper( this );
    connect( signalMapperSelectFile, SIGNAL( mapped( const QString& ) ), this, SLOT( SortAndDisplaySubjects() ) );
    for ( int i = 0; i < m_data.GetPrefixList().size(); ++i )
    {
        connect( m_paramTabFileCheckBoxMap[ m_data.GetPrefixList().at( i ) ], SIGNAL( toggled( const bool& ) ), signalMapperSelectFile,SLOT( map() ) );
        signalMapperSelectFile->setMapping( m_paramTabFileCheckBoxMap[ m_data.GetPrefixList().at( i ) ], m_data.GetPrefixList().at( i ) );
    }
    connect( this->para_subjectCovariateTab_covariateFile_checkBox, SIGNAL( toggled( const bool& ) ), this, SLOT( DisplayCovariates() ) );

    /** Map of Labels displaying the matrix data size of the files that have been chosen **/
    m_paramTabFileDataSizeLabelMap.insert( m_data.GetAxialDiffusivityPrefix(), this->subjectCovariateTab_adFileInfo_label );
    m_paramTabFileDataSizeLabelMap.insert( m_data.GetRadialDiffusivityPrefix(),this->subjectCovariateTab_rdFileInfo_label );
    m_paramTabFileDataSizeLabelMap.insert( m_data.GetMeanDiffusivityPrefix(), this->subjectCovariateTab_mdFileInfo_label );
    m_paramTabFileDataSizeLabelMap.insert( m_data.GetFractionalAnisotropyPrefix(), this->subjectCovariateTab_faFileInfo_label );
    m_paramTabFileDataSizeLabelMap.insert( m_data.GetCovariatePrefix(), this->subjectCovariateTab_covariateFileInfo_label );


    connect( this->subjectCovariateTab_loadSubjectFile_PushButton, SIGNAL( clicked() ), this, SLOT( LoadSubjectFile() ) );
    connect( this->subjectCovariateTab_resetSubjectFile_pushButton, SIGNAL( clicked() ), this, SLOT( ResetSubjectFile() ) );
    connect( m_subjectFileLineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( UpdateSubjectFileLineEdit( const QString&  ) ) );
    connect( this->subjectCovariateTab_saveCheckedSubjects_pushButton, SIGNAL( clicked() ), this, SLOT( SaveCheckedSubjects() ) );

    connect( this->subjectCovariateTab_checkAllVisible_pushButton, SIGNAL( clicked() ), this, SLOT( CheckAllSubjectsVisible() ) );
    connect( this->subjectCovariateTab_unCheckAllVisible_pushButton, SIGNAL( clicked() ), this, SLOT( UnCheckAllSubjectsVisible() ) );
    connect( this->subjectCovariateTab_matchedSubjectsInformation_listWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( SelectSubject( QListWidgetItem * ) ) );
    connect( this->subjectCovariateTab_search_lineEdit, SIGNAL( textEdited( const QString& ) ), this, SLOT( SearchAllSubjects() ) );
    connect( this->subjectCovariateTab_caseSensitive_checkBox, SIGNAL( toggled( const bool& ) ), this, SLOT( SetCaseSensitivity( const bool& ) ) );

    connect( this->para_subjectCovariateTab_covariates_listWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( SelectCovariate( QListWidgetItem * ) ) );
    connect( this->subjectCovariateTab_covariatesCheckAll_pushButton, SIGNAL( clicked() ), this, SLOT( CheckAllCovariates() ) );
    connect( this->subjectCovariateTab_covariatesUncheckAll_pushButton, SIGNAL( clicked() ), this, SLOT( UnCheckAllCovariates() ) );
}

void FADTTSWindow::InitExecutionTab()
{
    m_matlabThread = new MatlabThread();
    m_matlabThread->SetMatlabScript( &m_matlabScript );
    m_matlabThread->SetProcessing( &m_processing );

    m_logWindow = new QPlainTextEdit();
    m_logWindow = this->executionTab_log_plainTextEdit;
    m_logWindow->setReadOnly( true );

    m_progressBar = new QProgressBar();
    m_progressBar = this->executionTab_progressBar;
    m_progressBar->setMinimum( 0 );
    m_progressBar->setMaximum( 0 );
    m_progressBar->hide();

    connect( this->para_executionTab_fiberName_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( SetCurrentFiberNameInfo() ) );

    connect( this->executionTab_outputDir_pushButton, SIGNAL( clicked() ), this, SLOT( SetOutputDir() ) );
    connect( this->para_executionTab_outputDir_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( UpdateOutputDir( const QString& ) ) );

    connect( this->executionTab_matlab_pushButton, SIGNAL( clicked() ), this, SLOT( SetMatlabExe() ) );
    connect( this->soft_executionTab_matlab_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( UpdateMatlabExe( const QString& ) ) );

    connect( this->executionTab_mvcm_pushButton, SIGNAL( clicked() ), this, SLOT( SetMVCMPath() ) );
    connect( this->soft_executionTab_mvcm_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( UpdateMVCMPath( const QString& ) ) );

    connect( this->executionTab_run_pushButton, SIGNAL( clicked() ), this, SLOT( RunFADTTS() ) );
    connect( this->executionTab_stop_pushButton, SIGNAL( clicked() ), this, SLOT( StopFADTTS() ) );
    connect( m_matlabThread, SIGNAL( finished() ), this, SLOT( OnThreadFinished() ) );
    connect( this->para_executionTab_matlab_checkBox, SIGNAL( toggled( const bool& ) ), this, SLOT( RunningMatlabAfterFileGeneration( const bool& ) ) );
    connect( this->executionTab_clearLog_pushButton, SIGNAL( clicked() ), this, SLOT( ClearLog() ) );

    this->para_executionTab_nbrPermutations_spinBox->setMaximum( 2000 );

    this->executionTab_run_pushButton->setEnabled( true );
    this->executionTab_stop_pushButton->setEnabled( false );

    RunningMatlabAfterFileGeneration( false );
}

void FADTTSWindow::InitPlottingTab()
{
    m_qvtkWidget = new QVTKWidget();
    m_qvtkWidget = this->plottingTab_plot_qvtkWidget;

    m_plot = new Plot();
    m_plot->SetQVTKWidget( m_qvtkWidget );

    m_outcomeComboBox = new QComboBox();
    m_outcomeComboBox = this->plottingTab_loadSetDataTab_outcomeSelection_comboBox;

    m_covariateComboBox = new QComboBox();
    m_covariateComboBox = this->plottingTab_loadSetDataTab_covariateSelection_comboBox;

    connect( m_plot, SIGNAL( OutcomeUsed( const QStringList& ) ), this, SLOT( UpdateOutcomeGiven( const QStringList& ) ) );
    connect( m_plot, SIGNAL( CovariateUsed( const QMap< int, QString >& ) ), this, SLOT( UpdateCovariateGiven( const QMap< int, QString >& ) ) );

    connect( this->plottingTab_loadSetDataTab_plotSelection_comboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( SelectPlot( const QString& ) ) );
    connect( m_outcomeComboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( SelectOutcome( const QString& ) ) );
    connect( m_covariateComboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( SelectCovariate( const QString& ) ) );

    connect( this->plottingTab_loadSetDataTab_displayPlot_pushButton, SIGNAL( clicked() ), this, SLOT( DisplayPlot() ) );
    connect( this->plottingTab_loadSetDataTab_resetPlot_pushButton, SIGNAL( clicked() ), this, SLOT( ResetPlot() ) );
    connect( this->plottingTab_loadSetDataTab_savePlot_pushButton, SIGNAL( clicked() ), m_plot, SLOT( SavePlot() ) );
    connect( this->plottingTab_loadSetDataTab_yMin_checkBox, SIGNAL( toggled( const bool& ) ), this, SLOT( OnYMinToggled( const bool& ) ) );
    connect( this->plottingTab_loadSetDataTab_yMax_checkBox, SIGNAL( toggled( const bool& ) ), this, SLOT( OnYMaxToggled( const bool& ) ) );
    this->plottingTab_loadSetDataTab_yMin_checkBox->setChecked( false );
    this->plottingTab_loadSetDataTab_yMax_checkBox->setChecked( false );

    HideShowPlotTab();
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
    if( !fileList.isEmpty() )
    {
        UpdateLineEditsAfterAddingMultipleFiles( fileList );
    }
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


void FADTTSWindow::UpdateInputLineEditAfterFileEdition( const QString& newFilePathAfterFileEdition, const QString& prefID )
{
    m_inputTabInputFileLineEditMap[ prefID ]->setText( newFilePathAfterFileEdition );
}

void FADTTSWindow::UpdateCovariateMapAfterFileEdition( const QMap<int, QString>& newCovariateMapAfterFileEdition )
{
    m_data.ClearCovariates();
    m_data.SetCovariates() = newCovariateMapAfterFileEdition;
    DisplayCovariates();
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

    foreach ( QString prefID, m_data.GetPrefixList() )
    {
        m_inputTabInputFileLineEditMap[ prefID ]->clear();
    }

    QMap<QString, QStringList>::ConstIterator iter = map.begin();
    while( iter != map.constEnd() )
    {
        QString prefID = iter.key();
        QString file;
        if( iter.value().size() == 1 )
        {
            file = iter.value().first();
        }
        else
        {
            file.clear();
        }

        m_inputTabInputFileLineEditMap[ prefID ]->setText( file );
        ++iter;
    }
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
        DisplayCovariates();
    }

    UpdateAvailableFileParamTab();
    m_infoDialog.DisplayFileInformation();
}

void FADTTSWindow::DisplayInputLineEditIcon( const QString prefID, const QPixmap icon )
{
    DisplayIcon( m_inputTabIconLabelMap[ prefID ], icon );
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
/******************** Subjects/Covariates tab *******************/
/****************************************************************/

/***********************  Private slots  ************************/
void FADTTSWindow::DisplayCovariates()
{
    QMap<int, QString> covariateMap = m_data.GetCovariates();
    m_covariateListWidget->clear();

    if( !( covariateMap.isEmpty() ) && this->para_subjectCovariateTab_covariateFile_checkBox->isChecked() )
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
        this->subjectCovariateTab_covariatesInformation_label->show();
    }
    else
    {
        this->subjectCovariateTab_covariatesInformation_label->hide();
    }
}

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
    QLabel *label = this->subjectCovariateTab_iconLoadSubjectFile_label;
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
    if( !filePath.isEmpty() )
    {
        QFile exportedTXT( filePath );
        exportedTXT.open( QIODevice::WriteOnly );
        QTextStream ts( &exportedTXT );
        QListWidget *list = m_matchedSubjectListWidget;
        for( int i = 0; i < list->count(); i++ )
        {
            if( list->item( i )->checkState() == Qt::Checked )
            {
                ts << list->item( i )->text() <<  endl;
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
    QListWidget *list = m_matchedSubjectListWidget;
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
    QListWidget *list = m_matchedSubjectListWidget;
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

    SearchAllSubjects();
}

void FADTTSWindow::SearchAllSubjects()
{
    int nbrSubjectFind = SearchSubjects( m_matchedSubjectListWidget ) + SearchSubjects( m_unmatchedSubjectListWidget );
    if( this->subjectCovariateTab_search_lineEdit->text().isEmpty() )
    {
        this->subjectCovariateTab_nbrFound_label->clear();
    }
    else
    {
        this->subjectCovariateTab_nbrFound_label->setText( "found " + QString::number( nbrSubjectFind ) );
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

    SearchAllSubjects();
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
        bool isDefine = !( ( nbrRows == 0 ) | ( nbrColumns == 0 ) );
        if( isDefine )
        {
            text = tr( qPrintable( QString::number( nbrRows ) + "x" + QString::number( nbrColumns ) ) );
        }
        else
        {
            text = tr( "N/A" );
        }
        iterLabel.value()->setEnabled( isDefine );
        iterLabel.value()->setText( text );
        iterCheckBox.value()->setEnabled( isDefine );
        iterCheckBox.value()->setChecked( isDefine );
        ++iterLabel;
        ++iterCheckBox;
    }
}

void FADTTSWindow::DisplaySortedSubjects( const QStringList matchedSubjects, const QMap<QString, QStringList > unMatchedSubjectMap )
{
    m_matchedSubjectListWidget->clear();
    m_matchedSubjectListWidget->setUpdatesEnabled( false );
    m_matchedSubjectListWidget->setSelectionMode( QAbstractItemView::NoSelection );

    foreach( QString subject, matchedSubjects )
    {
        QListWidgetItem *item = new QListWidgetItem( subject, m_matchedSubjectListWidget );
        item->setCheckState( Qt::Checked );
        item->setBackgroundColor( m_green );
        item->setFlags( Qt::ItemIsEnabled );
        m_matchedSubjectListWidget->addItem( item );
    }
    m_matchedSubjectListWidget->setUpdatesEnabled( true );


    m_unmatchedSubjectListWidget->clear();
    m_unmatchedSubjectListWidget->setUpdatesEnabled( false );
    m_unmatchedSubjectListWidget->setSelectionMode( QAbstractItemView::NoSelection );
    QMap<QString, QStringList >::ConstIterator iterUnmatchedSubjects = unMatchedSubjectMap.begin();
    while( iterUnmatchedSubjects != unMatchedSubjectMap.constEnd() )
    {
        QStringList sortedText = iterUnmatchedSubjects.value();
        sortedText.sort();
        QString text = tr( qPrintable( iterUnmatchedSubjects.key() + " --> " + sortedText.join( ", " ) ) );
        QListWidgetItem *item = new QListWidgetItem( text, m_unmatchedSubjectListWidget );
        item->setBackgroundColor( m_red );
        item->setTextColor( m_lightBlack );
        m_unmatchedSubjectListWidget->addItem( item );
        ++iterUnmatchedSubjects;
    }
    m_unmatchedSubjectListWidget->setUpdatesEnabled( true );

    DisplaySubjectInformation();
    DisplayNbrSubjectSelected();
}

void FADTTSWindow::DisplaySubjectInformation()
{
    this->subjectCovariateTab_matchedSubjectsInformation_label->clear();
    this->subjectCovariateTab_unmatchedSubjectsInformation_label->clear();
    QString textMatchedSubjectsInformation;
    QString textUnmatchedSubjectsInformation;
    int nbrMatchedSubjects = m_matchedSubjectListWidget->count();
    int nbrUnmatchedSubjects = m_unmatchedSubjectListWidget->count();
    int nbrTotalSubjects = nbrMatchedSubjects + nbrUnmatchedSubjects;
    if( ( nbrMatchedSubjects != 0 ) || ( nbrUnmatchedSubjects != 0 ) )
    {
        if( ( nbrMatchedSubjects == 0 ) && ( nbrUnmatchedSubjects != 0 ) )
        {
            textMatchedSubjectsInformation = tr( qPrintable( "WARNING No matched! " + QString::number( nbrMatchedSubjects ) + "/" + QString::number( nbrTotalSubjects ) ) );
            textUnmatchedSubjectsInformation = tr( qPrintable( QString::number( nbrUnmatchedSubjects ) + "/" + QString::number( nbrTotalSubjects ) + " unmatched" ) );
        }
        else
        {
            if( ( nbrMatchedSubjects != 0 ) && ( nbrUnmatchedSubjects == 0 ) )
            {
                textMatchedSubjectsInformation = tr( qPrintable( "All subjects matched " + QString::number( nbrMatchedSubjects ) + "/" + QString::number( nbrTotalSubjects ) ) );
                textUnmatchedSubjectsInformation = tr( qPrintable( QString::number( nbrUnmatchedSubjects ) + "/" + QString::number( nbrTotalSubjects ) + " unmatched" ) );
            }
            else
            {
                textMatchedSubjectsInformation = tr( qPrintable( QString::number( nbrMatchedSubjects ) + "/" + QString::number( nbrTotalSubjects ) + " matched" ) );
                textUnmatchedSubjectsInformation = tr( qPrintable( QString::number( nbrUnmatchedSubjects ) + "/" + QString::number( nbrTotalSubjects ) + " unmatched" ) );
            }
        }
        this->subjectCovariateTab_matchedSubjectsInformation_label->setText( textMatchedSubjectsInformation );
        this->subjectCovariateTab_unmatchedSubjectsInformation_label->setText( textUnmatchedSubjectsInformation );
    }
    else
    {
        this->subjectCovariateTab_matchedSubjectsInformation_label->clear();
        this->subjectCovariateTab_unmatchedSubjectsInformation_label->clear();
    }
}

void FADTTSWindow::DisplayNbrSubjectSelected()
{
    this->subjectCovariateTab_checkedSubjectsInformation_label->clear();
    int nbrSubjectsSelected = 0;

    if( m_matchedSubjectListWidget->count() != 0 )
    {
        for( int i = 0; i < m_matchedSubjectListWidget->count(); i++ )
        {
            QListWidgetItem *item = m_matchedSubjectListWidget->item( i );

            if( item->checkState() == Qt::Checked )
            {
                nbrSubjectsSelected++;
            }
        }

        this->subjectCovariateTab_checkedSubjectsInformation_label->setText( tr( qPrintable( QString::number( nbrSubjectsSelected ) + " subjects selected") ) );
    }
}


int FADTTSWindow::SearchSubjects( QListWidget *list )
{
    int nbrFound = 0;
    QLineEdit *lineEdit = this->subjectCovariateTab_search_lineEdit;
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
                    nbrFound++;
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
    return nbrFound;
}


/****************************************************************/
/************************ Execution  tab ************************/
/****************************************************************/

/***********************  Private  slots  ***********************/
void FADTTSWindow::SetCurrentFiberNameInfo()
{
    HideShowPlotTab();
}

void FADTTSWindow::SetOutputDir()
{
    QLineEdit *lineEdit = this->para_executionTab_outputDir_lineEdit;
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
        lineEdit->setText( dirPath );
    }
}

void FADTTSWindow::UpdateOutputDir( const QString&  path )
{
    QLabel *label = this->executionTab_iconOutputDir_label;
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
    HideShowPlotTab();
}

void FADTTSWindow::RunningMatlabAfterFileGeneration( const bool& choice )
{
    QLabel *matlabIcon = this->executionTab_iconMatlab_label;
    QPushButton *matlabPushButton = this->executionTab_matlab_pushButton;
    QLineEdit *matlabLineEdit = this->soft_executionTab_matlab_lineEdit;
    if( !choice )
    {
        matlabLineEdit->clear();
    }
    matlabIcon->setEnabled( choice );
    matlabPushButton->setEnabled( choice );
    matlabLineEdit->setEnabled( choice );
}

void FADTTSWindow::SetMVCMPath()
{
    QLineEdit *lineEdit = this->soft_executionTab_mvcm_lineEdit;
    QString filePath = lineEdit->text();
    QDir dir = filePath;

    QString dirPath;
    if( dir.exists() && !filePath.isEmpty() )
    {
        dirPath = QFileDialog::getExistingDirectory( this, tr( "Choose MVCM Directory" ), dir.absolutePath(), QFileDialog::ShowDirsOnly );
    }
    else
    {
        dirPath = QFileDialog::getExistingDirectory( this, tr( "Choose MVCM Directory" ), m_mvcmPath, QFileDialog::ShowDirsOnly );
    }
    if( !dirPath.isEmpty() )
    {
        lineEdit->setText( dirPath );
    }
}

void FADTTSWindow::UpdateMVCMPath( const QString& path )
{
    QLabel *label = this->executionTab_iconMVCM_label;
    if( !path.isEmpty() )
    {
        if( QDir( path ).exists() )
        {
            DisplayIcon( label, m_okPixmap );
            m_mvcmPath = path;
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
    QLineEdit *lineEdit = this->soft_executionTab_matlab_lineEdit;
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
}

void FADTTSWindow::UpdateMatlabExe( const QString& executable )
{
    QFile matlabExe( executable );
    QLabel *label = this->executionTab_iconMatlab_label;
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

    QString fiberName = this->para_executionTab_fiberName_lineEdit->text();
    QMap<int, QString> selectedCovariates = GetSelectedCovariates();
    if( IsRunFADTTSOK( fiberName, selectedCovariates ) )
    {
        this->executionTab_run_pushButton->setEnabled( false );
        this->executionTab_stop_pushButton->setEnabled( true );

        SetMatlabThread( fiberName, selectedCovariates );

        m_progressBar->show();
        m_matlabThread->start();
    }
}

void FADTTSWindow::StopFADTTS()
{
    if( m_matlabThread->isRunning() )
    {
        QMessageBox::StandardButton closeBox =
                QMessageBox::question( this, tr( "FADTTSter" ), tr( "Data are still being processed.<br>Are you sure you want to stop the execution?" ),
                                       QMessageBox::No | QMessageBox::Yes, QMessageBox::No );
        switch( closeBox )
        {
        case QMessageBox::No:
            break;
        case QMessageBox::Yes:
        {
            *m_textStreamLog << "Warning! Thread terminated by user before script completed matlab script.\n" << endl;
            m_matlabThread->terminate();
            m_logWindow->insertPlainText( "Warning! Thread terminated by user before script completed matlab script.\n");
            break;
        }
        default:
            break;
        }
    }
}

void FADTTSWindow::DisplayLog()
{
    QScrollBar *scrollBar = m_logWindow->verticalScrollBar();

    QString line = m_textStreamLog->readAll();
    if( scrollBar->value() == scrollBar->maximum() )
    {
        m_logWindow->insertPlainText( line );
        scrollBar->setValue( scrollBar->maximum() );
    }
    else
    {
        m_logWindow->insertPlainText(line);
    }
}

void FADTTSWindow::ClearLog()
{
    m_logWindow->clear();
}

void FADTTSWindow::OnThreadFinished()
{
    this->executionTab_run_pushButton->setEnabled( true );
    this->executionTab_stop_pushButton->setEnabled( false );
    m_logFile->flush();
    m_logFile->close();
    m_progressBar->hide();
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
    m_outcomeSelected.clear();
    QMap< QPair< int, QString >, bool > selectedInputFiles;
    int i = 0;
    foreach ( QString prefID, m_data.GetPrefixList() )
    {
        QPair< int, QString > currentPair;
        if( m_paramTabFileCheckBoxMap[ prefID ]->isChecked() && !m_data.GetFilename( prefID ).isEmpty() )
        {
            currentPair.first = i;
            currentPair.second = m_data.GetFilename( prefID );
            m_outcomeSelected.insert( i, prefID );
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


QString FADTTSWindow::GenerateSelectedSubjectFile( QString outputDir )
{
    QFile selectedSubjects( outputDir + "/" + this->para_executionTab_fiberName_lineEdit->text() + "_subjectList.txt" );
    m_nbrSelectedSubjects = 0;
    if( selectedSubjects.open( QIODevice::WriteOnly ) )
    {
        QTextStream tsSelectedSubjects( &selectedSubjects );
        for( int i = 0; i < m_matchedSubjectListWidget->count(); i++ )
        {
            if( m_matchedSubjectListWidget->item( i )->checkState() == Qt::Checked )
            {
                m_nbrSelectedSubjects++;
                tsSelectedSubjects << m_matchedSubjectListWidget->item( i )->text() << endl;
            }
        }
        selectedSubjects.flush();
        selectedSubjects.close();
    }

    return selectedSubjects.fileName();
}


bool FADTTSWindow::IsRunFADTTSOK( QString fiberName, QMap<int, QString> selectedCovariates )
{
    bool fiberNameProvided = !fiberName.isEmpty();
    bool atLeastOneDataFileChosen = ( this->para_subjectCovariateTab_adFile_checkBox->isEnabled() || this->para_subjectCovariateTab_rdFile_checkBox->isEnabled() ||
                                      this->para_subjectCovariateTab_mdFile_checkBox->isEnabled() || this->para_subjectCovariateTab_faFile_checkBox->isEnabled() );
    bool covariateFileChosen = this->para_subjectCovariateTab_covariateFile_checkBox->isEnabled();
    bool atLeastOneDataFileChecked = ( this->para_subjectCovariateTab_adFile_checkBox->isChecked() || this->para_subjectCovariateTab_rdFile_checkBox->isChecked() ||
                                       this->para_subjectCovariateTab_mdFile_checkBox->isChecked() || this->para_subjectCovariateTab_faFile_checkBox->isChecked() );
    bool covariateFileChecked = this->para_subjectCovariateTab_covariateFile_checkBox->isChecked();

    bool atLeastOneCovariateChecked = selectedCovariates.count() != 0;
    bool mvcmPathSpecified = !this->soft_executionTab_mvcm_lineEdit->text().isEmpty();
    bool matlabExeSpecified;
    bool runMatlab = this->para_executionTab_matlab_checkBox->isChecked();
    m_matlabThread->SetRunMatlab( runMatlab );
    if( !runMatlab )
    {
        matlabExeSpecified = true;
    }
    else
    {
        matlabExeSpecified = !this->soft_executionTab_matlab_lineEdit->text().isEmpty();
    }


    if( !fiberNameProvided || !atLeastOneDataFileChosen || !covariateFileChosen ||
            !atLeastOneDataFileChecked || !covariateFileChecked ||
            !atLeastOneCovariateChecked || !mvcmPathSpecified || !matlabExeSpecified )
    {
        QString warningText = "<b>FADTTSter will not be executed for the following reason(s):</b><br>";
        if( !atLeastOneDataFileChosen || !covariateFileChosen )
        {
            warningText.append( "Inputs Tab<br>" );
            if( !atLeastOneDataFileChosen )
            {
                warningText.append( "- Provide at least 1 data file (AD, RD, MR or FA)<br>" );
            }
            if( !covariateFileChosen )
            {
                warningText.append( "- No covariate file provided<br>" );
            }
        }
        if( atLeastOneDataFileChosen || covariateFileChosen )
        {
            if( ( !atLeastOneDataFileChecked && atLeastOneDataFileChosen ) || ( !covariateFileChecked && covariateFileChosen ) || !atLeastOneCovariateChecked )
            {
                warningText.append( "Subjects / Covariates Tab<br>" );
                if( !atLeastOneDataFileChecked && atLeastOneDataFileChosen )
                {
                    warningText.append( "- Select at least 1 data file (AD, RD, MR or FA)<br>" );
                }
                if( !covariateFileChecked && covariateFileChosen )
                {
                    warningText.append( "- Covariate file not selected<br>" );
                }
                if( covariateFileChecked )
                {
                    if( !atLeastOneCovariateChecked )
                    {
                        warningText.append( "- Select at least 1 covariate<br>" );
                    }
                }
            }
        }
        if( !fiberNameProvided || !matlabExeSpecified || !mvcmPathSpecified )
        {
            warningText.append( "Execution Tab<br>" );
            if( !fiberNameProvided )
            {
                warningText.append( "- No fiber name provided<br>" );
            }
            if( !matlabExeSpecified )
            {
                warningText.append( "- Specify the matlab executable<br>" );
            }
            if( !mvcmPathSpecified )
            {
                warningText.append( "- Specify the path to FADTTS matlab function (MVCM)<br>" );
            }
        }
        WarningPopUp( warningText );
        return false;
    }
    else
    {
        return true;
    }
}

void FADTTSWindow::SetMatlabThread( QString fiberName, QMap<int, QString> selectedCovariates )
{
    QString outputDir = m_data.GetOutputDir() + "/FADTTSter_" + fiberName;
    QDir().mkpath( outputDir );

    QStringList selectedPrefixes = GetSelectedPrefixes();
    QMap< QPair< int, QString >, bool> selectedInputFiles = GetSelectedInputFiles();
    QString selectedSubjectListFilePath = GenerateSelectedSubjectFile( outputDir );
    QMap< QPair< int, QString >, bool> matlabInputFiles =
            m_processing.GenerateMatlabInputFiles( selectedInputFiles, m_outcomeSelected, selectedSubjectListFilePath,
                                                m_data.GetCovariateFileSubjectColumnID(), selectedCovariates, outputDir,
                                                fiberName );
    int nbrPermutations = this->para_executionTab_nbrPermutations_spinBox->value();
    bool omnibusON = this->para_executionTab_omnibus_checkBox->isChecked();
    bool postHocON = this->para_executionTab_postHoc_checkBox->isChecked();

    m_matlabScript.SetMatlabOutputDir( outputDir );
    m_matlabScript.SetMatlabScriptName( "/FADTTSAnalysis_MatlabScript_" + fiberName + "_" + QString::number( nbrPermutations ) + "perm.m" );
    m_matlabScript.InitMatlabScript();
    m_matlabScript.SetHeader();
    m_matlabScript.SetMVCMPath( this->soft_executionTab_mvcm_lineEdit->text() );
    m_matlabScript.SetFiberName( fiberName );
    m_matlabScript.SetDiffusionProperties( selectedPrefixes );
    m_matlabScript.SetNbrPermutation( nbrPermutations );
    m_matlabScript.SetCovariates( selectedCovariates );
    m_matlabScript.SetInputFiles( matlabInputFiles );
    m_matlabScript.SetOmnibus( omnibusON );
    m_matlabScript.SetPostHoc( postHocON );

    m_matlabThread->SetMatlabExe( this->soft_executionTab_matlab_lineEdit->text() );

    SetLogDisplay( outputDir, fiberName, matlabInputFiles, selectedCovariates, nbrPermutations, omnibusON, postHocON );
}

void FADTTSWindow::SetLogDisplay( QString outputDir, QString fiberName, QMap< QPair< int, QString >, bool> matlabInputFiles,
                                  QMap<int, QString> selectedCovariates, int nbrPermutations, bool omnibusON, bool postHocON )
{
    m_logWindow->clear();

    // Log File
    m_logFile = new::QFile( outputDir + "/" + fiberName + ".log" );
    m_logFile->open(QIODevice::ReadWrite);
    m_textStreamLog = new QTextStream( m_logFile );

    // QFileSystemWatcher
    QFileSystemWatcher* log_watcher = new QFileSystemWatcher( this );
    log_watcher->addPath( m_logFile->fileName() );
    connect( log_watcher, SIGNAL( fileChanged( QString ) ), this, SLOT( DisplayLog() ) );

    //
    m_matlabThread->SetLogFile( m_logFile );

    // Init log file
    *m_textStreamLog << QDate::currentDate().toString( "MM/dd/yyyy" ) <<
                            " " << QTime::currentTime().toString( "hh:mmap" ) << endl;
    *m_textStreamLog << "FADTTSter: " << QString( FADTTS_VERSION ).prepend( "V" ) << endl;
    *m_textStreamLog << "/****************************************************************/" << endl;
    *m_textStreamLog << "/********************** FADTTSter LOG FILE **********************/" << endl;
    *m_textStreamLog << "/****************************************************************/" << endl << endl;
    *m_textStreamLog << "/**********************       Inputs       **********************/" << endl;
    *m_textStreamLog << "- fiber name: " << fiberName << endl;
    *m_textStreamLog << "- nbr subjects: " << QString::number( m_nbrSelectedSubjects ) << endl;
    *m_textStreamLog << "- matlabInputFiles: ";
    QMap< QPair< int, QString >, bool>::ConstIterator iterMatlabInputFiles = matlabInputFiles.begin();
    while( iterMatlabInputFiles != matlabInputFiles.end() )
    {
        *m_textStreamLog << QFileInfo( QFile( iterMatlabInputFiles.key().second ) ).fileName() << ", ";
        ++iterMatlabInputFiles;
    }
    *m_textStreamLog << endl;
    *m_textStreamLog << "- nbr covariates: " << QString::number( selectedCovariates.size() ) << endl;
    *m_textStreamLog << "- selected covariates: ";
    QMap<int, QString>::ConstIterator iterCovariates = selectedCovariates.begin();
    while( iterCovariates != selectedCovariates.end() )
    {
        *m_textStreamLog << iterCovariates.value() << ", ";
        ++iterCovariates;
    }
    *m_textStreamLog << endl;
    *m_textStreamLog << "- nbr permutations: " << QString::number( nbrPermutations ) << endl;
    *m_textStreamLog << "- omnibus: " << omnibusON << endl;
    *m_textStreamLog << "- post hoc: " << postHocON << endl << endl;
    *m_textStreamLog << "/**********************       Process      **********************/" << endl;
    *m_textStreamLog << "Thread starts running..." << endl << endl;
    m_logFile->flush();
}

/****************************************************************/
/************************ Plotting  tab *************************/
/****************************************************************/

/***********************  Private  slots  ***********************/
void FADTTSWindow::UpdateOutcomeGiven( const QStringList& outcomeGiven )
{
    m_outcomeComboBox->clear();
    m_outcomeComboBox->addItem( "" );
    m_outcomeComboBox->addItems( outcomeGiven );
}

void FADTTSWindow::UpdateCovariateGiven( const QMap<int, QString> &covariateGiven )
{
    m_covariateComboBox->clear();
    m_covariateComboBox->addItem( "" );
    QMap<int, QString>::ConstIterator iterCovariateGiven = covariateGiven.begin();
    while( iterCovariateGiven != covariateGiven.end() )
    {
        m_covariateComboBox->addItem( iterCovariateGiven.value() );
        ++iterCovariateGiven;
    }
}


void FADTTSWindow::SelectPlot( const QString& plotSelected )
{
    m_plot->SelectPlot( plotSelected );

    if( plotSelected == "No Plot" )
    {
        PlotSelected( false, false, false );
    }
    if( plotSelected == "Raw Data" )
    {
        PlotSelected( true, true, true );
    }
    if( plotSelected == "Raw Stats" )
    {
        PlotSelected( true, true, true );
    }
    if( plotSelected == "Raw Betas" )
    {
        PlotSelected( true, true, false );
    }
    if( plotSelected == "Omnibus Local pvalues" )
    {
        PlotSelected( true, false, false );
    }
    if( plotSelected == "Omnibus FDR Local pvalues" )
    {
        PlotSelected( true, false, false );
    }
    if( plotSelected == "Omnibus FDR Significant Betas by Properties" )
    {
        PlotSelected( true, true, false );
    }
    if( plotSelected == "Omnibus FDR Significant Betas by Covariates" )
    {
        PlotSelected( true, false, true );
    }
    if( plotSelected == "Post-Hoc Local pvalues" )
    {
        PlotSelected( true, true, false );
    }
    if( plotSelected == "Post-Hoc FDR Local pvalues" )
    {
        PlotSelected( true, true, false );
    }
    if( plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        PlotSelected( true, true, false );
    }
    if( plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        PlotSelected( true, false, true );
    }
}

void FADTTSWindow::SelectOutcome( const QString& outcomeSelected )
{
    m_plot->SelectOutcome( outcomeSelected );
}

void FADTTSWindow::SelectCovariate( const QString& covariateSelected )
{
    m_plot->SelectCovariate( covariateSelected );
}

void FADTTSWindow::OnYMinToggled( const bool& checkState )
{
    if( !checkState )
    {
        this->plottingTab_loadSetDataTab_yMin_doubleSpinBox->setValue( -0.5 );
    }
    this->plottingTab_loadSetDataTab_yMin_doubleSpinBox->setEnabled( checkState );
}

void FADTTSWindow::OnYMaxToggled( const bool& checkState )
{
    if( !checkState )
    {
        this->plottingTab_loadSetDataTab_yMax_doubleSpinBox->setValue( 0.5 );
    }
    this->plottingTab_loadSetDataTab_yMax_doubleSpinBox->setEnabled( checkState );
}


void FADTTSWindow::DisplayPlot()
{
    m_plot->ResetPlot();
    m_plot->SetTitle( this->plottingTab_loadSetDataTab_title_lineEdit->text() );
    m_plot->SetxName( this->plottingTab_loadSetDataTab_xName_lineEdit->text() );
    m_plot->SetyName( this->plottingTab_loadSetDataTab_yName_lineEdit->text() );
    m_plot->SetYMin( this->plottingTab_loadSetDataTab_yMin_checkBox->isChecked(), this->plottingTab_loadSetDataTab_yMin_doubleSpinBox->value() );
    m_plot->SetYMax( this->plottingTab_loadSetDataTab_yMax_checkBox->isChecked(), this->plottingTab_loadSetDataTab_yMax_doubleSpinBox->value() );
    m_plot->DisplayVTKPlot();

    this->plottingTab_titleLegendTab->setEnabled( true );
    this->plottingTab_editTab->setEnabled( true );
}

void FADTTSWindow::ResetPlot()
{
    m_plot->ResetPlot();
}


/***********************  Private  functions  ***********************/
void FADTTSWindow::HideShowPlotTab()
{
    QString fibername = this->para_executionTab_fiberName_lineEdit->text();
    QString outputDirectory = this->para_executionTab_outputDir_lineEdit->text();
    QString matlabOutputDir = outputDirectory + "/FADTTSter_" + fibername + "/MatlabOutputs";
    if( !QDir( matlabOutputDir ).exists() )
    {
        matlabOutputDir = "";
    }
    else
    {
        m_plot->SetDirectory( outputDirectory + "/FADTTSter_" + fibername );
    }

    bool isMatlabOutputDirDefine = !matlabOutputDir.isEmpty();
    this->plottingTab_loadSetDataTab_load_groupBox->setEnabled( isMatlabOutputDirDefine );
    if( isMatlabOutputDirDefine )
    {
        this->plottingTab_loadSetDataTab_currentFibernameSet_label->setText( fibername );
        this->plottingTab_loadSetDataTab_currentOutputDirectorySet_label->setText( outputDirectory );
    }
    else
    {
        ResetPlotTab();
    }
    PlotSelected( false, false, false );
}


void FADTTSWindow::PlotSelected( bool isPlotSelected, bool outcome, bool covariate )
{
    m_outcomeComboBox->setEnabled( outcome );
    m_outcomeComboBox->setCurrentText( "" );
    this->plottingTab_loadSetDataTab_outcomeSelection_label->setEnabled( outcome );
    m_covariateComboBox->setEnabled( covariate );
    m_covariateComboBox->setCurrentText( "" );
    this->plottingTab_loadSetDataTab_covariateSelection_label->setEnabled( covariate );

    this->plottingTab_loadSetDataTab_set_groupBox->setEnabled( isPlotSelected );

    this->plottingTab_loadSetDataTab_title_lineEdit->clear();
    this->plottingTab_loadSetDataTab_xName_lineEdit->clear();
    this->plottingTab_loadSetDataTab_yName_lineEdit->clear();
    this->plottingTab_loadSetDataTab_yMin_checkBox->setChecked( false );
    this->plottingTab_loadSetDataTab_yMax_checkBox->setChecked( false );

    this->plottingTab_loadSetDataTab_displayPlot_pushButton->setEnabled( isPlotSelected );
}

void FADTTSWindow::ResetPlotTab()
{
    m_plot->ResetDataFile();
    m_plot->ResetPlot();

    this->plottingTab_loadSetDataTab_currentFibernameSet_label->setText( "N/A" );
    this->plottingTab_loadSetDataTab_currentOutputDirectorySet_label->setText( "N/A" );

    this->plottingTab_loadSetDataTab_plotSelection_comboBox->setCurrentText( "No Plot" );

    this->plottingTab_loadSetDataTab_displayPlot_pushButton->setEnabled( false );

    this->plottingTab_titleLegendTab->setEnabled( false );
    this->plottingTab_editTab->setEnabled( false );
}
