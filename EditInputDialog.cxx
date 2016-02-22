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
void EditInputDialog::SetData( Data *newData )
{
    m_data = newData;
}

QString& EditInputDialog::SetCurrentInputDir()
{
    return m_currentDir;
}

void EditInputDialog::ResetCovariateColumnID()
{
    m_covariateColumnIDSpinBox->setValue( 1 );
    /** Row and Column ID start at 1 for data display only.
     *  Otherwise they start at 0 (data modification, ...). **/
}


void EditInputDialog::DisplayDataEdition( const int &newDiffusionPropertyIndex )
{
    m_diffusionPropertyIndex = newDiffusionPropertyIndex;

    LoadData();
}


/***************************************************************/
/************************ Private slots ************************/
/***************************************************************/
void EditInputDialog::OnDeleteRows()
{
    QItemSelection currentSelection( m_dataTableWidget->selectionModel()->selection() );

    QList<int> rows;
    foreach( const QModelIndex &index, currentSelection.indexes() )
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

void EditInputDialog::OnDeleteColumns()
{
    QItemSelection currentSelection( m_dataTableWidget->selectionModel()->selection() );

    QList<int> columns;
    foreach( const QModelIndex &index, currentSelection.indexes() )
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


void EditInputDialog::OnCovariateColumnIDChanged( int columnID )
{
    /** Allow the user to change the position of the subjects in the data file.
     *  This position is transmited to the main window for further data processing **/
    m_covariateColumnID = columnID-1;
    emit UpdateCovariateColumnID( m_covariateColumnID );

}


bool EditInputDialog::OnSaveFile()
{
    QString newFilePath = QFileDialog::getSaveFileName( this, tr( qPrintable( "Save " + m_data->GetDiffusionPropertyName( m_diffusionPropertyIndex ).toUpper() + " file as ..." ) ),
                                                     m_currentDir + "/new" + m_data->GetDiffusionPropertyName( m_diffusionPropertyIndex ).toUpper() + "File.csv", tr( ".csv( *.csv ) ;; .*( * )" ) );
    if( !newFilePath.isEmpty() )
    {
        QFile exportedCSV( newFilePath );
        exportedCSV.open( QIODevice::WriteOnly );
        QTextStream ts( &exportedCSV );
        QStringList data;

        for( int row = 0; row < m_dataTableWidget->rowCount(); ++row )
        {
            data.clear();
            for( int column = 0; column < m_dataTableWidget->columnCount(); ++column )
            {
                data << m_dataTableWidget->item( row, column )->text();
            }
            ts << data.join( m_csvSeparator ) << endl;
        }
        exportedCSV.flush();
        exportedCSV.close();

        emit UpdateInputFile( m_diffusionPropertyIndex, newFilePath ); /** Update file path in mainWindow LineEdit **/

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

    m_covariateColumnIDSpinBox = new QSpinBox;
    m_covariateColumnIDSpinBox = m_ui->EditInputDialog_subjectColumnID_spinBox;

    m_rowDeleted = false;
    m_columnDeleted = false;
    m_covariateColumnID = 0;

    connect( m_ui->EditInputDialog_deleteRows_pushButton, SIGNAL( clicked() ), SLOT( OnDeleteRows() ) );
    connect( m_ui->EditInputDialog_deleteColumns_pushButton, SIGNAL( clicked() ), SLOT( OnDeleteColumns() ) );
    connect( m_covariateColumnIDSpinBox, SIGNAL( valueChanged( int ) ), SLOT( OnCovariateColumnIDChanged( int ) ) );
    connect( m_ui->EditInputDialog_saveFile_pushButton, SIGNAL( clicked() ), SLOT( OnSaveFile() ) );
}

void EditInputDialog::LoadData()
{
    QList<QStringList> fileData = m_data->GetFileData( m_diffusionPropertyIndex );
    int nbrRows = 0;
    int nbrColumns = 0;

    m_covariateColumnIDSpinBox->setMaximum( fileData.first().count() ); /** Limite the column selection to the number of covariates **/

    m_dataTableWidget->setRowCount( fileData.count() ); /** The number of StringLists gives the number of row **/
    m_dataTableWidget->setColumnCount( fileData.first().count() ); /** The number of Strings from initial StringList gives the number of column **/

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

    if( m_diffusionPropertyIndex == m_data->GetSubMatrixIndex() )
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
            bool save = OnSaveFile();
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
            ResetCovariateColumnID();
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
