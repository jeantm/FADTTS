#include "Plot.h"
#include <QDebug>

/***************************************************************/
/********************** Public functions ***********************/
/***************************************************************/
Plot::Plot( QObject *parent ) :
    QObject( parent )
{
}

void Plot::SetQVTKWidget( QVTKWidget *qvtkWidget )
{
    m_qvtkWidget = qvtkWidget;
    m_view = vtkSmartPointer<vtkContextView>::New();
    m_view->SetInteractor( m_qvtkWidget->GetInteractor() );
    m_qvtkWidget->SetRenderWindow( m_view->GetRenderWindow() );
    m_chart = vtkSmartPointer<vtkChartXY>::New();
//    m_table = vtkSmartPointer<vtkTable>::New();
//    m_tableSigBetas = vtkSmartPointer<vtkTable>::New();
}

void Plot::InitPlot( QString directory, QString fibername )
{
    m_directory = directory;
    m_matlabOutputDir = directory + "/MatlabOutputs";
    m_fibername = fibername;
    m_alpha = 0.05;

    GetRawDataFiles();
    GetBetaFiles();
    GetOmnibusLpvalueFiles();
    GetOmnibusFDRLpvalueFiles();
    GetPostHocFDRLpvalueFiles();

    SetProperties();
    SetCovariates();
    SetAbscissa();
}

void Plot::ResetDataFile()
{
    m_csvRawDataFile.clear();
    m_csvBetaFile.clear();
    m_csvOmnibusLpvalueFile.clear();
    m_csvOmnibusFDRLpvalueFile.clear();
    m_csvPostHocFDRLpvalueFile.clear();
}

void Plot::ResetPlot()
{
    m_chart->ClearPlots();
}


