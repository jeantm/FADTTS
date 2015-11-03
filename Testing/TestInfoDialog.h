#ifndef TESTINFODIALOG_H
#define TESTINFODIALOG_H

#include <iostream>

#include "InfoDialog.h"
#include "Processing.h"

class TestInfoDialog
{
public:
    TestInfoDialog();

    /**********************************************************************/
    /*************************** Test Functions ***************************/
    /**********************************************************************/
    bool Test_DisplayFileInformation( QString adInputFile, QString inputCOMPFile ); // Done


private:
    /**********************************************************************/
    /********************** Functions Used For Testing ********************/
    /**********************************************************************/
    void DisplayErrorFileInfoDisplayed( QString infoExpected, QString infoDisplayed );
};

#endif // TESTINFODIALOG_H
