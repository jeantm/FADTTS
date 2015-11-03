#include "TestInfoDialog.h"

int main( int argc, char *argv[] )
{
    QApplication *app = new QApplication( argc, argv );

    TestInfoDialog testInfoDialog;

    bool test = testInfoDialog.Test_DisplayFileInformation( argv[1], argv[2] );

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
