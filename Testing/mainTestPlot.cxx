#include "TestPlot.h"

#include <QApplication>

/*
 * argv[1] = rdRawDataToSortPath
 * argv[2] = faRawDataToSortPath
 * argv[3] = subMatrixRawDataToSortPath
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
 * argv[15] = transposedRDRawDataPath
 * argv[16] = transposedFARawDataPath
 * argv[17] = transposedOmnibusLpvaluesPath
 * argv[18] = transposedRDPostHocFDRPath
 * argv[20] = transposedFAPostHocFDRPath
 * argv[20] = dataDir
 */

int main( int argc, char *argv[] )
{
    QApplication *app = new QApplication( argc, argv );

    TestPlot testPlot;
    int nbrTests = 0;
    int nbrTestsPassed = 0;

    std::cerr << std::endl << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetQVTKWidget() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_QStringListToDouble() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_DataToDouble() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SortFilesByProperties( argv[1], argv[2], argv[3] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_TransposeData() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_TransposeDataInQMap() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;
    

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetRawData( argv[4], argv[5], argv[6], argv[15], argv[16], argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetBeta( argv[7], argv[8], argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetOmnibusLpvalue( argv[9], argv[17], argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetConfidenceBands( argv[11], argv[12], argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetPostHocFDRLpvalue( argv[13], argv[14], argv[18], argv[19], argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_GetRawDataFiles( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_GetBetaFiles( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_GetOmnibusLpvalueFiles( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_GetOmnibusFDRLpvalueFiles( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_GetConfidenceBandsFiles( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_GetPostHocFDRLpvalueFiles( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetPlots( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetProperties( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetSubjects( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetCovariates( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetAbscissa( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetSelectedPlot( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SetTitle() )
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
    if( testPlot.Test_SetAxis() )
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
    if( testPlot.Test_UpdateCovariatesNames( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_SeparateBinary( argv[2], argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

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
    if( testPlot.Test_UpdateOrdinate( argv[9] ) )
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
    if( testPlot.Test_LoadRawData( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_LoadRawStats( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_LoadBetas( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_LoadBetaByCovariate( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_LoadOmnibusLpvalues( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_LoadConfidenceBand( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_LoadPostHocFDRLpvalues( argv[20] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


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
    if( testPlot.Test_AddEntriesCovariatesBands() )
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

//    std::cerr << std::endl << nbrTests + 1 << "- ";
//    if( testPlot.Test_AddLineSigBetas() )
//    {
//        nbrTestsPassed++;
//    }
//    nbrTests++;


    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testPlot.Test_GetyMinMax() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

//    std::cerr << std::endl << nbrTests + 1 << "- ";
//    if( testPlot.Test_SetyMinMax() )
//    {
//        nbrTestsPassed++;
//    }
//    nbrTests++;











    app->exit();
    std::cout << std::endl << std::endl << std::endl;
    std::cout << "                   Tests Summary                " << std::endl;
    std::cout << "************************************************" << std::endl;
    std::cout << "* " << 100*nbrTestsPassed/nbrTests << "% tests passed, " << ( nbrTests - nbrTestsPassed ) << " test(s) failed out of " << nbrTests << " *" << std::endl;
    std::cout << "************************************************";
    std::cout << std::endl << std::endl << std::endl << std::endl;

    if( nbrTestsPassed == nbrTests )
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
