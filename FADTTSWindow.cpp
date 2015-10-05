#include "MainWindow.h"
#include "ui_MainWindow.h"

using namespace std;

/// Retrieve platform separator ("," ";" ...)
const QString MainWindow::m_csvSeparator = QLocale().groupSeparator();

const QColor MainWindow::m_green = QColor( 0,255,0,127 );
const QColor MainWindow::m_red = QColor( 255,0,0,127 );
const QColor MainWindow::m_grey = QColor( 220,220,220,255 );
const QColor MainWindow::m_yellow = QColor( 255,255,0,127 );
const QColor MainWindow::m_lightBlack = QColor( 0,0,0,191 );

const int MainWindow::m_IconSize = 12;


MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    m_mainUi( new Ui::MainWindow )
{
    m_mainUi->setupUi( this );
    m_editInputDialog = new EditInputDialog;
    m_infoDialog = new InfoDialog;

    m_sortedSubjectListWidget = new QListWidget();
    m_sortedSubjectListWidget = m_mainUi->sortedSubjects_listWidget;

    m_covariatesListWidget = new QListWidget();
    m_covariatesListWidget = m_mainUi->covariates_listWidget;

    m_okPixmap = QPixmap( ":/FADTTS_Icons/okIconOut.xpm" );
    m_koPixmap = QPixmap( ":/FADTTS_Icons/koIconOut.xpm" );
    m_warningPixmap = QPixmap( ":/FADTTS_Icons/warningIconOut.xpm" );

/*    m_okPixmap = QPixmap( "/NIRAL/work/jeantm/FADTTS/Project/PartialCode/windows/okIcon.png" );
    m_koPixmap = QPixmap( "/NIRAL/work/jeantm/FADTTS/Project/PartialCode/windows/koIcon.png" );
    m_warningPixmap = QPixmap( "/NIRAL/work/jeantm/FADTTS/Project/PartialCode/windows/warningIcon.png" );*/

    Init();
}

MainWindow::~MainWindow()
{
    delete m_mainUi;
    delete m_editInputDialog;
    delete m_infoDialog;
    delete m_sortedSubjectListWidget;
    delete m_covariatesListWidget;
}


/***************************************************************/
/**************************** Other ****************************/
/***************************************************************/

/*********************** Private function **********************/
void MainWindow::Init()
{
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

    DisplayCovariatesList();

    UpdateAvailableFileParamTab();
}

