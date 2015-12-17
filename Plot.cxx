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

void Plot::SetDirectory( QString directory )
{
    m_directory = directory;
    m_matlabOutputDir = directory + "/MatlabOutputs";
    SetPlotFiles();
}

void Plot::SelectPlot( QString plotSelected )
{
    m_plotSelected = plotSelected;
}

void Plot::SelectOutcome( QString outcome )
{
    m_outcome = outcome;
}

void Plot::SelectCovariate( QString covariateSelected )
{
    m_covariateSelected = covariateSelected;
}


void Plot::ResetDataFile()
{
    m_csvRawData.clear();
    m_csvBetas.clear();
    m_csvOmnibus.clear();
    m_csvPostHoc.clear();
}


void Plot::DisplayVTKPlot()
{
    // Resetting scene
    m_view->GetScene()->ClearItems();
    m_view->GetScene()->AddItem( m_chart );

    // Loading data
    QList<float> abscissa;
    QList<QStringList> ordinate;
    int nbrLine;
    QString yMin;
    QString yMax;
    bool dataAvailable = LoadData( abscissa, ordinate, nbrLine, yMin, yMax );

    if( dataAvailable )
    {
        // Creating a table with 2 named arrays
        vtkSmartPointer<vtkTable> table =
                vtkSmartPointer<vtkTable>::New();
        QMap< QString, vtkSmartPointer<vtkFloatArray> > axis;
        AddAxis( axis, table, nbrLine );

        // Adding data
        AddData( table, abscissa, ordinate, nbrLine );

        // Adding plots
        QMap< QString, vtkPlot* > plot;
        AddPlots( plot, table, nbrLine );

        // Setting chart properties
        QString title = "Title Test";
        QString xName = "Arclength";
        QString yName = "AD";
        qDebug() << "yMin: " << yMin.toFloat();
        qDebug() << "yMax: " << yMax.toFloat();
        SetChartProperties( title, xName, yName, yMin, yMax );
    }
}

void Plot::ResetPlot()
{
    m_chart->ClearPlots();
}


/***************************************************************/
/************************ Public  slots ************************/
/***************************************************************/
void Plot::SavePlot()
{
//    QRect rectangle;
//    QPixmap pixmap( rectangle.size() );
//    QRect rectangle;
//    QPixmap pixmap;
//    m_qvtkWidget->render( &pixmap, QPoint(), QRegion( rectangle ) );
//    pixmap.save( m_matlabOutputDir + "/example.png" );
}


/***************************************************************/
/********************** Private functions **********************/
/***************************************************************/
void Plot::SetPlotFiles()
{
    QStringList nameFilterRawData( "*RawData*.csv" );
    m_csvRawData = QDir( m_directory ).entryList( nameFilterRawData );

    QStringList nameFilterBetas( "*Betas*.csv" );
    m_csvBetas = QDir( m_matlabOutputDir ).entryList( nameFilterBetas );

    QStringList nameFilterOmnibus( "*Omnibus*.csv" );
    m_csvOmnibus = QDir( m_matlabOutputDir ).entryList( nameFilterOmnibus );

    QStringList nameFilterPostHoc( "*PostHoc*.csv" );
    m_csvPostHoc = QDir( m_matlabOutputDir ).entryList( nameFilterPostHoc );
}


void Plot::FindyMinMax( QStringList rowData, QString &yMin, QString &yMax )
{
    foreach( QString data, rowData )
    {
        if( yMin.isEmpty() )
        {
            yMin = data;
        }
        else
        {
            if( data < yMin )
            {
                yMin = data;
            }
        }
        if( yMax.isEmpty() )
        {
            yMax = data;
        }
        else
        {
            if( data > yMax )
            {
                yMax = data;
            }
        }
    }
}

