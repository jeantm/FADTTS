#ifndef PLOT_H
#define PLOT_H

#include "Processing.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>

#include <QVTKWidget.h>

#include <vtkVersion.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkChartXY.h>
#include <vtkTable.h>
#include <vtkPlotPoints.h>
#include <vtkAxis.h>
#include <vtkChartLegend.h>
#ifdef GL2PSENABLED
    #include <vtkGL2PSExporter.h>
#endif
#include <vtkTextProperty.h>
#include <vtkBrush.h>
#include <vtkPen.h>
#include <vtkCommand.h>
#include <vtkCallbackCommand.h>
#include <vtkContextMouseEvent.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>


void CallbackAddSelectedLine( vtkObject* caller, long unsigned int eventId, void* clientData, void* callData ); /// Not tested

void CallbackUpdateSelectedLines( vtkObject* caller, long unsigned int eventId, void* clientData, void* callData ); /// Not tested


class Plot : public QObject
{
    Q_OBJECT

public:
    friend class TestPlot; /** For unit tests **/
    friend class TestFADTTSWindow; /** For unit tests **/


    explicit Plot( QObject *parent = 0 );


    void SetQVTKWidget( QSharedPointer< QVTKWidget > qvtkWidget, bool isQCThreshold ); // Tested

    bool InitPlot( QString directory, QString fibername, double pvalueThreshold ); // Not Directly Tested

    bool InitQCThresholdPlot( QList< QStringList > rawData, QStringList matchedSubjects ); // Not Directly Tested

    bool DisplayPlot(); // Not Directly Tested

    bool DisplayQCThresholdPlot(); // Not Directly Tested

    void ResetPlotData(); // Tested

    void ClearPlot(); // Tested


    QString& SetFibername();

    void SetSelectedPlot( QString plotSelected ); // Tested

    QString& SetSelectedProperty(); // Tested

    QString& SetSelectedCovariate(); // Tested

    QMap< int, QPair< QString, QPair< bool, QString > > >& SetSelectionToDisPlay(); // Tested

    void SetCovariatesProperties( QMap< int, QPair< QString, QPair< bool, QString > > > covariatesProperties ); // Tested

    void UpdateLineToDisplay( QMap< int, QPair< QString, QPair< bool, QString > > > selectionToDisplay ); /// Not tested*


    void SetDefaultTitle( bool isBold, bool isItalic, double fontSize ); // Tested

    void SetCustomizedTitle( QString title, bool isBold, bool isItalic, double fontSize ); // Tested

    bool& SetGrid(); // Not Directly Tested

    void UpdateGrid( bool checkState ); // Not Directly Tested

    void SetAbscissaNotation( bool checkState );

    void UpdateAbscissaNotation( bool checkState );

    void SetDefaultAxis( double labelSize, double NameSize, bool isBold, bool isItalic, bool isYMinSet, double yMin, bool isYMaxSet, double yMax ); // Tested

    void SetCustomizedAxis( double labelSize, QString xName, QString yName, double NameSize, bool isBold, bool isItalic, bool isYMinSet, double yMin, bool isYMaxSet, double yMax ); // Tested

    void SetLegend( bool showLegend, QString position ); // Tested


    double& SetPvalueThreshold(); // Tested

    void UpdatePvalueThreshold( double pvalueThreshold ); /// Not tested*

    double& SetLineWidth(); // Tested

    void UpdateLineWidth(); /// Not tested*

    void SetSelectedLineColor( QString color ); // Tested

    void UpdateSelectedLineColor( QString color ); /// Not tested*

    void SetMarkerType( QString markerType ); // Tested

    double& SetMarkerSize(); // Tested

    void UpdateMarker(); /// Not tested*

    bool& SetBinaryBetas();

    void UpdateBinaryBetas( bool isPosNeg );

    double& SetQCThreshold(); // Tested

    QStringList& SetAtlasQCThreshold(); /// Not tested*

    bool& SetCroppingEnabled();

    int& SetArcLengthStartIndex();

    int& SetArcLengthEndIndex();


    void UpdateCovariatesNames( const QMap< int, QString >& newCovariatesNames ); // Tested


    void AddSelectedLine( vtkSmartPointer< vtkPlot > newLine ); /// Not tested*

    void UpdateLineSelection(); /// Not tested*


    void UpdateNAN( const QStringList& m_nanSubjects);

    void UpdateQCThreshold( double qcThreshold, bool emitSignal );

    void UpdateCropping();

