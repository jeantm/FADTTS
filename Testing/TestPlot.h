#ifndef TESTPLOT_H
#define TESTPLOT_H

#include "Plot.h"
#include "Processing.h"

#include <QCryptographicHash>
#include <QSet>

#include <iostream>


class TestPlot
{
public:
    TestPlot();

    /**********************************************************************/
    /*************************** Test Functions ***************************/
    /**********************************************************************/
    bool Test_SetQVTKWidget();


    bool Test_QStringListToDouble();

    bool Test_DataToDouble();

    bool Test_SortFilesByProperties( QString rdRawData, QString faRawData, QString subMatrixRawData );

    bool Test_TransposeData();

    bool Test_TransposeDataInQMap();

    bool Test_SetRawData( QString rdFilePath, QString faFilePath, QString subMatrixFilePath,
                          QString rdTransposeFilePath, QString faTransposeFilePath, QString dataDir );

    bool Test_SetBeta( QString rdBetaFilePath, QString faBetaFilePath, QString dataDir );

    bool Test_SetOmnibusLpvalue( QString omnibusLpvalueFilePath, QString omnibusLpvalueTransposedFilePath, QString dataDir );

    bool Test_SetConfidenceBands( QString rdConfidenceBandsFilePath, QString faConfidenceBandsFilePath, QString dataDir );

    bool Test_SetPostHocFDRLpvalue( QString rdPostHocFDRLpvalueFilePath, QString faPostHocFDRLpvalueFilePath,
                                    QString rdTransposePostHocFDRLpvalueFilePath, QString faTransposePostHocFDRLpvalueFilePath, QString dataDir );


    bool Test_GetRawDataFiles( QString dataDir, QString tempoDir );

    bool Test_GetBetaFiles( QString dataDir, QString tempoDir );

    bool Test_GetOmnibusLpvalueFiles( QString dataDir, QString tempoDir );

    bool Test_GetOmnibusFDRLpvalueFiles( QString dataDir, QString tempoDir );

    bool Test_GetConfidenceBandsFiles( QString dataDir, QString tempoDir );

    bool Test_GetPostHocFDRLpvalueFiles( QString dataDir, QString tempoDir );


    bool Test_SetPlots( QString dataDir, QString tempoDir );

    bool Test_SetProperties( QString dataDir, QString tempoDir );

    bool Test_SetSubjects( QString dataDir, QString tempoDir );

    bool Test_SetCovariates( QString dataDir, QString tempoDir );

    bool Test_SetAbscissa( QString dataDir, QString tempoDir );


    bool Test_SetSelectedPlot( QString dataDir );


    bool Test_SetCustomizedTitle();

    bool Test_SetDefaultTitle();

    bool Test_SetCustomizedAxis();

    bool Test_SetDefaultAxis();

    bool Test_SetLegend();


    bool Test_SetMarkerType();


    bool Test_UpdateCovariatesNames( QString dataDir , QString tempoDir );


    bool Test_SeparateBinary( QString faFilePath, QString dataDir );

    bool Test_GetMeanAndStdDv();

    bool Test_ProcessRawStats();

    bool Test_SetSelectionToDisplayProperties( QString omnibusLpvalueFilePath );

    bool Test_Tolog10();


    bool Test_LoadRawData( QString dataDir , QString tempoDir );

    bool Test_LoadRawStats( QString dataDir );

    bool Test_LoadBetas( QString dataDir, QString tempoDir );

    bool Test_LoadBetaByCovariate( QString dataDir );

    bool Test_LoadOmnibusLpvalues( QString dataDir );

    bool Test_LoadConfidenceBand( QString dataDir, QString tempoDir );

    bool Test_LoadPostHocFDRLpvalues( QString dataDir );

    bool Test_LoadData( QString dataDir );


    bool Test_AddEntriesRawData();

    bool Test_AddEntriesRawStats();

    bool Test_AddEntriesByPropertiesOrCovariates();

    bool Test_AddEntriesConfidenceBands();


    bool Test_SetData();


    bool Test_InitLines();

    bool Test_AddSignificantLevel();

    bool Test_AddLineSigBetas( QString dataDir, QString tempoDir ); /** /!\ PB WITH TEST /!\ **/


    bool Test_GetyMinMax();


    bool Test_SavePlot( QString plotPath , QString dataDir, QString tempoDir ); /** /!\ PB WITH TEST /!\ --> Exeption segm **/



private:
    /**********************************************************************/
    /********************** Functions Used For Testing ********************/
    /**********************************************************************/
    void DisplayError_QStringList( QStringList qStringListExpected, QStringList qStringListDisplayed, QString type );

    void DisplayError_QMapIntQString( QMap< int, QString > qMapExpected, QMap< int, QString > qMapDisplayed, QString type );

    template <typename T>
    void DisplayError_QListQList( T qListQListExpected, T qListQListDisplayed, QString type );

    QByteArray GetHashFile( QString filePath );

    bool CompareFile( QString filePath1, QString filePath2 );
};

#endif // TESTPLOT_H
