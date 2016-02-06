#include "InfoDialog.h"


/***************************************************************/
/********************** Public functions ***********************/
/***************************************************************/
InfoDialog::InfoDialog( QObject *parent ) :
    QObject( parent )
{
}


void InfoDialog::DisplayFileInformation()
{
    foreach( QString prefID, m_data->GetPrefixList() )
    {
        m_inputFileInformationLabelMap.value( prefID )->
                setText( tr( qPrintable( GetInputFileInformation( prefID ) ) ) );
    }
}

void InfoDialog::SetData( Data *data )
{
    m_data = data;
}

void InfoDialog::SetInformationLabelMap( QMap<QString, QLabel *> informationLabelMap )
{
    m_inputFileInformationLabelMap = informationLabelMap;
}

/***************************************************************/
/********************** Private functions **********************/
/***************************************************************/
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
        fileInformation.append( tr( qPrintable( "Filename: <i>" + fileName + "</i><br>" ) ) );
        fileInformation.append( tr( qPrintable( "Number of test subjects: <i>" + QString::number( m_data->GetNbrSubjects( prefID ) ) + "</i><br>" ) ) );
        fileInformation.append( tr( qPrintable( "Data matrix: <i>" + QString::number( nbRows ) + "x" + QString::number( nbColumns )  + "</i><br>" ) ) );
        if( prefID == m_data->GetCovariatePrefix() )
        {
            fileInformation.append( tr( qPrintable( "Number of covariates: <i>" + QString::number( m_data->GetCovariates().size()-1 )  + "</i>" ) ) );
            for( int c = 0; c < m_data->GetCovariates().size(); ++c )
            {
                if( c != m_data->GetCovariateFileSubjectColumnID() )
                {
                    fileInformation.append( tr( qPrintable( "<br>- <i>" + m_data->GetCovariates().value( c ) + "</i>" ) ) );
                }
            }
        }
    }

    return fileInformation;
}
