#include "FADTTSWindow.h"

/****************************************************************/
/******************** Configuration && Events *******************/
/****************************************************************/
const QColor FADTTSWindow::m_green = QColor( 0,255,0,127 );
const QColor FADTTSWindow::m_red = QColor( 255,0,0,127 );
const QColor FADTTSWindow::m_grey = QColor( 220,220,220,255 );
const QColor FADTTSWindow::m_yellow = QColor( 255,255,0,127 );
const QColor FADTTSWindow::m_lightBlack = QColor( 0,0,0,191 );

const QString FADTTSWindow::m_csvSeparator = QLocale().groupSeparator();

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
void FADTTSWindow::OnLoadParaSettings()
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

void FADTTSWindow::OnSaveParaSettings()
{
    SyncUiToModelStructure();
    QString dir;
    QString filename = QFileDialog::getSaveFileName( this , tr( "Save Param Settings" ) ,  "newParaSettings.xml" , tr( ".xml( *.xml ) ;; .*( * )" ) );
    if( !filename.isEmpty() )
    {
        QFileInfo fi( filename ) ;
        dir = fi.dir().absolutePath() ;
        Save_Parameter_Configuration( filename.toStdString() );
    }
}

void FADTTSWindow::OnLoadSoftSettings()
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

void FADTTSWindow::OnSaveSoftSettings()
{
    SyncUiToModelStructure();
    QString dir;
    QString filename = QFileDialog::getSaveFileName( this , tr( "Save Soft Settings" ) ,  "newSoftSettings.xml" , tr( ".xml( *.xml ) ;; .*( * )" ) );
    if( !filename.isEmpty() )
    {
        QFileInfo fi( filename ) ;
        dir = fi.dir().absolutePath() ;
        Save_Software_Configuration( filename.toStdString() );
    }
}

void FADTTSWindow::OnDisplayAbout()
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
void FADTTSWindow::InitMenuBar()
{
    /** Load parameters (para_) from a .xml file into the GUI interface **/
    connect( this->actionLoad_Param_Settings, SIGNAL( triggered() ), SLOT( OnLoadParaSettings() ) );
    /** Save parameters (para_) from the GUI interface into a .xml file **/
    connect( this->actionSave_Param_Settings, SIGNAL( triggered() ), SLOT( OnSaveParaSettings() ) );

    /** Load software parameters (soft_) from a .xml file into the GUI interface **/
    connect( this->actionLoad_Soft_Settings, SIGNAL( triggered() ), SLOT( OnLoadSoftSettings() ) );
    /** Save software parameters (soft_) from the GUI interface into a .xml file **/
    connect( this->actionSave_Soft_Settings, SIGNAL( triggered() ), SLOT( OnSaveSoftSettings() ) );

    connect( this->actionAbout, SIGNAL( triggered() ), SLOT( OnDisplayAbout() ) );
}

void FADTTSWindow::InitInputTab()
{
    connect( this->inputTab_addMultipleInputFiles_pushButton, SIGNAL( clicked() ), this, SLOT( OnAddInputFiles() ) );

    /** Map of PushButtons to add each file separetely and
     *  SignalMapper to link them to the slot AddFile() **/
    m_inputTabAddInputFilePushButtonMap.insert( m_data.GetAxialDiffusivityIndex(), this->inputTab_addADFile_pushButton );
    m_inputTabAddInputFilePushButtonMap.insert( m_data.GetRadialDiffusivityIndex(), this->inputTab_addRDFile_pushButton );
    m_inputTabAddInputFilePushButtonMap.insert( m_data.GetMeanDiffusivityIndex(), this->inputTab_addMDFile_pushButton );
    m_inputTabAddInputFilePushButtonMap.insert( m_data.GetFractionalAnisotropyIndex(), this->inputTab_addFAFile_pushButton );
    m_inputTabAddInputFilePushButtonMap.insert( m_data.GetSubMatrixIndex(), this->inputTab_addSubMatrixFile_pushButton );
    QSignalMapper *signalMapperAddFile = new QSignalMapper( this );
    connect( signalMapperAddFile, SIGNAL( mapped( int ) ), this, SLOT( OnAddInputFile( int ) ) );
    for( int i = 0; i < m_data.GetDiffusionPropertiesIndices().size(); ++i )
    {
        connect( m_inputTabAddInputFilePushButtonMap[ m_data.GetDiffusionPropertiesIndices().at( i ) ], SIGNAL( clicked() ), signalMapperAddFile,SLOT(map() ) );
        signalMapperAddFile->setMapping( m_inputTabAddInputFilePushButtonMap[ m_data.GetDiffusionPropertiesIndices().at( i ) ], m_data.GetDiffusionPropertiesIndices().at( i ) );
    }

    /** Map of LineEdits where the file path of each file is set and
     *  SignalMapper to link them to the slot UpdateInputLineEdit() **/
    m_inputTabInputFileLineEditMap.insert( m_data.GetAxialDiffusivityIndex(), this->para_inputTab_adFile_lineEdit );
    m_inputTabInputFileLineEditMap.insert( m_data.GetRadialDiffusivityIndex(), this->para_inputTab_rdFile_lineEdit );
    m_inputTabInputFileLineEditMap.insert( m_data.GetMeanDiffusivityIndex(), this->para_inputTab_mdFile_lineEdit );
    m_inputTabInputFileLineEditMap.insert( m_data.GetFractionalAnisotropyIndex(), this->para_inputTab_faFile_lineEdit );
    m_inputTabInputFileLineEditMap.insert( m_data.GetSubMatrixIndex(), this->para_inputTab_subMatrixFile_lineEdit );
    QSignalMapper *signalMapperUpdateLineEdit = new QSignalMapper( this );
    connect( signalMapperUpdateLineEdit, SIGNAL( mapped( int ) ), this, SLOT( OnSettingInputFile( int ) ) );
    for( int i = 0; i < m_data.GetDiffusionPropertiesIndices().size(); ++i )
    {
        connect( m_inputTabInputFileLineEditMap[ m_data.GetDiffusionPropertiesIndices().at( i ) ], SIGNAL( textChanged( const QString& ) ), signalMapperUpdateLineEdit,SLOT(map() ) );
        signalMapperUpdateLineEdit->setMapping( m_inputTabInputFileLineEditMap[ m_data.GetDiffusionPropertiesIndices().at( i ) ], m_data.GetDiffusionPropertiesIndices().at( i ) );
    }

    /** Map of Labels to set the icon information of each file entered in a LineEdit **/
    m_inputTabIconLabelMap.insert( m_data.GetAxialDiffusivityIndex(), this->inputTab_iconADFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetRadialDiffusivityIndex(), this->inputTab_iconRDFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetMeanDiffusivityIndex(), this->inputTab_iconMDFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetFractionalAnisotropyIndex(), this->inputTab_iconFAFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetSubMatrixIndex(), this->inputTab_iconSubMatrixFile_label );

    m_editInputDialog = QSharedPointer< EditInputDialog >( new EditInputDialog( this ) );
    m_editInputDialog->SetData( &m_data );
    connect( m_editInputDialog.data(), SIGNAL( UpdateInputFile( const int&, const QString& ) ), this, SLOT( OnUpdatingInputFile( const int&, const QString& ) ) );
    connect( m_editInputDialog.data(), SIGNAL( UpdateSubjectColumnID( int ) ), this, SLOT( OnUpdatingSubjectColumnID( int ) ) );

    /** Map of PushButtons to edit the files and
     *  SignalMapper to link them to the slot EditFile() **/
    m_inputTabEditInputFilePushButtonMap.insert( m_data.GetAxialDiffusivityIndex(), this->inputTab_editADFile_pushButton );
    m_inputTabEditInputFilePushButtonMap.insert( m_data.GetRadialDiffusivityIndex(), this->inputTab_editRDFile_pushButton );
    m_inputTabEditInputFilePushButtonMap.insert( m_data.GetMeanDiffusivityIndex(), this->inputTab_editMDFile_pushButton );
    m_inputTabEditInputFilePushButtonMap.insert( m_data.GetFractionalAnisotropyIndex(), this->inputTab_editFAFile_pushButton );
    m_inputTabEditInputFilePushButtonMap.insert( m_data.GetSubMatrixIndex(), this->inputTab_editSubMatrixFile_pushButton );
    QSignalMapper *signalMapperEditFile = new QSignalMapper( this );
    connect( signalMapperEditFile, SIGNAL( mapped( int ) ), this, SLOT( OnEditInputFile( int ) ) );
    for( int i = 0; i < m_data.GetDiffusionPropertiesIndices().size(); ++i )
    {
        connect( m_inputTabEditInputFilePushButtonMap[ m_data.GetDiffusionPropertiesIndices().at( i ) ], SIGNAL( clicked() ), signalMapperEditFile, SLOT(map() ) );
        signalMapperEditFile->setMapping( m_inputTabEditInputFilePushButtonMap[ m_data.GetDiffusionPropertiesIndices().at( i ) ], m_data.GetDiffusionPropertiesIndices().at( i ) );
    }

    /** Map of Labels to edit the file information and
     *  SignalMapper to link them to the slot EditFile() **/
    m_inputFileInformationLabelMap.insert( m_data.GetAxialDiffusivityIndex(), this->inputTab_adFileInfo_label );
    m_inputFileInformationLabelMap.insert( m_data.GetRadialDiffusivityIndex(), this->inputTab_rdFileInfo_label );
    m_inputFileInformationLabelMap.insert( m_data.GetMeanDiffusivityIndex(), this->inputTab_mdFileInfo_label );
    m_inputFileInformationLabelMap.insert( m_data.GetFractionalAnisotropyIndex(), this->inputTab_faFileInfo_label );
    m_inputFileInformationLabelMap.insert( m_data.GetSubMatrixIndex(), this->inputTab_subMatrixFileInfo_label );

    m_covariateListWidget = new QListWidget();
    m_covariateListWidget = this->para_inputTab_covariates_listWidget;
    connect( m_covariateListWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( OnCovariateClicked( QListWidgetItem * ) ) );
    connect( this->inputTab_covariatesCheckAll_pushButton, SIGNAL( clicked() ), this, SLOT( OnCheckAllCovariates() ) );
    connect( this->inputTab_covariatesUncheckAll_pushButton, SIGNAL( clicked() ), this, SLOT( OnUnCheckAllCovariates() ) );

    DisplayFileInformation();
}

void FADTTSWindow::InitSubjectCovariateTab()
{
    /*** Inputs Available ***/
    /** Map of CheckBoxes to select the files we want to work on and
     *  SignalMapper to link them to the slot SortSubjects() **/
    m_paramTabFileCheckBoxMap.insert( m_data.GetAxialDiffusivityIndex(), this->para_subjectTab_adFile_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetRadialDiffusivityIndex(), this->para_subjectTab_rdFile_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetMeanDiffusivityIndex(), this->para_subjectTab_mdFile_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetFractionalAnisotropyIndex(), this->para_subjectTab_faFile_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetSubMatrixIndex(), this->para_subjectTab_subMatrixFile_checkBox );
    QSignalMapper *signalMapperSelectFile = new QSignalMapper( this );
    connect( signalMapperSelectFile, SIGNAL( mapped( int ) ), this, SLOT( OnInputToggled() ) );
    for ( int i = 0; i < m_data.GetDiffusionPropertiesIndices().size(); ++i )
    {
        connect( m_paramTabFileCheckBoxMap[ m_data.GetDiffusionPropertiesIndices().at( i ) ], SIGNAL( toggled( bool ) ), signalMapperSelectFile,SLOT( map() ) );
        signalMapperSelectFile->setMapping( m_paramTabFileCheckBoxMap[ m_data.GetDiffusionPropertiesIndices().at( i ) ], m_data.GetDiffusionPropertiesIndices().at( i ) );
    }

    /** Map of Labels displaying the matrix data size of the files that have been chosen **/
//    m_paramTabFileDataSizeLabelMap.insert( m_data.GetAxialDiffusivityIndex(), this->subjectTab_adFileInfo_label );
//    m_paramTabFileDataSizeLabelMap.insert( m_data.GetRadialDiffusivityIndex(),this->subjectTab_rdFileInfo_label );
//    m_paramTabFileDataSizeLabelMap.insert( m_data.GetMeanDiffusivityIndex(), this->subjectTab_mdFileInfo_label );
//    m_paramTabFileDataSizeLabelMap.insert( m_data.GetFractionalAnisotropyIndex(), this->subjectTab_faFileInfo_label );
//    m_paramTabFileDataSizeLabelMap.insert( m_data.GetSubMatrixIndex(), this->subjectTab_subMatrixFileInfo_label );

    /*** QC Threshold ***/
    m_qcThresholdDialog = QSharedPointer< QCThresholdDialog >( new QCThresholdDialog( this ) );
    connect( this->subjectTab_applyQCThreshold_pushButton, SIGNAL( clicked() ), this, SLOT( OnApplyQCThreshold() ) );
    connect( m_qcThresholdDialog.data(), SIGNAL( ApplyQCThreshold( const QStringList&, const QStringList&, double ) ), this, SLOT( OnApplyingQCThreshold( const QStringList&, const QStringList&, double ) ) );

    /*** Subjects Lists ***/
    m_areSubjectsLoaded = false;

    m_matchedSubjectListWidget = new QListWidget();
    m_matchedSubjectListWidget = this->subjectTab_matchedSubjectsInformation_listWidget;
    connect( m_matchedSubjectListWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( OnSubjectClicked( QListWidgetItem * ) ) );
    connect( this->subjectTab_checkAllVisible_pushButton, SIGNAL( clicked() ), this, SLOT( OnCheckAllVisibleSubjects() ) );
    connect( this->subjectTab_unCheckAllVisible_pushButton, SIGNAL( clicked() ), this, SLOT( OnUnCheckAllVisibleSubjects() ) );

    m_unmatchedSubjectListWidget = new QListWidget();
    m_unmatchedSubjectListWidget = this->subjectTab_unmatchedSubjectsInformation_listWidget;

    /*** Search Subjects ***/
    m_caseSensitivity = Qt::CaseInsensitive;
    connect( this->subjectTab_search_lineEdit, SIGNAL( textEdited( const QString& ) ), this, SLOT( OnSearch() ) );
    connect( this->subjectTab_caseSensitive_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnSetCaseSensitivityToggled( bool ) ) );

    /*** Subjects File ***/
    m_subjectFileLineEdit = new QLineEdit();
    m_subjectFileLineEdit = this->para_subjectTab_subjectFile_lineEdit;
    connect( m_subjectFileLineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnSettingSubjectList( const QString&  ) ) );
    connect( this->subjectTab_loadSubjectFile_PushButton, SIGNAL( clicked() ), this, SLOT( OnLoadSubjectList() ) );
    connect( this->subjectTab_resetSubjectFile_pushButton, SIGNAL( clicked() ), this, SLOT( OnResetSubjectList() ) );
    connect( this->subjectTab_saveCheckedSubjects_pushButton, SIGNAL( clicked() ), this, SLOT( OnSaveCheckedSubjects() ) );
}

