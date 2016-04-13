#include "TestQCThresholdDialog.h"

int main( int argc, char *argv[] )
{
    TestQCThresholdDialog testQCThresholdDialog;
    int nbrTests = 0;
    int nbrTestsPassed = 0;


    //std::cerr << std::endl << std::endl << std::endl << "/************** Initialization **************/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testData.Test_InitData() )
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
