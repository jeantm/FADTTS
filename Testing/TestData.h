#ifndef TESTDATA_H
#define TESTDATA_H

#include "Data.h"

#include <iostream>


class TestData
{
public:
    TestData();

    /**********************************************************************/
    /*************************** Test Functions ***************************/
    /**********************************************************************/
    bool Test_InitData();

    bool Test_GetDiffusionPropertyIndices();

    bool Test_GetDiffusionPropertyName();

    bool Test_GetSetFilename();

    bool Test_GetSetFileData();

    bool Test_GetSetNbrRows();

    bool Test_GetSetNbrColumns();

    bool Test_GetSetSubjects();

    bool Test_GetSetNbrSubjects();

    bool Test_GetSetCovariates();

    bool Test_GetSetSubjectColumnID();

    bool Test_GetSetOutputDir();

    bool Test_ClearFileInformation();

    bool Test_AddInterceptToCovariates();
};

#endif // TESTDATA_H
