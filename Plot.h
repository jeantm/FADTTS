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
    explicit Plot( QObject *parent = 0 );


    void SetQVTKWidget( QVTKWidget *qvtkWidget );

    bool InitPlot( QString directory, QString fibername );

    void ResetPlotData();

    void ClearPlot();


    void SelectPlot( QString plotSelected );

    void SelectProperty( QString property );

    void SelectCovariate( QString covariateSelected );


    void SetLinesToDisPlay( const QMap< int, QPair< QString, QPair< bool, QString > > > currentLinesForDisplay );\

    void UpdateCovariatesNames( QMap< int, QString > newCovariatesNames );


    void SetTitle( QString title, bool isBold, bool isItalic, double fontSize );

    void SetDefaultTitle();

    void SetGrid( bool gridOn );

    void SetAxis( QString xName, QString yName, bool isBold, bool isItalic,
                  bool isYMinSet, double yMin, bool isYMaxSet, double yMax );

    void SetDefaultAxis();

    void SetLegend( QString position );

    void SetPvalueThreshold( double pvalueThreshold );

    void SetLineWidth( double lineWidth );

    void SetMarkerType( QString markerType );

    void SetMarkerSize( double markerSize );


    bool DisplayVTKPlot();


public slots:
    void OnSavePlot();

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

    QVTKWidget *m_qvtkWidget;
    vtkSmartPointer<vtkContextView> m_view;
    vtkSmartPointer<vtkChartXY> m_chart;
    QMap< int, vtkPlot* > m_line;


    QMap< QString, QList < QList < double > > > m_dataRawData, m_dataBeta, m_dataConfidenceBands, m_dataPostHocFDRLpvalue;

    QMap< int, QPair< QString, QPair< bool, QString > > > m_linesToDisplay;

    QMap< int, QString > m_covariatesNoIntercept, m_allCovariates, m_binaryCovariates, m_properties;

    QList< QList< int > > m_lineColors;

    QList < QStringList > m_dataCovariate;

    QList < QList < double > > m_dataOmnibusLpvalue, m_dataOmnibusFDRLpvalue, m_ordinate;

    QList< double > m_abscissa;

    QPair< bool, double > m_yMin, m_yMax;

    QStringList m_csvRawDataFiles, m_csvBetaFiles, m_csvOmnibusLpvalueFiles, m_csvOmnibusFDRLpvalueFiles,
    m_csvConfidenceBandsFiles, m_csvPostHocFDRLpvalueFiles, m_lineNames, m_subjects;

    QString m_matlabDirectory, m_directory, m_plotSelected, m_propertySelected, m_covariateSelected,
    m_fibername;

    double m_pvalueThreshold, m_lineWidth, m_markerSize;

    int m_nbrPlot, m_nbrPoint, m_markerType;

    bool m_isBinaryCovariatesSend, m_isAllCovariatesSend, m_isCovariatesNoInterceptSend, m_gridOn;


    void SortFilesByProperties( QString directory, QStringList files, QMap< QString, QList < QList < double > > > &data );

    void TransposeData( QList < QList < double > > &data, int firstRow, int firstColumn );

    void TransposeDataInQMap( QMap< QString, QList<QList<double> > > &data, int firstRow, int firstColumn );

    QList < double > QStringListToDouble( QStringList rowData );

    QList < QList < double > > DataToDouble( QList < QStringList > data );


    void SetRawData();

    void SetBeta();

    void SetOmnibusLpvalue( QStringList OmnibusLpvalueFiles, QList < QList < double > > &OmnibusLpvaluesData );

    void SetConfidenceBands();

    void SetPostHocFDRLpvalue();


    bool GetRawDataFiles();

    void GetBetaFiles();

    void GetOmnibusLpvalueFiles();

    void GetOmnibusFDRLpvalueFiles();

    void GetConfidenceBandsFiles();

    void GetPostHocFDRLpvalueFiles();


    void SetPlots();

    void SetProperties();

    void SetCovariates();

    void SetAbscissa();


    void ResetLoadData();


    void SeparateBinary( QList< QList < double > > &temp0Bin, QList< QList < double > > &temp1Bin );

    void GetMeanAndStdDv( QList< QList < double > > tempBin, QList < double > &tempMean, QList < double > &tempStdDv );

    void ProcessRawStats( QList< QList < double > > tempBin, QList < double > &tempBinMean, QList < double > &tempBinUp, QList < double > &tempBinDown );

    void LoadLinesToDisplay( QList< QList < double > > &ordinate );

    QList< QList < double > > Tolog10( QList< QList < double > > data );


    QList< QList < double > > LoadRawData();

    QList< QList < double > > LoadRawStats();

    QList< QList < double > > LoadBetas();

    QList< QList < double > > LoadBetaByCovariate();

    QList< QList < double > > LoadOmnibusLpvalues( QList<QList<double> > omnibusLpvalues );

    QList< QList < double > > LoadConfidenceBand();

    QList< QList < double > > LoadPostHocFDRLpvalues();

    bool LoadData();


    void AddEntriesRawData( vtkSmartPointer<vtkTable> &table );

    void AddEntriesRawStats( vtkSmartPointer<vtkTable> &table );

    void AddEntriesByPropertiesOrCovariates( vtkSmartPointer<vtkTable> &table );

    void AddEntriesCovariatesBands( vtkSmartPointer<vtkTable> &table );

    void AddEntries( vtkSmartPointer<vtkTable> &table );


    void SetData( vtkSmartPointer<vtkTable> &table );


    void InitLines();

    void AddSignificantLevel( vtkSmartPointer<vtkTable> table, double significantLevel );

    void AddLineSigBetas( vtkSmartPointer<vtkTable> table, bool betaDisplayedByProperties, bool isOmnibus, int i );

    void AddLineRawData( vtkSmartPointer<vtkTable> table );

    void AddLineRawStats( vtkSmartPointer<vtkTable> table );

    void AddLineBetas( vtkSmartPointer<vtkTable> table, bool isSigBeta, bool betaDisplayedByProperties, bool isOmnibus );

    void AddLineLPvalue( vtkSmartPointer<vtkTable> table );

    void AddLineLConfidenceBands( vtkSmartPointer<vtkTable> table );

    void AddLines( vtkSmartPointer<vtkTable> table );


    QList<double> GetyMinMax();

    void SetyMinMax();

    void SetChartProperties();
};

#endif // PLOT_H