void Plot::SelectPlot( QString plotSelected )
{
    m_plotSelected = plotSelected;
    if( m_plotSelected == "Raw Data" || m_plotSelected == "Raw Stats" )
    {
        emit CovariatesUsed( m_binaryCovariates );
    }
    else
    {
        if( m_plotSelected == "Raw Betas by Properties" || m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
                m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
        {
            emit CovariatesUsed( m_allCovariates );
        }
        else
        {
            emit CovariatesUsed( m_covariatesNoIntercept );
        }
    }
}

void Plot::SelectProperty( QString property )
{
    m_propertySelected = property;
}

void Plot::SelectCovariate( QString covariateSelected )
{
    m_covariateSelected = covariateSelected;
}


void Plot::SetTitle( QString title )
{
    m_title = title;
}

void Plot::SetxName( QString xName )
{
    m_xName = xName;
}

void Plot::SetyName( QString yName )
{
    m_yName = yName;
}

void Plot::SetYMin( bool yMinChecked, double yMin )
{
    m_yMinChecked = yMinChecked;
    m_yMinGiven = yMin;
}

void Plot::SetYMax( bool yMaxChecked, double yMax )
{
    m_yMaxChecked = yMaxChecked;
    m_yMaxGiven = yMax;
}


void Plot::DisplayVTKPlot()
{
    // Resetting scene
    m_view->GetScene()->ClearItems();
    m_view->GetScene()->AddItem( m_chart );

    // Loading data
    ResetLoadData();
    bool dataAvailable = LoadData();

    if( dataAvailable )
    {
        m_nbrPlot = m_ordinate.size();
        qDebug() << endl << "m_ordinate.size: " << m_ordinate.size() << " x " << m_ordinate.first().size();
        qDebug() << "m_nbrPlot: " << m_nbrPlot;
        qDebug() << "m_nbrPoint: " << m_nbrPoint;
        qDebug() << "m_alpha: " << m_alpha;

        // Creating a table with 2 named arrays
        qDebug() << endl << "Adding entry";
        vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();
        AddEntry( table );

        // Adding data
        qDebug() << endl << "Adding data";
        SetData( table );

        // Adding plots
        qDebug() << endl << "Adding plots";
        AddLines( table );

        // Setting chart properties
        qDebug() << endl << "Setting chart properties";
        SetChartProperties();
    }
    emit SetEditTab( dataAvailable );
}


void Plot::UpdatePlotTitle( QString titleName, bool isBold, bool isItalic, double titleSize )
{
//    qDebug() << "UpdatePlotTitle";
//    qDebug() << "titleName: " << titleName;
//    qDebug() << "isBold: " << isBold;
//    qDebug() << "isItalic: " << isItalic;
//    qDebug() << "titleSize: " << titleSize;
//    m_chart->SetTitle( titleName.toStdString() );
//    m_chart->GetTitleProperties()->SetBold( isBold );
//    m_chart->GetTitleProperties()->SetItalic( isItalic );
//    m_chart->GetTitleProperties()->SetFontSize( titleSize );

////    m_chart->Update();
////    m_view->GetScene();
////    m_view->Update();
////    m_chart->GetTitleProperties()->SetJustification();
}

void Plot::UpdatePlotAxis( QString xName, QString yName, bool isBold, bool isItalic )
{
//    qDebug() << "UpdatePlotAxis";
//    m_chart->GetAxis( vtkAxis::BOTTOM )->SetTitle( xName.toStdString() );
//    m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->SetBold( isBold );
//    m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->SetItalic( isItalic );

//    m_chart->GetAxis( vtkAxis::LEFT )->SetTitle( yName.toStdString() );
//    m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->SetBold( isBold );
//    m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->SetItalic( isItalic );
}

void Plot::UpdatePlotLegend( QString legendPosition, QString legendSelected, QString legendName, QString legendColor,
                             double lineThickness, QString markerType, double markerThickness )
{
//    qDebug() << "UpdatePlotLegend";
//    m_chart->GetLegend();
}


/***************************************************************/
/************************ Public  slots ************************/
/***************************************************************/
void Plot::SavePlot()
{
    QString fileName = m_matlabOutputDir + "/" +  m_fibername + "_";
    if( m_plotSelected == "Raw Data" )
    {
        fileName += m_propertySelected + "_" + m_covariateSelected + "_" + m_plotSelected.split( " " ).join( "_" );
    }
    if( m_plotSelected == "Raw Stats" )
    {
        fileName += m_propertySelected + "_" + m_covariateSelected + "_Average_StdDeviation";
    }
    if( m_plotSelected == "Raw Betas by Properties" )
    {
        fileName += m_propertySelected + "_Betas";
    }
    if( m_plotSelected == "Raw Betas by Covariates" )
    {
        fileName += m_covariateSelected + "_Betas";
    }
    if( m_plotSelected == "Omnibus Local pvalues" || m_plotSelected == "Omnibus FDR Local pvalues" )
    {
        fileName += m_plotSelected.split( " " ).join( "_" );
    }
    if( m_plotSelected == "Post-Hoc FDR Local pvalues" )
    {
        fileName += "PostHoc_FDR_Local_pvalues";
    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Properties" )
    {
        fileName += m_propertySelected + "_Omnibus_FDR_SigBetas";
    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Covariates" )
    {
        fileName += m_covariateSelected + "_Omnibus_FDR_SigBetas";
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        fileName += m_propertySelected + "_PostHoc_FDR_SigBetas";
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        fileName += m_covariateSelected + "_PostHoc_FDR_SigBetas";
    }
    fileName += ".png";

    QString filePath = QFileDialog::getSaveFileName( m_qvtkWidget, tr( "Save plot as ..." ), fileName, tr( ".png ( *.png ) ;; .*( * )" ) );
    if( !filePath.isEmpty() )
    {
        vtkSmartPointer<vtkWindowToImageFilter> imgFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
        imgFilter->SetInput( m_view->GetRenderWindow() );
        imgFilter->Update();

        vtkSmartPointer<vtkPNGWriter> writer = vtkSmartPointer<vtkPNGWriter>::New();
        writer->SetFileName( filePath.toStdString().c_str() );
        writer->SetInputConnection( imgFilter->GetOutputPort() );
        writer->Write();
    }
}

void Plot::AlphaValueChanged( const double &newAlpha )
{
    m_alpha = newAlpha;
}

void Plot::PlotSettingCheckState( const bool &plotSetting )
{
//    m_plotSetting = plotSetting;
}


/***************************************************************/
/********************** Private functions **********************/
/***************************************************************/
void Plot::SortFiles( QString directory, QStringList files, QMap< QString, QList<QList<double> > > &data )
{
    foreach( QString path, files )
    {
        QString currentPath = directory + "/" + path;
        QString index;
        if( path.contains( "_ad_", Qt::CaseInsensitive ) )
        {
            index = "AD";
        }
        if( path.contains( "_md_", Qt::CaseInsensitive ) )
        {
            index = "MD";
        }
        if( path.contains( "_rd_", Qt::CaseInsensitive ) )
        {
            index = "RD";
        }
        if( path.contains( "_fa_", Qt::CaseInsensitive ) )
        {
            index = "FA";
        }
        if( path.contains( "_comp_", Qt::CaseInsensitive ) )
        {
            m_csvCovariate = m_process.GetDataFromFile( currentPath );
        }

        if( !index.isNull() )
        {
            data.insert( index, ToDouble( m_process.GetDataFromFile( currentPath ) ) );
        }
    }
}

void Plot::TransposeData( QList < QList < double > > &data, int iMin, int jMin )
{
    QList < QList < double > > dataTransposed;
    for( int j = jMin; j < data.first().size(); j++ )
    {
        QList < double > rowData;
        for( int i = iMin; i < data.size(); i++ )
        {
            rowData.append( data.at( i ).at( j ) );
        }
        dataTransposed.append( rowData );
    }
    data.clear();
    data = dataTransposed;
}

void Plot::Transpose( QMap< QString, QList<QList<double> > > &data, int iMin, int jMin )
{
    QMap< QString, QList < QList < double > > >::Iterator iterData = data.begin();
    while( iterData != data.end() )
    {
        TransposeData( iterData.value(), iMin, jMin );
        ++iterData;
    }
}

QList < double > Plot::DataToDouble( QStringList rowData )
{
    QList < double > newRowData;
    foreach ( QString data, rowData )
    {
        newRowData.append( data.toDouble() );
    }
    return newRowData;
}

QList < QList < double > > Plot::ToDouble( QList < QStringList > data )
{
    QList < QList < double > > newData;
    foreach ( QStringList rowData, data )
    {
        newData.append( DataToDouble( rowData ) );
    }
    return newData;
}


void Plot::SetRawData( QStringList files, QMap< QString, QList<QList<double> > > &data )
{
    SortFiles( m_directory, files, data );
    Transpose( data, 1, 0 );
}

void Plot::SetBeta( QStringList files, QMap< QString, QList < QList < double > > > &data )
{
    SortFiles( m_matlabOutputDir, files, data );
}

void Plot::SetOmnibusLpvalue( QStringList files, QList < QList < double > > &OmnibusLpvaluesData )
{
    QString currentPath = m_matlabOutputDir + "/" + files.first();
    QList < QList < double > > dataTempo = ToDouble( m_process.GetDataFromFile( currentPath ) );
    TransposeData( dataTempo, 0, 0 );
    OmnibusLpvaluesData = dataTempo;
}

void Plot::SetPostHocFDRLpvalue( QStringList files, QMap< QString, QList < QList < double > > > &data )
{
    SortFiles( m_matlabOutputDir, files, data );
    Transpose( data, 0, 0 );
}


void Plot::GetRawDataFiles()
{
    QStringList nameFilterRawData( "*RawData.csv" );
    m_csvRawDataFile = QDir( m_directory ).entryList( nameFilterRawData );
    SetRawData( m_csvRawDataFile, m_csvRawData );

    QMap< QString, QList < QList < double > > >::ConstIterator iterRawData = m_csvRawData.begin();
    while( iterRawData != m_csvRawData.end() )
    {
        ++iterRawData;
    }
}

void Plot::GetBetaFiles()
{
    QStringList nameFilterBeta( "*Betas.csv" );
    m_csvBetaFile = QDir( m_matlabOutputDir ).entryList( nameFilterBeta );
    SetBeta( m_csvBetaFile, m_csvBeta );

    QMap< QString, QList < QList < double > > >::ConstIterator iterRawData = m_csvBeta.begin();
    while( iterRawData != m_csvBeta.end() )
    {
        ++iterRawData;
    }
}

void Plot::GetOmnibusLpvalueFiles()
{
    QStringList nameFilterOmnibusLpvalue( "*Omnibus_Local_pvalues.csv" );
    m_csvOmnibusLpvalueFile = QDir( m_matlabOutputDir ).entryList( nameFilterOmnibusLpvalue );
    SetOmnibusLpvalue( m_csvOmnibusLpvalueFile, m_csvOmnibusLpvalue );
}

void Plot::GetOmnibusFDRLpvalueFiles()
{
    QStringList nameFilterOmnibusFDRLpvalue( "*Omnibus_FDR_Local_pvalues.csv" );
    m_csvOmnibusFDRLpvalueFile = QDir( m_matlabOutputDir ).entryList( nameFilterOmnibusFDRLpvalue );
    SetOmnibusLpvalue( m_csvOmnibusFDRLpvalueFile, m_csvOmnibusFDRLpvalue );
}

void Plot::GetPostHocFDRLpvalueFiles()
{
    QStringList nameFilterPostHocFDRLpvalue( "*PostHoc_FDR_Local_pvalues.csv" );
    m_csvPostHocFDRLpvalueFile = QDir( m_matlabOutputDir ).entryList( nameFilterPostHocFDRLpvalue );
    SetPostHocFDRLpvalue( m_csvPostHocFDRLpvalueFile, m_csvPostHocFDRLpvalue );

    QMap< QString, QList < QList < double > > >::ConstIterator iterPostHocFDRLpvalue = m_csvPostHocFDRLpvalue.begin();
    while( iterPostHocFDRLpvalue != m_csvPostHocFDRLpvalue.end() )
    {
        ++iterPostHocFDRLpvalue;
    }
}


void Plot::SetProperties()
{
    QMap< int, QString > tempoProperty;
    foreach( QString path, m_csvRawDataFile )
    {
        if( path.contains( "_ad_", Qt::CaseInsensitive ) )
        {
            tempoProperty.insert( 0, "AD" );
        }
        if( path.contains( "_md_", Qt::CaseInsensitive ) )
        {
            tempoProperty.insert( 1, "RD" );
        }
        if( path.contains( "_rd_", Qt::CaseInsensitive ) )
        {
            tempoProperty.insert( 2, "MD" );
        }
        if( path.contains( "_fa_", Qt::CaseInsensitive ) )
        {
            tempoProperty.insert( 3, "FA" );
        }
    }
    m_propertiesUsed = tempoProperty.values();
    emit PropertiesUsed( m_propertiesUsed );
    qDebug() << endl << "m_propertiesUsed: " << m_propertiesUsed;
}

void Plot::SetCovariates()
{
    QStringList firstRaw = m_csvCovariate.first();
    QStringList firstDataRaw = m_csvCovariate.at( 1 );

    m_allCovariates.insert( 0, "Intercept" );
    for( int i = 1; i < firstRaw.size(); i++ )
    {
        m_allCovariates.insert( i, firstRaw.at( i ) );
        m_covariatesNoIntercept.insert( i, firstRaw.at( i ) );
        if( ( firstDataRaw .at( i ).toInt() == 0 ) || ( firstDataRaw.at( i ).toInt() == 1 ) )
        {
            m_binaryCovariates.insert( i, firstRaw.at( i ) );
        }
    }
    qDebug() << endl << "m_allCovariates: " << m_allCovariates;
    qDebug() << endl << "m_covariatesNoIntercept: " << m_covariatesNoIntercept;
    qDebug() << "m_binaryCovariates: " << m_binaryCovariates;
}

void Plot::SetAbscissa()
{
    m_abscissa.clear();
    m_abscissa.append( m_csvRawData.first().first() );
    m_nbrPoint = m_abscissa.size();
}


void Plot::ResetLoadData()
{
    m_yMin = 100;
    m_yMax = -100;
    m_ordinate.clear();
    m_line.clear();
//    m_table->SetNumberOfRows( 0 );
//    m_chart->ClearPlots();
//    m_tableSigBetas->SetNumberOfRows( 0 );
}


void Plot::SeparateBinary( QList< QList < double > > &temp0Bin, QList< QList < double > > &temp1Bin )
{
    int indexCovariate = m_binaryCovariates.key( m_covariateSelected );
    for( int i = 0; i < m_ordinate.size(); i++ )
    {
        if( m_csvCovariate.at( i + 1 ).at( indexCovariate ).toInt() == 0 )
        {
            temp0Bin.append( m_ordinate.at( i ) );
        }
        else
        {
            temp1Bin.append( m_ordinate.at( i ) );
        }
    }
}

void Plot::GetMeanAndStdDv( QList< QList < double > > tempBin, QList < double > &tempMean, QList < double > &tempStdDv )
{
    int nbrSubjects = tempBin.size();
    for( int i = 0; i < m_nbrPoint; i++ )
    {
        float currentMean = 0;
        for( int j = 0; j < nbrSubjects; j++ )
        {
            currentMean += tempBin.at( j ).at( i );
        }
        currentMean = currentMean / nbrSubjects;
        tempMean.append( currentMean );

        float currentStdDv = 0;
        for( int j = 0; j < nbrSubjects; j++ )
        {
            currentStdDv += std::sqrt( std::pow( currentMean - tempBin.at( j ).at( i ), 2 ) );
        }
        tempStdDv.append( currentStdDv / nbrSubjects );
    }
}

void Plot::ProcessRawStats( QList< QList < double > > tempBin, QList < double > &tempBinMean, QList < double > &tempBinUp, QList < double > &tempBinDown )
{
    QList < double > tempBinStdDv;
    GetMeanAndStdDv( tempBin, tempBinMean, tempBinStdDv );
    for( int i = 0; i < m_nbrPoint; i++ )
    {
        tempBinUp.append( tempBinMean.at( i ) + tempBinStdDv.at( i ) );
        tempBinDown.append( tempBinMean.at( i ) - tempBinStdDv.at( i ) );
    }
}

QList< QList < double > > Plot::Tolog10( QList< QList < double > > data )
{
    QList< QList < double > > dataToLog10;
    foreach( QList < double > rowData, data )
    {
        QList < double > tempoRowData;
        foreach( double dbl, rowData )
        {
            tempoRowData.append( - std::log10( dbl ) );
        }
        dataToLog10.append( tempoRowData );
    }
    return dataToLog10;
}


QList< QList < double > > Plot::LoadRawData()
{
    QList< QList < double > > ordinate = m_csvRawData.value( m_propertySelected );
    ordinate.removeFirst();
    return ordinate;
}

QList< QList < double > > Plot::LoadRawStats()
{
    QList< QList < double > > temp0Bin;
    QList< QList < double > > temp1Bin;
    SeparateBinary( temp0Bin, temp1Bin );

    QList < double > temp0BinUp;
    QList < double > temp0BinMean;
    QList < double > temp0BinDown;
    ProcessRawStats( temp0Bin, temp0BinMean, temp0BinUp, temp0BinDown );

    QList < double > temp1BinUp;
    QList < double > temp1BinMean;
    QList < double > temp1BinDown;
    ProcessRawStats( temp1Bin, temp1BinMean, temp1BinUp, temp1BinDown );

    QList< QList < double > > ordinate;
    ordinate.append( temp0BinUp );
    ordinate.append( temp0BinMean );
    ordinate.append( temp0BinDown );
    ordinate.append( temp1BinUp );
    ordinate.append( temp1BinMean );
    ordinate.append( temp1BinDown);

    m_ordinate.clear();

    return ordinate;
}

QList< QList < double > > Plot::LoadBetas()
{
    QList< QList < double > > ordinate;
    ordinate = m_csvBeta.value( m_propertySelected );
    return ordinate;
}

QList< QList < double > > Plot::LoadBetaByCovariate()
{
    QList< QList < double > > ordinate;
    foreach( QString property, m_propertiesUsed )
    {
        ordinate.append( m_csvBeta.value( property ).at( m_allCovariates.key( m_covariateSelected ) ) );
    }
    return ordinate;
}

QList< QList < double > > Plot::LoadOmnibusLpvalues()
{
    QList< QList < double > > ordinate;
    ordinate = Tolog10( m_csvOmnibusLpvalue );
    return ordinate;
}

QList< QList < double > > Plot::LoadOmnibusFDRLpvalues()
{
    QList< QList < double > > ordinate;
    ordinate = Tolog10( m_csvOmnibusFDRLpvalue );
    return ordinate;
}

QList< QList < double > > Plot::LoadPostHocFDRLpvalues()
{
    QList< QList < double > > ordinate;
    foreach( QString property, m_propertiesUsed )
    {
        ordinate.append( m_csvPostHocFDRLpvalue.value( property ).at( m_covariatesNoIntercept.key( m_covariateSelected ) - 1 ) );
    }
    return Tolog10( ordinate );
}

bool Plot::LoadData()
{
    if( ( m_plotSelected == "Raw Data" ) || ( m_plotSelected == "Raw Stats" ) )
    {
        if( !m_propertySelected.isEmpty() && !m_covariateSelected.isEmpty() )
        {
            m_ordinate = LoadRawData();

            if( m_plotSelected == "Raw Stats" )
            {
                m_ordinate = LoadRawStats();
            }
        }
    }
    if( m_plotSelected == "Raw Betas by Properties" )
    {
        if( !m_propertySelected.isEmpty() )
        {
            m_ordinate = LoadBetas();
        }
    }
    if( m_plotSelected == "Raw Betas by Covariates" )
    {
        if( !m_covariateSelected.isEmpty() )
        {
            m_ordinate = LoadBetaByCovariate();
        }
    }
    if( m_plotSelected == "Omnibus Local pvalues" )
    {
        m_ordinate = LoadOmnibusLpvalues();
    }
    if( m_plotSelected == "Omnibus FDR Local pvalues" )
    {
        m_ordinate = LoadOmnibusFDRLpvalues();
    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Properties" )
    {
        if( !m_propertySelected.isEmpty() )
        {
            m_ordinate = LoadBetas();
        }
    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Covariates" )
    {
        if( !m_covariateSelected.isEmpty() )
        {
            m_ordinate = LoadBetaByCovariate();
        }
    }
    if( m_plotSelected == "Post-Hoc FDR Local pvalues" )
    {
        if( !m_covariateSelected.isEmpty() )
        {
            m_ordinate = LoadPostHocFDRLpvalues();
        }
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        if( !m_propertySelected.isEmpty() )
        {
            m_ordinate = LoadBetas();
        }
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        if( !m_covariateSelected.isEmpty() )
        {
            m_ordinate = LoadBetaByCovariate();
        }
    }
    return !( m_abscissa.isEmpty() | m_ordinate.isEmpty() );
}


void Plot::AddEntryRawData( vtkSmartPointer<vtkTable> &table )
{
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkSmartPointer<vtkFloatArray> currentEntry = vtkSmartPointer<vtkFloatArray>::New();
        currentEntry->SetName( QString( "Subject " + QString::number( i + 1 ) ).toStdString().c_str() );
        table->AddColumn( currentEntry );
    }
}

void Plot::AddEntryRawStats( vtkSmartPointer<vtkTable> &table )
{
    vtkSmartPointer<vtkFloatArray> std0up = vtkSmartPointer<vtkFloatArray>::New();
    std0up->SetName( QString( m_covariateSelected + " = 0 std+" ).toStdString().c_str() );
    table->AddColumn( std0up );
    vtkSmartPointer<vtkFloatArray> mean0 = vtkSmartPointer<vtkFloatArray>::New();
    mean0->SetName( QString( m_covariateSelected + " = 0 mean" ).toStdString().c_str() );
    table->AddColumn( mean0 );
    vtkSmartPointer<vtkFloatArray> std0down = vtkSmartPointer<vtkFloatArray>::New();
    std0down->SetName( QString( m_covariateSelected + " = 0 std-" ).toStdString().c_str() );
    table->AddColumn( std0down );
    vtkSmartPointer<vtkFloatArray> std1up = vtkSmartPointer<vtkFloatArray>::New();
    std1up->SetName( QString( m_covariateSelected + " = 1 std+" ).toStdString().c_str() );
    table->AddColumn( std1up );
    vtkSmartPointer<vtkFloatArray> mean1 = vtkSmartPointer<vtkFloatArray>::New();
    mean1->SetName( QString( m_covariateSelected + " = 1 mean" ).toStdString().c_str() );
    table->AddColumn( mean1 );
    vtkSmartPointer<vtkFloatArray> std1down = vtkSmartPointer<vtkFloatArray>::New();
    std1down->SetName( QString( m_covariateSelected + " = 1 std-" ).toStdString().c_str() );
    table->AddColumn( std1down );
}

void Plot::AddEntryByProperties( vtkSmartPointer<vtkTable> &table, QMap< int, QString > covariates, int iShift )
{
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkSmartPointer<vtkFloatArray> currentEntry = vtkSmartPointer<vtkFloatArray>::New();
        currentEntry->SetName( covariates.value( i + iShift ).toStdString().c_str() );
        table->AddColumn( currentEntry );
    }
}

void Plot::AddEntryByCovariates( vtkSmartPointer<vtkTable> &table )
{
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkSmartPointer<vtkFloatArray> currentEntry = vtkSmartPointer<vtkFloatArray>::New();
        currentEntry->SetName( m_propertiesUsed.value( i ).toStdString().c_str() );
        table->AddColumn( currentEntry );
    }
}

void Plot::AddEntry( vtkSmartPointer<vtkTable> &table )
{
    vtkSmartPointer<vtkFloatArray> abscissa = vtkSmartPointer<vtkFloatArray>::New();
    abscissa->SetName( "Arc Length" );
    table->AddColumn( abscissa );

    if( m_plotSelected == "Raw Data" )
    {
        AddEntryRawData( table );
    }
    if( m_plotSelected == "Raw Stats" )
    {
        AddEntryRawStats( table );
    }
    if( m_plotSelected == "Raw Betas by Properties" || m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        AddEntryByProperties( table, m_allCovariates, 0 );
    }
    if( m_plotSelected == "Omnibus Local pvalues" || m_plotSelected == "Omnibus FDR Local pvalues" )
    {
        AddEntryByProperties( table, m_covariatesNoIntercept, 1 );
    }
    if( m_plotSelected == "Raw Betas by Covariates" ||
            m_plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Local pvalues" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        AddEntryByCovariates( table );
    }
}


void Plot::SetData( vtkSmartPointer<vtkTable> &table )
{
    table->SetNumberOfRows( m_nbrPoint );
    for( int i = 0; i < m_nbrPoint; i++ )
    {
        // Adding abscissa
        table->SetValue( i, 0, m_abscissa.at( i ) );

        // Adding data
        for( int j = 0; j < m_nbrPlot; j++ )
        {
            table->SetValue( i, j + 1, m_ordinate.at( j ).at( i ) );
        }
    }
}


void Plot::InitLines()
{
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        m_line.insert( i, m_chart->AddPlot( vtkChart::LINE ) );
    }
}

void Plot::AddLineSigBetas( vtkSmartPointer<vtkTable> table, bool betaDisplayedByProperties, bool isOmnibus, int i, int incrementColor )
{
    vtkSmartPointer<vtkTable> tableSigBetas = vtkSmartPointer<vtkTable>::New();
    vtkSmartPointer<vtkFloatArray> tempoAbscissaSigBeta = vtkSmartPointer<vtkFloatArray>::New();
    tempoAbscissaSigBeta->SetNumberOfComponents( 1 );
    tempoAbscissaSigBeta->SetName( "Abscissa SigBeta" );
    vtkSmartPointer<vtkFloatArray> tempoDataSigBeta = vtkSmartPointer<vtkFloatArray>::New();
    tempoDataSigBeta->SetNumberOfComponents( 1 );
    if( betaDisplayedByProperties )
    {
        tempoDataSigBeta->SetName( QString( m_allCovariates.value( i ) + " SigBeta" ).toStdString().c_str() );
    }
    else
    {
        tempoDataSigBeta->SetName( QString( m_propertiesUsed.value( i ) + " SigBeta" ).toStdString().c_str() );
    }
    int index = 0;

    for( int j = 0; j < m_nbrPoint; j++ )
    {
        if( betaDisplayedByProperties )
        {
            if( isOmnibus )
            {
                if( m_csvOmnibusFDRLpvalue.at( i - 1 ).at( j ) <= m_alpha )
                {
                    tempoAbscissaSigBeta->InsertNextValue( table->GetValue( j, 0 ).ToFloat() );
                    tempoDataSigBeta->InsertNextValue( table->GetValue( j, i + 1 ).ToFloat() );
                    index++;
                }
            }
            else
            {
                if( m_csvPostHocFDRLpvalue.value( m_propertySelected ).at( i - 1 ).at( j ) <= m_alpha )
                {
                    tempoAbscissaSigBeta->InsertNextValue( table->GetValue( j, 0 ).ToFloat() );
                    tempoDataSigBeta->InsertNextValue( table->GetValue( j, i + 1 ).ToFloat() );
                    index++;
                }
            }
        }
        else
        {
            if( isOmnibus )
            {
                if( m_csvOmnibusFDRLpvalue.at( m_covariatesNoIntercept.key( m_covariateSelected ) - 1 ).at( j ) <= m_alpha )
                {
                    tempoAbscissaSigBeta->InsertNextValue( table->GetValue( j, 0 ).ToFloat() );
                    tempoDataSigBeta->InsertNextValue( table->GetValue( j, i + 1 ).ToFloat() );
                    index++;
                }
            }
            else
            {
                if( m_csvPostHocFDRLpvalue.value( m_propertiesUsed.at( i ) ).at( m_covariatesNoIntercept.key( m_covariateSelected ) - 1 ).at( j ) <= m_alpha )
                {
                    tempoAbscissaSigBeta->InsertNextValue( table->GetValue( j, 0 ).ToFloat() );
                    tempoDataSigBeta->InsertNextValue( table->GetValue( j, i + 1 ).ToFloat() );
                    index++;
                }
            }
        }
    }

    tableSigBetas->SetNumberOfRows( index );
    tableSigBetas->AddColumn( tempoAbscissaSigBeta );
    tableSigBetas->AddColumn( tempoDataSigBeta );
    for (int k = 0; k < index; k++)
    {
        tableSigBetas->SetValue( k, 0, tempoAbscissaSigBeta->GetValue( k ) );
        tableSigBetas->SetValue( k, 1, tempoDataSigBeta->GetValue( k ) );
    }
    vtkPlot *sigBetas;
    sigBetas = m_chart->AddPlot( vtkChart::POINTS );
    sigBetas->SetInputData( tableSigBetas, 0, 1 );
    vtkPlotPoints::SafeDownCast( sigBetas )->SetMarkerStyle( vtkPlotPoints::CIRCLE );
    sigBetas->SetColor( incrementColor*i, incrementColor*i/2, 255 - i*incrementColor, 255 ); // Improve color
}

void Plot::AddLineRawData( vtkSmartPointer<vtkTable> &table )
{
    int indexCovariate = m_binaryCovariates.key( m_covariateSelected );
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkPlot *currentLine = m_line.value( i );
        currentLine->SetInputData( table, 0, i + 1 );
        if( m_csvCovariate.at( i + 1 ).at( indexCovariate ).toInt() == 0 )
        {
            currentLine->SetColor( 0, 0, 255, 255 );
        }
        else
        {
            currentLine->SetColor( 255, 0, 0, 255 );
        }
        currentLine->SetWidth( 2.0 );
    }
}

