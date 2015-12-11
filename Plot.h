#ifndef PLOT_H
#define PLOT_H

#include "Processing.h"

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

    void SetMatlabOutputDir( QString matlabOutputDir );


public slots:
    void DisplayVTKPlot();

    void ResetPlot();

    void SavePlot();


private:
    Processing m_process;

    Data *m_data;

    vtkSmartPointer<vtkContextView> m_view;
    vtkSmartPointer<vtkChartXY> m_chart;

    QString m_matlabOutputDir;

    QVTKWidget *m_qvtkWidget;


};

#endif // PLOT_H
