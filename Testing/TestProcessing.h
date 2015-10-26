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
    bool Test_RunScript();


    bool Test_isCOMP(); // Done

    bool Test_GetSelectedSubjectList( QString selectedSubjectListFileTest ); // Done

    bool Test_GenerateMatlabInputFiles( QString outputDir, QString inputADFile, QString inputCOMPFileTest1, QString inputCOMPFileTest2,
                                        QString matlabInputADFile, QString matlabInputCOMPFile, QString selectedSubjectListFileTest ); // Done



    bool Test_GetSubjectListFromInputFile( QString inputADFile, QString inputCOMPFileTest1, QString inputCOMPFileTest2 );  // Done

    bool Test_GetRefSubjectListFromSelectedInputFiles( QString inputADFile, QString inputCOMPFileTest1, QString inputCOMPFileTest2 ); // Done

    bool Test_GetRefSubjectList( QString inputADFile, QString inputCOMPFile, QString subjectListFilePath ); // Done


    bool Test_GetAllSubjectsFromSelectedInputFiles(); // Done

    bool Test_SortSubjectInInputFile(); // Done

    bool Test_AssignSortedSubject(); // Done


private:
    /**********************************************************************/
    /********************** Functions Used For Testing ********************/
    /**********************************************************************/
    QByteArray GetHashFile( QString filename );

    bool CompareFile( QString fileName1, QString fileName2 );

    bool CompareQStringList( QStringList qStringList1, QStringList qStringList2 );


    void DisplayErrorSubjectList( QStringList subjectListExpected, QStringList subjectListDisplayed );

    void DisplayErrorAllSubjects( QMap<QString, QStringList> expectedSubjects, QMap<QString, QStringList> selectedSubjects );

    void DisplayErrorSortedSubjects( QMap< QString, QMap<QString, bool> > expectedSortedSubjects, QMap< QString, QMap<QString, bool> > sortedSubjects );

    void DisplayErrorUnMatchedSubjects( QMap<QString, QStringList > unMatchedSubjectsExpected, QMap<QString, QStringList > unMatchedSubjectsDisplayed );
};

#endif // TESTPROCESSING_H
