#include "FADTTSWindow.h"
#include "ui_FADTTSWindow.h"


/****************************************************************/
/******************** Configuration & Events ********************/
/****************************************************************/
/// Retrieve platform separator ("," ";" ...)
const QString FADTTSWindow::m_csvSeparator = QLocale().groupSeparator();

const QColor FADTTSWindow::m_green = QColor( 0,255,0,127 );
const QColor FADTTSWindow::m_red = QColor( 255,0,0,127 );
const QColor FADTTSWindow::m_grey = QColor( 220,220,220,255 );
const QColor FADTTSWindow::m_yellow = QColor( 255,255,0,127 );
const QColor FADTTSWindow::m_lightBlack = QColor( 0,0,0,191 );

const int FADTTSWindow::m_IconSize = 12;


/********************** Public  functions ***********************/
FADTTSWindow::FADTTSWindow()
{
    m_editInputDialog = new EditInputDialog;
    m_infoDialog = new InfoDialog;

    m_sortedSubjectListWidget = new QListWidget();
    m_sortedSubjectListWidget = this->subjectsTab_sortedSubjects_listWidget;

    m_covariatesListWidget = new QListWidget();
    m_covariatesListWidget = this->para_parametersTab_covariates_listWidget;

    m_okPixmap = QPixmap( ":/FADTTS_Icons/okIconOut.xpm" );
    m_koPixmap = QPixmap( ":/FADTTS_Icons/koIconOut.xpm" );
    m_warningPixmap = QPixmap( ":/FADTTS_Icons/warningIconOut.xpm" );

    Init();
}

FADTTSWindow::~FADTTSWindow()
{
    delete m_mainUi;
    delete m_editInputDialog;
    delete m_infoDialog;
    delete m_sortedSubjectListWidget;
    delete m_covariatesListWidget;
}


/*********************** Private slots ***********************/
void FADTTSWindow::SaveParaConfigFile()
{
    QString dir;
    QString filename = QFileDialog::getSaveFileName( this , tr( "Save Parameter Configuration File" ) ,  dir , tr( ".xml( *.xml ) ;; .*( * )" ) );
    if( !filename.isEmpty() )
    {
        QFileInfo fi( filename ) ;
        dir = fi.dir().absolutePath() ;
        Save_Parameter_Configuration( filename.toStdString() );
    }
}

void FADTTSWindow::LoadParaConfigFile()
{
    QString dir;
    QString filename = QFileDialog::getOpenFileName( this , tr( "Load Parameter Configuration File" ) , dir , tr( ".xml( *.xml ) ;; .*( * )" ) );
    if( !filename.isEmpty() )
    {
        QFileInfo fi( filename ) ;
        dir = fi.dir().absolutePath() ;
        Load_Parameter_Configuration( filename.toStdString() );
    }
}

void FADTTSWindow::SaveSoftConfigFile()
{
    QString dir;
    QString filename = QFileDialog::getSaveFileName( this , tr( "Save Software Configuration File" ) , dir , tr( ".xml( *.xml ) ;; .*( * )" ) );
    if( !filename.isEmpty() )
    {
        QFileInfo fi( filename ) ;
        dir = fi.dir().absolutePath() ;
        Save_Software_Configuration( filename.toStdString() );
    }
}

void FADTTSWindow::LoadSoftConfigFile()
{
    QString dir;
    QString filename = QFileDialog::getOpenFileName( this , tr( "Load Software Configuration File" ) , dir , tr( ".xml( *.xml ) ;; .*( * )" ) );
    if( !filename.isEmpty() )
    {
        QFileInfo fi( filename ) ;
        dir = fi.dir().absolutePath() ;
        Load_Software_Configuration( filename.toStdString() );
    }
}


void FADTTSWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton closeMBox = QMessageBox::question( this,
                                                   tr( "FADTTS" ),
                                                   tr( "You are about to kill the running process.<br>Are you sure you want to continue?" ),
                                                   QMessageBox::No | QMessageBox::Yes, QMessageBox::No );
    switch( closeMBox )
    {
    case QMessageBox::No:
        event->ignore();
        break;
    case QMessageBox::Yes:
    {
        //        if( m_thread->isRunning() )
        //        {
        //            m_thread->terminate();
        //        }
        event->accept();
        break;
    }
    default:
        event->accept();
        break;
    }
}


/*********************** Private function **********************/
void FADTTSWindow::Init()
{
    InitMenuBar();

    int nbrDuplicates = m_data.InitData();
    if( nbrDuplicates != 0 )
    {
        CriticalPopUp( tr( qPrintable( "Careful, you have " + QString::number( nbrDuplicates ) + " duplicates<br>"
                       "in your file prefix list.<br>The application may not work properly." ) ) );
    }

    m_infoDialog->SetData( &m_data );
    m_editInputDialog->SetData( &m_data );

    InitInputTab();

    InitSubjectTab();

    InitParametersTab();

    InitRunTab();

    DisplayCovariatesList( m_data.GetCovariatesList() );

    UpdateAvailableFileParamTab();
}

void FADTTSWindow::InitMenuBar()
{
    connect( this->actionLoad_Settings, SIGNAL( triggered() ), SLOT( LoadParaConfigFile() ) );
    connect( this->actionSave_Settings, SIGNAL( triggered() ), SLOT( SaveParaConfigFile() ) );
    connect( this->actionLoad_Software, SIGNAL( triggered() ), SLOT( LoadSoftConfigFile() ) );
    connect( this->actionSave_Software, SIGNAL( triggered() ), SLOT( SaveSoftConfigFile() ) );
//    connect( this->actionAbout, SIGNAL( triggered() ), SLOT( LoadSoftConfigFile() ) );

}

