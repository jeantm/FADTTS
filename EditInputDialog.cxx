#include "EditInputDialog.h"
#include "ui_EditInputDialog.h"

//#include <QDebug>

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

void EditInputDialog::ResetSubjectColumnID()
{
    m_subjectColumnIDSpinBox->setValue( 1 );
    /** Row and Column ID start at 1 for data display only.
     *  Otherwise they start at 0 (data modification, ...). **/
}


void EditInputDialog::DisplayDataEdition( int newDiffusionPropertyIndex )
{
    m_diffusionPropertyIndex = newDiffusionPropertyIndex;

    LoadData();
    GetDuplicates();
}


/***************************************************************/
/************************ Private slots ************************/
/***************************************************************/
void EditInputDialog::OnDeleteRows()
{
    QItemSelection currentSelection( m_dataTableWidget->selectionModel()->selection() );

    QList< int > rows;
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

    QList< int > columns;
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
    m_subjectColumnIDSpinBox->setMaximum( m_dataTableWidget->columnCount() );
}


void EditInputDialog::OnRemoveDuplicates()
{
    if( !indexSecondeOccurrences.isEmpty() )
    {
        for( int i = indexSecondeOccurrences.size(); i > 0; i-- )
        {
            int currentIndex = indexSecondeOccurrences.at( i - 1 );

            if( m_diffusionPropertyIndex == m_data->GetSubMatrixIndex() )
            {
                m_dataTableWidget->removeRow( currentIndex );
                m_rowDeleted = true;
            }
            else
            {
                m_dataTableWidget->removeColumn( currentIndex );
                m_columnDeleted = true;
            }

        }

        indexSecondeOccurrences.clear();

        m_ui->EditInputDialog_removeDuplicates_info_label->setEnabled( false );
        m_ui->EditInputDialog_removeDuplicates_pushButton->setEnabled( false );
        m_ui->EditInputDialog_removeDuplicates_label->setEnabled( false );
    }
}


void EditInputDialog::OnSubjectColumnIDChanged( int columnID )
{
    /** Allow the user to change the position of the subjects in the data file.
     *  This position is transmited to the main window for further data processing **/
    m_subjectColumnID = columnID-1;
    emit UpdateSubjectColumnID( m_subjectColumnID );

}


bool EditInputDialog::OnSaveFile()
{
    QString filename = m_data->GetFilename( m_diffusionPropertyIndex ).isEmpty() ?
                "file_" + m_data->GetDiffusionPropertyName( m_diffusionPropertyIndex ).toUpper() :
                QFileInfo( QFile( m_data->GetFilename( m_diffusionPropertyIndex ) ) ).fileName();
    QString newFilePath = QFileDialog::getSaveFileName( this, tr( qPrintable( "Save " + m_data->GetDiffusionPropertyName( m_diffusionPropertyIndex ).toUpper() + " file as ..." ) ),
                                                        QFileInfo( QFile( m_data->GetFilename( m_diffusionPropertyIndex ) ) ).absolutePath() + "/NEW_" + filename, tr( ".csv( *.csv ) ;; .*( * )" ) );
    if( !newFilePath.isEmpty() )
    {
        SaveFile( newFilePath );

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


/***************************************************************/
/********************** Private functions **********************/
/***************************************************************/
void EditInputDialog::InitEditInputDialog()
{
    m_dataTableWidget = new QTableWidget;
    m_dataTableWidget = m_ui->EditInputDialog_data_tableWidget;
    m_dataTableWidget->setEditTriggers( QAbstractItemView::NoEditTriggers ); /** User unable to do any data modification in the tableWidget - except delete **/

    m_subjectColumnIDSpinBox = new QSpinBox;
    m_subjectColumnIDSpinBox = m_ui->EditInputDialog_subjectColumnID_spinBox;

    m_rowDeleted = false;
    m_columnDeleted = false;
    m_subjectColumnID = 0;

    connect( m_ui->EditInputDialog_deleteRows_pushButton, SIGNAL( clicked() ), SLOT( OnDeleteRows() ) );
    connect( m_ui->EditInputDialog_deleteColumns_pushButton, SIGNAL( clicked() ), SLOT( OnDeleteColumns() ) );
    connect( m_ui->EditInputDialog_removeDuplicates_pushButton, SIGNAL( clicked() ), SLOT( OnRemoveDuplicates() ) );
    connect( m_subjectColumnIDSpinBox, SIGNAL( valueChanged( int ) ), SLOT( OnSubjectColumnIDChanged( int ) ) );
    connect( m_ui->EditInputDialog_saveFile_pushButton, SIGNAL( clicked() ), SLOT( OnSaveFile() ) );
}

void EditInputDialog::LoadData()
{
    QList< QStringList > fileData = m_data->GetFileData( m_diffusionPropertyIndex );
    int nbrRows = 0;
    int nbrColumns = 0;

    m_dataTableWidget->setRowCount( fileData.count() ); /** The number of StringLists gives the number of row **/
    m_dataTableWidget->setColumnCount( fileData.first().count() ); /** The number of Strings from initial StringList gives the number of column **/

    m_subjectColumnIDSpinBox->setMaximum( m_dataTableWidget->columnCount() ); /** Limite the column selection to the number of covariates **/

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
        m_ui->EditInputDialog_subjectColumn_label->setEnabled( true );
        m_subjectColumnIDSpinBox->setEnabled( true );
    }
    else
    {
        m_ui->EditInputDialog_subjectColumn_label->setEnabled( false );
        m_subjectColumnIDSpinBox->setEnabled( false );
    }
}

void EditInputDialog::GetDuplicates()
{
    firstOccurrences.clear();
    indexSecondeOccurrences.clear();

    bool isSubMatrix  = m_diffusionPropertyIndex == m_data->GetSubMatrixIndex() ? true : false;
    int nbrSubjects = isSubMatrix ? m_dataTableWidget->rowCount() : m_dataTableWidget->columnCount();

    for( int i = 0; i < nbrSubjects; i++ )
    {
        QString currentSubject = m_dataTableWidget->item( isSubMatrix ? i : 0, isSubMatrix ? 0 : i )->text();
        if( !firstOccurrences.contains( currentSubject ) )
        {
            firstOccurrences.append( currentSubject );
        }
        else
        {
            indexSecondeOccurrences.append( i );
        }
    }

    bool duplicatesFound = !indexSecondeOccurrences.isEmpty();
    if( duplicatesFound )
    {
        QString nbrDuplicatesFound = tr( qPrintable( "<i><span style=""font-size:8pt;"">" + QString::number( indexSecondeOccurrences.size() ) + " duplicate(s) found</span></i>" ) );
        m_ui->EditInputDialog_removeDuplicates_info_label->setText( nbrDuplicatesFound );
    }
    m_ui->EditInputDialog_removeDuplicates_info_label->setEnabled( duplicatesFound );
    m_ui->EditInputDialog_removeDuplicates_pushButton->setEnabled( duplicatesFound );
    m_ui->EditInputDialog_removeDuplicates_label->setEnabled( duplicatesFound );
}

void EditInputDialog::ResetTableWidget()
{
    m_rowDeleted = false;
    m_columnDeleted = false;
    m_dataTableWidget->clear();
    m_dataTableWidget->setRowCount( 0 );
    m_dataTableWidget->setColumnCount( 0 );
}


void EditInputDialog::SaveFile( QString newFilePath )
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
}
