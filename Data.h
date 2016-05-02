#ifndef DATA_H
#define DATA_H

#include <QMap>

class Data
{
    friend class TestFADTTSWindow; /** For unit tests **/

public:
    explicit Data();


    void InitData(); // Tested



    /*************** Getters ***************/
    QList< int > GetDiffusionPropertiesIndices() const; // Tested

    int GetAxialDiffusivityIndex() const; // Tested

    int GetRadialDiffusivityIndex() const; // Tested

    int GetMeanDiffusivityIndex() const; // Tested

    int GetFractionalAnisotropyIndex() const; // Tested

    int GetSubMatrixIndex() const; // Tested

    QString GetDiffusionPropertyName( int diffusionPropertyIndex ) const; // Tested


    QString GetFilename( int diffusionPropertyIndex ) const; // Tested

    QList< QStringList > GetFileData( int diffusionPropertyIndex ) const; // Tested

    QStringList GetAtlas() const; // Tested

    int GetNbrRows( int diffusionPropertyIndex ) const; // Tested

    int GetNbrColumns( int diffusionPropertyIndex ) const; // Tested

    QMap< int, QStringList > GetSubjects() const; // Tested

    int GetNbrSubjects( int diffusionPropertyIndex ) const; // Tested


    QMap< int, QString > GetCovariates() const; // Tested

    int GetSubjectColumnID() const; // Tested

    QString GetOutputDir() const; // Tested



    /*************** Setters ***************/
    QString& SetFilename( int diffusionPropertyIndex ); // Tested

    QList< QStringList >& SetFileData( int diffusionPropertyIndex ); // Tested

    QStringList& SetAtlas(); // Tested

    int& SetNbrRows( int diffusionPropertyIndex ); // Tested

    int& SetNbrColumns( int diffusionPropertyIndex ); // Tested

    QStringList& SetSubjects( int diffusionPropertyIndex ); // Tested

    int& SetNbrSubjects( int diffusionPropertyIndex ); // Tested


    QMap< int, QString >& SetCovariates(); // Tested

    int& SetSubjectColumnID(); // Tested

    QString& SetOutputDir(); // Tested



    void ClearData( int diffusionPropertyIndex ); // Tested

    void ClearSubjects( int diffusionPropertyIndex ); // Tested

    void ClearCovariates(); // Not Directly Tested


    void AddInterceptToCovariates(); // Tested


private:
    enum diffusionProperties { AD, RD, MD, FA, SubMatrix };

    QMap< int, QList< QStringList > > m_fileDataMap;

    QMap< int, QStringList > m_subjectMap;

    QMap< int, QString > m_filenameMap, m_covariateMap;

    QMap< int, int > m_nbrRowsMap, m_nbrColumnsMap, m_nbrSubjectsMap;

    QStringList m_atlas;

    QString m_outputDir;

    int m_subjectColumnID;
};

#endif // DATA_H
