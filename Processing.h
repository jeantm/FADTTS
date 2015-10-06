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

    QStringList GetSubjectListFromInputFile( QString subjectListFile, int subjectCovariatesColumnId );

    QStringList GetRefSubjectListFromSelectedInputFiles( QMap<QString, bool> selectedInputFiles, int subjectsCovariatesColumnId );

    QMap< QString, QMap<QString, bool> > FindSubjectInInputFile( const QStringList refList, const QMap<QString, QStringList> subjectList );

    void AssignSortedSubject( const QMap< QString, QMap<QString, bool> > checkedSubject, QStringList& matchedSubjectList,
                                           QMap<QString, QStringList >& unMatchedSubjectList , QString subjectListFilePath );


    /***************** Run *****************/
    QMap<QString, bool> GenerateMatlabInputFiles( QMap<QString, bool> selectedInputFiles, QString selectedSubjectListFile,
                                  int subjectCovariatesColumnId, QMap<int, QString> selectedCovariates, QString currentOutputDir );

    QStringList GetSelectedSubjectList( QString selectedSubjectListFile );

    void ResetProcessing();


private:
    static const QString m_csvSeparator;

    QString m_selectedSubjectListFile;

    QStringList m_selectedSubjectList;

    QMap<QString, bool> m_selectedInputFiles, m_matlabInputFiles;

};

#endif // PROCESSING_H
