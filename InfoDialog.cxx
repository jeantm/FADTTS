#include "InfoDialog.h"
#include "ui_InfoDialog.h"

InfoDialog::InfoDialog( QWidget *parent ) :
    QDialog( parent ),
    m_ui( new Ui::InfoDialog )
{
    m_ui->setupUi( this );

    InitInfoDialog();
}

InfoDialog::~InfoDialog()
{
    delete m_adFileInfo_label;
    delete m_rdFileInfo_label;
    delete m_mdFileInfo_label;
    delete m_faFileInfo_label;
    delete m_compFileInfo_label;
    delete m_ui;
}


/***************************************************************/
/********************** Public functions ***********************/
/***************************************************************/
void InfoDialog::DisplayInfo()
{
    SetInfoFileLabelMap();
    foreach( QString prefix, m_data->GetPrefixList() )
    {
        m_fileInfoLabelMap.value( prefix )->setText( tr( qPrintable( "<center><b>" + prefix.toUpper() + " File</b></center><br>" + GetFileInfo( prefix ) ) ) );
    }
}

void InfoDialog::SetData( Data *newData )
{
    m_data = newData;
}


/***************************************************************/
/********************** Private functions **********************/
/***************************************************************/
void InfoDialog::InitInfoDialog()
{
    m_adFileInfo_label = new QLabel();
    m_adFileInfo_label = m_ui->adFileInfo_label;
    m_rdFileInfo_label = new QLabel();
    m_rdFileInfo_label = m_ui->rdFileInfo_label;
    m_mdFileInfo_label = new QLabel();
    m_mdFileInfo_label = m_ui->mdFileInfo_label;
    m_faFileInfo_label = new QLabel();
    m_faFileInfo_label = m_ui->faFileInfo_label;
    m_compFileInfo_label = new QLabel();
    m_compFileInfo_label = m_ui->compFileInfo_label;
}

void InfoDialog::SetInfoFileLabelMap()
{
    m_fileInfoLabelMap.insert( m_data->GetAxialDiffusivityPrefix(), m_adFileInfo_label );
    m_fileInfoLabelMap.insert( m_data->GetRadialDiffusivityPrefix(), m_rdFileInfo_label );
    m_fileInfoLabelMap.insert( m_data->GetMeanDiffusivityPrefix(), m_mdFileInfo_label );
    m_fileInfoLabelMap.insert( m_data->GetFractionalAnisotropyPrefix(), m_ui->faFileInfo_label );
    m_fileInfoLabelMap.insert( m_data->GetCovariatePrefix(), m_compFileInfo_label );
}

QString InfoDialog::GetFileInfo( const QString p )
{
    QString str;
    str.append( tr ( "<i>No File Information.<br>Please select a correct data file</i>" ) );

    const QString filename = m_data->GetFilename( p );
    QString fileName =  QFileInfo( QFile( filename ) ).fileName();
    if( !fileName.isEmpty() )
    {
        int nbRows = m_data->GetNbrRows( p );
        int nbColumns = m_data->GetNbrColumns( p );

        str.clear();
        str.append( tr( qPrintable( "<b>Filename</b> " + fileName + "<br>" ) ) );
        str.append( tr( qPrintable( "<b>Number of test subjects</b>  " + QString::number( m_data->GetNbrSubjects( p ) ) + "<br>" ) ) );
        str.append( tr( qPrintable( "<b>Data matrix</b>  " + QString::number( nbRows ) + "x" + QString::number( nbColumns )  + "<br>" ) ) );
        if( p == m_data->GetCovariatePrefix() )
        {
            str.append( tr( qPrintable( "<b>Number of covariates</b>  " + QString::number( m_data->GetCovariatesList().size()-1 ) ) ) );
            for( int c = 0; c < m_data->GetCovariatesList().size(); ++c )
            {
                if( c != m_data->GetSubjectColumnID() )
                {
                    str.append( tr( qPrintable( "<br>-  " + m_data->GetCovariatesList().value( c ) ) ) );
                }
            }
        }
    }

    return str;
}
