#include "Plot.h"
#include <QDebug>

const QMap< QString, QList< int > > Plot::m_allColors = InitColorMap();
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

bool Plot::InitPlot( QString directory, QString fibername )
{
    m_directory = directory;
    m_matlabDirectory = directory + "/MatlabOutputs";
    m_fibername = fibername;

    m_isBinaryCovariatesSend = false;
    m_isAllCovariatesSend = false;
    m_isCovariatesNoInterceptSend = false;

    bool isPlottingEnabled = GetRawDataFiles();
    if( isPlottingEnabled )
    {
        GetBetaFiles();
        GetOmnibusLpvalueFiles();
        GetOmnibusFDRLpvalueFiles();
        GetConfidenceBandsFiles();
        GetPostHocFDRLpvalueFiles();

        SetPlots();
        SetProperties();
        SetCovariates();
        SetAbscissa();

//        qDebug() << endl << "m_csvRawDataFiles: " << m_csvRawDataFiles ;
//        qDebug() << "m_dataRawData: " << m_dataRawData ;
//        qDebug() << "m_dataCovariate: " << m_dataCovariate ;

//        qDebug() << endl << "m_csvBetaFiles: " << m_csvBetaFiles ;
//        qDebug() << "m_dataBeta: " << m_dataBeta ;

//        qDebug() << endl << "m_csvOmnibusLpvalueFiles: " << m_csvOmnibusLpvalueFiles ;
//        qDebug() << "m_dataOmnibusLpvalue: " << m_dataOmnibusLpvalue ;

//        qDebug() << endl << "m_csvOmnibusFDRLpvalueFiles: " << m_csvOmnibusFDRLpvalueFiles ;
//        qDebug() << "m_dataOmnibusFDRLpvalue: " << m_dataOmnibusFDRLpvalue ;

//        qDebug() << endl << "m_csvConfidenceBandsFiles: " << m_csvConfidenceBandsFiles ;
//        qDebug() << "m_dataConfidenceBands: " << m_dataConfidenceBands ;

//        qDebug() << endl << "m_csvPostHocFDRLpvalueFiles: " << m_csvPostHocFDRLpvalueFiles ;
//        qDebug() << "m_dataPostHocFDRLpvalue: " << m_dataPostHocFDRLpvalue ;

//        qDebug() << endl << "m_allCovariates: " << m_allCovariates ;
//        qDebug() << "m_covariatesNoIntercept: " << m_covariatesNoIntercept ;
//        qDebug() << "m_binaryCovariates: " << m_binaryCovariates ;

//        qDebug() << endl << "m_properties: " << m_properties ;
    }
    else
    {
        QString warningMessage;
        warningMessage.append( "<center><b>Plotting Disabled</b></center>" );
        if( m_dataRawData.isEmpty() )
        {
            warningMessage.append( "No abcsissa can be genereted because no raw data file was detected. Please ensure that your raw data file names contain '_ad_', '_md_', '_rd_' or '_fa_' depending on the property and end with 'RawData.csv'.<br>");
        }
        if( m_dataCovariate.isEmpty() )
        {
            warningMessage.append( "No covariate file detected. Please ensure that your covariate file name contains '_subMatrix_' and ends with 'RawData.csv'.<br>");
        }
        QMessageBox::warning( m_qvtkWidget, tr( "WARNING" ), tr( qPrintable( warningMessage ) ), QMessageBox::Ok );
    }
    return isPlottingEnabled;
}

void Plot::ResetPlotData()
{
    m_csvRawDataFiles.clear();
    m_csvBetaFiles.clear();
    m_csvOmnibusLpvalueFiles.clear();
    m_csvOmnibusFDRLpvalueFiles.clear();
    m_csvConfidenceBandsFiles.clear();
    m_csvPostHocFDRLpvalueFiles.clear();

    m_dataRawData.clear();
    m_dataCovariate.clear();
    m_subjects.clear();
    m_dataBeta.clear();
    m_dataOmnibusLpvalue.clear();
    m_dataOmnibusFDRLpvalue.clear();
    m_dataConfidenceBands.clear();
    m_dataPostHocFDRLpvalue.clear();

    m_properties.clear();
    m_covariatesNoIntercept.clear();
    m_allCovariates.clear();
    m_binaryCovariates.clear();
}

