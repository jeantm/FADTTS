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


class Plot : public QObject
{
    Q_OBJECT

public:
    explicit Plot( QObject *parent = 0 );


    void SetQVTKWidget( QVTKWidget *qvtkWidget );

    void SetData( Data *data );

    void SetDirectory( QString directory );

    void SelectPlot( QString plotSelected );

    void SelectOutcome( QString outcome );    

    void SelectCovariate( QString covariateSelected );


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
    Processing m_process;

    Data *m_data;

    vtkSmartPointer<vtkContextView> m_view;
    vtkSmartPointer<vtkChartXY> m_chart;

    QString m_matlabOutputDir, m_directory,
    m_plotSelected, m_outcomeSelected, m_covariateSelected;

    QStringList m_csvRawDataFile, m_csvBetas, m_csvOmnibus, m_csvPostHoc;

    QMap < int, QList < QStringList > > m_csvRawData;

    QMap< QPair< int, QString >, bool > m_covariates;

    QVTKWidget *m_qvtkWidget;

    bool m_isCovariateBinary;

    int m_indexColumn;


    void ProcessCovariates();

    void SetRawData( QStringList rawDataFileNames );

    void SetRawDataFiles();

    void SetPlotFiles();


    void FindyMinMax( QStringList rowData, QString &yMin, QString &yMax );

    void LoadRawData( QList<float> &abscissa, QList<QStringList> &ordinate, int &nbrPlot, QString &yMin, QString &yMax );

    void SeparateData( QList<QStringList> ordinate, QList<QStringList> &temp0Bin, QList<QStringList> &temp1Bin);

    void GetMeanAndStdDv( QList<QStringList> tempBin, QStringList &tempMean, QStringList &tempStdDv );

    void ProcessRawStats( QList<QStringList> tempBin, QStringList &tempBinMean, QStringList &tempBinUp, QStringList &tempBinDown );

    void SetRawStatsData( QList<QStringList> &ordinate, QStringList &temp0BinUp, QStringList &temp0BinMean, QStringList &temp0BinDown,
                                QStringList &temp1BinUp, QStringList &temp1BinMean, QStringList &temp1BinDown );

    void LoadRawStats( QList<QStringList> &ordinate , QString &yMin, QString &yMax );

    bool LoadData( QList<float> &abscissa, QList<QStringList> &ordinate, int &nbrPlot , QString &yMin, QString &yMax );


    void AddAxis( QMap< QString, vtkSmartPointer<vtkFloatArray> > &axis, vtkSmartPointer<vtkTable> &table, int nbrPlot );

    void AddData( vtkSmartPointer<vtkTable> &table, QList<float> abscissa, QList<QStringList> ordinate, int nbrPlot );

    void AddPlots( QMap< QString, vtkPlot* > &plot, vtkSmartPointer<vtkTable> &table, int nbrPlot, float red, float green, float blue, float opacity );

    void SetChartProperties( QString title, QString xName, QString yName, QString yMin, QString yMax );
};

#endif // PLOT_H
