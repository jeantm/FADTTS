#ifndef TESTFADTTSWINDOW_H
#define TESTFADTTSWINDOW_H

#include "FADTTSWindow.h"

#include <iostream>


class TestFADTTSWindow
{
public:
    TestFADTTSWindow();


//    /**********************************************************************/
//    /*************************** Test Functions ***************************/
//    /**********************************************************************/
//    bool Test_UpdateCurrentDir(); // Done
//    bool Test_UpdateEditInputDialogCurrentDir(); // Done
//    bool Test_SetDir();

//    bool Test_DisplayCovariates(); // Done
//    bool Test_UpdateAvailableFileParamTab(); // Done
//    bool Test_SetInfoCovariateFileSubjectColumnID( QString inputCOMPFile ); // Done
//    bool Test_UpdateInputFileInformation( QString inputADFile, QString inputCOMPFile ); // Done
//    bool Test_DisplaySortedSubjects(); // Done
//    bool Test_DisplayNbrSubjectSelected(); // Done
//    bool Test_SearchSubjects();
//    bool Test_UpdateLineEditsAfterAddingMultipleFiles();

//    bool Test_SelectSubject();
//    bool Test_SelectCovariate();

//    bool Test_GetSelectedPrefixes();
//    bool Test_GetSelectedInputFiles();
//    bool Test_GetSelectedCovariates();
//    bool Test_GenerateSelectedSubjectFile();

//    bool Test_SyncUiToModelStructure();


//private:
//    /**********************************************************************/
//    /********************** Functions Used For Testing ********************/
//    /**********************************************************************/
//    void DisplayErrorNbrCovariates_DisplayCovariates( int nbrCovariates, int nbrCovariatesExpected );
//    void DisplayErrorNameCovariates_DisplayCovariates( QStringList covariatesDisplayed, QMap<int, QString> covariateMap );
//    void DisplayErrorCheckStateCovariates_DisplayCovariates( QStringList covariatesDisplayed, QList<Qt::CheckState> covariatesCheckState );

//    void DisplayErrorFileInfo_UpdateAvailableFileParamTab( QString expectedFileLabel, QString fileLabelDisplayed );
//    void DisplayErrorCheckStateAvailableFile_UpdateAvailableFileParamTab( bool checkStateExpected, bool checkStateDisplayed );

//    void DisplayErrorID_SetInfoCovariateFileSubjectColumnID( QString labelExpected, QString labelDisplayed );

//    void DisplayErrorNbrRows_UpdateInputFileInformation( int nbrRowsExpected, int nbrRowsLoaded );
//    void DisplayErrorNbrColumns_UpdateInputFileInformation( int nbrColumnsExpected, int nbrColumnsLoaded );
//    void DisplayErrorNbrSubjects_UpdateInputFileInformation( int nbrSubjectsExpected, int nbrSubjectsLoaded );

//    void DisplayErrorNbrSubjects_DisplayNbrSubjectSelected( int nbrSubjectsExpected, int nbrSubjectsDisplayed );
//    void DisplayErrorMatchedSubjects_DisplayNbrSubjectSelected( bool checkState, bool backgroundColor, bool flags );
//    void DisplayErrorUnMatchedSubjects_DisplayNbrSubjectSelected( bool text, bool backgroundColor, bool flags );
//    void DisplayErrorSortedSubjectsInformation_DisplayNbrSubjectSelected( QString expectedInformation, QString informationDisplayed );

//    void DisplayErrorCheckedSubjectsInformation_DisplayNbrSubjectSelected( QString checkedSubjectsInformationExpected, QString checkedSubjectsInformationDisplayed );


};

#endif // TESTFADTTSWINDOW_H
