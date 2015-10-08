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


    QMap<QString, QString> GetFilenameMap() const;

    QMap<QString, QStringList> GetSubjectsMap() const;

    QMap<QString, int> GetNbrRowsMap() const;

    QMap<QString, int> GetNbrColumnsMap() const;

    QMap<QString, int> GetNbrSubjectsMap() const;

    QString GetFilename( QString pref ) const;

    QStringList GetSubjects( QString pref ) const;

    int GetNbrRows( QString pref ) const;

    int GetNbrColumns( QString pref ) const;

    int GetNbrSubjects( QString pref ) const;

    QMap<int, QString> GetCovariatesList() const;


    int GetSubjectColumnID() const;

    QMap<QString, QString >::ConstIterator GetFilenameMapIterator();

    QMap<QString, QStringList >::ConstIterator GetSubjectsMapIterator();


    /***************************************/
    /*************** Setters ***************/
    /***************************************/
    QString& SetOutputDir();

    QString& SetFilename( QString pref );

    QStringList& SetSubjects( QString pref );

    int& SetNbrRows( QString pref );

    int& SetNbrColumns( QString pref );

    int& SetNbrSubjects( QString pref );

    QMap<int, QString>& SetCovariatesList( );


    QString& SetCovariatesPrefix();

    void SetSubjectColumnID( int id );


    /***************************************/
    /************** Functions **************/
    /***************************************/
    int InitData();

    void AddSubject( QString prefID, QString subjectID );

    void AddCovariate( int colunmID , QString covariate );

    void AddIntercept();

    void ClearFileInformation( QString prefID );

    void ClearSubjects( QString prefID );

    void ClearCovariatesList();


private:

    int m_subjectColumnID;

    QString m_outputDir,
    m_axialDiffusivityPrefix, m_radialDiffusivityPrefix,
    m_meanDiffusivityPrefix, m_fractionalAnisotropyPrefix, m_covariatesPrefix;

    QStringList m_PrefixList;

    QMap<QString, QStringList> m_SubjectsMap;

    QMap<QString, QString> m_filenameMap;

    QMap<QString, int> m_NbrRowsMap, m_NbrColumnsMap, m_NbrSubjectsMap;

    QMap<int, QString> m_covariatesList;
};

#endif // DATA_H