void FADTTSWindow::InitExecutionTab()
{
    /*** Settings ***/
    this->para_executionTab_nbrPermutations_spinBox->setMaximum( 2000 );

    /*** Output directory ***/
    connect( this->executionTab_outputDir_pushButton, SIGNAL( clicked() ), this, SLOT( OnBrowsingOutputDir() ) );
    connect( this->para_executionTab_outputDir_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnSettingOutputDir( const QString& ) ) );

    /*** Matlab Specification ***/
    m_isMatlabExeFound = false;

    connect( this->executionTab_mvcm_pushButton, SIGNAL( clicked() ), this, SLOT( OnBrowsingMVCMPath() ) );
    connect( this->soft_executionTab_mvcm_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnSettingMVCMPath( const QString& ) ) );

    connect( this->para_executionTab_runMatlab_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnRunMatlabToggled( bool ) ) );
    connect( this->executionTab_runMatlab_pushButton, SIGNAL( clicked() ), this, SLOT( OnBrowsingMatlabExe() ) );
    connect( this->soft_executionTab_runMatlab_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnSettingMatlabExe( const QString& ) ) );

    this->soft_executionTab_nbrCompThreads_spinBox->setMaximum( QThread::idealThreadCount() );

    /*** Run ***/
    m_progressBar = new QProgressBar();
    m_progressBar = this->executionTab_progressBar;
    m_progressBar->setMinimum( 0 );
    m_progressBar->setMaximum( 0 );
    m_progressBar->hide();

    m_matlabThread = new MatlabThread();
    connect( m_matlabThread, SIGNAL( finished() ), this, SLOT( OnMatlabThreadFinished() ) );

    connect( this->executionTab_run_pushButton, SIGNAL( clicked() ), this, SLOT( OnRun() ) );
    connect( this->executionTab_stop_pushButton, SIGNAL( clicked() ), this, SLOT( OnStop() ) );
    this->executionTab_run_pushButton->setEnabled( true );
    this->executionTab_stop_pushButton->setEnabled( false );

    /*** Log ***/
    m_logWindow = new QTextEdit();
    m_logWindow = this->executionTab_log_textEdit;
    m_logWindow->setReadOnly( true );

    connect( this->executionTab_clearLog_pushButton, SIGNAL( clicked() ), this, SLOT( OnClearLog() ) );


    OnRunMatlabToggled( false );
}

void FADTTSWindow::InitPlottingTab()
{
    m_qvtkWidget = QSharedPointer< QVTKWidget >( this->plottingTab_plot_qvtkWidget );

    m_plot = new Plot();
    m_plot->SetQVTKWidget( m_qvtkWidget );
    connect( m_plot, SIGNAL( PlotsUsed( const QStringList& ) ), this, SLOT( OnSettingPlotsUsed( const QStringList& ) ) );
    connect( m_plot, SIGNAL( AllPropertiesUsed( const QMap< int, QString >& ) ), this, SLOT( OnSettingAllPropertiesUsed( const QMap< int, QString >& ) ) );
    connect( m_plot, SIGNAL( AllCovariatesUsed( const QMap< int, QString >& ) ), this, SLOT( OnSettingAllCovariatesUsed( const QMap< int, QString >& ) ) );
    connect( m_plot, SIGNAL( CovariatesAvailableForPlotting( const QMap< int, QString >& ) ), this, SLOT( OnUpdatingCovariatesAvailable( const QMap< int, QString >& ) ) );
    connect( m_plot, SIGNAL( LinesSelected( const QStringList& ) ), this, SLOT( OnSettingLinesSelected( const QStringList& ) ) );

    /*** Load/Set Data Tab ***/
    connect( this->plottingTab_loadSetDataTab_browsePlotDirectory_pushButton, SIGNAL( clicked() ), this, SLOT( OnBrowsingPlotDir() ) );
    connect( this->para_plottingTab_loadSetDataTab_browsePlotDirectory_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnSettingPlotDir( const QString& ) ) );
    connect( this->para_plottingTab_loadSetDataTab_fibername_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnSettingPlotFibername( const QString& ) ) );


    m_plotComboBox = new QComboBox();
    m_plotComboBox = this->plottingTab_loadSetDataTab_plotSelection_comboBox;
    connect( m_plotComboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( OnPlotSelection( const QString& ) ) );

    m_propertyComboBox = new QComboBox();
    m_propertyComboBox = this->plottingTab_loadSetDataTab_propertySelection_comboBox;
    connect( m_propertyComboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( OnPropertySelection( const QString& ) ) );

    m_covariateComboBox = new QComboBox();
    m_covariateComboBox = this->plottingTab_loadSetDataTab_covariateSelection_comboBox;
    connect( m_covariateComboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( OnCovariateSelection( const QString& ) ) );

    m_lineDisplayedListWidget = new QListWidget();
    m_lineDisplayedListWidget = this->plottingTab_loadSetDataTab_linesToDisplay_listWidget;
    connect( m_lineDisplayedListWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( OnLineForDisplayClicked( QListWidgetItem * ) ) );
    connect( this->plottingTab_loadSetDataTab_checkAllLinesToDisplay_pushButton, SIGNAL( clicked() ), this, SLOT( OnCheckAllLinesToDisplay() ) );
    connect( this->plottingTab_loadSetDataTab_uncheckAllLinesToDisplay_pushButton, SIGNAL( clicked() ), this, SLOT( OnUncheckAllToDisplay() ) );

    m_lineSelectedListWidget = new QListWidget();
    m_lineSelectedListWidget = this->plottingTab_loadSetDataTab_linesSelected_listWidget;


    /*** Title/Axis/Legend Tab ***/
    connect( this->plottingTab_titleAxisLegendTab_useCustomizedTitle_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUseCustomizedTitle( bool ) ) );
    connect( this->plottingTab_titleAxisLegendTab_useCustomizedTitle_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingPlotTitle() ) );
    this->plottingTab_titleAxisLegendTab_useCustomizedTitle_checkBox->setChecked( false );

    connect( this->plottingTab_titleAxisLegendTab_titleName_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnUpdatingPlotTitle() ) );
    connect( this->plottingTab_titleAxisLegendTab_titleBold_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingPlotTitle() ) );
    connect( this->plottingTab_titleAxisLegendTab_titleItalic_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingPlotTitle() ) );
    connect( this->plottingTab_titleAxisLegendTab_titleSize_doubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnUpdatingPlotTitle() ) );

    connect( this->plottingTab_titleAxisLegendTab_gridOn_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingPlotGrid( bool ) ) );

    connect( this->plottingTab_titleAxisLegendTab_useCustomizedAxis_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUseCustomizedAxis( bool ) ) );
    connect( this->plottingTab_titleAxisLegendTab_useCustomizedAxis_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingPlotAxis() ) );
    this->plottingTab_titleAxisLegendTab_useCustomizedAxis_checkBox->setChecked( false );

    connect( this->plottingTab_titleAxisLegendTab_xName_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnUpdatingPlotAxis() ) );
    connect( this->plottingTab_titleAxisLegendTab_yName_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnUpdatingPlotAxis() ) );
    connect( this->plottingTab_titleAxisLegendTab_axisBold_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingPlotAxis() ) );
    connect( this->plottingTab_titleAxisLegendTab_axisItalic_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingPlotAxis() ) );

    connect( this->plottingTab_titleAxisLegendTab_yMax_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnYMaxToggled( bool ) ) );
    connect( this->plottingTab_titleAxisLegendTab_yMax_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingPlotAxis() ) );
    this->plottingTab_titleAxisLegendTab_yMax_checkBox->setChecked( false );

    connect( this->plottingTab_titleAxisLegendTab_yMin_doubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnYMinValueChanged( double ) ) );
    connect( this->plottingTab_titleAxisLegendTab_yMin_doubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnUpdatingPlotAxis() ) );

    connect( this->plottingTab_titleAxisLegendTab_yMin_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnYMinToggled( bool ) ) );
    connect( this->plottingTab_titleAxisLegendTab_yMin_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingPlotAxis() ) );
    this->plottingTab_titleAxisLegendTab_yMin_checkBox->setChecked( false );

    connect( this->plottingTab_titleAxisLegendTab_yMax_doubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnYMaxValueChanged( double ) ) );
    connect( this->plottingTab_titleAxisLegendTab_yMax_doubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnUpdatingPlotAxis() ) );

    connect( this->plottingTab_titleAxisLegendTab_legendPosition_comboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( OnUpdatingLegend( const QString& ) ) );


    /*** Edition Tab ***/
    connect( this->plottingTab_editionTab_pvalueThreshold_doubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnUpdatingPvalueThreshold( double ) ) );

    connect( this->plottingTab_editionTab_lineWidth_doubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnUpdatingLineWidth( double ) ) );
    connect( this->plottingTab_editionTab_selectedLineColor_comboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( OnUpdatingSelectedColorLine( const QString& ) ) );
    SetColorsComboBox( this->plottingTab_editionTab_selectedLineColor_comboBox );
    this->plottingTab_editionTab_selectedLineColor_comboBox->setCurrentText( "Red" );

    connect( this->plottingTab_editionTab_markerType_comboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( OnUpdatingMarkerType( const QString& ) ) );
    connect( this->plottingTab_editionTab_markerSize_doubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnUpdatingMarkerSize( double ) ) );



    connect( this->plottingTab_displayPlot_pushButton, SIGNAL( clicked() ), this, SLOT( OnDisplayPlot() ) );
    connect( this->plottingTab_resetPlot_pushButton, SIGNAL( clicked() ), this, SLOT( OnResetPlot() ) );
    connect( this->plottingTab_savePlot_pushButton, SIGNAL( clicked() ), m_plot, SLOT( OnSavePlot() ) );
    connect( this->plottingTab_loadPlotSettings_pushButton, SIGNAL( clicked() ), this, SLOT( OnLoadPlotSettings() ) );
    connect( this->plottingTab_savePlotSettings_pushButton, SIGNAL( clicked() ), this, SLOT( OnSavePlotSettings() ) );

    SetPlotTab();
}

void FADTTSWindow::InitFADTTSWindow()
{
    m_data.InitData();

    /** Initialization of the menu bar and all FADTTSter tabs **/
    InitMenuBar();
    InitInputTab();
    InitSubjectCovariateTab();
    InitExecutionTab();
    InitPlottingTab();

    InitAvailableDiffusionProperties();
}


QDir FADTTSWindow::UpdateCurrentDir( QString newfilePath, QString& currentDir )
{
    QDir dir = QFileInfo( QFile( newfilePath ) ).absolutePath();
    if( dir.exists() )
    {
        currentDir = dir.absolutePath();
    }

    return dir;
}

QDir FADTTSWindow::SetDir( QString filePath, QString currentDir )
{
    return ( !filePath.isEmpty() && QDir( QFileInfo( QFile( filePath ) ).absolutePath() ).exists() ) ?
                QFileInfo( QFile( filePath ) ).absolutePath() :
                currentDir;
}


void FADTTSWindow::WarningPopUp( QString warningMessage )
{
    QMessageBox::warning( this, tr( "WARNING" ), tr( qPrintable( warningMessage ) ), QMessageBox::Ok );
}

void FADTTSWindow::CriticalPopUp( QString criticalMessage )
{
    QMessageBox::critical( this, tr( "CRITICAL ERROR" ), tr( qPrintable( criticalMessage ) ), QMessageBox::Ok );
}

void FADTTSWindow::DisplayIcon( QLabel *label , const QPixmap& icon )
{
    if( !icon.isNull() )
    {
        label->setPixmap( icon.scaled( QSize( m_iconSize, m_iconSize ), Qt::IgnoreAspectRatio ) );
    }
}


/***************************************************************/
/************************** Input tab **************************/
/***************************************************************/

/***********************  Private slots  ***********************/
void FADTTSWindow::OnSettingInputFile( int diffusionPropertyIndex )
{
    QLineEdit *lineEdit = m_inputTabInputFileLineEditMap[ diffusionPropertyIndex ];
    QString filePath = lineEdit->text();
    QFile file( filePath );

    if( filePath.isEmpty() )
    {
        m_inputTabIconLabelMap[ diffusionPropertyIndex ]->clear();
        m_data.ClearData( diffusionPropertyIndex );
    }
    else
    {
        if( !file.open( QIODevice::ReadOnly ) )
        {
            DisplayInputLineEditIcon( diffusionPropertyIndex, m_koPixmap );
            m_data.ClearData( diffusionPropertyIndex );
        }
        else
        {
            file.close();
            QList< QStringList > fileData = m_processing.GetDataFromFile( filePath );
            if( m_processing.IsMatrixDimensionOK( fileData ) )
            {
                DisplayInputLineEditIcon( diffusionPropertyIndex, m_okPixmap );

                m_data.SetFilename( diffusionPropertyIndex ) = filePath;
                m_data.SetFileData( diffusionPropertyIndex ) = fileData;
            }
            else
            {
                DisplayInputLineEditIcon( diffusionPropertyIndex, m_koPixmap );

                m_data.ClearData( diffusionPropertyIndex );

                QString criticalMessage = m_data.GetDiffusionPropertyName( diffusionPropertyIndex ).toUpper()
                        + " data file corrupted:<br><i>" + m_inputTabInputFileLineEditMap[ diffusionPropertyIndex ]->text()
                        + "</i><br>For each row, the number of columns is not constant.<br>Check the data file provided.";
                CriticalPopUp( criticalMessage );
            }

            UpdateCurrentDir( filePath, m_currentInputFileDir );
        }
    }
    UpdateInputFileInformation( diffusionPropertyIndex );

    if( diffusionPropertyIndex == m_data.GetSubMatrixIndex() )
    {
        m_editInputDialog->ResetSubjectColumnID(); /** By default Subjects are on the 1st column. **/
        SetInfoSubjectColumnID();
    }

    OnInputToggled();
}

void FADTTSWindow::OnAddInputFiles()
{
    QString dir = m_currentInputFileDir;
    QStringList fileList;
    fileList = QFileDialog::getOpenFileNames( this, tr( "Choose Input Files" ), dir, tr( ".csv( *.csv ) ;; .*( * )" ) );
    if( !fileList.isEmpty() )
    {
        UpdateLineEditsAfterAddingMultipleFiles( fileList );
    }
}

void FADTTSWindow::OnAddInputFile( int diffusionPropertyIndex )
{
    QLineEdit *lineEdit = m_inputTabInputFileLineEditMap[ diffusionPropertyIndex ];
    QString filePath = lineEdit->text();
    QDir dir = SetDir( filePath, m_currentInputFileDir );
    QString file = QFileDialog::getOpenFileName( this, tr( qPrintable( "Choose " + m_data.GetDiffusionPropertyName( diffusionPropertyIndex ).toUpper() + " File" ) ), dir.absolutePath(), tr( ".csv( *.csv ) ;; .*( * )" ) );
    if( !file.isEmpty() )
    {
        lineEdit->setText( file );
    }
}