void FADTTSWindow::InitInputTab()
{
    connect( this->inputsTab_inputAddInputFiles_pushButton, SIGNAL( clicked() ), this, SLOT( AddFiles() ) );

    /// Map of PushButtons to add each file separetely and
    /// SignalMapper to link them to the slot AddFile()
    m_inputTabAddFilePushButtonMap.insert( m_data.GetAxialDiffusivityPrefix(), this->inputsTab_inputADfile_pushButton );
    m_inputTabAddFilePushButtonMap.insert( m_data.GetRadialDiffusivityPrefix(), this->inputsTab_inputRDfile_pushButton );
    m_inputTabAddFilePushButtonMap.insert( m_data.GetMeanDiffusivityPrefix(), this->inputsTab_inputMDfile_pushButton );
    m_inputTabAddFilePushButtonMap.insert( m_data.GetFractionalAnisotropyPrefix(), this->inputsTab_inputFAfile_pushButton );
    m_inputTabAddFilePushButtonMap.insert( m_data.GetCovariatesPrefix(), this->inputsTab_inputCOMPfile_pushButton );
    QSignalMapper *signalMapperAddFile = new QSignalMapper( this );
    connect( signalMapperAddFile, SIGNAL( mapped( const QString& ) ), this, SLOT( AddFile( const QString& ) ) );
    for( int i = 0; i < m_data.GetPrefixList().size(); ++i )
    {
        connect( m_inputTabAddFilePushButtonMap[ m_data.GetPrefixList().at( i ) ], SIGNAL( clicked() ), signalMapperAddFile,SLOT(map() ) );
        signalMapperAddFile->setMapping( m_inputTabAddFilePushButtonMap[ m_data.GetPrefixList().at( i ) ], m_data.GetPrefixList().at( i ) );
    }

    /// Map of LineEdits where the file path of each file is set and
    /// SignalMapper to link them to the slot UpdateInputLineEdit()
    m_inputTabFilePathLineEditMap.insert( m_data.GetAxialDiffusivityPrefix(), this->para_inputsTab_inputADfile_lineEdit );
    m_inputTabFilePathLineEditMap.insert( m_data.GetRadialDiffusivityPrefix(), this->para_inputsTab_inputRDfile_lineEdit );
    m_inputTabFilePathLineEditMap.insert( m_data.GetMeanDiffusivityPrefix(), this->para_inputsTab_inputMDfile_lineEdit );
    m_inputTabFilePathLineEditMap.insert( m_data.GetFractionalAnisotropyPrefix(), this->para_inputsTab_inputFAfile_lineEdit );
    m_inputTabFilePathLineEditMap.insert( m_data.GetCovariatesPrefix(), this->para_inputsTab_inputCOMPfile_lineEdit );
    QSignalMapper *signalMapperUpdateLineEdit = new QSignalMapper(this);
    connect( signalMapperUpdateLineEdit, SIGNAL( mapped( const QString& ) ), this, SLOT( UpdateInputLineEdit( const QString& ) ) );
    for( int i = 0; i < m_data.GetPrefixList().size(); ++i )
    {
        connect( m_inputTabFilePathLineEditMap[ m_data.GetPrefixList().at( i ) ], SIGNAL( textChanged( const QString& ) ), signalMapperUpdateLineEdit,SLOT(map() ) );
        signalMapperUpdateLineEdit->setMapping( m_inputTabFilePathLineEditMap[ m_data.GetPrefixList().at( i ) ], m_data.GetPrefixList().at( i ) );
    }

    /// Map of Labels to set the icon information of each file entered in a LineEdit
    m_inputTabIconLabelMap.insert( m_data.GetAxialDiffusivityPrefix(), this->inputsTab_iconInputADFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetRadialDiffusivityPrefix(), this->inputsTab_iconInputRDFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetMeanDiffusivityPrefix(), this->inputsTab_iconInputMDFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetFractionalAnisotropyPrefix(), this->inputsTab_iconInputFAFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetCovariatesPrefix(), this->inputsTab_iconInputCOMPFile_label );

    /// Map of PushButtons to edit the files and
    /// SignalMapper to link them to the slot EditFile()
    m_inputTabEditFilePushButtonMap.insert( m_data.GetAxialDiffusivityPrefix(), this->inputsTab_editInputADfile_pushButton );
    m_inputTabEditFilePushButtonMap.insert( m_data.GetRadialDiffusivityPrefix(), this->inputsTab_editInputRDfile_pushButton );
    m_inputTabEditFilePushButtonMap.insert( m_data.GetMeanDiffusivityPrefix(), this->inputsTab_editInputMDfile_pushButton );
    m_inputTabEditFilePushButtonMap.insert( m_data.GetFractionalAnisotropyPrefix(), this->inputsTab_editInputFAfile_pushButton );
    m_inputTabEditFilePushButtonMap.insert( m_data.GetCovariatesPrefix(), this->inputsTab_editInputCOMPfile_pushButton );
    QSignalMapper *signalMapperEditFile = new QSignalMapper( this );
    connect( signalMapperEditFile, SIGNAL( mapped( const QString& ) ), this, SLOT( EditFile( const QString& ) ) );
    for( int i = 0; i < m_data.GetPrefixList().size(); ++i )
    {
        connect( m_inputTabEditFilePushButtonMap[ m_data.GetPrefixList().at( i ) ], SIGNAL( clicked() ), signalMapperEditFile, SLOT(map() ) );
        signalMapperEditFile->setMapping( m_inputTabEditFilePushButtonMap[ m_data.GetPrefixList().at( i ) ], m_data.GetPrefixList().at( i ) );
    }

    connect( this->inputsTab_inputInfo_pushButton, SIGNAL( clicked() ), this, SLOT( DisplayInfoInputFiles() ) );

    /// Signal/Slot connection to receive updates from m_editInputDialog
    connect( m_editInputDialog, SIGNAL( FilePathChanged( QString, QString ) ), this, SLOT( UpdateLineEditAfterFileEdition( QString , QString ) ) );
    connect( m_editInputDialog, SIGNAL( CovariatesChanged( QStringList ) ), this, SLOT( UpdateCovariatesAfterFileEdition( QStringList ) ) );
    connect( m_editInputDialog, SIGNAL( SubjectColumnIDChanged( int ) ), this, SLOT( UpdateSubjectColumnIDAfterFileEdition( int ) ) );
}

