#ifndef PLOT_H
#define PLOT_H

#include "Processing.h"

#include <QDir>
#include <QFileDialog>

#include <QVTKWidget.h>
//#include <cmath>

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
#include <vtkPen.h>
#include <vtkChartLegend.h>
#include <vtkRenderWindow.h>
#include <vtkGL2PSExporter.h>


class Plot : public QObject
{
    Q_OBJECT

public:
    explicit Plot( QObject *parent = 0 );


    void SetQVTKWidget( QVTKWidget *qvtkWidget );

    void InitPlot( QString directory, QString fibername );

    void ResetDataFile();

    void ResetPlot();


    void SelectPlot( QString plotSelected );

    void SelectProperty( QString property );

    void SelectCovariate( QString covariateSelected );

    void SetTitle( QString title );

    void SetxName( QString xName );

    void SetyName( QString yName );

    void SetYMin( bool yMinChecked, double yMin );

    void SetYMax( bool yMaxChecked, double yMax );


    void DisplayVTKPlot();


    void UpdatePlotTitle( QString titleName, bool isBold, bool isItalic, double titleSize );

    void UpdatePlotAxis( QString xName, QString yName, bool isBold, bool isItalic );

    void UpdatePlotLegend( QString legendPosition, QString legendSelected, QString legendName, QString legendColor,
                           double lineThickness, QString markerType, double markerThickness );


public slots:
    void SavePlot();

    void AlphaValueChanged( const double& newAlpha );

    void PlotSettingCheckState( const bool& plotSetting );


signals:
    void PropertiesUsed( const QStringList& );

    void CovariatesUsed( const QMap< int, QString >& );

    void SetEditTab( bool& );


private:
    QVTKWidget *m_qvtkWidget;

    vtkSmartPointer<vtkContextView> m_view;

    vtkSmartPointer<vtkChartXY> m_chart;

    Processing m_process;

//    vtkSmartPointer<vtkTable> m_table, m_tableSigBetas;

    QMap< int, vtkPlot* > m_line;

    QMap< QString, QList < QList < double > > > m_csvRawData, m_csvBeta,
    m_csvPostHocLpvalue, m_csvPostHocFDRLpvalue;

    QMap< int, QString > m_covariatesNoIntercept, m_allCovariates, m_binaryCovariates;

    QList < QStringList > m_csvCovariate;

    QList < QList < double > > m_csvOmnibusLpvalue, m_csvOmnibusFDRLpvalue, m_ordinate;

    QList< double > m_abscissa;

    QStringList m_csvRawDataFile, m_csvBetaFile, m_csvOmnibusLpvalueFile, m_csvOmnibusFDRLpvalueFile,
    m_csvPostHocLpvalueFile, m_csvPostHocFDRLpvalueFile, m_propertiesUsed;

    QString m_matlabOutputDir, m_directory, m_plotSelected, m_propertySelected, m_covariateSelected,
    m_title, m_fibername, m_xName, m_yName;

    double m_yMin, m_yMax, m_yMinGiven, m_yMaxGiven, m_alpha;

    int m_indexColumn, m_nbrPlot, m_nbrPoint;

    bool m_isCovariateBinary, m_plotSetting, m_yMinChecked, m_yMaxChecked;



    void SortFiles( QString directory, QStringList files, QMap< QString, QList < QList < double > > > &data );

    void TransposeData( QList < QList < double > > &data, int iMin, int jMin );

    void Transpose( QMap< QString, QList < QList < double > > > &data, int iMin, int jMin );

    QList < double > DataToDouble( QStringList rowData );

    QList < QList < double > > ToDouble( QList < QStringList > data );


    void SetRawData( QStringList files, QMap< QString, QList < QList < double > > > &data );

    void SetBeta(QStringList files, QMap< QString, QList<QList<double> > > &data );

    void SetOmnibusLpvalue( QStringList files, QList < QList < double > > &OmnibusLpvaluesData );

    void SetPostHocFDRLpvalue( QStringList files, QMap< QString, QList < QList < double > > > &data );


    void GetRawDataFiles();

    void GetBetaFiles();

    void GetOmnibusLpvalueFiles();

    void GetOmnibusFDRLpvalueFiles();

    void GetPostHocFDRLpvalueFiles();


    void SetProperties();

    void SetCovariates();

    void SetAbscissa();


    void ResetLoadData();


    void SeparateBinary( QList< QList < double > > &temp0Bin, QList< QList < double > > &temp1Bin );

    void GetMeanAndStdDv( QList< QList < double > > tempBin, QList < double > &tempMean, QList < double > &tempStdDv );

    void ProcessRawStats( QList< QList < double > > tempBin, QList < double > &tempBinMean, QList < double > &tempBinUp, QList < double > &tempBinDown );

    QList< QList < double > > Tolog10( QList< QList < double > > data );


    QList< QList < double > > LoadRawData();

    QList< QList < double > > LoadRawStats();

    QList< QList < double > > LoadBetas();

    QList< QList < double > > LoadBetaByCovariate();

    QList< QList < double > > LoadOmnibusLpvalues();

    QList< QList < double > > LoadOmnibusFDRLpvalues();

    QList< QList < double > > LoadPostHocFDRLpvalues();

    bool LoadData();


    void AddEntryRawData( vtkSmartPointer<vtkTable> &table );

    void AddEntryRawStats( vtkSmartPointer<vtkTable> &table );

    void AddEntryByProperties( vtkSmartPointer<vtkTable> &table, QMap< int, QString > covariates, int iShift );

    void AddEntryByCovariates( vtkSmartPointer<vtkTable> &table );

    void AddEntry( vtkSmartPointer<vtkTable> &table );


    void SetData( vtkSmartPointer<vtkTable> &table );


    void InitLines();

    void AddLineSigBetas( vtkSmartPointer<vtkTable> table, bool betaDisplayedByProperties, bool isOmnibus, int i, int incrementColor );

    void AddLineRawData( vtkSmartPointer<vtkTable> &table );

    void AddLineRawStats( vtkSmartPointer<vtkTable> &table );

    void AddLineBetas( vtkSmartPointer<vtkTable> table, bool isSigBeta, bool betaDisplayedByProperties, bool isOmnibus );

    void AddLineLPvalue( vtkSmartPointer<vtkTable> &table );

    void AddLines( vtkSmartPointer<vtkTable> &table );


    void SetDefaultTitle();

    void FindyMinMax();

    void SetChartProperties();
};

#endif // PLOT_H
