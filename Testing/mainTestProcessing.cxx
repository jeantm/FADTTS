#include "TestProcessing.h"

int main( int argc, char *argv[] )
{
    TestProcessing testProcessing;

    bool test1 = testProcessing.Test_isCOMP();
    bool test2 = testProcessing.Test_GetSelectedSubjectList( argv[7] );
    bool test3 = testProcessing.Test_GenerateMatlabInputFiles( argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7] );
    bool test4 = testProcessing.Test_GetSubjectListFromInputFile( argv[2], argv[3], argv[4] );
    bool test5 = testProcessing.Test_GetRefSubjectListFromSelectedInputFiles( argv[2], argv[3], argv[4] );
    bool test6 = testProcessing.Test_GetRefSubjectList( argv[2], argv[4], argv[7] );
    bool test7 = testProcessing.Test_GetAllSubjectsFromSelectedInputFiles();
    bool test8 = testProcessing.Test_SortSubjectInInputFile();
    bool test9 = testProcessing.Test_AssignSortedSubject();

    std::cout << std::endl;
    if( test1 && test2 && test3 && test4 && test5 && test6 && test7 && test8 && test9 )
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