void FADTTSWindow::InitSubjectTab()
{
    /// Map of CheckBoxes to select the files we want to work on and
    /// SignalMapper to link them to the slot SortSubjects()
    m_paramTabFileCheckBoxMap.insert( m_data.GetAxialDiffusivityPrefix(), this->para_subjectsTab_adInput_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetRadialDiffusivityPrefix(), this->para_subjectsTab_rdInput_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetMeanDiffusivityPrefix(), this->para_subjectsTab_mdInput_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetFractionalAnisotropyPrefix(), this->para_subjectsTab_faInput_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetCovariatesPrefix(), this->para_subjectsTab_compInput_checkBox );
    QSignalMapper *signalMapperSelectFile = new QSignalMapper(this);
    connect( signalMapperSelectFile, SIGNAL( mapped( const QString& ) ), this, SLOT( SortSubjects() ) );
    connect( signalMapperSelectFile, SIGNAL( mapped( const QString& ) ), this, SLOT( SyncUiToModelStructure() ) );
    for ( int i = 0; i < m_data.GetPrefixList().size(); ++i )
    {
        connect( m_paramTabFileCheckBoxMap[ m_data.GetPrefixList().at( i ) ], SIGNAL( toggled( const bool& ) ), signalMapperSelectFile,SLOT( map() ) );
        signalMapperSelectFile->setMapping( m_paramTabFileCheckBoxMap[ m_data.GetPrefixList().at( i ) ], m_data.GetPrefixList().at( i ) );
    }

    /// Map of Labels displaying the matrix data size of the files that have been chosen
    m_paramTabFileSizeLabelMap.insert( m_data.GetAxialDiffusivityPrefix(), this->subjectsTab_adInputSize_label );
    m_paramTabFileSizeLabelMap.insert( m_data.GetRadialDiffusivityPrefix(),this->subjectsTab_rdInputSize_label );
    m_paramTabFileSizeLabelMap.insert( m_data.GetMeanDiffusivityPrefix(), this->subjectsTab_mdInputSize_label );
    m_paramTabFileSizeLabelMap.insert( m_data.GetFractionalAnisotropyPrefix(), this->subjectsTab_faInputSize_label );
    m_paramTabFileSizeLabelMap.insert( m_data.GetCovariatesPrefix(), this->subjectsTab_compInputSize_label );


    connect( this->subjectsTab_loadInputSubjectList_PushButton, SIGNAL( clicked() ), this, SLOT( LoadInputSubjectFiles() ) );
    connect( this->subjectsTab_reset_pushButton, SIGNAL( clicked() ), this, SLOT( ResetInputSubjectFiles() ) );
    connect( this->para_subjectsTab_inputSubjectList_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( UpdateInputSubjectListLineEdit( const QString&  ) ) );
    connect( this->subjectsTab_saveCheckedSubjectList_pushButton, SIGNAL( clicked() ), this, SLOT( SaveCheckedSubjectList() ) );

    connect( this->subjectsTab_checkAllVisible_pushButton, SIGNAL( clicked() ), this, SLOT( CheckAllSubjectsVisible() ) );
    connect( this->subjectsTab_unCheckAllVisible_pushButton, SIGNAL( clicked() ), this, SLOT( UnCheckAllSubjectsVisible() ) );
    connect( this->subjectsTab_sortedSubjects_listWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( SelectSubject( QListWidgetItem * ) ) );
    connect( this->subjectsTab_search_lineEdit, SIGNAL( textEdited( const QString& ) ), this, SLOT( Search() ) );
    connect( this->subjectsTab_caseSensitive_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( SetCaseSensitivity( bool ) ) );
}

void FADTTSWindow::InitParametersTab()
{
    connect( this->para_parametersTab_covariates_listWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( SelectCovariates( QListWidgetItem * ) ) );
    connect( this->parametersTab_covariatesCheckAll_pushButton, SIGNAL( clicked() ), this, SLOT( CheckAllCovariates() ) );
    connect( this->parametersTab_covariatesUncheckAll_pushButton, SIGNAL( clicked() ), this, SLOT( UnCheckAllCovariates() ) );

    connect( this->para_parametersTab_omnibus_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( SyncUiToModelStructure() ) );
    connect( this->para_parametersTab_testIndependent_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( SyncUiToModelStructure() ) );
    connect( this->para_parametersTab_nbrPermutations_spinBox, SIGNAL( valueChanged( int ) ), this, SLOT( SyncUiToModelStructure() ) );
}