void FADTTSWindow::OnEditInputFile( int diffusionPropertyIndex )
{
    if( m_data.GetFilename( diffusionPropertyIndex ).isEmpty() )
    {
        QString warningMessage = "<b>File Edition Unable</b><br>";
        warningMessage.append( m_inputTabInputFileLineEditMap[ diffusionPropertyIndex ]->text().isEmpty() ?
                                   "No file specified" :
                                   "Could not open the file:<br><i>" + m_inputTabInputFileLineEditMap[ diffusionPropertyIndex ]->text() + "</i>" );
        WarningPopUp( warningMessage );
    }
    else
    {
        LaunchEditInputDialog( diffusionPropertyIndex );
    }
}


void FADTTSWindow::OnCovariateClicked( QListWidgetItem *item )
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

void FADTTSWindow::OnCheckAllCovariates()
{
    SetCheckStateAllCovariates( Qt::Checked );
}

void FADTTSWindow::OnUnCheckAllCovariates()
{
    SetCheckStateAllCovariates( Qt::Unchecked );
}


void FADTTSWindow::OnUpdatingInputFile( const int &diffusionPropertyIndex, const QString& newFilePath )
{
    m_inputTabInputFileLineEditMap[ diffusionPropertyIndex ]->setText( newFilePath );
}

void FADTTSWindow::OnUpdatingSubjectColumnID( int newSubjectColumnID )
{
    /** Subjects are not on the 1st column anymore. **/
    m_data.SetSubjectColumnID() = newSubjectColumnID;
    UpdateInputFileInformation( m_data.GetSubMatrixIndex() );
    SetInfoSubjectColumnID();

    UpdateSubjectList();
}


/*********************** Private function ***********************/
void FADTTSWindow::SetCovariateListVisible( bool visible )
{
    m_covariateListWidget->setHidden( !visible );
    this->inputTab_covariatesCheckAll_pushButton->setHidden( !visible );
    this->inputTab_covariatesUncheckAll_pushButton->setHidden( !visible );
    this->inputTab_covariatesInformation_label->setHidden( !visible );
}

void FADTTSWindow::SetCovariateListWidget()
{
    m_covariateListWidget->clear();

    QMap< int, QString > covariates = m_data.GetCovariates();
    if( !covariates.isEmpty() )
    {
        QMap< int, QString >::ConstIterator iterCovariate = covariates.cbegin();
        while( iterCovariate != covariates.cend() )
        {
            QListWidgetItem *covariateItem = new QListWidgetItem( iterCovariate.value(), m_covariateListWidget );
            covariateItem->setCheckState( Qt::Checked );
            covariateItem->setFlags( Qt::ItemIsEnabled );
            m_covariateListWidget->addItem( covariateItem );
            ++iterCovariate;
        }

        m_covariateListWidget->setMaximumHeight( m_covariateListWidget->sizeHintForRow( 0 ) * ( m_covariateListWidget->count() + 1 ) );
    }
}

QString FADTTSWindow::GetInputFileInformation( int diffusionPropertyIndex ) const
{
    QString fileInformation;
    const QString filename = m_data.GetFilename( diffusionPropertyIndex );
    QString fileName =  QFileInfo( QFile( filename ) ).fileName();
    if( !fileName.isEmpty() )
    {
        fileInformation.append( tr( qPrintable( "Filename: <i>" + fileName + "</i><br>" ) ) );
        fileInformation.append( tr( qPrintable( "Number of subjects: <i>" + QString::number( m_data.GetNbrSubjects( diffusionPropertyIndex ) ) + "</i><br>" ) ) );
        fileInformation.append( tr( qPrintable( "Data matrix: <i>" + QString::number(  m_data.GetNbrRows( diffusionPropertyIndex ) ) + "x" + QString::number( m_data.GetNbrColumns( diffusionPropertyIndex ) )  + "</i><br>" ) ) );
        if( diffusionPropertyIndex == m_data.GetSubMatrixIndex() )
        {
            fileInformation.append( tr( qPrintable( "Number of covariates: <i>" + QString::number( m_data.GetCovariates().size()-1 )  + "</i>" ) ) );
        }
    }
    else
    {
        fileInformation.append( tr ( "<i>No File Information.<br>Please select a correct data file</i>" ) );
    }

    return fileInformation;
}

void FADTTSWindow::DisplayFileInformation()
{
    foreach( int diffusionPropertyIndex, m_data.GetDiffusionPropertiesIndices() )
    {
        QString info = tr( qPrintable( GetInputFileInformation( diffusionPropertyIndex ) ) );
        m_inputFileInformationLabelMap.value( diffusionPropertyIndex )->setText( info );

        if( diffusionPropertyIndex == m_data.GetSubMatrixIndex() )
        {
            SetCovariateListVisible( info.contains( "No File Information", Qt::CaseInsensitive )  ? false : true );
        }
    }
}

void FADTTSWindow::DisplayInputLineEditIcon( int diffusionPropertyIndex, const QPixmap& icon )
{
    DisplayIcon( m_inputTabIconLabelMap[ diffusionPropertyIndex ], icon );
}

void FADTTSWindow::UpdateInputFileInformation( int diffusionPropertyIndex )
{
    QList< QStringList > fileData = m_data.GetFileData( diffusionPropertyIndex );

    if( !fileData.isEmpty() )
    {
        m_data.SetNbrRows( diffusionPropertyIndex ) = fileData.count();
        m_data.SetNbrColumns( diffusionPropertyIndex ) = fileData.first().count();

        m_data.ClearSubjects( diffusionPropertyIndex );
        QStringList subjects = m_processing.GetSubjectsFromData( fileData, m_data.GetSubjectColumnID() );
        m_data.SetSubjects( diffusionPropertyIndex ) = subjects;
        m_data.SetNbrSubjects( diffusionPropertyIndex ) = subjects.count();

        if( diffusionPropertyIndex == m_data.GetSubMatrixIndex() )
        {
            m_data.ClearCovariates();
            m_data.SetCovariates() = m_processing.GetCovariatesFromData( fileData, m_data.GetSubjectColumnID() );
            /** Intercept representes everything that has not been classified in one of the previous
             *  covariates. It is important to add it as 1st element of m_covariatesList **/
            m_data.AddInterceptToCovariates();

            SetCovariateListWidget();
        }
    }

    UpdateAvailableDiffusionProperties( diffusionPropertyIndex );
    DisplayFileInformation();
}

void FADTTSWindow::UpdateLineEditsAfterAddingMultipleFiles( const QStringList& fileList )
{
    /** This function only works with filename that start with ad_, rd_, md_, fa_ and subMatrix_.
     *  If a prefix is detected more than once, the reated files will be ignored. **/
    QMap< int, QStringList > fileMap;
    foreach ( int diffusionPropertyIndex, m_data.GetDiffusionPropertiesIndices() )
    {
        foreach( QString file, fileList )
        {
            QString filename = QFileInfo( QFile( file ) ).fileName();
            if( filename.contains( "_" + m_data.GetDiffusionPropertyName( diffusionPropertyIndex ) + "_", Qt::CaseInsensitive ) ||
                    filename.startsWith( m_data.GetDiffusionPropertyName( diffusionPropertyIndex ) + "_", Qt::CaseInsensitive ) ||
                    filename.endsWith( "_" + m_data.GetDiffusionPropertyName( diffusionPropertyIndex ) + ".csv", Qt::CaseInsensitive ) )
            {
                ( fileMap[ diffusionPropertyIndex ] ).append( file );
            }
        }
    }

    foreach ( int diffusionPropertyIndex, m_data.GetDiffusionPropertiesIndices() )
    {
        m_inputTabInputFileLineEditMap[ diffusionPropertyIndex ]->clear();
    }

    QMap< int, QStringList >::ConstIterator iter = fileMap.cbegin();
    while( iter != fileMap.constEnd() )
    {
        int diffusionPropertyIndex = iter.key();
        m_inputTabInputFileLineEditMap[ diffusionPropertyIndex ]->setText( iter.value().size() == 1 ? iter.value().first() : "" );
        ++iter;
    }
}


void FADTTSWindow::SetInfoSubjectColumnID()
{
    this->inputTab_subjectColumnID_label->setText( !m_data.GetFileData( m_data.GetSubMatrixIndex() ).isEmpty() ?
                                                       tr( qPrintable( "<b><i><span style=""font-size:7pt;"">" + QString::number( m_data.GetSubjectColumnID() + 1 ) + "</span></i></b>" ) ) :
                                                       "" );
}


void  FADTTSWindow::LaunchEditInputDialog( int diffusionPropertyIndex )
{
    m_editInputDialog->setModal( true );
    m_editInputDialog->setWindowTitle( tr( qPrintable( "Edit " + m_data.GetDiffusionPropertyName( diffusionPropertyIndex).toUpper() + " File" ) ) );
    m_editInputDialog->DisplayDataEdition( diffusionPropertyIndex );
    m_editInputDialog->exec();
}


void FADTTSWindow::SetCheckStateAllCovariates( Qt::CheckState checkState )
{
    for( int i = 0; i < m_covariateListWidget->count(); i++ )
    {
        QListWidgetItem *currentItem = m_covariateListWidget->item( i );
        currentItem->setCheckState( ( checkState == Qt::Unchecked ) && ( currentItem->text() == "Intercept" )
                                    ? Qt::Checked : checkState );
    }
}


/****************************************************************/
/******************** Subjects/Covariates tab *******************/
/****************************************************************/

/***********************  Private slots  ************************/
void FADTTSWindow::OnApplyQCThreshold()
{
    QStringList matchedSubjects = GetCheckedMatchedSubjects();
    QMap< QString, QList< QStringList > > rawData = GetPropertyRawData();

    bool initDiaolog = m_qcThresholdDialog->InitPlot( rawData, matchedSubjects, this->para_subjectTab_qcThreshold_doubleSpinBox->value() );

    if( initDiaolog )
    {
        m_qcThresholdDialog->setModal( true );
        m_qcThresholdDialog->setWindowTitle( tr( "Apply QC Threshold" ) );
        m_qcThresholdDialog->exec();
    }
}

void FADTTSWindow::OnApplyingQCThreshold( const QStringList& subjectsCorrelated, const QStringList& subjectsNotCorrelated, double qcThreshold )
{
    m_failedQCThresholdSubjects.clear();
    m_failedQCThresholdSubjects = subjectsNotCorrelated;

    para_subjectTab_qcThreshold_doubleSpinBox->setValue( qcThreshold );
    m_qcThreshold = qcThreshold;

    for( int i = 0; i < m_matchedSubjectListWidget->count(); i++ )
    {
        QListWidgetItem *currentItem = m_matchedSubjectListWidget->item( i );
        if( m_failedQCThresholdSubjects.contains( currentItem->text() ) )
        {
            currentItem->setCheckState( Qt::Unchecked );
            currentItem->setBackgroundColor( m_grey );
        }

        if( subjectsCorrelated.contains( currentItem->text() ) )
        {
            currentItem->setCheckState( Qt::Checked );
            currentItem->setBackgroundColor( m_green );
        }
    }

    DisplayNbrSubjectSelected();
}


void FADTTSWindow::OnSearch()
{
    int nbrSubjectFind = SearchSubjects( m_matchedSubjectListWidget ) + SearchSubjects( m_unmatchedSubjectListWidget );
    this->subjectTab_nbrFound_label->
            setText( !this->subjectTab_search_lineEdit->text().isEmpty() ? "found " + QString::number( nbrSubjectFind ) : "" );
}

void FADTTSWindow::OnInputToggled()
{
    SetSelectedInputFiles();
    UpdateSubjectList();
}

void FADTTSWindow::OnSetCaseSensitivityToggled( bool checked )
{
    m_caseSensitivity = checked ? Qt::CaseSensitive : Qt::CaseInsensitive;
    OnSearch();
}


void FADTTSWindow::OnSubjectClicked( QListWidgetItem *item )
{
    if( item->flags() == Qt::ItemIsEnabled )
    {
        item->setCheckState( item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked );
        item->setBackgroundColor( item->checkState() == Qt::Checked ? m_green : m_grey );

        if( ( item->checkState() == Qt::Checked ) && m_failedQCThresholdSubjects.contains( item->text() ) )
        {
            m_failedQCThresholdSubjects.removeAll( item->text() );
        }
    }

    DisplayNbrSubjectSelected();
}

void FADTTSWindow::OnCheckAllVisibleSubjects()
{
    SetCheckStateAllVisibleSubjects( Qt::Checked );
}

void FADTTSWindow::OnUnCheckAllVisibleSubjects()
{
    SetCheckStateAllVisibleSubjects( Qt::Unchecked );
}

void FADTTSWindow::OnSaveCheckedSubjects()
{
    QString filePath = QFileDialog::getSaveFileName( this, tr( "Save subject list as ..." ), m_currentSubjectFileDir + "/newSubjectList.txt", tr( ".txt ( *.txt ) ;; .*( * )" ) );
    if( !filePath.isEmpty() )
    {
        SaveCheckedSubjects( filePath );
    }
}


void FADTTSWindow::OnLoadSubjectList()
{
    QLineEdit *lineEdit = m_subjectFileLineEdit;
    QString filePath = lineEdit->text();
    QDir dir = SetDir( filePath, m_currentSubjectFileDir );
    QString file = QFileDialog::getOpenFileName( this, tr( "Choose SubjectList File" ), dir.absolutePath(), tr( ".txt ( *.txt ) ;; .*( * )" ) );
    if( !file.isEmpty() )
    {
        lineEdit->setText( file );
    }
}

void FADTTSWindow::OnResetSubjectList()
{
    m_subjectFileLineEdit->clear();
}

void FADTTSWindow::OnSettingSubjectList( const QString& filePath )
{
    QLabel *label = this->subjectTab_iconLoadSubjectFile_label;
    m_loadedSubjects.clear();

    if( filePath.isEmpty() )
    {
        label->clear();
    }
    else
    {
        m_loadedSubjects = m_processing.GetSubjectsFromFileList( filePath );
        DisplayIcon( label, m_loadedSubjects.isEmpty() ? m_koPixmap : m_okPixmap );
        UpdateCurrentDir( filePath, m_currentSubjectFileDir );
    }
    m_areSubjectsLoaded = m_loadedSubjects.isEmpty() ? false : true;

    UpdateSubjectList();
}


/*********************** Private function ***********************/
void FADTTSWindow::SetSelectedInputFiles()
{
    m_propertySelected.clear();
    m_selectedFiles.clear();
    foreach ( int diffusionPropertyIndex, m_data.GetDiffusionPropertiesIndices() )
    {
        if( m_paramTabFileCheckBoxMap[ diffusionPropertyIndex ]->isChecked() && !m_data.GetFilename( diffusionPropertyIndex ).isEmpty() )
        {
            m_propertySelected.insert( diffusionPropertyIndex, m_data.GetDiffusionPropertyName( diffusionPropertyIndex ) );
            m_selectedFiles.insert( diffusionPropertyIndex, m_data.GetFilename( diffusionPropertyIndex ) );
        }
    }
}

