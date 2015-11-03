#include "TestFADTTSWindow.h"

int main( int argc, char *argv[] )
{
    QApplication *app = new QApplication( argc, argv );

    TestFADTTSWindow testFADTTSWindow;

    bool test1 = testFADTTSWindow.Test_UpdateCurrentDir();
    bool test2 = testFADTTSWindow.Test_UpdateEditInputDialogCurrentDir();

    bool test3 = testFADTTSWindow.Test_DisplayCovariates();
    bool test4 = testFADTTSWindow.Test_UpdateAvailableFileParamTab();
    bool test5 = testFADTTSWindow.Test_SetInfoCovariateFileSubjectColumnID( argv[2] );
    bool test6 = testFADTTSWindow.Test_UpdateInputFileInformation( argv[1], argv[2] );
    bool test7 = testFADTTSWindow.Test_DisplayNbrSubjectSelected();
    bool test8 = testFADTTSWindow.Test_DisplaySortedSubjects();
    bool test9 = true;
    bool test10 = true;
    bool test11 = true;
    bool test12 = true;
    bool test13 = true;
    bool test14 = true;
    bool test15 = true;
    bool test16 = true;
    bool test17 = true;
    bool test18 = true;
    bool test19 = true;
    bool test20 = true;
    bool test21 = true;
    bool test22 = true;

    app->exit();
    std::cout << std::endl;
    if( test1 && test2 && test3 && test4 && test5 && test6 && test7 && test8 && test9 && test10 &&
            test11 && test12 && test13 && test14 && test15 && test16 && test17 && test18 && test19 && test20 &&
            test21 && test22 )
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
