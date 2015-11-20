#ifndef FADTTSWINDOW_H
#define FADTTSWINDOW_H

#include "FADTTSWindowConfig.h"
#include "EditInputDialog.h"
#include "InfoDialog.h"
#include "Data.h"
#include "Processing.h"
#include "MatlabScript.h"
#include "MatlabThread.h"
#include "Plot.h"

#include <QSignalMapper>
#include <QFileSystemWatcher>
#include <QScrollBar>
#include <QDebug>


class FADTTSWindow : public FADTTSWindowConfig
{
    friend class TestFADTTSWindow;

    Q_OBJECT
    
public:
    /********* Configuration & Events *********/
    FADTTSWindow();

    ~FADTTSWindow();


private slots:
    /********* Configuration & Events *********/
    void SaveParaSettings();

    void LoadParaSettings();

    void SaveSoftSettings();

    void LoadSoftSettings();

    void DisplayAbout();


    void closeEvent(QCloseEvent *event);


    /*************** Input Tab ***************/
    void AddMultipleInputFiles();

    void AddInputFile( const QString& prefID );

    void UpdateInputLineEdit( const QString& prefID );

    void EditInputFile( const QString& prefID );


    void DisplayInputFileInformation();


    void UpdateInputLineEditAfterFileEdition( const QString& newFilePathAfterFileEdition, const QString& prefID );

    void UpdateCovariateMapAfterFileEdition( const QMap<int, QString>& newCovariateMapAfterFileEdition );

    void UpdateCovariateFileSubjectColumnIDAfterFileEdition( const int& newCovariateFileSubjectColumnIDAfterFileEdition );


    /*************** Subjects Tab ***************/
    void LoadSubjectFile();

    void ResetSubjectFile();

    void UpdateSubjectFileLineEdit( const QString& filePath );

    void SaveCheckedSubjects();


    void CheckAllSubjectsVisible();

    void UnCheckAllSubjectsVisible();

    void SelectSubject( QListWidgetItem *item );


    void SortAndDisplaySubjects();

    void SearchSubjects();

    void SetCaseSensitivity( bool checked );


    /***************** Parameters  Tab *****************/
    void DisplayCovariates();

    void SelectCovariate( QListWidgetItem *item );

    void CheckAllCovariates();

    void UnCheckAllCovariates();


    /****************** Execution Tab ******************/
    void SetOutputDir();

    void UpdateOutputDir( const QString& path );

    void SetMatlabExe();

    void UpdateMatlabExe( const QString& executable );

    void SetMVCMPath();

    void UpdateMVCMPath( const QString& path );


    void RunFADTTS();

    void StopFADTTS();

//    void WriteLog();


    /************** Quality Control  Tab **************/


private:
    static const QColor m_green;
    static const QColor m_red;
    static const QColor m_grey;
    static const QColor m_yellow;
    static const QColor m_lightBlack;

    static const int m_iconSize;

    QPixmap m_okPixmap;
    QPixmap m_koPixmap;
    QPixmap m_warningPixmap;

    QSharedPointer<EditInputDialog> m_editInputDialog;

    QSharedPointer<InfoDialog> m_infoDialog;

    QListWidget *m_sortedSubjectListWidget, *m_covariateListWidget;

    QVTKWidget *m_qvtkWidget;

//    QPlainTextEdit *m_log;

    QTextStream* m_textStreamLog;

    vtkSmartPointer<vtkContextView> m_view;


    QLineEdit *m_subjectFileLineEdit;

    typedef QMap<QString, QLabel*> labelMapType;
    labelMapType m_inputTabIconLabelMap, m_paramTabFileDataSizeLabelMap;

    typedef QMap<QString, QCheckBox*> checkBoxMapType;
    checkBoxMapType m_paramTabFileCheckBoxMap;

    typedef QMap<QString, QLineEdit*> lineEditMapType;
    lineEditMapType m_inputTabInputFileLineEditMap;

    typedef QMap<QString, QPushButton*> pushButtonMapType;
    pushButtonMapType m_inputTabAddInputFilePushButtonMap, m_inputTabEditInputFilePushButtonMap;

    Qt::CaseSensitivity caseSensitivity;


    Data m_data;

    Processing m_process;

    MatlabScript m_matlabScript;

    MatlabThread *m_matlabThread;

    Plot *m_plot;


    QString m_currentInputFileDir, m_currentSubjectFileDir, m_currentMatlabExeDir, m_mvcmPath;



    /********* Configuration & Events *********/
    void InitFADTTSWindow();

    void InitMenuBar();

    void InitInputTab();

    void InitSubjectTab();

    void InitExecutionTab();

    void InitQualityControlTab();


    void UpdateEditInputDialogCurrentDir( const QString newfilePath );

    QDir UpdateCurrentDir( const QString newfilePath, QString& currentDir );

    void SetDir( QDir& dir, QString filePath, QString currentDir );


    void WarningPopUp( const QString warningMessage );

    void CriticalPopUp( const QString criticalMessage );

    void DisplayIcon( QLabel *label , const QPixmap icon );


    /*************** Input Tab ***************/
    void UpdateLineEditsAfterAddingMultipleFiles( const QStringList fileList );


    void LaunchEditInputDialog( QString prefID );


    void UpdateInputFileInformation( const QString prefID );

    bool IsMatrixDimensionOK( const QList<QStringList> fileData );


    void DisplayInputLineEditIcon( const QString prefID , const QPixmap icon );

    void SetInfoCovariateFileSubjectColumnID();


    /*************** Subjects Tab ***************/
    void UpdateAvailableFileParamTab();

    void DisplaySortedSubjects( const QStringList matchedSubjectList, const QMap<QString, QStringList > unMatchedSubjectMap );

    void DisplayNbrSubjectSelected();


    /*************** Parameters Tab ***************/


    /*************** Execution Tab ****************/
    QStringList GetSelectedPrefixes();

    QMap< QPair< int, QString >, bool> GetSelectedInputFiles();

    QMap< QPair< int, QString >, QList<QStringList> > GetFileDataOfSelectedFiles();

    QMap<int, QString> GetSelectedCovariates();

    QString GenerateSelectedSubjectFile( QString outputDir );

    bool IsRunFADTTSOK( QString fiberName, QMap<int, QString> selectedCovariates );

    void SetMatlabThread( QString fiberName, QMap<int, QString> selectedCovariates );
};

#endif // FADTTSWINDOW_H
