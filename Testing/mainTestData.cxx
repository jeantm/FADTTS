#include "TestData.h"

int main( int argc, char *argv[] )
{
    TestData testData;
    int nbrTests = 0;
    int nbrTestsPassed = 0;

    std::cerr << std::endl << std::endl << nbrTests + 1 << "- ";
    if( testData.Test_InitData() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testData.Test_GetDiffusionPropertyIndices() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testData.Test_GetDiffusionPropertyName() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testData.Test_GetSetFilename() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testData.Test_GetSetFileData() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testData.Test_GetSetNbrRows() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testData.Test_GetSetNbrColumns() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testData.Test_GetSetSubjects() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testData.Test_GetSetNbrSubjects() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testData.Test_GetSetCovariates() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testData.Test_GetSetCovariateColumnID() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testData.Test_GetSetOutputDir() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testData.Test_ClearFileInformation() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testData.Test_AddInterceptToCovariates() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


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
