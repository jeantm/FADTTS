#include "Plot.h"
#include <QDebug>

/***************************************************************/
/********************** Public functions ***********************/
/***************************************************************/
Plot::Plot( QObject *parent ) :
    QObject( parent )
{
    m_chart = vtkSmartPointer<vtkChartXY>::New();
}


void Plot::SetQVTKWidget( QVTKWidget *qvtkWidget )
{
    m_qvtkWidget = qvtkWidget;
    m_view = vtkSmartPointer<vtkContextView>::New();
    m_view->SetInteractor( m_qvtkWidget->GetInteractor() );
    m_qvtkWidget->SetRenderWindow( m_view->GetRenderWindow() );
}

void Plot::SetData( Data *data )
{
    m_data = data;
}

void Plot::SetMatlabOutputDir( QString matlabOutputDir )
{
    m_matlabOutputDir = matlabOutputDir;
}


/***************************************************************/
/************************ Private slots ************************/
/***************************************************************/

/***************************************************************/
/********************** Private functions **********************/
/***************************************************************/
void Plot::DisplayVTKPlot()
{
    // Resetting scene
    m_view->GetScene()->ClearItems();
    m_view->GetScene()->AddItem( m_chart );

    // Loading data
    QString AD_MatlabInputFile = "/work/jeantm/Project/FADTTS_Origin/DataShaili/CT_L_Parietal-AllOutput/ad_CT_L_Parietal.txt";
    QList<QStringList> AD_MatlabInputData = m_process.GetDataFromFile( AD_MatlabInputFile );
    QList<float> abscissa;
    foreach( QStringList rowData, AD_MatlabInputData )
    {
        abscissa.append( rowData.first().toFloat() );
    }

    QString AD_BetasFile = "/work/jeantm/Project/FADTTS_Origin/DataShaili/CT_L_Parietal-AllOutput/CT-L-Parietal_AD_betas.csv";
    QList<QStringList> AD_BetasData = m_process.GetDataFromFile( AD_BetasFile );
    int nbrCovariates = AD_BetasData.size();


    // Creating a table with 2 named arrays
    vtkSmartPointer<vtkTable> table =
            vtkSmartPointer<vtkTable>::New();

    QMap< QString, vtkSmartPointer<vtkFloatArray> > axis;
    axis.insert( "Arclength", vtkSmartPointer<vtkFloatArray>::New() );
    axis.insert( "Intercept", vtkSmartPointer<vtkFloatArray>::New() );
    axis.insert( "SSRIExposure", vtkSmartPointer<vtkFloatArray>::New() );
    axis.insert( "Sex", vtkSmartPointer<vtkFloatArray>::New() );
    axis.insert( "GestAgeBirth", vtkSmartPointer<vtkFloatArray>::New() );
    axis.insert( "DaysSinceBirth", vtkSmartPointer<vtkFloatArray>::New() );
    axis.insert( "DTIDirection", vtkSmartPointer<vtkFloatArray>::New() );
    QMap< QString, vtkSmartPointer<vtkFloatArray> >::ConstIterator iterAxis = axis.begin();
    while( iterAxis != axis.end() )
    {
        iterAxis.value()->SetName( iterAxis.key().toUtf8().constData() );
        table->AddColumn( iterAxis.value().GetPointer() );
        ++iterAxis;
    }

    // Adding data
    int nbrPoints = abscissa.size();
    table->SetNumberOfRows( nbrPoints );
    for( int i = 0; i < nbrPoints; i++ )
    {
        table->SetValue( i, 0, abscissa.at( i ) );
        for( int j = 0; j < nbrCovariates; j++ )
        {
            table->SetValue( i, j + 1, AD_BetasData.at( j ).at( i ).toFloat() );
        }
    }

    // Adding plots
    QMap< QString, vtkPlot* > plot;
    plot.insert( "Intercept", m_chart->AddPlot( vtkChart::LINE ) );
    plot.insert( "SSRIExposure", m_chart->AddPlot( vtkChart::LINE ) );
    plot.insert( "Sex", m_chart->AddPlot( vtkChart::LINE ) );
    plot.insert( "GestAgeBirth", m_chart->AddPlot( vtkChart::LINE ) );
    plot.insert( "DaysSinceBirth", m_chart->AddPlot( vtkChart::LINE ) );
    plot.insert( "DTIDirection", m_chart->AddPlot( vtkChart::LINE ) );
    QMap< QString, vtkPlot* >::ConstIterator iterPlot = plot.begin();
    int i = 1;
    while( iterPlot != plot.end() )
    {
        iterPlot.value()->SetInputData( table.GetPointer(), 0, i );
        vtkPlotPoints::SafeDownCast( iterPlot.value() )->SetMarkerStyle( vtkPlotPoints::CIRCLE );
        iterPlot.value()->SetColor( 255*0.3*i, 255*0.2*i, 0, 255 );
        iterPlot.value()->SetWidth( 1.0 );
        ++iterPlot;
        i++;
    }

    // Setting chart properties
    m_chart->DrawAxesAtOriginOn();
    m_chart->GetAxis( vtkAxis::LEFT )->SetTitle( "AD" );
    m_chart->GetAxis( vtkAxis::LEFT )->SetRange( -0.05, 0.31 );
    m_chart->GetAxis( vtkAxis::LEFT )->SetBehavior( vtkAxis::FIXED );
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetTitle( "Arclength" );
    m_chart->SetShowLegend( true );


    m_chart->SetTitle( "Title Test" );
}


void Plot::ResetPlot()
{
    m_chart->ClearPlots();
}

void Plot::SavePlot()
{
//    QRect rectangle;
//    QPixmap pixmap( rectangle.size() );
//    QRect rectangle;
//    QPixmap pixmap;
//    m_qvtkWidget->render( &pixmap, QPoint(), QRegion( rectangle ) );
//    pixmap.save( m_matlabOutputDir + "/example.png" );
}
