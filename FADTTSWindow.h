#ifndef FADTTSWINDOW_H
#define FADTTSWINDOW_H

#include "FADTTSWindowConfig.h"
#include "EditInputDialog.h"
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
    void OnLoadParaSettings();

    void OnSaveParaSettings();

    void OnLoadSoftSettings();

    void OnSaveSoftSettings();

    void OnDisplayAbout();


    void closeEvent( QCloseEvent *event );


    /*************** Input Tab ***************/
    void OnAddInputFiles();


    void OnAddInputFile( const QString& prefID );

    void OnSettingInputFile( const QString& prefID );

    void OnEditInputFile( const QString& prefID );


    void OnUpdatingInputFile( const QString& newFilePath, const QString& prefID );

    void OnUpdatingCovariateColumnID( const int& newCovariateColumnID );


    /*********** Subjects/Covariates Tab ***********/
    void OnCovariateFileToggled();

    void OnCovariateClicked( QListWidgetItem *item );

    void OnCheckAllCovariates();

    void OnUnCheckAllCovariates();


    void OnInputToggled();

    void OnSearch();

    void OnSetCaseSensitivityToggled( const bool& checked );


    void OnSubjectClicked( QListWidgetItem *item );

    void OnCheckAllVisibleSubjects();

    void OnUnCheckAllVisibleSubjects();

    void OnSaveCheckedSubjects();


    void OnLoadList();

    void OnResetList();

    void OnSettingList( const QString& filePath );


    /****************** Execution Tab ******************/
    void OnSettingFiberName();

    void OnBrowsingOutputDir();

    void OnSettingOutputDir( const QString& path );

    void OnBrowsingMVCMPath();

    void OnSettingMVCMPath( const QString& path );

    void OnBrowsingMatlabExe();

    void OnSettingMatlabExe( const QString& executable );


    void OnRunMatlabToggled( const bool& choice );

    void OnRunOnKillDevil( const bool& choice );


    void OnRun();

    void OnStop();


    void OnDisplayLog();

    void OnClearLog();


    void OnMatlabThreadFinished();


    /************** Plotting  Tab **************/
    void OnSettingPlotsUsed( const QStringList &plotsAvailable );

    void OnSettingAllPropertiesUsed( const QMap< int, QString >& propertiesGiven );

    void OnSettingAllCovariatesUsed( QMap< int, QString > covariatesForDisplay );


    void OnUpdatingCovariatesAvailable( const QMap< int, QString >& covariateGiven );

    void OnUpdatingPropertyPlotColor( const QString& property );

    void OnUpdatingCovariatePlotColor( const QString& covariate );


    void OnPlotSelection( const QString& plotSelected );

    void OnPropertySelection( const QString& propertySelected );

    void OnCovariateSelection( const QString& covariateSelected );


    void OnLineForDisplayClicked( QListWidgetItem *item );

    void OnCheckAllLinesToDisplay();

    void OnUncheckAllToDisplay();


    void OnYMinToggled( const bool& checkState );

    void OnYMaxToggled( const bool& checkState );


    void OnDisplayPlot();

    void OnResetPlot();

    void OnLoadPlotSettings();

    void OnSavePlotSettings();


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

    typedef QMap< int, QPair< QString, QLabel* > > NameLabelMap;
    NameLabelMap m_propertiesNameLabelMap, m_covariatesNameLabelMap;

    typedef QMap< int, QPair< QString, QComboBox*> > comboBoxMapType;
    comboBoxMapType m_propertiesColorsComboBoxMap, m_covariatesColorsComboBoxMap;

    typedef QMap< int, QPair< QString, QLineEdit* > > covariateNameLineEditMap;
    covariateNameLineEditMap m_covariatesNameLineEditMap;

    typedef QMap< int, QPair< QString, QPair< bool, QString > > > displayMap;
    displayMap m_propertiesForDisplay, m_covariatesForDisplay, m_currentLinesForDisplay;


    QMap< int, QString > m_propertySelected;

    QListWidget *m_covariateListWidget, *m_matchedSubjectListWidget, *m_unmatchedSubjectListWidget,
    *m_plotListWidget;

    QComboBox *m_plotComboBox, *m_propertyComboBox, *m_covariateComboBox;

    QLineEdit *m_subjectFileLineEdit;

    QStringList m_loadedSubjects;

    QString m_currentInputFileDir, m_currentSubjectFileDir,
    m_currentMatlabExeDir, m_mvcmPath, m_previousPlotSelected;

    int m_nbrSelectedSubjects;

    bool m_areSubjectsLoaded, m_areLinesForDisplayProperties, m_isMatlabExeFound;


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


    QString GetInputFileInformation( const QString prefID );

    void DisplayFileInformation();


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

    QMap< int, QString > GetSelectedInputFiles();

    QMap<int, QString> GetSelectedCovariates();


    QStringList GenerateSelectedSubjectFile( QString outputDir );


    bool IsRunFADTTSOK( QString fiberName, QMap<int, QString> selectedCovariates );

    void SetMatlabThread( QString fiberName, QMap<int, QString> selectedCovariates );

    void SetLogDisplay( QString outputDir, QString fiberName, QMap<int, QString> matlabInputFiles,
                        QMap<int, QString> selectedCovariates );


    /************** Plotting  Tab **************/
    void SetPropertyEdition( const QStringList &propertiesAvailable );

    void SetCovariatesEdition( QMap< int, QString > covariatesForDisplay );

    void SetColorsComboBox( QComboBox* &comboBox );


    void ResetPropertyEdition();

    void ResetCovariateEdition();


    void SetCheckStateLinesToDisplay( Qt::CheckState checkState );

    void AddLinesForDisplay( bool isSelectionProperties );

    void EditCovariatesNames();


    void SetPlotTab();

    void HideLegendBinaryCovariate( bool hideLegend );

    void ResetPlotTab();


    void SetPlotOptions( bool isPlotSelected, bool propertySelectionAvailable, bool covariateSelectionAvailable,
                         bool lineSelectionAvailable );

    void PropertiesForDisplay( QStringList propertiesForDisplay );

};

#endif // FADTTSWINDOW_H
