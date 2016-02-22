#include "TestFADTTSWindow.h"

#include <QApplication>


int main( int argc, char *argv[] )
{
    QApplication *app = new QApplication( argc, argv );

    TestFADTTSWindow testFADTTSWindow;

    int nbrTests = 0;
    int nbrTestsPassed = 0;

//    std::cerr << std::endl << std::endl << nbrTests + 1 << "- ";
//    if( testFADTTSWindow.Test_SetCurrentDir() )
//    {
//        nbrTestsPassed++;
//    }
//    nbrTests++;
//    std::cerr << std::endl << nbrTests + 1 << "- ";
//    if( testFADTTSWindow.Test_CovariateColumnID() )
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
