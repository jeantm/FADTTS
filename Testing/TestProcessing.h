#ifndef TESTPROCESSING_H
#define TESTPROCESSING_H

#include "Processing.h"

#include <QDir>
#include <QCryptographicHash>
#include <QSet>

#include <iostream>


class TestProcessing
{
public:
    TestProcessing();

    /**********************************************************************/
    /*************************** Test Functions ***************************/
    /**********************************************************************/
    bool Test_GetDataFromFile( QString file_N_Path, QString file_R_Path, QString file_RN_Path );


    bool Test_IsMatrixDimensionOK( QString adFilePath );

    bool Test_IsSubMatrix();


    bool Test_GetSubjectsFromFile( QString subjectFilePath );

    bool Test_GetSubjectsFromData( QString adFilePath, QString subMatrixFilePath0, QString subMatrixFilePath3 );

    bool Test_GetCovariatesFromFileData( QString inputCovariateFileTest1, QString subMatrixFilePath3 );


    bool Test_GetAllSubjects( QString adFilePath, QString subMatrixFilePath0 );

    bool Test_GetSubjectsFromSelectedFiles();

    bool Test_SortSubjects();

    bool Test_AssignSortedSubject();


    bool Test_Transpose_noGUI();

    bool Test_RemoveUnmatchedSubjects_noGUI();

    bool Test_ToDouble_noGUI();

    bool Test_GetMean_noGUI();

    bool Test_ApplyPearsonCorrelation_noGUI();


    bool Test_GenerateMatlabInputFiles( QString adFilePath, QString subMatrix0FilePath, QString subMatrix3FilePath, QString adMatlabFilePath,
                                        QString subMatrix0MatlabFilePath, QString subjectsFilePath, QString outputDir );


private:
    /**********************************************************************/
    /********************** Functions Used For Testing ********************/
    /**********************************************************************/
    void DisplayError_GetSubjects( QStringList subjectsExpected, QStringList subjectsDisplayed );

    void DisplayError_GetSubjectsFromSelectedFiles( QMap< int, QStringList > expectedSubjects, QMap< int, QStringList > selectedSubjects );

    void DisplayError_SortedSubjects( QMap< QString, QMap< int, bool > > expectedSortedSubjects, QMap< QString, QMap< int, bool > > sortedSubjects );

    void DisplayError_UnMatchedSubjects( QMap< QString, QList< int > > unMatchedSubjectsExpected, QMap< QString, QList< int > > unMatchedSubjectsDisplayed );

    void DisplayError_GetCovariates( QMap< int, QString > covariatesExpected, QMap< int, QString > covariatesDisplayed );


    QByteArray GetHashFile( QString filePath );

    bool CompareFile( QString filePath1, QString filePath2 );
};

#endif // TESTPROCESSING_H
