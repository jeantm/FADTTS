#ifndef FADTTSWINDOW_H
#define FADTTSWINDOW_H

#include "ui_FADTTSWindow.h"

#include "EditInputDialog.h"
#include "QCThresholdDialog.h"
#include "Data.h"
#include "Processing.h"
#include "MatlabThread.h"
#include "Plot.h"
#include "Log.h"

#include <QMainWindow>
#include <QSignalMapper>
#include <QFileSystemWatcher>
#include <QScrollBar>
#include <QProgressBar>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>



class FADTTSWindow : public QMainWindow, public Ui::FADTTSWindow
{
    friend class TestFADTTSWindow; /** For unit tests **/

    Q_OBJECT

public:

    explicit FADTTSWindow( QWidget *parent = 0, Qt::WindowFlags f = 0 );

    ~FADTTSWindow();

    void LoadParaConfiguration( QString filename ); // Tested

    void LoadSoftConfiguration( QString filename ); // Tested


private slots:
    /********** Configuration & Events **********/
    void OnLoadParaConfiguration(); // Tested

    void OnSaveParaConfiguration(); // Tested

    void OnLoadSoftConfiguration(); // Tested

    void OnSaveSoftConfiguration(); // Tested

    void OnSaveNoGUIConfiguration();

    void OnDisplayUserGuide();

    void OnDisplayAbout(); /// Not tested


    void closeEvent( QCloseEvent *event ); /// Not tested


    /**************** Input  Tab ****************/
    void OnSettingInputFile( int diffusionPropertyID ); // Tested

    void OnAddInputFiles(); // Not Directly Tested

    void OnAddInputFile( int diffusionPropertyID ); // Not Directly Tested

    void OnEditInputFile( int diffusionPropertyID ); /// Not tested


    void OnCovariateClicked( QListWidgetItem *item ); // Tested

    void OnCheckAllCovariates(); // Tested

    void OnUnCheckAllCovariates(); // Tested


    void OnUpdatingInputFile( const int &diffusionPropertyIndex, const QString& newFilePath ); // Tested

    void OnUpdatingSubjectColumnID( int newSubjectColumnID ); // Tested


    /**************** Subjects Tab ****************/
    void OnApplyQCThreshold(); /// Not tested

    void OnApplyingQCThreshold( const QStringList& subjectsCorrelated, const QStringList& subjectsNotCorrelated, double qcThreshold ); // Tested


    void OnSearch(); // Tested

    void OnInputToggled(); // Tested

    void OnSetCaseSensitivityToggled( bool checked ); // Tested


    void OnSubjectClicked( QListWidgetItem *item ); // Tested

    void OnCheckAllVisibleSubjects(); // Tested

    void OnUnCheckAllVisibleSubjects(); // Tested

    void OnSaveCheckedSubjects(); // Tested


    void OnLoadSubjectList(); // Not Directly Tested

    void OnResetSubjectList(); // Tested

    void OnSettingSubjectList( const QString& filePath ); // Tested


    /************** Execution Tab **************/
    void OnBrowsingOutputDir(); // Not Directly Tested

    void OnSettingOutputDir( const QString& path ); // Not Directly Tested

    void OnBrowsingMVCMPath(); // Not Directly Tested

    void OnSettingMVCMPath( const QString& path ); // Not Directly Tested

    void OnBrowsingMatlabExe(); // Not Directly Tested

    void OnSettingMatlabExe( const QString& executable ); // Not Directly Tested


    void OnRunMatlabToggled( bool isChecked ); // Not Directly Tested

    void OnRun(); /// Not tested*

    void OnStop(); /// Not tested*

    void OnUpdatingLogActivity( const QString& LogActivity ); /// Not tested*

    void OnClearLog(); /// Not tested*


    void OnMatlabThreadFinished(); /// Not tested


    /************** Plotting  Tab **************/
    void OnBrowsingPlotDir(); /// Not tested

    void OnSettingPlotDir( const QString& path ); // Not Directly Tested

    void OnSettingPlotFibername( const QString& fibername ); /// Not tested*


    void OnSettingPlotsUsed( const QStringList& plotsAvailable ); // Tested

    void OnSettingAllPropertiesUsed( const QMap< int, QString >& allPropertiesUsed ); // Tested

    void OnSettingAllCovariatesUsed( const QMap< int, QString >& allCovariatesUsed ); // Tested


    void OnUpdatingCovariatesAvailable( const QMap< int, QString >& covariateGiven ); // Tested

    void OnUpdatingPropertyPlotColor( const QString& property ); // Tested

    void OnUpdatingCovariatePlotColor( const QString& covariate ); // Tested


    void OnPlotSelection( const QString& plotSelected ); // Not Directly Tested

    void OnPropertySelection( const QString& propertySelected ); // Tested

