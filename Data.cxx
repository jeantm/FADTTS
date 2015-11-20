#include "Data.h"

Data::Data()
{
}


/***************************************/
/*************** Getters ***************/
/***************************************/
QString Data::GetOutputDir() const
{
    return m_outputDir;
}

QStringList Data::GetPrefixList() const
{
    return m_prefixList;
}

QString Data::GetAxialDiffusivityPrefix() const
{
    return m_axialDiffusivityPrefix;
}

QString Data::GetRadialDiffusivityPrefix() const
{
    return m_radialDiffusivityPrefix;
}

QString Data::GetMeanDiffusivityPrefix() const
{
    return m_meanDiffusivityPrefix;
}

QString Data::GetFractionalAnisotropyPrefix() const
{
    return m_fractionalAnisotropyPrefix;
}

QString Data::GetCovariatePrefix() const
{
    return m_covariatePrefix;
}

int Data::GetCovariateFileSubjectColumnID() const
{
    return m_covariateFileSubjectColumnID;
}


QMap<QString, QStringList> Data::GetSubjects() const
{
    return m_subjectMap;
}

QMap<int, QString> Data::GetCovariates() const
{
    return m_covariateMap;
}


QString Data::GetFilename( QString prefID ) const
{
    return m_filenameMap[ prefID ];
}

QList<QStringList> Data::GetFileData( QString prefID ) const
{
    return m_fileDataMap[ prefID ];
}

int Data::GetNbrRows( QString prefID ) const
{
    return m_nbrRowsMap[ prefID ];
}

int Data::GetNbrColumns( QString prefID ) const
{
    return m_nbrColumnsMap[ prefID ];
}

int Data::GetNbrSubjects( QString prefID ) const
{
    return m_nbrSubjectsMap[ prefID ];
}


/***************************************/
/*************** Setters ***************/
/***************************************/
QString& Data::SetOutputDir()
{
    return m_outputDir;
}


QMap<int, QString>& Data::SetCovariates()
{
    return m_covariateMap;
}


QString& Data::SetFilename( QString prefID )
{
    return m_filenameMap[ prefID ];
}

QList<QStringList>& Data::SetFileData( QString prefID )
{
    return m_fileDataMap[ prefID ];
}

QStringList& Data::SetSubjects( QString prefID )
{
    return m_subjectMap[ prefID ];
}

int& Data::SetNbrRows( QString prefID )
{
    return m_nbrRowsMap[ prefID ];
}

int& Data::SetNbrColumns( QString prefID )
{
    return m_nbrColumnsMap[ prefID ];
}

int& Data::SetNbrSubjects( QString prefID )
{
    return m_nbrSubjectsMap[ prefID ];
}


int& Data::SetCovariateFileSubjectColumnID()
{
    return m_covariateFileSubjectColumnID;
}


/***************************************/
/*********** Other Functions ***********/
/***************************************/
int Data::InitData()
{
    m_axialDiffusivityPrefix = "ad";
    m_radialDiffusivityPrefix = "rd";
    m_meanDiffusivityPrefix = "md";
    m_fractionalAnisotropyPrefix = "fa";
    m_covariatePrefix = "COMP";

    m_prefixList << m_axialDiffusivityPrefix << m_radialDiffusivityPrefix << m_meanDiffusivityPrefix
                 << m_fractionalAnisotropyPrefix << m_covariatePrefix;

    foreach( QString prefID, m_prefixList )
    {
        m_filenameMap[ prefID ];
        m_nbrRowsMap[ prefID ];
        m_nbrColumnsMap[ prefID ];
        m_nbrSubjectsMap[ prefID ];
        ( m_subjectMap[ prefID ] );
    }

    m_covariateFileSubjectColumnID = 0;

    return m_prefixList.removeDuplicates();
}

void Data::SetSubjects(QString prefID, QStringList subjects )
{
    m_subjectMap[ prefID ].append( subjects );
}

void Data::AddCovariate( int colunmID, QString covariate )
{
    m_covariateMap.insert( colunmID, covariate );
}

void Data::AddInterceptToCovariates()
{
    /** The key is -1 so the Intercept is not taken into account as an input covariate **/
    m_covariateMap.insert( -1, "Intercept" );
}

void Data::ClearFileInformation( QString prefID )
{
    m_filenameMap[ prefID ].clear();
    m_fileDataMap[ prefID ].clear();
    m_nbrRowsMap[ prefID ] = 0;
    m_nbrColumnsMap[ prefID ] = 0;
    m_nbrSubjectsMap[ prefID ] = 0;
    ( m_subjectMap[ prefID ] ).clear();

    if( prefID == m_covariatePrefix )
    {
        m_covariateMap.clear();
    }
}

void Data::ClearSubjects( QString prefID )
{
    ( m_subjectMap[ prefID ] ).clear();
}

void Data::ClearCovariates()
{
    m_covariateMap.clear();
}
