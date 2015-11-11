#include <QApplication>

#include "TestProcessing.h"

int main( int argc, char *argv[] )
{
    QApplication *app = new QApplication(argc, argv);

    TestProcessing testProcessing;

    bool test1 = testProcessing.Test_IsMatrixDimensionOK( argv[2] );
    bool test2 = testProcessing.Test_IsCovariateFile();
    bool test3 = testProcessing.Test_GetSelectedSubjects( argv[7] );
    bool test4 = testProcessing.Test_GenerateMatlabInputFiles( argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7] );
    bool test5 = testProcessing.Test_GetSubjectsFromInputFile( argv[2], argv[3], argv[4] );
    bool test6 = testProcessing.Test_GetRefSubjectsFromSelectedInputFiles( argv[2], argv[3], argv[4] );
    bool test7 = testProcessing.Test_GetRefSubjects( argv[2], argv[4], argv[7] );
    bool test8 = testProcessing.Test_GetAllSubjectsFromSelectedInputFiles();
    bool test9 = testProcessing.Test_SortSubjects();
    bool test10 = testProcessing.Test_AssignSortedSubject();
    bool test11 = testProcessing.Test_GetCovariatesFromFileData( argv[3], argv[4] );

    std::cout << std::endl;
    app->exit();
    if( test1 && test2 && test3 && test4 && test5 && test6 &&
            test7 && test8 && test9 && test10 && test11 )
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