bool Plot::LoadData( QList<float> &abscissa, QList<QStringList> &ordinate, int &nbrLine, QString &yMin, QString &yMax )
{
    if( ( m_plotSelected == "Raw Data" ) || ( m_plotSelected == "Raw Stats" ) )
    {
        QStringList adFile = m_csvRawData.filter( "_" + m_outcome + "_", Qt::CaseInsensitive );
        if( !adFile.isEmpty() )
        {
            QList<QStringList> data = m_process.GetDataFromFile( m_directory + "/" + adFile.first() );
            data.removeFirst();
            foreach( QStringList rowData, data )
            {
                abscissa.append( rowData.first().toFloat() );
                rowData.removeAt( 0 );
                FindyMinMax( rowData, yMin, yMax );
                ordinate.append( rowData );
            }
            nbrLine = ordinate.size();
        }
        return !adFile.isEmpty();
    }
    if( m_plotSelected == "Omnibus" )
    {
//        QString AD_MatlabInputFile = "/work/jeantm/Project/FADTTS_Origin/DataShaili/CT_L_Parietal-AllOutput/ad_CT_L_Parietal.txt";
//        QList<QStringList> AD_MatlabInputData = m_process.GetDataFromFile( AD_MatlabInputFile );
//        foreach( QStringList rowData, AD_MatlabInputData )
//        {
//            abscissa.append( rowData.first().toFloat() );
//        }

//        QString AD_BetasFile = "/work/jeantm/Project/FADTTS_Origin/DataShaili/CT_L_Parietal-AllOutput/CT-L-Parietal_AD_betas.csv";
//        ordinate = m_process.GetDataFromFile( AD_BetasFile );
//        yMin = QString::number( -0.05 );
//        yMax = QString::number( 0.31 );
//        nbrLine = ordinate.size();
        return false;
    }
    if( m_plotSelected == "Post-Hoc" )
    {
        return false;
    }
    return false;
}

void Plot::AddAxis( QMap< QString, vtkSmartPointer<vtkFloatArray> > &axis, vtkSmartPointer<vtkTable> &table, int nbrLine )
{
    axis.insert( "Arclength", vtkSmartPointer<vtkFloatArray>::New() );
    for( int i = 0; i < nbrLine; i++ )
    {
        axis.insert( "Subject " + i, vtkSmartPointer<vtkFloatArray>::New() );
    }

    QMap< QString, vtkSmartPointer<vtkFloatArray> >::ConstIterator iterAxis = axis.begin();
    while( iterAxis != axis.end() )
    {
        iterAxis.value()->SetName( iterAxis.key().toUtf8().constData() );
        table->AddColumn( iterAxis.value().GetPointer() );
        ++iterAxis;
    }


//    QMap< QString, vtkSmartPointer<vtkFloatArray> > axis;
//    axis.insert( "Arclength", vtkSmartPointer<vtkFloatArray>::New() );
//    axis.insert( "Intercept", vtkSmartPointer<vtkFloatArray>::New() );
//    axis.insert( "SSRIExposure", vtkSmartPointer<vtkFloatArray>::New() );
//    axis.insert( "Sex", vtkSmartPointer<vtkFloatArray>::New() );
//    axis.insert( "GestAgeBirth", vtkSmartPointer<vtkFloatArray>::New() );
//    axis.insert( "DaysSinceBirth", vtkSmartPointer<vtkFloatArray>::New() );
//    axis.insert( "DTIDirection", vtkSmartPointer<vtkFloatArray>::New() );
//    QMap< QString, vtkSmartPointer<vtkFloatArray> >::ConstIterator iterAxis = axis.begin();
//    while( iterAxis != axis.end() )
//    {
//        iterAxis.value()->SetName( iterAxis.key().toUtf8().constData() );
//        table->AddColumn( iterAxis.value().GetPointer() );
//        ++iterAxis;
//    }
}

