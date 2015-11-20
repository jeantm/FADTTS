#include "Plot.h"


/***************************************************************/
/********************** Public functions ***********************/
/***************************************************************/
Plot::Plot( QObject *parent ) :
    QObject( parent )
{
}


void Plot::SetData( Data *newData )
{
    m_data = newData;
}

void Plot::SetView( vtkSmartPointer<vtkContextView> newView )
{
    m_view = newView;
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
    vtkSmartPointer<vtkChartXY> chart =
            vtkSmartPointer<vtkChartXY>::New();
    m_view->GetScene()->AddItem(chart);

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
    plot.insert( "Intercept", chart->AddPlot( vtkChart::LINE ) );
    plot.insert( "SSRIExposure", chart->AddPlot( vtkChart::LINE ) );
    plot.insert( "Sex", chart->AddPlot( vtkChart::LINE ) );
    plot.insert( "GestAgeBirth", chart->AddPlot( vtkChart::LINE ) );
    plot.insert( "DaysSinceBirth", chart->AddPlot( vtkChart::LINE ) );
    plot.insert( "DTIDirection", chart->AddPlot( vtkChart::LINE ) );
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
    chart->GetAxis( vtkAxis::LEFT )->SetTitle( "AD" );
    chart->GetAxis( vtkAxis::LEFT )->SetRange( -0.05, 0.31 );
    chart->GetAxis( vtkAxis::LEFT )->SetBehavior( vtkAxis::FIXED );
    chart->GetAxis( vtkAxis::BOTTOM )->SetTitle( "Arclength" );
    chart->SetShowLegend( true );
}
