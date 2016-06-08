#include "FADTTSWindow.h"

//#include <QDebug>


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
FADTTSWindow::FADTTSWindow( QWidget *parent, Qt::WindowFlags f ):
    QMainWindow( parent, f )
{
    this->setupUi( this );

    m_okPixmap = QPixmap( ":/Icons/Resources/Icons/okIcon.xpm" );
    m_koPixmap = QPixmap( ":/Icons/Resources/Icons/koIcon.xpm" );
    m_warningPixmap = QPixmap( ":/Icons/Resources/Icons/warningIcon.xpm" );

    InitFADTTSWindow();
}

FADTTSWindow::~FADTTSWindow()
{
    m_editInputDialog.clear();
}


void FADTTSWindow::LoadParaConfiguration( QString filename )
{
    QString text;
    QFile refJSON( filename );
    refJSON.open( QIODevice::ReadOnly | QIODevice::Text );
    text = refJSON.readAll();

    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson( text.toUtf8(), &jsonError );
    if( jsonError.error == QJsonParseError::NoError )
    {
        QJsonObject jsonObject_param = jsonDoc.object().value( "paraConfiguration" ).toObject();

        /****** 1st tab: Inputs ******/
        QJsonObject inputTab = jsonObject_param.value( "inputTab" ).toObject();
        QJsonObject inputFiles = inputTab.value( "inputFiles" ).toObject();
        for( int i = 0; i < m_inputTabInputFileLineEditMap.size(); i++ )
        {
            m_inputTabInputFileLineEditMap.value( i )->setText( inputFiles.value( m_data.GetDiffusionPropertyName( i ) ).toString() );
        }
        OnUpdatingSubjectColumnID( inputFiles.value( "subjectColumnID" ).toInt( 0 ) );

        QJsonObject covariates = inputTab.value( "covariates" ).toObject();
        for( int i = 0; i < std::min( m_covariateListWidget->count(), covariates.size() ) ; i++ )
        {
            QListWidgetItem *currentItem = m_covariateListWidget->item( i );
            currentItem->setCheckState( covariates.value( currentItem->text() ).toBool() ? Qt::Checked : Qt::Unchecked );
        }

        /****** 2nd tab: Subjects ******/
        QJsonObject subjectTab = jsonObject_param.value( "subjectTab" ).toObject();
        QJsonObject filesUsed = subjectTab.value( "filesUsed" ).toObject();
        for( int i = 0; i < m_paramTabFileCheckBoxMap.size(); i++ )
        {
            m_paramTabFileCheckBoxMap.value( i )->setChecked( m_paramTabFileCheckBoxMap.value( i )->isEnabled() ? filesUsed.value( m_data.GetDiffusionPropertyName( i ) ).toBool() : false );
        }

        para_subjectTab_qcThresoldOnAtlas_radioButton->setChecked( subjectTab.value( "useAtlas" ).toBool() );
        para_subjectTab_qcThreshold_doubleSpinBox->setValue( subjectTab.value( "qcThreshold" ).toDouble( 0.85 ) );
        para_subjectTab_subjectFile_lineEdit->setText( subjectTab.value( "subjectListPath" ).toString() );

        /****** 3rd tab: Execution ******/
        QJsonObject executionTab = jsonObject_param.value( "executionTab" ).toObject();
        QJsonObject settings = executionTab.value( "settings" ).toObject();
        para_executionTab_fiberName_lineEdit->setText( settings.value( "fiberName" ).toString() );
        para_executionTab_nbrPermutations_spinBox->setValue( settings.value( "nbrPermutations" ).toInt( 100 ) );
        para_executionTab_confidenceBandsThreshold_doubleSpinBox->setValue( settings.value( "confidenceBandThreshold" ).toDouble( 0.05 ) );
        para_executionTab_pvalueThreshold_doubleSpinBox->setValue( settings.value( "pvalueThreshold" ).toDouble( 0.05 ) );
        para_executionTab_omnibus_checkBox->setChecked( settings.value( "omnibus" ).toBool() );
        para_executionTab_postHoc_checkBox->setChecked( settings.value( "posthoc" ).toBool() );

        para_executionTab_mvcm_lineEdit->setText( executionTab.value( "matlabSpecifications" ).toObject().value( "fadttsDir" ).toString() );
        para_executionTab_outputDir_lineEdit->setText( executionTab.value( "outputDir" ).toString() );

        /****** 4th tab: Plotting ******/
        QJsonObject plottingTab = jsonObject_param.value( "plottingTab" ).toObject();
        para_plottingTab_loadSetDataTab_browsePlotDirectory_lineEdit->setText( plottingTab.value( "plotdir" ).toString() );
        para_plottingTab_loadSetDataTab_fibername_lineEdit->setText( plottingTab.value( "fiberName" ).toString() );
    }
    else
    {
        QString warningMessage = "<b>Unable to load para configuration</b><br>";
        warningMessage.append( QString( "<br>Trying to load:<br><i>" + filename + "</i><br>" ) );
        warningMessage.append( QString( "<br>Error:<i> " + jsonError.errorString().toUpper() + "</i><br>" ) );
        WarningPopUp( warningMessage );
    }
}

void FADTTSWindow::LoadSoftConfiguration( QString filename )
{
    QString text;
    QFile refJSON( filename );
    refJSON.open( QIODevice::ReadOnly | QIODevice::Text );
    text = refJSON.readAll();

    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson( text.toUtf8(), &jsonError );
    if( jsonError.error == QJsonParseError::NoError )
    {
        QJsonObject jsonObject_soft = jsonDoc.object().value( "softConfiguration" ).toObject();

        /****** 3rd tab: Execution ******/
        QJsonObject executionTab = jsonObject_soft.value( "executionTab" ).toObject();
        QJsonObject matlabSpecifications = executionTab.value( "matlabSpecifications" ).toObject();

        soft_executionTab_runMatlab_checkBox->setChecked( matlabSpecifications.value( "runMatlab" ).toBool() );
        soft_executionTab_matlabExe_lineEdit->setText( matlabSpecifications.value( "matlabExe" ).toString() );
    }
    else
    {
        QString warningMessage = "<b>Unable to load soft configuration</b><br>";
        warningMessage.append( QString( "<br>Trying to load:<br><i>" + filename + "</i><br>" ) );
        warningMessage.append( QString( "<br>Error:<i> " + jsonError.errorString().toUpper() + "</i><br>" ) );
        WarningPopUp( warningMessage );
    }
}



/*********************** Private slots ***********************/
void FADTTSWindow::OnLoadParaConfiguration()
{
    QString dir;
    QString filename = QFileDialog::getOpenFileName( this, tr( "Load Parameters Configuration" ), dir, tr( ".json( *.json ) ;; .*( * )" ) );
    if( !filename.isEmpty() )
    {
        LoadParaConfiguration( filename );
    }
}

void FADTTSWindow::OnSaveParaConfiguration()
{
    QString filename = QFileDialog::getSaveFileName( this, tr( "Save Parameters Configuration" ), "newParaConfiguration.json", tr( ".json( *.json ) ;; .*( * )" ) );
        if( !filename.isEmpty() )
    {
        SaveParaConfiguration( filename );
    }
}

void FADTTSWindow::OnLoadSoftConfiguration()
{
    QString dir;
    QString filename = QFileDialog::getOpenFileName( this , tr( "Load Software Configuration" ) , dir , tr( ".json( *.json ) ;; .*( * )" ) );
    if( !filename.isEmpty() )
    {
        LoadSoftConfiguration( filename );
    }
}

void FADTTSWindow::OnSaveSoftConfiguration()
{
    QString filename = QFileDialog::getSaveFileName( this, tr( "Save Software Configuration" ), "newSoftConfiguration.json", tr( ".json( *.json ) ;; .*( * )" ) );
        if( !filename.isEmpty() )
    {
        SaveSoftConfiguration( filename );
    }
}

void FADTTSWindow::OnSaveNoGUIConfiguration()
{
    QString filename = QFileDialog::getSaveFileName( this, tr( "Save noGUI Configuration" ), "newNoGUIConfiguration.json", tr( ".json( *.json ) ;; .*( * )" ) );
        if( !filename.isEmpty() )
    {
        SaveNoGUIConfiguration( filename );
    }
}


void FADTTSWindow::OnDisplayUserGuide()
{
    QResource resource( ":/UserGuide/Resources/UserGuide/UserGuide.txt" );
    QFile UserGuideFile( resource.absoluteFilePath() );
    QString UserGuideText;
    if( UserGuideFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        UserGuideText = UserGuideFile.readAll();
    }
    else
    {
        UserGuideText = "No Description found";
    }

    QString messageBoxTitle = QString( FADTTS_TITLE ) + ": User Guide";
    QMessageBox::information( this, tr( qPrintable( messageBoxTitle ) ), tr( qPrintable( UserGuideText ) ), QMessageBox::Ok );
}


void FADTTSWindow::OnDisplayAbout()
{
    QString messageBoxTitle = "About " + QString( FADTTS_TITLE );
    QString aboutFADTTS;
    aboutFADTTS = "<b>Version:</b> " + QString( FADTTS_VERSION ) + "<br>"
            "<b>Contributor(s):</b> " + QString( FADTTS_CONTRIBUTORS ) + "<br>"
            "<b>Web Site:</b> www.addWebSite.com";
    QMessageBox::information( this, tr( qPrintable( messageBoxTitle ) ), tr( qPrintable( aboutFADTTS ) ), QMessageBox::Ok );
}


