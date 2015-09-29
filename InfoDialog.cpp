#include "InfoDialog.h"
#include "ui_InfoDialog.h"

InfoDialog::InfoDialog( QWidget *parent ) :
    QDialog( parent ),
    m_ui( new Ui::InfoDialog )
{
    m_ui->setupUi( this );
}

InfoDialog::~InfoDialog()
{
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
/************************ Private slots ************************/
/***************************************************************/

/***************************************************************/
/********************** Private functions **********************/
/***************************************************************/
QString InfoDialog::GetFileInfo( const QString p )
{
    QString str;
    str.append( tr ( "<i>No File Information.<br>Please select a correct data file</i>" ) );
    const QString filename = m_data->GetFilename( p );
    QFile file( filename );

    if( file.open( QIODevice::ReadOnly ) )
    {
        int nbRows = m_data->GetNbrRows( p );
        int nbColumns = m_data->GetNbrColumns( p );

        str.clear();
        str.append( tr( qPrintable( "<b>Filename</b> " + QFileInfo( QFile( filename ) ).fileName() + "<br>" ) ) );
        if( p == m_data->GetCovariatesPrefix() )
        {
            str.append( tr( qPrintable( "<b>Number of test subjects</b>  " + QString::number( m_data->GetNbrSubjects( p ) ) + "<br>" ) ) );
            str.append( tr( qPrintable( "<b>Data matrix</b>  " + QString::number( nbRows ) + "x" + QString::number( nbColumns ) + "<br>" ) ) );
            str.append( tr( qPrintable( "<b>Number of COMP</b>  " + QString::number( m_data->GetCovariatesList().size()-1 ) ) ) );
            for( int c = 1; c < m_data->GetCovariatesList().size(); ++c )
            {
                str.append( tr( qPrintable( "<br>-  " + m_data->GetCovariatesList().at( c ) ) ) );
            }
        }
        else
        {
            if( m_data->GetPrefixList().contains( p ) )
            {
                str.append( tr( qPrintable( "<b>Number of subjects</b>  " + QString::number( m_data->GetNbrSubjects( p ) ) + "<br>" ) ) );
                str.append( tr( qPrintable( "<b>Data matrix</b>  " + QString::number( nbRows ) + "x" + QString::number( nbColumns ) ) ) );
            }
        }
    }
    return str;
}


void InfoDialog::SetInfoFileLabelMap()
{
    m_fileInfoLabelMap.insert( m_data->GetAxialDiffusivityPrefix(), m_ui->adFileInfo_label );
    m_fileInfoLabelMap.insert( m_data->GetRadialDiffusivityPrefix(), m_ui->rdFileInfo_label );
    m_fileInfoLabelMap.insert( m_data->GetMeanDiffusivityPrefix(), m_ui->mdFileInfo_label );
    m_fileInfoLabelMap.insert( m_data->GetFractionalAnisotropyPrefix(), m_ui->faFileInfo_label );
    m_fileInfoLabelMap.insert( m_data->GetCovariatesPrefix(), m_ui->compFileInfo_label );
}
