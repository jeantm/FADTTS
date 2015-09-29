#ifndef PROCESSING_H
#define PROCESSING_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QMap>
#include <QCheckBox>
#include <QListWidget>
#include <QDebug>
#include <QLocale>


class Processing
{
public:
    Processing();

    QStringList GenerateMatlabInputFiles( QStringList selectedInputFiles, QString selectedSubjectListFile,
                                  int subjectCovariatesColumnId, QList<int> selectedCovariates, QString currentFileOutputDir );

    QStringList GetSelectedSubjectList( QString selectedSubjectListFile );

    QStringList GetSubjectListFromDataFile( QString subjectListFile, int subjectCovariatesColumnId );

    QStringList GetRefSubjectListFromSelectedInputFiles( QStringList selectedInputFiles, int subjectsCovariatesColumnId );

    void ResetProcessing();

    void AssignSortedSubject( const QMap< QString, QMap<QString, bool> > checkedSubject, QStringList& matchedSubjectList,
                                           QMap<QString, QStringList >& unMatchedSubjectList , QString subjectListFilePath );

    QMap< QString, QMap<QString, bool> > FindSubjectInInputFile( const QStringList refList, const QMap<QString, QStringList> subjectList );


private:
    static const QString m_csvSeparator;

    QString m_selectedSubjectListFile;

    QStringList m_matlabInputFiles, m_selectedSubjectList, m_selectedInputFiles;

};

#endif // PROCESSING_H
