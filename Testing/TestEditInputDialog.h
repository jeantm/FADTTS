#ifndef TESTEDITINPUTDIALOG_H
#define TESTEDITINPUTDIALOG_H

#include "EditInputDialog.h"

#include <QCryptographicHash>
#include <QSet>

#include <iostream>


class TestEditInputDialog
{
public:
    TestEditInputDialog();

    /**********************************************************************/
    /*************************** Test Functions ***************************/
    /**********************************************************************/
    bool Test_SetCurrentDir();

    bool Test_CovariateColumnID();


    bool Test_OnDelete( QString adFilePath );


    bool Test_LoadDisplayData( QString adFilePath, QString subMatrixFilePath );


    bool Test_OnSaveFile( QString outputDir, QString adFilePath, QString newADFile );


private:
    /**********************************************************************/
    /********************** Functions Used For Testing ********************/
    /**********************************************************************/
    bool TestQTableWidget( QList<QStringList> expectedData, QSharedPointer<EditInputDialog> editInputDialog );


    QByteArray GetHashFile( QString filePath );

    bool CompareFile( QString filePath1, QString filePath2 );
};

#endif // TESTEDITINPUTDIALOG_H