void MainWindow::InitInputTab()
{
    /// Map of PushButtons to add each file separetely and
    /// SignalMapper to link them to the slot AddFile()
    m_inputTabAddFilePushButtonMap.insert( m_data.GetAxialDiffusivityPrefix(), m_mainUi->inputADfile_pushButton );
    m_inputTabAddFilePushButtonMap.insert( m_data.GetRadialDiffusivityPrefix(), m_mainUi->inputRDfile_pushButton );
    m_inputTabAddFilePushButtonMap.insert( m_data.GetMeanDiffusivityPrefix(), m_mainUi->inputMDfile_pushButton );
    m_inputTabAddFilePushButtonMap.insert( m_data.GetFractionalAnisotropyPrefix(), m_mainUi->inputFAfile_pushButton );
    m_inputTabAddFilePushButtonMap.insert( m_data.GetCovariatesPrefix(), m_mainUi->inputCOMPfile_pushButton );
    QSignalMapper *signalMapperAddFile = new QSignalMapper( this );
    QObject::connect( signalMapperAddFile, SIGNAL( mapped( const QString& ) ), this, SLOT( AddFile( const QString& ) ) );
    for( int i = 0; i < m_data.GetPrefixList().size(); ++i )
    {
        QObject::connect( m_inputTabAddFilePushButtonMap[ m_data.GetPrefixList().at( i ) ], SIGNAL( clicked() ), signalMapperAddFile,SLOT(map() ) );
        signalMapperAddFile->setMapping( m_inputTabAddFilePushButtonMap[ m_data.GetPrefixList().at( i ) ], m_data.GetPrefixList().at( i ) );
    }

    /// Map of LineEdits where the file path of each file is set and
    /// SignalMapper to link them to the slot UpdateInputLineEdit()
    m_inputTabFilePathLineEditMap.insert( m_data.GetAxialDiffusivityPrefix(), m_mainUi->para_inputADfile_lineEdit );
    m_inputTabFilePathLineEditMap.insert( m_data.GetRadialDiffusivityPrefix(), m_mainUi->para_inputRDfile_lineEdit );
    m_inputTabFilePathLineEditMap.insert( m_data.GetMeanDiffusivityPrefix(), m_mainUi->para_inputMDfile_lineEdit );
    m_inputTabFilePathLineEditMap.insert( m_data.GetFractionalAnisotropyPrefix(), m_mainUi->para_inputFAfile_lineEdit );
    m_inputTabFilePathLineEditMap.insert( m_data.GetCovariatesPrefix(), m_mainUi->para_inputCOMPfile_lineEdit );
    QSignalMapper *signalMapperUpdateLineEdit = new QSignalMapper(this);
    QObject::connect( signalMapperUpdateLineEdit, SIGNAL( mapped( const QString& ) ), this, SLOT( UpdateInputLineEdit( const QString& ) ) );
    for( int i = 0; i < m_data.GetPrefixList().size(); ++i )
    {
        QObject::connect( m_inputTabFilePathLineEditMap[ m_data.GetPrefixList().at( i ) ], SIGNAL( textChanged( const QString& ) ), signalMapperUpdateLineEdit,SLOT(map() ) );
        signalMapperUpdateLineEdit->setMapping( m_inputTabFilePathLineEditMap[ m_data.GetPrefixList().at( i ) ], m_data.GetPrefixList().at( i ) );
    }

    /// Map of Labels to set the icon information of each file entered in a LineEdit
    m_inputTabIconLabelMap.insert( m_data.GetAxialDiffusivityPrefix(), m_mainUi->iconInputADFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetRadialDiffusivityPrefix(), m_mainUi->iconInputRDFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetMeanDiffusivityPrefix(), m_mainUi->iconInputMDFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetFractionalAnisotropyPrefix(), m_mainUi->iconInputFAFile_label );
    m_inputTabIconLabelMap.insert( m_data.GetCovariatesPrefix(), m_mainUi->iconInputCOMPFile_label );

    /// Map of PushButtons to edit the files and
    /// SignalMapper to link them to the slot EditFile()
    m_inputTabEditFilePushButtonMap.insert( m_data.GetAxialDiffusivityPrefix(), m_mainUi->editInputADfile_pushButton );
    m_inputTabEditFilePushButtonMap.insert( m_data.GetRadialDiffusivityPrefix(), m_mainUi->editInputRDfile_pushButton );
    m_inputTabEditFilePushButtonMap.insert( m_data.GetMeanDiffusivityPrefix(), m_mainUi->editInputMDfile_pushButton );
    m_inputTabEditFilePushButtonMap.insert( m_data.GetFractionalAnisotropyPrefix(), m_mainUi->editInputFAfile_pushButton );
    m_inputTabEditFilePushButtonMap.insert( m_data.GetCovariatesPrefix(), m_mainUi->editInputCOMPfile_pushButton );
    QSignalMapper *signalMapperEditFile = new QSignalMapper( this );
    QObject::connect( signalMapperEditFile, SIGNAL( mapped( const QString& ) ), this, SLOT( EditFile( const QString& ) ) );
    for( int i = 0; i < m_data.GetPrefixList().size(); ++i )
    {
        QObject::connect( m_inputTabEditFilePushButtonMap[ m_data.GetPrefixList().at( i ) ], SIGNAL( clicked() ), signalMapperEditFile, SLOT(map() ) );
        signalMapperEditFile->setMapping( m_inputTabEditFilePushButtonMap[ m_data.GetPrefixList().at( i ) ], m_data.GetPrefixList().at( i ) );
    }

    /// Signal/Slot connection to receive updates from m_editInputDialog
    QObject::connect( m_editInputDialog, SIGNAL( FilePathChanged( QString, QString ) ), this, SLOT( UpdateLineEditAfterFileEdition( QString , QString ) ) );
    QObject::connect( m_editInputDialog, SIGNAL( CovariatesChanged( QStringList ) ), this, SLOT( UpdateCovariatesAfterFileEdition( QStringList ) ) );
    QObject::connect( m_editInputDialog, SIGNAL( SubjectColumnIDChanged( int ) ), this, SLOT( UpdateSubjectColumnIDAfterFileEdition( int ) ) );
}

