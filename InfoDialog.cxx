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
    delete m_ui;
}


/***************************************************************/
/********************** Public functions ***********************/
/***************************************************************/
void InfoDialog::DisplayFileInformation()
{
    SetFileInformationLabelMap();
    foreach( QString prefID, m_data->GetPrefixList() )
    {
        m_inputFileInformationLabelMap.value( prefID )->
                setText( tr( qPrintable( "<center><b>" + prefID.toUpper() + " File</b></center><br>" + GetInputFileInformation( prefID ) ) ) );
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

void InfoDialog::SetFileInformationLabelMap()
{
    m_inputFileInformationLabelMap.insert( m_data->GetAxialDiffusivityPrefix(), m_adFileInfo_label );
    m_inputFileInformationLabelMap.insert( m_data->GetRadialDiffusivityPrefix(), m_rdFileInfo_label );
    m_inputFileInformationLabelMap.insert( m_data->GetMeanDiffusivityPrefix(), m_mdFileInfo_label );
    m_inputFileInformationLabelMap.insert( m_data->GetFractionalAnisotropyPrefix(), m_ui->faFileInfo_label );
    m_inputFileInformationLabelMap.insert( m_data->GetCovariatePrefix(), m_compFileInfo_label );
}

QString InfoDialog::GetInputFileInformation( const QString prefID )
{
    QString fileInformation;
    fileInformation.append( tr ( "<i>No File Information.<br>Please select a correct data file</i>" ) );

    const QString filename = m_data->GetFilename( prefID );
    QString fileName =  QFileInfo( QFile( filename ) ).fileName();
    if( !fileName.isEmpty() )
    {
        int nbRows = m_data->GetNbrRows( prefID );
        int nbColumns = m_data->GetNbrColumns( prefID );

        fileInformation.clear();
        fileInformation.append( tr( qPrintable( "<b>Filename</b> " + fileName + "<br>" ) ) );
        fileInformation.append( tr( qPrintable( "<b>Number of test subjects</b>  " + QString::number( m_data->GetNbrSubjects( prefID ) ) + "<br>" ) ) );
        fileInformation.append( tr( qPrintable( "<b>Data matrix</b>  " + QString::number( nbRows ) + "x" + QString::number( nbColumns )  + "<br>" ) ) );
        if( prefID == m_data->GetCovariatePrefix() )
        {
            fileInformation.append( tr( qPrintable( "<b>Number of covariates</b>  " + QString::number( m_data->GetCovariates().size()-1 ) ) ) );
            for( int c = 0; c < m_data->GetCovariates().size(); ++c )
            {
                if( c != m_data->GetCovariateFileSubjectColumnID() )
                {
                    fileInformation.append( tr( qPrintable( "<br>-  " + m_data->GetCovariates().value( c ) ) ) );
                }
            }
        }
    }

    return fileInformation;
}
