#include "EditInputDialog.h"
#include "ui_EditInputDialog.h"

const QString EditInputDialog::m_csvSeparator = QLocale().groupSeparator();

EditInputDialog::EditInputDialog( QWidget *parent ) :
    QDialog( parent ),
    m_ui( new Ui::EditInputDialog )
{
    m_ui->setupUi( this );

    InitEditInputDialog();
}

EditInputDialog::~EditInputDialog()
{
    delete m_subjectColumnSpinBox;
    delete m_fileInformationLabel;
    delete m_subjectColumnLabel;
    delete m_dataTableWidget;
    delete m_ui;
}

/***************************************************************/
/********************** Public functions ***********************/
/***************************************************************/
void EditInputDialog::DisplayData()
{
    UploadData();
    RefreshFileInfo();
}

void EditInputDialog::SetData( Data *newData )
{
    m_data = newData;
}

void EditInputDialog::SetPrefix( const QString &prefixID )
{
    m_prefix = prefixID;
}

void EditInputDialog::SetInputFile( const QString &inputFile )
{
    m_inputFile = inputFile;
}

void EditInputDialog::SetCurrentInputDir( const QString &newCurrentInputDir )
{
    m_currentInputDir = newCurrentInputDir;
}

void EditInputDialog::ResetSubjectColumnID()
{
    m_subjectColumnSpinBox->setValue( 1 );
    /** Row and Column ID start at 1 for data display only.
     *  Otherwise they start at 0 (data modification, ...). **/
}


/***************************************************************/
/************************ Private slots ************************/
/***************************************************************/
void EditInputDialog::DeleteRows()
{
    QItemSelection selection( m_dataTableWidget->selectionModel()->selection() );

    QList<int> rows;
    foreach( const QModelIndex &index, selection.indexes() )
    {
        rows.append( index.row() );
    }
    qSort( rows );

    int prev = -1;
    for( int i = rows.count() - 1; i >= 0; i -= 1 )
    {
        int current = rows.at( i );
        if( current != prev )
        {
            m_dataTableWidget->removeRow( current );
            prev = current;
        }
    }
    m_rowDeleted = true;

    RefreshFileInfo();
}

void EditInputDialog::DeleteColumns()
{
    QItemSelection selection( m_dataTableWidget->selectionModel()->selection() );

    QList<int> columns;
    foreach( const QModelIndex &index, selection.indexes() )
    {
        columns.append( index.column() );
    }
    qSort( columns );

    int prev = -1;
    for( int i = columns.count() - 1; i >= 0; i -= 1 )
    {
        int current = columns.at( i );
        if( current != prev )
        {
            m_dataTableWidget->removeColumn( current );
            prev = current;
        }
    }
    m_columnDeleted = true;

    RefreshFileInfo();
}

void EditInputDialog::ChangeSubjectColumnID( int idColumn )
{
    /** Allow the user to manage the position of the subjects in the data file.
     *  This position is transmited to the main window in anticipation of further data processing **/
    m_subjectColumnID = idColumn-1;
    emit SubjectColumnIDChanged( m_subjectColumnID );
    RefreshFileInfo();
}

void EditInputDialog::Save()
{
    SaveCSVFile();
    RefreshFileInfo();
}


/***************************************************************/
/********************** Private functions **********************/
/***************************************************************/
void EditInputDialog::InitEditInputDialog()
{
    m_dataTableWidget = new QTableWidget;
    m_dataTableWidget = m_ui->data_tableWidget;
    m_dataTableWidget->setEditTriggers( QAbstractItemView::NoEditTriggers ); /** User unable to do any data modification in the tableWidget - except delete **/

    m_subjectColumnSpinBox = new QSpinBox;
    m_subjectColumnSpinBox = m_ui->para_subjectColumn_spinBox;

    m_fileInformationLabel = new QLabel;
    m_fileInformationLabel = m_ui->fileInformation_label;

    m_subjectColumnLabel = new QLabel;
    m_subjectColumnLabel = m_ui->subjectColumn_label;

    m_rowDeleted = false;
    m_columnDeleted = false;
    m_subjectColumnID = 0;

    connect( m_ui->deleteRows_pushButton, SIGNAL( clicked() ), SLOT( DeleteRows() ) );
    connect( m_ui->deleteColumns_pushButton, SIGNAL( clicked() ), SLOT( DeleteColumns() ) );
    connect( m_subjectColumnSpinBox, SIGNAL( valueChanged( int ) ), SLOT( ChangeSubjectColumnID( int ) ) );
    connect( m_ui->saveFile_pushButton, SIGNAL( clicked() ), SLOT( Save() ) );

}

void EditInputDialog::UploadData()
{
    Processing process;
    QList<QStringList> data = process.GetDataFromFile( m_inputFile );
    m_isCOMP = process.IsCOMPFile( data.at( 1 ) );
    int row = 0, col = 0;

    m_subjectColumnSpinBox->setMaximum( data.at( 0 ).count() ); /** Limite the column seleection to the number of covariates **/

    m_dataTableWidget->setRowCount( data.count() ); /** The number of StringLists gives the number of row **/
    m_dataTableWidget->setColumnCount( data.at( 0 ).count() ); /** The number of String from intial StringList gives the number of column **/

    m_dataTableWidget->setUpdatesEnabled( false );
    foreach( QStringList l, data )
    {
        foreach( QString str, l )
        {
            /** Remove quotes if str quoted **/
            if( str.endsWith( '"' ) )
            {
                str.chop( 1 );
            }
            if( str.startsWith( '"' ) )
            {
                str.remove( 0, 1 );
            }
            m_dataTableWidget->setItem( row, col++, new QTableWidgetItem( str ) );
        }
        row++; col=0;
    }
    m_dataTableWidget->setUpdatesEnabled( true );
}