void FADTTSWindow::InitRunTab()
{
    connect( this->runTab_outputDir_pushButton, SIGNAL( clicked() ), this, SLOT( SetOutputDir() ) );
    connect( this->para_runTab_outputDir_lineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( UpdateOutputDirLineEdit( const QString& ) ) );

/********** ADD MATLAB EXECUTABLE **********/

    connect( this->runTab_run_pushButton, SIGNAL( clicked() ), this, SLOT( RunFADTTS() ) );
}


void FADTTSWindow::UpdateCurrentDirEditInputDialog(const QString newfilePath, QString& currentDir )
{

    QDir dir = UpdateCurrentDir( newfilePath, currentDir );
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


void FADTTSWindow::WarningPopUp( const QString warningText )
{
    QMessageBox::warning( this, tr( "WARNING" ), tr( qPrintable( warningText ) ), QMessageBox::Ok );
}

void FADTTSWindow::CriticalPopUp( const QString criticalText )
{
    QMessageBox::critical( this, tr( "CRITICAL ERROR" ), tr( qPrintable( criticalText ) ), QMessageBox::Ok );
}

void FADTTSWindow::DisplayIcon( QLabel *label , const QPixmap icon )
{
    label->setPixmap( icon.scaled( QSize( m_IconSize, m_IconSize ), Qt::IgnoreAspectRatio ) );
}


/***************************************************************/
/************************** Input tab **************************/
/***************************************************************/

/***********************  Private slots  ***********************/
void FADTTSWindow::AddFiles()
{
    QString dirPath = m_currentFileInputDir;
    QStringList fileList;
    fileList = QFileDialog::getOpenFileNames( this, tr( "Choose CSV files" ), dirPath, tr( ".csv( *.csv ) ;; .*( * )" ) );
    AddFiles( fileList );
}

void FADTTSWindow::AddFile( const QString& prefID )
{
    QLineEdit *lineEdit = m_inputTabFilePathLineEditMap[ prefID ];
    QString filePath = lineEdit->text();
    QDir dir = QFileInfo( QFile( filePath ) ).absolutePath();
    if( filePath.isEmpty() || !dir.exists() )
    {
        dir = m_currentFileInputDir;
    }

    QString file = QFileDialog::getOpenFileName( this, tr( qPrintable( "Choose " + prefID + " file" ) ), dir.absolutePath(), tr( ".csv( *.csv ) ;; .*( * )" ) );
    QFile importedCSV( file );
    if( !importedCSV.open( QIODevice::ReadOnly ) )
    {
        return;
    }
    else
    {
        importedCSV.close();
        lineEdit->setText( file );
    }
}

void FADTTSWindow::UpdateInputLineEdit( const QString& prefID )
{
    QLineEdit *lineEdit = m_inputTabFilePathLineEditMap[ prefID ];
    QString filePath = lineEdit->text();
    if( !filePath.isEmpty() )
    {
        UpdateCurrentDirEditInputDialog( filePath, m_currentFileInputDir );
    }

    if( prefID == m_data.GetCovariatesPrefix() )
    {
        m_editInputDialog->ResetSubjectColumnID();   /// By default Subjects are on 1st column
    }

    SetInfoSubjectColumnID();

    UpdateFileInformation( prefID );

    SortSubjects();

    SyncUiToModelStructure();

}

void FADTTSWindow::EditFile( const QString& prefID )
{
    if( m_data.GetFilename( prefID ).isEmpty() )
    {
        QString warningMessage = tr( "<center>File edition unable</center>" );
        if( m_inputTabFilePathLineEditMap[ prefID ]->text().isEmpty() )
        {
            warningMessage.append( tr( "No file specified" ) );
        }
        else
        {
            warningMessage.append( tr( qPrintable( "Could not open the file:<br><i>" + m_inputTabFilePathLineEditMap[ prefID ]->text() + "</i>" ) ) );
        }
        WarningPopUp( warningMessage );
    }
    else
    {
        LaunchEditInputWindow( prefID );
    }
}


void FADTTSWindow::DisplayInfoInputFiles()
{
    m_infoDialog->setModal( true );
    m_infoDialog->setWindowTitle( tr( "Files Information" ) );
    m_infoDialog->DisplayInfo();
    m_infoDialog->exec();
}


void FADTTSWindow::UpdateLineEditAfterFileEdition( const QString& newFilePathAfterFileEdition, const QString& prefID )
{
    m_inputTabFilePathLineEditMap[ prefID ]->setText( newFilePathAfterFileEdition );
}

void FADTTSWindow::UpdateCovariatesAfterFileEdition( const QStringList& newCovariatesAfterFileEdition )
{
    m_data.ClearCovariatesList();
    m_data.SetCovariatesList() = newCovariatesAfterFileEdition;
    DisplayCovariatesList( m_data.GetCovariatesList() );

    SyncUiToModelStructure();
}

void FADTTSWindow::UpdateSubjectColumnIDAfterFileEdition( const int&  newSubjectColumnIDAfterFileEdition )
{
    /// Subjects not on the 1st column anymore
    m_data.SetSubjectColumnID( newSubjectColumnIDAfterFileEdition );

    SetInfoSubjectColumnID();
    UpdateFileInformation( m_data.GetCovariatesPrefix() );
    SortSubjects();
}


/*********************** Private function ***********************/
void FADTTSWindow::AddFiles( const QStringList fileList )
{
    QMap<QString, QStringList > map;
    foreach( QString file, fileList )
    {
        QString p = QFileInfo( QFile ( file ) ).fileName().split( '_' ).first();
        /// If m_filePrefixList contains the prefix, the file is linked to this prefix
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
            /// If a prefix is linked with more than one file, those files are ignored
            file.clear();
        }

        if( m_inputTabFilePathLineEditMap.contains( prefix ) )
        {
            m_inputTabFilePathLineEditMap[ prefix ]->setText( file );
        }
        ++iter;
    }
}


