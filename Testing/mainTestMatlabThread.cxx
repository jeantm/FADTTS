#include "TestMatlabThread.h"

/*
 * argv[1] = refmatlabScriptWithPlot
 *
 * argv[2] = myFDR
 *
 * argv[3] = tempDir
 */

int main( int argc, char *argv[] )
{
    Q_INIT_RESOURCE( FADTTS_Resources );

    TestMatlabThread testMatlabThread;
    int nbrTests = 0;
    int nbrTestsPassed = 0;

    /************** Initialization *************/
    std::cerr << std::endl << std::endl << std::endl << "/************** Initialization *************/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testMatlabThread.Test_InitMatlabScript( argv[1] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl;
    /************ Set Matlab Script ************/
    std::cerr << std::endl << "/************ Set Matlab Script ************/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testMatlabThread.Test_SetHeader() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testMatlabThread.Test_SetMVCMPath() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testMatlabThread.Test_SetFiberName() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testMatlabThread.Test_SetDiffusionProperties() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testMatlabThread.Test_SetInputFiles() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testMatlabThread.Test_SetCovariates() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testMatlabThread.Test_SetNbrPermutation() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testMatlabThread.Test_SetOmnibus() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testMatlabThread.Test_SetPostHoc() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testMatlabThread.Test_SetConfidenceBandsThreshold() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testMatlabThread.Test_SetPvalueThreshold() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl;
    /************* File Generation *************/
    std::cerr << std::endl << "/************* File Generation *************/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testMatlabThread.Test_GenerateMFiles( argv[2], argv[3] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;




    std::cerr << std::endl << std::endl << std::endl << std::endl << std::endl;
    std::cerr << "                   Tests Summary                " << std::endl;
    std::cerr << "*************************************************" << std::endl;
    std::cerr << "* " << 100*nbrTestsPassed/nbrTests << "% tests passed, " <<
                 ( nbrTests - nbrTestsPassed ) << " test(s) failed out of " <<
                 nbrTests << " *" << std::endl;
    std::cerr << "*************************************************";
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