void Plot::AddLineRawStats( vtkSmartPointer<vtkTable> &table )
{
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkPlot *currentLine = m_line.value( i );
        currentLine->SetInputData( table, 0, i + 1 );
        if( i < 3 )
        {
            currentLine->SetColor( 0, 0, 255, 255 );
        }
        else
        {
            currentLine->SetColor( 255, 0, 0, 255 );
        }

        if( !( i == 1 || i == 4 ) )
        {
            currentLine->GetPen()->SetLineType( vtkPen::DASH_LINE );
        }
        currentLine->SetWidth( 2.0 );
    }
}

void Plot::AddLineBetas( vtkSmartPointer<vtkTable> table, bool isSigBeta, bool betaDisplayedByProperties, bool isOmnibus )
{
    double increment = 255 / m_nbrPlot;
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkPlot *currentLine = m_line.value( i );
        currentLine->SetInputData( table, 0, i + 1 );
        currentLine->SetColor( increment*i, increment*i/2, 255 - i*increment, 255 ); // Improve color
        currentLine->SetWidth( 2.0 );

        if( isSigBeta )
        {
            if( betaDisplayedByProperties && ( i > 0 ) )
            {
                AddLineSigBetas( table, betaDisplayedByProperties, isOmnibus, i, increment );
            }
            if( !betaDisplayedByProperties )
            {
                AddLineSigBetas( table, betaDisplayedByProperties, isOmnibus, i, increment );
            }
        }
    }
}

