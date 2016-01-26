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
#include <QProgressBar>
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

    void UpdateInputLineEditAfterFileEdition( const QString& newFilePathAfterFileEdition, const QString& prefID );

    void UpdateCovariateMapAfterFileEdition( const QMap<int, QString>& newCovariateMapAfterFileEdition );

    void UpdateCovariateFileSubjectColumnIDAfterFileEdition( const int& newCovariateFileSubjectColumnIDAfterFileEdition );


    /*********** Subjects/Covariates Tab ***********/
    void DisplayCovariates();

    void SelectCovariate( QListWidgetItem *item );

    void CheckAllCovariates();

    void UnCheckAllCovariates();


    void LoadSubjectFile();

    void ResetSubjectFile();

    void UpdateSubjectFileLineEdit( const QString& filePath );

    void SaveCheckedSubjects();


    void CheckAllVisibleSubjects();

    void UnCheckAllVisibleSubjects();

    void SelectSubject( QListWidgetItem *item );


    void SortAndDisplaySubjects();

    void SearchAllSubjects();

    void SetCaseSensitivity( bool checked );


    /****************** Execution Tab ******************/
    void SetCurrentFiberNameInfo();

    void SetOutputDir();

    void UpdateOutputDir( const QString& path );

    void SetMVCMPath();

    void UpdateMVCMPath( const QString& path );

    void RunningMatlabAfterFileGeneration( const bool& choice );

    void SetMatlabExe();

    void UpdateMatlabExe( const QString& executable );


    void RunFADTTS();

    void StopFADTTS();

    void DisplayLog();

    void ClearLog();

    void OnThreadFinished();


    /************** Plotting  Tab **************/
    void SetPlotsAvailable( const QStringList &plotsAvailable );

    void SetAllPropertiesUsed( const QStringList& propertiesGiven );

    void SetAllCovariatesUsed( QMap< int, QString > covariatesForDisplay );

    void UpdateCovariatesAvailable( const QMap< int, QString >& covariateGiven );

    void PropertiesForDisplay( QStringList propertiesForDisplay );


    void UpdatePropertyPlotColor( const QString& property );

    void UpdateCovariatePlotColor( const QString& covariate );


    void SelectPlot( const QString& plotSelected );

    void SelectPropertyToPlot( const QString& propertySelected );

    void SelectCovariateToPlot( const QString& covariateSelected );

    void UpdateLineForDisplay( QListWidgetItem *item );

    void CheckAllToDisplay();

    void UncheckAllToDisplay();


    void OnYMinToggled( const bool& checkState );

    void OnYMaxToggled( const bool& checkState );


    void DisplayPlot();

    void ResetPlot();

    void LoadPlotSettings();

    void SavePlotSettings();


