#ifndef PLOT_H
#define PLOT_H

#include "Processing.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include <QVTKWidget.h>

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkContextScene.h>
#include <vtkContextView.h>
#include <vtkChartXY.h>
#include <vtkTable.h>
#include <vtkFloatArray.h>
#include <vtkPlotPoints.h>
#include <vtkAxis.h>
#include <vtkTextProperty.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkTIFFWriter.h>
#include <vtkGL2PSExporter.h>
#include <vtkPen.h>
#include <vtkChartLegend.h>
#include <vtkRenderWindow.h>
#include <vtkContextMouseEvent.h>


class Plot : public QObject
{
    Q_OBJECT

public:
    friend class TestPlot; /** For unit tests **/
    friend class TestFADTTSWindow; /** For unit tests **/


    explicit Plot( QObject *parent = 0 );


    void SetQVTKWidget( QSharedPointer<QVTKWidget> qvtkWidget ); // Tested

    bool InitPlot( QString directory, QString fibername ); /// Not directly tested

    void ResetPlotData(); /// Not tested

    void ClearPlot(); /// Not tested


    void SetSelectedPlot( QString plotSelected ); // Tested

    void SetSelectedProperty( QString propertySelected ); /// Not tested

    void SetSelectedCovariate( QString covariateSelected ); /// Not tested

    void SetSelectionToDisPlay( const QMap< int, QPair< QString, QPair< bool, QString > > >& currentSelectionToDisplay ); /// Not tested


    void SetTitle( QString title, bool isBold, bool isItalic, double fontSize ); // Tested

    void SetDefaultTitle(); // Tested

    void SetGrid( bool gridOn ); /// Not tested

    void SetAxis( QString xName, QString yName, bool isBold, bool isItalic, bool isYMinSet, double yMin, bool isYMaxSet, double yMax ); // Tested

    void SetDefaultAxis(); // Tested

    void SetLegend( QString position ); // Tested


    void SetPvalueThreshold( double pvalueThreshold ); /// Not tested

    void SetLineWidth( double lineWidth ); /// Not tested

    void SetMarkerType( QString markerType ); // Tested

    void SetMarkerSize( double markerSize ); /// Not tested


    void UpdateCovariatesNames( const QMap<int, QString>& newCovariatesNames ); // Tested


    bool DisplayVTKPlot(); /// Not directly tested


public slots:
    void OnSavePlot(); /// Not tested


signals:
    void PlotsUsed( const QStringList& );

    void AllPropertiesUsed( const QMap< int, QString >& );

    void AllCovariatesUsed( const QMap< int, QString >& );

    void CovariatesAvailableForPlotting( const QMap< int, QString >& );


private:
    static QMap< QString, QList< int > > InitColorMap()
    {
        QMap< QString, QList< int > > newColorMap;
        newColorMap.insert( "Red", QList< int >() << 255 << 0  << 0 );
        newColorMap.insert( "Lime", QList< int >() << 0 << 255 << 0 );
        newColorMap.insert( "Blue", QList< int >() << 0 << 0 << 255 );
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
        newColorMap.insert( "Black", QList< int >() <<  0 << 0 << 0 );

        return newColorMap;
    }


    static const QMap< QString, QList< int > > m_allColors;

    Processing m_processing;

    QSharedPointer<QVTKWidget> m_qvtkWidget;
    vtkSmartPointer<vtkContextView> m_view;
    vtkSmartPointer<vtkChartXY> m_chart;
    QMap< int, vtkPlot* > m_line;


    QMap< QString, QList< QList< double > > > m_dataRawData, m_dataBeta, m_dataConfidenceBands, m_dataPostHocFDRLpvalue;

    QMap< int, QPair< QString, QPair< bool, QString > > > m_selectionToDisplay;

    QMap< int, QString > m_covariatesNoIntercept, m_allCovariates, m_binaryCovariates, m_properties;

    QList< QList< int > > m_lineColors;

    QList< QStringList > m_dataSubMatrix;

    QList< QList< double > > m_dataOmnibusLpvalue, m_dataOmnibusFDRLpvalue, m_ordinate;

    QList< double > m_abscissa;

    QPair< bool, double > m_yMin, m_yMax;

