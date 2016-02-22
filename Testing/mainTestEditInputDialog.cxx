#include "TestEditInputDialog.h"

#include <QApplication>

/*
 * argv[1] = adFilePath
 * argv[2] = subMatrix0FilePath
 * argv[3] = adFilePathMofied
 * argv[4] = tempDir
 */

int main( int argc, char *argv[] )
{
    QApplication *app = new QApplication( argc, argv );

    TestEditInputDialog testEditInputDialog;
    int nbrTests = 0;
    int nbrTestsPassed = 0;

    std::cerr << std::endl << std::endl << nbrTests + 1 << "- ";
    if( testEditInputDialog.Test_SetCurrentDir() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testEditInputDialog.Test_CovariateColumnID() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testEditInputDialog.Test_OnDelete( argv[1] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testEditInputDialog.Test_LoadDisplayData( argv[1], argv[2] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testEditInputDialog.Test_OnSaveFile( argv[4], argv[1], argv[3] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


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