void  FADTTSWindow::LaunchEditInputWindow( QString prefID )
{
    m_editInputDialog->SetPrefix( prefID );
    m_editInputDialog->SetInputFile( m_inputTabFilePathLineEditMap[ prefID ]->text() );
    m_editInputDialog->setModal( true );
    m_editInputDialog->setWindowTitle( tr( qPrintable( "Edit " + prefID + " File" ) ) );
    m_editInputDialog->DisplayData();
    m_editInputDialog->exec();
}


void FADTTSWindow::UpdateFileInformation( const QString prefID )
{
    QString filePath = m_inputTabFilePathLineEditMap[ prefID ]->text();
    QFile file( filePath );
    if( !file.open( QIODevice::ReadOnly ) )
    {
        if( !m_inputTabFilePathLineEditMap[ prefID ]->text().isEmpty() )
        {
            DisplayIcon( prefID, m_koPixmap );
        }
        else
        {
            m_inputTabIconLabelMap[ prefID ]->clear();
        }
        m_data.ClearFileInformation( prefID );
    }
    else
    {
        QTextStream ts( &file );
        QList<QStringList> data;
        while( !ts.atEnd() )
        {
            data.append( ts.readLine().split( m_csvSeparator ) );
        }
        file.close();
        int nbRows = data.count();
        int nbColumns = data.at( 0 ).count();

        if( IsMatrixDimensionOK( data ) )
        {
            DisplayIcon( prefID, m_okPixmap );
            m_data.SetFilename( prefID ) = filePath;
            m_data.ClearSubjects( prefID );

            if( prefID == m_data.GetCovariatesPrefix() )
            {
                m_data.SetNbrRows( prefID ) = nbRows-1;
                m_data.SetNbrColumns( prefID ) = nbColumns-1;
                m_data.SetNbrSubjects( prefID ) = nbRows-1;

                m_data.ClearCovariatesList();
                for( int c = 0; c < nbColumns; ++c )
                {
                    if( c != m_data.GetSubjectColumnID() )
                    {
                        m_data.AddCovariate( data.at( 0 ).at( c ) );
                    }
                }
                /// Intercept representes everything that has not been classified in one of the previous
                /// cavariates. It is important to add it as 1st element of m_covariatesList
                m_data.AddIntercept();
                for( int r = 1; r < nbRows; r++ )
                {
                    m_data.AddSubject( prefID, data.at( r ).at( m_data.GetSubjectColumnID() ) );
                }
            }
            else
            {
                if( m_data.GetPrefixList().contains( prefID ) )
                {
                    m_data.SetNbrRows( prefID ) = nbRows-1;
                    m_data.SetNbrColumns( prefID ) = nbColumns;
                    m_data.SetNbrSubjects( prefID ) = nbColumns-1;
                    for( int c = 1; c < nbColumns; c++ )
                    {
                        m_data.AddSubject( prefID, data.at( 0 ).at( c ) );
                    }
                }
            }
        }
        else
        {
            DisplayIcon( prefID, m_koPixmap );

            QString criticalText = tr( qPrintable( "Data file corrupted:<br><i>" + m_data.GetFilename( prefID ) + "</i><br>"
                    "For each row, the number of columns is not constant.<br>"
                    "We advise you to check your data file." ) );
            CriticalPopUp( criticalText );

            m_data.ClearFileInformation( prefID );
            return;
        }
    }

    DisplayCovariatesList( m_data.GetCovariatesList() );
    UpdateAvailableFileParamTab();
}

bool FADTTSWindow::IsMatrixDimensionOK( const QList<QStringList> data )
{
    int dataSize = data.at( 0 ).count();
    foreach (QStringList strL,  data)
    {
        if( dataSize != strL.count() )
        {
            return false;
        }
    }
    return true;
}


void FADTTSWindow::DisplayIcon( const QString prefID, const QPixmap icon )
{
    QLabel *label = m_inputTabIconLabelMap[ prefID ];
    label->setPixmap( icon.scaled( QSize( m_IconSize, m_IconSize ), Qt::IgnoreAspectRatio ) );
}

void FADTTSWindow::SetInfoSubjectColumnID()
{
    QString filePath = m_inputTabFilePathLineEditMap[ m_data.GetCovariatesPrefix() ]->text();
    if( !filePath.isEmpty() && QFile( filePath ).open( QIODevice::ReadOnly ) )
    {
        QFile( filePath ).close();
        this->inputsTab_subjectColumnID_label->setText( tr( qPrintable( "<b><i><span style=""font-size:7pt;"">" +
                                                      QString::number( m_data.GetSubjectColumnID() + 1 ) + "<i/><b/><span/>") ) );
    }
    else
    {
        this->inputsTab_subjectColumnID_label->clear();
    }
}


/****************************************************************/
/************************* Subjects tab *************************/
/****************************************************************/

/*********************** Public  function ***********************/


