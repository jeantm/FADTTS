#include "TestProcessing.h"

/*
 * argv[1] = testGetDataFromFile_N
 * argv[2] = testGetDataFromFile_R
 * argv[3] = testGetDataFromFile_RN
 *
 * argv[4] = adFilePath
 * argv[5] = subMatrix0FilePath
 * argv[6] = subMatrix3FilePath
 *
 * argv[7] = adMatlabFilePath
 * argv[8] = subMatrixMatlabFilePath
 * argv[9] = subjectsListFilePath
 *
 * qrgv[10] = tempDir
 */

int main( int argc, char *argv[] )
{
    TestProcessing testProcessing;
    int nbrTests = 0;
    int nbrTestsPassed = 0;


    /********** File & Data Processing *********/
    std::cerr << std::endl << std::endl << std::endl << "/********** File & Data Processing *********/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testProcessing.Test_GetDataFromFile( argv[1], argv[2], argv[3]) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testProcessing.Test_IsMatrixDimensionOK( argv[4] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testProcessing.Test_IsSubMatrix() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testProcessing.Test_GetSubjectsFromFileList( argv[9] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testProcessing.Test_GetSubjectsFromData( argv[4], argv[5], argv[6] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testProcessing.Test_GetCovariatesFromFileData( argv[5], argv[6] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl;
    /************* Plot Processing *************/
    std::cerr << std::endl << "/************* Plot Processing *************/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testProcessing.Test_Transpose() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testProcessing.Test_RemoveUnmatchedSubjects() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testProcessing.Test_QStringListToDouble() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testProcessing.Test_DataToDouble() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testProcessing.Test_GetMean() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testProcessing.Test_ApplyPearsonCorrelation() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl;
    /*** Functions used for Subjects Display ***/
    std::cerr << std::endl << "/*** Functions used for Subjects Display ***/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testProcessing.Test_GetAllSubjects( argv[4], argv[5] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testProcessing.Test_GetSubjectsFromSelectedFiles() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testProcessing.Test_SortSubjects() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;

    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testProcessing.Test_AssignSortedSubject() )
    {
        nbrTestsPassed++;
    }
    nbrTests++;


    std::cerr << std::endl;
    /***************** Others ******************/
    std::cerr << std::endl << "/***************** Others ******************/";
    std::cerr << std::endl << nbrTests + 1 << "- ";
    if( testProcessing.Test_GenerateMatlabInputFiles( argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], argv[10] ) )
    {
        nbrTestsPassed++;
    }
    nbrTests++;




    std::cerr << std::endl << std::endl << std::endl << std::endl << std::endl;
    std::cerr << "                   Tests Summary                " << std::endl;
    std::cerr << "*************************************************" << std::endl;
    std::cerr << "* " << 100*nbrTestsPassed/nbrTests << "% tests passed, " <<
                 ( nbrTests - nbrTestsPassed ) << " test(s) failed out of " <<
                 nbrTests << " *" << std::endl;
    std::cerr << "*************************************************";
    std::cerr << std::endl << std::endl << std::endl;

    if( nbrTestsPassed == nbrTests )
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