void Plot::AddData( vtkSmartPointer<vtkTable> &table, QList<float> abscissa, QList<QStringList> ordinate, int nbrLine )
{
    int nbrPoints = abscissa.size();
    table->SetNumberOfRows( nbrPoints );
    for( int i = 0; i < nbrPoints; i++ )
    {
        table->SetValue( i, 0, abscissa.at( i ) );
        for( int j = 0; j < nbrLine; j++ )
        {
            table->SetValue( i, j + 1, ordinate.at( j ).at( i ).toFloat() );
        }
    }


//    // Adding data
//    int nbrPoints = abscissa.size();
//    table->SetNumberOfRows( nbrPoints );
//    for( int i = 0; i < nbrPoints; i++ )
//    {
//        table->SetValue( i, 0, abscissa.at( i ) );
//        for( int j = 0; j < nbrCovariates; j++ )
//        {
//            table->SetValue( i, j + 1, ordinate.at( j ).at( i ).toFloat() );
//        }
//    }
}

void Plot::AddPlots( QMap< QString, vtkPlot* > &plot, vtkSmartPointer<vtkTable> &table, int nbrLine )
{
    for( int i = 0; i < nbrLine; i++ )
    {
        plot.insert( "Subject " + i, m_chart->AddPlot( vtkChart::LINE ) );
    }
    QMap< QString, vtkPlot* >::ConstIterator iterPlot = plot.begin();
    int i = 1;
    while( iterPlot != plot.end() )
    {
        iterPlot.value()->SetInputData( table.GetPointer(), 0, i );
        iterPlot.value()->SetColor( 0, 0, 255, 255 );
        iterPlot.value()->SetWidth( 1.0 );
        ++iterPlot;
        i++;
    }


//    plot.insert( "Intercept", m_chart->AddPlot( vtkChart::LINE ) );
//    plot.insert( "SSRIExposure", m_chart->AddPlot( vtkChart::LINE ) );
//    plot.insert( "Sex", m_chart->AddPlot( vtkChart::LINE ) );
//    plot.insert( "GestAgeBirth", m_chart->AddPlot( vtkChart::LINE ) );
//    plot.insert( "DaysSinceBirth", m_chart->AddPlot( vtkChart::LINE ) );
//    plot.insert( "DTIDirection", m_chart->AddPlot( vtkChart::LINE ) );
//    QMap< QString, vtkPlot* >::ConstIterator iterPlot = plot.begin();
//    int i = 1;
//    while( iterPlot != plot.end() )
//    {
//        iterPlot.value()->SetInputData( table.GetPointer(), 0, i );
//        vtkPlotPoints::SafeDownCast( iterPlot.value() )->SetMarkerStyle( vtkPlotPoints::CIRCLE );
//        iterPlot.value()->SetColor( 255*0.3*i, 255*0.2*i, 0, 255 );
//        iterPlot.value()->SetWidth( 1.0 );
//        ++iterPlot;
//        i++;
//    }
}

void Plot::SetChartProperties( QString title, QString xName, QString yName, QString yMin, QString yMax )
{
    m_chart->DrawAxesAtOriginOn();
    m_chart->GetAxis( vtkAxis::LEFT )->SetTitle( yName.toStdString() );
    m_chart->GetAxis( vtkAxis::LEFT )->SetRange( yMin.toFloat(), yMax.toFloat() );
    m_chart->GetAxis( vtkAxis::LEFT )->SetBehavior( vtkAxis::FIXED );
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetTitle( xName.toStdString() );
    m_chart->SetTitle( title.toStdString() );


//    m_chart->DrawAxesAtOriginOn();
//    m_chart->GetAxis( vtkAxis::LEFT )->SetTitle( "AD" );
//    m_chart->GetAxis( vtkAxis::LEFT )->SetRange( -0.05, 0.31 );
//    m_chart->GetAxis( vtkAxis::LEFT )->SetBehavior( vtkAxis::FIXED );
//    m_chart->GetAxis( vtkAxis::BOTTOM )->SetTitle( "Arclength" );
//    m_chart->SetShowLegend( true );
//    m_chart->SetTitle( "Title Test" );
}
