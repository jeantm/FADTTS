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


    bool IsMatrixDimensionOK( const QList<QStringList> data );

    bool IsCovariateFile( const QStringList file );


    QStringList GetSubjectsFromData( QList<QStringList> data, int covariateColumnID );


    QStringList GetAllSubjects( QMap<QString, QStringList> subjectsMap );

    QMap<QString, QStringList> GetSubjectsFromSelectedFiles( const QMap<QString, QCheckBox*> checkBoxMap, const QMap<QString, QStringList > subjectsMap );

    QMap< QString, QMap<QString, bool> > SortSubjects( const QStringList allSubjectsList, const QMap<QString, QStringList> allSubjects );

    void AssignSortedSubject( const QMap< QString, QMap<QString, bool> > checkedSubjects, QStringList& matchedSubjects,
                              QMap<QString, QStringList >& unMatchedSubjects );


    QList<QStringList> GetDataFromFile( QString filePath );

    QMap<int, QString> GetCovariatesFromData( QList<QStringList> dataCovariateFile, int covariateColumnID );


    QMap< int, QString > GenerateMatlabInputs( QString outputDir, QString fiberName,
                                               QMap<int, QString> inputs, QMap< int, QString > properties,
                                               QMap<int, QString> covariates, QStringList subjects, int covariateColumnID );


private:
    static const QString m_csvSeparator;
};

#endif // PROCESSING_H
