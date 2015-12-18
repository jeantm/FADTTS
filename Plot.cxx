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
    m_outcomeSelected = outcome;
}

void Plot::SelectCovariate( QString covariateSelected )
{
    m_covariateSelected = covariateSelected;
}


void Plot::ResetDataFile()
{
    m_csvRawDataFile.clear();
    m_csvBetas.clear();
    m_csvOmnibus.clear();
    m_csvPostHoc.clear();
}


void Plot::IsCovariateBinary()
{
    m_isCovariateBinary = false;
    m_indexColumn = 0;
    QMap< QPair< int, QString >, bool >::ConstIterator iterCovariates = m_covariates.begin();
    while( iterCovariates != m_covariates.end() )
    {
        if( iterCovariates.key().second == m_covariateSelected )
        {
            m_isCovariateBinary = iterCovariates.value();
            m_indexColumn = iterCovariates.key().first;
        }
        ++iterCovariates;
    }
}

void Plot::DisplayVTKPlot()
{
    // Resetting scene
    m_view->GetScene()->ClearItems();
    m_view->GetScene()->AddItem( m_chart );

    // Loading data
    QList<float> abscissa;
    QList<QStringList> ordinate;
    int nbrPlot;
    QString yMin;
    QString yMax;
    bool dataAvailable = LoadData( abscissa, ordinate, nbrPlot, yMin, yMax );

    if( dataAvailable )
    {
        qDebug() << "abscissa.size: " << abscissa.size();
        qDebug() << "ordinate.size: " << ordinate.size() << " x " << ordinate.first().size();
        qDebug() << "nbrPlot: " << nbrPlot;

        // Creating a table with 2 named arrays
        vtkSmartPointer<vtkTable> table =
                vtkSmartPointer<vtkTable>::New();
        QMap< QString, vtkSmartPointer<vtkFloatArray> > axis;
        AddAxis( axis, table, nbrPlot );

        // Adding data
        AddData( table, abscissa, ordinate, nbrPlot );

        // Adding plots
        QMap< QString, vtkPlot* > plot;
        float red = 0;
        float green = 0;
        float blue = 255;
        float opacity = 255;
        AddPlots( plot, table, nbrPlot, red, green, blue, opacity  );

        // Setting chart properties
        QString title = "Title Test";
        QString xName = "Arclength";
        QString yName = "AD";
        yMin.toFloat();
        yMax.toFloat();
        SetChartProperties( title, xName, yName, yMin, yMax);
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
void Plot::ProcessCovariates()
{
    QStringList firstRaw = m_csvRawData.value( 4 ).first();
    QStringList firstDataRaw = m_csvRawData.value( 4 ).at( 1 );
    QStringList covariates;
    qDebug() << "firstRaw: " << firstRaw;
    qDebug() << "firstDataRaw: " << firstDataRaw;

    for( int i = 1; i < firstRaw.size(); i++ )
    {
        QPair< int, QString > currentPair;
        currentPair.first = i ;
        currentPair.second = firstRaw.at( i );
        bool currentBool = ( firstDataRaw .at( i ).toInt() == 0 ) | ( firstDataRaw.at( i ).toInt() == 1 );
        m_covariates.insert( currentPair, currentBool );

        covariates.append( currentPair.second );
    }
    emit CovariateUsed( covariates );
    qDebug() << "m_covariates: " << m_covariates;
}

void Plot::SetRawData( QStringList rawDataFileNames )
{
    foreach( QString path, rawDataFileNames )
    {
        QString currentPath = m_directory + "/" + path;
        int index;
        if( path.contains( "_ad_", Qt::CaseInsensitive ) )
        {
            index = 0;
        }
        if( path.contains( "_md_", Qt::CaseInsensitive ) )
        {
            index = 1;
        }
        if( path.contains( "_rd_", Qt::CaseInsensitive ) )
        {
            index = 2;
        }
        if( path.contains( "_fa_", Qt::CaseInsensitive ) )
        {
            index = 3;
        }
        if( path.contains( "_comp_", Qt::CaseInsensitive ) )
        {
            index = 4;
        }
        m_csvRawData.insert( index, m_process.GetDataFromFile( currentPath ) );
    }
    ProcessCovariates();
}

void Plot::SetRawDataFiles()
{
    QStringList nameFilterRawData( "*RawData*.csv" );
    m_csvRawDataFile = QDir( m_directory ).entryList( nameFilterRawData );
    SetRawData( m_csvRawDataFile );
}

void Plot::SetPlotFiles()
{
    SetRawDataFiles();

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


void Plot::LoadRawData( QList<float> &abscissa, QList<QStringList> &ordinate, int &nbrPlot, QString &yMin, QString &yMax )
{
    QStringList rawDataFile = m_csvRawDataFile.filter( "_" + m_outcomeSelected + "_", Qt::CaseInsensitive );
    if( !rawDataFile.isEmpty() )
    {
        QList<QStringList> data = m_process.GetDataFromFile( m_directory + "/" + rawDataFile.first() );
        data.removeFirst();
        foreach( QStringList rowData, data )
        {
            abscissa.append( rowData.first().toFloat() );
            rowData.removeFirst();
            FindyMinMax( rowData, yMin, yMax );
            ordinate.append( rowData );
        }
    }
}


void Plot::SeparateData( QList<QStringList> ordinate, QList<QStringList> &temp0Bin, QList<QStringList> &temp1Bin)
{
    for( int i = 0; i < ordinate.size(); i++ )
    {
        QStringList temp0Subj;
        QStringList temp1Subj;
        for( int j = 0; j < ordinate.first().size(); j++ )
        {
            if( m_isCovariateBinary && ( m_csvRawData.value( 4 ).at( j ).at( m_indexColumn ).toInt() == 0 ) )
            {
                temp0Subj.append( ordinate.at( i ).at( j ) );
            }
            else
            {
                temp1Subj.append( ordinate.at( i ).at( j ) );
            }
        }
        if( !temp0Subj.isEmpty() )
        {
            temp0Bin.append( temp0Subj );
        }
        if( !temp1Subj.isEmpty() )
        {
            temp1Bin.append( temp1Subj );
        }
    }
}

void Plot::GetMeanAndStdDv( QList<QStringList> tempBin, QStringList &tempMean, QStringList &tempStdDv )
{
    int nbrPts = tempBin.size();
    int nbrSubjects = tempBin.first().size();
    for( int i = 0; i < nbrPts; i++ )
    {
        float currentMean = 0;
        for( int j = 0; j < nbrSubjects; j++ )
        {
            currentMean += tempBin.at( i ).at( j ).toFloat();
        }
        currentMean = currentMean / nbrSubjects;
        tempMean.append( QString::number( currentMean / nbrSubjects ) );

        float currentStdDv = 0;
        for( int j = 0; j < nbrSubjects; j++ )
        {
            currentStdDv += std::sqrt( std::pow( currentMean - tempBin.at( i ).at( j ).toFloat(), currentMean - tempBin.at( i ).at( j ).toFloat() ) );
        }
        tempStdDv.append( QString::number( currentMean / nbrSubjects ) );
    }
}

void Plot::ProcessRawStats( QList<QStringList> tempBin, QStringList &tempBinMean, QStringList &tempBinUp, QStringList &tempBinDown )
{
    QStringList tempBinStdDv;
    if( !tempBin.isEmpty() )
    {
        GetMeanAndStdDv( tempBin, tempBinMean, tempBinStdDv );
        for( int i = 0; i < tempBin.size(); i++ )
        {
            tempBinUp.append( QString::number( tempBinMean.at( i ).toFloat() + tempBinStdDv.at( i ).toFloat() ) );
            tempBinDown.append( QString::number( tempBinMean.at( i ).toFloat() - tempBinStdDv.at( i ).toFloat() ) );
        }
    }
}

void Plot::SetRawStatsData( QList<QStringList> &ordinate, QStringList &temp0BinUp, QStringList &temp0BinMean, QStringList &temp0BinDown,
                            QStringList &temp1BinUp, QStringList &temp1BinMean, QStringList &temp1BinDown )
{
    int nbrPts = ordinate.size();
    ordinate.clear();
    for( int i = 0; i < nbrPts; i++ )
    {
        QStringList row;
        if( !temp0BinUp.isEmpty() )
        {
            row.append( QString::number( temp0BinUp.at( i ).toFloat() ) );
            row.append( QString::number( temp0BinMean.at( i ).toFloat() ) );
            row.append( QString::number( temp0BinDown.at( i ).toFloat() ) );
        }
        row.append( QString::number( temp1BinUp.at( i ).toFloat() ) );
        row.append( QString::number( temp1BinMean.at( i ).toFloat() ) );
        row.append( QString::number( temp1BinDown.at( i ).toFloat() ) );
        ordinate.append( row );
    }
}

void Plot::LoadRawStats( QList<QStringList> &ordinate, QString &yMin, QString &yMax )
{
    if( !ordinate.isEmpty() )
    {
        QList<QStringList> temp0Bin;
        QList<QStringList> temp1Bin;
        SeparateData( ordinate, temp0Bin, temp1Bin);

        QStringList temp0BinUp;
        QStringList temp0BinMean;
        QStringList temp0BinDown;
        ProcessRawStats( temp0Bin, temp0BinMean, temp0BinUp, temp0BinDown);

        QStringList temp1BinUp;
        QStringList temp1BinMean;
        QStringList temp1BinDown;
        ProcessRawStats( temp1Bin, temp1BinMean, temp1BinUp, temp1BinDown);

        SetRawStatsData( ordinate, temp0BinUp, temp0BinMean, temp0BinDown, temp1BinUp, temp1BinMean, temp1BinDown );

        yMin.clear();
        yMax.clear();
        foreach ( QStringList rowData , ordinate)
        {
            FindyMinMax( rowData, yMin, yMax );
        }

        qDebug() << "new ordinate:" << ordinate.size() << "x" << ordinate.first().size();
    }
}

bool Plot::LoadData( QList<float> &abscissa, QList<QStringList> &ordinate, int &nbrPlot, QString &yMin, QString &yMax )
{
    nbrPlot = 0;
    if( ( m_plotSelected == "Raw Data" ) || ( m_plotSelected == "Raw Stats" ) )
    {
        LoadRawData( abscissa, ordinate, nbrPlot, yMin, yMax );

        if( m_plotSelected == "Raw Stats" )
        {
            LoadRawStats( ordinate, yMin, yMax );
        }
        nbrPlot = ordinate.first().size();

        return !( abscissa.isEmpty() | ordinate.isEmpty() );
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
//        nbrPlot = ordinate.size();
        return false;
    }
    if( m_plotSelected == "Post-Hoc" )
    {
        return false;
    }
    return false;
}


void Plot::AddAxis( QMap< QString, vtkSmartPointer<vtkFloatArray> > &axis, vtkSmartPointer<vtkTable> &table, int nbrPlot )
{
    axis.insert( "Arclength", vtkSmartPointer<vtkFloatArray>::New() );
    for( int i = 0; i < nbrPlot; i++ )
    {
        axis.insert( "Subject " + QString::number( i + 1 ), vtkSmartPointer<vtkFloatArray>::New() );
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

void Plot::AddData( vtkSmartPointer<vtkTable> &table, QList<float> abscissa, QList<QStringList> ordinate, int nbrPlot )
{
    int nbrPoints = abscissa.size();
    table->SetNumberOfRows( nbrPoints );
    for( int i = 0; i < nbrPoints; i++ )
    {
        table->SetValue( i, 0, abscissa.at( i ) );
        for( int j = 0; j < nbrPlot; j++ )
        {
            table->SetValue( i, j + 1, ordinate.at( i ).at( j ).toFloat() );
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

void Plot::AddPlots( QMap< QString, vtkPlot* > &plot, vtkSmartPointer<vtkTable> &table, int nbrPlot, float red, float green, float blue, float opacity )
{
    for( int i = 0; i < nbrPlot; i++ )
    {
        plot.insert( "Subject " + QString::number( i + 1 ), m_chart->AddPlot( vtkChart::LINE ) );
    }

    QMap< QString, vtkPlot* >::ConstIterator iterPlot = plot.begin();
    int i = 1;
    while( iterPlot != plot.end() )
    {
        iterPlot.value()->SetInputData( table.GetPointer(), 0, i );

        if( m_isCovariateBinary && ( m_csvRawData.value( 4 ).at( i ).at( m_indexColumn ).toInt() == 0 ) )
        {
            iterPlot.value()->SetColor( 0, 0, 255, 255 );
        }
        else
        {
            iterPlot.value()->SetColor( 255, 0, 0, 255 );
        }
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


