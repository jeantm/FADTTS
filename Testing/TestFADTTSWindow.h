#ifndef TESTFADTTSWINDOW_H
#define TESTFADTTSWINDOW_H

#include <iostream>

#include "FADTTSWindow.h"

class TestFADTTSWindow
{
public:
    TestFADTTSWindow();


    /**********************************************************************/
    /*************************** Test Functions ***************************/
    /**********************************************************************/
    bool Test_UpdateCurrentDir(); // Done
    bool Test_UpdateCurrentDirEditInputDialog(); // Done

    bool Test_DisplayCovariatesList(); // Done
    bool Test_UpdateAvailableFileParamTab( QString adInputFile );
    bool Test_SetInfoSubjectColumnID();
    bool Test_IsMatrixDimensionOK();
    bool Test_UpdateFileInformation();
    bool Test_DisplayNbrSubjectsSelected();
    bool Test_DisplaySortedSubjectList();
    bool Test_Search();
    bool Test_SortSubjects();
    bool Test_UpdateInputLineEdit();
    bool Test_AddFiles();

    bool Test_UpdateInputSubjectListLineEdit();

    bool Test_SelectSubject();

    bool Test_SelectCovariates();

    bool Test_UpdateOutputDirLineEdit();

    bool Test_GetSelectedPrefixes();
    bool Test_GetSelectedInputFiles();
    bool Test_GetSelectedCovariates();
    bool Test_GenerateSelectedSubjectList();

    bool Test_SyncUiToModelStructure();


private:
    /**********************************************************************/
    /********************** Functions Used For Testing ********************/
    /**********************************************************************/
    void DisplayErrorNbrCovariates( int nbrCovariates, int nbrCovariatesExpected );
    void DisplayErrorTextCovariates( QStringList covariatesDisplayed, QMap<int, QString> covariateMap );
    void DisplayErrorCheckStateCovariates( QStringList covariatesDisplayed, QList<Qt::CheckState> covariatesCheckState );

    void DisplayErrorTextAvailableFile( QString expectedFileLabel, QString fileLabelDisplayed );
    void DisplayErrorCheckStateAvailableFile( bool checkStateExpected, bool checkStateDisplayed );
};

#endif // TESTFADTTSWINDOW_H