/***********************  Private slots  ************************/
void FADTTSWindow::LoadInputSubjectFiles()
{
    QLineEdit *lineEdit = this->para_subjectsTab_inputSubjectList_lineEdit;
    QString filePath;
    QDir dir = QFileInfo( QFile( lineEdit->text() ) ).absolutePath();
    if( !dir.exists() || dir.dirName() == "." ) // Do we want it?
    {
        filePath = QFileDialog::getOpenFileName( this, tr( "Choose subject list file" ), m_currentSubjectListInputDir, tr( ".txt( *.txt ) ;; .*( * )" ) );
    }
    else
    {
        filePath = QFileDialog::getOpenFileName( this, tr( "Choose subject list file" ), dir.absolutePath(), tr( ".txt ( *.txt ) ;; .*( * )" ) );
    }

    QFile importedTXT( filePath );
    if( !importedTXT.open( QIODevice::ReadOnly ) )
    {
        return;
    }
    else
    {
        importedTXT.close();
        lineEdit->setText( filePath );
    }
}

void FADTTSWindow::ResetInputSubjectFiles()
{
    this->para_subjectsTab_inputSubjectList_lineEdit->clear();
}

void FADTTSWindow::UpdateInputSubjectListLineEdit( const QString& textLineEdit )
{
    if( !textLineEdit.isEmpty() )
    {
        UpdateCurrentDir( textLineEdit, m_currentSubjectListInputDir );
    }

    SortSubjects();

    SyncUiToModelStructure();
}

