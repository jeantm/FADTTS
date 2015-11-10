#ifndef TESTPROCESSING_H
#define TESTPROCESSING_H

#include <QCryptographicHash>
#include <QApplication>

#include <iostream>

#include "Processing.h"

class TestProcessing
{
public:
    TestProcessing();

    /**********************************************************************/
    /*************************** Test Functions ***************************/
    /**********************************************************************/
    bool Test_IsMatrixDimensionOK( QString inputADFile ); // Done

    bool Test_IsCovariateFile(); // Done

    bool Test_GetSelectedSubjects( QString selectedSubjectsFileTest ); // Done

    bool Test_GenerateMatlabInputFiles( QString outputDir, QString inputADFile, QString inputCovariateFileTest1, QString inputCovariateFileTest2,
                                        QString matlabInputADFile, QString matlabInputCovariateFile, QString selectedSubjectsFileTest );  // Done



    bool Test_GetSubjectsFromInputFile( QString inputADFile, QString inputCovariateFileTest1, QString inputCovariateFileTest2 );  // Done

    bool Test_GetRefSubjectsFromSelectedInputFiles( QString inputADFile, QString inputCovariateFileTest1, QString inputCovariateFileTest2 ); // Done

    bool Test_GetRefSubjects( QString inputADFile, QString inputCovariateFile, QString subjectsFilePath ); // Done


    bool Test_GetAllSubjectsFromSelectedInputFiles(); // Done

    bool Test_SortSubjects(); // Done

    bool Test_AssignSortedSubject(); // Done

    bool Test_GetCovariatesFromFileData( QString inputCovariateFileTest1, QString inputCovariateFileTest2 ); // Done


private:
    /**********************************************************************/
    /********************** Functions Used For Testing ********************/
    /**********************************************************************/
    QByteArray GetHashFile( QString filename );

    bool CompareFile( QString fileName1, QString fileName2 );

    bool CompareQStringList( QStringList qStringList1, QStringList qStringList2 );


    void DisplayErrorSubjects( QStringList subjectsExpected, QStringList subjectsDisplayed );

    void DisplayErrorAllSubjects( QMap<QString, QStringList> expectedSubjects, QMap<QString, QStringList> selectedSubjects );

    void DisplayErrorSortedSubjects( QMap< QString, QMap<QString, bool> > expectedSortedSubjects, QMap< QString, QMap<QString, bool> > sortedSubjects );

    void DisplayErrorUnMatchedSubjects( QMap<QString, QStringList > unMatchedSubjectsExpected, QMap<QString, QStringList > unMatchedSubjectsDisplayed );

    void DisplayErrorCovariates( QMap<int, QString> covariatesExpected, QMap<int, QString> covariatesDisplayed );
};

#endif // TESTPROCESSING_H