void MainWindow::InitSubjectTab()
{
    /// Map of CheckBoxes to select the files we want to work on and
    /// SignalMapper to link them to the slot SortSubjects()
    m_paramTabFileCheckBoxMap.insert( m_data.GetAxialDiffusivityPrefix(), m_mainUi->para_adData_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetRadialDiffusivityPrefix(), m_mainUi->para_rdData_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetMeanDiffusivityPrefix(), m_mainUi->para_mdData_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetFractionalAnisotropyPrefix(), m_mainUi->para_faData_checkBox );
    m_paramTabFileCheckBoxMap.insert( m_data.GetCovariatesPrefix(), m_mainUi->para_compData_checkBox );
    QSignalMapper *signalMapperSelectFile = new QSignalMapper(this);
    QObject::connect( signalMapperSelectFile, SIGNAL( mapped( const QString& ) ), this, SLOT( SortSubjects() ) );
    for ( int i = 0; i < m_data.GetPrefixList().size(); ++i )
    {
        QObject::connect( m_paramTabFileCheckBoxMap[ m_data.GetPrefixList().at( i ) ], SIGNAL( toggled( const bool& ) ), signalMapperSelectFile,SLOT( map() ) );
        signalMapperSelectFile->setMapping( m_paramTabFileCheckBoxMap[ m_data.GetPrefixList().at( i ) ], m_data.GetPrefixList().at( i ) );
    }

    /// Map of Labels displaying the matrix data size of the files that have been chosen
    m_paramTabFileSizeLabelMap.insert( m_data.GetAxialDiffusivityPrefix(), m_mainUi->adDataSize_label );
    m_paramTabFileSizeLabelMap.insert( m_data.GetRadialDiffusivityPrefix(),m_mainUi->rdDataSize_label );
    m_paramTabFileSizeLabelMap.insert( m_data.GetMeanDiffusivityPrefix(), m_mainUi->mdDataSize_label );
    m_paramTabFileSizeLabelMap.insert( m_data.GetFractionalAnisotropyPrefix(), m_mainUi->faDataSize_label );
    m_paramTabFileSizeLabelMap.insert( m_data.GetCovariatesPrefix(), m_mainUi->compDataSize_label );
}


void MainWindow::UpdateCurrentDirEditInputDialog(const QString newfilePath, QString& currentDir )
{

    QDir dir = UpdateCurrentDir( newfilePath, currentDir );
    if( dir.exists() )
    {
        m_editInputDialog->SetCurrentInputDir( dir.absolutePath() );
    }
}

QDir MainWindow::UpdateCurrentDir( const QString newfilePath, QString& currentDir )
{
    QDir dir = QFileInfo( QFile( newfilePath ) ).absolutePath();
    if( dir.exists() )
    {
        currentDir = dir.absolutePath();
    }

    return dir;
}

void MainWindow::WarningPopUp( const QString warningText )
{
    QMessageBox::warning( this, tr( "WARNING" ), tr( qPrintable( warningText ) ), QMessageBox::Ok );
}

void MainWindow::CriticalPopUp( const QString criticalText )
{
    QMessageBox::critical( this, tr( "CRITICAL ERROR" ), tr( qPrintable( criticalText ) ), QMessageBox::Ok );
}

void MainWindow::DisplayIcon( QLabel *label , const QPixmap icon )
{
    label->setPixmap( icon.scaled( QSize( m_IconSize, m_IconSize ), Qt::IgnoreAspectRatio ) );
}


/***************************************************************/
/************************** Input tab **************************/
/***************************************************************/

/***********************  Private slots  ***********************/
void MainWindow::on_inputAddDataFiles_pushButton_clicked()
{
    QString dirPath = m_currentFileInputDir;
    QStringList fileList;
    fileList = QFileDialog::getOpenFileNames( this, tr( "Choose CSV files" ), dirPath, tr( ".csv( *.csv ) ;; .*( * )" ) );
    AddFiles( fileList );
}

void MainWindow::on_inputInfo_pushButton_clicked()
{
    m_infoDialog->setModal( true );
    m_infoDialog->setWindowTitle( tr( "Files Information" ) );
    m_infoDialog->DisplayInfo();
    m_infoDialog->exec();
}

void MainWindow::UpdateLineEditAfterFileEdition( const QString& newFilePathAfterFileEdition, const QString& prefID )
{
    m_inputTabFilePathLineEditMap[ prefID ]->setText( newFilePathAfterFileEdition );
}

void MainWindow::UpdateCovariatesAfterFileEdition( const QStringList& newCovariatesAfterFileEdition )
{
    m_data.ClearCovariatesList();
    m_data.SetCovariatesList() = newCovariatesAfterFileEdition;
    DisplayCovariatesList();
}

void MainWindow::UpdateSubjectColumnIDAfterFileEdition( const int&  newSubjectColumnIDAfterFileEdition )
{
    /// Subjects not on the 1st column anymore
    m_data.SetSubjectColumnID( newSubjectColumnIDAfterFileEdition );

    SetInfoSubjectColumnID();

    UpdateFileInformation( m_data.GetCovariatesPrefix() );
    SortSubjects();
}

