#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QStringList>
#include <QMap>

class Data : public QObject
{
    Q_OBJECT

public:
    explicit Data(QObject *parent = 0);

    /***************************************/
    /*************** Getters ***************/
    /***************************************/
    QString GetOutputDir() const;

    QStringList GetPrefixList() const;

    QString GetAxialDiffusivityPrefix() const;

    QString GetRadialDiffusivityPrefix() const;

    QString GetMeanDiffusivityPrefix() const;

    QString GetFractionalAnisotropyPrefix() const;

    QString GetCovariatePrefix() const;

    int GetCovariateFileSubjectColumnID() const;


    QMap<QString, QStringList> GetSubjects() const;

    QMap<int, QString> GetCovariates() const;


    QString GetFilename( QString pref ) const;

    QList<QStringList> GetFileData( QString pref ) const;

    int GetNbrRows( QString pref ) const;

    int GetNbrColumns( QString pref ) const;

    int GetNbrSubjects( QString pref ) const;


    /***************************************/
    /*************** Setters ***************/
    /***************************************/
    QString& SetOutputDir();

    int& SetCovariateFileSubjectColumnID();


    QMap<int, QString>& SetCovariates();


    QString& SetFilename( QString prefID );

    QList<QStringList>& SetFileData( QString prefID );

    QStringList& SetSubjects( QString prefID );

    int& SetNbrRows( QString prefID );

    int& SetNbrColumns( QString prefID );

    int& SetNbrSubjects( QString prefID );


    /***************************************/
    /*********** Other Functions ***********/
    /***************************************/
    int InitData();

    void SetSubjects( QString prefID, QStringList subjects );

    void AddCovariate( int colunmID , QString covariate );

    void AddInterceptToCovariates();

    void ClearFileInformation( QString prefID );

    void ClearSubjects( QString prefID );

    void ClearCovariates();


private:
    QMap<QString, QStringList> m_subjectMap;

    QMap<QString, QString> m_filenameMap;

    QMap< QString, QList<QStringList> > m_fileDataMap;

    QMap<QString, int> m_nbrRowsMap, m_nbrColumnsMap, m_nbrSubjectsMap;

    QMap<int, QString> m_covariateMap;

    QStringList m_prefixList;

    QString m_outputDir,
    m_axialDiffusivityPrefix, m_radialDiffusivityPrefix,
    m_meanDiffusivityPrefix, m_fractionalAnisotropyPrefix, m_covariatePrefix;

    int m_covariateFileSubjectColumnID;
};

#endif // DATA_H