void FADTTSWindow::UpdateAvailableDiffusionProperties( int diffusionPropertyIndex )
{
//    QLabel *currentLabel = m_paramTabFileDataSizeLabelMap.value( diffusionPropertyIndex );
    QCheckBox *currentCheckBox = m_paramTabFileCheckBoxMap.value( diffusionPropertyIndex );
    int nbrRows = m_data.GetNbrSubjects( diffusionPropertyIndex );
    int nbrColumns = m_data.GetNbrColumns( diffusionPropertyIndex );
    int nbrSubjects = m_data.GetNbrSubjects( diffusionPropertyIndex );

    bool isDefine = !( ( nbrRows == 0 ) | ( nbrColumns == 0 ) | ( nbrSubjects == 0 ) );
//    QString text = isDefine ? tr( qPrintable( QString::number( m_data.GetNbrSubjects( diffusionPropertyIndex ) ) + " subjects" ) ) :
//                              tr( "N/A" );
//    currentLabel->setEnabled( isDefine );
//    currentLabel->setText( text );
    currentCheckBox->setEnabled( isDefine );
    currentCheckBox->setChecked( isDefine );
}

void FADTTSWindow::InitAvailableDiffusionProperties()
{
    foreach( int index, m_data.GetDiffusionPropertiesIndices() )
    {
        UpdateAvailableDiffusionProperties( index );
    }
}

QMap< int, bool > FADTTSWindow::GetDiffusionPropertiesCheckState()
{
    QMap< int, bool > diffusionPropertyCheckStatus;

    QMap< int, QCheckBox* >::ConstIterator iterCheckBoxMap = m_paramTabFileCheckBoxMap.cbegin();
    while( iterCheckBoxMap != m_paramTabFileCheckBoxMap.cend() )
    {
        diffusionPropertyCheckStatus.insert( iterCheckBoxMap.key(), iterCheckBoxMap.value()->isChecked() );
        ++iterCheckBoxMap;
    }

    return diffusionPropertyCheckStatus;
}


void FADTTSWindow::DisplaySubjectInformation()
{
    this->subjectTab_matchedSubjectsInformation_label->clear();
    this->subjectTab_unmatchedSubjectsInformation_label->clear();

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
                textUnmatchedSubjectsInformation.clear();
            }
            else
            {
                textMatchedSubjectsInformation = tr( qPrintable( QString::number( nbrMatchedSubjects ) + "/" + QString::number( nbrTotalSubjects ) + " matched" ) );
                textUnmatchedSubjectsInformation = tr( qPrintable( QString::number( nbrUnmatchedSubjects ) + "/" + QString::number( nbrTotalSubjects ) + " unmatched" ) );
            }
        }
        this->subjectTab_matchedSubjectsInformation_label->setText( textMatchedSubjectsInformation );
        this->subjectTab_unmatchedSubjectsInformation_label->setText( textUnmatchedSubjectsInformation );
    }
    else
    {
        this->subjectTab_matchedSubjectsInformation_label->clear();
        this->subjectTab_unmatchedSubjectsInformation_label->clear();
    }
}

void FADTTSWindow::DisplayNbrSubjectSelected()
{
    this->subjectTab_checkedSubjectsInformation_label->clear();
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

        this->subjectTab_checkedSubjectsInformation_label->setText( tr( qPrintable( QString::number( nbrSubjectsSelected ) + " subjects selected") ) );
    }
}

void FADTTSWindow::DisplaySortedSubjects( const QStringList& matchedSubjects, const QMap< QString, QList< int > >& unMatchedSubjectMap )
{
    m_failedQCThresholdSubjects.clear();

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
    QMap< QString, QList< int > >::ConstIterator iterUnmatchedSubjects = unMatchedSubjectMap.cbegin();
    while( iterUnmatchedSubjects != unMatchedSubjectMap.constEnd() )
    {
        QStringList sortedText;
        foreach( int index, iterUnmatchedSubjects.value() )
        {
            if( index == -1 )
            {
                sortedText.append( "loaded subjects" );
            }
            else
            {
                sortedText.append( m_data.GetDiffusionPropertyName( index ) );
            }
        }

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

void FADTTSWindow::UpdateSubjectList()
{
    QMap< int, bool > diffusionPropertiesCheckState = GetDiffusionPropertiesCheckState();
    QMap< int, QStringList > allSubjects = m_processing.GetSubjectsFromSelectedFiles( diffusionPropertiesCheckState, m_data.GetSubjects() );
    if( m_areSubjectsLoaded )
    {
        allSubjects.insert( -1, m_loadedSubjects );
    }

    QStringList allSubjectsList = m_processing.GetAllSubjects( allSubjects );
    if( m_areSubjectsLoaded )
    {
        allSubjectsList.append( m_loadedSubjects );
        allSubjectsList.removeDuplicates();
    }

    QMap< QString, QMap< int, bool > > sortedSubjects = m_processing.SortSubjects( allSubjectsList, allSubjects );

    QStringList matchedSubjects;
    QMap< QString, QList< int > > unMatchedSubjects;
    m_processing.AssignSortedSubject( sortedSubjects, matchedSubjects, unMatchedSubjects );

    DisplaySortedSubjects( matchedSubjects, unMatchedSubjects );

    OnSearch();
}

void FADTTSWindow::SetCheckStateAllVisibleSubjects( Qt::CheckState checkState )
{
    for( int i = 0; i < m_matchedSubjectListWidget->count(); i++ )
    {
        QListWidgetItem* current = m_matchedSubjectListWidget->item( i );
        if( !current->isHidden() && current->flags() == Qt::ItemIsEnabled )
        {
            current->setCheckState( checkState );
            current->setBackgroundColor( checkState ? m_green : m_grey );

            if( ( current->checkState() == Qt::Checked ) && m_failedQCThresholdSubjects.contains( current->text() ) )
            {
                m_failedQCThresholdSubjects.removeAll( current->text() );
            }
        }
    }

    DisplayNbrSubjectSelected();
}

int FADTTSWindow::SearchSubjects( QListWidget *list )
{
    int nbrFound = 0;
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
            regExp.setCaseSensitivity( m_caseSensitivity );
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

void FADTTSWindow::SaveCheckedSubjects( QString filePath )
{
    QFile exportedTXT( filePath );
    exportedTXT.open( QIODevice::WriteOnly );
    QTextStream ts( &exportedTXT );

    for( int i = 0; i < m_matchedSubjectListWidget->count(); i++ )
    {
        if( m_matchedSubjectListWidget->item( i )->checkState() == Qt::Checked )
        {
            ts << m_matchedSubjectListWidget->item( i )->text() <<  endl;
        }
    }
    exportedTXT.flush();
    exportedTXT.close();

    QString previousFilePath = m_subjectFileLineEdit->text();
    m_subjectFileLineEdit->setText( filePath );
    if( previousFilePath == filePath )
    {
        /** If filePath does not change, an update is needed to display the right subject list **/
        UpdateSubjectList();
    }
}


QStringList FADTTSWindow::GetCheckedMatchedSubjects()
{
    QStringList matchedSubjects;
    QList< QListWidgetItem * > items = m_matchedSubjectListWidget->findItems( "*", Qt::MatchWrap | Qt::MatchWildcard );
    foreach( QListWidgetItem *currentItem, items )
    {
        if( currentItem->checkState() == Qt::Checked )
        {
            matchedSubjects.append( currentItem->text() );
        }
    }

    return matchedSubjects;
}

QMap< QString, QList< QStringList > > FADTTSWindow::GetPropertyRawData()
{
    QMap< QString, QList< QStringList > > fileData;
    foreach( int diffusionPropertyIndex, m_propertySelected.keys() )
    {
        if( m_data.GetDiffusionPropertyName( diffusionPropertyIndex ) != "submatrix" )
        {
            fileData.insert( m_data.GetDiffusionPropertyName( diffusionPropertyIndex ).toUpper(), m_data.GetFileData( diffusionPropertyIndex ) );
        }
    }

    return fileData;
}


/****************************************************************/
/************************ Execution  tab ************************/
/****************************************************************/

/***********************  Private  slots  ***********************/
void FADTTSWindow::OnBrowsingOutputDir()
{
    QLineEdit *lineEdit = this->para_executionTab_outputDir_lineEdit;
    QString filePath = lineEdit->text();
    QDir dir = filePath;

    QString dirPath;
    dirPath = ( dir.exists() && !filePath.isEmpty() ) ?
                QFileDialog::getExistingDirectory( this, tr( "Choose Output Directory" ), dir.absolutePath(), QFileDialog::ShowDirsOnly ) :
                QFileDialog::getExistingDirectory( this, tr( "Choose Output Directory" ), m_data.GetOutputDir(), QFileDialog::ShowDirsOnly );
    if( !dirPath.isEmpty() )
    {
        lineEdit->setText( dirPath );
    }
}

void FADTTSWindow::OnSettingOutputDir( const QString& path )
{
    QLabel *label = this->executionTab_iconOutputDir_label;
    if( !path.isEmpty() )
    {
        DisplayIcon( label, QDir( path ).exists() ? m_okPixmap : m_koPixmap );
        if( QDir( path ).exists() )
        {
            m_data.SetOutputDir() = path;
        }
    }
    else
    {
        label->clear();
    }
}

void FADTTSWindow::OnBrowsingMVCMPath()
{
    QLineEdit *lineEdit = this->soft_executionTab_mvcm_lineEdit;
    QString filePath = lineEdit->text();
    QDir dir = filePath;

    QString dirPath = ( dir.exists() && !filePath.isEmpty() ) ?
                QFileDialog::getExistingDirectory( this, tr( "Choose MVCM Directory" ), dir.absolutePath(), QFileDialog::ShowDirsOnly ) :
                QFileDialog::getExistingDirectory( this, tr( "Choose MVCM Directory" ), m_mvcmPath, QFileDialog::ShowDirsOnly );
    if( !dirPath.isEmpty() )
    {
        lineEdit->setText( dirPath );
    }
}

void FADTTSWindow::OnSettingMVCMPath( const QString& path )
{
    QLabel *label = this->executionTab_iconMVCM_label;
    if( !path.isEmpty() )
    {
        QStringList nameFilterMVCM( "*MVCM*" );
        bool mvcmFilesExist = !QStringList( QStringList() << QDir( path ).entryList( nameFilterMVCM ) ).isEmpty();
        DisplayIcon( label, mvcmFilesExist ? m_okPixmap : m_koPixmap );
        if( mvcmFilesExist )
        {
            m_mvcmPath = path;
        }
    }
    else
    {
        label->clear();
    }
}

void FADTTSWindow::OnBrowsingMatlabExe()
{
    QLineEdit *lineEdit = this->soft_executionTab_runMatlab_lineEdit;
    QString filePath = lineEdit->text();
    QString file;
    QDir dir;

    dir = ( !filePath.isEmpty() && QDir( QFileInfo( QFile( filePath ) ).absolutePath() ).exists() ) ?
                QFileInfo( QFile( filePath ) ).absolutePath() : m_currentMatlabExeDir;

    file = QFileDialog::getOpenFileName( this, tr( "Choose Matlab Executable" ), dir.absolutePath() );
    if( !file.isEmpty() )
    {
        lineEdit->setText( file );
    }
}

void FADTTSWindow::OnSettingMatlabExe( const QString& executable )
{
    QFile matlabExe( executable );
    QLabel *label = this->executionTab_iconMatlab_label;
    if( executable.isEmpty() )
    {
        label->clear();
        m_isMatlabExeFound = false;
    }
    else
    {
        if( matlabExe.open( QIODevice::ReadOnly ) )
        {
            matlabExe.close();
            DisplayIcon( label, m_okPixmap );
            m_matlabThread->SetMatlabExe() = executable;
            m_isMatlabExeFound = true;
        }
        else
        {
            DisplayIcon( label, m_koPixmap );
            m_isMatlabExeFound = false;
        }

        UpdateCurrentDir( executable, m_currentMatlabExeDir );
    }
}


void FADTTSWindow::OnRunMatlabToggled( bool choice )
{
    this->executionTab_runMatlab_pushButton->setEnabled( choice );
    this->soft_executionTab_runMatlab_lineEdit->setEnabled( choice );
    this->executionTab_iconMatlab_label->setEnabled( choice );
    this->executionTab_nbrCompThreads_label->setEnabled( choice );
    this->soft_executionTab_nbrCompThreads_spinBox->setEnabled( choice );

    m_matlabThread->SetRunMatlab() = choice;
}


void FADTTSWindow::OnRun()
{ 
    SyncUiToModelStructure();

    SetSelectedCovariates();
    SetFibername();

    if( IsFADTTSReadyToBeRun() )
    {
        this->executionTab_run_pushButton->setEnabled( false );
        this->executionTab_stop_pushButton->setEnabled( true );
        m_progressBar->show();

        SetMatlabScript();

        m_matlabThread->start();

        *m_textStreamLog << endl << "File generation completed..." << endl;
    }
}

void FADTTSWindow::OnStop()
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
            *m_textStreamLog << "Warning! Thread terminated by user before completed matlab script." << endl;
            m_matlabThread->terminate();
            m_logWindow->insertPlainText( "Warning! Thread terminated by user before completed matlab script.\n");
            break;
        }
        default:
            break;
        }
    }
}

void FADTTSWindow::OnDisplayLog()
{
    QScrollBar *scrollBar = m_logWindow->verticalScrollBar();

    QString line = m_textStreamLog->readAll();
    m_logWindow->insertPlainText( line );

    scrollBar->setValue( scrollBar->maximum() );
    //    if( scrollBar->value() == scrollBar->maximum() )
    //    {
    //        m_logWindow->insertPlainText( line );
    //        scrollBar->setValue( scrollBar->maximum() );
    //    }
    //    else
    //    {
    //        m_logWindow->insertPlainText( line );
    //    }
}

void FADTTSWindow::OnClearLog()
{
    m_logWindow->clear();
}


void FADTTSWindow::OnMatlabThreadFinished()
{
    this->executionTab_run_pushButton->setEnabled( true );
    this->executionTab_stop_pushButton->setEnabled( false );
    m_logFile->flush();
    m_logFile->close();
    m_progressBar->hide();
}