void FADTTSWindow::SaveCheckedSubjectList()
{
    QString filePath = QFileDialog::getSaveFileName( this, tr( "Save subject list as ..." ), m_currentSubjectListInputDir, tr( ".txt ( *.txt ) ;; .*( * )" ) );
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

        QString previousFilePath = this->para_subjectsTab_inputSubjectList_lineEdit->text();
        this->para_subjectsTab_inputSubjectList_lineEdit->setText( filePath );
        if( previousFilePath == filePath )
        {
            /// If filePath does not change, an update is needed to display the right subjects list
            SortSubjects();
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
    DisplayNbrSubjectsSelected();
}


void FADTTSWindow::SortSubjects()
{
    QString subjectListFilename = this->para_subjectsTab_inputSubjectList_lineEdit->text();
    QStringList refSubjectList = GetRefSubjectList( subjectListFilename );
    QMap<QString, QStringList> subjectListFromSelectedInputFile = GetAllSubjectsFromSelectedInputFiles();
    QMap< QString, QMap<QString, bool> > sortedSubjects = m_processing.FindSubjectInInputFile( refSubjectList, subjectListFromSelectedInputFile );

    QStringList matchedSubjectList;
    QMap<QString, QStringList > unMatchedSubjectList;
    m_processing.AssignSortedSubject( sortedSubjects, matchedSubjectList, unMatchedSubjectList, subjectListFilename );

    DisplaySortedSubjectList( refSubjectList, matchedSubjectList, unMatchedSubjectList );

    Search();
}

void FADTTSWindow::Search()
{
    QListWidget *list = m_sortedSubjectListWidget;
    QLineEdit *lineEdit = this->subjectsTab_search_lineEdit;
    QString search_str = lineEdit->text();
    QPalette palette;
    if( !search_str.isEmpty() )
    {
        if(  list->count() != 0  )
        {
            search_str.prepend( "*" );
            search_str.append( "*" );
            QRegExp regExp( search_str );
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

    Search();
}


/*********************** Private function ***********************/
void FADTTSWindow::UpdateAvailableFileParamTab()
{
    QString text;
    labelMapType::ConstIterator iterLabel = m_paramTabFileSizeLabelMap.begin();
    checkBoxMapType::ConstIterator iterCheckBox = m_paramTabFileCheckBoxMap.begin();
    while( iterLabel != m_paramTabFileSizeLabelMap.constEnd() )
    {
        int nbRows = m_data.GetNbrRows( iterCheckBox.key() );
        int nbColumns = m_data.GetNbrColumns( iterCheckBox.key() );
        if( ( nbRows == 0 ) || ( nbColumns == 0 ) )
        {
            text = tr( "N/A" );
            iterLabel.value()->setEnabled( false );
            iterCheckBox.value()->setCheckState( Qt::Unchecked );
            iterCheckBox.value()->setEnabled( false );
        }
        else
        {
            text = tr( qPrintable( QString::number( nbRows ) + "x" + QString::number( nbColumns ) ) );
            iterLabel.value()->setEnabled( true );
            iterCheckBox.value()->setCheckState( Qt::Checked );
            iterCheckBox.value()->setEnabled( true );
        }
        iterLabel.value()->setText( text );
        ++iterLabel;
        ++iterCheckBox;
    }

}

QStringList FADTTSWindow::GetRefSubjectList( const QString subjectListFilePath )
{
    QFile subjectFile( subjectListFilePath );
    QStringList refSubjectList;
    QLabel *label = this->subjectsTab_iconLoadList_label;
    if( !subjectFile.open( QIODevice::ReadOnly ) )
    {
        if( !subjectFile.fileName().isEmpty() )
        {
            DisplayIcon( label, m_koPixmap );
        }
        else
        {
            label->clear();
        }

        QMap<QString, bool> selectedInputFiles = GetSelectedInputFiles();
        refSubjectList = m_processing.GetRefSubjectListFromSelectedInputFiles( selectedInputFiles, m_data.GetSubjectColumnID() );
    }
    else
    {
        DisplayIcon( label, m_okPixmap );
        QTextStream ts( &subjectFile );
        while( !ts.atEnd() )
        {
            refSubjectList.append( ts.readLine() );
        }
        subjectFile.close();
    }
    refSubjectList.removeDuplicates();
    return refSubjectList;
}

QMap<QString, QStringList> FADTTSWindow::GetAllSubjectsFromSelectedInputFiles()
{
    QMap<QString, QStringList> selectedSubjectList;
    checkBoxMapType::ConstIterator iterPrefixCheckBox = m_paramTabFileCheckBoxMap.begin();
    QMap<QString, QStringList >::ConstIterator iterSubjectsList = m_data.GetSubjectsMapIterator();
    while( iterPrefixCheckBox != m_paramTabFileCheckBoxMap.constEnd() )
    {
        if( iterPrefixCheckBox.value()->isChecked() )
        {
            ( selectedSubjectList[iterSubjectsList.key()] ).append( iterSubjectsList.value() );
        }
        ++iterPrefixCheckBox;
        ++iterSubjectsList;
    }

    return selectedSubjectList;
}

void FADTTSWindow::DisplaySortedSubjectList( const QStringList refSubjectList, const QStringList matchedSubjectList, const QMap<QString, QStringList > unMatchedSubjectList )
{
    m_sortedSubjectListWidget->clear();
    m_sortedSubjectListWidget->setUpdatesEnabled( false );
    m_sortedSubjectListWidget->setSelectionMode( QAbstractItemView::NoSelection );

    QStringListIterator iterMatchedSubjectList( matchedSubjectList );
    while( iterMatchedSubjectList.hasNext() )
    {
        QString text = QString( iterMatchedSubjectList.next().toLocal8Bit().data() );
        QListWidgetItem *listItem = new QListWidgetItem( text, m_sortedSubjectListWidget );
        listItem->setCheckState( Qt::Checked );
        listItem->setBackgroundColor( m_green );
        listItem->setFlags( Qt::ItemIsEnabled );
        m_sortedSubjectListWidget->addItem( listItem );
    }

    QMap<QString, QStringList >::ConstIterator iterUnMatchedSubjectList = unMatchedSubjectList.begin();
    while( iterUnMatchedSubjectList != unMatchedSubjectList.constEnd() )
    {
        QStringList sortedText = iterUnMatchedSubjectList.value();
        sortedText.sort();
        QString text = tr( qPrintable( iterUnMatchedSubjectList.key() + " --> " + sortedText.join( ", " ) ) );
        QListWidgetItem *listItem = new QListWidgetItem( text, m_sortedSubjectListWidget );
        listItem->setBackgroundColor( m_red );
        listItem->setTextColor( m_lightBlack );
        m_sortedSubjectListWidget->addItem( listItem );
        ++iterUnMatchedSubjectList;
    }
    m_sortedSubjectListWidget->setUpdatesEnabled( true );

    this->subjectsTab_sortSubjects_label->clear();
    QString text;
    if( ( unMatchedSubjectList.isEmpty() ) && ( !matchedSubjectList.isEmpty() ) )
    {
        text = tr( qPrintable( "All subjects matched ( " + QString::number( matchedSubjectList.size() ) + " )" ) );
    }
    else if( ( !unMatchedSubjectList.isEmpty() ) && ( matchedSubjectList.isEmpty() ) )
    {
        text = tr( qPrintable( "Warning! No subject matched ( " + QString::number( unMatchedSubjectList.size() ) + " )" ) );
    }
    else if( !unMatchedSubjectList.isEmpty() && ( !matchedSubjectList.isEmpty() ) )
    {
        text = tr( qPrintable( QString::number( matchedSubjectList.size() ) + "/" + QString::number( refSubjectList.size() ) + " matched      " +
                QString::number( unMatchedSubjectList.size() ) + "/" + QString::number( refSubjectList.size() ) + " unmatched" ) );
    }
    //else both are empty
    this->subjectsTab_sortSubjects_label->setText( text );

    DisplayNbrSubjectsSelected();
}

void FADTTSWindow::DisplayNbrSubjectsSelected()
{
    this->subjectsTab_checkedSubjects_label->clear();
    int nbrSubjectsSelected = 0;
    for( int i = 0; i < m_sortedSubjectListWidget->count(); i++ )
    {
        QListWidgetItem *item = m_sortedSubjectListWidget->item( i );
        if( item->checkState() == Qt::Checked )
        {
            nbrSubjectsSelected++;
        }
    }

    if( m_sortedSubjectListWidget->count() != 0 )
    {
        this->subjectsTab_checkedSubjects_label->setText( tr( qPrintable( QString::number( nbrSubjectsSelected ) + " subjects selected") ) );
    }
}


/****************************************************************/
/************************ Parameters tab ************************/
/****************************************************************/

/***********************  Private  slots  ***********************/
void FADTTSWindow::SelectCovariates( QListWidgetItem *item )
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
            int ignoreWarning = QMessageBox::warning( this,
                                                      tr( "Uncheck Intercept" ),
                                                      tr( "You are about to uncheck the Intercept."
                                                          "This action is not recommended.<br>"
                                                          "Are you sure you want to do it?" ),
                                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if( ignoreWarning == QMessageBox::Yes )
            {
                item->setCheckState( Qt::Unchecked );
            }
        }
    }

    SyncUiToModelStructure();
}

void FADTTSWindow::CheckAllCovariates()
{
    QListWidget *covariatesList = m_covariatesListWidget;
    for( int i = 0; i < covariatesList->count(); i++ )
    {
        covariatesList->item( i )->setCheckState( Qt::Checked );
    }

    SyncUiToModelStructure();
}

void FADTTSWindow::UnCheckAllCovariates()
{
    QListWidget *covariatesList = m_covariatesListWidget;
    for( int i = 0; i < covariatesList->count(); i++ )
    {
        QListWidgetItem *currentItem = covariatesList->item( i );
        if( !currentItem->text().contains( "Intercept" ) )
        {
            currentItem->setCheckState( Qt::Unchecked );
        }
    }

    SyncUiToModelStructure();
}


/*********************** Private function ***********************/
void FADTTSWindow::DisplayCovariatesList( QStringList covariatesList )
{
    m_covariatesListWidget->clear();
    if( !( covariatesList.isEmpty() ) )
    {
        foreach( QString covar, covariatesList )
        {
            QListWidgetItem *covariateItem = new QListWidgetItem( covar, m_covariatesListWidget );
            covariateItem->setCheckState( Qt::Checked );
            covariateItem->setFlags( Qt::ItemIsEnabled );
            m_covariatesListWidget->addItem( covariateItem );
        }
    }
}


/****************************************************************/
/*************************** Run  tab ***************************/
/****************************************************************/

/*********************** Public  function ***********************/

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

void FADTTSWindow::UpdateOutputDirLineEdit( const QString&  textLineEdit )
{
    QLabel *label = this->runTab_iconOutputDir_label;
    if( !textLineEdit.isEmpty() )
    {
        if( QDir( textLineEdit ).exists() )
        {
            DisplayIcon( label, m_okPixmap );
            m_data.SetOutputDir() = textLineEdit;
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

    SyncUiToModelStructure();
}


void FADTTSWindow::RunFADTTS()
{
    QMap<int, QString> selectedCovariates = GetSelectedCovariates();

    if( m_data.GetFilename( m_data.GetCovariatesPrefix() ).isEmpty() || this->para_inputsTab_fiberName_lineEdit->text().isEmpty() || selectedCovariates.count() == 0  )
    {
        QString warningText = "<b>FADTTS will not be executed for the following reason(s):</b><br>";
        if( this->para_inputsTab_fiberName_lineEdit->text().isEmpty() )
        {
            warningText.append( "- No fiber name provided <i>Inputs Tab</i><br>" );
        }
        if( m_data.GetFilename( m_data.GetCovariatesPrefix() ).isEmpty() )
        {
            warningText.append( "- No correct input covariate file provided<i>Inputs Tab</i><br>" );
        }
        if( selectedCovariates.count() == 0 )
        {
            warningText.append( "- Select at least 1 covariate <i>Parameters Tab</i><br>" );
        }
        WarningPopUp( warningText );
    }
    else
    {
        m_processing.ResetProcessing();

        QMap<QString, bool> selectedInputFiles = GetSelectedInputFiles();

        QString selectedSubjectListFilePath = GenerateSelectedSubjectList();


        QStringList selectedPrefixes = GetSelectedPrefixes();

        QMap<QString, bool> matlabInputFiles = m_processing.GenerateMatlabInputFiles( selectedInputFiles, selectedSubjectListFilePath,
                                                       m_data.GetSubjectColumnID(), selectedCovariates, m_data.GetOutputDir() );

        m_scriptMatlab.GenerateMatlabScript( this->para_inputsTab_fiberName_lineEdit->text(), selectedPrefixes, matlabInputFiles,
                                             selectedCovariates, this->para_parametersTab_nbrPermutations_spinBox->value(), m_data.GetOutputDir() );
    }
}


/*********************** Private function ***********************/
QMap<QString, bool> FADTTSWindow::GetSelectedInputFiles()
{
    QMap<QString, bool> selectedInputFiles;
    foreach ( QString prefID, m_data.GetPrefixList() )
    {
        if( m_paramTabFileCheckBoxMap[ prefID ]->isChecked() )
        {
            if( prefID != m_data.GetCovariatesPrefix() )
            {
                selectedInputFiles.insert( m_data.GetFilename( prefID ), false );
            }
            else
            {
                selectedInputFiles.insert( m_data.GetFilename( prefID ), true );
            }
        }
    }
    return selectedInputFiles;
}

QString FADTTSWindow::GenerateSelectedSubjectList()
{
    QFile selectedSubjects( m_data.GetOutputDir() + "/subjectList.txt" );
    if( selectedSubjects.open( QIODevice::WriteOnly ) )
    {
        QTextStream tsSelectedSubjects( &selectedSubjects );
        for( int i = 0; i < m_sortedSubjectListWidget->count(); i++ )
        {
            if( m_sortedSubjectListWidget->item( i )->checkState() == Qt::Checked )
            {
                tsSelectedSubjects << QObject::tr( qPrintable( m_sortedSubjectListWidget->item( i )->text() ) ) << endl;
            }
        }
        selectedSubjects.close();
    }
    return selectedSubjects.fileName();
}

QMap<int, QString> FADTTSWindow::GetSelectedCovariates()
{
    QMap<int, QString> selectedCovariates;
    selectedCovariates.insert( -1, "Intercept" );
    QListWidget *list = m_covariatesListWidget;
    for( int i = 0; i < list->count(); i++ )
    {
        QListWidgetItem *currentItem = list->item( i );
        if( !currentItem->text().contains( "Intercept" ) && currentItem->checkState() == Qt::Checked )
        {
            selectedCovariates.insert( i, currentItem->text() );
        }
    }
    return selectedCovariates;
}


QStringList FADTTSWindow::GetSelectedPrefixes()
{
    QStringList selectedPrefixes;
    foreach ( QString prefID, m_data.GetPrefixList() )
    {
        if( m_paramTabFileCheckBoxMap[ prefID ]->isChecked() )
        {
            selectedPrefixes.append( prefID );
        }
    }
    return selectedPrefixes;
}