    void OnCovariateSelection( const QString& covariateSelected ); // Tested


    void OnLineForDisplayClicked( QListWidgetItem *item ); // Tested

    void OnCheckAllLinesToDisplay(); // Tested

    void OnUncheckAllToDisplay(); // Tested


    void OnSettingLinesSelected( const QStringList& linesSelected ); // Tested


    void OnUseCustomizedTitle( bool checkState ); /// Not tested

    void OnUpdatingPlotTitle(); // Not Directly Tested


    void OnUpdatingPlotGrid( bool checkState ); // Not Directly Tested

    void OnUpdatingAbscissaNotation( bool checkState );

    void OnUseCustomizedAxis( bool checkState ); /// Not tested

    void OnYMinToggled( bool checkState ); /// Not tested

    void OnYMaxToggled( bool checkState ); /// Not tested

    void OnYMinValueChanged( double yMinValue ); // Tested

    void OnYMaxValueChanged( double yMaxValue ); // Tested

    void OnUpdatingPlotAxis(); // Not Directly Tested


    void OnUsingLegend( bool checkState );

    void OnUpdatingLegend( const QString& legendPosition ); // Not Directly Tested


    void OnUpdatingPvalueThreshold( double pvalueThreshold ); // Not Directly Tested

    void OnUpdatingLineWidth( double lineWidth ); // Not Directly Tested

    void OnUpdatingSelectedColorLine( const QString& color ); // Not Directly Tested

    void OnUpdatingMarkerType( const QString& markerType ); // Not Directly Tested

    void OnUpdatingMarkerSize( double markerSize ); // Not Directly Tested


    void OnDisplayPlot(); // Not Directly Tested

    void OnResetPlot(); // Not Directly Tested

    void OnLoadPlotSettings(); // Tested

    void OnSavePlotSettings(); // Tested


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

    typedef QMap< int, QPushButton* > pushButtonMapType;
    typedef QMap< int, QLineEdit* > lineEditMapType;
    typedef QMap< int, QCheckBox* > checkBoxMapType;
    typedef QMap< int, QLabel* > labelMapType;
    typedef QMap< int, QPair< QString, QPair< bool, QString > > > displayMapType;
    typedef QMap< int, QPair< QString, QLabel* > > nameLabelMapType;
    typedef QMap< int, QPair< QString, QComboBox* > > comboBoxMapType;
    typedef QMap< int, QPair< QString, QLineEdit* > > covariateNameLineEditMapType;


    Data m_data;

    Processing m_processing;


    /**************** Input Tab ****************/
    QSharedPointer< EditInputDialog > m_editInputDialog;

    QListWidget *m_covariateListWidget;

    pushButtonMapType m_inputTabAddInputFilePushButtonMap, m_inputTabEditInputFilePushButtonMap;
    lineEditMapType m_inputTabInputFileLineEditMap;
    labelMapType m_inputTabIconLabelMap, m_inputFileInformationLabelMap;

    QString m_currentInputFileDir;


    /**************** Subjects Tab ****************/
    QSharedPointer< QCThresholdDialog > m_qcThresholdDialog;

    Qt::CaseSensitivity m_caseSensitivity;

    QListWidget *m_matchedSubjectListWidget, *m_unmatchedSubjectListWidget;

    checkBoxMapType m_paramTabFileCheckBoxMap;

    QLineEdit *m_subjectFileLineEdit;
    QStringList m_loadedSubjects, m_failedQCThresholdSubjects;
    QString m_currentSubjectFileDir;
    bool m_areSubjectsLoaded;


    /************** Execution Tab **************/
    Log *m_log;
    MatlabThread *m_matlabThread;

    QTextEdit *m_logWindow;

    QProgressBar *m_progressBar;

    QMap< int, QString > m_propertySelected, m_selectedFiles, m_selectedCovariates;

    QString m_fibername, m_currentMatlabExeDir, m_mvcmPath;

    double m_qcThreshold;

    int m_nbrSelectedSubjects;

    bool m_isMatlabExeFound;


    /************** Plotting  Tab **************/
    QSharedPointer< QVTKWidget > m_qvtkWidget;

    Plot *m_plot;

    QListWidget *m_lineDisplayedListWidget, *m_lineSelectedListWidget;

    displayMapType m_propertiesForDisplay, m_covariatesForDisplay, m_currentLinesForDisplay;

    nameLabelMapType m_propertiesNameLabelMap, m_covariatesNameLabelMap;
    comboBoxMapType m_propertiesColorsComboBoxMap, m_covariatesColorsComboBoxMap;
    covariateNameLineEditMapType m_covariatesNameLineEditMap;

    QComboBox *m_plotComboBox, *m_propertyComboBox, *m_covariateComboBox;

    QMap< int, QString > m_previousPropertiesUsed, m_previousCovariatesUsed;

