#ifndef FADTTSWINDOW_H
#define FADTTSWINDOW_H

#include "FADTTSWindowConfig.h"
#include "EditInputDialog.h"
#include "QCThresholdDialog.h"
#include "Data.h"
#include "Processing.h"
#include "MatlabThread.h"
#include "Plot.h"

#include <QSignalMapper>
#include <QFileSystemWatcher>
#include <QScrollBar>
#include <QProgressBar>


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
    void OnSettingInputFile( int diffusionPropertyID ); // Tested

    void OnAddInputFiles(); // Not Directly Tested

    void OnAddInputFile( int diffusionPropertyID ); // Not Directly Tested

    void OnEditInputFile( int diffusionPropertyID ); /// Not tested


    void OnUpdatingInputFile( const int &diffusionPropertyIndex, const QString& newFilePath ); // Tested

    void OnUpdatingSubjectColumnID( int newSubjectColumnID ); // Tested


    /********* Subjects/Covariates Tab *********/
    void OnCovariateFileToggled(); // Tested

    void OnCovariateClicked( QListWidgetItem *item ); // Tested

    void OnCheckAllCovariates(); // Tested

    void OnUnCheckAllCovariates(); // Tested


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
    void OnSettingFiberName( const QString& fibername ); // Not Directly Tested

    void OnBrowsingOutputDir(); // Not Directly Tested

    void OnSettingOutputDir( const QString& path ); // Not Directly Tested

    void OnBrowsingMVCMPath(); // Not Directly Tested

    void OnSettingMVCMPath( const QString& path ); // Not Directly Tested

    void OnBrowsingMatlabExe(); // Not Directly Tested

    void OnSettingMatlabExe( const QString& executable ); // Not Directly Tested


    void OnRunMatlabToggled( bool choice ); // Not Directly Tested


    void OnRun(); /// Not tested

    void OnStop(); /// Not tested

    void OnDisplayLog(); /// Not tested

    void OnClearLog(); /// Not tested


    void OnMatlabThreadFinished(); /// Not tested


    /************** Plotting  Tab **************/
    void OnBrowsingPlotDir();

    void OnSettingPlotDir( const QString& path );

    void OnSettingPlotFibername( const QString& fibername );


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


    void OnSettingLinesSelected( const QStringList& linesSelected ); /*** WRITE TEST ***/


    void OnUseCustomizedTitle( bool checkState ); /*** WRITE TEST ***/

    void OnUpdatingPlotTitle(); /*** WRITE TEST ***/


    void OnUpdatingPlotGrid( bool checkState ); /*** WRITE TEST ***/

    void OnUseCustomizedAxis( bool checkState ); /*** WRITE TEST ***/

    void OnYMinToggled( bool checkState ); /** /!\ PB WITH TEST /!\ **/

    void OnYMaxToggled( bool checkState ); /** /!\ PB WITH TEST /!\ **/

    void OnYMinValueChanged( double yMinValue ); /*** WRITE TEST ***/

    void OnYMaxValueChanged( double yMaxValue ); /*** WRITE TEST ***/

    void OnUpdatingPlotAxis(); /*** WRITE TEST ***/


    void OnUpdatingLegend( const QString& legendPosition ); /*** WRITE TEST ***/


    void OnUpdatingPvalueThreshold( double pvalueThreshold ); /*** WRITE TEST ***/

    void OnUpdatingLineWidth( double lineWidth ); /*** WRITE TEST ***/

    void OnUpdatingSelectedColorLine( const QString& color ); /*** WRITE TEST ***/

    void OnUpdatingMarkerType( const QString& markerType ); /*** WRITE TEST ***/

    void OnUpdatingMarkerSize( double markerSize ); /*** WRITE TEST ***/


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

    pushButtonMapType m_inputTabAddInputFilePushButtonMap, m_inputTabEditInputFilePushButtonMap;
    lineEditMapType m_inputTabInputFileLineEditMap;
    labelMapType m_inputTabIconLabelMap, m_inputFileInformationLabelMap;

    QString m_currentInputFileDir;


    /********* Subjects/Covariates Tab *********/
    Qt::CaseSensitivity m_caseSensitivity;

    QListWidget *m_covariateListWidget, *m_matchedSubjectListWidget, *m_unmatchedSubjectListWidget;

    checkBoxMapType m_paramTabFileCheckBoxMap;
    labelMapType m_paramTabFileDataSizeLabelMap;

    QMap< int, QString > m_selectedCovariates;

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
    QSharedPointer< QVTKWidget > m_qvtkWidget;

    Plot *m_plot;

    QListWidget *m_lineDisplayedListWidget, *m_lineSelectedListWidget;

    displayMapType m_propertiesForDisplay, m_covariatesForDisplay, m_currentLinesForDisplay;

    nameLabelMapType m_propertiesNameLabelMap, m_covariatesNameLabelMap;
    comboBoxMapType m_propertiesColorsComboBoxMap, m_covariatesColorsComboBoxMap;
    covariateNameLineEditMapType m_covariatesNameLineEditMap;

    QComboBox *m_plotComboBox, *m_propertyComboBox, *m_covariateComboBox;

    QString m_plotSelected;

    bool m_isPlotReady, m_areLinesForDisplayProperties;



    /********** Configuration & Events **********/
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

    void DisplaySortedSubjects( const QStringList& matchedSubjectList, const QMap< QString, QList< int > >& unMatchedSubjectMap ); // Tested

    void UpdateSubjectList(); // Tested

    void SetCheckStateAllVisibleSubjects( Qt::CheckState checkState ); // Tested

    int SearchSubjects( QListWidget *list ); // Tested

    void SaveCheckedSubjects( QString filePath ); // Tested


    /************** Execution Tab **************/
    QStringList GenerateSelectedSubjectFile( QString outputDir ); // Tested


    bool IsFADTTSReadyToBeRun(); /// Not tested

    void SetLogDisplay( QString outputDir, const QMap< int, QString >& matlabInputFiles,
                        const QMap< int, QString >& selectedCovariates ); /// Not tested

    void SetMatlabScript(); // Not Directly Tested


    /************** Plotting  Tab **************/
    void SetColorsComboBox( QComboBox* &comboBox ); // Tested

    void ResetPropertyEdition(); // Tested

    void SetPropertyEdition( const QStringList& propertiesAvailable ); // Tested

    void SetPropertiesForDisplay( const QStringList& propertiesForDisplay ); // Tested

    void ResetCovariateEdition(); // Tested

    void SetCovariateEdition( const QMap< int, QString >& allCovariatesUsed ); // Tested

    void SetCovariatesForDisplay( const QMap< int, QString >& covariatesForDisplay ); // Tested


    void SetPlotOptions( bool isPlotSelected, bool propertySelectionAvailable, bool covariateSelectionAvailable ); /** /!\ PB WITH TEST /!\ **/

    void AddLinesForDisplay( bool isSelectionProperties ); // Tested

    void SetCheckStateLinesToDisplay( Qt::CheckState checkState ); // Tested

    void SetSelectionLinesDisplayedVisible( bool visible ); /*** WRITE TEST ***/

    void EditCovariatesNames(); // Tested


    void UpdatePlotAxis(); /*** WRITE TEST ***/

    void UpdatePlotTitle(); /*** WRITE TEST ***/

    void UpdatePlot(); /*** WRITE TEST ***/


    void ResetPlotTab(); // Tested

    void SetPlotTab(); // Tested

    void LoadPlotSettings( QString filePath ); // Tested

    void SavePlotSettings( QString filePath ); // Tested
};

#endif // FADTTSWINDOW_H
