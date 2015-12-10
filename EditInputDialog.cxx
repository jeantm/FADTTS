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
    delete m_ui;
}

/***************************************************************/
/********************** Public functions ***********************/
/***************************************************************/
void EditInputDialog::DisplayFileData()
{
    UploadFileData();
}

void EditInputDialog::SetData( Data *newData )
{
    m_data = newData;
}

void EditInputDialog::SetLineEditPrefix( const QString &newLineEditPrefixID )
{
    m_lineEditPrefix = newLineEditPrefixID;
}

void EditInputDialog::SetInputFile( const QString &newInputFile )
{
    m_inputFile = newInputFile;
}

void EditInputDialog::SetCurrentInputDir( const QString &newCurrentInputDir )
{
    m_currentInputDir = newCurrentInputDir;
}

void EditInputDialog::ResetCovariateFileSubjectColumnID()
{
    m_covariateFileSubjectColumnIDSpinBox->setValue( 1 );
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
}

void EditInputDialog::ChangeCovariateFileSubjectColumnID( int idColumn )
{
    /** Allow the user to manage the position of the subjects in the data file.
     *  This position is transmited to the main window in anticipation of further data processing **/
    m_covariateFileSubjectColumnID = idColumn-1;
    emit CovariateFileSubjectColumnIDChanged( m_covariateFileSubjectColumnID );

}

bool EditInputDialog::SaveFile()
{
    QString filePath = QFileDialog::getSaveFileName( this, tr( qPrintable( "Save " + m_lineEditPrefix.toUpper() + " file as ..." ) ), m_currentInputDir, tr( ".csv( *.csv ) ;; .*( * )" ) );
    if( !filePath.isEmpty() )
    {
        QFile exportedCSV( filePath );
        exportedCSV.open( QIODevice::WriteOnly );
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
        emit FilePathChanged( m_inputFile, m_lineEditPrefix ); /** Update file path in mainWindow LineEdit **/

        m_columnDeleted = false;
        m_rowDeleted = false;
        return true;
    }
    else
    {
        return false;
    }
}


/***************************************************************/
/********************** Private functions **********************/
/***************************************************************/
void EditInputDialog::InitEditInputDialog()
{
    m_dataTableWidget = new QTableWidget;
    m_dataTableWidget = m_ui->EditInputDialog_data_tableWidget;
    m_dataTableWidget->setEditTriggers( QAbstractItemView::NoEditTriggers ); /** User unable to do any data modification in the tableWidget - except delete **/

    m_covariateFileSubjectColumnIDSpinBox = new QSpinBox;
    m_covariateFileSubjectColumnIDSpinBox = m_ui->EditInputDialog_subjectColumnID_spinBox;

    m_rowDeleted = false;
    m_columnDeleted = false;
    m_covariateFileSubjectColumnID = 0;

    connect( m_ui->EditInputDialog_deleteRows_pushButton, SIGNAL( clicked() ), SLOT( DeleteRows() ) );
    connect( m_ui->EditInputDialog_deleteColumns_pushButton, SIGNAL( clicked() ), SLOT( DeleteColumns() ) );
    connect( m_covariateFileSubjectColumnIDSpinBox, SIGNAL( valueChanged( int ) ), SLOT( ChangeCovariateFileSubjectColumnID( int ) ) );
    connect( m_ui->EditInputDialog_saveFile_pushButton, SIGNAL( clicked() ), SLOT( SaveFile() ) );
}

void EditInputDialog::UploadFileData()
{
    Processing process;
    QList<QStringList> fileData = m_data->GetFileData( m_lineEditPrefix );
    m_isCovariateFile = process.IsCovariateFile( fileData.at( 1 ) );
    int nbrRows = 0;
    int nbrColumns = 0;

    m_covariateFileSubjectColumnIDSpinBox->setMaximum( fileData.at( 0 ).count() ); /** Limite the column seleection to the number of covariates **/

    m_dataTableWidget->setRowCount( fileData.count() ); /** The number of StringLists gives the number of row **/
    m_dataTableWidget->setColumnCount( fileData.at( 0 ).count() ); /** The number of String from intial StringList gives the number of column **/

    m_dataTableWidget->setUpdatesEnabled( false );
    foreach( QStringList row, fileData )
    {
        foreach( QString data, row )
        {
            /** Remove quotes if element quoted **/
            if( data.endsWith( '"' ) )
            {
                data.chop( 1 );
            }
            if( data.startsWith( '"' ) )
            {
                data.remove( 0, 1 );
            }
            m_dataTableWidget->setItem( nbrRows, nbrColumns++, new QTableWidgetItem( data ) );
        }
        nbrRows++; nbrColumns=0;
    }
    m_dataTableWidget->setUpdatesEnabled( true );

    if( m_lineEditPrefix == m_data->GetCovariatePrefix() )
    {
        m_ui->EditInputDialog_subjectColumn_label->show();
        m_ui->EditInputDialog_subjectColumnID_spinBox->show();
    }
    else
    {
        m_ui->EditInputDialog_subjectColumn_label->hide();
        m_ui->EditInputDialog_subjectColumnID_spinBox->hide();
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
        QMessageBox::StandardButton closeMessageBox =
                QMessageBox::question( this, tr( "Edit File" ), tr( "The data have been modified.<br>Do you want to save your changes?" ),
                                       QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save );
        switch( closeMessageBox )
        {
        case QMessageBox::Save:
        {
            bool save = SaveFile();
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
            ResetCovariateFileSubjectColumnID();
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