void EditInputDialog::RefreshFileInfo()
{
    /** Refresh the file edition window and update the information displayed.
     *  Called everytime a modification is made **/

    QString str;
    m_subjectColumnSpinBox->setHidden( true );
    m_subjectColumnLabel->setHidden( true );

    int nbRows = m_dataTableWidget->rowCount();
    int nbColumns = m_dataTableWidget->columnCount();

    str.append( tr( qPrintable( "<br><br><b>Filename</b> " + QFileInfo( QFile( m_inputFile ) ).fileName() + "<br>" ) ) );
    if( nbRows == 0 || nbColumns == 0 )
    {
        str.append( tr( qPrintable( "<br>No data to display" ) ) );
    }
    else
    {
        if( m_prefix == m_data->GetCovariatePrefix() )
        {
            /** Subject column selection only available for covariate files **/
            m_subjectColumnSpinBox->setHidden( false );
            m_subjectColumnLabel->setHidden( false );

            if( m_isCOMP ) /** The file matches the expected file structure **/
            {
                str.append( tr( qPrintable( "<br><b>Number of test subjects</b>  " + QString::number( nbRows-1 ) + "<br>" ) ) );
                str.append( tr( qPrintable( "<br><b>Data matrix</b>  " + QString::number( nbRows-1 ) + "x" + QString::number( nbColumns-1 ) + "<br>" ) ) );
                str.append( tr( qPrintable( "<br><b>Number of covariates</b>  " + QString::number( nbColumns-1 ) ) ) );
                m_covariates.clear();
                for( int c = 0; c < nbColumns; ++c )
                {
                    if( c != m_subjectColumnID )
                    {
                        QString cov = m_dataTableWidget->item( 0, c )->text();
                        str.append( tr( qPrintable( "<br>-  " + cov ) ) );
                        m_covariates.insert( c, cov );
                    }
                }
                m_covariates.insert( -1, tr( "Intercept" ) );
                emit CovariatesChanged( m_covariates ); /** The new covariates ( column ID and name ) are sent to the main window **/
            }
            else
            {
                str.append( tr( qPrintable( "<br><center><i>WARNING<br><br>Please make sure you have uploaded<br>a " + m_prefix.toUpper() + " file</i></center>" ) ) );
            }
        }
        else /** AD/RD/MD or FA file edition **/
        {
            if( !m_isCOMP ) /** The file matches the expected file structure **/
            {
                str.append( tr( qPrintable( "<br><b>Number of test subjects</b>  " + QString::number( nbColumns-1 ) + "<br>" ) ) );
                str.append( tr( qPrintable( "<br><b>Data matrix</b>  " + QString::number( nbRows-1 ) + "x" + QString::number( nbColumns ) ) ) );
            }
            else
            {
                str.append( tr( qPrintable( "<br><center><i>WARNING<br><br>Please make sure you have uploaded<br>a(n) " + m_prefix.toUpper() + " file</i></center>" ) ) );
            }
        }
    }
    m_fileInformationLabel->setText( str );
}

bool EditInputDialog::SaveCSVFile()
{
    QString filePath = QFileDialog::getSaveFileName( this, tr( qPrintable( "Save " + m_prefix.toUpper() + " file as ..." ) ), m_currentInputDir, tr( ".csv( *.csv ) ;; .*( * )" ) );
    QFile exportedCSV( filePath );
    if( exportedCSV.open( QIODevice::WriteOnly ) )
    {
        QTextStream ts( &exportedCSV );
        QStringList data;

        for( int r = 0; r < m_dataTableWidget->rowCount(); ++r )
        {
            data.clear();
            for( int c = 0; c < m_dataTableWidget->columnCount(); ++c )
            {
                data << m_dataTableWidget->item( r, c )->text();
            }
            ts << data.join( m_csvSeparator ) << endl;
        }
        exportedCSV.flush();
        exportedCSV.close();

        m_inputFile = filePath;
        emit FilePathChanged( m_inputFile, m_prefix ); /** Update file path in main window LineEdit **/

        m_columnDeleted = false;
        m_rowDeleted = false;
        return true;
    }
    else
    {
        return false;
    }
}

void EditInputDialog::ResetTableWidget()
{
    m_rowDeleted = false;
    m_columnDeleted = false;
    m_dataTableWidget->clear();
    m_dataTableWidget->setRowCount( 0 );
    m_dataTableWidget->setColumnCount( 0 );
}

void EditInputDialog::closeEvent( QCloseEvent *event )
{
    if( m_rowDeleted || m_columnDeleted )
    {
        QMessageBox::StandardButton closeMBox = QMessageBox::question( this,
                                                                       tr( "Edit File" ),
                                                                       tr( "The data have been modified.<br>Do you want to save your changes?" ),
                                                                       QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                                                       QMessageBox::Save );
        switch( closeMBox )
        {
        case QMessageBox::Save:
        {
            bool save = SaveCSVFile();
            if( save )
            {
                ResetTableWidget();
                event->accept();
            }
            else
            {
                event->ignore();
            }
        }
            break;
        case QMessageBox::Discard:
            ResetTableWidget();
            ResetSubjectColumnID();
            event->accept();
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
        default:
            event->accept();
            break;
        }
    }
    else
    {
        ResetTableWidget();
        event->accept();
    }
}
