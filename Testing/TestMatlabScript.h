#ifndef TESTMATLABSCRIPT_H
#define TESTMATLABSCRIPT_H

#include <QCryptographicHash>

#include <iostream>

#include "MatlabScript.h"

class TestMatlabScript
{
public:
    TestMatlabScript();


    /**********************************************************************/
    /*************************** Test Functions ***************************/
    /**********************************************************************/
    bool Test_GenerateMatlabScript( QString outputDir, QString matlabInputADFile, QString matlabInputCOMPFile, QString expectedMatlabScript ); // Done

private:
    /**********************************************************************/
    /********************** Functions Used For Testing ********************/
    /**********************************************************************/
    QByteArray GetHashFile( QString filename );

    bool CompareFile( QString fileName1, QString fileName2 );
};

#endif // TESTMATLABSCRIPT_H