    void ShowHideProfileCropping( bool show );



public slots:
    void OnSavePlot(); // Tested


signals:
    void PlotsUsed( const QStringList& );

    void AllPropertiesUsed( const QMap< int, QString >& );

    void AllCovariatesUsed( const QMap< int, QString >& );

    void CovariatesAvailableForPlotting( const QMap< int, QString >& );

    void LinesSelected( const QStringList& );

    void UpdateSubjectsCorrelated( const QStringList&, const QStringList& );


private:
    enum dataKind{ RawData, Betas, OmnibusLpvalues, OmnibusFDRLpvalues, ConfidenceBands, PostHocpvalues, PostHocFDRLpvalues };

    /************************/
    /**** INITIALIZATION ****/
    /************************/
    Processing m_processing;
    QSharedPointer< QVTKWidget > m_qvtkWidget;
    vtkSmartPointer< vtkContextView > m_view;
    vtkSmartPointer< vtkChartXY > m_chart;
    QMap< int, vtkSmartPointer< vtkPlot > > m_line;
    QString m_directory, m_matlabDirectory, m_fibername;

    /********************/
    /**** LINE COLOR ****/
    /********************/
    static QMap< QString, QList< int > > InitColorMap()
    {
        QMap< QString, QList< int > > newColorMap;
        newColorMap.insert( "Red", QList< int >() << 255 << 0  << 0 );
        newColorMap.insert( "Lime", QList< int >() << 0 << 255 << 0 );
        newColorMap.insert( "Blue", QList< int >() << 0 << 0 << 255 );
        newColorMap.insert( "Carolina Blue", QList< int >() << 123 << 175 << 212 );
        newColorMap.insert( "Yellow", QList< int >() << 255 << 200 << 0 );
        newColorMap.insert( "Cyan", QList< int >() << 0 << 255 << 255 );
        newColorMap.insert( "Magenta", QList< int >() << 255 << 0 << 255 );
        newColorMap.insert( "Olive", QList< int >() << 128 << 128 << 0 );
        newColorMap.insert( "Teal", QList< int >() << 0 << 128 << 128 );
        newColorMap.insert( "Purple", QList< int >() << 128 << 0 << 128 );
        newColorMap.insert( "Rosy Brown", QList< int >() << 210 << 147 << 141 );
        newColorMap.insert( "Dark Sea Green", QList< int >() << 141 << 210 << 147 );
        newColorMap.insert( "Corn Flower Blue", QList< int >() << 147 << 141 << 210 );
        newColorMap.insert( "Maroon", QList< int >() << 128 << 0 << 0 );
        newColorMap.insert( "Green", QList< int >() << 0 << 128  << 0 );
        newColorMap.insert( "Navy", QList< int >() << 0 << 0 << 128 );
        newColorMap.insert( "Orange", QList< int >() << 255 << 128 << 0 );
        newColorMap.insert( "Mint", QList< int >() << 0 << 255 << 128 );
        newColorMap.insert( "Pink", QList< int >() << 255 << 0 << 128 );
        newColorMap.insert( "Brown", QList< int >() << 139 << 69 << 19 );
        newColorMap.insert( "Black", QList< int >() << 0 << 0 << 0 );

        return newColorMap;
    }
    static const QMap< QString, QList< int > > m_allColors;

    QList< QList< int > > m_lineColors;
    double m_selectedLineColor[ 3 ];

    /************************/
    /**** LINE SELECTION ****/
    /************************/
    typedef struct
    {
        vtkSmartPointer< vtkPlot > line;
        double color[ 3 ];
        double lineWidth;
    } struct_selectedLine;
    struct_selectedLine m_selectedLine;

    typedef struct
    {
        QMap< int, struct_selectedLine > selectedLines;
        bool isEmpty;
    } struct_selectedLines;
    struct_selectedLines m_previousSelectedLines, m_currentSelectedLines;

    QList< double > m_selectedLineIndex;
    QStringList m_selectedLineLabels;

    /***************/
    /**** FILES ****/
    /***************/
    QStringList m_csvRawDataFiles, m_csvBetaFiles, m_csvOmnibusLpvalueFiles, m_csvOmnibusFDRLpvalueFiles, m_csvConfidenceBandsFiles, m_csvPostHocFDRLpvalueFiles;

    /**************/
    /**** DATA ****/
    /**************/
    // Binary Data
    QMap< QString, QList< QList< double > > > m_dataRawData;
    QMap< QString, QMap< int, QMap< int, QList< QList< double > > > > > m_dataBinaryRawData, m_dataBinaryRawStats;
    QMap< QString, QMap< int, QMap< int, QStringList > > > m_subjectsBinaryRawData;
    QStringList m_subjects;