void FADTTSWindow::closeEvent( QCloseEvent *event )
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
            m_log->AddText( "\nWarning! Thread terminated by user before completed matlab script.\n" );
            m_matlabThread->terminate();
            m_log->CloseLogFile();
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
void FADTTSWindow::SaveParaConfiguration( QString filename )
{
    QJsonObject jsonObject;
    QJsonObject jsonObject_param;
    QJsonObject inputTab;
    QJsonObject subjectTab;
    QJsonObject executionTab;
    QJsonObject plottingTab;

    /****** 1st tab: Inputs ******/
    QJsonObject inputFiles;
    for( int i = 0; i < m_inputTabInputFileLineEditMap.size(); i++ )
    {
        inputFiles.insert( m_data.GetDiffusionPropertyName( i ), m_inputTabInputFileLineEditMap.value( i )->text() );
    }
    inputFiles.insert( "subjectColumnID", m_data.GetSubjectColumnID() );
    inputTab.insert( "inputFiles", inputFiles );

    QJsonObject covariates;
    for( int i = 0; i < m_covariateListWidget->count(); i++ )
    {
        covariates.insert( m_covariateListWidget->item( i )->text(), ( m_covariateListWidget->item( i )->checkState() == Qt::Checked ) ? true : false );
    }
    inputTab.insert( "covariates", covariates );

    /****** 2nd tab: Subjects ******/
    QJsonObject filesUsed;
    for( int i = 0; i < m_paramTabFileCheckBoxMap.size(); i++ )
    {
        filesUsed.insert( m_data.GetDiffusionPropertyName( i ), m_paramTabFileCheckBoxMap.value( i )->isChecked() );
    }
    subjectTab.insert( "filesUsed", filesUsed );

    subjectTab.insert( "useAtlas", para_subjectTab_qcThresoldOnAtlas_radioButton->isChecked() );
    subjectTab.insert( "qcThreshold", para_subjectTab_qcThreshold_doubleSpinBox->value() );
    subjectTab.insert( "subjectListPath", para_subjectTab_subjectFile_lineEdit->text() );

    /****** 3rd tab: Execution ******/
    QJsonObject settings;
    settings.insert( "fiberName", para_executionTab_fiberName_lineEdit->text() );
    settings.insert( "nbrPermutations", para_executionTab_nbrPermutations_spinBox->value() );
    settings.insert( "confidenceBandThreshold", para_executionTab_confidenceBandsThreshold_doubleSpinBox->value() );
    settings.insert( "pvalueThreshold", para_executionTab_pvalueThreshold_doubleSpinBox->value() );
    settings.insert( "omnibus", para_executionTab_omnibus_checkBox->isChecked() );
    settings.insert( "posthoc", para_executionTab_postHoc_checkBox->isChecked() );
    executionTab.insert( "settings", settings );

    QJsonObject matlabSpecifications;
    matlabSpecifications.insert( "fadttsDir", para_executionTab_mvcm_lineEdit->text() );
    executionTab.insert( "matlabSpecifications", matlabSpecifications );

    executionTab.insert( "outputDir", para_executionTab_outputDir_lineEdit->text() );

    /****** 4th tab: Plotting ******/
    plottingTab.insert( "plotdir", para_plottingTab_loadSetDataTab_browsePlotDirectory_lineEdit->text() );
    plottingTab.insert( "fiberName", para_plottingTab_loadSetDataTab_fibername_lineEdit->text() );


    jsonObject_param.insert( "inputTab", inputTab );
    jsonObject_param.insert( "subjectTab", subjectTab );
    jsonObject_param.insert( "executionTab", executionTab );
    jsonObject_param.insert( "plottingTab", plottingTab );
    jsonObject.insert( "paraConfiguration", jsonObject_param );

    QJsonDocument jsonDoc;
    jsonDoc.setObject( jsonObject );
    QFile exportedJSON( filename );
    exportedJSON.open( QIODevice::WriteOnly | QIODevice::Text );
    exportedJSON.write( jsonDoc.toJson( QJsonDocument::Indented ) );
    exportedJSON.flush();
    exportedJSON.close();
}

void FADTTSWindow::SaveSoftConfiguration( QString filename )
{
    QJsonObject jsonObject;
    QJsonObject jsonObject_soft;
    QJsonObject executionTab;

    /****** 3rd tab: Execution ******/
    QJsonObject matlabSpecifications;
    matlabSpecifications.insert( "runMatlab", soft_executionTab_runMatlab_checkBox->isChecked() );
    matlabSpecifications.insert( "matlabExe", soft_executionTab_matlabExe_lineEdit->text() );
    executionTab.insert( "matlabSpecifications", matlabSpecifications );

    jsonObject_soft.insert( "executionTab", executionTab );
    jsonObject.insert( "softConfiguration", jsonObject_soft );

    QJsonDocument jsonDoc;
    jsonDoc.setObject( jsonObject );
    QFile exportedJSON( filename );
    exportedJSON.open( QIODevice::WriteOnly | QIODevice::Text );
    exportedJSON.write( jsonDoc.toJson( QJsonDocument::Indented ) );
    exportedJSON.flush();
    exportedJSON.close();
}

void FADTTSWindow::SaveNoGUIConfiguration( QString filename )
{
    QJsonObject jsonObject;
    QJsonObject jsonObject_noGUI;
    QJsonObject inputFiles;
    QJsonObject covariates;
    QJsonObject subjects;
    QJsonObject settings;
    QJsonObject matlabSpecifications;

    /****** Input Files ******/
    foreach( int propertyIndex, m_data.GetDiffusionPropertiesIndices() )
    {
        QJsonObject currentPropertyObject;
        bool isFileSelected = m_paramTabFileCheckBoxMap.value( propertyIndex )->isChecked()
                && m_paramTabFileCheckBoxMap.value( propertyIndex )->isEnabled();

        currentPropertyObject.insert( "index", propertyIndex );
        currentPropertyObject.insert( "path", isFileSelected ? m_inputTabInputFileLineEditMap.value( propertyIndex )->text() : "" );
        if( m_data.GetDiffusionPropertyName( propertyIndex ) == m_data.GetDiffusionPropertyName( m_data.GetSubMatrixIndex() ) )
        {
            currentPropertyObject.insert( "subjectColumnID", m_data.GetSubjectColumnID() );
        }

        inputFiles.insert( m_data.GetDiffusionPropertyName( propertyIndex ), currentPropertyObject );
    }

    /******  Covariates  ******/
    for( int i = 0; i < m_covariateListWidget->count(); i++ )
    {
        QListWidgetItem *currentItem = m_covariateListWidget->item( i );
        QString currentCovariateName = currentItem->text();
        bool currentCovariateSelected = currentItem->checkState() == Qt::Checked ? true : false;
        int currentCovariateIndex = m_data.GetCovariates().key( currentCovariateName );

        QJsonObject currentCovariateObject;
        currentCovariateObject.insert( "index", currentCovariateIndex );
        currentCovariateObject.insert( "selected", currentCovariateSelected );

        covariates.insert( currentCovariateName, currentCovariateObject );
    }

    /******   Subjects   ******/
    subjects.insert( "subjectListPath", para_subjectTab_subjectFile_lineEdit->text() );

    QJsonObject qcThreshold;
    qcThreshold.insert( "apply", !m_failedQCThresholdSubjects.isEmpty() );
    qcThreshold.insert( "useAtlas", para_subjectTab_qcThresoldOnAtlas_radioButton->isChecked() );
    qcThreshold.insert( "value", para_subjectTab_qcThreshold_doubleSpinBox->value() );
    subjects.insert( "qcThreshold", qcThreshold );

    /******   Settings   ******/
    settings.insert( "fiberName", para_executionTab_fiberName_lineEdit->text() );
    settings.insert( "nbrPermutations", para_executionTab_nbrPermutations_spinBox->value() );
    settings.insert( "confidenceBandThreshold", para_executionTab_confidenceBandsThreshold_doubleSpinBox->value() );
    settings.insert( "pvalueThreshold", para_executionTab_pvalueThreshold_doubleSpinBox->value() );
    settings.insert( "omnibus", para_executionTab_omnibus_checkBox->isChecked() );
    settings.insert( "posthoc", para_executionTab_postHoc_checkBox->isChecked() );

    /******  Output Dir  ******/
    jsonObject_noGUI.insert( "outputDir", para_executionTab_outputDir_lineEdit->text() );

    /* Matlab  Specifications */
    matlabSpecifications.insert( "fadttsDir", para_executionTab_mvcm_lineEdit->text() );
    matlabSpecifications.insert( "runMatlab", soft_executionTab_runMatlab_checkBox->isChecked() );
    matlabSpecifications.insert( "matlabExe", soft_executionTab_matlabExe_lineEdit->text() );


    jsonObject_noGUI.insert( "inputFiles", inputFiles );
    jsonObject_noGUI.insert( "covariates", covariates );
    jsonObject_noGUI.insert( "subjects", subjects );
    jsonObject_noGUI.insert( "settings", settings );
    jsonObject_noGUI.insert( "matlabSpecifications", matlabSpecifications );
    jsonObject.insert( "noGUIConfiguration", jsonObject_noGUI );

    QJsonDocument jsonDoc;
    jsonDoc.setObject( jsonObject );
    QFile exportedJSON( filename );
    exportedJSON.open( QIODevice::WriteOnly | QIODevice::Text );
    exportedJSON.write( jsonDoc.toJson( QJsonDocument::Indented ) );
    exportedJSON.flush();
    exportedJSON.close();
}



void FADTTSWindow::InitMenuBar()
{
    /** Load parameters (para_) from a .json file into the GUI interface **/
    connect( actionLoad_Para_Configuration, SIGNAL( triggered() ), SLOT( OnLoadParaConfiguration() ) );
    /** Save parameters (para_) from the GUI interface into a .json file **/
    connect( actionSave_Para_Configuration, SIGNAL( triggered() ), SLOT( OnSaveParaConfiguration() ) );

    /** Load software parameters (soft_) from a .json file into the GUI interface **/
    connect( actionLoad_Soft_Configuration, SIGNAL( triggered() ), SLOT( OnLoadSoftConfiguration() ) );
    /** Save software parameters (soft_) from the GUI interface into a .json file **/
    connect( actionSave_Soft_Configuration, SIGNAL( triggered() ), SLOT( OnSaveSoftConfiguration() ) );

    /** Save parameters (para_) and software parameters (soft_) from the GUI interface into a .json file for the FADTTSter --noGUI mode**/
    connect( actionSave_noGUI_Configuration, SIGNAL( triggered() ), SLOT( OnSaveNoGUIConfiguration() ) );

    connect( actionUser_Guide, SIGNAL( triggered() ), SLOT( OnDisplayUserGuide() ) );
    connect( actionAbout, SIGNAL( triggered() ), SLOT( OnDisplayAbout() ) );
}