/*********************** Private function ***********************/
void FADTTSWindow::GenerateFailedQCThresholdSubjectFile( QString outputDir )
{
    QFile failedQCThresholdSubjectFile( outputDir + "/" + this->para_executionTab_fiberName_lineEdit->text() + "_failed_QCThreshold_SubjectList.txt" );

    if( failedQCThresholdSubjectFile.open( QIODevice::WriteOnly ) )
    {
        QTextStream tsFailedQCThresholdSubjectFile( &failedQCThresholdSubjectFile );

        tsFailedQCThresholdSubjectFile << "After failing the QC Threshold (" << QString::number( m_qcThreshold ) << ") the following subjects were removed from the study:" << endl;

        for( int i = 0; i < m_failedQCThresholdSubjects.size(); i++ )
        {
            tsFailedQCThresholdSubjectFile << "- " << m_failedQCThresholdSubjects.at( i ) << endl;
        }
        failedQCThresholdSubjectFile.flush();
        failedQCThresholdSubjectFile.close();
    }
}

QStringList FADTTSWindow::GenerateSelectedSubjectFile( QString outputDir )
{
    QStringList selectedSubjects;
    QFile selectedSubjectFile( outputDir + "/" + this->para_executionTab_fiberName_lineEdit->text() + "_subjectList.txt" );
    m_nbrSelectedSubjects = 0;

    if( selectedSubjectFile.open( QIODevice::WriteOnly ) )
    {
        QTextStream tsSelectedSubjectFile( &selectedSubjectFile );
        for( int i = 0; i < m_matchedSubjectListWidget->count(); i++ )
        {
            if( m_matchedSubjectListWidget->item( i )->checkState() == Qt::Checked )
            {
                m_nbrSelectedSubjects++;
                tsSelectedSubjectFile << m_matchedSubjectListWidget->item( i )->text() << endl;
                selectedSubjects.append( m_matchedSubjectListWidget->item( i )->text() );
            }
        }
        selectedSubjectFile.flush();
        selectedSubjectFile.close();
    }

    if( !m_failedQCThresholdSubjects.isEmpty() )
    {
        GenerateFailedQCThresholdSubjectFile( outputDir );
    }

    return selectedSubjects;
}


void FADTTSWindow::SetSelectedCovariates()
{
    m_selectedCovariates.clear();

    if( m_paramTabFileCheckBoxMap.value( m_data.GetSubMatrixIndex() )->isChecked() )
    {
        for( int i = 0; i < m_covariateListWidget->count(); i++ )
        {
            QListWidgetItem *currentItem = m_covariateListWidget->item( i );
            if( currentItem->checkState() == Qt::Checked )
            {
                if( currentItem->text() == "Intercept" )
                {
                    m_selectedCovariates.insert( - 1, currentItem->text() );
                }
                else
                {
                    m_selectedCovariates.insert( m_data.GetCovariates().key( currentItem->text() ), currentItem->text() );
                }
            }
        }
    }
}

void FADTTSWindow::SetFibername()
{
    m_fibername = this->para_executionTab_fiberName_lineEdit->text();
}

