#include "TestMatlabScript.h"

/*
 * argv[1] = adMatlabFilePath
 * argv[2] = subMatrixMatlabFilePath
 * qrgv[3] = outputdir
 * argv[4] = expectedMatlabScript
 */

int main( int argc, char *argv[] )
{
    Q_INIT_RESOURCE( FADTTS_Resources );

    TestMatlabScript testMatlabScript;

    bool test = testMatlabScript.Test_GenerateMatlabScript( argv[1], argv[2], argv[3], argv[4] );

    std::cout << std::endl;
    if( test )
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
