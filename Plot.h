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
#include <vtkGL2PSExporter.h>
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


    void SetQVTKWidget( QSharedPointer< QVTKWidget > qvtkWidget ); // Tested

    bool InitPlot( QString directory, QString fibername ); // Not Directly Tested

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


    void SetDefaultTitle(); // Tested

    void SetCustomizedTitle( QString title, bool isBold, bool isItalic, double fontSize ); // Tested

    bool& SetGrid(); // Not Directly Tested

    void UpdateGrid( bool checkState ); // Not Directly Tested

    void SetAbscissaNotation( bool checkState );

    void UpdateAbscissaNotation( bool checkState );

    void SetDefaultAxis(); // Tested

    void SetCustomizedAxis( double labelSize, QString xName, QString yName, double NameSize, bool isBold, bool isItalic, bool isYMinSet, double yMin, bool isYMaxSet, double yMax ); // Tested

    void SetLegend( bool showLegend, QString position ); // Tested


    double& SetPvalueThreshold(); // Tested

    void UpdatePvalueThresold( bool customizedTitle ); /// Not tested*

    double& SetLineWidth(); // Tested

    void UpdateLineWidth(); /// Not tested*

    void SetSelectedLineColor( QString color ); // Tested

    void UpdateSelectedLineColor( QString color ); /// Not tested*

    void SetMarkerType( QString markerType ); // Tested

    double& SetMarkerSize(); // Tested

    void UpdateMarker(); /// Not tested*

    double& SetQCThreshold(); // Tested

    QStringList& SetAtlasQCThreshold(); /// Not tested*


    void UpdateCovariatesNames( const QMap< int, QString >& newCovariatesNames ); // Tested


    void AddSelectedLine( vtkSmartPointer< vtkPlot > newLine ); /// Not tested*

    void UpdateLineSelection(); /// Not tested*



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

    Processing m_processing;

    QSharedPointer< QVTKWidget > m_qvtkWidget;
    vtkSmartPointer< vtkContextView > m_view;
    vtkSmartPointer< vtkChartXY > m_chart;
    QMap< int, vtkSmartPointer< vtkPlot > > m_line;

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


    QMap< QString, QList< QList< double > > > m_dataRawData, m_dataBeta, m_dataConfidenceBands, m_dataPostHocFDRLpvalue;

    QMap< int, QPair< QString, QPair< bool, QString > > > m_selectionToDisplay, m_covariatesProperties;

    QMap< int, QString > m_covariatesNoIntercept, m_allCovariates, m_binaryCovariates, m_properties;

    QList< QList< int > > m_lineColors;

    QList< QStringList > m_dataSubMatrix;

    QList< QList< double > > m_dataOmnibusLpvalue, m_dataOmnibusFDRLpvalue, m_ordinate;

    QList< double > m_abscissa, m_selectedLineIndex;

    QPair< bool, double > m_yMin, m_yMax;

    QStringList m_csvRawDataFiles, m_csvBetaFiles, m_csvOmnibusLpvalueFiles, m_csvOmnibusFDRLpvalueFiles,
    m_csvConfidenceBandsFiles, m_csvPostHocFDRLpvalueFiles, m_lineNames, m_subjects, m_plotsUsed, m_selectedLineLabels, m_matchedSubjects, m_atlasQCThreshold;

    QString m_matlabDirectory, m_directory, m_plotSelected, m_propertySelected, m_covariateSelected,
    m_fibername;

    double m_selectedLineColor[ 3 ], m_yMinMax[ 2 ], m_pvalueThreshold, m_lineWidth, m_markerSize, m_qcThreshold;

    int m_nbrPlots, m_nbrPoints, m_markerType, m_abscissaNotation;

    bool m_isBinaryCovariatesSent, m_isAllCovariatesSent, m_isCovariatesNoInterceptSent, m_gridOn;


    QList< double > QStringListToDouble( const QStringList& rowData ); // Tested

    QList< QList< double > > DataToDouble( const QList< QStringList >& data ); // Tested

    void SortFilesByProperties( QString directory, const QStringList& files, QMap< QString, QList< QList< double > > >& data ); // Tested

    void TransposeData( QList< QList< double > >& data, int firstRow, int firstColumn ); // Tested

    void TransposeDataInQMap( QMap< QString, QList< QList< double > > >& data, int firstRow, int firstColumn ); // Tested

    void RemoveUnmatchedSubjects( QList<QStringList> &rawData ); // Tested


    void SetRawData(); // Tested

    bool SetRawDataQCThreshold( QList<QStringList> &rawData ); // Tested

    void SetBeta(); // Tested

    void SetOmnibusLpvalue( const QStringList& omnibusLpvalueFiles, QList< QList< double > >& omnibusLpvaluesData ); // Tested

    void SetConfidenceBands(); // Tested

    void SetPostHocFDRLpvalue(); // Tested


    bool GetRawDataFiles(); // Tested

    void GetBetaFiles(); // Tested

    void GetOmnibusLpvalueFiles(); // Tested

    void GetOmnibusFDRLpvalueFiles(); // Tested

    void GetConfidenceBandsFiles(); // Tested

    void GetPostHocFDRLpvalueFiles(); // Tested


    void SetPlots(); // Tested

    void SetProperties(); // Tested

    void SetSubjects(); // Tested

    void SetCovariates(); // Tested

    void SetAbscissa(); // Tested


    void SeparateBinary( QList< QList< double > >& temp0Bin, QList< QList< double > >& temp1Bin ); // Tested

    void GetMeanAndStdDv( const QList< QList< double > >& tempBin, QList< double >& tempMean, QList< double >& tempStdDv ); // Tested

    QList< double > GetMean( const QList< QList< double > >& rawData ); // Tested

    void ProcessRawStats( const QList< QList< double > >& tempBin, QList< double >& tempBinMean, QList< double >& tempBinUp, QList< double >& tempBinDown ); // Tested

    void SetSelectionToDisplayProperties(); // Tested

    QList< QList< double > > ToLog10( const QList< QList< double > >& data ); // Tested


    QList< QList< double > > LoadRawData(); // Tested

    QList< QList< double > > LoadRawStats(); // Tested

    QList< QList< double > > LoadSigBetasOnAverageRawData(); /// Not Tested*

    QList< QList< double > > LoadBetas(); // Tested

    QList< QList< double > > LoadBetaByCovariate(); // Tested

    QList< QList< double > > LoadOmnibusLpvalues( QList< QList< double > > omnibusLpvalues ); // Tested

    QList< QList< double > > LoadConfidenceBand(); // Tested

    QList< QList< double > > LoadPostHocFDRLpvalues(); // Tested

    bool LoadData(); // Not Directly Tested


    void AddEntriesRawData( vtkSmartPointer< vtkTable >& table ); // Tested

    void AddEntriesRawStats( vtkSmartPointer< vtkTable >& table ); // Tested

    void AddEntriesSigBetasOnAverageRawData( vtkSmartPointer< vtkTable >& table ); /// Not Tested*

    void AddEntriesByPropertiesOrCovariates( vtkSmartPointer< vtkTable >& table ); // Tested

    void AddEntriesConfidenceBands( vtkSmartPointer< vtkTable >& table ); // Tested

    void AddEntries( vtkSmartPointer< vtkTable >& table ); // Tested


    void SetData( vtkSmartPointer< vtkTable >& table ); // Tested


    double ApplyPearsonCorrelation( int indexLine, const QList<double>& meanRawData ); // Tested

    void InitLines(); // Tested

    void AddSignificantLevel( double significantLevel ); // Tested

    void AddMean( QList< double > meanRawData ); // Not Directly Tested

    void AddLineSigBetas( const vtkSmartPointer< vtkTable >& table, bool betaDisplayedByProperties, bool isOmnibus, int i ); // Not Directly Tested

    void AddLineRawData( const vtkSmartPointer< vtkTable >& table ); // Not Directly Tested

    void AddLineRawStats( const vtkSmartPointer< vtkTable >& table ); // Not Directly Tested

    void AddLineSigBetasOnAverageRawData( const vtkSmartPointer< vtkTable >& table ); /// Not tested*

    void AddLineBetas( const vtkSmartPointer< vtkTable >& table ); // Not Directly Tested

    void AddLineLPvalue( const vtkSmartPointer< vtkTable >& table ); // Not Directly Tested

    void AddLineLConfidenceBands( const vtkSmartPointer< vtkTable >& table ); // Not Directly Tested

    void AddLines( const vtkSmartPointer< vtkTable >& table ); // Not Directly Tested

    void AddQCThresholdLines( const vtkSmartPointer< vtkTable >& table, const QList< double >& atlas ); // Not Directly Tested


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
