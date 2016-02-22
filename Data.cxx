#include "Data.h"

Data::Data()
{
}


void Data::InitData()
{
    foreach( int diffusionPropertyIndex, GetDiffusionPropertiesIndices() )
    {
        m_filenameMap[ diffusionPropertyIndex ];
        m_fileDataMap[ diffusionPropertyIndex ];
        m_nbrRowsMap[ diffusionPropertyIndex ];
        m_nbrColumnsMap[ diffusionPropertyIndex ];
        m_subjectMap[ diffusionPropertyIndex ];
        m_nbrSubjectsMap[ diffusionPropertyIndex ];
    }

    m_covariateColumnID = 0;
}



/*************** Getters ***************/
QList<int> Data::GetDiffusionPropertiesIndices() const
{
    QList<int> diffusionPropertiesIndices = QList<int>() << AD << RD << MD << FA << SubMatrix;
    return diffusionPropertiesIndices;
}

int Data::GetAxialDiffusivityIndex() const
{
    return AD;
}

int Data::GetRadialDiffusivityIndex() const
{
    return RD;
}

int Data::GetMeanDiffusivityIndex() const
{
    return MD;
}

int Data::GetFractionalAnisotropyIndex() const
{
    return FA;
}

int Data::GetSubMatrixIndex() const
{
    return SubMatrix;
}

QString Data::GetDiffusionPropertyName( int diffusionPropertyIndex ) const
{
    switch( diffusionPropertyIndex )
    {
    case AD:
        return "ad";
    case RD:
        return "rd";
    case MD:
        return "md";
    case FA:
        return "fa";
    case SubMatrix:
        return "subMatrix";
    default:
        return "No valid diffussion property";
    }
}


QString Data::GetFilename( int diffusionPropertyIndex ) const
{
    return m_filenameMap[ diffusionPropertyIndex ];
}

QList<QStringList> Data::GetFileData( int diffusionPropertyIndex ) const
{
    return m_fileDataMap[ diffusionPropertyIndex ];
}

int Data::GetNbrRows( int diffusionPropertyIndex ) const
{
    return m_nbrRowsMap[ diffusionPropertyIndex ];
}

int Data::GetNbrColumns( int diffusionPropertyIndex ) const
{
    return m_nbrColumnsMap[ diffusionPropertyIndex ];
}

QMap<int, QStringList> Data::GetSubjects() const
{
    return m_subjectMap;
}

int Data::GetNbrSubjects( int diffusionPropertyIndex ) const
{
    return m_nbrSubjectsMap[ diffusionPropertyIndex ];
}


QMap<int, QString> Data::GetCovariates() const
{
    return m_covariateMap;
}

int Data::GetCovariateColumnID() const
{
    return m_covariateColumnID;
}

QString Data::GetOutputDir() const
{
    return m_outputDir;
}



/*************** Setters ***************/
QString& Data::SetFilename( int diffusionPropertyIndex )
{
    return m_filenameMap[ diffusionPropertyIndex ];
}

QList<QStringList>& Data::SetFileData( int diffusionPropertyIndex )
{
    return m_fileDataMap[ diffusionPropertyIndex ];
}

int& Data::SetNbrRows( int diffusionPropertyIndex )
{
    return m_nbrRowsMap[ diffusionPropertyIndex ];
}

int& Data::SetNbrColumns( int diffusionPropertyIndex )
{
    return m_nbrColumnsMap[ diffusionPropertyIndex ];
}

QStringList& Data::SetSubjects( int diffusionPropertyIndex )
{
    return m_subjectMap[ diffusionPropertyIndex ];
}

int& Data::SetNbrSubjects( int diffusionPropertyIndex )
{
    return m_nbrSubjectsMap[ diffusionPropertyIndex ];
}


QMap<int, QString>& Data::SetCovariates()
{
    return m_covariateMap;
}

int& Data::SetCovariateColumnID()
{
    return m_covariateColumnID;
}

QString& Data::SetOutputDir()
{
    return m_outputDir;
}



void Data::ClearFileInformation( int diffusionPropertyIndex )
{
    m_filenameMap[ diffusionPropertyIndex ].clear();
    m_fileDataMap[ diffusionPropertyIndex ].clear();
    m_nbrRowsMap[ diffusionPropertyIndex ] = 0;
    m_nbrColumnsMap[ diffusionPropertyIndex ] = 0;
    m_subjectMap[ diffusionPropertyIndex ].clear();
    m_nbrSubjectsMap[ diffusionPropertyIndex ] = 0;


    if( diffusionPropertyIndex == SubMatrix )
    {
        ClearCovariates();
    }
}

void Data::ClearSubjects( int diffusionPropertyIndex )
{
    m_subjectMap[ diffusionPropertyIndex ].clear();
}

void Data::ClearCovariates()
{
    m_covariateMap.clear();
}


void Data::AddInterceptToCovariates()
{
    /** Key set at -1 so the Intercept is not taken into account when processing covariates **/
    m_covariateMap.insert( -1, "Intercept" );
}
