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
    bool Test_SubjectColumnID();


    bool Test_OnDelete( QString adFilePath );


    bool Test_LoadDisplayData( QString adFilePath, QString subMatrixFilePath );


    bool Test_OnSaveFile( QString adFilePath, QString newADFile , QString tempoDir );


private:
    /**********************************************************************/
    /********************** Functions Used For Testing ********************/
    /**********************************************************************/
    bool TestQTableWidget( QList<QStringList> expectedData, QSharedPointer<EditInputDialog> editInputDialog );


    QByteArray GetHashFile( QString filePath );

    bool CompareFile( QString filePath1, QString filePath2 );
};

#endif // TESTEDITINPUTDIALOG_H
