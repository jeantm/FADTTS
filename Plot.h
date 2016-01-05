#ifndef PLOT_H
#define PLOT_H

#include "Processing.h"

#include <QDir>

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


class Plot : public QObject
{
    Q_OBJECT

public:
    explicit Plot( QObject *parent = 0 );


    void SetQVTKWidget( QVTKWidget *qvtkWidget );

    void SetDirectory( QString directory );

    void SelectPlot( QString plotSelected );

    void SelectOutcome( QString outcome );    

    void SelectCovariate( QString covariateSelected );

    void SetTitle( QString title );

    void SetxName( QString xName );

    void SetyName( QString yName );

    void SetYMin( bool yMinChecked, double yMin );

    void SetYMax( bool yMaxChecked, double yMax );


    void ResetDataFile();


    void IsCovariateBinary();

    void DisplayVTKPlot();

    void ResetPlot();


public slots:
    void SavePlot();


signals:
    void OutcomeUsed( const QStringList&  );

    void CovariateUsed( const QStringList&  );


private:
    QVTKWidget *m_qvtkWidget;

    Processing m_process;

    vtkSmartPointer<vtkContextView> m_view;

    vtkSmartPointer<vtkChartXY> m_chart;

    vtkSmartPointer<vtkTable> m_table;

    QMap< QPair< int, QString >, vtkSmartPointer<vtkFloatArray> > m_axis;

    QMap< QPair< int, QString >, vtkPlot* > m_plot;

    QMap< int, QList < QStringList > > m_csvRawData, m_csvBeta, m_csvOmnibusLpvalue, m_csvOmnibusFDRLpvalue,
    m_csvPostHocLpvalue, m_csvPostHocFDRLpvalue;

    QMap< QPair < int, QString >, bool > m_covariates;

    QList< double > m_abscissa;

    QList< QList < double > > m_ordinate;

    QStringList m_csvRawDataFile, m_csvBetaFile, m_csvOmnibusLpvalueFile, m_csvOmnibusFDRLpvalueFile,
    m_csvPostHocLpvalueFile, m_csvPostHocFDRLpvalueFile;

    QString m_matlabOutputDir, m_directory,
    m_plotSelected, m_outcomeSelected, m_covariateSelected,
    m_title, m_xName, m_yName;

    double m_yMin, m_yMax, m_yMinGiven, m_yMaxGiven;

    int m_indexColumn, m_nbrPlot, m_nbrPoint;

    bool m_isCovariateBinary, m_yMinChecked, m_yMaxChecked;


    void SetCovariates();

    void SetFiles( QStringList files, QMap< int, QList < QStringList > > &data );

    void GetRawDataFiles();

    void GetBetaFiles();

    void GetOmnibusLpvalueFiles();

    void GetOmnibusFDRLpvalueFiles();

    void GetPostHocLpvalueFiles();

    void GetPostHocFDRLpvalueFiles();

    void ResetLoadData();

    QList < double > DataToDouble( QStringList rowData );


    void FindyMinMax();

    void LoadAbscissa();

    void LoadRawData();

    void SeparateData( QList< QList < double > > &temp0Bin, QList< QList < double > > &temp1Bin );

    void GetMeanAndStdDv( QList< QList < double > > tempBin, QList < double > &tempMean, QList < double > &tempStdDv );

    void ProcessRawStats( QList< QList < double > > tempBin, QList < double > &tempBinMean, QList < double > &tempBinUp, QList < double > &tempBinDown );

    void SetRawStatsData( QList < double > &temp0BinUp, QList < double > &temp0BinMean, QList < double > &temp0BinDown,
                          QList < double > &temp1BinUp, QList < double > &temp1BinMean, QList < double > &temp1BinDown );

    void LoadRawStats();

    void LoadOmnibus();

    void LoadPostHoc();

    bool LoadData();


    void AddAxis();

    void AddData();

    void AddPlots( float red, float green, float blue, float opacity );

    void SetChartProperties();
};

#endif // PLOT_H