void MainWindow::AddFile( const QString& prefID )
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

void MainWindow::EditFile( const QString& prefID )
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

void MainWindow::UpdateInputLineEdit( const QString& prefID )
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
}


/*********************** Private function ***********************/
void MainWindow::AddFiles( const QStringList fileList )
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

    QMap<QString, QStringList >::ConstIterator iter = map.begin();

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

void MainWindow::UpdateFileInformation( const QString prefID )
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

    DisplayCovariatesList();
    UpdateAvailableFileParamTab();
}

bool MainWindow::IsMatrixDimensionOK( const QList<QStringList> data )
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

void MainWindow::DisplayIcon( const QString prefID, const QPixmap icon )
{
    QLabel *label = m_inputTabIconLabelMap[ prefID ];
    label->setPixmap( icon.scaled( QSize( m_IconSize, m_IconSize ), Qt::IgnoreAspectRatio ) );
}

void MainWindow::SetInfoSubjectColumnID()
{
    QString filePath = m_inputTabFilePathLineEditMap[ m_data.GetCovariatesPrefix() ]->text();
    if( !filePath.isEmpty() && QFile( filePath ).open( QIODevice::ReadOnly ) )
    {
        QFile( filePath ).close();
        m_mainUi->subjectColumnID_label->setText( tr( qPrintable( "<b><i><span style=""font-size:7pt;"">" +
                                                      QString::number( m_data.GetSubjectColumnID() + 1 ) + "<i/><b/><span/>") ) );
    }
    else
    {
        m_mainUi->subjectColumnID_label->clear();
    }
}

void  MainWindow::LaunchEditInputWindow( QString prefID )
{
    m_editInputDialog->SetPrefix( prefID );
    m_editInputDialog->SetInputFile( m_inputTabFilePathLineEditMap[ prefID ]->text() );
    m_editInputDialog->setModal( true );
    m_editInputDialog->setWindowTitle( tr( qPrintable( "Edit " + prefID + " File" ) ) );
    m_editInputDialog->DisplayData();
    m_editInputDialog->exec();
}


/****************************************************************/
/************************* Subjects tab *************************/
/****************************************************************/

/*********************** Public  function ***********************/


/***********************  Private slots  ************************/
void MainWindow::on_loadInputSubjectList_PushButton_clicked()
{
    QLineEdit *lineEdit = m_mainUi->para_inputSubjectList_lineEdit;
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

void MainWindow::on_reset_pushButton_clicked()
{
    m_mainUi->para_inputSubjectList_lineEdit->clear();
}

void MainWindow::on_para_inputSubjectList_lineEdit_textChanged( const QString& textLineEdit )
{
    if( !textLineEdit.isEmpty() )
    {
        UpdateCurrentDir( textLineEdit, m_currentSubjectListInputDir );

    }

    SortSubjects();
}

void MainWindow::on_saveCheckedSubjectList_pushButton_clicked()
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

        QString previousFilePath = m_mainUi->para_inputSubjectList_lineEdit->text();
        m_mainUi->para_inputSubjectList_lineEdit->setText( filePath );
        if( previousFilePath == filePath )
        {
            /// If filePath does not change, an update is needed to display the right subjects list
            SortSubjects();
        }
    }
}


void MainWindow::on_checkAllVisible_pushButton_clicked()
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

void MainWindow::on_unCheckAllVisible_pushButton_clicked()
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

void MainWindow::on_sortedSubjects_listWidget_itemClicked( QListWidgetItem *item )
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
}

void MainWindow::on_para_search_lineEdit_textEdited()
{
    Search();
}

void MainWindow::on_caseSensitive_checkBox_toggled( bool checked )
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


void MainWindow::SortSubjects()
{
    QStringList refSubjectList = GetRefSubjectList( m_mainUi->para_inputSubjectList_lineEdit->text() );
    QMap<QString, QStringList> subjectListFromSelectedInputFile = GetAllSubjectsFromSelectedInputFiles();
    QMap< QString, QMap<QString, bool> > sortedSubjects = m_processing.FindSubjectInInputFile( refSubjectList, subjectListFromSelectedInputFile );
    QStringList matchedSubjectList;
    QMap<QString, QStringList > unMatchedSubjectList;
    m_processing.AssignSortedSubject( sortedSubjects, matchedSubjectList, unMatchedSubjectList, m_mainUi->para_inputSubjectList_lineEdit->text() );
    DisplaySortedSubjectList( refSubjectList, matchedSubjectList, unMatchedSubjectList );
    Search();
}


