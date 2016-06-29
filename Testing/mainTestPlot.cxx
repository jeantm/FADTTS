#include "TestPlot.h"

#include <QApplication>

/*
 * argv[1] = rdRawDataToSortPath
 * argv[2] = faRawDataToSortPath
 * argv[3] = subMatrixRawDataToSortPath
 *
 * argv[4] = rdRawDataPath
 * argv[5] = faRawDataPath
 * argv[6] = subMatrixRawDataPath
 * argv[7] = rdBetaPath
 * argv[8] = faBetaPath
 * argv[9] = omnibusLpvaluesPath
 * argv[10] = omnibusFDRLpvaluesPath
 * argv[11] = rdConfidenceBandsPath
 * argv[12] = faConfidenceBandsPath
 * argv[13] = rdPostHocFDRLpvaluesPath
 * argv[14] = faPostHocFDRLpvaluesPath
 *
 * argv[15] = transposedRDRawDataPath
 * argv[16] = transposedFARawDataPath
 * argv[17] = transposedOmnibusLpvaluesPath
 * argv[18] = transposedRDPostHocFDRPath
 * argv[19] = transposedFAPostHocFDRPath
 *
 * argv[20] = plotPath
 *
 * argv[21] = dataDir
 * argv[22] = tempoDir
 */

int main( int argc, char *argv[] )
{
    QApplication *app = new QApplication( argc, argv );

    TestPlot testPlot;
    int nbrTests = 0;
    int nbrTestsPassed = 0;


    /************** Initialization *************/
    std::cerr << std::endl << std::endl << std::endl << "/************** Initialization *************/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetQVTKWidget() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl;
    /***************** Set Data ****************/
    std::cerr << std::endl << "/***************** Set Data ****************/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SortFilesByProperties( argv[1], argv[3] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetRawData( argv[4], argv[5], argv[6], argv[15], argv[16], argv[21] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetRawDataQCThreshold( argv[5] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetBetaByPropertiesCovariates( argv[7], argv[8], argv[21] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetOmnibusLpvalue( argv[9], argv[17], argv[21] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetConfidenceBands( argv[11], argv[12], argv[21] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetPostHocFDRLpvalue( argv[13], argv[14], argv[18], argv[19], argv[21] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl;
    /***************** Get Data ****************/
    std::cerr << std::endl << "/***************** Get Data ****************/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_GetRawDataFiles( argv[21], argv[22] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_GetBetaFiles( argv[21], argv[22] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_GetOmnibusLpvalueFiles( argv[21], argv[22] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_GetOmnibusFDRLpvalueFiles( argv[21], argv[22] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_GetConfidenceBandsFiles( argv[21], argv[22] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_GetPostHocFDRLpvalueFiles( argv[21], argv[22] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl;
    /***************** Settings ****************/
    std::cerr << std::endl << "/***************** Settings ****************/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetPlots( argv[21], argv[22] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetProperties( argv[21], argv[22] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetSubjects( argv[21], argv[22] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetCovariates( argv[21], argv[22] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetSelectedPlot( argv[21] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetCustomizedTitle() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetDefaultTitle() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetCustomizedAxis() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetDefaultAxis() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetLegend() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetMarkerType() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_UpdateCovariatesNames( argv[21], argv[22] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl;
    /************** Loading Data ***************/
    std::cerr << std::endl << "/************** Loading Data ***************/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_GetMeanAndStdDv() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_ProcessRawStats() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetSelectionToDisplayProperties() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_Tolog10() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_LoadRawData( argv[21], argv[22] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_LoadRawStats( argv[21] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_LoadBetas( argv[21], argv[22] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_LoadBetaByCovariate( argv[21] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_LoadOmnibusLpvalues( argv[21] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_LoadConfidenceBand( argv[21], argv[22] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_LoadPostHocFDRLpvalues( argv[21] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl;
    /************** Adding Entries *************/
    std::cerr << std::endl << "/************** Adding Entries *************/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_AddEntriesRawData() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_AddEntriesRawStats() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_AddEntriesByPropertiesOrCovariates() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_AddEntriesConfidenceBands() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetData() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_InitLines() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_AddSignificantLevel() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl;
    /**************** Set  Axis ****************/
    std::cerr << std::endl << "/**************** Set  Axis ****************/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_GetyMinMax() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl;
    /**************** Save Plot ****************/
    std::cerr << std::endl << "/**************** Save Plot ****************/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SavePlot( argv[20], argv[21], argv[22] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;




    app->exit();
    std::cerr << std::endl << std::endl << std::endl << std::endl << std::endl;
    std::cerr << "                   Tests Summary                " << std::endl;
    std::cerr << "************************************************" << std::endl;
    std::cerr << "* " << 100*nbrTestsPassed/nbrTests << "% tests passed, " <<
                 ( nbrTests - nbrTestsPassed ) << " test(s) failed out of " <<
                 nbrTests << " *" << std::endl;
    std::cerr << "************************************************";
    std::cerr << std::endl << std::endl << std::endl;

    if( nbrTestsPassed == nbrTests )
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