void FADTTSWindow::InitInputTab()
{
    connect( inputTab_addMultipleInputFiles_pushButton, SIGNAL( clicked() ), this, SLOT( OnAddInputFiles() ) );

    /** Map of PushButtons to add each file separetely and
     *  SignalMapper to link them to the slot AddFile() **/
    m_inputTabAddInputFilePushButtonMap.insert( m_data.GetAxialDiffusivityIndex(), inputTab_addADFile_pushButton );
    m_inputTabAddInputFilePushButtonMap.insert( m_data.GetRadialDiffusivityIndex(), inputTab_addRDFile_pushButton );
    m_inputTabAddInputFilePushButtonMap.insert( m_data.GetMeanDiffusivityIndex(), inputTab_addMDFile_pushButton );
    m_inputTabAddInputFilePushButtonMap.insert( m_data.GetFractionalAnisotropyIndex(), inputTab_addFAFile_pushButton );
    m_inputTabAddInputFilePushButtonMap.insert( m_data.GetSubMatrixIndex(), inputTab_addSubMatrixFile_pushButton );
    QSignalMapper *signalMapperAddFile = new QSignalMapper( this );
    connect( signalMapperAddFile, SIGNAL( mapped( int ) ), this, SLOT( OnAddInputFile( int ) ) );
    for( int i = 0; i < m_data.GetDiffusionPropertiesIndices().size(); ++i )
    {
        connect( m_inputTabAddInputFilePushButtonMap[ m_data.GetDiffusionPropertiesIndices().at( i ) ], SIGNAL( clicked() ), signalMapperAddFile,SLOT(map() ) );
        signalMapperAddFile->setMapping( m_inputTabAddInputFilePushButtonMap[ m_data.GetDiffusionPropertiesIndices().at( i ) ], m_data.GetDiffusionPropertiesIndices().at( i ) );
    }

    /** Map of LineEdits where the file path of each file is set and
     *  SignalMapper to link them to the slot UpdateInputLineEdit() **/
    m_inputTabInputFileLineEditMap.insert( m_data.GetAxialDiffusivityIndex(), para_inputTab_adFile_lineEdit );
    m_inputTabInputFileLineEditMap.insert( m_data.GetRadialDiffusivityIndex(), para_inputTab_rdFile_lineEdit );
    m_inputTabInputFileLineEditMap.insert( m_data.GetMeanDiffusivityIndex(), para_inputTab_mdFile_lineEdit );
    m_inputTabInputFileLineEditMap.insert( m_data.GetFractionalAnisotropyIndex(), para_inputTab_faFile_lineEdit );
    m_inputTabInputFileLineEditMap.insert( m_data.GetSubMatrixIndex(), para_inputTab_subMatrixFile_lineEdit );
    QSignalMapper *signalMapperUpdateLineEdit = new QSignalMapper( this );
    connect( signalMapperUpdateLineEdit, SIGNAL( mapped( int ) ), this, SLOT( OnSettingInputFile( int ) ) );
    for( int i = 0; i < m_data.GetDiffusionPropertiesIndices().size(); ++i )
    {
        connect( m_inputTabInputFileLineEditMap[ m_data.GetDiffusionPropertiesIndices().at( i ) ], SIGNAL( textChanged( const QString& ) ), signalMapperUpdateLineEdit,SLOT(map() ) );
        signalMapperUpdateLineEdit->setMapping( m_inputTabInputFileLineEditMap[ m_data.GetDiffusionPropertiesIndices().at( i ) ], m_data.GetDiffusionPropertiesIndices().at( i ) );
    }

    /** Map of Labels to set the icon information of each file entered in a LineEdit **/
    m_inputTabIconLabelMap.insert( m_data.GetAxialDiffusivityIndex(), inputTab_iconADFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetRadialDiffusivityIndex(), inputTab_iconRDFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetMeanDiffusivityIndex(), inputTab_iconMDFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetFractionalAnisotropyIndex(), inputTab_iconFAFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetSubMatrixIndex(), inputTab_iconSubMatrixFile_label );

    m_editInputDialog = QSharedPointer< EditInputDialog >( new EditInputDialog( this ) );
    m_editInputDialog->SetData( &m_data );
    connect( m_editInputDialog.data(), SIGNAL( UpdateInputFile( const int&, const QString& ) ), this, SLOT( OnUpdatingInputFile( const int&, const QString& ) ) );
    connect( m_editInputDialog.data(), SIGNAL( UpdateSubjectColumnID( int ) ), this, SLOT( OnUpdatingSubjectColumnID( int ) ) );

    /** Map of PushButtons to edit the files and
     *  SignalMapper to link them to the slot EditFile() **/
    m_inputTabEditInputFilePushButtonMap.insert( m_data.GetAxialDiffusivityIndex(), inputTab_editADFile_pushButton );
    m_inputTabEditInputFilePushButtonMap.insert( m_data.GetRadialDiffusivityIndex(), inputTab_editRDFile_pushButton );
    m_inputTabEditInputFilePushButtonMap.insert( m_data.GetMeanDiffusivityIndex(), inputTab_editMDFile_pushButton );
    m_inputTabEditInputFilePushButtonMap.insert( m_data.GetFractionalAnisotropyIndex(), inputTab_editFAFile_pushButton );
    m_inputTabEditInputFilePushButtonMap.insert( m_data.GetSubMatrixIndex(), inputTab_editSubMatrixFile_pushButton );
    QSignalMapper *signalMapperEditFile = new QSignalMapper( this );
    connect( signalMapperEditFile, SIGNAL( mapped( int ) ), this, SLOT( OnEditInputFile( int ) ) );
    for( int i = 0; i < m_data.GetDiffusionPropertiesIndices().size(); ++i )
    {
        connect( m_inputTabEditInputFilePushButtonMap[ m_data.GetDiffusionPropertiesIndices().at( i ) ], SIGNAL( clicked() ), signalMapperEditFile, SLOT(map() ) );
        signalMapperEditFile->setMapping( m_inputTabEditInputFilePushButtonMap[ m_data.GetDiffusionPropertiesIndices().at( i ) ], m_data.GetDiffusionPropertiesIndices().at( i ) );
    }

    /** Map of Labels to edit the file information and
     *  SignalMapper to link them to the slot EditFile() **/
    m_inputFileInformationLabelMap.insert( m_data.GetAxialDiffusivityIndex(), inputTab_adFileInfo_label );
    m_inputFileInformationLabelMap.insert( m_data.GetRadialDiffusivityIndex(), inputTab_rdFileInfo_label );
    m_inputFileInformationLabelMap.insert( m_data.GetMeanDiffusivityIndex(), inputTab_mdFileInfo_label );
    m_inputFileInformationLabelMap.insert( m_data.GetFractionalAnisotropyIndex(), inputTab_faFileInfo_label );
    m_inputFileInformationLabelMap.insert( m_data.GetSubMatrixIndex(), inputTab_subMatrixFileInfo_label );

    m_covariateListWidget = new QListWidget();
    m_covariateListWidget = para_inputTab_covariates_listWidget;
    connect( m_covariateListWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( OnCovariateClicked( QListWidgetItem * ) ) );
    connect( inputTab_covariatesCheckAll_pushButton, SIGNAL( clicked() ), this, SLOT( OnCheckAllCovariates() ) );
    connect( inputTab_covariatesUncheckAll_pushButton, SIGNAL( clicked() ), this, SLOT( OnUnCheckAllCovariates() ) );

    DisplayFileInformation();

    m_afterEdition = false;
}

void FADTTSWindow::InitSubjectsTab()
{
    /*** Inputs Available ***/
    /** Map of CheckBoxes to select the files we want to work on and
     *  SignalMapper to link them to the slot SortSubjects() **/
    m_paramTabFileCheckBoxMap.insert( m_data.GetAxialDiffusivityIndex(), para_subjectTab_adFile_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetRadialDiffusivityIndex(), para_subjectTab_rdFile_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetMeanDiffusivityIndex(), para_subjectTab_mdFile_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetFractionalAnisotropyIndex(), para_subjectTab_faFile_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetSubMatrixIndex(), para_subjectTab_subMatrixFile_checkBox );
    QSignalMapper *signalMapperSelectFile = new QSignalMapper( this );
    connect( signalMapperSelectFile, SIGNAL( mapped( int ) ), this, SLOT( OnInputToggled() ) );
    for ( int i = 0; i < m_data.GetDiffusionPropertiesIndices().size(); ++i )
    {
        connect( m_paramTabFileCheckBoxMap[ m_data.GetDiffusionPropertiesIndices().at( i ) ], SIGNAL( toggled( bool ) ), signalMapperSelectFile,SLOT( map() ) );
        signalMapperSelectFile->setMapping( m_paramTabFileCheckBoxMap[ m_data.GetDiffusionPropertiesIndices().at( i ) ], m_data.GetDiffusionPropertiesIndices().at( i ) );
    }

    /*** QC Threshold ***/
    m_qcThresholdDialog = QSharedPointer< QCThresholdDialog >( new QCThresholdDialog( this ) );
    connect( subjectTab_applyQCThreshold_pushButton, SIGNAL( clicked() ), this, SLOT( OnApplyQCThreshold() ) );
    connect( m_qcThresholdDialog.data(), SIGNAL( ApplyQCThreshold( const QStringList&, const QStringList&, double, bool ) ), this, SLOT( OnQCThresholdApplied( const QStringList&, const QStringList&, double, bool ) ) );
    connect( m_qcThresholdDialog.data(), SIGNAL( NanSujects( const QStringList& ) ), this, SLOT( OnNanSujects( const QStringList& ) ) );

    /*** Subjects Lists ***/
    m_areSubjectsLoaded = false;

    m_matchedSubjectListWidget = new QListWidget();
    m_matchedSubjectListWidget = subjectTab_matchedSubjectsInformation_listWidget;
    connect( m_matchedSubjectListWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( OnSubjectClicked( QListWidgetItem * ) ) );
    connect( subjectTab_checkAllVisible_pushButton, SIGNAL( clicked() ), this, SLOT( OnCheckAllVisibleSubjects() ) );
    connect( subjectTab_unCheckAllVisible_pushButton, SIGNAL( clicked() ), this, SLOT( OnUnCheckAllVisibleSubjects() ) );

    m_unmatchedSubjectListWidget = new QListWidget();
    m_unmatchedSubjectListWidget = subjectTab_unmatchedSubjectsInformation_listWidget;

    /*** Search Subjects ***/
    m_caseSensitivity = Qt::CaseInsensitive;
    connect( subjectTab_search_lineEdit, SIGNAL( textEdited( const QString& ) ), this, SLOT( OnSearch() ) );
    connect( subjectTab_caseSensitive_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnSetCaseSensitivityToggled( bool ) ) );

    /*** Subjects File ***/
    m_subjectFileLineEdit = new QLineEdit();
    m_subjectFileLineEdit = para_subjectTab_subjectFile_lineEdit;
    connect( m_subjectFileLineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnSettingSubjectList( const QString&  ) ) );
    connect( subjectTab_loadSubjectFile_PushButton, SIGNAL( clicked() ), this, SLOT( OnLoadSubjectList() ) );
    connect( subjectTab_resetSubjectFile_pushButton, SIGNAL( clicked() ), this, SLOT( OnResetSubjectList() ) );
    connect( subjectTab_saveCheckedSubjects_pushButton, SIGNAL( clicked() ), this, SLOT( OnSaveCheckedSubjects() ) );

    OnInputToggled();
}

void FADTTSWindow::InitExecutionTab()
{
    /*** Settings ***/
    para_executionTab_nbrPermutations_spinBox->setMaximum( 2000 );

    /*** Output directory ***/
    connect( executionTab_outputDir_pushButton, SIGNAL( clicked() ), this, SLOT( OnBrowsingOutputDir() ) );
    connect( para_executionTab_outputDir_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnSettingOutputDir( const QString& ) ) );

    /*** Matlab Specification ***/
    m_isMatlabExeFound = false;

    connect( executionTab_mvcm_pushButton, SIGNAL( clicked() ), this, SLOT( OnBrowsingMVCMPath() ) );
    connect( para_executionTab_mvcm_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnSettingMVCMPath( const QString& ) ) );

    connect( soft_executionTab_runMatlab_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnRunMatlabToggled( bool ) ) );
    connect( executionTab_matlabExe_pushButton, SIGNAL( clicked() ), this, SLOT( OnBrowsingMatlabExe() ) );
    connect( soft_executionTab_matlabExe_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnSettingMatlabExe( const QString& ) ) );

    /*** Run ***/
    m_progressBar = new QProgressBar();
    m_progressBar = executionTab_progressBar;
    m_progressBar->setMinimum( 0 );
    m_progressBar->setMaximum( 0 );
    m_progressBar->hide();

    m_matlabThread = new MatlabThread();
    connect( m_matlabThread, SIGNAL( finished() ), this, SLOT( OnMatlabThreadFinished() ) );

    connect( executionTab_run_pushButton, SIGNAL( clicked() ), this, SLOT( OnRun() ) );
    connect( executionTab_stop_pushButton, SIGNAL( clicked() ), this, SLOT( OnStop() ) );
    executionTab_run_pushButton->setEnabled( true );
    executionTab_stop_pushButton->setEnabled( false );

    /*** Log ***/
    m_log = new Log;
    m_log->SetMatlabScript( m_matlabThread );
    connect( m_log, SIGNAL( UpdateLogActivity( const QString& ) ), this, SLOT( OnUpdatingLogActivity( const QString& ) ) );

    m_logWindow = new QTextEdit();
    m_logWindow = executionTab_log_textEdit;
    m_logWindow->setReadOnly( true );

    connect( executionTab_clearLog_pushButton, SIGNAL( clicked() ), this, SLOT( OnClearLog() ) );

    OnRunMatlabToggled( false );
}