private:
    static const QColor m_green;
    static const QColor m_red;
    static const QColor m_grey;
    static const QColor m_yellow;
    static const QColor m_lightBlack;

    static const QString m_csvSeparator;

    static const int m_iconSize;

    QPixmap m_okPixmap;
    QPixmap m_koPixmap;
    QPixmap m_warningPixmap;

    Qt::CaseSensitivity caseSensitivity;

    Data m_data;
    InfoDialog m_infoDialog;
    QSharedPointer<EditInputDialog> m_editInputDialog;
    Processing m_processing;
    MatlabScript m_matlabScript;
    MatlabThread *m_matlabThread;
    Plot *m_plot;

    QVTKWidget *m_qvtkWidget;

    QFile *m_logFile;
    QTextStream* m_textStreamLog;
    QPlainTextEdit *m_logWindow;
    QProgressBar *m_progressBar;

    typedef QMap<QString, QLabel*> labelMapType;
    labelMapType m_inputTabIconLabelMap, m_paramTabFileDataSizeLabelMap, m_inputFileInformationLabelMap;
    typedef QMap<QString, QCheckBox*> checkBoxMapType;
    checkBoxMapType m_paramTabFileCheckBoxMap;
    typedef QMap<QString, QLineEdit*> lineEditMapType;
    lineEditMapType m_inputTabInputFileLineEditMap;
    typedef QMap<QString, QPushButton*> pushButtonMapType;
    pushButtonMapType m_inputTabAddInputFilePushButtonMap, m_inputTabEditInputFilePushButtonMap;
    typedef QMap< int, QPair< QString, QComboBox*> > comboBoxMapType;
    comboBoxMapType m_propertiesColorsComboBoxMap, m_covariatesColorsComboBoxMap;
    typedef QMap< int, QPair< QString, QLineEdit* > > covariateNameMap;
    covariateNameMap m_covariatesNameLineEditMap;
    typedef QMap< int, QPair< QString, QPair< bool, QString > > > displayMap;
    displayMap m_propertiesForDisplay, m_covariatesForDisplay, m_currentLinesForDisplay;

    QMap< int, QString > m_propertySelected;

    QListWidget *m_covariateListWidget, *m_matchedSubjectListWidget, *m_unmatchedSubjectListWidget,
    *m_plotListWidget;

    QComboBox *m_plotComboBox, *m_propertyComboBox, *m_covariateComboBox;

    QLineEdit *m_subjectFileLineEdit;

    QStringList m_loadedSubjects ,m_plotRawDataStats, m_plotBetaByPorperties, m_plotBetaByCovariates;

    QString m_currentInputFileDir, m_currentSubjectFileDir,
    m_currentMatlabExeDir, m_mvcmPath, m_previousPlotSelected;

    int m_nbrSelectedSubjects;

    bool m_areSubjectsLoaded, m_areLinesForDisplayProperties;


    /********* Configuration & Events *********/
    void InitFADTTSWindow();

    void InitMenuBar();

    void InitInputTab();

    void InitSubjectCovariateTab();

    void InitExecutionTab();

    void InitPlottingTab();


    void UpdateEditInputDialogCurrentDir( const QString newfilePath );

    QDir UpdateCurrentDir( const QString newfilePath, QString& currentDir );

    void SetDir( QDir& dir, QString filePath, QString currentDir );


    void WarningPopUp( const QString warningMessage );

    void CriticalPopUp( const QString criticalMessage );

    void DisplayIcon( QLabel *label , const QPixmap icon );


    /*************** Input Tab ***************/
    void UpdateLineEditsAfterAddingMultipleFiles( const QStringList fileList );


    void UpdateInputFileInformation( const QString prefID );

    void DisplayInputLineEditIcon( const QString prefID , const QPixmap icon );

    void LaunchEditInputDialog( QString prefID );


    void SetInfoCovariateFileSubjectColumnID();


    /*********** Subjects/Covariates Tab **********/
    void UpdateAvailableFileParamTab();


    void SetCheckStateAllVisibleSubjects( Qt::CheckState checkState );


    void DisplaySortedSubjects( const QStringList matchedSubjectList, const QMap<QString, QStringList > unMatchedSubjectMap );

    void DisplaySubjectInformation();

    void DisplayNbrSubjectSelected();


    int SearchSubjects( QListWidget *list );


    /*************** Execution Tab ****************/
    QStringList GetSelectedPrefixes();

    QMap< QPair< int, QString >, bool> GetSelectedInputFiles();

    QMap< QPair< int, QString >, QList<QStringList> > GetFileDataOfSelectedFiles();

    QMap<int, QString> GetSelectedCovariates();


    QString GenerateSelectedSubjectFile( QString outputDir );


    bool IsRunFADTTSOK( QString fiberName, QMap<int, QString> selectedCovariates );

    void SetMatlabThread( QString fiberName, QMap<int, QString> selectedCovariates );

    void SetLogDisplay( QString outputDir, QString fiberName, QMap< QPair< int, QString >, bool> matlabInputFiles,
                        QMap<int, QString> selectedCovariates, int nbrPermutations, bool omnibusON, bool postHocON );


    /************** Plotting  Tab **************/
    void SetPropertyEdition( const QStringList &propertiesAvailable );

    void SetCovariatesEdition( QMap< int, QString > covariatesForDisplay );

    void SetColorsComboBox( QComboBox* &comboBox );

    void SetCheckStateLinesToDisplay( Qt::CheckState checkState );

    void AddLinesForDisplay( bool isSelectionProperties );

    void EditCovariatesNames();


    void HideShowPlotTab();

    void ResetPlotTab();


    void PlotSelected( bool isPlotSelected, bool propertySelectionAvailable, bool covariateSelectionAvailable,
                       bool lineSelectionAvailable, QString plotSelected );

};

#endif // FADTTSWINDOW_H
