#ifndef LOG_H
#define LOG_H

#include "MatlabThread.h"

#include <QObject>
#include <QFile>
#include <QTextEdit>
#include <QTextStream>
#include <QFileSystemWatcher>


class Log : public QObject
{
    Q_OBJECT

private slots:
    void OnLogFileChange();


public:
    explicit Log(QObject *parent = 0);
    

    void SetMatlabScript( MatlabThread *matlabThread );


    void SetLogFile( QString outputDir, QString fibername );

    void SetFileWatcher();

    void InitLog( QString outputDir, QString fibername, const QMap< int, QString >& matlabInputFiles, const QMap< int, QString >& selectedCovariates,
                  QStringList loadedSubjects, QString subjectFile, int nbrSelectedSubjects, QStringList failedQCThresholdSubjects, double qcThreshold,
                  int nbrPermutations, double confidenceBandsThreshold, double pvalueThreshold, bool omnibus, bool posthoc, QString mvcmDir,
                  bool runMatlab, QString matlabExe, int nbrCompThreads );


    void AddText( QString text );

    QString GetTextStream() const;

    void CloseLogFile();


signals:
    void UpdateLogActivity( const QString& );


private:
    MatlabThread *m_matlabThread;

    QTextStream* m_textStreamLog;

    QFile *m_logFile;
};

#endif // LOG_H
