#ifndef TESTEDITINPUTDIALOG_H
#define TESTEDITINPUTDIALOG_H

#include <iostream>

#include "EditInputDialog.h"

class TestEditInputDialog
{
public:
    TestEditInputDialog();

    /**********************************************************************/
    /*************************** Test Functions ***************************/
    /**********************************************************************/
    bool Test_UploadData( QString adInputFile ); // Done

    bool Test_Delete( QString adInputFile ); // Done

    bool Test_RefreshFileInfo( QString adInputFile, QString inputCOMPFile ); // Done


private:
    /**********************************************************************/
    /********************** Functions Used For Testing ********************/
    /**********************************************************************/
    bool TestQTableWidget( QList<QStringList> expectedData, QSharedPointer<EditInputDialog> editInputDialog );

    void DisplayErrorFileInfoDisplayed( QString infoExpected, QString infoDisplayed );
};

#endif // TESTEDITINPUTDIALOG_H
