#ifndef PROCESSING_H
#define PROCESSING_H

#include <QLocale>
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
    void RunScript( QString matlabExe, QString scriptPath );


    bool IsCOMPFile( const QStringList strList );

    QStringList GetSelectedSubjectList( QString selectedSubjectListFile );

    QMap<QString, bool> GenerateMatlabInputFiles( QMap<QString, bool> selectedInputFiles, QString selectedSubjectListFile,
                                                  int subjectCovariatesColumnId, QMap<int, QString> selectedCovariates,
                                                  QString outputDir, QString fiberName );


    /*****************************************************/
    /****************** Data Processing ******************/
    /*****************************************************/
    QStringList GetSubjectListFromInputFile( QList<QStringList> dataInInputFile, int subjectCovariatesColumnId );

    QStringList GetRefSubjectListFromSelectedInputFiles( QMap<QString, QList<QStringList> > dataInSelectedInputFiles, int subjectCovariatesColumnId );

    QStringList GetRefSubjectList( const QString subjectListFilePath, QMap<QString, QList<QStringList> > dataInSelectedFiles, int subjectCovariatesColumnId );

    QMap<QString, QStringList> GetAllSubjectsFromSelectedInputFiles( const QMap<QString, QCheckBox*> checkBoxMap, const QMap<QString, QStringList > subjectsMap );

    QMap< QString, QMap<QString, bool> > SortSubjectInInputFile( const QStringList refList, const QMap<QString, QStringList> subjectMap );

    void AssignSortedSubject( const QMap< QString, QMap<QString, bool> > checkedSubject, QStringList& matchedSubjects,
                              QMap<QString, QStringList >& unMatchedSubjects );


    QList<QStringList> GetDataFromFile( QString fileName );


private:
    static const QString m_csvSeparator;

    QProcess* m_process;
};

#endif // PROCESSING_H