void Plot::AddLineLPvalue( vtkSmartPointer<vtkTable> &table )
{
    double increment = 255 / m_nbrPlot;
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkPlot *currentLine = m_line.value( i );
        currentLine->SetInputData( table, 0, i + 1 );
        currentLine->SetColor( increment*i, increment*i/2, 255 - i*increment, 255 ); // Improve color
        currentLine->SetWidth( 2.0 );
    }
}


void Plot::AddLines( vtkSmartPointer<vtkTable> &table )
{
    InitLines();

    if( m_plotSelected == "Raw Data" )
    {
        AddLineRawData( table );
    }
    if( m_plotSelected == "Raw Stats" )
    {
        AddLineRawStats( table );
    }
    if( m_plotSelected == "Raw Betas by Properties" )
    {
        AddLineBetas( table, false, false, false );
    }
    if( m_plotSelected == "Raw Betas by Covariates" )
    {
        AddLineBetas( table, false, false, false );
    }
    if( m_plotSelected == "Omnibus Local pvalues" || m_plotSelected == "Omnibus FDR Local pvalues" ||
            m_plotSelected == "Post-Hoc FDR Local pvalues" )
    {
        AddLineLPvalue( table );
    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Properties" )
    {
        AddLineBetas( table, true, true, true );
    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Covariates" )
    {
        AddLineBetas( table, true, false, true );
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        AddLineBetas( table, true, true, false );
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        AddLineBetas( table, true, false, false );
    }
}













void Plot::SetDefaultTitle()
{
    if( m_plotSelected == "Raw Data" )
    {
        m_title = m_fibername + " " + m_propertySelected + " " + m_covariateSelected + " " + m_plotSelected;
    }
    if( m_plotSelected == "Raw Stats" )
    {
        m_title = m_fibername + " " + m_propertySelected + " " + m_covariateSelected + " Average and Standard Deviation";
    }
    if( m_plotSelected == "Raw Betas by Properties" )
    {
        m_title = m_fibername + " " + m_propertySelected + " Betas";
    }
    if( m_plotSelected == "Raw Betas by Covariates" )
    {
        m_title = m_fibername + " " + m_covariateSelected + " Betas";
    }
    if( m_plotSelected == "Omnibus Local pvalues" || m_plotSelected == "Omnibus FDR Local pvalues"
            || m_plotSelected == "Post-Hoc FDR Local pvalues" )
    {
        m_title = m_fibername + " " + m_plotSelected;
    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Properties" )
    {
        m_title = m_fibername + " " +  m_propertySelected + " Omnibus FDR Significant Betas";
    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Covariates" )
    {
        m_title = m_fibername + " " + m_covariateSelected + " Omnibus FDR Significant Betas";
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        m_title = m_fibername + " " + m_propertySelected + " Post-Hoc FDR Significant Betas";
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        m_title = m_fibername + " " + m_covariateSelected + " Post-Hoc FDR Significant Betas";
    }
}

void Plot::FindyMinMax()
{
    foreach( QList < double > rowData, m_ordinate )
    {
        foreach( double data, rowData )
        {
            if( m_yMin == 100 )
            {
                m_yMin = data;
            }
            else
            {
                if( data < m_yMin && data != QString( "inf" ).toDouble() )
                {
                    m_yMin = data;
                }
            }
            if( m_yMax == -100 )
            {
                m_yMax = data;
            }
            else
            {
                if( data > m_yMax && data != QString( "inf" ).toDouble() )
                {
                    m_yMax = data;
                }
            }
        }
    }
}

void Plot::SetChartProperties()
{
//    if( !m_plotSetting )
//    {
//        SetDefaultTitle();
//        m_xName = "Arc Length";
//        m_yName = "";
//    }

//    FindyMinMax();
//    if( m_yMinChecked )
//    {
//        m_yMin = m_yMinGiven;
//    }
//    if( m_yMaxChecked )
//    {
//        m_yMax = m_yMaxGiven;
//    }
//    qDebug() << "m_yMin: " << m_yMin << " | m_yMax: " << m_yMax;

//    m_chart->GetAxis( vtkAxis::BOTTOM )->SetTitle( m_xName.toStdString() );
//    m_chart->GetAxis( vtkAxis::BOTTOM )->SetRange( m_abscissa.first(), m_abscissa.last() );
//    m_chart->GetAxis( vtkAxis::BOTTOM )->SetMinimumLimit( m_abscissa.first() );
//    m_chart->GetAxis( vtkAxis::BOTTOM )->SetMaximumLimit( m_abscissa.last() );
//    m_chart->GetAxis( vtkAxis::BOTTOM )->GetGridPen()->SetColor( 127, 127, 127, 255 );

//    m_chart->GetAxis( vtkAxis::LEFT )->SetTitle( m_yName.toStdString() );
//    m_chart->GetAxis( vtkAxis::LEFT )->SetRange( m_yMin, m_yMax );
//    m_chart->GetAxis( vtkAxis::LEFT )->SetMinimumLimit( m_yMin );
//    m_chart->GetAxis( vtkAxis::LEFT )->SetMaximumLimit( m_yMax );
//    m_chart->GetAxis( vtkAxis::LEFT )->GetGridPen()->SetColor( 127, 127, 127, 255 );

//    m_chart->SetTitle( m_title.toStdString() );
//    m_chart->GetTitleProperties()->BoldOn();
//    m_chart->GetTitleProperties()->ItalicOn();
//    m_chart->GetTitleProperties()->SetFontSize( 12.0 );
////    m_chart->GetTitleProperties()->SetItalic( true );
////    m_chart->GetTitleProperties()->SetFontSize( 15.00 );

    m_chart->SetShowLegend( true );
//    m_chart->GetLegend()->SetDragEnabled( true );
}