void Plot::ClearPlot()
{
    m_chart->ClearPlots();
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
            m_plotSelected == "Betas with Omnibus Confidence Bands" || m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
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
            m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" || m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
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


void Plot::SetLinesToDisPlay( const QMap< int, QPair< QString, QPair< bool, QString > > > currentLinesForDisplay )
{
    m_linesToDisplay.clear();
    m_linesToDisplay = currentLinesForDisplay;
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

void Plot::SetMarkerSize( double markerSize )
{
    m_markerSize = markerSize;
}

void Plot::SetPvalueThreshold( double pvalueThreshold )
{
    m_pvalueThreshold = pvalueThreshold;
}

void Plot::SetLineWidth( double lineWidth )
{
    m_lineWidth = lineWidth;
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
        title = m_fibername + " " + m_plotSelected + " " + m_covariateSelected + " (" + m_propertySelected + ")";
    }
    if( m_plotSelected == "Raw Stats" )
    {
        title = m_fibername + " Average and Standard Deviation " + m_covariateSelected + " (" + m_propertySelected + ")";
    }
    if( m_plotSelected == "Raw Betas by Properties" )
    {
        title = m_fibername + " " + m_propertySelected + " Beta Values";
    }
    if( m_plotSelected == "Raw Betas by Covariates" )
    {
        title = m_fibername + " " + m_covariateSelected + " Beta Values";
    }
    if( m_plotSelected == "Omnibus Local pvalues" || m_plotSelected == "Omnibus FDR Local pvalues" )
    {
        title = m_fibername + " " + m_plotSelected;
    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Properties" )
    {
        title = m_fibername + " Omnibus FDR Significant Beta Values " + m_propertySelected + " alpha=" + QString::number( m_pvalueThreshold );
    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Covariates" )
    {
        title = m_fibername + " Omnibus FDR Significant Beta Values " + m_covariateSelected + " alpha=" + QString::number( m_pvalueThreshold );
    }
    if( m_plotSelected == "Betas with Omnibus Confidence Bands" )
    {
        title = m_fibername + " Beta Values with Omnibus Confidence Bands " + m_covariateSelected + " (" + m_propertySelected + ")";
    }
    if( m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" )
    {
        title = m_fibername + " Post-Hoc FDR Local pvalues " + m_covariateSelected;
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        title = m_fibername + " Post-Hoc FDR Significant Beta Values " + m_propertySelected + " alpha=" + QString::number( m_pvalueThreshold );
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        title = m_fibername + " Post-Hoc FDR Significant Beta Values " + m_covariateSelected + " alpha=" + QString::number( m_pvalueThreshold );
    }
    m_chart->SetTitle( title.toStdString() );
    m_chart->GetTitleProperties()->SetBold( false );
    m_chart->GetTitleProperties()->SetItalic( false );
    m_chart->GetTitleProperties()->SetFontSize( 12.0 );
}

void Plot::SetGrid( bool gridOn )
{
    m_gridOn = gridOn;
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

    m_yMin.first = isYMinSet ? true : false;
    if( m_yMin.first )
    {
        m_yMin.second = yMin;
    }

    m_yMax.first = isYMaxSet ? true : false;
    if( m_yMax.first )
    {
        m_yMax.second = yMax;
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

    m_yMin.first = false;
    m_yMax.first = false;
}

void Plot::SetLegend( QString position )
{
    vtkSmartPointer<vtkChartLegend> legend = m_chart->GetLegend();

    int verticalPosition;
    int horizontalPosition;
    if( position.contains( "Top" ) )
    {
        verticalPosition = vtkChartLegend::TOP;
    }
    if( position.contains( "Middle" ) )
    {
        verticalPosition = vtkChartLegend::CENTER;
    }
    if( position.contains( "Bottom" ) )
    {
        verticalPosition = vtkChartLegend::BOTTOM;
    }

    if( position.contains( "Left" ) )
    {
        horizontalPosition = vtkChartLegend::LEFT;
    }
    if( position.contains( "Center" ) )
    {
        horizontalPosition = vtkChartLegend::CENTER;
    }
    if( position.contains( "Right" ) )
    {
        horizontalPosition = vtkChartLegend::RIGHT;
    }
    legend->SetVerticalAlignment( verticalPosition );
    legend->SetHorizontalAlignment( horizontalPosition );

//    legend->SetInline( false );

    if( m_plotSelected == "Raw Data" || m_plotSelected == "Betas with Omnibus Confidence Bands" )
    {
        m_chart->SetShowLegend( false );
    }
    else
    {
        m_chart->SetShowLegend( true );
    }
}


bool Plot::DisplayVTKPlot()
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

        // Adding entries
        vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();
        AddEntries( table );

        // Adding data
        SetData( table );

        // Adding lines
        AddLines( table );

        // Setting chart properties
        SetChartProperties();
    }

    return dataAvailable;
}


void Plot::UpdateCovariatesNames( QMap< int, QString > newCovariateName )
{
    QMap< int, QString >::ConstIterator iterNewCovariatesNames = newCovariateName.begin();
    while( iterNewCovariatesNames != newCovariateName.end() )
    {
        if( m_allCovariates.keys().contains( iterNewCovariatesNames.key() ) )
        {
            m_allCovariates[ iterNewCovariatesNames.key() ] = iterNewCovariatesNames.value();
        }
        if( m_covariatesNoIntercept.keys().contains( iterNewCovariatesNames.key() ) )
        {
            m_covariatesNoIntercept[ iterNewCovariatesNames.key() ] = iterNewCovariatesNames.value();
        }
        if( m_binaryCovariates.keys().contains( iterNewCovariatesNames.key() ) )
        {
            m_binaryCovariates[ iterNewCovariatesNames.key() ] = iterNewCovariatesNames.value();
        }
        ++iterNewCovariatesNames;
    }

    if( m_plotSelected == "Raw Data" || m_plotSelected == "Raw Stats" )
    {
        emit CovariatesAvailableForPlotting( m_binaryCovariates );
    }
    if( m_plotSelected == "Raw Betas by Properties" || m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
            m_plotSelected == "Betas with Omnibus Confidence Bands" || m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        emit CovariatesAvailableForPlotting( m_allCovariates );
    }
    if( m_plotSelected == "Raw Betas by Covariates" || m_plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" || m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        emit CovariatesAvailableForPlotting( m_covariatesNoIntercept );
    }
}


/***************************************************************/
/************************ Public  slots ************************/
/***************************************************************/
void Plot::OnSavePlot()
{
    QString fileName = m_matlabDirectory + "/" +  m_fibername + "_";
    if( m_plotSelected == "Raw Data" )
    {
        fileName += m_fibername + "_" + m_plotSelected.split( " " ).join( "_" ) + "_" + m_covariateSelected + "_" + m_propertySelected;
    }
    if( m_plotSelected == "Raw Stats" )
    {
        fileName += m_fibername + "_AverageStdDeviation_" + m_covariateSelected + "_" + m_propertySelected;
    }
    if( m_plotSelected == "Raw Betas by Properties" )
    {
        fileName += m_fibername + "_Betas_" + m_propertySelected;
    }
    if( m_plotSelected == "Raw Betas by Covariates" )
    {
        fileName += m_fibername + "_Betas_" + m_covariateSelected;
    }
    if( m_plotSelected == "Omnibus Local pvalues" || m_plotSelected == "Omnibus FDR Local pvalues" )
    {
        fileName += m_fibername + "_" + m_plotSelected.split( " " ).join( "_" );
    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Properties" )
    {
        fileName += m_fibername + "_Omnibus_FDR_SigBetas_" + m_propertySelected;
    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Covariates" )
    {
        fileName += m_fibername + "_Omnibus_FDR_SigBetas_" + m_covariateSelected;
    }
    if( m_plotSelected == "Betas with Omnibus Confidence Bands" )
    {
        fileName += m_fibername + "_Betas_Omnibus_Confidence_Bands_" + m_covariateSelected + "_" + m_propertySelected;
    }
    if( m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" )
    {
        fileName += m_fibername + "_PostHoc_FDR_Local_pvalues_" + m_covariateSelected;
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        fileName += m_fibername + "_PostHoc_FDR_SigBetas_" + m_propertySelected;
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        fileName +=  m_fibername + "_PostHoc_FDR_SigBetas_" + m_covariateSelected;
    }
    fileName += ".pdf";

    QString filePath = QFileDialog::getSaveFileName( m_qvtkWidget, tr( "Save plot as ..." ), fileName, tr( ".pdf ( *.pdf ) ;; .*( * )" ) );
    if( !filePath.isEmpty() )
    {
        vtkSmartPointer<vtkGL2PSExporter> writer = vtkSmartPointer<vtkGL2PSExporter>::New();
        writer->SetRenderWindow( m_view->GetRenderWindow() );
        writer->SetFilePrefix( filePath.split( "." ).first().toStdString().c_str() );
        writer->SetFileFormat( vtkGL2PSExporter::PDF_FILE );
        writer->CompressOff();
        writer->LandscapeOff();
        writer->DrawBackgroundOn();
        writer->SetSortToBSP();
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
        if( path.contains( "_ad_", Qt::CaseInsensitive ) || path.contains( "_ad.csv", Qt::CaseInsensitive ) )
        {
            index = "AD";
        }
        if( path.contains( "_md_", Qt::CaseInsensitive ) || path.contains( "_md.csv", Qt::CaseInsensitive ) )
        {
            index = "MD";
        }
        if( path.contains( "_rd_", Qt::CaseInsensitive ) || path.contains( "_rd.csv", Qt::CaseInsensitive ) )
        {
            index = "RD";
        }
        if( path.contains( "_fa_", Qt::CaseInsensitive ) || path.contains( "_fa.csv", Qt::CaseInsensitive ) )
        {
            index = "FA";
        }
        if( path.contains( "_subMatrix_", Qt::CaseInsensitive ) || path.contains( "_subMatrix.csv", Qt::CaseInsensitive ) )
        {
            m_dataCovariate = m_processing.GetDataFromFile( currentPath );
            if( !m_dataCovariate.isEmpty() )
            {
                for( int i = 1; i < m_dataCovariate.size(); i++ )
                {
                    m_subjects.append( m_dataCovariate.at( i ).first() );
                }
            }
        }

        if( !index.isNull() )
        {
            data.insert( index, DataToDouble( m_processing.GetDataFromFile( currentPath ) ) );
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
    SortFilesByProperties( m_directory, m_csvRawDataFiles, m_dataRawData );
    TransposeDataInQMap( m_dataRawData, 1, 0 );
}

void Plot::SetBeta()
{
    SortFilesByProperties( m_matlabDirectory, m_csvBetaFiles, m_dataBeta );
}

void Plot::SetOmnibusLpvalue( QStringList OmnibusLpvalueFiles, QList < QList < double > > &OmnibusLpvaluesData )
{
    QString currentPath = m_matlabDirectory + "/" + OmnibusLpvalueFiles.first();
    QList < QList < double > > dataTempo = DataToDouble( m_processing.GetDataFromFile( currentPath ) );
    TransposeData( dataTempo, 0, 0 );
    OmnibusLpvaluesData = dataTempo;
}

void Plot::SetConfidenceBands()
{
    SortFilesByProperties( m_matlabDirectory, m_csvConfidenceBandsFiles, m_dataConfidenceBands );
}

void Plot::SetPostHocFDRLpvalue()
{
    SortFilesByProperties( m_matlabDirectory, m_csvPostHocFDRLpvalueFiles, m_dataPostHocFDRLpvalue );
    TransposeDataInQMap( m_dataPostHocFDRLpvalue, 0, 0 );
}


bool Plot::GetRawDataFiles()
{
    QStringList nameFilterRawData( "*RawData.csv" );
    m_csvRawDataFiles = QDir( m_directory ).entryList( nameFilterRawData );
    SetRawData();

    return( !m_dataRawData.isEmpty() && !m_dataCovariate.isEmpty() );
}

void Plot::GetBetaFiles()
{
    QStringList nameFilterBeta( "*_Betas_*.csv" );
    m_csvBetaFiles = QDir( m_matlabDirectory ).entryList( nameFilterBeta );
    SetBeta();
}

void Plot::GetOmnibusLpvalueFiles()
{
    QStringList nameFilterOmnibusLpvalue( "*Omnibus_Local_pvalues.csv" );
    m_csvOmnibusLpvalueFiles = QDir( m_matlabDirectory ).entryList( nameFilterOmnibusLpvalue );
    if( !m_csvOmnibusLpvalueFiles.isEmpty() )
    {
        SetOmnibusLpvalue( m_csvOmnibusLpvalueFiles, m_dataOmnibusLpvalue );
    }
}

void Plot::GetOmnibusFDRLpvalueFiles()
{
    QStringList nameFilterOmnibusFDRLpvalue( "*Omnibus_FDR_Local_pvalues.csv" );
    m_csvOmnibusFDRLpvalueFiles = QDir( m_matlabDirectory ).entryList( nameFilterOmnibusFDRLpvalue );
    if( !m_csvOmnibusFDRLpvalueFiles.isEmpty() )
    {
        SetOmnibusLpvalue( m_csvOmnibusFDRLpvalueFiles, m_dataOmnibusFDRLpvalue );
    }
}

void Plot::GetConfidenceBandsFiles()
{
    QStringList nameFilterConfidenceBands( "*_Omnibus_ConfidenceBands_*.csv" );
    m_csvConfidenceBandsFiles = QDir( m_matlabDirectory ).entryList( nameFilterConfidenceBands );
    SetConfidenceBands();
}

void Plot::GetPostHocFDRLpvalueFiles()
{
    QStringList nameFilterPostHocFDRLpvalue( "*_PostHoc_FDR_Local_pvalues_*.csv" );
    m_csvPostHocFDRLpvalueFiles = QDir( m_matlabDirectory ).entryList( nameFilterPostHocFDRLpvalue );
    SetPostHocFDRLpvalue();
}

void Plot::SetPlots()
{
    QStringList plotsUsed;
    if( !m_dataRawData.isEmpty() )
    {
        plotsUsed.append( QStringList() << "Raw Data" << "Raw Stats" );
    }
    if( !m_dataBeta.isEmpty() )
    {
        plotsUsed.append( QStringList() << "Raw Betas by Properties" << "Raw Betas by Covariates" );
    }
    if( !m_dataOmnibusLpvalue.isEmpty() )
    {
        plotsUsed.append( QStringList() << "Omnibus Local pvalues" );
    }
    if( !m_dataOmnibusFDRLpvalue.isEmpty() )
    {
        plotsUsed.append( QStringList() << "Omnibus FDR Local pvalues" );
    }
    if( !m_dataBeta.isEmpty() && !m_dataOmnibusFDRLpvalue.isEmpty() )
    {
        plotsUsed.append( QStringList() << "Omnibus FDR Significant Betas by Properties" <<
                          "Omnibus FDR Significant Betas by Covariates" );
    }
    if( !m_dataBeta.isEmpty() && !m_dataConfidenceBands.isEmpty() )
    {
        plotsUsed.append( QStringList() << "Betas with Omnibus Confidence Bands" );
    }
    if( !m_dataPostHocFDRLpvalue.isEmpty() )
    {
        plotsUsed.append( QStringList() << "Post-Hoc FDR Local pvalues by Covariates" );
    }
    if( !m_dataBeta.isEmpty() && !m_dataPostHocFDRLpvalue.isEmpty() )
    {
        plotsUsed.append( QStringList() << "Post-Hoc FDR Significant Betas by Properties" <<
                          "Post-Hoc FDR Significant Betas by Covariates" );
    }
    emit PlotsUsed( plotsUsed );
}

void Plot::SetProperties()
{
    foreach( QString path, m_csvRawDataFiles )
    {
        if( path.contains( "_ad_", Qt::CaseInsensitive ) )
        {
            m_properties.insert( 0, "AD" );
        }
        if( path.contains( "_rd_", Qt::CaseInsensitive ) )
        {
            m_properties.insert( 1, "RD" );
        }
        if( path.contains( "_md_", Qt::CaseInsensitive ) )
        {
            m_properties.insert( 2, "MD" );
        }
        if( path.contains( "_fa_", Qt::CaseInsensitive ) )
        {
            m_properties.insert( 3, "FA" );
        }
    }
    emit AllPropertiesUsed( m_properties );
}

void Plot::SetCovariates()
{
    QStringList firstRaw = m_dataCovariate.first();
    QStringList firstDataRaw = m_dataCovariate.at( 1 );

    m_allCovariates.insert( 0, "Intercept" );
    for( int i = 1; i < firstRaw.size(); i++ )
    {
        m_allCovariates.insert( i, firstRaw.at( i ) );
        m_covariatesNoIntercept.insert( i, firstRaw.at( i ) );
        if( ( firstDataRaw .at( i ).toDouble() == 0 ) || ( firstDataRaw.at( i ).toDouble() == 1 ) )
        {
            m_binaryCovariates.insert( i, firstRaw.at( i ) );
        }
    }
    emit AllCovariatesUsed( m_allCovariates );
}

void Plot::SetAbscissa()
{
    m_abscissa.clear();
    m_abscissa.append( m_dataRawData.first().first() );
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
        if( m_dataCovariate.at( i + 1 ).at( indexCovariate ).toDouble() == 0 )
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
    QMap< int, QPair< QString, QPair< bool, QString > > >::ConstIterator iterLineToDisplay = m_linesToDisplay.end();
    while( iterLineToDisplay != m_linesToDisplay.begin() )
    {
        --iterLineToDisplay;
        if( iterLineToDisplay.value().second.first )
        {
            m_lineNames.prepend( iterLineToDisplay.value().first );
            m_lineColors.prepend( m_allColors.value( iterLineToDisplay.value().second.second ) );
        }
        else
        {
            ordinate.removeAt( iterLineToDisplay.key() - shift );
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
    QList< QList < double > > ordinate = m_dataRawData.value( m_propertySelected );
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
    ordinate = m_dataBeta.value( m_propertySelected );
    LoadLinesToDisplay( ordinate );

    return ordinate;
}

QList< QList < double > > Plot::LoadBetaByCovariate()
{
    QList< QList < double > > ordinate;
    QMap< int, QString >::ConstIterator iterProperties = m_properties.begin();
    while( iterProperties != m_properties.end() )
    {
        ordinate.append( m_dataBeta.value( iterProperties.value() ).at( m_allCovariates.key( m_covariateSelected ) ) );
        ++iterProperties;
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

QList< QList < double > > Plot::LoadConfidenceBand()
{
    QList< QList < double > > ordinate;
    ordinate.append( m_dataConfidenceBands.value( m_propertySelected ).at( 2*m_allCovariates.key( m_covariateSelected ) + 1 ) );
    ordinate.append( m_dataBeta.value( m_propertySelected ).at( m_allCovariates.key( m_covariateSelected ) ) );
    ordinate.append( m_dataConfidenceBands.value( m_propertySelected ).at( 2*m_allCovariates.key( m_covariateSelected ) ) );

    return ordinate;
}

QList< QList < double > > Plot::LoadPostHocFDRLpvalues()
{
    QList< QList < double > > ordinate;
    QMap< int, QString >::ConstIterator iterProperties = m_properties.begin();
    while( iterProperties != m_properties.end() )
    {
        ordinate.append( m_dataPostHocFDRLpvalue.value( iterProperties.value() ).at( m_covariatesNoIntercept.key( m_covariateSelected ) - 1 ) );
        ++iterProperties;
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
        m_ordinate = LoadOmnibusLpvalues( m_dataOmnibusLpvalue );
    }
    if( m_plotSelected == "Omnibus FDR Local pvalues" )
    {
        m_ordinate = LoadOmnibusLpvalues( m_dataOmnibusFDRLpvalue );
    }
    if( m_plotSelected == "Betas with Omnibus Confidence Bands" )
    {
        if( !m_propertySelected.isEmpty() && !m_covariateSelected.isEmpty() )
        {
            m_ordinate = LoadConfidenceBand();
        }
    }
    if( m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" )
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
        currentEntry->SetName( m_subjects.at( i ).toStdString().c_str() );
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

void Plot::AddEntriesCovariatesBands( vtkSmartPointer<vtkTable> &table )
{
    vtkSmartPointer<vtkFloatArray> upperConfidenceBand = vtkSmartPointer<vtkFloatArray>::New();
    upperConfidenceBand->SetName( "Upper Confidence Band" );
    table->AddColumn( upperConfidenceBand );
    vtkSmartPointer<vtkFloatArray> betas = vtkSmartPointer<vtkFloatArray>::New();
    betas->SetName( "Betas" );
    table->AddColumn( betas );
    vtkSmartPointer<vtkFloatArray> lowerConfidenceBand = vtkSmartPointer<vtkFloatArray>::New();
    lowerConfidenceBand->SetName( "Lower Confidence Band" );
    table->AddColumn( lowerConfidenceBand );
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
    if( m_plotSelected == "Raw Betas by Properties" || m_plotSelected == "Raw Betas by Covariates" ||
            m_plotSelected == "Omnibus Local pvalues" || m_plotSelected == "Omnibus FDR Local pvalues" ||
            m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
            m_plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        AddEntriesByPropertiesOrCovariates( table );
    }
    if( m_plotSelected == "Betas with Omnibus Confidence Bands" )
    {
        AddEntriesCovariatesBands( table );
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

void Plot::AddSignificantLevel( vtkSmartPointer<vtkTable> table, double significantLevel )
{
    vtkSmartPointer<vtkFloatArray> abscissaSigLevel = vtkSmartPointer<vtkFloatArray>::New();
    abscissaSigLevel->SetNumberOfComponents( 1 );
    abscissaSigLevel->SetName( "Abscissa SigLevel" );
    vtkSmartPointer<vtkFloatArray> dataSigLevel = vtkSmartPointer<vtkFloatArray>::New();
    dataSigLevel->SetNumberOfComponents( 1 );
    dataSigLevel->SetName( "Significant Level" );
    for( int i = 0; i < m_nbrPoint; i++ )
    {
        abscissaSigLevel->InsertNextValue( m_abscissa.at( i ) );
        dataSigLevel->InsertNextValue( significantLevel );
    }

    vtkSmartPointer<vtkTable> tableSigLevel = vtkSmartPointer<vtkTable>::New();
    tableSigLevel->SetNumberOfRows( m_nbrPoint );
    tableSigLevel->AddColumn( abscissaSigLevel );
    tableSigLevel->AddColumn( dataSigLevel );

    vtkPlot *sigLevelLine;
    sigLevelLine = m_chart->AddPlot( vtkChart::LINE );
    sigLevelLine->SetInputData( tableSigLevel, 0, 1 );
    sigLevelLine->GetPen()->SetLineType( vtkPen::DASH_LINE );
    sigLevelLine->SetColor( m_allColors.value( "Black" ).first(), m_allColors.value( "Black" ).at( 1 ), m_allColors.value( "Black" ).at( 2 ), 255 );
    sigLevelLine->SetWidth( m_lineWidth / 2 );
}

void Plot::AddLineSigBetas( vtkSmartPointer<vtkTable> table, bool betaDisplayedByProperties, bool isOmnibus, int i )
{
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
                if( m_dataOmnibusFDRLpvalue.at( m_allCovariates.key( m_lineNames.at( i ) ) - 1 ).at( j ) <= m_pvalueThreshold )
                {
                    abscissaSigBetaTempo->InsertNextValue( table->GetValue( j, 0 ).ToFloat() );
                    dataSigBetaTempo->InsertNextValue( table->GetValue( j, i + 1 ).ToFloat() );
                    index++;
                }
            }
            else
            {
                if( m_dataPostHocFDRLpvalue.value( m_propertySelected ).at( m_allCovariates.key( m_lineNames.at( i ) ) - 1 ).at( j ) <= m_pvalueThreshold )
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
                if( m_dataOmnibusFDRLpvalue.at( m_allCovariates.key( m_covariateSelected ) - 1 ).at( j ) <= m_pvalueThreshold )
                {
                    abscissaSigBetaTempo->InsertNextValue( table->GetValue( j, 0 ).ToFloat() );
                    dataSigBetaTempo->InsertNextValue( table->GetValue( j, i + 1 ).ToFloat() );
                    index++;
                }
            }
            else
            {
                if( m_dataPostHocFDRLpvalue.value( m_lineNames.at( i ) ).at( m_allCovariates.key( m_covariateSelected ) - 1 ).at( j ) <= m_pvalueThreshold )
                {
                    abscissaSigBetaTempo->InsertNextValue( table->GetValue( j, 0 ).ToFloat() );
                    dataSigBetaTempo->InsertNextValue( table->GetValue( j, i + 1 ).ToFloat() );
                    index++;
                }
            }
        }
    }

    if( ( dataSigBetaTempo->GetSize() != 0 ) && ( dataSigBetaTempo->GetSize() != 0 ) )
    {
        vtkSmartPointer<vtkTable> tableSigBetas = vtkSmartPointer<vtkTable>::New();
        tableSigBetas->SetNumberOfRows( index );
        tableSigBetas->AddColumn( abscissaSigBetaTempo );
        tableSigBetas->AddColumn( dataSigBetaTempo );

        vtkPlot *sigBetas;
        sigBetas = m_chart->AddPlot( vtkChart::POINTS );
        sigBetas->SetInputData( tableSigBetas, 0, 1 );
        vtkPlotPoints::SafeDownCast( sigBetas )->SetMarkerStyle( m_markerType );
        vtkPlotPoints::SafeDownCast( sigBetas )->SetMarkerSize( m_markerSize );
        sigBetas->SetColor( m_lineColors.at( i ).first(), m_lineColors.at( i ).at( 1 ), m_lineColors.at( i ).at( 2 ), 255 );
    }
}

void Plot::AddLineRawData( vtkSmartPointer<vtkTable> table )
{
    int indexCovariate = m_binaryCovariates.key( m_covariateSelected );
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkPlot *currentLine = m_line.value( i );
        currentLine->SetInputData( table, 0, i + 1 );
        if( m_dataCovariate.at( i + 1 ).at( indexCovariate ).toDouble() == 0 )
        {
            currentLine->SetColor( 0, 0, 0, 255 );
            currentLine->SetWidth( m_lineWidth / 2 );
        }
        else
        {
            QList < int > color = m_allColors.value( m_linesToDisplay.value( m_binaryCovariates.key( m_covariateSelected ) ).second.second );
            currentLine->SetColor( color.first(), color.at( 1 ), color.at( 2 ), 255 );
            currentLine->SetWidth( m_lineWidth );
        }
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

        if( ( i == 0 || i == 3 ) )
        {
            currentLine->GetPen()->SetLineType( vtkPen::DASH_LINE );
        }
        if( ( i == 2 || i == 5 ) )
        {
            currentLine->GetPen()->SetLineType( vtkPen::DASH_DOT_DOT_LINE );
        }
        currentLine->SetWidth( m_lineWidth );
    }
}

void Plot::AddLineBetas( vtkSmartPointer<vtkTable> table, bool isSigBeta, bool betaDisplayedByProperties, bool isOmnibus )
{
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkPlot *currentLine = m_line.value( i );
        currentLine->SetInputData( table, 0, i + 1 );
        currentLine->SetColor( m_lineColors.at( i ).first(), m_lineColors.at( i ).at( 1 ), m_lineColors.at( i ).at( 2 ), 255 );
        currentLine->SetWidth( m_lineWidth );

        if( isSigBeta )
        {

            if( betaDisplayedByProperties && ( m_lineNames.at( i ) != m_allCovariates.first() ) )
            {
                AddLineSigBetas( table, betaDisplayedByProperties, isOmnibus, i );
            }
            if( !betaDisplayedByProperties )
            {
                AddLineSigBetas( table, betaDisplayedByProperties, isOmnibus, i );
            }
        }
    }

    if( isSigBeta )
    {
        AddSignificantLevel( table, 0 );
    }
}

void Plot::AddLineLPvalue( vtkSmartPointer<vtkTable> table )
{
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkPlot *currentLine = m_line.value( i );
        currentLine->SetInputData( table, 0, i + 1 );
        currentLine->SetColor( m_lineColors.at( i ).first(), m_lineColors.at( i ).at( 1 ), m_lineColors.at( i ).at( 2 ), 255 );
        currentLine->SetWidth( m_lineWidth );
    }

    AddSignificantLevel( table, -log10( m_pvalueThreshold ) );
}

void Plot::AddLineLConfidenceBands( vtkSmartPointer<vtkTable> table )
{
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkPlot *currentLine = m_line.value( i );
        currentLine->SetInputData( table, 0, i + 1 );
        if( i%2 )
        {
            currentLine->SetColor( 0, 0, 255, 255 );
            currentLine->GetPen()->SetLineType( vtkPen::DASH_LINE );
        }
        else
        {
            currentLine->SetColor( 255, 0, 0, 255 );
        }
        currentLine->SetWidth( m_lineWidth );
    }

    AddSignificantLevel( table, 0 );
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
    if( m_plotSelected == "Raw Betas by Properties" || m_plotSelected == "Raw Betas by Covariates" )
    {
        AddLineBetas( table, false, false, false );
    }
    if( m_plotSelected == "Omnibus Local pvalues" || m_plotSelected == "Omnibus FDR Local pvalues" ||
            m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" )
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
    if( m_plotSelected == "Betas with Omnibus Confidence Bands" )
    {
        AddLineLConfidenceBands( table );
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
    double yMin = m_yMin.first ? m_yMin.second : 1000;
    double yMax = m_yMax.first ? m_yMax.second : -1000;
    foreach( QList < double > rowData, m_ordinate )
    {
        foreach( double data, rowData )
        {
            if( !m_yMin.first && data < yMin && data != QString( "inf" ).toDouble() )
            {
                yMin = data;
            }

            if( !m_yMax.first && data > yMax && data != QString( "inf" ).toDouble() )
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

//    m_chart->GetAxis( vtkAxis::LEFT )->SetRange( yMinMax.first(), yMinMax.at( 1 ) );
//    m_chart->GetAxis( vtkAxis::LEFT )->SetMinimumLimit( yMinMax.first() );
//    m_chart->GetAxis( vtkAxis::LEFT )->SetMaximumLimit( yMinMax.at( 1 ) );
}

void Plot::SetChartProperties()
{
//    m_chart->GetAxis( vtkAxis::BOTTOM )->SetRange( m_abscissa.first(), m_abscissa.last() );
//    m_chart->GetAxis( vtkAxis::BOTTOM )->SetMinimumLimit( m_abscissa.first() );
//    m_chart->GetAxis( vtkAxis::BOTTOM )->SetMaximumLimit( m_abscissa.last() );

    SetyMinMax();

    m_chart->GetAxis( vtkAxis::BOTTOM )->GetGridPen()->SetWidth( 0.25 );
    m_chart->GetAxis( vtkAxis::BOTTOM )->GetGridPen()->SetColor( 0, 0, 0, m_gridOn ? 255 : 0 );
    m_chart->GetAxis( vtkAxis::LEFT )->GetGridPen()->SetWidth( 0.25 );
    m_chart->GetAxis( vtkAxis::LEFT )->GetGridPen()->SetColor( 0, 0, 0, m_gridOn ? 255 : 0 );


    m_chart->SetSelectionMethod( vtkChartXY::SELECTION_COLUMNS );
    m_chart->SetActionToButton( vtkChart::SELECT, vtkContextMouseEvent::LEFT_BUTTON );
    m_chart->SetActionToButton( vtkChart::ZOOM, vtkContextMouseEvent::MIDDLE_BUTTON );
    m_chart->SetActionToButton( vtkChart::PAN, vtkContextMouseEvent::RIGHT_BUTTON );
}
