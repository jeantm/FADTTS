#ifndef TESTMATLABTHREAD_H
#define TESTMATLABTHREAD_H

#include "MatlabThread.h"

#include <QCryptographicHash>

#include <iostream>


class TestMatlabThread
{
public:
    TestMatlabThread();

    /**********************************************************************/
    /*************************** Test Functions ***************************/
    /**********************************************************************/

    /*************** Script ***************/
    bool Test_InitMatlabScript( QString refMatlabScriptPath );


    bool Test_SetHeader();


    bool Test_SetNbrCompThreads();

    bool Test_SetMVCMPath();


    bool Test_SetFiberName();

    bool Test_SetDiffusionProperties();

    bool Test_SetInputFiles();

    bool Test_SetCovariates();


    bool Test_SetNbrPermutation();

    bool Test_SetOmnibus();

    bool Test_SetPostHoc();

    bool Test_SetConfidenceBandsThreshold();

    bool Test_SetPvalueThreshold();


    bool Test_GenerateMFiles( QString myFDR,  QString outputDir );


private:
    /**********************************************************************/
    /********************** Functions Used For Testing ********************/
    /**********************************************************************/
    QByteArray GetHashFile( QString filePath );

    bool CompareFile( QString filePath1, QString filePath2 );
};

#endif // TESTMATLABTHREAD_H
