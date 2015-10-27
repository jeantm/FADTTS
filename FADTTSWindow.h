#ifndef FADTTSWINDOW_H
#define FADTTSWINDOW_H

#include <QSharedPointer>

#include "FADTTSWindowConfig.h"
#include "Data.h"
#include "EditInputDialog.h"
#include "InfoDialog.h"
#include "Processing.h"
#include "MatlabScript.h"


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
    void SaveParaConfigFile();

    void LoadParaConfigFile();

    void SaveSoftConfigFile();

    void LoadSoftConfigFile();


    void closeEvent(QCloseEvent *event);


    /*************** Input Tab ***************/
    void AddMultipleFiles();

    void AddFile( const QString& prefID );

    void UpdateInputLineEdit( const QString& prefID );

    void EditFile( const QString& prefID );


    void DisplayInfoInputFiles();


    void UpdateLineEditAfterFileEdition( const QString& newFilePathAfterFileEdition, const QString& prefID );

    void UpdateCovariatesAfterFileEdition( const QMap<int, QString>& newCovariatesAfterFileEdition );

    void UpdateSubjectColumnIDAfterFileEdition( const int& newSubjectColumnIDAfterFileEdition );


    /*************** Subjects Tab ***************/
    void LoadInputSubjectFile();

    void ResetInputSubjectFiles();

    void UpdateInputSubjectListLineEdit( const QString& textLineEdit );

    void SaveCheckedSubjectList();


    void CheckAllSubjectsVisible();

    void UnCheckAllSubjectsVisible();

    void SelectSubject( QListWidgetItem *item );


    void SortSubjects();

    void Search();

    void SetCaseSensitivity( bool checked );


    /***************** Parameters  Tab *****************/
    void SelectCovariates( QListWidgetItem *item );

    void CheckAllCovariates();

    void UnCheckAllCovariates();


    /******************** Run  Tab ********************/
    void SetOutputDir();

    void UpdateOutputDirLineEdit( const QString& textLineEdit );

    void SetMatlabExe(); /******** WARNING BAD CODING. FUNCTION NEEDS TO BE CORRECTED ********/


    void RunFADTTS();


private:
    static const QString m_csvSeparator;

    static const QColor m_green;
    static const QColor m_red;
    static const QColor m_grey;
    static const QColor m_yellow;
    static const QColor m_lightBlack;

    static const int m_IconSize;

//    Ui::FADTTSWindow *m_mainUi;

    QSharedPointer<EditInputDialog> m_editInputDialog;

    QSharedPointer<InfoDialog> m_infoDialog;

    QListWidget *m_sortedSubjectListWidget, *m_covariatesListWidget;

//    QLabel *m_adInputSizeLabel, *m_rdInputSizeLabel, *m_mdInputSizeLabel,
//    *m_faInputSizeLabel, *m_compInputSizeLabel;

//    QCheckBox *m_adInputSizeCheckBox, *m_rdInputSizeCheckBox, *m_mdInputSizeCheckBox,
//    *m_faInputSizeCheckBox, *m_compInputSizeCheckBox;

    QPixmap m_okPixmap;
    QPixmap m_koPixmap;
    QPixmap m_warningPixmap;

    typedef QMap<QString, QLabel*> labelMapType;
    labelMapType m_paramTabFileSizeLabelMap, m_inputTabIconLabelMap;

    typedef QMap<QString, QCheckBox*> checkBoxMapType;
    checkBoxMapType m_paramTabFileCheckBoxMap;

    typedef QMap<QString, QLineEdit*> lineEditMapType;
    lineEditMapType m_inputTabFilePathLineEditMap;

    typedef QMap<QString, QPushButton*> pushButtonMapType;
    pushButtonMapType m_inputTabAddFilePushButtonMap, m_inputTabEditFilePushButtonMap;

    Qt::CaseSensitivity caseSensitivity;

    QString m_currentFileInputDir, m_currentSubjectListInputDir, m_currentMatlabExeDir;

    Data m_data;

    Processing m_processing;

    MatlabScript m_matlabScript;


    /********* Configuration & Events *********/
    void Init();

    void InitMenuBar();

    void InitInputTab();

    void InitSubjectTab();

    void InitParametersTab();

    void InitRunTab();


    void UpdateCurrentDirEditInputDialog(const QString newfilePath, QString& currentDir );

    QDir UpdateCurrentDir(const QString newfilePath, QString& currentDir );


    void WarningPopUp( const QString warningText );

    void CriticalPopUp( const QString criticalText );

    void DisplayIcon( QLabel *label , const QPixmap icon );


    /*************** Input Tab ***************/
    void AddFiles( const QStringList fileList );


    void LaunchEditInputWindow( QString prefID );


    void UpdateFileInformation( const QString prefID );

    bool IsMatrixDimensionOK( const QList<QStringList> data );


    void DisplayIcon( const QString prefID , const QPixmap icon );

    void SetInfoSubjectColumnID();


    /*************** Subjects Tab ***************/
    void UpdateAvailableFileParamTab();

    void DisplaySortedSubjectList( const QStringList subjectListRef, const QStringList matchedSubjectList, const QMap<QString, QStringList > unMatchedSubjectList );

    void DisplayNbrSubjectsSelected();

    /*************** Parameters Tab ***************/
    void DisplayCovariatesList( QMap<int, QString> covariatesList );


    /*************** Run Tab ***************/
    QStringList GetSelectedPrefixes();

    QMap<QString, bool> GetSelectedInputFiles();

    QMap<QString, QList<QStringList> > GetDataInSelectedFiles();

    QMap<int, QString> GetSelectedCovariates();

    QString GenerateSelectedSubjectList();

};

#endif // FADTTSWINDOW_H