void FADTTSWindow::InitPlottingTab()
{
    m_qvtkWidget = QSharedPointer< QVTKWidget >( plottingTab_plot_qvtkWidget );

    m_plot = new Plot();
    m_plot->SetQVTKWidget( m_qvtkWidget );
    connect( m_plot, SIGNAL( PlotsUsed( const QStringList& ) ), this, SLOT( OnSettingPlotsUsed( const QStringList& ) ) );
    connect( m_plot, SIGNAL( AllPropertiesUsed( const QMap< int, QString >& ) ), this, SLOT( OnSettingAllPropertiesUsed( const QMap< int, QString >& ) ) );
    connect( m_plot, SIGNAL( AllCovariatesUsed( const QMap< int, QString >& ) ), this, SLOT( OnSettingAllCovariatesUsed( const QMap< int, QString >& ) ) );
    connect( m_plot, SIGNAL( CovariatesAvailableForPlotting( const QMap< int, QString >& ) ), this, SLOT( OnUpdatingCovariatesAvailable( const QMap< int, QString >& ) ) );
    connect( m_plot, SIGNAL( LinesSelected( const QStringList& ) ), this, SLOT( OnSettingLinesSelected( const QStringList& ) ) );

    /*** Load/Set Data Tab ***/
    connect( plottingTab_loadSetDataTab_browsePlotDirectory_pushButton, SIGNAL( clicked() ), this, SLOT( OnBrowsingPlotDir() ) );
    connect( para_plottingTab_loadSetDataTab_browsePlotDirectory_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnSettingPlotDir( const QString& ) ) );
    connect( para_plottingTab_loadSetDataTab_fibername_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnSettingPlotFibername( const QString& ) ) );


    m_plotComboBox = new QComboBox();
    m_plotComboBox = plottingTab_loadSetDataTab_plotSelection_comboBox;
    connect( m_plotComboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( OnPlotSelection( const QString& ) ) );

    m_propertyComboBox = new QComboBox();
    m_propertyComboBox = plottingTab_loadSetDataTab_propertySelection_comboBox;
    connect( m_propertyComboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( OnPropertySelection( const QString& ) ) );

    m_covariateComboBox = new QComboBox();
    m_covariateComboBox = plottingTab_loadSetDataTab_covariateSelection_comboBox;
    connect( m_covariateComboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( OnCovariateSelection( const QString& ) ) );

    m_lineDisplayedListWidget = new QListWidget();
    m_lineDisplayedListWidget = plottingTab_loadSetDataTab_linesToDisplay_listWidget;
    connect( m_lineDisplayedListWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( OnLineForDisplayClicked( QListWidgetItem * ) ) );
    connect( plottingTab_loadSetDataTab_checkAllLinesToDisplay_pushButton, SIGNAL( clicked() ), this, SLOT( OnCheckAllLinesToDisplay() ) );
    connect( plottingTab_loadSetDataTab_uncheckAllLinesToDisplay_pushButton, SIGNAL( clicked() ), this, SLOT( OnUncheckAllToDisplay() ) );

    m_lineSelectedListWidget = new QListWidget();
    m_lineSelectedListWidget = plottingTab_loadSetDataTab_linesSelected_listWidget;


    /*** Title/Axis/Legend Tab ***/
    connect( plottingTab_titleAxisLegendTab_useDefaultTitle_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUseDefaultTitle( bool ) ) );
    connect( plottingTab_titleAxisLegendTab_useDefaultTitle_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingPlotTitle() ) );

    connect( plottingTab_titleAxisLegendTab_titleName_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnUpdatingPlotTitle() ) );
    connect( plottingTab_titleAxisLegendTab_titleBold_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingPlotTitle() ) );
    connect( plottingTab_titleAxisLegendTab_titleItalic_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingPlotTitle() ) );
    connect( plottingTab_titleAxisLegendTab_titleSize_doubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnUpdatingPlotTitle() ) );

    connect( plottingTab_titleAxisLegendTab_gridOn_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingPlotGrid( bool ) ) );
    connect( plottingTab_titleAxisLegendTab_scientificNotation_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingAbscissaNotation( bool ) ) );

    connect( plottingTab_titleAxisLegendTab_useDefaultAxis_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUseDefaultdAxis( bool ) ) );
    connect( plottingTab_titleAxisLegendTab_useDefaultAxis_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingPlotAxis() ) );

    connect( plottingTab_titleAxisLegendTab_axisLabelSize_doubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnUpdatingPlotAxis() ) );
    connect( plottingTab_titleAxisLegendTab_xName_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnUpdatingPlotAxis() ) );
    connect( plottingTab_titleAxisLegendTab_yName_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnUpdatingPlotAxis() ) );
    connect( plottingTab_titleAxisLegendTab_axisNameSize_doubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnUpdatingPlotAxis() ) );
    connect( plottingTab_titleAxisLegendTab_axisBold_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingPlotAxis() ) );
    connect( plottingTab_titleAxisLegendTab_axisItalic_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingPlotAxis() ) );

    connect( plottingTab_titleAxisLegendTab_yMax_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnYMaxToggled( bool ) ) );
    connect( plottingTab_titleAxisLegendTab_yMax_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingPlotAxis() ) );
    plottingTab_titleAxisLegendTab_yMax_checkBox->setChecked( false );

    connect( plottingTab_titleAxisLegendTab_yMin_doubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnYMinValueChanged( double ) ) );
    connect( plottingTab_titleAxisLegendTab_yMin_doubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnUpdatingPlotAxis() ) );

    connect( plottingTab_titleAxisLegendTab_yMin_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnYMinToggled( bool ) ) );
    connect( plottingTab_titleAxisLegendTab_yMin_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingPlotAxis() ) );
    plottingTab_titleAxisLegendTab_yMin_checkBox->setChecked( false );

    connect( plottingTab_titleAxisLegendTab_yMax_doubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnYMaxValueChanged( double ) ) );
    connect( plottingTab_titleAxisLegendTab_yMax_doubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnUpdatingPlotAxis() ) );

    connect( plottingTab_titleAxisLegendTab_showLegend_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUsingLegend( bool ) ) );
    connect( plottingTab_titleAxisLegendTab_legendPosition_comboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( OnUpdatingLegend( const QString& ) ) );


    /*** Edition Tab ***/
    connect( plottingTab_editionTab_pvalueThreshold_doubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnUpdatingPvalueThreshold( double ) ) );

    connect( plottingTab_editionTab_lineWidth_doubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnUpdatingLineWidth( double ) ) );
    connect( plottingTab_editionTab_selectedLineColor_comboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( OnUpdatingSelectedColorLine( const QString& ) ) );
    SetColorsComboBox( plottingTab_editionTab_selectedLineColor_comboBox );
    plottingTab_editionTab_selectedLineColor_comboBox->setCurrentText( "Red" );

    connect( plottingTab_editionTab_markerType_comboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( OnUpdatingMarkerType( const QString& ) ) );
    connect( plottingTab_editionTab_markerSize_doubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnUpdatingMarkerSize( double ) ) );
    connect( plottingTab_editionTab_binaryBetas_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnUpdatingBinaryBetas( bool ) ) );


    connect( plottingTab_displayPlot_pushButton, SIGNAL( clicked() ), this, SLOT( OnDisplayPlot() ) );
    connect( plottingTab_resetPlot_pushButton, SIGNAL( clicked() ), this, SLOT( OnResetPlot() ) );
    connect( plottingTab_savePlot_pushButton, SIGNAL( clicked() ), m_plot, SLOT( OnSavePlot() ) );
    connect( plottingTab_loadPlotSettings_pushButton, SIGNAL( clicked() ), this, SLOT( OnLoadPlotSettings() ) );
    connect( plottingTab_savePlotSettings_pushButton, SIGNAL( clicked() ), this, SLOT( OnSavePlotSettings() ) );

    SetPlotTab();
}