    QString m_plotSelected;

    bool m_isPlotReady, m_areLinesForDisplayProperties;



    /********** Configuration & Events **********/
    void SaveParaConfiguration( QString filename ); // Tested

    void SaveSoftConfiguration( QString filename ); // Tested

    void SaveNoGUIConfiguration( QString filename ); // Tested


    void InitMenuBar(); // Not Directly Tested

    void InitInputTab(); // Not Directly Tested

    void InitSubjectCovariateTab(); // Not Directly Tested

    void InitExecutionTab(); // Not Directly Tested

    void InitPlottingTab(); // Not Directly Tested

    void InitFADTTSWindow(); // Not Directly Tested


    QDir UpdateCurrentDir( QString newfilePath, QString& currentDir ); // Tested

    QDir SetDir( QString filePath, QString currentDir ); // Tested


    void WarningPopUp( QString warningMessage ); /// Not tested

    void CriticalPopUp( QString criticalMessage ); /// Not tested

    void DisplayIcon( QLabel *label , const QPixmap& icon ); // Tested


    /**************** Input Tab ****************/
    void SetCovariateListVisible( bool visible ); /// Not tested

    void SetCovariateListWidget(); // Tested

    QString GetInputFileInformation( int diffusionPropertyID ) const; // Tested

    void AddAtlas( const QList< QStringList >& fileData ); /// Not tested*

    void DisplayFileInformation(); // Tested

    void DisplayInputLineEditIcon( int diffusionPropertyID, const QPixmap& icon ); // Tested

    void UpdateInputFileInformation( int diffusionPropertyID ); // Tested

    void UpdateLineEditsAfterAddingMultipleFiles( const QStringList& fileList ); // Tested


    void SetCheckStateAllCovariates( Qt::CheckState checkState );  // Tested


    void SetInfoSubjectColumnID(); // Tested


    void LaunchEditInputDialog( int diffusionPropertyID ); // Not Directly Tested


    /**************** Subjects Tab ****************/
    void SetSelectedInputFiles(); // Tested

    void UpdateAvailableDiffusionProperties( int diffusionPropertyID ); // Tested

    void InitAvailableDiffusionProperties(); // Tested

    QMap< int, bool > GetDiffusionPropertiesCheckState();  // Tested


    void DisplaySubjectInformation(); // Tested

    void DisplayNbrSubjectSelected(); // Tested

    void DisplaySortedSubjects( const QStringList& matchedSubjectList, const QMap< QString, QList< int > >& unMatchedSubjectMap ); // Tested

    void UpdateSubjectList(); // Tested

    void SetCheckStateAllVisibleSubjects( Qt::CheckState checkState ); // Tested

    int SearchSubjects( QListWidget *list ); // Tested

    void SaveCheckedSubjects( QString filePath ); // Tested


   QStringList GetCheckedMatchedSubjects(); // Tested

   QMap< QString, QList< QStringList > > GetPropertyRawData(); // Tested


    /************** Execution Tab **************/
    void GenerateFailedQCThresholdSubjectFile( QString outputDir ); // Tested

    QStringList GenerateSelectedSubjectFile( QString outputDir ); // Tested

    void SetSelectedCovariates(); // Tested

    void SetFibername(); // Tested

    bool canFADTTSterBeRun(); /// Not tested

    void SetMatlabScript(); // Not Directly Tested


    /************** Plotting  Tab **************/
    void SetColorsComboBox( QComboBox* &comboBox ); // Tested

    void ResetPropertyEdition(); // Tested

    void SetPropertyEdition( const QStringList& propertiesAvailable ); // Tested

    void SetPropertiesForDisplay( const QStringList& propertiesForDisplay ); // Tested

    void ResetCovariateEdition(); // Tested

    void SetCovariateEdition( const QMap< int, QString >& allCovariatesUsed ); // Tested

    void SetCovariatesForDisplay( const QMap< int, QString >& covariatesForDisplay ); // Tested


    void SetPlotOptions( bool isPlotSelected, bool propertySelectionAvailable, bool covariateSelectionAvailable ); /// Not tested

    void AddLinesForDisplay(); // Tested

    void SetCheckStateLinesToDisplay( Qt::CheckState checkState ); // Tested

    void SetSelectionLinesDisplayedVisible( bool visible ); /// Not tested

    void EditCovariatesNames(); // Tested


    void UpdatePlotAxis(); // Not Directly Tested

    void UpdatePlotTitle(); // Not Directly Tested

    void UpdatePlot(); // Not Directly Tested


    void ResetPlotTab(); // Tested

    void SetPlotTab(); // Tested

    void LoadPlotSettings( QString filePath ); // Tested

    void SavePlotSettings( QString filePath ); // Tested
};

#endif // FADTTSWINDOW_H
