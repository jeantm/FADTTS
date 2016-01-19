#include "Plot.h"
#include <QDebug>

const QMap< QString, QList < int > > Plot::m_allColors = InitColorMap();
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
}

void Plot::InitPlot( QString directory, QString fibername )
{
    m_directory = directory;
    m_matlabDirectory = directory + "/MatlabOutputs";
    m_fibername = fibername;

    GetRawDataFiles();
    GetBetaFiles();
    GetOmnibusLpvalueFiles();
    GetOmnibusFDRLpvalueFiles();
    GetPostHocFDRLpvalueFiles();

    SetPlots();
    SetProperties();
    SetCovariates();
    SetAbscissa();
}

void Plot::ResetDataFile()
{
    m_csvRawDataFiles.clear();
    m_csvBetaFiles.clear();
    m_csvOmnibusLpvalueFiles.clear();
    m_csvOmnibusFDRLpvalueFiles.clear();
    m_csvPostHocFDRLpvalueFiles.clear();

    m_csvRawData.clear();
    m_csvCovariate.clear();
    m_csvBeta.clear();
    m_csvOmnibusLpvalue.clear();
    m_csvOmnibusFDRLpvalue.clear();
    m_csvPostHocLpvalue.clear();
    m_csvPostHocFDRLpvalue.clear();

    m_covariatesNoIntercept.clear();
    m_allCovariates.clear();
    m_binaryCovariates.clear();
}

void Plot::ResetPlot()
{
    m_chart->ClearPlots();
    m_yMin.second = false;
    m_yMax.second = false;
}


