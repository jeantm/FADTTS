#ifndef PLOT_H
#define PLOT_H

#include "Data.h"
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


class Plot : public QObject
{
    Q_OBJECT

public:
    explicit Plot( QObject *parent = 0 );

    void SetData( Data *newData );

    void SetView( vtkSmartPointer<vtkContextView> newView );


public slots:
    void DisplayVTKPlot();

private:
    Processing m_process;

    Data *m_data;

    vtkSmartPointer<vtkContextView> m_view;
};

#endif // PLOT_H