    // Raw Betas
    QMap< QString, QList< QList< double > > > m_dataBetasByProperties;
    QMap< int, QList< QList< double > > > m_dataBetasByCovariates;

    // Omnibus Local pvalues
    QList< QList< double > > m_dataOmnibusLpvalue, m_dataOmnibusFDRLpvalue;

    // Omnibus Sig Betas
    QMap< QString, QList< QList< double > > > m_dataOmnibusFDRSigBetasByProperties, m_abscissaOmnibusFDRSigBetasByProperties;
    QMap< int, QList< QList< double > > > m_dataOmnibusFDRSigBetasByCovariates, m_abscissaOmnibusFDRSigBetasByCovariates;

    // Confidence Bands
    QMap< QString, QList< QList< double > > > m_dataConfidenceBands;

    // Post-Hoc FDR Local pvalues
    QMap< QString, QList< QList< double > > > m_dataPostHocFDRLpvalue;
    QMap< int, QList< QList< double > > > m_dataPostHocFDRLpvaluesByCovariates;

    // Post-Hoc FDR Sig Betas on Average Raw Data
    QMap< QString, QMap< QString, QList< QList< double > > > > m_dataPostHocFDRSigBetasOnAverageRawData, m_abscissaPostHocFDRSigBetasOnAverageRawData;

    // Post-Hoc Sig Betas
    QMap< QString, QList< QList< double > > > m_dataPostHocFDRSigBetasByProperties, m_abscissaPostHocFDRSigBetasByProperties;
    QMap< int, QList< QList< double > > >  m_dataPostHocFDRSigBetasByCovariates, m_abscissaPostHocFDRSigBetasByCovariates;

    // Covariates
    QMap< int, QString > m_covariatesNoIntercept, m_allCovariates, m_binaryCovariates;
    bool m_isBinaryCovariatesSent, m_isAllCovariatesSent, m_isCovariatesNoInterceptSent;

    // Properties
    QMap< int, QString > m_properties;

    /******************/
    /**** PLOTTING ****/
    /******************/
    QMap< int, QPair< QString, QPair< bool, QString > > > m_selectionToDisplay, m_covariatesProperties;
    QList< QList< double > > m_ordinate;
    QList< double > m_abscissa;
    QStringList m_plotsAvailable, m_lineNames;
    QString m_plotSelected, m_propertySelected, m_covariateSelected;
    int m_nbrPlots, m_nbrPoints;
    bool m_isPosNeg;

    /**************/
    /**** AXIS ****/
    /**************/
    QPair< bool, double > m_yMin, m_yMax;
    double m_yMinMax[ 2 ];
    int m_abscissaNotation;
    bool m_gridOn;

    /*****************/
    /**** OPTIONS ****/
    /*****************/
    double m_pvalueThreshold, m_lineWidth, m_markerSize;
    int m_markerType;

    /**********************/
    /**** QC THRESHOLD ****/
    /**********************/
    QStringList m_matchedSubjects, m_atlasQCThreshold;
    double m_qcThreshold;
    int m_arcLengthStartIndex, m_arcLengthEndIndex;
    bool m_croppingEnabled;


    void SortFilesByProperties( QString directory, const QStringList& files, QMap< QString, QList< QList< double > > >& data, int dataKind ); // Tested

    void SetRawData(); /// Write new test

    void SetRawSats(); /// Write new test

    void SetBetasByCovariate(); /// Write new test

    void SetOmnibusLpvalue( const QStringList& omnibusLpvalueFiles, QList< QList< double > >& omnibusLpvaluesData ); /// Write new test

    void SetOmnibusFDRSigBetasByProperties(); /// Not tested*

    void SetOmnibusFDRSigBetasByCovariates(); /// Not tested*

    void SetPostHocFDRLpvaluesByCovariates(); /// Not tested*

    void SetPostHocFDRSigBetasOnAverageRawData(); /// Not tested*

    void SetPostHocFDRSigBetasByProperties(); /// Not tested*

    void SetPostHocFDRSigBetasByCovariates(); /// Not tested*

    bool SetRawDataQCThreshold( const QList<QStringList>& rawData ); // Tested


    void GetRawDataFiles(); // Tested

    void GetBetaFiles(); // Tested

    void GetOmnibusLpvalueFiles(); // Tested

    void GetOmnibusFDRLpvalueFiles(); // Tested

    void GetConfidenceBandsFiles(); // Tested

