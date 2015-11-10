#ifndef PROCESSING_H
#define PROCESSING_H

#include <QLocale>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QCheckBox>
#include <QListWidget>
#include <QProcess>
#include <QDebug>

#include "Data.h"

class Processing
{
public:
    Processing();

    /*****************************************************/
    /****************** Running Process ******************/
    /*****************************************************/
    void RunScript( QString matlabExe, QString matlabScript );

    bool IsMatrixDimensionOK( const QList<QStringList> fileData );

    bool IsCovariateFile(const QStringList fileData );

    QStringList GetSelectedSubjects( QString selectedSubjectFile );

    QMap< QPair< int, QString >, bool> GenerateMatlabInputFiles( QMap< QPair< int, QString >, bool > selectedInputFiles, QString selectedSubjectFile,
                                                  int covariateFileSubjectColumnId, QMap<int, QString> selectedCovariates,
                                                  QString outputDir, QString fiberName );


    /*****************************************************/
    /****************** Data Processing ******************/
    /*****************************************************/
    QStringList GetSubjectsFromInputFile( QList<QStringList> dataInInputFile, int covariateFileSubjectColumnID );

    QStringList GetRefSubjectsFromSelectedInputFiles( QMap< QPair< int, QString >, QList<QStringList> > dataInSelectedInputFiles, int covariateFileSubjectColumnID );

    QStringList GetRefSubjects( const QString subjectFilePath, QMap< QPair< int, QString >, QList<QStringList> > dataInSelectedInputFiles, int covariateFileSubjectColumnID );

    QMap<QString, QStringList> GetAllSubjectsFromSelectedInputFiles( const QMap<QString, QCheckBox*> checkBoxMap, const QMap<QString, QStringList > subjectsMap );

    QMap< QString, QMap<QString, bool> > SortSubjects( const QStringList refSubjects, const QMap<QString, QStringList> selectedSubjects );

    void AssignSortedSubject( const QMap< QString, QMap<QString, bool> > checkedSubjects, QStringList& matchedSubjects,
                              QMap<QString, QStringList >& unMatchedSubjects );


    QList<QStringList> GetDataFromFile( QString filePath );

    QMap<int, QString> GetCovariatesFromFileData( QList<QStringList> dataCovariateFile, int covariateFileSubjectColumnID );


private:
    static const QString m_csvSeparator;

    QProcess* m_process;
};

#endif // PROCESSING_H
