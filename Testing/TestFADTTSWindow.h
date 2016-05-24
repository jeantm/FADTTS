#ifndef TESTFADTTSWINDOW_H
#define TESTFADTTSWINDOW_H

#include "FADTTSWindow.h"

#include <QCryptographicHash>
#include <QSet>

#include <iostream>


class TestFADTTSWindow
{
public:
    TestFADTTSWindow();


    /**********************************************************************/
    /*************************** Test Functions ***************************/
    /**********************************************************************/

    /********** Configuration & Events **********/
    bool Test_UpdateCurrentDir();

    bool Test_SetDir( QString paraFilePath, QString dataDir, QString tempoDir );

    bool Test_DisplayIcon( QString dataDir );



    bool Test_OnLoadSaveParaConfiguration( QString paraFilePath, QString noParaFilePath, QString tempoDir );

    bool Test_OnLoadSaveSoftConfiguration( QString softFilePath, QString noSoftFilePath, QString tempoDir );


    /************** Subjects  Tab ***************/
    bool Test_OnQCThresholdApplied();

    bool Test_SetSelectedInputFiles();

    bool Test_UpdateAvailableDiffusionProperties();

    bool Test_GetDiffusionPropertiesCheckState();

    bool Test_DisplaySubjectInformation();

    bool Test_DisplayNbrSubjectSelected();

    bool Test_DisplaySortedSubjects();

    bool Test_SetCheckStateAllVisibleSubjects();

    bool Test_SearchSubjects();

    bool Test_SaveCheckedSubjects( QString dataDir, QString tempoDir );

    bool Test_GetCheckedMatchedSubjects();

    bool Test_GetPropertyRawData();



    bool Test_OnInputToggled();

    bool Test_OnSetCaseSensitivityToggled();

    bool Test_OnSubjectClicked();

    bool Test_OnSettingSubjectList( QString subjectListPath );



    /**************** Input  Tab ****************/
    bool Test_SetSelectedCovariates();

    bool Test_SetCheckStateAllCovariates();

    bool Test_GetDisplayInputFileInformation();

    bool Test_DisplayInputLineEditIcon( QString dataDir );

    bool Test_UpdateInputFileInformation( QString rdFilePath, QString faFilePath, QString subMatrixFilePath );

    bool Test_UpdateLineEditsAfterAddingMultipleFiles( QString dataDir );

    bool Test_SetInfoSubjectColumnID( QString subMatrixFilePath );



    bool Test_OnSettingInputFile( QString rdFilePath, QString subMatrixFilePath );

    bool Test_OnUpdatingInputFile();

    bool Test_OnUpdatingSubjectColumnID( QString subMatrixRawDataPath );

    bool Test_OnCovariateClicked();



    /************** Plotting  Tab **************/
    bool Test_SetProperties();

    bool Test_SetPropertiesForDisplay();

    bool Test_SetCovariates();

    bool Test_SetCovariatesForDisplay();

    bool Test_AddLinesForDisplay();

    bool Test_SetCheckStateLinesToDisplay();

    bool Test_EditCovariatesNames();

    bool Test_SetResetPlotTab( QString dataDir, QString tempoDir );

    bool Test_LoadSavePlotSettings( QString dataDir, QString tempoDir );



    bool Test_OnSettingPlotsUsed();

    bool Test_OnSettingAllPropertiesUsed();

    bool Test_OnSettingAllCovariatesUsed();

    bool Test_OnUpdatingCovariatesAvailable();

    bool Test_OnUpdatingPropertyPlotColor();

    bool Test_OnUpdatingCovariatePlotColor();

    bool Test_OnPropertySelection();

    bool Test_OnCovariateSelection();

    bool Test_OnLineForDisplayClicked();

    bool Test_OnSettingLinesSelected();

    bool Test_OnYChanged();



    /************** Execution Tab **************/
    bool Test_GenerateSelectedSubjectFile( QString subjectListPath , QString tempoDir );

    bool Test_GenerateFailedQCThresholdSubjectFile( QString dataDir, QString tempoDir );

//    bool Test_SetLogDisplay(); /** /!\ WRITE TEST /!\ **/



//    bool Test_OnRun(); /** /!\ WRITE TEST /!\ **/

//    bool Test_OnStop(); /** /!\ WRITE TEST /!\ **/

//    bool Test_OnDisplayLog(); /** /!\ WRITE TEST /!\ **/

//    bool Test_OnClearLog(); /** /!\ WRITE TEST /!\ **/

//    bool Test_OnMatlabThreadFinished(); /** /!\ WRITE TEST /!\ **/



private:
    /**********************************************************************/
    /********************** Functions Used For Testing ********************/
    /**********************************************************************/
    void AddIcon( QSharedPointer< FADTTSWindow > fadttsWindow, QString dataDir );


    void DisplayError_int( int intExpected, int intDisplayed );

    void DisplayError_QString( QString qStringExpected, QString qStringDisplayed , QString type );

    void DisplayError_QStringList( QStringList qStringListExpected, QStringList qStringListDisplayed, QString type );

    void DisplayError_QMapIntQString( QMap< int, QString > qMapExpected, QMap< int, QString > qMapDisplayed, QString type );

    void DisplayError_matchedSubjects( bool checkState, bool backgroundColor, bool flags );

    void DisplayError_unmatchedSubjects( bool text, bool backgroundColor, bool flags );


    QByteArray GetHashFile( QString filePath );

    bool CompareFile( QString filePath1, QString filePath2 );

};

#endif // TESTFADTTSWINDOW_H