void Plot::SelectPlot( QString plotSelected )
{
    m_plotSelected = plotSelected;
    if( m_plotSelected == "Raw Data" || m_plotSelected == "Raw Stats" )
    {
        if( !m_isBinaryCovariatesSend )
        {
            emit CovariatesAvailableForPlotting( m_binaryCovariates );
            m_isBinaryCovariatesSend = true;
            m_isAllCovariatesSend = false;
            m_isCovariatesNoInterceptSend = false;
        }
    }
    if( m_plotSelected == "Raw Betas by Properties" || m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" || m_plotSelected == "Post-Hoc FDR Local pvalues" )
    {
        if( !m_isAllCovariatesSend )
        {
            emit CovariatesAvailableForPlotting( m_allCovariates );
            m_isBinaryCovariatesSend = false;
            m_isAllCovariatesSend = true;
            m_isCovariatesNoInterceptSend = false;
        }
    }
    if( m_plotSelected == "Raw Betas by Covariates" || m_plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        if( !m_isCovariatesNoInterceptSend )
        {
            emit CovariatesAvailableForPlotting( m_covariatesNoIntercept );
            m_isBinaryCovariatesSend = false;
            m_isAllCovariatesSend = false;
            m_isCovariatesNoInterceptSend = true;
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


void Plot::SetLinesToDisPlay( const QMap< QPair< int, QString >, QPair< bool, QString > > currentLinesForDisplay )
{
    m_linesToDisplay.clear();
    m_linesToDisplay = currentLinesForDisplay;
    qDebug() << endl << endl << "m_linesToDisplay: " << m_linesToDisplay << endl << endl;
}

void Plot::SetMarkerType( QString markerType )
{
    if( markerType == "Cross" )
    {
        m_markerType = vtkPlotPoints::CROSS;
    }
    if( markerType == "Plus" )
    {
        m_markerType = vtkPlotPoints::PLUS;
    }
    if( markerType == "Square" )
    {
        m_markerType = vtkPlotPoints::SQUARE;
    }
    if( markerType == "Circle" )
    {
        m_markerType = vtkPlotPoints::CIRCLE;
    }
    if( markerType == "Diamond" )
    {
        m_markerType = vtkPlotPoints::DIAMOND;
    }
}

void Plot::SetAlpha( double alpha )
{
    m_alpha = alpha;
}

void Plot::SetTitle( QString title, bool isBold, bool isItalic, double fontSize )
{
    m_chart->SetTitle( title.toStdString() );
    m_chart->GetTitleProperties()->SetBold( isBold );
    m_chart->GetTitleProperties()->SetItalic( isItalic );
    m_chart->GetTitleProperties()->SetFontSize( fontSize );
}

void Plot::SetDefaultTitle()
{
    QString title;
    if( m_plotSelected == "Raw Data" )
    {
        title = m_fibername + " " + m_propertySelected + " " + m_covariateSelected + " " + m_plotSelected;
    }
    if( m_plotSelected == "Raw Stats" )
    {
        title = m_fibername + " " + m_propertySelected + " " + m_covariateSelected + " Average and Standard Deviation";
    }
    if( m_plotSelected == "Raw Betas by Properties" )
    {
        title = m_fibername + " " + m_propertySelected + " Betas";
    }
    if( m_plotSelected == "Raw Betas by Covariates" )
    {
        title = m_fibername + " " + m_covariateSelected + " Betas";
    }
    if( m_plotSelected == "Omnibus Local pvalues" || m_plotSelected == "Omnibus FDR Local pvalues"
            || m_plotSelected == "Post-Hoc FDR Local pvalues" )
    {
        title = m_fibername + " " + m_plotSelected;
    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Properties" )
    {
        title = m_fibername + " " +  m_propertySelected + " Omnibus FDR Significant Betas";
    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Covariates" )
    {
        title = m_fibername + " " + m_covariateSelected + " Omnibus FDR Significant Betas";
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        title = m_fibername + " " + m_propertySelected + " Post-Hoc FDR Significant Betas";
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        title = m_fibername + " " + m_covariateSelected + " Post-Hoc FDR Significant Betas";
    }
    m_chart->SetTitle( title.toStdString() );
    m_chart->GetTitleProperties()->SetBold( false );
    m_chart->GetTitleProperties()->SetItalic( false );
    m_chart->GetTitleProperties()->SetFontSize( 12.0 );
}

void Plot::SetAxis( QString xName, QString yName, bool isBold, bool isItalic,
              bool isYMinSet, double yMin, bool isYMaxSet, double yMax )
{
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetTitle( xName.toStdString() );
    m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->SetBold( isBold );
    m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->SetItalic( isItalic );

    m_chart->GetAxis( vtkAxis::LEFT )->SetTitle( yName.toStdString() );
    m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->SetBold( isBold );
    m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->SetItalic( isItalic );

    if( isYMinSet )
    {
        m_yMin.first = yMin;
        m_yMin.second = true;
    }
    if( isYMaxSet )
    {
        m_yMax.first = yMax;
        m_yMax.second = true;
    }
}

void Plot::SetDefaultAxis()
{
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetTitle( "Arc Length" );
    m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->SetBold( false );
    m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->SetItalic( false );

    m_chart->GetAxis( vtkAxis::LEFT )->SetTitle( "" );
    m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->SetBold( false );
    m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->SetItalic( false );
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

        // Adding entries
        qDebug() << endl << "Adding entries";
        vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();
        AddEntries( table );

        // Adding data
        qDebug() << endl << "Adding data";
        SetData( table );

        // Adding lines
        qDebug() << endl << "Adding lines";
        AddLines( table );

        // Setting chart properties
        qDebug() << endl << "Setting chart properties";
        SetChartProperties();
    }
}


/***************************************************************/
/************************ Public  slots ************************/
/***************************************************************/


void Plot::SavePlot()
{
    QString fileName = m_matlabDirectory + "/" +  m_fibername + "_";
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


/***************************************************************/
/********************** Private functions **********************/
/***************************************************************/
void Plot::SortFilesByProperties( QString directory, QStringList files, QMap< QString, QList<QList<double> > > &data )
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
            data.insert( index, DataToDouble( m_process.GetDataFromFile( currentPath ) ) );
        }
    }
}

void Plot::TransposeData( QList < QList < double > > &data, int firstRow, int firstColumn )
{
    QList < QList < double > > dataTransposed;
    for( int j = firstColumn; j < data.first().size(); j++ )
    {
        QList < double > rowData;
        for( int i = firstRow; i < data.size(); i++ )
        {
            rowData.append( data.at( i ).at( j ) );
        }
        dataTransposed.append( rowData );
    }
    data.clear();
    data = dataTransposed;
}

void Plot::TransposeDataInQMap( QMap< QString, QList<QList<double> > > &data, int firstRow, int firstColumn )
{
    QMap< QString, QList < QList < double > > >::Iterator iterData = data.begin();
    while( iterData != data.end() )
    {
        TransposeData( iterData.value(), firstRow, firstColumn );
        ++iterData;
    }
}

QList < double > Plot::QStringListToDouble( QStringList rowData )
{
    QList < double > newRowData;
    foreach ( QString data, rowData )
    {
        newRowData.append( data.toDouble() );
    }
    return newRowData;
}

QList < QList < double > > Plot::DataToDouble( QList < QStringList > data )
{
    QList < QList < double > > newData;
    foreach ( QStringList rowData, data )
    {
        newData.append( QStringListToDouble( rowData ) );
    }
    return newData;
}


void Plot::SetRawData()
{
    SortFilesByProperties( m_directory, m_csvRawDataFiles, m_csvRawData );
    TransposeDataInQMap( m_csvRawData, 1, 0 );
}

void Plot::SetBeta()
{
    SortFilesByProperties( m_matlabDirectory, m_csvBetaFiles, m_csvBeta );
}

void Plot::SetOmnibusLpvalue( QStringList OmnibusLpvalueFiles, QList < QList < double > > &OmnibusLpvaluesData )
{
    QString currentPath = m_matlabDirectory + "/" + OmnibusLpvalueFiles.first();
    QList < QList < double > > dataTempo = DataToDouble( m_process.GetDataFromFile( currentPath ) );
    TransposeData( dataTempo, 0, 0 );
    OmnibusLpvaluesData = dataTempo;
}

void Plot::SetPostHocFDRLpvalue()
{
    SortFilesByProperties( m_matlabDirectory, m_csvPostHocFDRLpvalueFiles, m_csvPostHocFDRLpvalue );
    TransposeDataInQMap( m_csvPostHocFDRLpvalue, 0, 0 );
}


void Plot::GetRawDataFiles()
{
    QStringList nameFilterRawData( "*RawData.csv" );
    m_csvRawDataFiles = QDir( m_directory ).entryList( nameFilterRawData );
    SetRawData();

    QMap< QString, QList < QList < double > > >::ConstIterator iterRawData = m_csvRawData.begin();
    while( iterRawData != m_csvRawData.end() )
    {
        ++iterRawData;
    }
}

void Plot::GetBetaFiles()
{
    QStringList nameFilterBeta( "*Betas.csv" );
    m_csvBetaFiles = QDir( m_matlabDirectory ).entryList( nameFilterBeta );
    SetBeta();

    QMap< QString, QList < QList < double > > >::ConstIterator iterRawData = m_csvBeta.begin();
    while( iterRawData != m_csvBeta.end() )
    {
        ++iterRawData;
    }
}

void Plot::GetOmnibusLpvalueFiles()
{
    QStringList nameFilterOmnibusLpvalue( "*Omnibus_Local_pvalues.csv" );
    m_csvOmnibusLpvalueFiles = QDir( m_matlabDirectory ).entryList( nameFilterOmnibusLpvalue );
    SetOmnibusLpvalue( m_csvOmnibusLpvalueFiles, m_csvOmnibusLpvalue );
}

void Plot::GetOmnibusFDRLpvalueFiles()
{
    QStringList nameFilterOmnibusFDRLpvalue( "*Omnibus_FDR_Local_pvalues.csv" );
    m_csvOmnibusFDRLpvalueFiles = QDir( m_matlabDirectory ).entryList( nameFilterOmnibusFDRLpvalue );
    SetOmnibusLpvalue( m_csvOmnibusFDRLpvalueFiles, m_csvOmnibusFDRLpvalue );
}

void Plot::GetPostHocFDRLpvalueFiles()
{
    QStringList nameFilterPostHocFDRLpvalue( "*PostHoc_FDR_Local_pvalues.csv" );
    m_csvPostHocFDRLpvalueFiles = QDir( m_matlabDirectory ).entryList( nameFilterPostHocFDRLpvalue );
    SetPostHocFDRLpvalue();

    QMap< QString, QList < QList < double > > >::ConstIterator iterPostHocFDRLpvalue = m_csvPostHocFDRLpvalue.begin();
    while( iterPostHocFDRLpvalue != m_csvPostHocFDRLpvalue.end() )
    {
        ++iterPostHocFDRLpvalue;
    }
}

void Plot::SetPlots()
{
    QStringList plotsUsed;
    if( !m_csvRawDataFiles.isEmpty() )
    {
        plotsUsed.append( QStringList() << "Raw Data" << "Raw Stats" );
    }
    if( !m_csvBetaFiles.isEmpty() )
    {
        plotsUsed.append( QStringList() << "Raw Betas by Properties" << "Raw Betas by Covariates" );
    }
    if( !m_csvOmnibusLpvalueFiles.isEmpty() )
    {
        plotsUsed.append( QStringList() << "Omnibus Local pvalues" );
    }
    if( !m_csvOmnibusFDRLpvalueFiles.isEmpty() )
    {
        plotsUsed.append( QStringList() << "Omnibus FDR Local pvalues" );
    }
    if( !m_csvBetaFiles.isEmpty() && !m_csvOmnibusFDRLpvalueFiles.isEmpty() )
    {
        plotsUsed.append( QStringList() << "Omnibus FDR Significant Betas by Properties" <<
                          "Omnibus FDR Significant Betas by Covariates" );
    }
    if( !m_csvPostHocFDRLpvalueFiles.isEmpty() )
    {
        plotsUsed.append( QStringList() << "Post-Hoc FDR Local pvalues" );
    }
    if( !m_csvBetaFiles.isEmpty() && !m_csvPostHocFDRLpvalueFiles.isEmpty() )
    {
        plotsUsed.append( QStringList() << "Post-Hoc FDR Significant Betas by Properties" <<
                          "Post-Hoc FDR Significant Betas by Covariates" );
    }
    emit PlotsUsed( plotsUsed );
}

void Plot::SetProperties()
{
    QMap< int, QString > propertyTempo;
    foreach( QString path, m_csvRawDataFiles )
    {
        if( path.contains( "_ad_", Qt::CaseInsensitive ) )
        {
            propertyTempo.insert( 0, "AD" );
        }
        if( path.contains( "_md_", Qt::CaseInsensitive ) )
        {
            propertyTempo.insert( 1, "RD" );
        }
        if( path.contains( "_rd_", Qt::CaseInsensitive ) )
        {
            propertyTempo.insert( 2, "MD" );
        }
        if( path.contains( "_fa_", Qt::CaseInsensitive ) )
        {
            propertyTempo.insert( 3, "FA" );
        }
    }
    m_properties = propertyTempo.values();
    emit AllPropertiesUsed( m_properties );
    qDebug() << endl << "m_properties: " << m_properties;
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
    emit AllCovariatesUsed( m_allCovariates );
    qDebug() << endl << "m_allCovariates: " << m_allCovariates;
    qDebug() << "m_covariatesNoIntercept: " << m_covariatesNoIntercept;
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
    m_ordinate.clear();
    m_line.clear();
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

void Plot::LoadLinesToDisplay( QList< QList < double > > &ordinate )
{
    m_lineNames.clear();
    m_lineColors.clear();
    int shift = 0;
    if( m_plotSelected == "Omnibus Local pvalues" || m_plotSelected == "Omnibus FDR Local pvalues" )
    {
        m_linesToDisplay.remove( m_linesToDisplay.firstKey() );
        shift = 1;
    }
    QMap< QPair< int, QString >, QPair< bool, QString > >::ConstIterator iterLineToDisplay = m_linesToDisplay.end();
    while( iterLineToDisplay != m_linesToDisplay.begin() )
    {
        --iterLineToDisplay;
        if( iterLineToDisplay.value().first )
        {
            m_lineNames.prepend( iterLineToDisplay.key().second );
            m_lineColors.prepend( m_allColors.value( iterLineToDisplay.value().second ) );
        }
        else
        {
            ordinate.removeAt( iterLineToDisplay.key().first - shift );
        }
    }
}

QList< QList < double > > Plot::Tolog10( QList< QList < double > > data )
{
    QList< QList < double > > dataToLog10;
    foreach( QList < double > rowData, data )
    {
        QList < double > rowDataTempo;
        foreach( double dbl, rowData )
        {
            rowDataTempo.append( - std::log10( dbl ) );
        }
        dataToLog10.append( rowDataTempo );
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
    LoadLinesToDisplay( ordinate );

    return ordinate;
}

QList< QList < double > > Plot::LoadBetaByCovariate()
{
    QList< QList < double > > ordinate;
    foreach( QString property, m_properties )
    {
        ordinate.append( m_csvBeta.value( property ).at( m_allCovariates.key( m_covariateSelected ) ) );
    }
    LoadLinesToDisplay( ordinate );

    return ordinate;
}

QList< QList < double > > Plot::LoadOmnibusLpvalues( QList< QList < double > > omnibusLpvalues )
{
    QList< QList < double > > ordinate;
    ordinate = Tolog10( omnibusLpvalues );
    LoadLinesToDisplay( ordinate );

    return ordinate;
}

QList< QList < double > > Plot::LoadPostHocFDRLpvalues()
{
    QList< QList < double > > ordinate;
    foreach( QString property, m_properties )
    {
        ordinate.append( m_csvPostHocFDRLpvalue.value( property ).at( m_covariatesNoIntercept.key( m_covariateSelected ) - 1 ) );
    }
    LoadLinesToDisplay( ordinate );

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
    if( m_plotSelected == "Raw Betas by Properties" || m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        if( !m_propertySelected.isEmpty() )
        {
            m_ordinate = LoadBetas();
        }
    }
    if( m_plotSelected == "Raw Betas by Covariates" || m_plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        if( !m_covariateSelected.isEmpty() )
        {
            m_ordinate = LoadBetaByCovariate();
        }
    }
    if( m_plotSelected == "Omnibus Local pvalues" )
    {
        m_ordinate = LoadOmnibusLpvalues( m_csvOmnibusLpvalue );
    }
    if( m_plotSelected == "Omnibus FDR Local pvalues" )
    {
        m_ordinate = LoadOmnibusLpvalues( m_csvOmnibusFDRLpvalue );
    }
    if( m_plotSelected == "Post-Hoc FDR Local pvalues" )
    {
        if( !m_covariateSelected.isEmpty() )
        {
            m_ordinate = LoadPostHocFDRLpvalues();
        }
    }
    return !( m_abscissa.isEmpty() | m_ordinate.isEmpty() );
}


void Plot::AddEntriesRawData( vtkSmartPointer<vtkTable> &table )
{
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkSmartPointer<vtkFloatArray> currentEntry = vtkSmartPointer<vtkFloatArray>::New();
        currentEntry->SetName( QString( "Subject " + QString::number( i + 1 ) ).toStdString().c_str() );
        table->AddColumn( currentEntry );
    }
}

void Plot::AddEntriesRawStats( vtkSmartPointer<vtkTable> &table )
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

void Plot::AddEntriesByPropertiesOrCovariates( vtkSmartPointer<vtkTable> &table )
{
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkSmartPointer<vtkFloatArray> currentEntry = vtkSmartPointer<vtkFloatArray>::New();
        currentEntry->SetName( m_lineNames.at( i ).toStdString().c_str() );
        table->AddColumn( currentEntry );
    }
}

void Plot::AddEntries( vtkSmartPointer<vtkTable> &table )
{
    vtkSmartPointer<vtkFloatArray> abscissa = vtkSmartPointer<vtkFloatArray>::New();
    abscissa->SetName( "Arc Length" );
    table->AddColumn( abscissa );

    if( m_plotSelected == "Raw Data" )
    {
        AddEntriesRawData( table );
    }
    if( m_plotSelected == "Raw Stats" )
    {
        AddEntriesRawStats( table );
    }
    if( m_plotSelected == "Raw Betas by Properties" || m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        AddEntriesByPropertiesOrCovariates( table );
    }
    if( m_plotSelected == "Omnibus Local pvalues" || m_plotSelected == "Omnibus FDR Local pvalues" )
    {
        AddEntriesByPropertiesOrCovariates( table );
    }
    if( m_plotSelected == "Raw Betas by Covariates" ||
            m_plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Local pvalues" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        AddEntriesByPropertiesOrCovariates( table );
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
    vtkSmartPointer<vtkFloatArray> abscissaSigBetaTempo = vtkSmartPointer<vtkFloatArray>::New();
    abscissaSigBetaTempo->SetNumberOfComponents( 1 );
    abscissaSigBetaTempo->SetName( "Abscissa SigBeta" );
    vtkSmartPointer<vtkFloatArray> dataSigBetaTempo = vtkSmartPointer<vtkFloatArray>::New();
    dataSigBetaTempo->SetNumberOfComponents( 1 );
    dataSigBetaTempo->SetName( QString( m_lineNames.at( i ) + " SigBeta" ).toStdString().c_str() );

    int index = 0;
    for( int j = 0; j < m_nbrPoint; j++ )
    {
        if( betaDisplayedByProperties )
        {
            if( isOmnibus )
            {
                if( m_csvOmnibusFDRLpvalue.at( i - 1 ).at( j ) <= m_alpha )
                {
                    abscissaSigBetaTempo->InsertNextValue( table->GetValue( j, 0 ).ToFloat() );
                    dataSigBetaTempo->InsertNextValue( table->GetValue( j, i + 1 ).ToFloat() );
                    index++;
                }
            }
            else
            {
                if( m_csvPostHocFDRLpvalue.value( m_propertySelected ).at( i - 1 ).at( j ) <= m_alpha )
                {
                    abscissaSigBetaTempo->InsertNextValue( table->GetValue( j, 0 ).ToFloat() );
                    dataSigBetaTempo->InsertNextValue( table->GetValue( j, i + 1 ).ToFloat() );
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
                    abscissaSigBetaTempo->InsertNextValue( table->GetValue( j, 0 ).ToFloat() );
                    dataSigBetaTempo->InsertNextValue( table->GetValue( j, i + 1 ).ToFloat() );
                    index++;
                }
            }
            else
            {
                if( m_csvPostHocFDRLpvalue.value( m_properties.at( i ) ).at( m_covariatesNoIntercept.key( m_covariateSelected ) - 1 ).at( j ) <= m_alpha )
                {
                    abscissaSigBetaTempo->InsertNextValue( table->GetValue( j, 0 ).ToFloat() );
                    dataSigBetaTempo->InsertNextValue( table->GetValue( j, i + 1 ).ToFloat() );
                    index++;
                }
            }
        }
    }

    tableSigBetas->SetNumberOfRows( index );
    tableSigBetas->AddColumn( abscissaSigBetaTempo );
    tableSigBetas->AddColumn( dataSigBetaTempo );
    for (int k = 0; k < index; k++)
    {
        tableSigBetas->SetValue( k, 0, abscissaSigBetaTempo->GetValue( k ) );
        tableSigBetas->SetValue( k, 1, dataSigBetaTempo->GetValue( k ) );
    }
    vtkPlot *sigBetas;
    sigBetas = m_chart->AddPlot( vtkChart::POINTS );
    sigBetas->SetInputData( tableSigBetas, 0, 1 );
    vtkPlotPoints::SafeDownCast( sigBetas )->SetMarkerStyle( m_markerType );
    sigBetas->SetColor( m_lineColors.at( i ).at( 0 ), m_lineColors.at( i ).at( 1 ), m_lineColors.at( i ).at( 2 ), 255 ); // Improve color
}

void Plot::AddLineRawData( vtkSmartPointer<vtkTable> table )
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

void Plot::AddLineRawStats( vtkSmartPointer<vtkTable> table )
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
        currentLine->SetColor( m_lineColors.at( i ).at( 0 ), m_lineColors.at( i ).at( 1 ), m_lineColors.at( i ).at( 2 ), 255 ); // Improve color
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

void Plot::AddLineLPvalue( vtkSmartPointer<vtkTable> table )
{
    double increment = 255 / m_nbrPlot;
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkPlot *currentLine = m_line.value( i );
        currentLine->SetInputData( table, 0, i + 1 );
        currentLine->SetColor( m_lineColors.at( i ).at( 0 ), m_lineColors.at( i ).at( 1 ), m_lineColors.at( i ).at( 2 ), 255 ); // Improve color
        currentLine->SetWidth( 2.0 );
    }
}

void Plot::AddLines( vtkSmartPointer<vtkTable> table )
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


QList< double > Plot::GetyMinMax()
{
    QList< double > yMinMax;
    double yMin = m_yMin.second ? m_yMin.first : 1000;
    double yMax = m_yMax.second ? m_yMax.first : -1000;
    foreach( QList < double > rowData, m_ordinate )
    {
        foreach( double data, rowData )
        {
            if( !m_yMin.second && data < yMin && data != QString( "inf" ).toDouble() )
            {
                yMin = data;
            }

            if( !m_yMax.second && data > yMax && data != QString( "inf" ).toDouble() )
            {
                yMax = data;
            }
        }
    }
    yMinMax.append( yMin );
    yMinMax.append( yMax );
    return yMinMax;
}

void Plot::SetyMinMax()
{
    QList< double > yMinMax = GetyMinMax();

    m_chart->GetAxis( vtkAxis::LEFT )->SetMinimumLimit( yMinMax.first() );
    m_chart->GetAxis( vtkAxis::LEFT )->SetMaximumLimit( yMinMax.at( 1 ) );
    m_chart->GetAxis( vtkAxis::LEFT )->SetRange( yMinMax.first(), yMinMax.at( 1 ) );
    qDebug() << "yMin: " << yMinMax.first() << " | yMax: " << yMinMax.at( 1 );
}

void Plot::SetChartProperties()
{
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetRange( m_abscissa.first(), m_abscissa.last() );
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetMinimumLimit( m_abscissa.first() );
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetMaximumLimit( m_abscissa.last() );
    m_chart->GetAxis( vtkAxis::BOTTOM )->GetGridPen()->SetColor( 127, 127, 127, 255 );

    SetyMinMax();
    m_chart->GetAxis( vtkAxis::LEFT )->GetGridPen()->SetColor( 127, 127, 127, 255 );

    m_chart->SetShowLegend( true );
//    m_chart->GetLegend()->SetDragEnabled( true );
}