    void GetPostHocFDRLpvalueFiles(); // Tested


    bool SetPlots(); // Tested

    void SetProperties( const QStringList& properties ); // Tested

    void SetSubjects( const QList< QStringList >& rawDataSubMatrix ); // Tested

    bool IsCovariateBinary( const QList< QStringList >& data, int indexCovariate );

    void SetCovariates( const QList< QStringList >& data, int dataKind ); // Tested


    void GetMeanAndStdDv( const QList< QList< double > >& binaryRawData, QList< double >& tempMean, QList< double >& tempStdDv ); // Tested

    void ProcessRawStats( const QList< QList< double > >& binaryRawData, QList< double >& binaryRawDataMean, QList< double >& binaryRawDataUp, QList< double >& binaryRawDataDown ); // Tested

    void SetSelectionToDisplayProperties(); // Tested

    QList< QList< double > > ToLog10( const QList< QList< double > >& data ); // Tested


    QList< QList< double > > LoadRawData(); // Tested

    QList< QList< double > > LoadRawStats(); // Tested

    QList< QList< double > > LoadBetasByProperties(); // Tested

    QList< QList< double > > LoadBetasByCovariates(); // Tested

    QList< QList< double > > LoadConfidenceBand(); // Tested

    QList< QList< double > > LoadPostHocFDRLpvalues(); // Tested

    QList< QList< double > > LoadSigBetasOnAverageRawData(); /// Not Tested*

    QList< QList< double > > LoadOmnibusLpvalues(); // Tested

    bool LoadData(); // Not Directly Tested


    void AddEntriesRawData( vtkSmartPointer< vtkTable >& table ); // Tested

    void AddEntriesRawStats( vtkSmartPointer< vtkTable >& table ); // Tested

    void AddEntriesSigBetasOnAverageRawData( vtkSmartPointer< vtkTable >& table ); /// Not Tested*

    void AddEntriesByPropertiesOrCovariates( vtkSmartPointer< vtkTable >& table ); // Tested

    void AddEntriesConfidenceBands( vtkSmartPointer< vtkTable >& table ); // Tested

    void AddEntries( vtkSmartPointer< vtkTable >& table ); // Tested


    void SetData( vtkSmartPointer< vtkTable >& table ); // Tested


    void InitLines(); // Tested

    void InitQCThresholdLines();

    void AddSignificantLevel( double significantLevel ); // Tested

    void AddMean( QList< double > meanRawData ); // Not Directly Tested

    void AddCrop( bool previousExist );

    void AddSigBetas( const QList< double >& dataSigBetas, const QList< double >& abscissaSigBetas, int index ); // Not Directly Tested

    void AddSigBetasOnAverageRawData( const QList< double >& sigBetas, const QList< double >& sigBetasAbscissa, const QList< int >& markerColor, QString IDLegend );

    void AddLineRawData( const vtkSmartPointer< vtkTable >& table ); // Not Directly Tested

    void AddLineRawStats( const vtkSmartPointer< vtkTable >& table ); // Not Directly Tested

    void AddLineSigBetasOnAverageRawData( const vtkSmartPointer< vtkTable >& table ); /// Not tested*

    void AddLineBetas( const vtkSmartPointer< vtkTable >& table ); // Not Directly Tested

    void AddLineLPvalue( const vtkSmartPointer< vtkTable >& table, int shift ); // Not Directly Tested

    void AddLineLConfidenceBands( const vtkSmartPointer< vtkTable >& table ); // Not Directly Tested

    void AddLines( const vtkSmartPointer< vtkTable >& table ); // Not Directly Tested

    void AddQCThresholdLines( const vtkSmartPointer< vtkTable >& table ); // Not Directly Tested


    void GetyMinMax(); // Tested

    void SetAbscissaProperties( double xMin, double xMax ); /// Not tested*

    void SetOrdinateProperties( double yMin, double yMax ); /// Not tested*

    void SetAxisProperties(); // Not Directly Tested

    void SetQCThresholdAxisProperties(); /// Not tested*


    int LineAlreadySelected( vtkSmartPointer< vtkPlot > line ); /// Not tested*

    double* GetSelectedLineColor(); // Tested

    double GetLineWidth() const; // Tested

    void SetObservers(); /// Not tested


    void SavePlot( QString filePath ); /** /!\ PB WITH TEST /!\ **/


    void ZoomOut(); // Not Directly Tested


private slots:
    void OnMouseEvent( QMouseEvent* event );
};

#endif // PLOT_H
