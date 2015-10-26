#include "TestMatlabScript.h"

int main( int argc, char *argv[] )
{
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
