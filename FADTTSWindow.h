#ifndef FADTTSWINDOW_H
#define FADTTSWINDOW_H

#include "FADTTSWindowConfig.h"
#include "EditInputDialog.h"
#include "Data.h"
#include "Processing.h"
#include "MatlabThread.h"
#include "Plot.h"

#include <QSignalMapper>
#include <QFileSystemWatcher>
#include <QScrollBar>
#include <QProgressBar>
#include <QDebug>

class FADTTSWindow : public FADTTSWindowConfig
{
//    friend class TestFADTTSWindow;

    Q_OBJECT

public:
    FADTTSWindow();

    ~FADTTSWindow();


private slots:
    /********** Configuration & Events **********/
    void OnLoadParaSettings();

    void OnSaveParaSettings();

    void OnLoadSoftSettings();

    void OnSaveSoftSettings();

    void OnDisplayAbout();


    void closeEvent( QCloseEvent *event );


    /**************** Input  Tab ****************/
    void OnAddInputFiles();


    void OnAddInputFile( const int& diffusionPropertyID );

    void OnSettingInputFile( const int& diffusionPropertyID );

    void OnEditInputFile( const int& diffusionPropertyID );


    void OnUpdatingInputFile( const int &diffusionPropertyIndex, const QString& newFilePath );

    void OnUpdatingCovariateColumnID( const int& newCovariateColumnID );


    /********* Subjects/Covariates Tab *********/
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


    /************** Execution Tab **************/
    void OnSettingFiberName( const QString& fibername );

    void OnBrowsingOutputDir();

    void OnSettingOutputDir( const QString& path );

    void OnBrowsingMVCMPath();

    void OnSettingMVCMPath( const QString& path );

    void OnBrowsingMatlabExe();

    void OnSettingMatlabExe( const QString& executable );


    void OnRunMatlabToggled( const bool& choice );

    void OnRunOnKillDevil( const bool& choice );

    void OnKillDevilQueueSelection( const QString& queue );

    void OnKillDevilAllocatedMemoryChanged( const int& allocatedMemory );


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

    typedef QMap<int, QPushButton*> pushButtonMapType;
    typedef QMap<int, QLineEdit*> lineEditMapType;
    typedef QMap<int, QCheckBox*> checkBoxMapType;
    typedef QMap<int, QLabel*> labelMapType;
    typedef QMap< int, QPair< QString, QPair< bool, QString > > > displayMapType;
    typedef QMap< int, QPair< QString, QLabel* > > nameLabelMapType;
    typedef QMap< int, QPair< QString, QComboBox*> > comboBoxMapType;
    typedef QMap< int, QPair< QString, QLineEdit* > > covariateNameLineEditMapType;


    Data m_data;

    Processing m_processing;


    /**************** Input Tab ****************/
    QSharedPointer<EditInputDialog> m_editInputDialog;

    pushButtonMapType m_inputTabAddInputFilePushButtonMap, m_inputTabEditInputFilePushButtonMap;
    lineEditMapType m_inputTabInputFileLineEditMap;
    labelMapType m_inputTabIconLabelMap, m_inputFileInformationLabelMap;

    QString m_currentInputFileDir;


    /********* Subjects/Covariates Tab *********/
    Qt::CaseSensitivity caseSensitivity;

    QListWidget *m_covariateListWidget, *m_matchedSubjectListWidget, *m_unmatchedSubjectListWidget;

    checkBoxMapType m_paramTabFileCheckBoxMap;
    labelMapType m_paramTabFileDataSizeLabelMap;

    QMap<int, QString> m_selectedCovariates;

    QLineEdit *m_subjectFileLineEdit;
    QStringList m_loadedSubjects;
    QString m_currentSubjectFileDir;
    int m_nbrSelectedSubjects;
    bool m_areSubjectsLoaded;


    /************** Execution Tab **************/
    MatlabThread *m_matlabThread;

    QPlainTextEdit *m_logWindow;
    QTextStream* m_textStreamLog;
    QFile *m_logFile;

    QProgressBar *m_progressBar;

    QMap< int, QString > m_propertySelected, m_selectedFiles;

    QString m_fibername, m_currentMatlabExeDir, m_mvcmPath;

    bool m_isMatlabExeFound;


    /************** Plotting  Tab **************/
    QSharedPointer<QVTKWidget> m_qvtkWidget;

    Plot *m_plot;

    QListWidget *m_plotListWidget;

    displayMapType m_propertiesForDisplay, m_covariatesForDisplay, m_currentLinesForDisplay;

    nameLabelMapType m_propertiesNameLabelMap, m_covariatesNameLabelMap;
    comboBoxMapType m_propertiesColorsComboBoxMap, m_covariatesColorsComboBoxMap;
    covariateNameLineEditMapType m_covariatesNameLineEditMap;

    QComboBox *m_plotComboBox, *m_propertyComboBox, *m_covariateComboBox;

    QString m_previousPlotSelected;

    bool m_areLinesForDisplayProperties;




    /********** Configuration & Events **********/
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


    /**************** Input Tab ****************/
    void UpdateLineEditsAfterAddingMultipleFiles( const QStringList fileList );


    void UpdateInputFileInformation( const int diffusionPropertyID );

    void DisplayInputLineEditIcon( const int diffusionPropertyID, const QPixmap icon );

    void LaunchEditInputDialog( const int diffusionPropertyID );


    QString GetInputFileInformation( const int diffusionPropertyID );

    void DisplayFileInformation();


    void SetInfoCovariateFileSubjectColumnID();


    /********* Subjects/Covariates Tab *********/
    void UpdateAvailableDiffusionProperties();

    QMap< int, bool > GetDiffusionPropertiesCheckState( QMap< int, QCheckBox* > checkBoxMap );


    void SetCheckStateAllCovariates( Qt::CheckState checkState );

    void SetSelectedCovariates();


    void SetCheckStateAllVisibleSubjects( Qt::CheckState checkState );


    void DisplaySortedSubjects( const QStringList matchedSubjectList, const QMap<QString, QList<int> > unMatchedSubjectMap );

    void DisplaySubjectInformation();

    void DisplayNbrSubjectSelected();


    int SearchSubjects( QListWidget *list );


    /************** Execution Tab **************/
    void SetSelectedInputFiles();


    QStringList GenerateSelectedSubjectFile( QString outputDir );


    bool IsRunFADTTSOK();

    void SetMatlabScript();

    void SetLogDisplay( QString outputDir, QMap<int, QString> matlabInputFiles,
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