/*********************** Private function ***********************/
void MainWindow::UpdateAvailableFileParamTab()
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

QStringList MainWindow::GetRefSubjectList( const QString subjectListFilePath )
{
    QFile subjectFile( subjectListFilePath );
    QStringList refSubjectList;
    QLabel *label = m_mainUi->iconLoadList_label;
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

        QStringList selectedInputFiles = GetSelectedInputFiles();
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

QMap<QString, QStringList> MainWindow::GetAllSubjectsFromSelectedInputFiles()
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

void MainWindow::DisplaySortedSubjectList( const QStringList refSubjectList, const QStringList matchedSubjectList, const QMap<QString, QStringList > unMatchedSubjectList )
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

    m_mainUi->sortSubjects_label->clear();
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
    m_mainUi->sortSubjects_label->setText( text );
}

void MainWindow::Search()
{
    QListWidget *list = m_sortedSubjectListWidget;
    QLineEdit *lineEdit = m_mainUi->para_search_lineEdit;
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


/****************************************************************/
/************************ Parameters tab ************************/
/****************************************************************/

/***********************  Private  slots  ***********************/
void MainWindow::on_covariates_listWidget_itemClicked( QListWidgetItem *item )
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
}

void MainWindow::on_covariatesCheckAll_pushButton_clicked()
{
    QListWidget *list = m_covariatesListWidget;
    for( int i = 0; i < list->count(); i++ )
    {
        list->item( i )->setCheckState( Qt::Checked );
    }
}

void MainWindow::on_covariatesUncheckAll_pushButton_clicked()
{
    QListWidget *list = m_covariatesListWidget;
    for( int i = 0; i < list->count(); i++ )
    {
        QListWidgetItem *currentItem = list->item( i );
        if( !currentItem->text().contains( "Intercept" ) )
        {
            currentItem->setCheckState( Qt::Unchecked );
        }
    }
}


/*********************** Private function ***********************/
void MainWindow::DisplayCovariatesList()
{
    m_covariatesListWidget->clear();
    if( !( m_data.GetCovariatesList().isEmpty() ) )
    {
        foreach( QString covar, m_data.GetCovariatesList() )
        {
            QListWidgetItem *listItem = new QListWidgetItem( covar, m_covariatesListWidget );
            listItem->setCheckState( Qt::Checked );
            listItem->setFlags( Qt::ItemIsEnabled );
            m_covariatesListWidget->addItem( listItem );
        }
    }
}


/****************************************************************/
/*************************** Run  tab ***************************/
/****************************************************************/

/*********************** Public  function ***********************/

/***********************  Private  slots  ***********************/
void MainWindow::on_outputDir_pushButton_clicked()
{
    QLineEdit *lineEdit = m_mainUi->para_outputDir_lineEdit;
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

void MainWindow::on_para_outputDir_lineEdit_textChanged( const QString&  textLineEdit )
{
    QLabel *label = m_mainUi->iconOutputDir_label;
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
}

void MainWindow::on_run_pushButton_clicked()
{
    m_processing.ResetProcessing();

    QStringList selectedInputFiles = GetSelectedInputFiles();

    QString selectedSubjectListFilePath = GenerateSelectedSubjectList();

    QList<int> selectedCovariates = GetSelectedCovariates();

    QStringList matlabFiles = m_processing.GenerateMatlabInputFiles( selectedInputFiles, selectedSubjectListFilePath,
                                                   m_data.GetSubjectColumnID(), selectedCovariates, m_data.GetOutputDir() );
}

/*********************** Private function ***********************/
QString MainWindow::GenerateSelectedSubjectList()
{
    QFile selectedSubjects( m_data.GetOutputDir() + "/selectedSubjectList.txt" );
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

QStringList MainWindow::GetSelectedInputFiles()
{
    QStringList selectedInputFiles;
    foreach ( QString prefID, m_data.GetPrefixList() )
    {
        if( m_paramTabFileCheckBoxMap[ prefID ]->isChecked() )
        {
            selectedInputFiles.append( m_data.GetFilename( prefID ) );
        }
    }
    return selectedInputFiles;
}


QList<int> MainWindow::GetSelectedCovariates()
{
    QList<int> selectedCovariates;
    QListWidget *list = m_covariatesListWidget;
    for( int i = 0; i < list->count(); i++ )
    {
        QListWidgetItem *currentItem = list->item( i );
        if( !currentItem->text().contains( "Intercept" ) && currentItem->checkState() == Qt::Checked )
        {
            selectedCovariates.append( i );
        }
    }
    return selectedCovariates;
}
