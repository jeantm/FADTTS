#ifndef FADTTS_noGUI_H
#define FADTTS_noGUI_H

#include "Processing.h"
#include "MatlabThread.h"
#include "Log.h"

#include <QObject>
#include <QCloseEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>

class FADTTS_noGUI : public QObject
{
    Q_OBJECT

public:
    FADTTS_noGUI( QObject *parent = 0 );


int RunFADTTSter_noGUI( const QJsonObject& jsonObject_noGUI );


private slots:

void OnMatlabThreadFinished();

void OnKillFADTTSter();


private:

Processing m_processing;
MatlabThread *m_matlabThread;
Log *m_log;

enum diffusionProperties { AD, RD, MD, FA, SubMatrix } m_diffusionProperties;

/*** Inputs ***/
QMap< int, QString > m_inputs;
QMap< int, QString > m_properties;
QMap< int, QString > m_covariates;
int m_subjectColumnID;

/*** Subjects ***/
QStringList m_subjects, m_loadedSubjects, m_failedQCThresholdSubjects;
QString m_subjectFile;
double m_qcThreshold;
int m_nbrSelectedSubjects;

/*** Settings ***/
QString m_fibername;
int m_nbrPermutations;
double m_confidenceBandThreshold;
double m_pvalueThreshold;
bool m_omnibus;
bool m_posthoc;

/*** Output ***/
QString m_outputDir;

/*** Matlab Specifications ***/
QString m_mvmcDir;
bool m_runMatlab;
QString m_matlabExe;
int m_nbrCompThreads;


void InitFADTTS_noGUI();


void GetInputFiles( const QJsonObject& inputFiles );

void GetCovariates( const QJsonObject& covariates );

QMap< int, QStringList > GetInputSubjects();

void SetQCThreshold( const QJsonObject& qcThresholdObject );

void GetSubjects( const QJsonObject& subjects );

void GetSettings( const QJsonObject& settings );

void GetMatlabSpecifications( const QJsonObject& matlabSpecifications );

void GetOutput( QString outputDir );


bool canFADTTSterBeRun();

void GenerateSubjectFile();

void SetMatlabScript();
};

#endif // FADTTS_noGUI_H