bool FADTTSWindow::IsFADTTSReadyToBeRun()
{
    bool atLeastOneDiffusionPropertyEnabled = ( m_paramTabFileCheckBoxMap.value( m_data.GetAxialDiffusivityIndex() )->isEnabled() ||
                                                m_paramTabFileCheckBoxMap.value( m_data.GetRadialDiffusivityIndex() )->isEnabled() ||
                                                m_paramTabFileCheckBoxMap.value( m_data.GetMeanDiffusivityIndex() )->isEnabled() ||
                                                m_paramTabFileCheckBoxMap.value( m_data.GetFractionalAnisotropyIndex() )->isEnabled() );
    bool subMatrixEnabled = m_paramTabFileCheckBoxMap.value( m_data.GetSubMatrixIndex() )->isEnabled();

    bool atLeastOneDiffusionPropertyChecked = ( m_paramTabFileCheckBoxMap.value( m_data.GetAxialDiffusivityIndex() )->isChecked() ||
                                                m_paramTabFileCheckBoxMap.value( m_data.GetMeanDiffusivityIndex() )->isChecked() ||
                                                m_paramTabFileCheckBoxMap.value( m_data.GetRadialDiffusivityIndex() )->isChecked() ||
                                                m_paramTabFileCheckBoxMap.value( m_data.GetFractionalAnisotropyIndex() )->isChecked() );
    bool subMatrixChecked = m_paramTabFileCheckBoxMap.value( m_data.GetSubMatrixIndex() )->isChecked();

    bool atLeastOneCovariateChecked = m_selectedCovariates.count() != 0;

    bool fiberNameProvided = !m_fibername.isEmpty();

    bool mvcmPathSpecified = !this->soft_executionTab_mvcm_lineEdit->text().isEmpty();
    bool matlabExeSpecified = !this->para_executionTab_runMatlab_checkBox->isChecked() ? true : m_isMatlabExeFound;

    if( !atLeastOneDiffusionPropertyEnabled || !subMatrixEnabled ||
            !atLeastOneDiffusionPropertyChecked || !subMatrixChecked ||
            !atLeastOneCovariateChecked ||
            !fiberNameProvided ||
            !mvcmPathSpecified || !matlabExeSpecified )
    {
        QString warningText = "<b>FADTTSter will not be executed for the following reason(s):</b><br>";
        if( !atLeastOneDiffusionPropertyEnabled || !subMatrixEnabled )
        {
            warningText.append( "Inputs Tab<br>" );
            if( !atLeastOneDiffusionPropertyEnabled )
            {
                warningText.append( "- Provide at least 1 data file (AD, RD, MR or FA)<br>" );
            }
            if( !subMatrixEnabled )
            {
                warningText.append( "- No covariate file provided<br>" );
            }
        }
        if( atLeastOneDiffusionPropertyEnabled || subMatrixEnabled )
        {
            if( ( !atLeastOneDiffusionPropertyChecked && atLeastOneDiffusionPropertyEnabled ) || ( !subMatrixChecked && subMatrixEnabled ) || !atLeastOneCovariateChecked )
            {
                warningText.append( "Subjects / Covariates Tab<br>" );
                if( !atLeastOneDiffusionPropertyChecked && atLeastOneDiffusionPropertyEnabled )
                {
                    warningText.append( "- Select at least 1 data file (AD, RD, MR or FA)<br>" );
                }
                if( !subMatrixChecked && subMatrixEnabled )
                {
                    warningText.append( "- Covariate file not selected<br>" );
                }
                if( subMatrixChecked )
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

void FADTTSWindow::SetLogDisplay( QString outputDir, const QMap< int, QString >& matlabInputFiles, const QMap< int, QString >& selectedCovariates )
{
    m_logWindow->clear();

    // Log File
    m_logFile = new::QFile( outputDir + "/" + m_fibername + ".log" );
    m_logFile->open( QIODevice::ReadWrite );
    m_matlabThread->SetLogFile( m_logFile );
    m_textStreamLog = new QTextStream( m_logFile );

    // QFileSystemWatcher
    QFileSystemWatcher* log_watcher = new QFileSystemWatcher( this );
    log_watcher->addPath( m_logFile->fileName() );
    connect( log_watcher, SIGNAL( fileChanged( QString ) ), this, SLOT( OnDisplayLog() ) );

    // Init log file
    *m_textStreamLog << QDate::currentDate().toString( "MM/dd/yyyy" ) <<
                        " " << QTime::currentTime().toString( "hh:mmap" ) << endl;
    *m_textStreamLog << "FADTTSter: " << QString( FADTTS_VERSION ).prepend( "V" ) << endl;
    *m_textStreamLog << "/****************************************************************/" << endl;
    *m_textStreamLog << "/********************** FADTTSter LOG FILE **********************/" << endl;
    *m_textStreamLog << "/****************************************************************/" << endl << endl;

    *m_textStreamLog << "/**********************       Inputs       **********************/" << endl;
    *m_textStreamLog << "- input files: ";
    QStringList files;
    QMap< int, QString >::ConstIterator iterMatlabInputFiles = matlabInputFiles.cbegin();
    while( iterMatlabInputFiles != matlabInputFiles.cend() )
    {
        files.append( QFileInfo( QFile( iterMatlabInputFiles.value() ) ).fileName() );
        ++iterMatlabInputFiles;
    }
    *m_textStreamLog << files.join( ", " ) << endl;
    *m_textStreamLog << "- nbr covariates: " << QString::number( selectedCovariates.size() ) << endl;
    *m_textStreamLog << "- covariates: ";
    QStringList covariates;
    QMap< int, QString >::ConstIterator iterCovariates = selectedCovariates.cbegin();
    while( iterCovariates != selectedCovariates.cend() )
    {
        covariates.append( iterCovariates.value() );
        ++iterCovariates;
    }
    *m_textStreamLog << covariates.join( ", " ) << endl;
    if( !m_loadedSubjects.isEmpty() )
    {
        *m_textStreamLog << "- subject file: " << m_subjectFileLineEdit->text() << endl;
    }
    *m_textStreamLog << "- nbr subjects: " << QString::number( m_nbrSelectedSubjects ) << endl;
    if( !m_failedQCThresholdSubjects.isEmpty() )
    {
        *m_textStreamLog << "/!\\ That number INCLUDE the " << QString::number( m_failedQCThresholdSubjects.size() ) << " subject(s) that was/were removed from the study after failing the QC Threshold set at " << QString::number( m_qcThreshold ) << endl << endl;
    }

    *m_textStreamLog << "/**********************      Settings      **********************/" << endl;
    *m_textStreamLog << "- fiber name: " << m_fibername << endl;
    *m_textStreamLog << "- nbr permutations: " << QString::number( this->para_executionTab_nbrPermutations_spinBox->value() ) << endl;
    *m_textStreamLog << "- confidence band threshold: " << QString::number( this->para_executionTab_confidenceBandsThreshold_doubleSpinBox->value() ) << endl;
    *m_textStreamLog << "- pvalue threshold: " << QString::number( this->para_executionTab_pvalueThreshold_doubleSpinBox->value() ) << endl;
    *m_textStreamLog << QString( this->para_executionTab_omnibus_checkBox->isChecked() ? "- omnibus: true" : "- omnibus: false" ) << endl;
    *m_textStreamLog << QString( this->para_executionTab_postHoc_checkBox->isChecked() ? "- post hoc: true" : "- post hoc: false" ) << endl << endl;

    *m_textStreamLog << "/**********************       Output       **********************/" << endl;
    *m_textStreamLog << "- output directory: " << outputDir << endl << endl;

    *m_textStreamLog << "/********************* Matlab Specification *********************/" << endl;
    *m_textStreamLog << "- FADTTS directory: " << this->soft_executionTab_mvcm_lineEdit->text() << endl << endl;
    if( this->para_executionTab_runMatlab_checkBox->isChecked() )
    {
        *m_textStreamLog << "Matlab will be run after file generation" << endl;
        *m_textStreamLog << "- matlab executable: " << this->soft_executionTab_runMatlab_lineEdit->text() << endl;
        *m_textStreamLog << "- nbr computational threads: " << soft_executionTab_nbrCompThreads_spinBox->value() << endl;
    }
    else
    {
        *m_textStreamLog << "/!\\ Matlab will not be run after file generation (user choice)" << endl;
    }
    *m_textStreamLog << endl << endl << endl;

    *m_textStreamLog << "/**********************       Process      **********************/" << endl;
    *m_textStreamLog << "Thread starts running..." << endl;

    m_logFile->flush();
}

void FADTTSWindow::SetMatlabScript()
{
    QString outputDir = m_data.GetOutputDir() + "/FADTTSter_" + m_fibername;
    QDir().mkpath( outputDir );

    QMap< int, QString > matlabInputFiles = m_processing.GenerateMatlabInputs( outputDir,m_fibername, m_selectedFiles, m_propertySelected, m_selectedCovariates,
                                                                               m_data.GetSubjectColumnID(), GenerateSelectedSubjectFile( outputDir ) );

    m_matlabThread->InitMatlabScript( outputDir, "FADTTSterAnalysis_" + m_fibername + "_" + QString::number( this->para_executionTab_nbrPermutations_spinBox->value() ) + "perm.m" );
    m_matlabThread->SetHeader();
    m_matlabThread->SetNbrCompThreads( this->para_executionTab_runMatlab_checkBox->isChecked(), this->soft_executionTab_nbrCompThreads_spinBox->value() );
    m_matlabThread->SetMVCMPath( this->soft_executionTab_mvcm_lineEdit->text() );
    m_matlabThread->SetFiberName( m_fibername );
    m_matlabThread->SetDiffusionProperties( m_propertySelected.values() );
    m_matlabThread->SetNbrPermutation( this->para_executionTab_nbrPermutations_spinBox->value() );
    m_matlabThread->SetCovariates( m_selectedCovariates );
    m_matlabThread->SetInputFiles( matlabInputFiles );
    m_matlabThread->SetOmnibus( this->para_executionTab_omnibus_checkBox->isChecked() );
    m_matlabThread->SetPostHoc( this->para_executionTab_postHoc_checkBox->isChecked() );
    m_matlabThread->SetConfidenceBandsThreshold( this->para_executionTab_confidenceBandsThreshold_doubleSpinBox->value() );
    m_matlabThread->SetPvalueThreshold( this->para_executionTab_pvalueThreshold_doubleSpinBox->value() );

    SetLogDisplay( outputDir, matlabInputFiles, m_selectedCovariates );
}


/****************************************************************/
/************************ Plotting  tab *************************/
/****************************************************************/

/***********************  Private  slots  ***********************/
void FADTTSWindow::OnBrowsingPlotDir()
{
    QLineEdit *lineEdit = this->para_plottingTab_loadSetDataTab_browsePlotDirectory_lineEdit;
    QString filePath = lineEdit->text();
    QDir dir = filePath;

    QString dirPath;
    dirPath = QFileDialog::getExistingDirectory( this, tr( "Choose Plot Directory" ), ( dir.exists() && !filePath.isEmpty() ) ? dir.absolutePath() : m_data.GetOutputDir(), QFileDialog::ShowDirsOnly );
    if( !dirPath.isEmpty() )
    {
        lineEdit->setText( dirPath );
    }
}

void FADTTSWindow::OnSettingPlotDir( const QString& path )
{
    QLabel *browsePlotDirIcon = this->plottingTab_loadSetDataTab_browsePlotDirectoryIcon_label;
    QLabel *plotDirInfo = this->plottingTab_loadSetDataTab_plotDirectoryInfo_label;
    QLabel *plotDirIcon = this->plottingTab_loadSetDataTab_plotDirectoryIcon_label;
    QLabel *fibernameIcon = this->plottingTab_loadSetDataTab_fibernameIcon_label;

    if( !path.isEmpty() )
    {
        bool pathExists = QDir( path ).exists();

        DisplayIcon( browsePlotDirIcon, pathExists ? m_okPixmap : m_koPixmap );

        if( pathExists )
        {
            QString containingFolder = QDir( path ).dirName();
            if( containingFolder.contains( "FADTTSter_" ) )
            {
                this->para_plottingTab_loadSetDataTab_fibername_lineEdit->setText( containingFolder.remove( "FADTTSter_" ) );
            }
            else
            {
                this->para_plottingTab_loadSetDataTab_fibername_lineEdit->clear();
            }
        }
        else
        {
            plotDirInfo->hide();
            plotDirIcon->clear();
            fibernameIcon->clear();
        }
    }
    else
    {
        browsePlotDirIcon->clear();
        plotDirInfo->hide();
        plotDirIcon->clear();
        fibernameIcon->clear();
    }

    SetPlotTab();
}

void FADTTSWindow::OnSettingPlotFibername( const QString& fibername )
{
    QLabel *plotDirInfo = this->plottingTab_loadSetDataTab_plotDirectoryInfo_label;
    QLabel *fibernameIcon = this->plottingTab_loadSetDataTab_fibernameIcon_label;

    if( plotDirInfo->isHidden() )
    {
        fibernameIcon->clear();
    }
    else
    {
        if( m_isPlotReady )
        {
            QString fibernameNoBlank = QString( fibername ).remove( " ", Qt::CaseInsensitive );
            DisplayIcon( fibernameIcon, !fibernameNoBlank.isEmpty() ? m_okPixmap : m_warningPixmap );
            m_plot->SetFibername() = fibernameNoBlank;
        }
        else
        {
            fibernameIcon->clear();
        }
    }
}


void FADTTSWindow::OnSettingPlotsUsed( const QStringList& plotsAvailable )
{
    m_plotComboBox->clear();
    if( !plotsAvailable.isEmpty() )
    {
        m_plotComboBox->addItem( "No Plot" );
        m_plotComboBox->addItems( plotsAvailable );
    }
}

void FADTTSWindow::OnSettingAllPropertiesUsed( const QMap< int, QString >& allPropertiesUsed )
{
    if( allPropertiesUsed.isEmpty() || allPropertiesUsed != m_previousPropertiesUsed )
    {
        SetPropertyEdition( allPropertiesUsed.values() );

        QString currentProperty = m_propertyComboBox->currentText();
        m_propertyComboBox->clear();
        m_propertyComboBox->addItem( "" );
        m_propertyComboBox->addItems( allPropertiesUsed.values() );
        m_propertyComboBox->setCurrentText( currentProperty );

        SetPropertiesForDisplay( allPropertiesUsed.values() );

        m_previousPropertiesUsed = allPropertiesUsed;
    }

}

void FADTTSWindow::OnSettingAllCovariatesUsed( const QMap< int, QString >& allCovariatesUsed )
{
    if( allCovariatesUsed.isEmpty() || allCovariatesUsed != m_previousCovariatesUsed )
    {
        SetCovariateEdition( allCovariatesUsed );
        SetCovariatesForDisplay( allCovariatesUsed );

        m_previousCovariatesUsed = allCovariatesUsed;
    }

    if( allCovariatesUsed == m_previousCovariatesUsed )
    {
        EditCovariatesNames();
    }
}


void FADTTSWindow::OnUpdatingCovariatesAvailable( const QMap< int, QString >& covariateAvailable )
{
    QString currentCovariate = m_covariateComboBox->currentText();
    m_covariateComboBox->clear();
    m_covariateComboBox->addItem( "" );
    QMap< int, QString >::ConstIterator iterCovariateAvailable = covariateAvailable.cbegin();
    while( iterCovariateAvailable != covariateAvailable.cend() )
    {
        m_covariateComboBox->addItem( iterCovariateAvailable.value() );
        ++iterCovariateAvailable;
    }
    m_covariateComboBox->setCurrentText( currentCovariate );
}

void FADTTSWindow::OnUpdatingPropertyPlotColor( const QString& property )
{
    displayMapType::Iterator currentIterator = m_propertiesForDisplay.begin();
    bool iterFound = false;
    while( !iterFound && currentIterator != m_propertiesForDisplay.end() )
    {
        if( currentIterator.value().first == property )
        {
            iterFound = true;
            currentIterator.value().second.second = m_propertiesColorsComboBoxMap.value( currentIterator.key() ).second->currentText();
        }
        else
        {
            ++currentIterator;
        }
    }
    m_currentLinesForDisplay = m_areLinesForDisplayProperties ? m_propertiesForDisplay : m_covariatesForDisplay;
}

void FADTTSWindow::OnUpdatingCovariatePlotColor( const QString& covariate )
{
    displayMapType::Iterator currentIterator = m_covariatesForDisplay.begin();
    bool iterFound = false;
    while( !iterFound && currentIterator != m_covariatesForDisplay.end() )
    {
        if( currentIterator.value().first == covariate )
        {
            iterFound = true;
            currentIterator.value().second.second = m_covariatesColorsComboBoxMap.value( currentIterator.key() ).second->currentText();
        }
        else
        {
            ++currentIterator;
        }
    }
    m_currentLinesForDisplay = m_areLinesForDisplayProperties ? m_propertiesForDisplay : m_covariatesForDisplay;
}


void FADTTSWindow::OnPlotSelection( const QString& plotSelected )
{
    m_plot->SetSelectedPlot( plotSelected );

    if( plotSelected == "No Plot" )
    {
        SetPlotOptions( false, false, false );
    }
    if( plotSelected == "Raw Data" || plotSelected == "Raw Stats" ||
            plotSelected == "Betas with Omnibus Confidence Bands" )
    {
        SetPlotOptions( true, true, true );
    }
    if( plotSelected == "Raw Betas by Properties" || plotSelected == "Omnibus FDR Significant Betas by Properties" ||
            plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        SetPlotOptions( true, true, false );
    }
    if( plotSelected == "Raw Betas by Covariates" || plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
            plotSelected == "Post-Hoc FDR Local pvalues by Covariates" || plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        SetPlotOptions( true, false, true );
    }
    if( plotSelected == "Omnibus Local pvalues" || plotSelected == "Omnibus FDR Local pvalues" )
    {
        SetPlotOptions( true, false, false );
    }

    m_plotSelected = plotSelected; // to remove later
}

void FADTTSWindow::OnPropertySelection( const QString& propertySelected )
{
    m_plot->SetSelectedProperty() = propertySelected;
}

void FADTTSWindow::OnCovariateSelection( const QString& covariateSelected )
{
    m_plot->SetSelectedCovariate() = covariateSelected;
}


void FADTTSWindow::OnLineForDisplayClicked( QListWidgetItem *item )
{
    if( item->flags() == Qt::ItemIsEnabled )
    {
        item->setCheckState( item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked );

        displayMapType::Iterator currentIterator = m_currentLinesForDisplay.begin();
        bool iterFound = false;
        while( !iterFound && ( currentIterator != m_currentLinesForDisplay.end() ) )
        {
            if( currentIterator.value().first == item->text() )
            {
                iterFound = true;
                currentIterator.value().second.first = item->checkState();
            }
            else
            {
                ++currentIterator;
            }
        }
    }

    if( m_areLinesForDisplayProperties )
    {
        m_propertiesForDisplay = m_currentLinesForDisplay;
    }
    else
    {
        m_covariatesForDisplay = m_currentLinesForDisplay;
    }

    m_plot->UpdateLineToDisplay( m_currentLinesForDisplay );
}

void FADTTSWindow::OnCheckAllLinesToDisplay()
{
    SetCheckStateLinesToDisplay( Qt::Checked );

    m_plot->UpdateLineToDisplay( m_currentLinesForDisplay );
}

void FADTTSWindow::OnUncheckAllToDisplay()
{
    SetCheckStateLinesToDisplay( Qt::Unchecked );

    m_plot->UpdateLineToDisplay( m_currentLinesForDisplay );
}


void FADTTSWindow::OnSettingLinesSelected( const QStringList& linesSelected )
{
    m_lineSelectedListWidget->clear();

    if( linesSelected.isEmpty() )
    {
        m_lineSelectedListWidget->hide();
    }
    else
    {
        QListWidgetItem *item = new QListWidgetItem( QString::number( linesSelected.size() ) + " line(s) selected:", m_lineSelectedListWidget );
        m_lineSelectedListWidget->addItem( item );

        for( int i = 1; i < linesSelected.size() + 1; i++ )
        {
            QListWidgetItem *currentLineSelectedItem = new QListWidgetItem( QString( "- " + linesSelected.at( i - 1 ) ), m_lineSelectedListWidget );
            m_lineSelectedListWidget->addItem( currentLineSelectedItem );
        }

        m_lineSelectedListWidget->setMaximumHeight( m_lineSelectedListWidget->sizeHintForRow( 0 ) * ( m_lineSelectedListWidget->count() + 1 ) );

        m_lineSelectedListWidget->show();
    }
}


void FADTTSWindow::OnUseCustomizedTitle( bool checkState )
{
    this->plottingTab_titleAxisLegendTab_titleName_lineEdit->setEnabled( checkState );
    this->plottingTab_titleAxisLegendTab_titleBold_checkBox->setEnabled( checkState );
    this->plottingTab_titleAxisLegendTab_titleItalic_checkBox->setEnabled( checkState );
    this->plottingTab_titleAxisLegendTab_titleSize_label->setEnabled( checkState );
    this->plottingTab_titleAxisLegendTab_titleSize_doubleSpinBox->setEnabled( checkState );
}

void FADTTSWindow::OnUpdatingPlotTitle()
{
    UpdatePlotTitle();
}


void FADTTSWindow::OnUpdatingPlotGrid( bool checkState )
{
    m_plot->UpdateGrid( checkState );
}

void FADTTSWindow::OnUseCustomizedAxis( bool checkState )
{
    this->plottingTab_titleAxisLegendTab_xName_label->setEnabled( checkState );
    this->plottingTab_titleAxisLegendTab_xName_lineEdit->setEnabled( checkState );
    this->plottingTab_titleAxisLegendTab_yName_label->setEnabled( checkState );
    this->plottingTab_titleAxisLegendTab_yName_lineEdit->setEnabled( checkState );
    this->plottingTab_titleAxisLegendTab_axisBold_checkBox->setEnabled( checkState );
    this->plottingTab_titleAxisLegendTab_axisItalic_checkBox->setEnabled( checkState );
    this->plottingTab_titleAxisLegendTab_yMin_checkBox->setEnabled( checkState );
    this->plottingTab_titleAxisLegendTab_yMin_doubleSpinBox->setEnabled( checkState && this->plottingTab_titleAxisLegendTab_yMin_checkBox->isChecked() );
    this->plottingTab_titleAxisLegendTab_yMax_checkBox->setEnabled( checkState );
    this->plottingTab_titleAxisLegendTab_yMax_doubleSpinBox->setEnabled( checkState && this->plottingTab_titleAxisLegendTab_yMax_checkBox->isChecked() );
}

void FADTTSWindow::OnYMinToggled( bool checkState )
{
    QDoubleSpinBox* yMinDoubleSpinBox = this->plottingTab_titleAxisLegendTab_yMin_doubleSpinBox;
    QDoubleSpinBox* yMaxDoubleSpinBox = this->plottingTab_titleAxisLegendTab_yMax_doubleSpinBox;

    yMinDoubleSpinBox->setEnabled( checkState );

    if( checkState )
    {
        yMaxDoubleSpinBox->setMinimum( yMinDoubleSpinBox->value() );
    }
    else
    {
        yMaxDoubleSpinBox->setMinimum( - 100 );
    }
}

void FADTTSWindow::OnYMaxToggled( bool checkState )
{
    QDoubleSpinBox* yMinDoubleSpinBox = this->plottingTab_titleAxisLegendTab_yMin_doubleSpinBox;
    QDoubleSpinBox* yMaxDoubleSpinBox = this->plottingTab_titleAxisLegendTab_yMax_doubleSpinBox;

    yMaxDoubleSpinBox->setEnabled( checkState );

    if( checkState )
    {
        yMinDoubleSpinBox->setMaximum( yMaxDoubleSpinBox->value() );
    }
    else
    {
        yMinDoubleSpinBox->setMaximum( 100 );
    }
}

void FADTTSWindow::OnYMinValueChanged( double yMinValue )
{
    this->plottingTab_titleAxisLegendTab_yMax_doubleSpinBox->setMinimum( yMinValue );
}

void FADTTSWindow::OnYMaxValueChanged( double yMaxValue )
{
    this->plottingTab_titleAxisLegendTab_yMin_doubleSpinBox->setMaximum( yMaxValue );
}

void FADTTSWindow::OnUpdatingPlotAxis()
{
    UpdatePlotAxis();
}


void FADTTSWindow::OnUpdatingLegend( const QString& legendPosition )
{
    m_plot->SetLegend( legendPosition );
}


void FADTTSWindow::OnUpdatingPvalueThreshold( double pvalueThreshold )
{
    m_plot->SetPvalueThreshold() = pvalueThreshold;
    m_plot->UpdatePvalueThresold( this->plottingTab_titleAxisLegendTab_useCustomizedTitle_checkBox->isChecked() );
}

void FADTTSWindow::OnUpdatingLineWidth( double lineWidth )
{
    m_plot->SetLineWidth() = lineWidth;
    m_plot->UpdateLineWidth();
}

void FADTTSWindow::OnUpdatingSelectedColorLine( const QString& color )
{
    m_plot->UpdateSelectedLineColor( color );
}

void FADTTSWindow::OnUpdatingMarkerType( const QString& markerType )
{
    m_plot->SetMarkerType( markerType );
    m_plot->UpdateMarker();
}

void FADTTSWindow::OnUpdatingMarkerSize( double markerSize )
{
    m_plot->SetMarkerSize() = markerSize;
    m_plot->UpdateMarker();
}


void FADTTSWindow::OnDisplayPlot()
{
    m_plot->ClearPlot();
    m_lineSelectedListWidget->hide();

    if( m_plotSelected.contains( "by Covariates" ) )
    {
        AddLinesForDisplay( true );
    }
    else
    {
        if(  m_plotSelected.contains( "by Properties" ) ||  m_plotSelected.contains( "Local pvalues" ) )
        {
            AddLinesForDisplay( false );
        }
    }

    EditCovariatesNames();
    UpdatePlot();

    bool isPlotDisplayed = m_plot->DisplayPlot();
    this->plottingTab_savePlot_pushButton->setEnabled( isPlotDisplayed );
    this->plottingTab_resetPlot_pushButton->setEnabled( isPlotDisplayed );

    bool isVisible = isPlotDisplayed && ( m_plotSelected.contains( "Betas by" ) || m_plotSelected.contains( "Local pvalues" ) );
    SetSelectionLinesDisplayedVisible( isVisible );
}

void FADTTSWindow::OnResetPlot()
{
    m_plot->ClearPlot();
    this->plottingTab_savePlot_pushButton->setEnabled( false );
    this->plottingTab_resetPlot_pushButton->setEnabled( false );
}

void FADTTSWindow::OnLoadPlotSettings()
{
    QString filename = QFileDialog::getOpenFileName( this , tr( "Load Plot Settings" ) , "" , tr( ".csv( *.csv ) ;; .*( * )" ) );
    if( !filename.isEmpty() )
    {
        LoadPlotSettings( filename );
    }
}

void FADTTSWindow::OnSavePlotSettings()
{
    QString filePath = QFileDialog::getSaveFileName( this, tr( qPrintable( "Save Plot Settings as ..." ) ), "newPlotSettings.csv", tr( ".csv( *.csv ) ;; .*( * )" ) );
    if( !filePath.isEmpty() )
    {
        SavePlotSettings( filePath );
    }
}



/***********************  Private  functions  ***********************/
void FADTTSWindow::SetColorsComboBox( QComboBox* &comboBox )
{
    QStringList colors;
    colors.append( QStringList() << "Red" << "Lime" << "Blue" << "Carolina Blue"
                   << "Yellow" << "Cyan" << "Magenta"
                   << "Olive" << "Teal" << "Purple"
                   << "Rosy Brown" << "Dark Sea Green" << "Corn Flower Blue"
                   << "Maroon" << "Green" << "Navy"
                   << "Orange" << "Mint" << "Pink"
                   << "Brown" << "Black" );
    comboBox->addItems( colors );
}

void FADTTSWindow::ResetPropertyEdition()
{
    nameLabelMapType::ConstIterator iterPropertiesColorsLabel = m_propertiesNameLabelMap.cbegin();
    while( iterPropertiesColorsLabel != m_propertiesNameLabelMap.cend() )
    {
        delete iterPropertiesColorsLabel.value().second;
        ++iterPropertiesColorsLabel;
    }
    m_propertiesNameLabelMap.clear();

    comboBoxMapType::ConstIterator iterPropertiesColorsComboBoxMap = m_propertiesColorsComboBoxMap.cbegin();
    while( iterPropertiesColorsComboBoxMap != m_propertiesColorsComboBoxMap.cend() )
    {
        delete iterPropertiesColorsComboBoxMap.value().second;
        ++iterPropertiesColorsComboBoxMap;
    }
    m_propertiesColorsComboBoxMap.clear();
}

void FADTTSWindow::SetPropertyEdition( const QStringList& propertiesAvailable )
{
    ResetPropertyEdition();

    int i = 0;
    foreach( QString property, propertiesAvailable )
    {
        QLabel *newLabel = new QLabel( property );
        newLabel->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Preferred );

        QComboBox *newComboBox = new QComboBox();
        SetColorsComboBox( newComboBox );
        newComboBox->setCurrentIndex( i );

        gridLayout_PropertiesColors->addWidget( newLabel, i, 0 );
        gridLayout_PropertiesColors->addWidget( newComboBox, i, 1 );
        gridLayout_PropertiesColors->addItem( new QSpacerItem( 27, 27, QSizePolicy::Expanding, QSizePolicy::Fixed ), i, 2 );

        QPair< QString, QLabel* > currentLabelPair;
        currentLabelPair.first = property;
        currentLabelPair.second = newLabel;
        m_propertiesNameLabelMap.insert( i, currentLabelPair );

        QPair< QString, QComboBox* > currentComboBoxPair;
        currentComboBoxPair.first = property;
        currentComboBoxPair.second = newComboBox;
        m_propertiesColorsComboBoxMap.insert( i, currentComboBoxPair );
        i++;
    }

    QSignalMapper *signalMapperUpdatePropertyPlotColor = new QSignalMapper( this );
    connect( signalMapperUpdatePropertyPlotColor, SIGNAL( mapped( const QString& ) ), this, SLOT( OnUpdatingPropertyPlotColor( const QString& ) ) );
    comboBoxMapType::ConstIterator iterPropertyColor = m_propertiesColorsComboBoxMap.cbegin();
    while( iterPropertyColor != m_propertiesColorsComboBoxMap.cend() )
    {
        connect( iterPropertyColor.value().second, SIGNAL( currentIndexChanged( const QString& ) ), signalMapperUpdatePropertyPlotColor,SLOT(map() ) );
        signalMapperUpdatePropertyPlotColor->setMapping( iterPropertyColor.value().second, iterPropertyColor.value().first );
        ++iterPropertyColor;
    }
}

void FADTTSWindow::SetPropertiesForDisplay( const QStringList& propertiesForDisplay )
{
    m_propertiesForDisplay.clear();
    int i = 0;
    foreach( QString property, propertiesForDisplay )
    {
        QPair< bool, QString > currentPairSelectedColor;
        currentPairSelectedColor.first = true;
        currentPairSelectedColor.second = m_propertiesColorsComboBoxMap.value( i ).second->currentText();

        QPair< QString, QPair< bool, QString > > currentPair;
        currentPair.first = property;
        currentPair.second = currentPairSelectedColor;

        m_propertiesForDisplay.insert( i, currentPair );
        i++;
    }
}

void FADTTSWindow::ResetCovariateEdition()
{
    nameLabelMapType::ConstIterator iterCovariatesColorsLabel = m_covariatesNameLabelMap.cbegin();
    while( iterCovariatesColorsLabel != m_covariatesNameLabelMap.cend() )
    {
        delete iterCovariatesColorsLabel.value().second;
        ++iterCovariatesColorsLabel;
    }
    m_covariatesNameLabelMap.clear();

    comboBoxMapType::ConstIterator iterCoveriatesColorsComboBoxMap = m_covariatesColorsComboBoxMap.cbegin();
    while( iterCoveriatesColorsComboBoxMap != m_covariatesColorsComboBoxMap.cend() )
    {
        delete iterCoveriatesColorsComboBoxMap.value().second;
        ++iterCoveriatesColorsComboBoxMap;
    }
    m_covariatesColorsComboBoxMap.clear();

    covariateNameLineEditMapType::ConstIterator iterCoveriatesColorsLineEdit = m_covariatesNameLineEditMap.cbegin();
    while( iterCoveriatesColorsLineEdit != m_covariatesNameLineEditMap.cend() )
    {
        delete iterCoveriatesColorsLineEdit.value().second;
        ++iterCoveriatesColorsLineEdit;
    }
    m_covariatesNameLineEditMap.clear();
}

void FADTTSWindow::SetCovariateEdition( const QMap< int, QString >& allCovariatesUsed )
{
    ResetCovariateEdition();

    QMap< int, QString >::ConstIterator iterCovariateForDisplay = allCovariatesUsed.cbegin();
    while( iterCovariateForDisplay != allCovariatesUsed.cend() )
    {
        QLabel *newLabel = new QLabel( iterCovariateForDisplay.value() );
        newLabel->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Preferred );

        QComboBox *newComboBox = new QComboBox();
        SetColorsComboBox( newComboBox );
        newComboBox->setCurrentIndex( iterCovariateForDisplay.key() );

        QLineEdit *newLineEdit = new QLineEdit();

        gridLayout_CovariatesEdition->addWidget( newLabel, iterCovariateForDisplay.key(), 0 );
        gridLayout_CovariatesEdition->addWidget( newComboBox, iterCovariateForDisplay.key(), 1 );
        gridLayout_CovariatesEdition->addWidget( newLineEdit, iterCovariateForDisplay.key(), 2 );

        QPair< QString, QLabel* > currentLabelPair;
        currentLabelPair.first = iterCovariateForDisplay.value();
        currentLabelPair.second = newLabel;
        m_covariatesNameLabelMap.insert( iterCovariateForDisplay.key(), currentLabelPair );

        QPair< QString, QComboBox* > currentComboBoxPair;
        currentComboBoxPair.first = iterCovariateForDisplay.value();
        currentComboBoxPair.second = newComboBox;
        m_covariatesColorsComboBoxMap.insert( iterCovariateForDisplay.key(), currentComboBoxPair );

        QPair< QString, QLineEdit* > currentLineEditPair;
        currentLineEditPair.first = iterCovariateForDisplay.value();
        currentLineEditPair.second = newLineEdit;
        m_covariatesNameLineEditMap.insert( iterCovariateForDisplay.key(), currentLineEditPair );

        ++iterCovariateForDisplay;
    }

    QSignalMapper *signalMapperUpdateCovariatePlotColor = new QSignalMapper( this );
    connect( signalMapperUpdateCovariatePlotColor, SIGNAL( mapped( const QString& ) ), this, SLOT( OnUpdatingCovariatePlotColor( const QString& ) ) );
    comboBoxMapType::ConstIterator iterCovariateColor = m_covariatesColorsComboBoxMap.cbegin();
    while( iterCovariateColor != m_covariatesColorsComboBoxMap.cend() )
    {
        connect( iterCovariateColor.value().second, SIGNAL( currentIndexChanged( const QString& ) ), signalMapperUpdateCovariatePlotColor,SLOT(map() ) );
        signalMapperUpdateCovariatePlotColor->setMapping( iterCovariateColor.value().second, iterCovariateColor.value().first );
        ++iterCovariateColor;
    }
}

void FADTTSWindow::SetCovariatesForDisplay( const QMap< int, QString >& covariatesForDisplay )
{
    m_covariatesForDisplay.clear();
    QMap< int, QString >::ConstIterator iterCovariateForDisplay = covariatesForDisplay.cbegin();
    while( iterCovariateForDisplay != covariatesForDisplay.cend() )
    {
        QPair< bool, QString > currentPairSelectedColor;
        currentPairSelectedColor.first = true;
        currentPairSelectedColor.second = m_covariatesColorsComboBoxMap.value( iterCovariateForDisplay.key() ).second->currentText();

        QPair< QString, QPair< bool, QString > > currentPair;
        currentPair.first = iterCovariateForDisplay.value();
        currentPair.second = currentPairSelectedColor;

        m_covariatesForDisplay.insert( iterCovariateForDisplay.key(), currentPair );
        ++iterCovariateForDisplay;
    }
}


void FADTTSWindow::SetPlotOptions( bool isPlotSelected, bool propertySelectionAvailable, bool covariateSelectionAvailable )
{
    this->plottingTab_displayPlot_pushButton->setEnabled( isPlotSelected );

    m_propertyComboBox->setEnabled( propertySelectionAvailable );
    this->plottingTab_loadSetDataTab_propertySelection_label->setEnabled( propertySelectionAvailable );

    m_covariateComboBox->setEnabled( covariateSelectionAvailable );
    this->plottingTab_loadSetDataTab_covariateSelection_label->setEnabled( covariateSelectionAvailable );
}

void FADTTSWindow::AddLinesForDisplay( bool isSelectionProperties )
{
    m_lineDisplayedListWidget->clear();
    m_areLinesForDisplayProperties = isSelectionProperties;
    m_currentLinesForDisplay = m_areLinesForDisplayProperties ? m_propertiesForDisplay : m_covariatesForDisplay;
    displayMapType::ConstIterator iterLinesForDisplay = m_currentLinesForDisplay.cbegin();
    while( iterLinesForDisplay != m_currentLinesForDisplay.cend() )
    {
        QListWidgetItem *covariateItem = new QListWidgetItem( iterLinesForDisplay.value().first, m_lineDisplayedListWidget );
        covariateItem->setCheckState( iterLinesForDisplay.value().second.first ? Qt::Checked : Qt::Unchecked );
        covariateItem->setFlags( Qt::ItemIsEnabled );
        m_lineDisplayedListWidget->addItem( covariateItem );
        ++iterLinesForDisplay;
    }
    if( m_plotComboBox->currentText() == "Omnibus Local pvalues" ||
            m_plotComboBox->currentText() == "Omnibus FDR Local pvalues" )
    {
        m_lineDisplayedListWidget->item( 0 )->setHidden( true );
    }

    m_lineDisplayedListWidget->setMaximumHeight( m_lineDisplayedListWidget->sizeHintForRow( 0 ) * ( m_lineDisplayedListWidget->count() + 1 ) );
}

void FADTTSWindow::SetCheckStateLinesToDisplay( Qt::CheckState checkState )
{
    for( int i = 0; i < m_lineDisplayedListWidget->count(); i++ )
    {
        QListWidgetItem * currentItem = m_lineDisplayedListWidget->item( i );
        currentItem->setCheckState( checkState );
        displayMapType::Iterator currentIterator = m_currentLinesForDisplay.begin();
        while( currentIterator != m_currentLinesForDisplay.end() )
        {
            currentIterator.value().second.first = currentItem->checkState();
            ++currentIterator;
        }
    }
    if( m_areLinesForDisplayProperties )
    {
        m_propertiesForDisplay = m_currentLinesForDisplay;
    }
    else
    {
        m_covariatesForDisplay = m_currentLinesForDisplay;
    }
}

void FADTTSWindow::SetSelectionLinesDisplayedVisible( bool visible )
{
    this->plottingTab_loadSetDataTab_linesToDisplay_label->setHidden( !visible );
    m_lineDisplayedListWidget->setHidden( !visible );
    this->plottingTab_loadSetDataTab_checkAllLinesToDisplay_pushButton->setHidden( !visible );
    this->plottingTab_loadSetDataTab_uncheckAllLinesToDisplay_pushButton->setHidden( !visible );
}

void FADTTSWindow::EditCovariatesNames()
{
    covariateNameLineEditMapType::ConstIterator iterCovariatesName = m_covariatesNameLineEditMap.cbegin();
    displayMapType::ConstIterator iterCovariatesForDisplay = m_covariatesForDisplay.cbegin();
    QMap< int, QString > newCovariatesNames;
    int currentIndex = m_covariateComboBox->currentIndex();

    int i = 0;
    while( iterCovariatesName != m_covariatesNameLineEditMap.cend() )
    {
        QString newCovariateName = iterCovariatesName.value().second->text().isEmpty() ? iterCovariatesName.value().first : iterCovariatesName.value().second->text();
        m_covariatesForDisplay[ iterCovariatesName.key() ].first = newCovariateName;
        newCovariatesNames.insert( iterCovariatesName.key(), newCovariateName );
        ++iterCovariatesName;
        ++iterCovariatesForDisplay;
    }
    m_plot->UpdateCovariatesNames( newCovariatesNames );
    m_covariateComboBox->setCurrentIndex( currentIndex );

    AddLinesForDisplay( m_areLinesForDisplayProperties );
}


void FADTTSWindow::UpdatePlotAxis()
{
    if( this->plottingTab_titleAxisLegendTab_useCustomizedAxis_checkBox->isChecked() )
    {
        m_plot->SetCustomizedAxis( this->plottingTab_titleAxisLegendTab_xName_lineEdit->text(),
                                   this->plottingTab_titleAxisLegendTab_yName_lineEdit->text(),
                                   this->plottingTab_titleAxisLegendTab_axisBold_checkBox->isChecked(),
                                   this->plottingTab_titleAxisLegendTab_axisItalic_checkBox->isChecked(),
                                   this->plottingTab_titleAxisLegendTab_yMin_checkBox->isChecked(),
                                   this->plottingTab_titleAxisLegendTab_yMin_doubleSpinBox->value(),
                                   this->plottingTab_titleAxisLegendTab_yMax_checkBox->isChecked(),
                                   this->plottingTab_titleAxisLegendTab_yMax_doubleSpinBox->value() );
    }
    else
    {
        m_plot->SetDefaultAxis();
    }
}

void FADTTSWindow::UpdatePlotTitle()
{
    if( this->plottingTab_titleAxisLegendTab_useCustomizedTitle_checkBox->isChecked() )
    {
        m_plot->SetCustomizedTitle( this->plottingTab_titleAxisLegendTab_titleName_lineEdit->text(),
                                    this->plottingTab_titleAxisLegendTab_titleBold_checkBox->isChecked(),
                                    this->plottingTab_titleAxisLegendTab_titleItalic_checkBox->isChecked(),
                                    this->plottingTab_titleAxisLegendTab_titleSize_doubleSpinBox->value() );
    }
    else
    {
        m_plot->SetDefaultTitle();
    }
}

void FADTTSWindow::UpdatePlot()
{
    m_plot->SetSelectionToDisPlay() = m_currentLinesForDisplay;
    m_plot->SetCovariatesProperties( m_covariatesForDisplay );

    m_plot->SetGrid() = this->plottingTab_titleAxisLegendTab_gridOn_checkBox->isChecked();
    UpdatePlotAxis();

    m_plot->SetLegend( this->plottingTab_titleAxisLegendTab_legendPosition_comboBox->currentText() );

    m_plot->SetPvalueThreshold() = this->plottingTab_editionTab_pvalueThreshold_doubleSpinBox->value();

    m_plot->SetLineWidth() = this->plottingTab_editionTab_lineWidth_doubleSpinBox->value();
    m_plot->SetSelectedLineColor( this->plottingTab_editionTab_selectedLineColor_comboBox->currentText() );

    m_plot->SetMarkerType( this->plottingTab_editionTab_markerType_comboBox->currentText() );
    m_plot->SetMarkerSize() = this->plottingTab_editionTab_markerSize_doubleSpinBox->value();

    UpdatePlotTitle();
}


void FADTTSWindow::ResetPlotTab()
{
    m_plotComboBox->setCurrentText( "No Plot" );

    this->plottingTab_titleAxisLegendTab_titleName_lineEdit->clear();

    this->plottingTab_displayPlot_pushButton->setEnabled( false );
    this->plottingTab_resetPlot_pushButton->setEnabled( false );
    this->plottingTab_savePlot_pushButton->setEnabled( false );

    this->plottingTab_loadPlotSettings_pushButton->setEnabled( false );
    this->plottingTab_savePlotSettings_pushButton->setEnabled( false );    
}

void FADTTSWindow::SetPlotTab()
{
    m_isPlotReady = false;
    m_plot->ClearPlot();
    m_plot->ResetPlotData();
    m_lineSelectedListWidget->hide();
    SetSelectionLinesDisplayedVisible( false );

    QLabel *fibernameIcon = this->plottingTab_loadSetDataTab_fibernameIcon_label;
    QLabel *plotDirInfo = this->plottingTab_loadSetDataTab_plotDirectoryInfo_label;
    QLabel *plotDirIcon = this->plottingTab_loadSetDataTab_plotDirectoryIcon_label;
    QString fibername = this->para_plottingTab_loadSetDataTab_fibername_lineEdit->text().remove( " ", Qt::CaseInsensitive );
    QString directory = this->para_plottingTab_loadSetDataTab_browsePlotDirectory_lineEdit->text();

    if( QDir( directory ).exists() && !directory.isEmpty() )
    {
        m_isPlotReady = m_plot->InitPlot( directory, fibername );
    }
    else
    {
        ResetPlotTab();
    }

    if( m_isPlotReady )
    {
        DisplayIcon( fibernameIcon, !fibername.isEmpty() ? m_okPixmap : m_warningPixmap );
    }
    else
    {
        fibernameIcon->clear();
    }
    if( !directory.isEmpty() )
    {
        plotDirInfo->show();
        DisplayIcon( plotDirIcon, m_isPlotReady ? m_okPixmap : m_koPixmap );
    }

    this->plottingTab_loadSetDataTab_set_widget->setEnabled( m_isPlotReady );
    this->plottingTab_titleAxisLegendTab->setEnabled( m_isPlotReady );
    this->plottingTab_editionTab->setEnabled( m_isPlotReady );
    this->plottingTab_loadPlotSettings_pushButton->setEnabled( m_isPlotReady );
    this->plottingTab_savePlotSettings_pushButton->setEnabled( m_isPlotReady );

    SetPlotOptions( false, false, false );
}

void FADTTSWindow::LoadPlotSettings( QString filePath )
{
    QList< QStringList > plotSettings = m_processing.GetDataFromFile( filePath );
    int nbrFixedSettings = 20;

    qDebug() << endl << plotSettings;

    if( plotSettings.size() == ( nbrFixedSettings + m_propertiesColorsComboBoxMap.size() + 2*m_covariatesColorsComboBoxMap.size() ) )
    {
        int index = 0;

        /*** Title/Axis/Legend Data Tab ***/
        this->plottingTab_titleAxisLegendTab_useCustomizedTitle_checkBox->setChecked( plotSettings.at( index ).at( 1 ).toInt() == 0 ? false : true );
        index++;
        this->plottingTab_titleAxisLegendTab_titleBold_checkBox->setChecked( plotSettings.at( index ).at( 1 ).toInt() == 0 ? false : true );
        index++;
        this->plottingTab_titleAxisLegendTab_titleItalic_checkBox->setChecked( plotSettings.at( index ).at( 1 ).toInt() == 0 ? false : true );
        index++;
        this->plottingTab_titleAxisLegendTab_titleSize_doubleSpinBox->setValue( plotSettings.at( index ).at( 1 ).toDouble() );
        index++;

        this->plottingTab_titleAxisLegendTab_gridOn_checkBox->setChecked( plotSettings.at( index ).at( 1 ).toInt() == 0 ? false : true );
        index++;
        this->plottingTab_titleAxisLegendTab_useCustomizedAxis_checkBox->setChecked( plotSettings.at( index ).at( 1 ).toInt() == 0 ? false : true );
        index++;
        if( plotSettings.at( index ).size() == 1 )
        {
            this->plottingTab_titleAxisLegendTab_xName_lineEdit->clear();
        }
        else
        {
            this->plottingTab_titleAxisLegendTab_xName_lineEdit->setText( plotSettings.at( index ).at( 1 ) );
        }
        index++;
        if( plotSettings.at( index ).size() == 1 )
        {
            this->plottingTab_titleAxisLegendTab_yName_lineEdit->clear();
        }
        else
        {
            this->plottingTab_titleAxisLegendTab_yName_lineEdit->setText( plotSettings.at( index ).at( 1 ) );
        }
        index++;
        this->plottingTab_titleAxisLegendTab_axisBold_checkBox->setChecked( plotSettings.at( index ).at( 1 ).toInt() == 0 ? false : true );
        index++;
        this->plottingTab_titleAxisLegendTab_axisItalic_checkBox->setChecked( plotSettings.at( index ).at( 1 ).toInt() == 0 ? false : true );
        index++;
        this->plottingTab_titleAxisLegendTab_yMin_checkBox->setChecked( plotSettings.at( index ).at( 1 ).toInt() == 0 ? false : true );
        index++;
        this->plottingTab_titleAxisLegendTab_yMin_doubleSpinBox->setValue( plotSettings.at( index ).at( 1 ).toDouble() );
        index++;
        this->plottingTab_titleAxisLegendTab_yMax_checkBox->setChecked( plotSettings.at( index ).at( 1 ).toInt() == 0 ? false : true );
        index++;
        this->plottingTab_titleAxisLegendTab_yMax_doubleSpinBox->setValue( plotSettings.at( index ).at( 1 ).toDouble() );
        index++;

        this->plottingTab_titleAxisLegendTab_legendPosition_comboBox->setCurrentText( plotSettings.at( index ).at( 1 ) );
        index++;

        /*** Edition Tab ***/
        this->plottingTab_editionTab_pvalueThreshold_doubleSpinBox->setValue( plotSettings.at( index ).at( 1 ).toDouble() );
        index++;

        this->plottingTab_editionTab_lineWidth_doubleSpinBox->setValue( plotSettings.at( index ).at( 1 ).toDouble() );
        index++;
        this->plottingTab_editionTab_selectedLineColor_comboBox->setCurrentText( plotSettings.at( index ).at( 1 ) );
        index++;

        this->plottingTab_editionTab_markerType_comboBox->setCurrentText( plotSettings.at( index ).at( 1 ) );
        index++;
        this->plottingTab_editionTab_markerSize_doubleSpinBox->setValue( plotSettings.at( index ).at( 1 ).toDouble() );
        index++;

        comboBoxMapType::ConstIterator iterPropertyColor = m_propertiesColorsComboBoxMap.cbegin();
        while( iterPropertyColor != m_propertiesColorsComboBoxMap.cend() )
        {
            iterPropertyColor.value().second->setCurrentText( plotSettings.at( index ).at( 1 ) );
            index++;
            ++iterPropertyColor;
        }

        comboBoxMapType::ConstIterator iterCovariateColor = m_covariatesColorsComboBoxMap.cbegin();
        covariateNameLineEditMapType::ConstIterator iterCovariateName = m_covariatesNameLineEditMap.cbegin();
        while( iterCovariateColor != m_covariatesColorsComboBoxMap.cend() )
        {
            iterCovariateColor.value().second->setCurrentText( plotSettings.at( index ).at( 1 ) );
            index++;
            if( plotSettings.at( index ).size() == 1 )
            {
                iterCovariateName.value().second->clear();
            }
            else
            {
                iterCovariateName.value().second->setText( plotSettings.at( index ).at( 1 ) );
            }
            index++;
            ++iterCovariateColor;
            ++iterCovariateName;
        }

        EditCovariatesNames();
    }
}

void FADTTSWindow::SavePlotSettings( QString filePath )
{
    QFile exportedCSV( filePath );
    exportedCSV.open( QIODevice::WriteOnly );
    QTextStream ts( &exportedCSV );

    /*** Title/Axis/Legend Data Tab ***/
    ts << ( QStringList() << "setTitle_checkState" << QString::number( this->plottingTab_titleAxisLegendTab_useCustomizedTitle_checkBox->isChecked() ) ).join( m_csvSeparator ) << endl;
    ts << ( QStringList() << "titleBold_checkState" << QString::number( this->plottingTab_titleAxisLegendTab_titleBold_checkBox->isChecked() ) ).join( m_csvSeparator ) << endl;
    ts << ( QStringList() << "titleItalic_checkState" << QString::number( this->plottingTab_titleAxisLegendTab_titleItalic_checkBox->isChecked() ) ).join( m_csvSeparator ) << endl;
    ts << ( QStringList() << "titleSize_value" << QString::number( this->plottingTab_titleAxisLegendTab_titleSize_doubleSpinBox->value() ) ).join( m_csvSeparator ) << endl;

    ts << ( QStringList() << "grid" << QString::number( this->plottingTab_titleAxisLegendTab_gridOn_checkBox->isChecked() ) ).join( m_csvSeparator ) << endl;
    ts << ( QStringList() << "setAxis_checkState" << QString::number( this->plottingTab_titleAxisLegendTab_useCustomizedAxis_checkBox->isChecked() ) ).join( m_csvSeparator ) << endl;
    ts << ( QStringList() << "xAxis_name" << this->plottingTab_titleAxisLegendTab_xName_lineEdit->text() ).join( m_csvSeparator ) << endl;
    ts << ( QStringList() << "yAxis_name" << this->plottingTab_titleAxisLegendTab_yName_lineEdit->text() ).join( m_csvSeparator ) << endl;
    ts << ( QStringList() << "axisBold_checkState" << QString::number( this->plottingTab_titleAxisLegendTab_axisBold_checkBox->isChecked() ) ).join( m_csvSeparator ) << endl;
    ts << ( QStringList() << "axisItalic_checkState" << QString::number( this->plottingTab_titleAxisLegendTab_axisItalic_checkBox->isChecked() ) ).join( m_csvSeparator ) << endl;
    ts << ( QStringList() << "yMin_checkState" << QString::number( this->plottingTab_titleAxisLegendTab_yMin_checkBox->isChecked() ) ).join( m_csvSeparator ) << endl;
    ts << ( QStringList() << "yMin_value" << QString::number( this->plottingTab_titleAxisLegendTab_yMin_doubleSpinBox->value() ) ).join( m_csvSeparator ) << endl;
    ts << ( QStringList() << "yMax_checkState" << QString::number( this->plottingTab_titleAxisLegendTab_yMax_checkBox->isChecked() ) ).join( m_csvSeparator ) << endl;
    ts << ( QStringList() << "yMax_value" << QString::number( this->plottingTab_titleAxisLegendTab_yMax_doubleSpinBox->value() ) ).join( m_csvSeparator ) << endl;

    ts << ( QStringList() << "legend_position" << this->plottingTab_titleAxisLegendTab_legendPosition_comboBox->currentText() ).join( m_csvSeparator ) << endl;

    /*** Edition Tab ***/
    ts << ( QStringList() << "alpha_value" << QString::number( this->plottingTab_editionTab_pvalueThreshold_doubleSpinBox->value() ) ).join( m_csvSeparator ) << endl;

    ts << ( QStringList() << "lineWidth" << QString::number( this->plottingTab_editionTab_lineWidth_doubleSpinBox->value() ) ).join( m_csvSeparator ) << endl;
    ts << ( QStringList() << "selectedLine_color" << this->plottingTab_editionTab_selectedLineColor_comboBox->currentText() ).join( m_csvSeparator ) << endl;

    ts << ( QStringList() << "markerType" << this->plottingTab_editionTab_markerType_comboBox->currentText() ).join( m_csvSeparator ) << endl;
    ts << ( QStringList() << "markerSize" << QString::number( this->plottingTab_editionTab_markerSize_doubleSpinBox->value() ) ).join( m_csvSeparator ) << endl;

    comboBoxMapType::ConstIterator iterPropertyColor = m_propertiesColorsComboBoxMap.cbegin();
    while( iterPropertyColor != m_propertiesColorsComboBoxMap.cend() )
    {
        ts << ( QStringList() << ( iterPropertyColor.value().first  + "_color" ) << iterPropertyColor.value().second->currentText() ).join( m_csvSeparator ) << endl;
        ++iterPropertyColor;
    }

    comboBoxMapType::ConstIterator iterCovariateColor = m_covariatesColorsComboBoxMap.cbegin();
    covariateNameLineEditMapType::ConstIterator iterCovariateName = m_covariatesNameLineEditMap.cbegin();
    while( iterCovariateColor != m_covariatesColorsComboBoxMap.cend() )
    {
        ts << ( QStringList() << ( iterCovariateColor.value().first + "_color" ) << iterCovariateColor.value().second->currentText() ).join( m_csvSeparator ) << endl;
        ts << ( QStringList() << ( iterCovariateColor.value().first + "_newName" ) << iterCovariateName.value().second->text() ).join( m_csvSeparator ) << endl;
        ++iterCovariateColor;
        ++iterCovariateName;
    }

    exportedCSV.flush();
    exportedCSV.close();
}
