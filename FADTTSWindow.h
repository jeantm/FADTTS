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
    friend class TestFADTTSWindow; /** For unit tests **/

    Q_OBJECT

public:
    FADTTSWindow();

    ~FADTTSWindow();


private slots:
    /********** Configuration & Events **********/
    void OnLoadParaSettings(); // Tested

    void OnSaveParaSettings(); // Tested

    void OnLoadSoftSettings(); // Tested

    void OnSaveSoftSettings(); // Tested

    void OnDisplayAbout(); /// Not tested


    void closeEvent( QCloseEvent *event ); /// Not tested


    /**************** Input  Tab ****************/
    void OnSettingInputFile( const int& diffusionPropertyID ); // Tested

    void OnAddInputFiles(); /// Not directly tested

    void OnAddInputFile( const int& diffusionPropertyID ); /// Not directly tested

    void OnEditInputFile( const int& diffusionPropertyID ); /// Not tested


    void OnUpdatingInputFile( const int &diffusionPropertyIndex, const QString& newFilePath ); // Tested

    void OnUpdatingSubjectColumnID( const int& newSubjectColumnID ); // Tested


    /********* Subjects/Covariates Tab *********/
    void OnCovariateFileToggled(); // Tested

    void OnCovariateClicked( QListWidgetItem *item ); // Tested

    void OnCheckAllCovariates(); // Tested

    void OnUnCheckAllCovariates(); // Tested


    void OnSearch(); // Tested

    void OnInputToggled(); // Tested

    void OnSetCaseSensitivityToggled( const bool& checked ); // Tested


    void OnSubjectClicked( QListWidgetItem *item ); // Tested

    void OnCheckAllVisibleSubjects(); // Tested

    void OnUnCheckAllVisibleSubjects(); // Tested

    void OnSaveCheckedSubjects(); /// Not tested


    void OnLoadSubjectList(); /// Not tested

    void OnResetSubjectList(); // Tested

    void OnSettingSubjectList( const QString& filePath ); // Tested


    /************** Execution Tab **************/
    void OnSettingFiberName( const QString& fibername ); // Tested

    void OnBrowsingOutputDir(); /// Not tested

    void OnSettingOutputDir( const QString& path ); // Tested

    void OnBrowsingMVCMPath(); /// Not tested

    void OnSettingMVCMPath( const QString& path );

    void OnBrowsingMatlabExe(); /// Not tested

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
    void OnSettingPlotsUsed( const QStringList& plotsAvailable ); // Tested

    void OnSettingAllPropertiesUsed( const QMap< int, QString >& allPropertiesUsed ); // Tested

    void OnSettingAllCovariatesUsed( const QMap< int, QString >& allCovariatesUsed ); // Tested


    void OnUpdatingCovariatesAvailable( const QMap< int, QString >& covariateGiven ); // Tested

    void OnUpdatingPropertyPlotColor( const QString& property ); // Tested

    void OnUpdatingCovariatePlotColor( const QString& covariate ); // Tested


    void OnPlotSelection( const QString& plotSelected ); // Tested

    void OnPropertySelection( const QString& propertySelected ); // Tested

    void OnCovariateSelection( const QString& covariateSelected ); // Tested


    void OnLineForDisplayClicked( QListWidgetItem *item ); /** /!\ Write Test /!\ **/

    void OnCheckAllLinesToDisplay(); // Tested

    void OnUncheckAllToDisplay(); // Tested


    void OnYMinToggled( const bool& checkState ); /** /!\ PB with test /!\ **/

    void OnYMaxToggled( const bool& checkState ); /** /!\ PB with test /!\ **/


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
    Qt::CaseSensitivity m_caseSensitivity;

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

    QMap< int, QString > m_propertySelected, m_selectedFiles, m_previousPropertiesUsed, m_previousCovariatesUsed;

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
    void InitMenuBar(); /// Not tested

    void InitInputTab(); /// Not tested

    void InitSubjectCovariateTab(); /// Not tested

    void InitExecutionTab(); /// Not tested

    void InitPlottingTab(); /// Not tested

    void InitFADTTSWindow(); /// Not tested


    QDir UpdateCurrentDir( QString newfilePath, QString& currentDir ); // Tested

    QDir SetDir( QString filePath, QString currentDir ); // Tested


    void WarningPopUp( QString warningMessage ); /// Not tested

    void CriticalPopUp( QString criticalMessage ); /// Not tested

    void DisplayIcon( QLabel *label , const QPixmap& icon ); // Tested


    /**************** Input Tab ****************/
    QString GetInputFileInformation( int diffusionPropertyID ) const; // Tested

    void DisplayFileInformation(); // Tested

    void DisplayInputLineEditIcon( int diffusionPropertyID, const QPixmap& icon ); // Tested

    void UpdateInputFileInformation( int diffusionPropertyID ); // Tested

    void UpdateLineEditsAfterAddingMultipleFiles( const QStringList& fileList ); // Tested


    void SetInfoSubjectColumnID(); // Tested

    void LaunchEditInputDialog( int diffusionPropertyID ); /// Not tested


    /********* Subjects/Covariates Tab *********/
    void SetSelectedInputFiles(); // Tested

    void UpdateAvailableDiffusionProperties( int diffusionPropertyID ); // Tested

    void InitAvailableDiffusionProperties(); // Tested

    QMap< int, bool > GetDiffusionPropertiesCheckState();  // Tested


    void SetSelectedCovariates(); // Tested

    void SetCheckStateAllCovariates( Qt::CheckState checkState ); // Tested


    void DisplaySubjectInformation(); // Tested

    void DisplayNbrSubjectSelected(); // Tested

    void DisplaySortedSubjects( const QStringList& matchedSubjectList, const QMap<QString, QList<int> >& unMatchedSubjectMap ); // Tested

    void UpdateSubjectList(); /// Not directly tested

    void SetCheckStateAllVisibleSubjects( Qt::CheckState checkState ); // Tested

    int SearchSubjects( QListWidget *list ); // Tested


    /************** Execution Tab **************/
    QStringList GenerateSelectedSubjectFile( QString outputDir ); // Tested


    bool IsFADTTSReadyToBeRun(); /// Not tested

    void SetLogDisplay( QString outputDir, const QMap< int, QString >& matlabInputFiles, const QMap<int, QString>& selectedCovariates ); /// Not tested

    void SetMatlabScript(); /// Not tested


    /************** Plotting  Tab **************/
    void SetColorsComboBox( QComboBox* &comboBox ); // Tested

    void ResetPropertyEdition(); // Tested

    void SetPropertyEdition( const QStringList& propertiesAvailable ); // Tested

    void SetPropertiesForDisplay( const QStringList& propertiesForDisplay ); // Tested

    void ResetCovariateEdition(); // Tested

    void SetCovariateEdition( const QMap< int, QString >& allCovariatesUsed ); // Tested

    void SetCovariatesForDisplay( const QMap< int, QString >& covariatesForDisplay ); // Tested


    void SetPlotOptions( bool isPlotSelected, bool propertySelectionAvailable,
                         bool covariateSelectionAvailable, bool lineSelectionAvailable ); /** /!\ PB with test /!\ **/

    void AddLinesForDisplay( bool isSelectionProperties ); // Tested

    void SetCheckStateLinesToDisplay( Qt::CheckState checkState ); // Tested

    void EditCovariatesNames(); // Tested


    void ResetPlotTab(); // Tested

    void SetPlotTab(); // Tested


    void HideLegendBinaryCovariate( bool hideLegend ); /// Not tested
};

#endif // FADTTSWINDOW_H
