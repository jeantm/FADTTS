#include "TestFADTTSWindow.h"

#include <QApplication>

/*
 * argv[1] = testParaSettings
 * argv[2] = testParaNoSettings
 * argv[3] = testSoftSettings
 * argv[4] = testParaNoSettings
 *
 * argv[5] = rdRawDataPath
 * argv[6] = faRawDataPath
 * argv[7] = subMatrixRawDataPath
 * argv[8] = subjectListPath
 *
 * argv[9] = mdRawDataPath1
 * argv[10] = rdRawDataPath1
 * argv[11] = subMatrixRawDataPath1
 * argv[12] = mdBetaPath1
 * argv[13] = rdBetaPath1
 * argv[14] = mdConfidenceBandsPath1
 * argv[15] = rdConfidenceBandsPath1
 * argv[16] = mdPostHocFDRLpvaluesPath1
 * argv[17] = rdPostHocFDRLpvaluesPath1
 *
 * argv[18] = rdRawDataPath2
 * argv[19] = faRawDataPath2
 * argv[20] = subMatrixRawDataPath2
 * argv[21] = adBetaPath2
 * argv[22] = faBetaPath2
 * argv[23] = omnibusFDRLpvaluesPath2
 *
 * argv[24] = plotSettingsPath;
 * argv[25] = failedQCThreshold;
 *
 * argv[26] = okIcon
 * argv[27] = koIcon
 * argv[28] = warningIcon
 *
 * argv[29] = dataDir
 * argv[30] = tempoDir
 */

int main( int argc, char *argv[] )
{
    QApplication *app = new QApplication( argc, argv );

    TestFADTTSWindow testFADTTSWindow;
    int nbrTests = 0;
    int nbrTestsPassed = 0;


    /********** Configuration & Events **********/
    std::cerr << std::endl << std::endl << "/********** Configuration & Events **********/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_UpdateCurrentDir() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_SetDir( argv[1], argv[29], argv[30] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_DisplayIcon( argv[29] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;



    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnLoadSaveParaSettings( argv[1], argv[2], argv[29], argv[30] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnLoadSaveSoftSettings( argv[3], argv[4], argv[29], argv[30] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;



    std::cerr << std::endl;
    /********* Subjects/Covariates Tab *********/
    std::cerr << std::endl << "/********* Subjects/Covariates Tab *********/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnApplyingQCThreshold() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_SetSelectedInputFiles() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_UpdateAvailableDiffusionProperties() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_GetDiffusionPropertiesCheckState() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_DisplaySubjectInformation() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_DisplayNbrSubjectSelected() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_DisplaySortedSubjects() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_SetCheckStateAllVisibleSubjects() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_SearchSubjects() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_SaveCheckedSubjects( argv[29], argv[30] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_GetCheckedMatchedSubjects() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_GetPropertyRawData() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;



    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnInputToggled() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnSetCaseSensitivityToggled() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnSubjectClicked() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnSettingSubjectList( argv[8] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;



    std::cerr << std::endl;
    /**************** Input Tab ****************/
    std::cerr << std::endl << "/**************** Input Tab ****************/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_SetSelectedCovariates() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_SetCheckStateAllCovariates() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_GetDisplayInputFileInformation() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_DisplayInputLineEditIcon( argv[29] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_UpdateInputFileInformation( argv[5], argv[6], argv[7] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_UpdateLineEditsAfterAddingMultipleFiles( argv[29] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_SetInfoSubjectColumnID( argv[7] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;



    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnSettingInputFile( argv[5], argv[7] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnUpdatingInputFile() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnUpdatingSubjectColumnID( argv[7] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnCovariateClicked() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;



    std::cerr << std::endl;
    /************** Plotting  Tab **************/
    std::cerr << std::endl << "/************** Plotting  Tab **************/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_SetProperties() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_SetPropertiesForDisplay() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_SetCovariates() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_SetCovariatesForDisplay() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_AddLinesForDisplay() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_SetCheckStateLinesToDisplay() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_EditCovariatesNames() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_SetResetPlotTab( argv[29], argv[30] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_LoadSavePlotSettings( argv[29], argv[30] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;



    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnSettingPlotsUsed() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnSettingAllPropertiesUsed() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnUpdatingCovariatesAvailable() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnUpdatingPropertyPlotColor() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnUpdatingCovariatePlotColor() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnPropertySelection() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnCovariateSelection() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnLineForDisplayClicked() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnSettingLinesSelected() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_OnYChanged() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;



    std::cerr << std::endl;
    /************** Execution Tab **************/
    std::cerr << std::endl << "/************** Execution Tab **************/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_GenerateSelectedSubjectFile( argv[8], argv[30] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testFADTTSWindow.Test_GenerateFailedQCThresholdSubjectFile( argv[29], argv[30] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


//    std::cerr << std::endl << nbrTests + 1 << "- ";
//    if( testFADTTSWindow.Test_SetLogDisplay() )
//    {
//        nbrTestsPassed++;
//    }
//    nbrTests++;



//    std::cerr << std::endl << nbrTests + 1 << "- ";
//    if( testFADTTSWindow.Test_OnRun() )
//    {
//        nbrTestsPassed++;
//    }
//    nbrTests++;

//    std::cerr << std::endl << nbrTests + 1 << "- ";
//    if( testFADTTSWindow.Test_OnStop() )
//    {
//        nbrTestsPassed++;
//    }
//    nbrTests++;

//    std::cerr << std::endl << nbrTests + 1 << "- ";
//    if( testFADTTSWindow.Test_OnDisplayLog() )
//    {
//        nbrTestsPassed++;
//    }
//    nbrTests++;

//    std::cerr << std::endl << nbrTests + 1 << "- ";
//    if( testFADTTSWindow.Test_OnClearLog() )
//    {
//        nbrTestsPassed++;
//    }
//    nbrTests++;

//    std::cerr << std::endl << nbrTests + 1 << "- ";
//    if( testFADTTSWindow.Test_OnMatlabThreadFinished() )
//    {
//        nbrTestsPassed++;
//    }
//    nbrTests++;




    app->exit();
    std::cerr << std::endl << std::endl << std::endl;
    std::cerr << "                   Tests Summary                " << std::endl;
    std::cerr << "*************************************************" << std::endl;
    std::cerr << "* " << 100*nbrTestsPassed/nbrTests << "% tests passed, " <<
                 ( nbrTests - nbrTestsPassed ) << " test(s) failed out of " <<
                 nbrTests << " *" << std::endl;
    std::cerr << "*************************************************";
    std::cerr << std::endl << std::endl << std::endl << std::endl;

    if( nbrTestsPassed == nbrTests )
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
