#include "TestInfoDialog.h"

int main( int argc, char *argv[] )
{
    TestInfoDialog testInfoDialog;

    bool test = testInfoDialog.Test_DisplayInfo( argv[1], argv[2] );

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