    QStringList m_csvRawDataFiles, m_csvBetaFiles, m_csvOmnibusLpvalueFiles, m_csvOmnibusFDRLpvalueFiles,
    m_csvConfidenceBandsFiles, m_csvPostHocFDRLpvalueFiles, m_lineNames, m_subjects, m_plotsUsed;

    QString m_matlabDirectory, m_directory, m_plotSelected, m_propertySelected, m_covariateSelected,
    m_fibername;

    double m_pvalueThreshold, m_lineWidth, m_markerSize;

    int m_nbrPlot, m_nbrPoint, m_markerType;

    bool m_isBinaryCovariatesSent, m_isAllCovariatesSent, m_isCovariatesNoInterceptSent, m_gridOn;


    QList< double > QStringListToDouble( const QStringList& rowData ); // Tested

    QList< QList< double > > DataToDouble( const QList<QStringList>& data ); // Tested

    void SortFilesByProperties( QString directory, const QStringList& files, QMap<QString, QList<QList<double> > >& data ); // Tested

    void TransposeData( QList<QList<double> >& data, int firstRow, int firstColumn ); // Tested

    void TransposeDataInQMap( QMap< QString, QList<QList<double> > >& data, int firstRow, int firstColumn ); // Tested


    void SetRawData(); // Tested

    void SetBeta(); // Tested

    void SetOmnibusLpvalue( const QStringList& omnibusLpvalueFiles, QList<QList<double> >& omnibusLpvaluesData ); // Tested

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

    void ProcessRawStats( const QList< QList< double > >& tempBin, QList< double >& tempBinMean, QList< double >& tempBinUp, QList< double >& tempBinDown ); // Tested

    void UpdateOrdinate( QList< QList< double > >& ordinate ); // Tested

    QList< QList< double > > ToLog10( const QList< QList< double > >& data ); // Tested


    QList< QList< double > > LoadRawData(); // Tested

    QList< QList< double > > LoadRawStats(); // Tested

    QList< QList< double > > LoadBetas(); // Tested

    QList< QList< double > > LoadBetaByCovariate(); // Tested

    QList< QList< double > > LoadOmnibusLpvalues( QList<QList<double> > omnibusLpvalues ); // Tested

    QList< QList< double > > LoadConfidenceBand(); // Tested

    QList< QList< double > > LoadPostHocFDRLpvalues(); // Tested

    bool LoadData(); /// Not directly tested


    void AddEntriesRawData( vtkSmartPointer<vtkTable>& table ); // Tested

    void AddEntriesRawStats( vtkSmartPointer<vtkTable>& table ); // Tested

    void AddEntriesByPropertiesOrCovariates( vtkSmartPointer<vtkTable>& table ); // Tested

    void AddEntriesCovariatesBands( vtkSmartPointer<vtkTable>& table ); // Tested

    void AddEntries( vtkSmartPointer<vtkTable>& table ); /// Not directly tested


    void SetData( vtkSmartPointer<vtkTable>& table ); // Tested


    void InitLines(); // Tested

    void AddSignificantLevel( double significantLevel ); // Tested

    void AddLineSigBetas( const vtkSmartPointer<vtkTable>& table, bool betaDisplayedByProperties, bool isOmnibus, int i ); /** /!\ WRITE TEST /!\ **/

    void AddLineRawData( const vtkSmartPointer<vtkTable>& table ); /// Not tested

    void AddLineRawStats( const vtkSmartPointer<vtkTable>& table ); /// Not tested

    void AddLineBetas( const vtkSmartPointer<vtkTable>& table, bool isSigBeta, bool betaDisplayedByProperties, bool isOmnibus ); /// Not tested

    void AddLineLPvalue( const vtkSmartPointer<vtkTable>& table ); /// Not tested

    void AddLineLConfidenceBands( const vtkSmartPointer<vtkTable>& table ); /// Not tested

    void AddLines( const vtkSmartPointer<vtkTable>& table ); /// Not directly tested


    QList<double> GetyMinMax(); // Tested

    void SetyMinMax(); /** /!\ WRITE TEST /!\ **/

    void SetChartProperties(); /// Not tested
};

#endif // PLOT_H
