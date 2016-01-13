#ifndef PROCESSING_H
#define PROCESSING_H

#include "Data.h"

#include <QDate>
#include <QObject>
#include <QProcess>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QCheckBox>
#include <QMap>


class Processing : public QObject
{
    friend class TestProcessing; /** For unit tests **/

    Q_OBJECT

public:
    explicit Processing( QObject *parent = 0 );


    bool IsMatrixDimensionOK( const QList<QStringList> fileData );

    bool IsCovariateFile(const QStringList fileData );


    QStringList GetSubjectsFromInputFile( QList<QStringList> dataInInputFile, int covariateFileSubjectColumnID );

    QStringList GetRefSubjects( const QString subjectFilePath, QMap< QPair< int, QString >, QList<QStringList> > dataInSelectedInputFiles, int covariateFileSubjectColumnID );

    QMap<QString, QStringList> GetAllSubjectsFromSelectedInputFiles( const QMap<QString, QCheckBox*> checkBoxMap, const QMap<QString, QStringList > subjectsMap );

    QMap< QString, QMap<QString, bool> > SortSubjects( const QStringList refSubjects, const QMap<QString, QStringList> selectedSubjects );

    void AssignSortedSubject( const QMap< QString, QMap<QString, bool> > checkedSubjects, QStringList& matchedSubjects,
                              QMap<QString, QStringList >& unMatchedSubjects );


    QList<QStringList> GetDataFromFile( QString filePath );

    QMap<int, QString> GetCovariatesFromFileData( QList<QStringList> dataCovariateFile, int covariateFileSubjectColumnID );


    QMap< QPair< int, QString >, bool> GenerateMatlabInputFiles( QMap< QPair< int, QString >, bool > selectedInputFiles, QMap< int, QString > propertySelected,
                                                                 QString selectedSubjectFile, int covariateFileSubjectColumnId, QMap<int, QString> selectedCovariates,
                                                                 QString outputDir, QString fiberName );


private:
    static const QString m_csvSeparator;


    QStringList GetSelectedSubjects( QString selectedSubjectFile );

    QStringList GetRefSubjectsFromSelectedInputFiles( QMap< QPair< int, QString >, QList<QStringList> > dataInSelectedInputFiles, int covariateFileSubjectColumnID );
};

#endif // PROCESSING_H
