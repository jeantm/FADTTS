#ifndef PLOT_H
#define PLOT_H

#include "Processing.h"

#include <QDir>

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


class Plot : public QObject
{
    Q_OBJECT

public:
    explicit Plot( QObject *parent = 0 );


    void SetQVTKWidget( QVTKWidget *qvtkWidget );

    void SetData( Data *data );

    void SetDirectory( QString directory );

    void SelectOutcome( QString outcome );

    void SelectPlot( QString plotSelected );

    void SelectCovariate( QString covariateSelected );


    void ResetDataFile();


    void DisplayVTKPlot();

    void ResetPlot();


public slots:
    void SavePlot();


private:
    Processing m_process;

    Data *m_data;

    vtkSmartPointer<vtkContextView> m_view;
    vtkSmartPointer<vtkChartXY> m_chart;

    QString m_matlabOutputDir, m_directory,
    m_plotSelected, m_outcome, m_covariateSelected;

    QStringList m_csvRawData, m_csvBetas, m_csvOmnibus, m_csvPostHoc;

    QVTKWidget *m_qvtkWidget;


    void SetPlotFiles();


    void FindyMinMax( QStringList rowData, QString &yMin, QString &yMax );

    bool LoadData( QList<float> &abscissa, QList<QStringList> &ordinate, int &nbrLine , QString &yMin, QString &yMax );

    void AddAxis( QMap< QString, vtkSmartPointer<vtkFloatArray> > &axis, vtkSmartPointer<vtkTable> &table, int nbrLine );

    void AddData( vtkSmartPointer<vtkTable> &table, QList<float> abscissa, QList<QStringList> ordinate, int nbrLine );

    void AddPlots( QMap< QString, vtkPlot* > &plot, vtkSmartPointer<vtkTable> &table, int nbrLine );

    void SetChartProperties( QString title, QString xName, QString yName, QString yMin, QString yMax );
};

#endif // PLOT_H
