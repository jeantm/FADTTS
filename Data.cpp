#include "Data.h"

Data::Data( QObject *parent ) :
    QObject(parent)
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
    return m_PrefixList;
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

QString Data::GetCovariatesPrefix() const
{
    return m_covariatesPrefix;
}


QMap<QString, QString> Data::GetFilenameMap() const
{
    return m_filenameMap;
}

QMap<QString, QStringList> Data::GetSubjectsMap() const
{
    return m_SubjectsMap;
}

QMap<QString, int> Data::GetNbrRowsMap() const
{
    return m_NbrRowsMap;
}

QMap<QString, int> Data::GetNbrColumnsMap() const
{
    return m_NbrColumnsMap;
}

QMap<QString, int> Data::GetNbrSubjectsMap() const
{
    return m_NbrSubjectsMap;
}

QString Data::GetFilename( QString pref ) const
{
    return m_filenameMap[ pref ];
}

QStringList Data::GetSubjects( QString pref ) const
{
    return m_SubjectsMap[ pref ];
}

int Data::GetNbrRows( QString pref ) const
{
    return m_NbrRowsMap[ pref ];
}

int Data::GetNbrColumns( QString pref ) const
{
    return m_NbrColumnsMap[ pref ];
}

int Data::GetNbrSubjects( QString pref ) const
{
    return m_NbrSubjectsMap[ pref ];
}

QStringList Data::GetCovariatesList() const
{
    return m_covariatesList;
}


int Data::GetSubjectColumnID() const
{
    return m_subjectColumnID;
}

QMap<QString, QString >::ConstIterator Data::GetFilenameMapIterator()
{
    return m_filenameMap.begin();
}


QMap<QString, QStringList >::ConstIterator Data::GetSubjectsMapIterator()
{
    return m_SubjectsMap.begin();
}


/***************************************/
/*************** Setters ***************/
/***************************************/
QString& Data::SetOutputDir()
{
    return m_outputDir;
}
QString& Data::SetFilename( QString pref )
 {
     return m_filenameMap[ pref ];
 }

QStringList& Data::SetSubjects( QString pref )
{
    return m_SubjectsMap[ pref ];
}

int& Data::SetNbrRows( QString pref )
{
    return m_NbrRowsMap[ pref ];
}

int& Data::SetNbrColumns( QString pref )
{
    return m_NbrColumnsMap[ pref ];
}

int& Data::SetNbrSubjects( QString pref )
{
    return m_NbrSubjectsMap[ pref ];
}

QStringList& Data::SetCovariatesList()
{
    return m_covariatesList;
}


QString& Data::SetCovariatesPrefix()
{
    return m_covariatesPrefix;
}

void Data::SetSubjectColumnID( int id )
{
    m_subjectColumnID = id;
}


/***************************************/
/************** Functions **************/
/***************************************/
int Data::InitData()
{
    m_axialDiffusivityPrefix = "ad";
    m_radialDiffusivityPrefix = "rd";
    m_meanDiffusivityPrefix = "md";
    m_fractionalAnisotropyPrefix = "fa";
    m_covariatesPrefix = "COMP";

    m_PrefixList << m_axialDiffusivityPrefix << m_radialDiffusivityPrefix << m_meanDiffusivityPrefix
                 << m_fractionalAnisotropyPrefix << m_covariatesPrefix;

    foreach( QString pref, m_PrefixList )
    {
        m_filenameMap[ pref ];
        m_NbrRowsMap[ pref ];
        m_NbrColumnsMap[ pref ];
        m_NbrSubjectsMap[ pref ];
        ( m_SubjectsMap[ pref ] );
    }

    m_subjectColumnID = 0;

    return m_PrefixList.removeDuplicates();
}

void Data::AddSubject( QString prefID, QString subjectID )
{
    m_SubjectsMap[ prefID ].append( tr( qPrintable( subjectID ) ) );
}

void Data::AddCovariate( QString covariate )
{
    m_covariatesList.append( tr( qPrintable( covariate ) ) );
}

void Data::AddIntercept()
{
    m_covariatesList.prepend( tr( "Intercept" ) );
}

void Data::ClearFileInformation( QString prefID )
{
    m_filenameMap[ prefID ].clear();
    m_NbrRowsMap[ prefID ] = 0;
    m_NbrColumnsMap[ prefID ] = 0;
    m_NbrSubjectsMap[ prefID ] = 0;
    ( m_SubjectsMap[ prefID ] ).clear();

    if( prefID == m_covariatesPrefix )
    {
        m_covariatesList.clear();
    }
}

void Data::ClearSubjects( QString prefID )
{
    ( m_SubjectsMap[ prefID ] ).clear();
}

void Data::ClearCovariatesList()
{
    m_covariatesList.clear();
}
