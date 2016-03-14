#ifndef DATA_H
#define DATA_H

#include <QMap>
#include <vector>

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

    int GetNbrRows( int diffusionPropertyIndex ) const; // Tested

    int GetNbrColumns( int diffusionPropertyIndex ) const; // Tested

    QMap< int, QStringList > GetSubjects() const; // Tested

    int GetNbrSubjects( int diffusionPropertyIndex ) const; // Tested


    QMap< int, QString > GetCovariates() const; // Tested

    int GetSubjectColumnID() const; // Tested

    QString GetOutputDir() const; // Tested



    /*************** Setters ***************/
    QString& SetFilename( int diffusionPropertyIndexID ); // Tested

    QList< QStringList >& SetFileData( int diffusionPropertyIndexID ); // Tested

    int& SetNbrRows( int diffusionPropertyIndexID ); // Tested

    int& SetNbrColumns( int diffusionPropertyIndexID ); // Tested

    QStringList& SetSubjects( int diffusionPropertyIndexID ); // Tested

    int& SetNbrSubjects( int diffusionPropertyIndexID ); // Tested


    QMap< int, QString >& SetCovariates(); // Tested

    int& SetSubjectColumnID(); // Tested

    QString& SetOutputDir(); // Tested



    void ClearData( int diffusionPropertyIndexID ); // Tested

    void ClearSubjects( int diffusionPropertyIndexID ); // Tested

    void ClearCovariates(); // Not Directly Tested


    void AddInterceptToCovariates(); // Tested


private:
    enum diffusionProperties { AD, RD, MD, FA, SubMatrix } m_diffusionProperties;

    QMap< int, QList< QStringList > > m_fileDataMap;

    QMap< int, QStringList > m_subjectMap;

    QMap< int, QString > m_filenameMap, m_covariateMap;

    QMap< int, int > m_nbrRowsMap, m_nbrColumnsMap, m_nbrSubjectsMap;

    QString m_outputDir;

    int m_subjectColumnID;
};

#endif // DATA_H