void FADTTSWindow::InitFADTTSWindow()
{
    m_data.InitData();

    /** Initialization of the menu bar and all FADTTSter tabs **/
    InitMenuBar();
    InitInputTab();
    InitSubjectsTab();
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
    m_data.ClearData( diffusionPropertyIndex );

    QLineEdit *lineEdit = m_inputTabInputFileLineEditMap[ diffusionPropertyIndex ];
    QString filePath = lineEdit->text();
    QFile file( filePath );

    if( filePath.isEmpty() )
    {
        m_inputTabIconLabelMap[ diffusionPropertyIndex ]->clear();
    }
    else
    {
        if( !file.open( QIODevice::ReadOnly ) )
        {
            DisplayInputLineEditIcon( diffusionPropertyIndex, m_koPixmap );
        }
        else
        {
            file.close();
            QList< QStringList > fileData = m_processing.GetDataFromFile( filePath );
            if( m_processing.IsMatrixDimensionOK( fileData ) )
            {
                if( !m_processing.AreDuplicatesFound( fileData ) )
                {
                    DisplayInputLineEditIcon( diffusionPropertyIndex, m_okPixmap );

                    m_data.SetFilename( diffusionPropertyIndex ) = filePath;
                    m_data.SetFileData( diffusionPropertyIndex ) = fileData;

                    if( diffusionPropertyIndex == m_data.GetFractionalAnisotropyIndex() )
                    {
                        AddAtlas( fileData );
                    }
                }
                else
                {
                    DisplayInputLineEditIcon( diffusionPropertyIndex, m_warningPixmap );
                    m_data.SetFileData( diffusionPropertyIndex ) = fileData;
                }
            }
            else
            {
                DisplayInputLineEditIcon( diffusionPropertyIndex, m_koPixmap );

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
        if( !m_afterEdition )
        {
            m_editInputDialog->ResetSubjectColumnID(); /** By default Subjects are on the 1st column. **/
        }
        else
        {
            m_afterEdition = false;
        }
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
    if( m_data.GetFileData( diffusionPropertyIndex ).isEmpty() )
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
    m_afterEdition = true;
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
    if( !visible )
    {
        m_covariateListWidget->clear();
    }
    m_covariateListWidget->setHidden( !visible );
    inputTab_covariatesCheckAll_pushButton->setHidden( !visible );
    inputTab_covariatesUncheckAll_pushButton->setHidden( !visible );
    inputTab_covariatesInformation_label->setHidden( !visible );
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

        if( m_covariateListWidget->count() != 0 )
        {
            m_covariateListWidget->setMaximumHeight( m_covariateListWidget->sizeHintForRow( 0 ) * ( m_covariateListWidget->count() + 1 ) );
        }
    }
}

void FADTTSWindow::AddAtlas( const QList< QStringList >& fileData )
{
    if( fileData.first().last().contains( "atlas", Qt::CaseInsensitive ) )
    {
        QStringList atlas;
        for( int i = 1; i < fileData.size(); i++ )
        {
            atlas.append( fileData.at( i ).last() );
        }
        m_data.SetAtlas() = atlas;
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
            fileInformation.append( tr( qPrintable( "Number of covariates: <i>" + QString::number( m_data.GetCovariates().size()-1 )  + " ( + Intercept )</i>" ) ) );
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
    QString filePath = m_data.SetFilename( diffusionPropertyIndex );

    if( !fileData.isEmpty() && !filePath.isEmpty() )
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
    inputTab_subjectColumnID_label->setText( !m_data.GetFileData( m_data.GetSubMatrixIndex() ).isEmpty() ?
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
    QList< QStringList > rawData = m_data.GetFileData( m_data.GetFractionalAnisotropyIndex() );
    QStringList atlas = para_subjectTab_qcThresoldOnAtlas_radioButton->isChecked() ? m_data.GetAtlas() : QStringList();

    if( matchedSubjects.isEmpty() || ( para_subjectTab_qcThresoldOnAtlas_radioButton->isChecked() && atlas.isEmpty() ) )
    {
        QString warningMessage = "<b>Unable to apply QC Threshold</b><br>";
        if( matchedSubjects.isEmpty() )
        {
            warningMessage.append( QString( "<br>- No subjects selected<br>" ) );
        }
        if( para_subjectTab_qcThresoldOnAtlas_radioButton->isChecked() && atlas.isEmpty() )
        {
            warningMessage.append( QString( "<br>- No atlas detected" ) );
        }
        WarningPopUp( warningMessage );
    }

    if( !matchedSubjects.isEmpty() && !rawData.isEmpty() && ( para_subjectTab_qcThresoldOnAtlas_radioButton->isChecked() ? !atlas.isEmpty() : true ) )
    {
        bool initDiaolog = m_qcThresholdDialog->InitPlot( rawData, atlas, matchedSubjects, para_subjectTab_qcThreshold_doubleSpinBox->value() );

        if( initDiaolog )
        {
            m_qcThresholdDialog->setModal( true );
            m_qcThresholdDialog->setWindowTitle( tr( "Apply QC Threshold" ) );
            m_qcThresholdDialog->exec();
        }
    }
}

void FADTTSWindow::OnQCThresholdApplied( const QStringList& subjectsCorrelated, const QStringList& subjectsNotCorrelated, double qcThreshold, bool windowClosed )
{
    if( windowClosed )
    {
        m_failedQCThresholdSubjects.append( subjectsNotCorrelated );
        m_failedQCThresholdSubjects.removeDuplicates();
        m_failedQCThresholdSubjects.sort();
    }

    para_subjectTab_qcThreshold_doubleSpinBox->setValue( qcThreshold );
    m_qcThreshold = qcThreshold;

    for( int i = 0; i < m_matchedSubjectListWidget->count(); i++ )
    {
        QListWidgetItem *currentItem = m_matchedSubjectListWidget->item( i );
        if( subjectsNotCorrelated.contains( currentItem->text() ) )
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

void FADTTSWindow::OnNanSujects( const QStringList& nanSubjects )
{
    for( int i = 0; i < m_matchedSubjectListWidget->count(); i++ )
    {
        QListWidgetItem *currentItem = m_matchedSubjectListWidget->item( i );
        if( nanSubjects.contains( currentItem->text() ) )
        {
            currentItem->setCheckState( Qt::Unchecked );
            currentItem->setBackgroundColor( m_grey );
        }
    }

    DisplayNbrSubjectSelected();
}


void FADTTSWindow::OnSearch()
{
    int nbrSubjectFind = SearchSubjects( m_matchedSubjectListWidget ) + SearchSubjects( m_unmatchedSubjectListWidget );
    subjectTab_nbrFound_label->
            setText( !subjectTab_search_lineEdit->text().isEmpty() ? "found " + QString::number( nbrSubjectFind ) : "" );
}

void FADTTSWindow::OnInputToggled()
{
    SetSelectedInputFiles();
    UpdateSubjectList();

    bool isEnable = m_paramTabFileCheckBoxMap[ m_data.GetFractionalAnisotropyIndex() ]->isChecked() && m_paramTabFileCheckBoxMap[ m_data.GetFractionalAnisotropyIndex() ]->isEnabled();
    subjectTab_qcThreshold_widget->setEnabled( isEnable );
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
        UseSavedSubjectList( filePath );
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
    QLabel *label = subjectTab_iconLoadSubjectFile_label;
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
        bool isSelected = m_paramTabFileCheckBoxMap[ diffusionPropertyIndex ]->isChecked();
        if( isSelected )
        {
            m_propertySelected.insert( diffusionPropertyIndex, m_data.GetDiffusionPropertyName( diffusionPropertyIndex ) );
            m_selectedFiles.insert( diffusionPropertyIndex, m_data.GetFilename( diffusionPropertyIndex ) );
        }
    }
}

void FADTTSWindow::UpdateAvailableDiffusionProperties( int diffusionPropertyIndex )
{
    QCheckBox *currentCheckBox = m_paramTabFileCheckBoxMap.value( diffusionPropertyIndex );
    int nbrRows = m_data.GetNbrSubjects( diffusionPropertyIndex );
    int nbrColumns = m_data.GetNbrColumns( diffusionPropertyIndex );
    int nbrSubjects = m_data.GetNbrSubjects( diffusionPropertyIndex );

    bool isDefine = !( ( nbrRows == 0 ) | ( nbrColumns == 0 ) | ( nbrSubjects == 0 ) );
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
    subjectTab_matchedSubjectsInformation_label->clear();
    subjectTab_unmatchedSubjectsInformation_label->clear();

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
        subjectTab_matchedSubjectsInformation_label->setText( textMatchedSubjectsInformation );
        subjectTab_unmatchedSubjectsInformation_label->setText( textUnmatchedSubjectsInformation );
    }
    else
    {
        subjectTab_matchedSubjectsInformation_label->clear();
        subjectTab_unmatchedSubjectsInformation_label->clear();
    }
}

void FADTTSWindow::DisplayNbrSubjectSelected()
{
    subjectTab_checkedSubjectsInformation_label->clear();
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

        subjectTab_checkedSubjectsInformation_label->setText( tr( qPrintable( QString::number( nbrSubjectsSelected ) + "/" + QString::number( m_matchedSubjectListWidget->count() ) + " subjects selected") ) );
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
    QLineEdit *lineEdit = subjectTab_search_lineEdit;
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

void FADTTSWindow::UseSavedSubjectList( QString filePath )
{
    bool use = false;
    QMessageBox::StandardButton questionBox =
            QMessageBox::question( this, tr( "Use Saved Subject List" ), tr( qPrintable( "Do you want to use the subject list<br><i>" + filePath + "</i><br>as new reference list?" ) ),
                                   QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes );
    switch( questionBox )
    {
    case QMessageBox::No:
        use = false;
        break;
    case QMessageBox::Yes:
    {
        use = true;
        break;
    }
    default:
        use = false;
        break;
    }

    if( use )
    {
        QString previousFilePath = m_subjectFileLineEdit->text();
        m_subjectFileLineEdit->setText( filePath );
        if( previousFilePath == filePath )
        {
            /** If filePath does not change, an update is needed to display the right subject list **/
            UpdateSubjectList();
        }
    }
}

void FADTTSWindow::SaveCheckedSubjects( QString filePath )
{
    QFile exportedTXT( filePath );
    exportedTXT.open( QIODevice::WriteOnly );
    QTextStream ts( &exportedTXT );

    for( int i = 0; i < m_matchedSubjectListWidget->count(); i++ )
    {
        if( m_matchedSubjectListWidget->item( i )->checkState() == Qt::Checked && !m_matchedSubjectListWidget->item( i )->isHidden() )
        {
            ts << m_matchedSubjectListWidget->item( i )->text() <<  endl;
        }
    }
    exportedTXT.flush();
    exportedTXT.close();
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
    QLineEdit *lineEdit = para_executionTab_outputDir_lineEdit;
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
    QLabel *label = executionTab_iconOutputDir_label;
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
    QLineEdit *lineEdit = para_executionTab_mvcm_lineEdit;
    QString filePath = lineEdit->text();
    QDir dir = filePath;

    QString dirPath = ( dir.exists() && !filePath.isEmpty() ) ?
                QFileDialog::getExistingDirectory( this, tr( "Choose FADTTS Directory" ), dir.absolutePath(), QFileDialog::ShowDirsOnly ) :
                QFileDialog::getExistingDirectory( this, tr( "Choose FADTTS Directory" ), m_mvcmPath, QFileDialog::ShowDirsOnly );
    if( !dirPath.isEmpty() )
    {
        lineEdit->setText( dirPath );
    }
}

void FADTTSWindow::OnSettingMVCMPath( const QString& path )
{
    QLabel *label = executionTab_iconMVCM_label;
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
    QLineEdit *lineEdit = soft_executionTab_matlabExe_lineEdit;
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
    QLabel *label = executionTab_iconMatlabExe_label;
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


void FADTTSWindow::OnRunMatlabToggled( bool isChecked )
{
    executionTab_matlabExe_pushButton->setEnabled( isChecked );
    soft_executionTab_matlabExe_lineEdit->setEnabled( isChecked );
    executionTab_iconMatlabExe_label->setEnabled( isChecked );

    m_matlabThread->SetRunMatlab() = isChecked;
}


void FADTTSWindow::OnRun()
{ 
    SetSelectedCovariates();
    SetFibername();

    if( canFADTTSterBeRun() )
    {
        OnClearLog();

        executionTab_run_pushButton->setEnabled( false );
        executionTab_stop_pushButton->setEnabled( true );
        m_progressBar->show();

        SetMatlabScript();

        m_matlabThread->start();
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
            m_log->AddText( "\nWarning! Thread terminated by user before completed matlab script.\n" );
            m_matlabThread->terminate();
            m_log->CloseLogFile();
            m_logWindow->insertPlainText( "\nWarning! Thread terminated by user before completed matlab script.\n");
            break;
        }
        default:
            break;
        }
    }
}

void FADTTSWindow::OnUpdatingLogActivity( const QString& LogActivity )
{
    QScrollBar *scrollBar = m_logWindow->verticalScrollBar();
    m_logWindow->insertPlainText( LogActivity );
    scrollBar->setValue( scrollBar->maximum() );
}

void FADTTSWindow::OnClearLog()
{
    m_logWindow->clear();
}


void FADTTSWindow::OnMatlabThreadFinished()
{
    executionTab_run_pushButton->setEnabled( true );
    executionTab_stop_pushButton->setEnabled( false );
    m_log->CloseLogFile();
    m_progressBar->hide();
}


/*********************** Private function ***********************/
void FADTTSWindow::GenerateFailedQCThresholdSubjectFile( QString outputDir )
{
    QFile failedQCThresholdSubjectFile( outputDir + "/" + para_executionTab_fiberName_lineEdit->text() + "_subjectList_FAILED_QCThreshold.txt" );

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
    QFile selectedSubjectFile( outputDir + "/" + para_executionTab_fiberName_lineEdit->text() + "_subjectList.txt" );
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
    m_fibername = para_executionTab_fiberName_lineEdit->text();
}

bool FADTTSWindow::IsAtLeastOneSubjectSelected()
{
    for( int i = 0; i < m_matchedSubjectListWidget->count(); i++ )
    {
        if( m_matchedSubjectListWidget->item( i )->checkState() == Qt::Checked )
        {
            return true;
        }
    }

    return false;
}

bool FADTTSWindow::canFADTTSterBeRun()
{
    bool atLeastOneDiffusionPropertyEnabled = ( m_paramTabFileCheckBoxMap.value( m_data.GetAxialDiffusivityIndex() )->isEnabled() ||
                                                m_paramTabFileCheckBoxMap.value( m_data.GetRadialDiffusivityIndex() )->isEnabled() ||
                                                m_paramTabFileCheckBoxMap.value( m_data.GetMeanDiffusivityIndex() )->isEnabled() ||
                                                m_paramTabFileCheckBoxMap.value( m_data.GetFractionalAnisotropyIndex() )->isEnabled() );
    bool subMatrixEnabled = m_paramTabFileCheckBoxMap.value( m_data.GetSubMatrixIndex() )->isEnabled();
    bool atLeastOneCovariateChecked = m_selectedCovariates.count() != 0;

    bool atLeastOneDiffusionPropertyChecked = ( m_paramTabFileCheckBoxMap.value( m_data.GetAxialDiffusivityIndex() )->isChecked() ||
                                                m_paramTabFileCheckBoxMap.value( m_data.GetMeanDiffusivityIndex() )->isChecked() ||
                                                m_paramTabFileCheckBoxMap.value( m_data.GetRadialDiffusivityIndex() )->isChecked() ||
                                                m_paramTabFileCheckBoxMap.value( m_data.GetFractionalAnisotropyIndex() )->isChecked() );
    bool subMatrixChecked = m_paramTabFileCheckBoxMap.value( m_data.GetSubMatrixIndex() )->isChecked();
    bool atLeastOneSubjectSelected = IsAtLeastOneSubjectSelected();

    bool fiberNameProvided = !m_fibername.isEmpty();
    bool mvcmPathSpecified = !para_executionTab_mvcm_lineEdit->text().isEmpty();
    bool matlabExeSpecified = !soft_executionTab_runMatlab_checkBox->isChecked() ? true : m_isMatlabExeFound;

    if( !atLeastOneDiffusionPropertyEnabled || !subMatrixEnabled || !atLeastOneCovariateChecked ||
            !atLeastOneDiffusionPropertyChecked || !subMatrixChecked || !atLeastOneSubjectSelected ||
            !fiberNameProvided || !mvcmPathSpecified || !matlabExeSpecified )
    {
        QString warningText = "<b>FADTTSter will not be executed for the following reason(s):</b><br>";
        if( !atLeastOneDiffusionPropertyEnabled || !subMatrixEnabled || !atLeastOneCovariateChecked )
        {
            warningText.append( "Inputs Tab<br>" );
            if( !atLeastOneDiffusionPropertyEnabled )
            {
                warningText.append( "- Provide at least 1 diffusion profile file (AD, RD, MR or FA)<br>" );
            }
            if( !subMatrixEnabled )
            {
                warningText.append( "- No covariate file provided<br>" );
            }
            else
            {
                if( subMatrixChecked )
                {
                    if( !atLeastOneCovariateChecked )
                    {
                        warningText.append( "- Select at least 1 covariate<br>" );
                    }
                }
            }
        }
        if( atLeastOneDiffusionPropertyEnabled || subMatrixEnabled )
        {
            if( ( !atLeastOneDiffusionPropertyChecked && atLeastOneDiffusionPropertyEnabled ) || ( !subMatrixChecked && subMatrixEnabled ) ||
                    ( ( atLeastOneDiffusionPropertyChecked || subMatrixChecked ) || !atLeastOneSubjectSelected ) )
            {
                warningText.append( "Subjects Tab<br>" );
                if( !atLeastOneDiffusionPropertyChecked && atLeastOneDiffusionPropertyEnabled )
                {
                    warningText.append( "- Select at least 1 diffusion profile (AD, RD, MR or FA)<br>" );
                }
                if( !subMatrixChecked && subMatrixEnabled )
                {
                    warningText.append( "- Covariate file not selected<br>" );
                }
                if( ( atLeastOneDiffusionPropertyChecked || subMatrixChecked ) || !atLeastOneSubjectSelected )
                {
                    warningText.append( "- Select at least 1 subject<br>" );
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

void FADTTSWindow::SetMatlabScript()
{
    QString outputDir = m_data.GetOutputDir() + "/FADTTSter_" + m_fibername;
    QDir().mkpath( outputDir );

    QMap< int, QString > matlabInputFiles = m_processing.GenerateMatlabInputs( outputDir,m_fibername, m_selectedFiles, m_propertySelected, m_selectedCovariates,
                                                                               m_data.GetSubjectColumnID(), GenerateSelectedSubjectFile( outputDir ) );

    m_matlabThread->InitMatlabScript( outputDir, "FADTTSterAnalysis_" + m_fibername + "_" + QString::number( para_executionTab_nbrPermutations_spinBox->value() ) + "perm.m" );
    m_matlabThread->SetHeader();
    m_matlabThread->SetMVCMPath( para_executionTab_mvcm_lineEdit->text() );
    m_matlabThread->SetFiberName( m_fibername );
    m_matlabThread->SetDiffusionProperties( m_propertySelected.values() );
    m_matlabThread->SetNbrPermutation( para_executionTab_nbrPermutations_spinBox->value() );
    m_matlabThread->SetCovariates( m_selectedCovariates );
    m_matlabThread->SetInputFiles( matlabInputFiles );
    m_matlabThread->SetOmnibus( para_executionTab_omnibus_checkBox->isChecked() );
    m_matlabThread->SetPostHoc( para_executionTab_postHoc_checkBox->isChecked() );
    m_matlabThread->SetConfidenceBandsThreshold( para_executionTab_confidenceBandsThreshold_doubleSpinBox->value() );
    m_matlabThread->SetPvalueThreshold( para_executionTab_pvalueThreshold_doubleSpinBox->value() );

    m_log->SetLogFile( outputDir, m_fibername );
    m_log->SetFileWatcher();
    m_log->InitLog( outputDir, m_fibername, matlabInputFiles, m_selectedCovariates, m_loadedSubjects, m_subjectFileLineEdit->text(), m_nbrSelectedSubjects,
                    m_failedQCThresholdSubjects, m_qcThreshold, para_executionTab_nbrPermutations_spinBox->value(), para_executionTab_confidenceBandsThreshold_doubleSpinBox->value(),
                    para_executionTab_pvalueThreshold_doubleSpinBox->value(), para_executionTab_omnibus_checkBox->isChecked(), para_executionTab_postHoc_checkBox->isChecked(),
                    para_executionTab_mvcm_lineEdit->text(), soft_executionTab_runMatlab_checkBox->isChecked(), soft_executionTab_matlabExe_lineEdit->text() );

    SaveParaConfiguration( QString( outputDir + "/configuration_para_" + m_fibername + ".json" ) );
    SaveSoftConfiguration( QString( outputDir + "/configuration_soft_" + m_fibername + ".json" ) );
    SaveNoGUIConfiguration( QString( outputDir + "/configuration_noGUI_" + m_fibername + ".json" ) );
}


/****************************************************************/
/************************ Plotting  tab *************************/
/****************************************************************/

/***********************  Private  slots  ***********************/
void FADTTSWindow::OnBrowsingPlotDir()
{
    QLineEdit *lineEdit = para_plottingTab_loadSetDataTab_browsePlotDirectory_lineEdit;
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
    QLabel *browsePlotDirIcon = plottingTab_loadSetDataTab_browsePlotDirectoryIcon_label;
    QLabel *plotDirInfo = plottingTab_loadSetDataTab_plotDirectoryInfo_label;
    QLabel *plotDirIcon = plottingTab_loadSetDataTab_plotDirectoryIcon_label;
    QLabel *fibernameIcon = plottingTab_loadSetDataTab_fibernameIcon_label;

    if( !path.isEmpty() )
    {
        bool pathExists = QDir( path ).exists();

        DisplayIcon( browsePlotDirIcon, pathExists ? m_okPixmap : m_koPixmap );

        if( pathExists )
        {
            QString containingFolder = QDir( path ).dirName();
            if( containingFolder.contains( "FADTTSter_" ) )
            {
                para_plottingTab_loadSetDataTab_fibername_lineEdit->setText( containingFolder.remove( "FADTTSter_" ) );
            }
            else
            {
                para_plottingTab_loadSetDataTab_fibername_lineEdit->clear();
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
    QLabel *plotDirInfo = plottingTab_loadSetDataTab_plotDirectoryInfo_label;
    QLabel *fibernameIcon = plottingTab_loadSetDataTab_fibernameIcon_label;

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
            plotSelected == "Omnibus Betas with Confidence Bands" ||
            plotSelected == "Post-Hoc FDR Significant Betas on Average Raw Data" )
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

    m_plotSelected = plotSelected;
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
    ClickLineForDisplay( item );

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


void FADTTSWindow::OnUseDefaultTitle( bool checkState )
{
    plottingTab_titleAxisLegendTab_titleName_lineEdit->setEnabled( !checkState );
    plottingTab_titleAxisLegendTab_titleName_label->setEnabled( !checkState );
}

void FADTTSWindow::OnUpdatingPlotTitle()
{
    UpdatePlotTitle();
}


void FADTTSWindow::OnUpdatingPlotGrid( bool checkState )
{
    m_plot->UpdateGrid( checkState );
}

void FADTTSWindow::OnUpdatingAbscissaNotation( bool checkState )
{
    m_plot->UpdateAbscissaNotation( checkState );
}

void FADTTSWindow::OnUseDefaultdAxis( bool checkState )
{
    plottingTab_titleAxisLegendTab_xName_label->setEnabled( !checkState );
    plottingTab_titleAxisLegendTab_xName_lineEdit->setEnabled( !checkState );
    plottingTab_titleAxisLegendTab_yName_label->setEnabled( !checkState );
    plottingTab_titleAxisLegendTab_yName_lineEdit->setEnabled( !checkState );
//    plottingTab_titleAxisLegendTab_yMin_doubleSpinBox->setEnabled( checkState && plottingTab_titleAxisLegendTab_yMin_checkBox->isChecked() );
//    plottingTab_titleAxisLegendTab_yMax_doubleSpinBox->setEnabled( checkState && plottingTab_titleAxisLegendTab_yMax_checkBox->isChecked() );
}

void FADTTSWindow::OnYMinToggled( bool checkState )
{
    QDoubleSpinBox* yMinDoubleSpinBox = plottingTab_titleAxisLegendTab_yMin_doubleSpinBox;
    QDoubleSpinBox* yMaxDoubleSpinBox = plottingTab_titleAxisLegendTab_yMax_doubleSpinBox;

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
    QDoubleSpinBox* yMinDoubleSpinBox = plottingTab_titleAxisLegendTab_yMin_doubleSpinBox;
    QDoubleSpinBox* yMaxDoubleSpinBox = plottingTab_titleAxisLegendTab_yMax_doubleSpinBox;

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
    plottingTab_titleAxisLegendTab_yMax_doubleSpinBox->setMinimum( yMinValue );
}

void FADTTSWindow::OnYMaxValueChanged( double yMaxValue )
{
    plottingTab_titleAxisLegendTab_yMin_doubleSpinBox->setMaximum( yMaxValue );
}

void FADTTSWindow::OnUpdatingPlotAxis()
{
    UpdatePlotAxis();
}


void FADTTSWindow::OnUsingLegend( bool checkState )
{
    m_plot->SetLegend( checkState, plottingTab_titleAxisLegendTab_legendPosition_comboBox->currentText() );
}

void FADTTSWindow::OnUpdatingLegend( const QString& legendPosition )
{
    m_plot->SetLegend( plottingTab_titleAxisLegendTab_showLegend_checkBox->isChecked(), legendPosition );
}


void FADTTSWindow::OnUpdatingPvalueThreshold( double pvalueThreshold )
{
    m_plot->UpdatePvalueThreshold( pvalueThreshold );
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

void FADTTSWindow::OnUpdatingBinaryBetas( bool checkState )
{
    m_plot->UpdateBinaryBetas( checkState );
}


void FADTTSWindow::OnDisplayPlot()
{
    m_plot->ClearPlot();
    m_lineSelectedListWidget->hide();

    if( m_plotSelected == "Raw Betas by Covariates" ||
            m_plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" )
    {
        m_areLinesForDisplayProperties = true;
    }
    else
    {
        if(  m_plotSelected == "Raw Betas by Properties" ||
             m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
             m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" ||
             m_plotSelected == "Omnibus Local pvalues" ||
             m_plotSelected == "Omnibus FDR Local pvalues" )
        {
            m_areLinesForDisplayProperties = false;
        }
    }

    EditCovariatesNames();
    UpdatePlot();

    bool isPlotDisplayed = m_plot->DisplayPlot();

//    UpdatePlot();
    plottingTab_savePlot_pushButton->setEnabled( isPlotDisplayed );
    plottingTab_resetPlot_pushButton->setEnabled( isPlotDisplayed );

    bool isSelectionLinesDisplayedVisible = isPlotDisplayed && ( m_plotSelected == "Raw Betas by Properties" ||
                                                                 m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
                                                                 m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" ||
                                                                 m_plotSelected == "Raw Betas by Covariates" ||
                                                                 m_plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
                                                                 m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" ||
                                                                 m_plotSelected == "Omnibus Local pvalues" ||
                                                                 m_plotSelected == "Omnibus FDR Local pvalues" ||
                                                                 m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" );
    SetSelectionLinesDisplayedVisible( isSelectionLinesDisplayedVisible );
}

void FADTTSWindow::OnResetPlot()
{
    m_plot->ClearPlot();

    m_lineSelectedListWidget->hide();
    SetSelectionLinesDisplayedVisible( false );

    plottingTab_savePlot_pushButton->setEnabled( false );
    plottingTab_resetPlot_pushButton->setEnabled( false );
}

void FADTTSWindow::OnLoadPlotSettings()
{
    QString filename = QFileDialog::getOpenFileName( this , tr( "Load Plot Settings" ) , "" , tr( ".json( *.json ) ;; .*( * )" ) );
    if( !filename.isEmpty() )
    {
        LoadPlotSettings( filename );
        EditCovariatesNames();
    }
}

void FADTTSWindow::OnSavePlotSettings()
{
    QString filePath = QFileDialog::getSaveFileName( this, tr( "Save Plot Settings as ..." ), "newPlotSettings.json", tr( ".json( *.json ) ;; .*( * )" ) );
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
    plottingTab_displayPlot_pushButton->setEnabled( isPlotSelected );

    m_propertyComboBox->setEnabled( propertySelectionAvailable );
    plottingTab_loadSetDataTab_propertySelection_label->setEnabled( propertySelectionAvailable );

    m_covariateComboBox->setEnabled( covariateSelectionAvailable );
    plottingTab_loadSetDataTab_covariateSelection_label->setEnabled( covariateSelectionAvailable );
}

void FADTTSWindow::AddLinesForDisplay()
{
    m_lineDisplayedListWidget->clear();
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

void FADTTSWindow::ClickLineForDisplay( QListWidgetItem *item )
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

void FADTTSWindow::SetSelectionLinesDisplayedVisible( bool isVisible )
{
    plottingTab_loadSetDataTab_linesToDisplay_label->setHidden( !isVisible );
    m_lineDisplayedListWidget->setHidden( !isVisible );
    plottingTab_loadSetDataTab_checkAllLinesToDisplay_pushButton->setHidden( !isVisible );
    plottingTab_loadSetDataTab_uncheckAllLinesToDisplay_pushButton->setHidden( !isVisible );
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

    AddLinesForDisplay();
}


void FADTTSWindow::UpdatePlotAxis()
{
    if( plottingTab_titleAxisLegendTab_useDefaultAxis_checkBox->isChecked() )
    {
        m_plot->SetDefaultAxis( plottingTab_titleAxisLegendTab_axisLabelSize_doubleSpinBox->value(),
                                plottingTab_titleAxisLegendTab_axisNameSize_doubleSpinBox->value(),
                                plottingTab_titleAxisLegendTab_axisBold_checkBox->isChecked(),
                                plottingTab_titleAxisLegendTab_axisItalic_checkBox->isChecked(),
                                plottingTab_titleAxisLegendTab_yMin_checkBox->isChecked(),
                                plottingTab_titleAxisLegendTab_yMin_doubleSpinBox->value(),
                                plottingTab_titleAxisLegendTab_yMax_checkBox->isChecked(),
                                plottingTab_titleAxisLegendTab_yMax_doubleSpinBox->value() );
    }
    else
    {
        m_plot->SetCustomizedAxis( plottingTab_titleAxisLegendTab_axisLabelSize_doubleSpinBox->value(),
                                           plottingTab_titleAxisLegendTab_xName_lineEdit->text(),
                                           plottingTab_titleAxisLegendTab_yName_lineEdit->text(),
                                           plottingTab_titleAxisLegendTab_axisNameSize_doubleSpinBox->value(),
                                           plottingTab_titleAxisLegendTab_axisBold_checkBox->isChecked(),
                                           plottingTab_titleAxisLegendTab_axisItalic_checkBox->isChecked(),
                                           plottingTab_titleAxisLegendTab_yMin_checkBox->isChecked(),
                                           plottingTab_titleAxisLegendTab_yMin_doubleSpinBox->value(),
                                           plottingTab_titleAxisLegendTab_yMax_checkBox->isChecked(),
                                           plottingTab_titleAxisLegendTab_yMax_doubleSpinBox->value() );
    }
}

void FADTTSWindow::UpdatePlotTitle()
{
    if( plottingTab_titleAxisLegendTab_useDefaultTitle_checkBox->isChecked() )
    {
        m_plot->SetDefaultTitle( plottingTab_titleAxisLegendTab_titleBold_checkBox->isChecked(),
                                         plottingTab_titleAxisLegendTab_titleItalic_checkBox->isChecked(),
                                         plottingTab_titleAxisLegendTab_titleSize_doubleSpinBox->value() );
    }
    else
    {
        m_plot->SetCustomizedTitle( plottingTab_titleAxisLegendTab_titleName_lineEdit->text(),
                                    plottingTab_titleAxisLegendTab_titleBold_checkBox->isChecked(),
                                    plottingTab_titleAxisLegendTab_titleItalic_checkBox->isChecked(),
                                    plottingTab_titleAxisLegendTab_titleSize_doubleSpinBox->value() );
    }
}

void FADTTSWindow::UpdatePlot()
{
    m_plot->SetSelectionToDisPlay() = m_currentLinesForDisplay;
    m_plot->SetCovariatesProperties( m_covariatesForDisplay );

    m_plot->SetGrid() = plottingTab_titleAxisLegendTab_gridOn_checkBox->isChecked();
    m_plot->SetAbscissaNotation( plottingTab_titleAxisLegendTab_scientificNotation_checkBox->isChecked() );
    UpdatePlotAxis();

    m_plot->SetLegend( plottingTab_titleAxisLegendTab_showLegend_checkBox->isChecked(), plottingTab_titleAxisLegendTab_legendPosition_comboBox->currentText() );

    m_plot->SetPvalueThreshold() = plottingTab_editionTab_pvalueThreshold_doubleSpinBox->value();

    m_plot->SetLineWidth() = plottingTab_editionTab_lineWidth_doubleSpinBox->value();
    m_plot->SetSelectedLineColor( plottingTab_editionTab_selectedLineColor_comboBox->currentText() );

    m_plot->SetMarkerType( plottingTab_editionTab_markerType_comboBox->currentText() );
    m_plot->SetMarkerSize() = plottingTab_editionTab_markerSize_doubleSpinBox->value();
    m_plot->SetBinaryBetas() = plottingTab_editionTab_binaryBetas_checkBox->isChecked();

    UpdatePlotTitle();
}


void FADTTSWindow::ResetPlotTab()
{
    m_plotComboBox->setCurrentText( "No Plot" );

    plottingTab_titleAxisLegendTab_titleName_lineEdit->clear();

    plottingTab_displayPlot_pushButton->setEnabled( false );
    plottingTab_resetPlot_pushButton->setEnabled( false );
    plottingTab_savePlot_pushButton->setEnabled( false );

    plottingTab_loadPlotSettings_pushButton->setEnabled( false );
    plottingTab_savePlotSettings_pushButton->setEnabled( false );
}

void FADTTSWindow::SetPlotTab()
{
    m_isPlotReady = false;
    m_plot->ClearPlot();
    m_plot->ResetPlotData();
    m_lineSelectedListWidget->hide();
    SetSelectionLinesDisplayedVisible( false );

    QLabel *fibernameIcon = plottingTab_loadSetDataTab_fibernameIcon_label;
    QLabel *plotDirInfo = plottingTab_loadSetDataTab_plotDirectoryInfo_label;
    QLabel *plotDirIcon = plottingTab_loadSetDataTab_plotDirectoryIcon_label;
    QString fibername = para_plottingTab_loadSetDataTab_fibername_lineEdit->text().remove( " ", Qt::CaseInsensitive );
    QString directory = para_plottingTab_loadSetDataTab_browsePlotDirectory_lineEdit->text();

    if( QDir( directory ).exists() && !directory.isEmpty() )
    {
        m_isPlotReady = m_plot->InitPlot( directory, fibername, para_executionTab_pvalueThreshold_doubleSpinBox->value() );
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


    plottingTab_loadSetDataTab_set_widget->setEnabled( m_isPlotReady );
    plottingTab_titleAxisLegendTab->setEnabled( m_isPlotReady );
    plottingTab_editionTab->setEnabled( m_isPlotReady );
    plottingTab_loadPlotSettings_pushButton->setEnabled( m_isPlotReady );
    plottingTab_savePlotSettings_pushButton->setEnabled( m_isPlotReady );

    SetPlotOptions( false, false, false );

    if( m_isPlotReady )
    {
        QStringList filter = QStringList() << "*plotSettings*.json" << "*PlotSettings*.json";
        QStringList plotSettings = QDir( directory ).entryList( filter, QDir::Files );
        if( !plotSettings.isEmpty() )
        {
            LoadPlotSettings( QString( directory + "/" + plotSettings.first() ) );
        }
    }
}

void FADTTSWindow::LoadPlotSettings( QString filePath )
{
    QString text;
    QFile refJSON( filePath );
    refJSON.open( QIODevice::ReadOnly | QIODevice::Text );
    text = refJSON.readAll();

    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson( text.toUtf8(), &jsonError );
    if( jsonError.error == QJsonParseError::NoError )
    {
        QJsonObject jsonObject = jsonDoc.object();

        /*** Title/Axis/Legend Data Tab ***/
        QJsonObject title = jsonObject.value( "title" ).toObject();
        plottingTab_titleAxisLegendTab_useDefaultTitle_checkBox->setChecked( title.value( "defaultTitle" ).toBool() );
        plottingTab_titleAxisLegendTab_titleBold_checkBox->setChecked( title.value( "bold" ).toBool() );
        plottingTab_titleAxisLegendTab_titleItalic_checkBox->setChecked( title.value( "italic" ).toBool() );
        plottingTab_titleAxisLegendTab_titleSize_doubleSpinBox->setValue( title.value( "size" ).toDouble() );

        QJsonObject axis = jsonObject.value( "axis" ).toObject();
        plottingTab_titleAxisLegendTab_gridOn_checkBox->setChecked( axis.value( "showGrid" ).toBool() );
        plottingTab_titleAxisLegendTab_scientificNotation_checkBox->setChecked( axis.value( "scientificNotation" ).toBool() );
        plottingTab_titleAxisLegendTab_useDefaultAxis_checkBox->setChecked( axis.value( "defaultAxis" ).toBool() );
        plottingTab_titleAxisLegendTab_axisLabelSize_doubleSpinBox->setValue( axis.value( "labelSize" ).toDouble() );
        plottingTab_titleAxisLegendTab_xName_lineEdit->setText( axis.value( "xName" ).toString() );
        plottingTab_titleAxisLegendTab_yName_lineEdit->setText( axis.value( "yName" ).toString() );
        plottingTab_titleAxisLegendTab_axisNameSize_doubleSpinBox->setValue( axis.value( "nameSize" ).toDouble() );
        plottingTab_titleAxisLegendTab_axisBold_checkBox->setChecked( axis.value( "bold" ).toBool() );
        plottingTab_titleAxisLegendTab_axisItalic_checkBox->setChecked( axis.value( "italic" ).toBool() );
        plottingTab_titleAxisLegendTab_yMin_checkBox->setChecked( axis.value( "yMin" ).toObject().value( "set" ).toBool() );
        plottingTab_titleAxisLegendTab_yMin_doubleSpinBox->setValue( axis.value( "yMin" ).toObject().value( "value" ).toDouble() );
        plottingTab_titleAxisLegendTab_yMax_checkBox->setChecked( axis.value( "yMax" ).toObject().value( "set" ).toBool() );
        plottingTab_titleAxisLegendTab_yMax_doubleSpinBox->setValue( axis.value( "yMax" ).toObject().value( "value" ).toDouble() );

        QJsonObject legend = jsonObject.value( "legend" ).toObject();
        plottingTab_titleAxisLegendTab_showLegend_checkBox->setChecked( legend.value( "show" ).toBool() );
        plottingTab_titleAxisLegendTab_legendPosition_comboBox->setCurrentText( legend.value( "position" ).toString() );

        /*** Edition Tab ***/
        QJsonObject pvalueThreshold = jsonObject.value( "pvalueThreshold" ).toObject();
        plottingTab_editionTab_pvalueThreshold_doubleSpinBox->setValue( pvalueThreshold.value( "value" ).toDouble() );

        QJsonObject line = jsonObject.value( "line" ).toObject();
        plottingTab_editionTab_lineWidth_doubleSpinBox->setValue( line.value( "lineWidth" ).toDouble() );
        plottingTab_editionTab_selectedLineColor_comboBox->setCurrentText( line.value( "colorSelection" ).toString() );

        QJsonObject marker = jsonObject.value( "marker" ).toObject();
        plottingTab_editionTab_markerType_comboBox->setCurrentText( marker.value( "type" ).toString() );
        plottingTab_editionTab_markerSize_doubleSpinBox->setValue( marker.value( "size" ).toDouble() );
        plottingTab_editionTab_binaryBetas_checkBox->setChecked( marker.value( "binaryBetas" ).toBool() );

        /*** properties Edition ***/
        QJsonArray propertiesEdition = jsonObject.value( "propertiesEdition" ).toArray();
        QMap< int, QString > properties;
        for( int i = 0; i < m_propertiesColorsComboBoxMap.size(); i++ )
        {
            QString currentProperty = m_propertiesColorsComboBoxMap.value( i ).first;
            properties.insert( i, currentProperty );
        }

        for( int i = 0; i < propertiesEdition.size(); i++ )
        {
            QJsonObject currentPropertyObject = propertiesEdition.at( i ).toObject();
            QString currentProperty = currentPropertyObject.value( "name" ).toString();
            if( properties.values().contains( currentProperty ) )
            {
                m_propertiesColorsComboBoxMap.value( properties.key( currentProperty ) ).second->setCurrentText( currentPropertyObject.value( "color" ).toString() );
            }
        }

        /*** covariates Edition ***/
        QJsonArray covariatesEdition = jsonObject.value( "covariatesEdition" ).toArray();
        QMap< int, QString > covariates;
        for( int i = 0; i < m_covariatesColorsComboBoxMap.size(); i++ )
        {
            QString currentCovariate = m_covariatesColorsComboBoxMap.value( i ).first;
            covariates.insert( i, currentCovariate );
        }

        for( int i = 0; i < covariatesEdition.size(); i++ )
        {
            QJsonObject currentCovariateObject = covariatesEdition.at( i ).toObject();
            QString currentCovariate = currentCovariateObject.value( "name" ).toString();
            if( covariates.values().contains( currentCovariate ) )
            {
                m_covariatesColorsComboBoxMap.value( covariates.key( currentCovariate ) ).second->setCurrentText( currentCovariateObject.value( "color" ).toString() );
                m_covariatesNameLineEditMap.value( covariates.key( currentCovariate ) ).second->setText( currentCovariateObject.value( "newName" ).toString() );
            }
        }
    }
    else
    {
        QString warningMessage = "<b>Unable to load plot settings</b><br>";
        warningMessage.append( QString( "<br>Trying to load:<br><i>" + filePath + "</i><br>" ) );
        warningMessage.append( QString( "<br>Error:<i> " + jsonError.errorString().toUpper() + "</i><br>" ) );
        WarningPopUp( warningMessage );
    }
}

void FADTTSWindow::SavePlotSettings( QString filePath )
{
    QJsonObject jsonObject;
    QJsonObject title;
    QJsonObject axis;
    QJsonObject legend;
    QJsonObject pvalueThreshold;
    QJsonObject line;
    QJsonObject marker;
    QJsonArray propertiesEdition;
    QJsonArray covariatesEdition;

    /*** Title/Axis/Legend Data Tab ***/
    title.insert( "defaultTitle", plottingTab_titleAxisLegendTab_useDefaultTitle_checkBox->isChecked() );
    title.insert( "bold", plottingTab_titleAxisLegendTab_titleBold_checkBox->isChecked() );
    title.insert( "italic", plottingTab_titleAxisLegendTab_titleItalic_checkBox->isChecked() );
    title.insert( "size", plottingTab_titleAxisLegendTab_titleSize_doubleSpinBox->value() );

    axis.insert( "showGrid", plottingTab_titleAxisLegendTab_gridOn_checkBox->isChecked() );
    axis.insert( "scientificNotation", plottingTab_titleAxisLegendTab_scientificNotation_checkBox->isChecked() );
    axis.insert( "defaultAxis", plottingTab_titleAxisLegendTab_useDefaultAxis_checkBox->isChecked() );
    axis.insert( "labelSize", plottingTab_titleAxisLegendTab_axisLabelSize_doubleSpinBox->value() );
    axis.insert( "xName", plottingTab_titleAxisLegendTab_xName_lineEdit->text() );
    axis.insert( "yName", plottingTab_titleAxisLegendTab_yName_lineEdit->text() );
    axis.insert( "nameSize", plottingTab_titleAxisLegendTab_axisNameSize_doubleSpinBox->value() );
    axis.insert( "bold", plottingTab_titleAxisLegendTab_axisBold_checkBox->isChecked() );
    axis.insert( "italic", plottingTab_titleAxisLegendTab_axisItalic_checkBox->isChecked() );
    QJsonObject yMin;
    yMin.insert( "set", plottingTab_titleAxisLegendTab_yMin_checkBox->isChecked() );
    yMin.insert( "value", plottingTab_titleAxisLegendTab_yMin_doubleSpinBox->value() );
    axis.insert( "yMin", yMin );
    QJsonObject yMax;
    yMax.insert( "set", plottingTab_titleAxisLegendTab_yMax_checkBox->isChecked() );
    yMax.insert( "value", plottingTab_titleAxisLegendTab_yMax_doubleSpinBox->value() );
    axis.insert( "yMax", yMax );

    legend.insert( "show", plottingTab_titleAxisLegendTab_showLegend_checkBox->isChecked() );
    legend.insert( "position", plottingTab_titleAxisLegendTab_legendPosition_comboBox->currentText() );

    /*** Edition Tab ***/
    pvalueThreshold.insert( "value", plottingTab_editionTab_pvalueThreshold_doubleSpinBox->value() );

    line.insert( "lineWidth", plottingTab_editionTab_lineWidth_doubleSpinBox->value() );
    line.insert( "colorSelection", plottingTab_editionTab_selectedLineColor_comboBox->currentText() );

    marker.insert( "type", plottingTab_editionTab_markerType_comboBox->currentText() );
    marker.insert( "size", plottingTab_editionTab_markerSize_doubleSpinBox->value() );
    marker.insert( "binaryBetas", plottingTab_editionTab_binaryBetas_checkBox->isChecked() );

    /*** properties Edition ***/
    comboBoxMapType::ConstIterator iterPropertyColor = m_propertiesColorsComboBoxMap.cbegin();
    while( iterPropertyColor != m_propertiesColorsComboBoxMap.cend() )
    {
        QJsonObject currentProperty;
        currentProperty.insert( "name", iterPropertyColor.value().first );
        currentProperty.insert( "color", iterPropertyColor.value().second->currentText() );

        propertiesEdition.append( currentProperty );

        ++iterPropertyColor;
    }

    /*** covariates Edition ***/
    comboBoxMapType::ConstIterator iterCovariateColor = m_covariatesColorsComboBoxMap.cbegin();
    covariateNameLineEditMapType::ConstIterator iterCovariateName = m_covariatesNameLineEditMap.cbegin();
    while( iterCovariateColor != m_covariatesColorsComboBoxMap.cend() )
    {
        QJsonObject currentCovariate;
        currentCovariate.insert( "name", iterCovariateColor.value().first );
        currentCovariate.insert( "color", iterCovariateColor.value().second->currentText() );
        currentCovariate.insert( "newName", iterCovariateName.value().second->text() );

        covariatesEdition.append( currentCovariate );

        ++iterCovariateColor;
        ++iterCovariateName;
    }

    jsonObject.insert( "title", title );
    jsonObject.insert( "axis", axis );
    jsonObject.insert( "legend", legend );
    jsonObject.insert( "pvalueThreshold", pvalueThreshold );
    jsonObject.insert( "line", line );
    jsonObject.insert( "marker", marker );
    jsonObject.insert( "propertiesEdition", propertiesEdition );
    jsonObject.insert( "covariatesEdition", covariatesEdition );

    QJsonDocument jsonDoc;
    jsonDoc.setObject( jsonObject );
    QFile exportedJSON( filePath );
    exportedJSON.open( QIODevice::WriteOnly | QIODevice::Text );
    exportedJSON.write( jsonDoc.toJson( QJsonDocument::Indented ) );
    exportedJSON.flush();
    exportedJSON.close();
}
