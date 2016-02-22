#ifndef TESTPLOT_H
#define TESTPLOT_H

#include "Plot.h"
#include "Processing.h"

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


    bool Test_GetRawDataFiles( QString dataDir );

    bool Test_GetBetaFiles( QString dataDir );

    bool Test_GetOmnibusLpvalueFiles( QString dataDir );

    bool Test_GetOmnibusFDRLpvalueFiles( QString dataDir );

    bool Test_GetConfidenceBandsFiles( QString dataDir );

    bool Test_GetPostHocFDRLpvalueFiles( QString dataDir );


    bool Test_SetPlots( QString dataDir );

    bool Test_SetProperties( QString dataDir );

    bool Test_SetSubjects( QString dataDir );

    bool Test_SetCovariates( QString dataDir );

    bool Test_SetAbscissa( QString dataDir );


    bool Test_SetSelectedPlot( QString dataDir );


    bool Test_SetTitle();

    bool Test_SetDefaultTitle();

    bool Test_SetAxis();

    bool Test_SetDefaultAxis();

    bool Test_SetLegend();


    bool Test_SetMarkerType();


    bool Test_UpdateCovariatesNames( QString dataDir );


    bool Test_SeparateBinary( QString faFilePath, QString dataDir );

    bool Test_GetMeanAndStdDv();

    bool Test_ProcessRawStats();

    bool Test_UpdateOrdinate( QString omnibusLpvalueFilePath );

    bool Test_Tolog10();


    bool Test_LoadRawData( QString dataDir );

    bool Test_LoadRawStats( QString dataDir );

    bool Test_LoadBetas( QString dataDir );

    bool Test_LoadBetaByCovariate( QString dataDir );

    bool Test_LoadOmnibusLpvalues( QString dataDir );

    bool Test_LoadConfidenceBand( QString dataDir );

    bool Test_LoadPostHocFDRLpvalues( QString dataDir );

    bool Test_LoadData( QString dataDir );


    bool Test_AddEntriesRawData();

    bool Test_AddEntriesRawStats();

    bool Test_AddEntriesByPropertiesOrCovariates();

    bool Test_AddEntriesCovariatesBands();


    bool Test_SetData();


    bool Test_InitLines();

    bool Test_AddSignificantLevel();

    bool Test_AddLineSigBetas(); // write test


    bool Test_GetyMinMax();

    bool Test_SetyMinMax(); // write test



private:
    /**********************************************************************/
    /********************** Functions Used For Testing ********************/
    /**********************************************************************/
    void DisplayError_FileList( QStringList fileListExpected, QStringList fileListDisplayed );

    void DisplayError_SubjectList( QStringList subjectListExpected, QStringList subjectListDisplayed );

    void DisplayError_PlotList( QStringList plotListExpected, QStringList plotListDisplayed );

    void DisplayError_LineNameList( QStringList lineNameListExpected, QStringList lineNameListDisplayed );

    void DisplayError_EntryList( QStringList entryListExpected, QStringList entryListDisplayed );

    void DisplayError_QStringList( QStringList qStringListExpected, QStringList qStringListDisplayed, QString type );


    void DisplayError_Properties( QMap<int, QString> propertiesExpected, QMap<int, QString> propertiesDisplayed );

    void DisplayError_Covariates( QMap<int, QString> covariatesExpected, QMap<int, QString> covariatesDisplayed );

    void DisplayError_QMapIntQString( QMap<int, QString> qMapExpected, QMap<int, QString> qMapDisplayed, QString type );

    template <typename T>
    void DisplayError_QListQList( T qListQListExpected, T qListQListDisplayed, QString type );
};

#endif // TESTPLOT_H
