#include "Plot.h"

#include <QVTKWidget.h>

//#include <QDebug>


void CallbackAddSelectedLine( vtkObject* caller, long unsigned int eventId, void* clientData, void* callData )
{
    Plot* plot = static_cast< Plot* >( clientData );
    vtkSmartPointer< vtkPlot > currentLine = static_cast< vtkPlot* >( caller );

    plot->AddSelectedLine( currentLine );
}

void CallbackUpdateSelectedLines( vtkObject* caller, long unsigned int eventId, void* clientData, void* callData )
{
    Plot* plot = static_cast< Plot* >( clientData );

    plot->UpdateLineSelection();
}



const QMap< QString, QList< int > > Plot::m_allColors = InitColorMap();
/***************************************************************/
/********************** Public functions ***********************/
/***************************************************************/
Plot::Plot( QObject *parent ) :
    QObject( parent )
{
}


void Plot::SetQVTKWidget( QSharedPointer< QVTKWidget > qvtkWidget, bool isQCThreshold )
{
    m_qvtkWidget = QSharedPointer< QVTKWidget >( qvtkWidget );
    m_view = vtkSmartPointer< vtkContextView >::New();
    m_chart = vtkSmartPointer< vtkChartXY >::New();

    m_view->SetInteractor( m_qvtkWidget->GetInteractor() );
    m_qvtkWidget->SetRenderWindow( m_view->GetRenderWindow() );

    if( !isQCThreshold )
    {
        connect( m_qvtkWidget.data(), SIGNAL( mouseEvent( QMouseEvent * ) ), this, SLOT( OnMouseEvent( QMouseEvent * ) ) );
    }
}

bool Plot::InitPlot( QString directory, QString fibername, double pvalueThreshold )
{
    m_directory = directory;
    m_matlabDirectory = directory + "/MatlabOutputs";
    m_fibername = fibername;
    m_pvalueThreshold = pvalueThreshold;

    m_isBinaryCovariatesSent = false;
    m_isAllCovariatesSent = false;
    m_isCovariatesNoInterceptSent = false;

    GetRawDataFiles();
    GetBetaFiles();
    GetOmnibusLpvalueFiles();
    GetOmnibusFDRLpvalueFiles();
    GetConfidenceBandsFiles();
    GetPostHocFDRLpvalueFiles();

    bool arePlotsAvailable = SetPlots();

    return arePlotsAvailable;
}

bool Plot::InitQCThresholdPlot( QList< QStringList > rawData, QStringList matchedSubjects )
{
    ClearPlot();
    ResetPlotData();

    m_matchedSubjects = matchedSubjects;
    m_subjects = rawData.first();

    m_plotSelected = "Raw Data";
    m_propertySelected = "FA";
    m_lineWidth = 0.75;

    bool isPlottingEnabled = SetRawDataQCThreshold( rawData );

    return isPlottingEnabled;
}

bool Plot::DisplayPlot()
{
    // Resetting scene and Observers
    m_view->GetScene()->ClearItems();
    m_view->GetScene()->AddItem( m_chart );
    m_chart->RemoveAllObservers();

    // Loading data
    bool dataAvailable = LoadData();
    if( dataAvailable )
    {
        m_nbrPlots = m_ordinate.size();

        // Adding entries
        vtkSmartPointer< vtkTable > table = vtkSmartPointer< vtkTable >::New();
        AddEntries( table );

        // Adding data
        SetData( table );

        // Adding lines
        AddLines( table );

        // Setting axis properties and observers
        SetAxisProperties();
        SetObservers();
    }

    return dataAvailable;
}

bool Plot::DisplayQCThresholdPlot()
{
    // Resetting scene and Observers
    m_view->GetScene()->ClearItems();
    m_view->GetScene()->AddItem( m_chart );

    // Loading data
    bool dataAvailable = LoadData();
    if( dataAvailable )
    {
        m_nbrPlots = m_ordinate.size();

        // Adding entries
        vtkSmartPointer< vtkTable > table = vtkSmartPointer< vtkTable >::New();
        AddEntries( table );

        // Adding data
        SetData( table );

        // Adding lines
        AddQCThresholdLines( table );

        // Setting axis properties and observers
        SetQCThresholdAxisProperties();

//        m_chart->SetInteractive( false );
    }

    return dataAvailable;
}

void Plot::ResetPlotData()
{
    m_directory.clear();
    m_matlabDirectory.clear();
    m_fibername.clear();

    m_csvRawDataFiles.clear();
    m_csvBetaFiles.clear();
    m_csvOmnibusLpvalueFiles.clear();
    m_csvOmnibusFDRLpvalueFiles.clear();
    m_csvConfidenceBandsFiles.clear();
    m_csvPostHocFDRLpvalueFiles.clear();

    m_dataRawData.clear();
    m_subjects.clear();
    m_dataBinaryRawData.clear();
    m_dataBinaryRawStats.clear();
    m_subjectsBinaryRawData.clear();

    m_dataBetasByProperties.clear();
    m_dataBetasByCovariates.clear();

    m_dataOmnibusLpvalue.clear();
    m_dataOmnibusFDRLpvalue.clear();

    m_dataOmnibusFDRSigBetasByProperties.clear();
    m_abscissaOmnibusFDRSigBetasByProperties.clear();
    m_dataOmnibusFDRSigBetasByCovariates.clear();
    m_abscissaOmnibusFDRSigBetasByCovariates.clear();

    m_dataConfidenceBands.clear();

    m_dataPostHocFDRLpvalue.clear();
    m_dataPostHocFDRLpvaluesByCovariates.clear();

    m_dataPostHocFDRSigBetasOnAverageRawData.clear();
    m_abscissaPostHocFDRSigBetasOnAverageRawData.clear();

    m_dataPostHocFDRSigBetasByProperties.clear();
    m_abscissaPostHocFDRSigBetasByProperties.clear();
    m_dataPostHocFDRSigBetasByCovariates.clear();
    m_abscissaPostHocFDRSigBetasByCovariates.clear();

    m_properties.clear();
    m_allCovariates.clear();
    m_covariatesNoIntercept.clear();
    m_binaryCovariates.clear();
    m_abscissa.clear();

    m_plotsAvailable.clear();
    m_lineNames.clear();
    m_ordinate.clear();

    m_matchedSubjects.clear();
    m_atlasQCThreshold.clear();
}

void Plot::ClearPlot()
{
    m_selectedLineLabels.clear();

    m_currentSelectedLines.selectedLines.clear();
    m_currentSelectedLines.isEmpty = true;

    m_previousSelectedLines.selectedLines.clear();
    m_previousSelectedLines.isEmpty = true;

    for( int i = 0; i < m_chart->GetNumberOfPlots(); i++ )
    {
        m_chart->GetPlot( i )->RemoveAllObservers();
    }
    m_chart->ClearPlots();
}


QString& Plot::SetFibername()
{
    return m_fibername;
}

void Plot::SetSelectedPlot( QString plotSelected )
{
    m_plotSelected = plotSelected;
    if( m_plotSelected == "Raw Data" ||
            m_plotSelected == "Raw Stats" )
    {
        if( !m_isBinaryCovariatesSent )
        {
            emit CovariatesAvailableForPlotting( m_binaryCovariates );
            m_isBinaryCovariatesSent = true;
            m_isAllCovariatesSent = false;
            m_isCovariatesNoInterceptSent = false;
        }
    }
    if( m_plotSelected == "Raw Betas by Properties" ||
            m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" ||
            m_plotSelected == "Omnibus Betas with Confidence Bands" )
    {
        if( !m_isAllCovariatesSent )
        {
            emit CovariatesAvailableForPlotting( m_allCovariates );
            m_isBinaryCovariatesSent = false;
            m_isAllCovariatesSent = true;
            m_isCovariatesNoInterceptSent = false;
        }
    }
    if( m_plotSelected == "Raw Betas by Covariates" ||
            m_plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas on Average Raw Data" )
    {
        if( !m_isCovariatesNoInterceptSent )
        {
            emit CovariatesAvailableForPlotting( m_covariatesNoIntercept );
            m_isBinaryCovariatesSent = false;
            m_isAllCovariatesSent = false;
            m_isCovariatesNoInterceptSent = true;
        }
    }
}

QString& Plot::SetSelectedProperty()
{
    return m_propertySelected;
}

QString& Plot::SetSelectedCovariate()
{
    return m_covariateSelected;
}

QMap< int, QPair< QString, QPair< bool, QString > > >& Plot::SetSelectionToDisPlay()
{
    return m_selectionToDisplay;
}

void Plot::SetCovariatesProperties( QMap< int, QPair< QString, QPair< bool, QString > > > covariatesProperties )
{
    m_covariatesProperties = covariatesProperties;
}

void Plot::UpdateLineToDisplay( QMap< int, QPair< QString, QPair< bool, QString > > > selectionToDisplay )
{
    m_selectionToDisplay = selectionToDisplay;

    int shift = ( m_plotSelected == "Omnibus Local pvalues" || m_plotSelected == "Omnibus FDR Local pvalues" ) ? 1 : 0;

    for( int indexPlot = 0; indexPlot < m_chart->GetNumberOfPlots() - 1; indexPlot++ )
    {
        vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( indexPlot );

        if( indexPlot < m_nbrPlots )
        {
            currentLine->SetVisible( m_selectionToDisplay.value( indexPlot + shift ).second.first );
        }
        else
        {
            QString currentName = QString::fromStdString( currentLine->GetLabel() );
            for( int i = 0; i < m_selectionToDisplay.size(); i++ )
            {
                if( currentName.contains( m_selectionToDisplay.value( i ).first ) )
                {
                    currentLine->SetVisible( m_selectionToDisplay.value( i ).second.first );
                }
            }
        }
    }

    SetAxisProperties();

    m_view->Render();
}


void Plot::SetDefaultTitle( bool isBold, bool isItalic, double fontSize )
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
    if( m_plotSelected == "Omnibus Betas with Confidence Bands" )
    {
        title = m_fibername + " Beta Values with Omnibus Confidence Bands " + m_covariateSelected + " (" + m_propertySelected + ")";
    }
    if( m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" )
    {
        title = m_fibername + " Post-Hoc FDR Local pvalues " + m_covariateSelected;
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas on Average Raw Data" )
    {
        title = m_fibername + " " + m_plotSelected + " " + m_covariateSelected + " (" + m_propertySelected + ")";
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
    m_chart->GetTitleProperties()->SetBold( isBold );
    m_chart->GetTitleProperties()->SetItalic( isItalic );
    m_chart->GetTitleProperties()->SetFontSize( fontSize );

    m_view->Render();
}

void Plot::SetCustomizedTitle( QString title, bool isBold, bool isItalic, double fontSize )
{
    m_chart->SetTitle( title.toStdString() );
    m_chart->GetTitleProperties()->SetBold( isBold );
    m_chart->GetTitleProperties()->SetItalic( isItalic );
    m_chart->GetTitleProperties()->SetFontSize( fontSize );

    m_view->Render();
}

bool& Plot::SetGrid()
{
    return m_gridOn;
}

void Plot::UpdateGrid( bool checkState )
{
    m_gridOn = checkState;
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetGridVisible( m_gridOn );
    m_chart->GetAxis( vtkAxis::LEFT )->SetGridVisible( m_gridOn );

    m_view->Render();
}

void Plot::SetAbscissaNotation( bool checkState )
{
    m_abscissaNotation = checkState ? vtkAxis::SCIENTIFIC_NOTATION : vtkAxis::STANDARD_NOTATION;
}

void Plot::UpdateAbscissaNotation( bool checkState )
{
    m_abscissaNotation = checkState ? vtkAxis::SCIENTIFIC_NOTATION : vtkAxis::STANDARD_NOTATION;
    m_chart->GetAxis( vtkAxis::LEFT )->SetNotation( m_abscissaNotation );

    m_view->Render();
}

void Plot::SetDefaultAxis( double labelSize, double NameSize, bool isBold, bool isItalic, bool isYMinSet, double yMin, bool isYMaxSet, double yMax )
{
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetTitle( "Arc Length" );
    m_chart->GetAxis( vtkAxis::BOTTOM )->GetLabelProperties()->SetFontSize( labelSize );
    m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->SetBold( isBold );
    m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->SetItalic( isItalic );
    m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->SetFontSize( NameSize );

    if( m_plotSelected == "Omnibus Local pvalues" ||
            m_plotSelected == "Omnibus FDR Local pvalues" ||
            m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" )
    {
        m_chart->GetAxis( vtkAxis::LEFT )->SetTitle( "-log10" );
    }
    else
    {
        m_chart->GetAxis( vtkAxis::LEFT )->SetTitle( "" );
    }
    m_chart->GetAxis( vtkAxis::LEFT )->GetLabelProperties()->SetFontSize( labelSize );
    m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->SetBold( isBold );
    m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->SetItalic( isItalic );
    m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->SetFontSize( NameSize );

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

    if( !m_abscissa.isEmpty() )
    {
        SetAxisProperties();
    }

    m_view->Render();
}

void Plot::SetCustomizedAxis( double labelSize, QString xName, QString yName, double NameSize, bool isBold, bool isItalic, bool isYMinSet, double yMin, bool isYMaxSet, double yMax )
{
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetTitle( xName.toStdString() );
    m_chart->GetAxis( vtkAxis::BOTTOM )->GetLabelProperties()->SetFontSize( labelSize );
    m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->SetBold( isBold );
    m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->SetItalic( isItalic );
    m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->SetFontSize( NameSize );

    m_chart->GetAxis( vtkAxis::LEFT )->SetTitle( yName.toStdString() );
    m_chart->GetAxis( vtkAxis::LEFT )->GetLabelProperties()->SetFontSize( labelSize );
    m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->SetBold( isBold );
    m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->SetItalic( isItalic );
    m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->SetFontSize( NameSize );

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

    if( !m_abscissa.isEmpty() )
    {
        SetAxisProperties();
    }

    m_view->Render();
}

void Plot::SetLegend( bool showLegend, QString position )
{
    vtkSmartPointer< vtkChartLegend > legend = m_chart->GetLegend();

    int verticalPosition;
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

    int horizontalPosition;
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
    legend->GetBrush()->SetColor( 255, 255, 255, 255 );

    m_chart->SetShowLegend( showLegend && !( m_plotSelected == "Raw Data" ) );

    m_view->Render();
}


double& Plot::SetPvalueThreshold()
{
    return m_pvalueThreshold;
}

void Plot::UpdatePvalueThreshold( double pvalueThreshold )
{
    m_pvalueThreshold = pvalueThreshold;

    if( !m_dataBetasByCovariates.isEmpty() && !m_dataOmnibusFDRLpvalue.isEmpty() )
    {
        SetOmnibusFDRSigBetasByProperties();
        SetOmnibusFDRSigBetasByCovariates();
    }
    if( !m_dataBetasByCovariates.isEmpty() && !m_dataPostHocFDRLpvalue.isEmpty() )
    {
        SetPostHocFDRSigBetasByProperties();
        SetPostHocFDRSigBetasByCovariates();
    }
    if( !m_dataBetasByCovariates.isEmpty() && !m_dataPostHocFDRSigBetasOnAverageRawData .isEmpty() && !m_dataRawData.isEmpty() )
    {
        SetPostHocFDRSigBetasOnAverageRawData();
    }

    if( m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" ||
            m_plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas on Average Raw Data" )
    {
        const vtkSmartPointer< vtkTable >& table = m_chart->GetPlot( 0 )->GetInput();
        int nbrPlotToDelete = m_chart->GetNumberOfPlots() - m_nbrPlots;
        int nbrPlotDeleted = 0;

        while( nbrPlotDeleted != nbrPlotToDelete )
        {
            m_chart->RemovePlot( m_chart->GetNumberOfPlots() - 1 );
            nbrPlotDeleted++;
        }

        if( m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
                m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" ||
                m_plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
                m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
        {
            AddLineBetas( table );
        }
        if( m_plotSelected == "Post-Hoc FDR Significant Betas on Average Raw Data" )
        {
            AddLineSigBetasOnAverageRawData( table );
        }
    }

    if( m_plotSelected == "Omnibus Local pvalues" ||
            m_plotSelected == "Omnibus FDR Local pvalues" ||
            m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" )
    {
        m_chart->RemovePlot( m_chart->GetNumberOfPlots() - 1 );
        AddSignificantLevel( -log10( m_pvalueThreshold ) );
    }

    m_view->Render();
}

double& Plot::SetLineWidth()
{
    return m_lineWidth;
}

void Plot::UpdateLineWidth()
{
    for( int i = 0; i < m_chart->GetNumberOfPlots() ; i++ )
    {
        vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( i );
        double currentLineWidth;

        if( m_plotSelected == "Raw Data" )
        {
            int indexCovariate = m_binaryCovariates.key( m_covariateSelected );
            int lineIndex = LineAlreadySelected( currentLine );
            if( m_covariateSelected.isEmpty() || m_dataRawData.last().at( i ).at( indexCovariate - 1 ) == 0 )
            {
                if( lineIndex != -1 )
                {
                    m_previousSelectedLines.selectedLines[ lineIndex ].lineWidth = m_lineWidth / 2;
                    currentLineWidth = 3 * m_lineWidth;
                }
                else
                {
                    currentLineWidth = m_lineWidth / 2;
                }
            }
            else
            {
                if( lineIndex != -1 )
                {
                    m_previousSelectedLines.selectedLines[ lineIndex ].lineWidth = m_lineWidth;
                    currentLineWidth = 3 * m_lineWidth;
                }
                else
                {
                    currentLineWidth = m_lineWidth;
                }
            }
        }
        else
        {
            if( ( m_plotSelected != "Raw Stats" || m_plotSelected != "Post-Hoc FDR Significant Betas on Average Raw Data" )
                    && ( i == m_chart->GetNumberOfPlots() - 1 ) )
            {
                currentLineWidth = m_lineWidth / 2;
            }
            else
            {
                currentLineWidth = m_lineWidth;
            }
        }

        currentLine->SetWidth( currentLineWidth );
    }

    m_view->Render();
}

void Plot::SetSelectedLineColor( QString color )
{
    m_selectedLineColor[ 0 ] = m_allColors.value( color ).at( 0 );
    m_selectedLineColor[ 1 ] = m_allColors.value( color ).at( 1 );
    m_selectedLineColor[ 2 ] = m_allColors.value( color ).at( 2 );
}

void Plot::UpdateSelectedLineColor( QString color )
{
    SetSelectedLineColor( color );

    if( !m_previousSelectedLines.selectedLines.isEmpty() )
    {
        QMap< int, struct_selectedLine >::ConstIterator iterCurrentSelectedLine = m_previousSelectedLines.selectedLines.cbegin();
        while( iterCurrentSelectedLine != m_previousSelectedLines.selectedLines.cend() )
        {
            vtkSmartPointer< vtkPlot > currentLine = iterCurrentSelectedLine.value().line;

            double* selectedRGB = GetSelectedLineColor();
            currentLine->SetColor( selectedRGB[ 0 ], selectedRGB[ 1 ], selectedRGB[ 2 ], 255 );

            ++iterCurrentSelectedLine;
        }
    }

    m_view->Render();
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

double& Plot::SetMarkerSize()
{
    return m_markerSize;
}

void Plot::UpdateMarker()
{
    if( m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" ||
            m_plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        for( int i = m_nbrPlots; i < m_chart->GetNumberOfPlots() - 1; i++ )
        {
            vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( i );
            vtkPlotPoints::SafeDownCast( currentLine )->SetMarkerStyle( m_markerType );
            vtkPlotPoints::SafeDownCast( currentLine )->SetMarkerSize( m_markerSize );
        }
    }

    if( m_plotSelected == "Post-Hoc FDR Significant Betas on Average Raw Data" )
    {
        int nbrPlotToUpdate = m_chart->GetNumberOfPlots() - m_nbrPlots;
        int nbrPlotUpdated = 0;

        while( nbrPlotUpdated != nbrPlotToUpdate )
        {
            vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( m_chart->GetNumberOfPlots() - 1 - nbrPlotUpdated );
            vtkPlotPoints::SafeDownCast( currentLine )->SetMarkerStyle( m_markerType );
            vtkPlotPoints::SafeDownCast( currentLine )->SetMarkerSize( m_markerSize );

            nbrPlotUpdated++;
        }
    }

    m_view->Render();
}

bool& Plot::SetBinaryBetas()
{
    return m_isPosNeg;
}

void Plot::UpdateBinaryBetas( bool isPosNeg )
{
    m_isPosNeg = isPosNeg;

    if( m_plotSelected == "Post-Hoc FDR Significant Betas on Average Raw Data" && !m_covariateSelected.isEmpty() )
    {
        int nbrPlotToDelete = m_chart->GetNumberOfPlots() - m_nbrPlots;
        int nbrPlotDeleted = 0;

        while( nbrPlotDeleted != nbrPlotToDelete )
        {
            m_chart->RemovePlot( m_chart->GetNumberOfPlots() - 1 );
            nbrPlotDeleted++;
        }

        if( !m_covariateSelected.isEmpty() )
        {
            QList< double > negativeSigBetas = m_dataPostHocFDRSigBetasOnAverageRawData.value( m_propertySelected ).value( "Negative" ).at( m_covariatesNoIntercept.key( m_covariateSelected ) - 1 );
            QList< double > positiveSigBetas = m_dataPostHocFDRSigBetasOnAverageRawData.value( m_propertySelected ).value( "Positive" ).at( m_covariatesNoIntercept.key( m_covariateSelected ) - 1 );
            QList< double > sigBetas = QList< double >() << negativeSigBetas << positiveSigBetas;
            QList< double > abscissaNegativeSigBetas = m_abscissaPostHocFDRSigBetasOnAverageRawData.value( m_propertySelected ).value( "Negative" ).at( m_covariatesNoIntercept.key( m_covariateSelected ) - 1 );
            QList< double > abscissaPositiveSigBetas = m_abscissaPostHocFDRSigBetasOnAverageRawData.value( m_propertySelected ).value( "Positive" ).at( m_covariatesNoIntercept.key( m_covariateSelected ) - 1 );
            QList< double > abscissaSigBetas = QList< double >() << abscissaNegativeSigBetas << abscissaPositiveSigBetas;

            if( m_isPosNeg )
            {
                AddSigBetasOnAverageRawData( positiveSigBetas, abscissaPositiveSigBetas, m_allColors.value( "Lime" ), QString( m_covariateSelected + " Positive SigBetas" ) );
                AddSigBetasOnAverageRawData( negativeSigBetas, abscissaNegativeSigBetas, m_allColors.value( "Red" ), QString( m_covariateSelected + " Negative SigBetas" ) );
            }
            else
            {
                QList< int > markerColorSigBetas = m_allColors.value( m_covariatesProperties.value( m_allCovariates.key( m_covariateSelected ) ).second.second );
                AddSigBetasOnAverageRawData( sigBetas, abscissaSigBetas, markerColorSigBetas, QString( m_covariateSelected + " SigBetas" ) );
            }
        }
    }

    m_view->Render();
}

double& Plot::SetQCThreshold()
{
    return m_qcThreshold;
}

QStringList& Plot::SetAtlasQCThreshold()
{
    return m_atlasQCThreshold;
}

bool& Plot::SetCroppingEnabled()
{
    return m_croppingEnabled;
}

int& Plot::SetArcLengthStartIndex()
{
    return m_arcLengthStartIndex;
}

int& Plot::SetArcLengthEndIndex()
{
    return m_arcLengthEndIndex;
}


void Plot::UpdateCovariatesNames( const QMap< int, QString >& newCovariateName )
{
    QMap< int, QString >::ConstIterator iterNewCovariatesNames = newCovariateName.cbegin();
    while( iterNewCovariatesNames != newCovariateName.cend() )
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

    if( m_plotSelected == "Raw Data" ||
            m_plotSelected == "Raw Stats" )
    {
        emit CovariatesAvailableForPlotting( m_binaryCovariates );
    }
    if( m_plotSelected == "Raw Betas by Properties" ||
            m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
            m_plotSelected == "Omnibus Betas with Confidence Bands" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        emit CovariatesAvailableForPlotting( m_allCovariates );
    }
    if( m_plotSelected == "Raw Betas by Covariates" ||
            m_plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas on Average Raw Data" )
    {
        emit CovariatesAvailableForPlotting( m_covariatesNoIntercept );
    }
}


void Plot::AddSelectedLine( vtkSmartPointer< vtkPlot > newLine )
{
    struct_selectedLine newSelectedLine;
    QString newLabel = QString::fromStdString( newLine->GetLabel() );
    int newSelectedLineIndex = m_currentSelectedLines.selectedLines.size();
    int indexLineAlreadySelected = LineAlreadySelected( newLine );

    if( indexLineAlreadySelected != -1 )
    {
        struct_selectedLine alreadySelectedLine = m_previousSelectedLines.selectedLines.value( indexLineAlreadySelected );
        newSelectedLine.line = alreadySelectedLine.line;
        newSelectedLine.color[ 0 ] = alreadySelectedLine.color[ 0 ];
        newSelectedLine.color[ 1 ] = alreadySelectedLine.color[ 1 ];
        newSelectedLine.color[ 2 ] = alreadySelectedLine.color[ 2 ];
        newSelectedLine.lineWidth = alreadySelectedLine.lineWidth;
    }
    else
    {
        double newRGB[3];
        newLine->GetColor( newRGB );
        double newLineWidth = newLine->GetWidth();

        newSelectedLine.line = newLine;
        newSelectedLine.color[ 0 ] = newRGB[ 0 ];
        newSelectedLine.color[ 1 ] = newRGB[ 1 ];
        newSelectedLine.color[ 2 ] = newRGB[ 2 ];
        newSelectedLine.lineWidth = newLineWidth;
    }

    if( !m_selectedLineLabels.contains( newLabel ) )
    {
        m_currentSelectedLines.selectedLines.insert( newSelectedLineIndex, newSelectedLine );

        m_selectedLineLabels.append( newLabel );
        double index = m_chart->GetPlotIndex( newLine );
        m_selectedLineIndex.append( index );
    }
    m_currentSelectedLines.isEmpty = false;
}

void Plot::UpdateLineSelection()
{
    LinesSelected( m_selectedLineLabels );

    if( !m_previousSelectedLines.isEmpty )
    {
        QMap< int, struct_selectedLine >::ConstIterator iterPreviousSelectedLine = m_previousSelectedLines.selectedLines.cbegin();
        while( iterPreviousSelectedLine != m_previousSelectedLines.selectedLines.cend() )
        {
            struct_selectedLine previousSelectedLine = iterPreviousSelectedLine.value();
            vtkSmartPointer< vtkPlot > previousLine = previousSelectedLine.line;
            double* previousRGB = previousSelectedLine.color;
            double previousLineWidth = previousSelectedLine.lineWidth;

            previousLine->SetColor( 255 * previousRGB[ 0 ], 255 * previousRGB[ 1 ], 255 * previousRGB[ 2 ], 255 );
            previousLine->SetWidth( previousLineWidth );

            ++iterPreviousSelectedLine;
        }
    }

    if( !m_currentSelectedLines.isEmpty )
    {
        m_previousSelectedLines.selectedLines = m_currentSelectedLines.selectedLines;
        m_previousSelectedLines.isEmpty = false;

        QMap< int, struct_selectedLine >::ConstIterator iterCurrentSelectedLine = m_currentSelectedLines.selectedLines.cbegin();
        while( iterCurrentSelectedLine != m_currentSelectedLines.selectedLines.cend() )
        {
            vtkSmartPointer< vtkPlot > currentLine = iterCurrentSelectedLine.value().line;

            double* selectedRGB = GetSelectedLineColor();
            double selectedLineWidth = 3 * GetLineWidth();

            currentLine->SetColor( selectedRGB[ 0 ], selectedRGB[ 1 ], selectedRGB[ 2 ], 255 );
            currentLine->SetWidth( selectedLineWidth );

            ++iterCurrentSelectedLine;
        }

        foreach( int lineIndex, m_selectedLineIndex )
        {
            m_chart->GetPlot( lineIndex )->SetSelection( NULL );
        }

        m_selectedLineLabels.clear();
        m_selectedLineIndex.clear();
        m_currentSelectedLines.selectedLines.clear();
        m_currentSelectedLines.isEmpty = true;
    }
}


void Plot::UpdateNAN( const QStringList& nanSubjects )
{
    for( int plotIndex = m_chart->GetNumberOfPlots(); plotIndex > 0 ; plotIndex-- )
    {
        vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( plotIndex - 1 );
        if( nanSubjects.contains( QString( currentLine->GetLabel() ) ) )
        {
            int currentIndex = m_chart->GetPlotIndex( currentLine );
            m_chart->RemovePlot( currentIndex );
            m_dataRawData[ m_propertySelected ].removeAt( currentIndex );
        }
    }

    m_nbrPlots = m_dataRawData[ m_propertySelected ].size();

    UpdateQCThreshold( m_qcThreshold, false );

    m_view->Render();
}

void Plot::UpdateQCThreshold( double qcThreshold, bool emitSignal )
{
    m_qcThreshold = qcThreshold;
    QList< double > atlas = m_processing.QStringListToDouble( m_atlasQCThreshold );
    QList< double > refLine = !atlas.isEmpty() ? atlas : m_processing.GetMean( m_dataRawData.value( m_propertySelected ), 0 );
    QStringList subjectsCorrelated, subjectsNotCorrelated;

    for( int i = 0; i < m_nbrPlots; i++ )
    {
        vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( i );
        QList< int > color;

        double pearsonCorrelation = m_processing.ApplyPearsonCorrelation( m_dataRawData.value( m_propertySelected ).at( i ), refLine, 0 );

        if( pearsonCorrelation < m_qcThreshold )
        {
            subjectsNotCorrelated.append( QString( currentLine->GetLabel() ) );

            color = m_allColors.value( "Red" );
            currentLine->GetPen()->SetLineType( vtkPen::DASH_LINE );
        }
        else
        {
            subjectsCorrelated.append( QString( currentLine->GetLabel() ) );

            color = m_allColors.value( "Carolina Blue" );
            currentLine->GetPen()->SetLineType( vtkPen::SOLID_LINE );
        }

        currentLine->SetColor(  color.first(), color.at( 1 ), color.at( 2 ), 255 );
        currentLine->SetWidth( m_lineWidth );
    }

    if( emitSignal )
    {
        emit UpdateSubjectsCorrelated( subjectsCorrelated, subjectsNotCorrelated );
    }

    m_view->Render();
}

void Plot::UpdateCropping()
{
    AddCrop( true );

    m_view->Render();
}

void Plot::ShowHideProfileCropping( bool show )
{
    m_chart->GetPlot( m_chart->GetNumberOfPlots() - 1 )->SetVisible( show );
    m_chart->GetPlot( m_chart->GetNumberOfPlots() - 2 )->SetVisible( show );

    m_view->Render();
}


/***************************************************************/
/************************ Public  slots ************************/
/***************************************************************/
void Plot::OnSavePlot()
{
    QString fileName = m_matlabDirectory + "/" +  m_fibername + "_";
    if( m_plotSelected == "Raw Data" )
    {
        fileName += m_plotSelected.split( " " ).join( "_" ) + "_" + m_covariateSelected + "_" + m_propertySelected;
    }
    if( m_plotSelected == "Raw Stats" )
    {
        fileName += "AverageStdDeviation_" + m_covariateSelected + "_" + m_propertySelected;
    }
    if( m_plotSelected == "Raw Betas by Properties" )
    {
        fileName += "Betas_" + m_propertySelected;
    }
    if( m_plotSelected == "Raw Betas by Covariates" )
    {
        fileName += "Betas_" + m_covariateSelected;
    }
    if( m_plotSelected == "Omnibus Local pvalues" ||
            m_plotSelected == "Omnibus FDR Local pvalues" )
    {
        fileName += m_plotSelected.split( " " ).join( "_" );
    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Properties" )
    {
        fileName += "Omnibus_FDR_SigBetas_" + m_propertySelected;
    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Covariates" )
    {
        fileName += "Omnibus_FDR_SigBetas_" + m_covariateSelected;
    }
    if( m_plotSelected == "Omnibus Betas with Confidence Bands" )
    {
        fileName += "Betas_Omnibus_Confidence_Bands_" + m_covariateSelected + "_" + m_propertySelected;
    }
    if( m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" )
    {
        fileName += "PostHoc_FDR_Local_pvalues_" + m_covariateSelected;
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas on Average Raw Data" )
    {
        fileName += "PostHoc_FDR_SigBetas_Average_Raw_Data_" + m_covariateSelected + "_" + m_propertySelected;
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        fileName += "PostHoc_FDR_SigBetas_" + m_propertySelected;
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        fileName += "PostHoc_FDR_SigBetas_" + m_covariateSelected;
    }

    fileName +=".eps";

    QString filePath = QFileDialog::getSaveFileName( m_qvtkWidget.data(), tr( "Save plot as ..." ), fileName, tr( ".eps ( *.eps ) ;; .*( * )" ) );
    if( !filePath.isEmpty() )
    {
        SavePlot( filePath );
    }
}


/***************************************************************/
/********************** Private functions **********************/
/***************************************************************/
void Plot::SortFilesByProperties( QString directory, const QStringList& files, QMap< QString, QList< QList< double > > >& data, int dataKind )
{
    bool adFound = false;
    bool mdFound = false;
    bool rdFound = false;
    bool faFound = false;
    bool subMatrixFound = false;

    foreach( QString path, files )
    {
        QString currentPath = directory + ( ( directory.endsWith( "/" ) || path.startsWith( "/" ) ) ? "" : "/" ) + path;
        QString index;
        if( ( path.contains( "_ad_", Qt::CaseInsensitive ) || path.contains( "_ad.csv", Qt::CaseInsensitive ) ) && !adFound )
        {
            adFound = true;
            index = "AD";
        }
        if( ( path.contains( "_md_", Qt::CaseInsensitive ) || path.contains( "_md.csv", Qt::CaseInsensitive ) ) && !mdFound )
        {
            mdFound = true;
            index = "MD";
        }
        if( ( path.contains( "_rd_", Qt::CaseInsensitive ) || path.contains( "_rd.csv", Qt::CaseInsensitive ) ) && !rdFound )
        {
            rdFound = true;
            index = "RD";
        }
        if( ( path.contains( "_fa_", Qt::CaseInsensitive ) || path.contains( "_fa.csv", Qt::CaseInsensitive ) ) && !faFound )
        {
            faFound = true;
            index = "FA";
        }
        if( ( path.contains( "_subMatrix_", Qt::CaseInsensitive ) || path.contains( "_subMatrix.csv", Qt::CaseInsensitive ) ||
              path.contains( "_comp_", Qt::CaseInsensitive ) || path.contains( "_comp.csv", Qt::CaseInsensitive ) ) && !subMatrixFound )
        {
            subMatrixFound = true;
            index = "SUBMATRIX";
        }

        if( !index.isNull() )
        {
            QList< QStringList > dataTemp = m_processing.GetDataFromFile( currentPath );

            if( dataTemp.first().first().contains( "arclength", Qt::CaseInsensitive ) || dataKind == RawData )
            {
                if( dataKind == RawData )
                {
                    if( index == "SUBMATRIX" )
                    {
                        SetCovariates( dataTemp, RawData );
                        dataTemp.removeFirst();
                        dataTemp = m_processing.Transpose( dataTemp );
                        SetSubjects( dataTemp );
                        dataTemp.removeFirst();
                        dataTemp = m_processing.Transpose( dataTemp );
                    }
                    else
                    {
                        dataTemp.removeFirst();
                        dataTemp = m_processing.Transpose( dataTemp );
                        if( m_abscissa.isEmpty() )
                        {
                            m_abscissa = m_processing.QStringListToDouble( dataTemp.first() );
                            m_nbrPoints = m_abscissa.size();
                        }
                        dataTemp.removeFirst();
                    }
                }

                if( dataKind == Betas || dataKind == ConfidenceBands )
                {
                    dataTemp = m_processing.Transpose( dataTemp );
                    if( m_allCovariates.isEmpty() )
                    {
                        SetCovariates( dataTemp, dataKind );
                    }
                    dataTemp.removeFirst();
                    dataTemp = m_processing.Transpose( dataTemp );
                    if( m_abscissa.isEmpty() )
                    {
                        m_abscissa = m_processing.QStringListToDouble( dataTemp.first() );
                        m_nbrPoints = m_abscissa.size();
                    }
                    dataTemp.removeFirst();
                }

                if( dataKind == PostHocFDRLpvalues )
                {
                    if( m_allCovariates.isEmpty() )
                    {
                        SetCovariates( dataTemp, PostHocFDRLpvalues );
                    }
                    dataTemp.removeFirst();
                    dataTemp = m_processing.Transpose( dataTemp );
                    if( m_abscissa.isEmpty() )
                    {
                        m_abscissa = m_processing.QStringListToDouble( dataTemp.first() );
                        m_nbrPoints = m_abscissa.size();
                    }
                    dataTemp.removeFirst();
                }

                data.insert( index, m_processing.DataToDouble( dataTemp ) );
            }
        }
    }

    if( m_properties.isEmpty() )
    {
        SetProperties( data.keys() );
    }
}


void Plot::SetRawData()
{
    foreach( QString property, m_properties )
    {
        QList< QList< double > > rawData = m_dataRawData.value( property );

        foreach( int binaryCovariateIndex, m_binaryCovariates.keys() )
        {
            QList< QList< double > > binaryRawData0;
            QList< QList< double > > binaryRawData1;
            QStringList subjects0;
            QStringList subjects1;

            for( int i = 0; i < rawData.size(); i++ )
            {
                if( m_dataRawData.last().at( i ).at( binaryCovariateIndex - 1 ) == 0 )
                {
                    binaryRawData0.append( rawData.at( i ) );
                    subjects0.append( m_subjects.at( i ) );
                }
                else
                {
                    binaryRawData1.append( rawData.at( i ) );
                    subjects1.append( m_subjects.at( i ) );
                }
            }

            m_dataBinaryRawData[ property ][  binaryCovariateIndex ][ 0 ] = binaryRawData0;
            m_dataBinaryRawData[ property ][ binaryCovariateIndex ][ 1 ] = binaryRawData1;
            m_subjectsBinaryRawData[ property ][ binaryCovariateIndex ][ 0 ] = subjects0;
            m_subjectsBinaryRawData[ property ][ binaryCovariateIndex ][ 1 ] = subjects1;
        }
    }
}

void Plot::SetRawSats()
{
    foreach( QString property, m_properties )
    {
        foreach( int binaryCovariateIndex, m_binaryCovariates.keys() )
        {
            QList< QList< double > > rawData = m_dataRawData.value( property );
            QList< double > rawDataUp;
            QList< double > rawDataMean;
            QList< double > rawDataDown;
            QList< QList< double > > tempRawStats;

            QList< QList< double > > binaryRawData0 = m_dataBinaryRawData.value( property ).value( binaryCovariateIndex ).value( 0 );
            QList< double > binaryRawData0Up;
            QList< double > binaryRawData0Mean;
            QList< double > binaryRawData0Down;
            QList< QList< double > > tempRawStats0;

            QList< QList< double > > binaryRawData1 = m_dataBinaryRawData.value( property ).value( binaryCovariateIndex ).value( 1 );
            QList< double > binaryRawData1Up;
            QList< double > binaryRawData1Mean;
            QList< double > binaryRawData1Down;
            QList< QList< double > > tempRawStats1;


            if( !rawData.isEmpty() )
            {
                ProcessRawStats( rawData, rawDataMean, rawDataUp, rawDataDown );
                tempRawStats.append( rawDataUp );
                tempRawStats.append( rawDataMean );
                tempRawStats.append( rawDataDown );
            }
            if( !binaryRawData0.isEmpty() )
            {
                ProcessRawStats( binaryRawData0, binaryRawData0Mean, binaryRawData0Up, binaryRawData0Down );
                tempRawStats0.append( binaryRawData0Up );
                tempRawStats0.append( binaryRawData0Mean );
                tempRawStats0.append( binaryRawData0Down );
            }
            if( !binaryRawData1.isEmpty() )
            {
                ProcessRawStats( binaryRawData1, binaryRawData1Mean, binaryRawData1Up, binaryRawData1Down );
                tempRawStats1.append( binaryRawData1Up );
                tempRawStats1.append( binaryRawData1Mean );
                tempRawStats1.append( binaryRawData1Down);
            }

            m_dataBinaryRawStats[ property ][ -1 ][ -1 ] = tempRawStats;
            m_dataBinaryRawStats[ property ][ binaryCovariateIndex ][ 0 ] = tempRawStats0;
            m_dataBinaryRawStats[ property ][ binaryCovariateIndex ][ 1 ] = tempRawStats1;
        }
    }
}

void Plot::SetBetasByCovariate()
{
    foreach( int covariateIndex, m_covariatesNoIntercept.keys() )
    {
        QList< QList< double > > tempBetasByCovariates;
        foreach( QString property, m_properties.values() )
        {
            tempBetasByCovariates.append( m_dataBetasByProperties.value( property ).at( covariateIndex ) );
        }

        m_dataBetasByCovariates.insert( covariateIndex, tempBetasByCovariates );
    }
}

void Plot::SetOmnibusLpvalue( const QStringList& omnibusLpvalueFiles, QList< QList< double > >& omnibusLpvaluesData )
{
    QString currentPath = m_matlabDirectory + "/" + omnibusLpvalueFiles.first();
    QList< QStringList > dataTemp = m_processing.GetDataFromFile( currentPath );

    if( dataTemp.first().first().contains( "arclength", Qt::CaseInsensitive ) )
    {
        if( m_allCovariates.isEmpty() )
        {
            SetCovariates( dataTemp, OmnibusLpvalues );
        }
        dataTemp.removeFirst();
        dataTemp = m_processing.Transpose( dataTemp );
        if( m_abscissa.isEmpty() )
        {
            m_abscissa = m_processing.QStringListToDouble( dataTemp.first() );
            m_nbrPoints = m_abscissa.size();
        }
        dataTemp.removeFirst();

        omnibusLpvaluesData = m_processing.DataToDouble( dataTemp );
    }
}

void Plot::SetOmnibusFDRSigBetasByProperties()
{
    m_dataOmnibusFDRSigBetasByProperties.clear();
    m_abscissaOmnibusFDRSigBetasByProperties.clear();

    foreach( QString property, m_properties.values() )
    {
        QList< QList< double > > tempOmnibusFDRSigBetasByProperties;
        QList< QList< double > > tempAbscissaOmnibusFDRSigBetasByProperties;

        foreach( int covariateIndex, m_covariatesNoIntercept.keys() )
        {
            QList< double > tempData;
            QList< double > tempAbscissa;

            for( int i = 0; i < m_nbrPoints; i++ )
            {
                if( m_dataOmnibusFDRLpvalue.at( covariateIndex - 1 ).at( i ) <= m_pvalueThreshold )
                {
                    tempData.append( m_dataBetasByProperties.value( property ).at( covariateIndex ).at( i ) );
                    tempAbscissa.append( m_abscissa.at( i ) );
                }
            }

            tempOmnibusFDRSigBetasByProperties.append( tempData );
            tempAbscissaOmnibusFDRSigBetasByProperties.append( tempAbscissa );
        }

        m_dataOmnibusFDRSigBetasByProperties[ property ] = tempOmnibusFDRSigBetasByProperties;
        m_abscissaOmnibusFDRSigBetasByProperties[ property ] = tempAbscissaOmnibusFDRSigBetasByProperties;
    }
}

void Plot::SetOmnibusFDRSigBetasByCovariates()
{
    m_dataOmnibusFDRSigBetasByCovariates.clear();
    m_abscissaOmnibusFDRSigBetasByCovariates.clear();

    foreach( int covariateIndex, m_covariatesNoIntercept.keys() )
    {
        QList< QList< double > > tempOmnibusFDRSigBetasByCovariates;
        QList< QList< double > > tempAbscissaOmnibusFDRSigBetasByCovariates;

        foreach( QString property, m_properties.values() )
        {
            QList< double > tempData;
            QList< double > tempAbscissa;

            for( int i = 0; i < m_nbrPoints; i++ )
            {
                if( m_dataOmnibusFDRLpvalue.at( covariateIndex - 1 ).at( i ) <= m_pvalueThreshold )
                {
                    tempData.append( m_dataBetasByProperties.value( property ).at( covariateIndex ).at( i ) );
                    tempAbscissa.append( m_abscissa.at( i ) );
                }
            }

            tempOmnibusFDRSigBetasByCovariates.append( tempData );
            tempAbscissaOmnibusFDRSigBetasByCovariates.append( tempAbscissa );
        }

        m_dataOmnibusFDRSigBetasByCovariates[ covariateIndex ] = tempOmnibusFDRSigBetasByCovariates;
        m_abscissaOmnibusFDRSigBetasByCovariates[ covariateIndex ] = tempAbscissaOmnibusFDRSigBetasByCovariates;
    }
}

void Plot::SetPostHocFDRLpvaluesByCovariates()
{
    foreach( int covariateIndex, m_covariatesNoIntercept.keys() )
    {
        QList< QList< double > > tempPostHocFDRLpvaluesByCovariates;

        foreach( QString property, m_properties.values() )
        {
            tempPostHocFDRLpvaluesByCovariates.append( m_dataPostHocFDRLpvalue.value( property ).at( covariateIndex - 1 ) );
        }

        m_dataPostHocFDRLpvaluesByCovariates[ covariateIndex ] = ToLog10( tempPostHocFDRLpvaluesByCovariates );
    }
}

void Plot::SetPostHocFDRSigBetasOnAverageRawData()
{
    m_dataPostHocFDRSigBetasOnAverageRawData.clear();
    m_dataPostHocFDRSigBetasOnAverageRawData.clear();
    m_abscissaPostHocFDRSigBetasOnAverageRawData.clear();
    m_abscissaPostHocFDRSigBetasOnAverageRawData.clear();

    foreach( QString property, m_properties.values() )
    {
        QList< double > meanBetas = m_processing.GetMean( m_dataRawData.value( property ), 0 );
        QList< QList< double > > tempPostHocFDRSigBetasOnAverageRawData_Positive;
        QList< QList< double > > tempPostHocFDRSigBetasOnAverageRawData_Negative;
        QList< QList< double > > tempAbscissaPostHocFDRSigBetasOnAverageRawData_Positive;
        QList< QList< double > > tempAbscissaPostHocFDRSigBetasOnAverageRawData_Negative;

        foreach( int covariateIndex, m_covariatesNoIntercept.keys() )
        {
            QList< double > tempData_Positive;
            QList< double > tempData_Negative;
            QList< double > tempAbscissa_Positive;
            QList< double > tempAbscissa_Negative;

            for( int i = 0; i < m_nbrPoints; i++ )
            {
                if( m_dataPostHocFDRLpvalue.value( property ).at( covariateIndex - 1 ).at( i ) <= m_pvalueThreshold )
                {
                    double sigBeta = m_dataBetasByProperties.value( property ).at( covariateIndex ).at( i );
                    if( sigBeta > 0 )
                    {
                        tempData_Positive.append( meanBetas.at( i ) );
                        tempAbscissa_Positive.append( m_abscissa.at( i ) );
                    }
                    else
                    {
                        tempData_Negative.append( meanBetas.at( i ) );
                        tempAbscissa_Negative.append( m_abscissa.at( i ) );
                    }

                }
            }

            tempPostHocFDRSigBetasOnAverageRawData_Positive.append( tempData_Positive );
            tempPostHocFDRSigBetasOnAverageRawData_Negative.append( tempData_Negative );
            tempAbscissaPostHocFDRSigBetasOnAverageRawData_Positive.append( tempAbscissa_Positive );
            tempAbscissaPostHocFDRSigBetasOnAverageRawData_Negative.append( tempAbscissa_Negative );
        }

        m_dataPostHocFDRSigBetasOnAverageRawData[ property ][ "Positive" ] = tempPostHocFDRSigBetasOnAverageRawData_Positive;
        m_dataPostHocFDRSigBetasOnAverageRawData[ property ][ "Negative" ] = tempPostHocFDRSigBetasOnAverageRawData_Negative;
        m_abscissaPostHocFDRSigBetasOnAverageRawData[ property ][ "Positive" ] = tempAbscissaPostHocFDRSigBetasOnAverageRawData_Positive;
        m_abscissaPostHocFDRSigBetasOnAverageRawData[ property ][ "Negative" ] = tempAbscissaPostHocFDRSigBetasOnAverageRawData_Negative;
    }
}

void Plot::SetPostHocFDRSigBetasByProperties()
{
    m_dataPostHocFDRSigBetasByProperties.clear();
    m_abscissaPostHocFDRSigBetasByProperties.clear();

    foreach( QString property, m_properties.values() )
    {
        QList< QList< double > > tempPostHocFDRSigBetasByProperties;
        QList< QList< double > > tempAbscissaPostHocFDRSigBetasByProperties;

        foreach( int covariateIndex, m_covariatesNoIntercept.keys() )
        {
            QList< double > tempData;
            QList< double > tempAbscissa;

            for( int i = 0; i < m_nbrPoints; i++ )
            {
                if( m_dataPostHocFDRLpvalue.value( property ).at( covariateIndex - 1 ).at( i ) <= m_pvalueThreshold )
                {
                    tempData.append( m_dataBetasByProperties.value( property ).at( covariateIndex ).at( i ) );
                    tempAbscissa.append( m_abscissa.at( i ) );
                }
            }

            tempPostHocFDRSigBetasByProperties.append( tempData );
            tempAbscissaPostHocFDRSigBetasByProperties.append( tempAbscissa );
        }

        m_dataPostHocFDRSigBetasByProperties[ property ] = tempPostHocFDRSigBetasByProperties;
        m_abscissaPostHocFDRSigBetasByProperties[ property ] = tempAbscissaPostHocFDRSigBetasByProperties;
    }
}

void Plot::SetPostHocFDRSigBetasByCovariates()
{
    m_dataPostHocFDRSigBetasByCovariates.clear();
    m_abscissaPostHocFDRSigBetasByCovariates.clear();

    foreach( int covariateIndex, m_covariatesNoIntercept.keys() )
    {
        QList< QList< double > > tempPostHocFDRSigBetasByCovariates;
        QList< QList< double > > tempAbscissaPostHocFDRSigBetasByCovariates;

        foreach( QString property, m_properties.values() )
        {
            QList< double > tempData;
            QList< double > tempAbscissa;

            for( int i = 0; i < m_nbrPoints; i++ )
            {
                if( m_dataPostHocFDRLpvalue.value( property ).at( covariateIndex - 1 ).at( i ) <= m_pvalueThreshold )
                {
                    tempData.append( m_dataBetasByProperties.value( property ).at( covariateIndex ).at( i ) );
                    tempAbscissa.append( m_abscissa.at( i ) );
                }
            }

            tempPostHocFDRSigBetasByCovariates.append( tempData );
            tempAbscissaPostHocFDRSigBetasByCovariates.append( tempAbscissa );
        }

        m_dataPostHocFDRSigBetasByCovariates[ covariateIndex ] = tempPostHocFDRSigBetasByCovariates;
        m_abscissaPostHocFDRSigBetasByCovariates[ covariateIndex ] = tempAbscissaPostHocFDRSigBetasByCovariates;
    }
}

bool Plot::SetRawDataQCThreshold( const QList< QStringList >& rawData )
{
    QList< QStringList > faData = rawData;
    faData.removeFirst();
    faData = m_processing.Transpose( faData );
    faData.removeFirst();

    bool subjectsRemoved;
    if( !m_matchedSubjects.isEmpty() )
    {
        QList< QStringList > tempData = m_processing.Transpose( rawData );

        QStringList arcLength = tempData.first();
        arcLength.removeFirst();
        m_abscissa = m_processing.QStringListToDouble( arcLength );
        m_nbrPoints = m_abscissa.size();

        subjectsRemoved = m_processing.RemoveUnmatchedSubjects( tempData, m_subjects, m_matchedSubjects );
        tempData = m_processing.Transpose( tempData );
        tempData.removeFirst();
        tempData = m_processing.Transpose( tempData );

        QList< QList< double > > tempDoubleData = m_processing.DataToDouble( tempData );
        m_processing.NANToZeros( tempDoubleData );
        m_dataRawData.insert( m_propertySelected, tempDoubleData );
    }

    return !m_dataRawData.value( m_propertySelected ).isEmpty() && subjectsRemoved;
}


void Plot::GetRawDataFiles()
{
    QStringList nameFilterRawData = QStringList() << "*_RawData_*.csv" << "*_RawData.csv";
    m_csvRawDataFiles = QDir( m_directory ).entryList( nameFilterRawData, QDir::Files );
    m_csvRawDataFiles.sort();
    SortFilesByProperties( m_directory, m_csvRawDataFiles, m_dataRawData, RawData );

    if( !m_dataRawData.isEmpty() )
    {
        SetRawData();
        SetRawSats();
    }
}

void Plot::GetBetaFiles()
{
    QStringList nameFilterBeta = QStringList() << "*_Betas_*.csv" << "*_Betas.csv";
    m_csvBetaFiles = QDir( m_matlabDirectory ).entryList( nameFilterBeta, QDir::Files );
    m_csvBetaFiles.sort();
    SortFilesByProperties( m_matlabDirectory, m_csvBetaFiles, m_dataBetasByProperties, Betas );

    if( !m_dataBetasByProperties.isEmpty() )
    {
        SetBetasByCovariate();
    }
}

void Plot::GetOmnibusLpvalueFiles()
{
    QStringList nameFilterOmnibusLpvalue( "*Omnibus_Local_pvalues.csv" );
    m_csvOmnibusLpvalueFiles = QDir( m_matlabDirectory ).entryList( nameFilterOmnibusLpvalue, QDir::Files );
    m_csvOmnibusLpvalueFiles.sort();

    if( !m_csvOmnibusLpvalueFiles.isEmpty() )
    {
        SetOmnibusLpvalue( m_csvOmnibusLpvalueFiles, m_dataOmnibusLpvalue );
    }
}

void Plot::GetOmnibusFDRLpvalueFiles()
{
    QStringList nameFilterOmnibusFDRLpvalue( "*Omnibus_FDR_Local_pvalues.csv" );
    m_csvOmnibusFDRLpvalueFiles = QDir( m_matlabDirectory ).entryList( nameFilterOmnibusFDRLpvalue, QDir::Files );
    m_csvOmnibusFDRLpvalueFiles.sort();

    if( !m_csvOmnibusFDRLpvalueFiles.isEmpty() )
    {
        SetOmnibusLpvalue( m_csvOmnibusFDRLpvalueFiles, m_dataOmnibusFDRLpvalue );
    }

    if( !m_dataBetasByCovariates.isEmpty() && !m_dataOmnibusFDRLpvalue.isEmpty() )
    {
        SetOmnibusFDRSigBetasByProperties();
        SetOmnibusFDRSigBetasByCovariates();
    }
}

void Plot::GetConfidenceBandsFiles()
{
    QStringList nameFilterConfidenceBands( "*_Omnibus_ConfidenceBands_*.csv" );
    m_csvConfidenceBandsFiles = QDir( m_matlabDirectory ).entryList( nameFilterConfidenceBands, QDir::Files );
    m_csvConfidenceBandsFiles.sort();
    SortFilesByProperties( m_matlabDirectory, m_csvConfidenceBandsFiles, m_dataConfidenceBands, ConfidenceBands );
}

void Plot::GetPostHocFDRLpvalueFiles()
{
    QStringList nameFilterPostHocFDRLpvalue = QStringList() << "*_PostHoc_FDR_Local_pvalues_*.csv" << "*_PostHoc_FDR_Local_pvalues.csv";
    m_csvPostHocFDRLpvalueFiles = QDir( m_matlabDirectory ).entryList( nameFilterPostHocFDRLpvalue, QDir::Files );
    m_csvPostHocFDRLpvalueFiles.sort();
    SortFilesByProperties( m_matlabDirectory, m_csvPostHocFDRLpvalueFiles, m_dataPostHocFDRLpvalue, PostHocFDRLpvalues );

    if( !m_dataBetasByCovariates.isEmpty() && !m_dataPostHocFDRLpvalue.isEmpty() )
    {
        SetPostHocFDRLpvaluesByCovariates();
        if( !m_dataRawData.isEmpty() )
        {
            SetPostHocFDRSigBetasOnAverageRawData();
        }
        SetPostHocFDRSigBetasByProperties();
        SetPostHocFDRSigBetasByCovariates();
    }
}


bool Plot::SetPlots()
{
    m_plotsAvailable.clear();
    if( !m_abscissa.isEmpty() )
    {
        if( !m_properties.isEmpty() && !m_allCovariates.isEmpty() && !m_covariatesNoIntercept.isEmpty() && !m_binaryCovariates.isEmpty() )
        {
            if( !m_dataBinaryRawData.isEmpty() )
            {
                m_plotsAvailable.append( QStringList() << "Raw Data" );
            }
            if( !m_dataBinaryRawStats.isEmpty() )
            {
                m_plotsAvailable.append( QStringList() << "Raw Stats" );
            }
        }
        if( !m_properties.isEmpty() && !m_dataBetasByProperties.isEmpty() )
        {
            m_plotsAvailable.append( QStringList() << "Raw Betas by Properties" );
        }
        if( !m_allCovariates.isEmpty() && !m_covariatesNoIntercept.isEmpty() && !m_dataBetasByCovariates.isEmpty() )
        {
            m_plotsAvailable.append( QStringList() << "Raw Betas by Covariates" );
        }
        if( !m_dataOmnibusLpvalue.isEmpty() )
        {
            m_plotsAvailable.append( QStringList() << "Omnibus Local pvalues" );
        }
        if( !m_dataOmnibusFDRLpvalue.isEmpty() )
        {
            m_plotsAvailable.append( QStringList() << "Omnibus FDR Local pvalues" );
        }
        if( !m_properties.isEmpty() && !m_dataOmnibusFDRSigBetasByProperties.isEmpty() )
        {
            m_plotsAvailable.append( QStringList() << "Omnibus FDR Significant Betas by Properties" );
        }
        if( !m_allCovariates.isEmpty() && !m_covariatesNoIntercept.isEmpty() && !m_dataOmnibusFDRSigBetasByCovariates.isEmpty() )
        {
            m_plotsAvailable.append( QStringList() << "Omnibus FDR Significant Betas by Covariates" );
        }
        if( !m_properties.isEmpty() && !m_allCovariates.isEmpty() && !m_covariatesNoIntercept.isEmpty() && !m_dataBetasByProperties.isEmpty() && !m_dataConfidenceBands.isEmpty() )
        {
            m_plotsAvailable.append( QStringList() << "Omnibus Betas with Confidence Bands" );
        }
        if( !m_allCovariates.isEmpty() && !m_covariatesNoIntercept.isEmpty() && !m_dataPostHocFDRLpvaluesByCovariates.isEmpty() )
        {
            m_plotsAvailable.append( QStringList() << "Post-Hoc FDR Local pvalues by Covariates" );
        }
        if( !m_properties.isEmpty() && !m_allCovariates.isEmpty() && !m_covariatesNoIntercept.isEmpty() && !m_dataBinaryRawData.isEmpty() && !m_dataPostHocFDRSigBetasOnAverageRawData.isEmpty() )
        {
            m_plotsAvailable.append( QStringList() << "Post-Hoc FDR Significant Betas on Average Raw Data" );
        }
        if( !m_properties.isEmpty() && !m_dataPostHocFDRSigBetasByProperties.isEmpty() )
        {
            m_plotsAvailable.append( QStringList() << "Post-Hoc FDR Significant Betas by Properties" );
        }
        if( !m_allCovariates.isEmpty() && !m_covariatesNoIntercept.isEmpty() && !m_dataPostHocFDRSigBetasByCovariates.isEmpty() )
        {
            m_plotsAvailable.append( QStringList() << "Post-Hoc FDR Significant Betas by Covariates" );
        }
    }

    bool plotsAvailable = !m_plotsAvailable.isEmpty();
    if( plotsAvailable )
    {
        emit PlotsUsed( m_plotsAvailable );
    }

    return plotsAvailable;
}

void Plot::SetProperties( const QStringList& properties )
{
    foreach( QString property, properties )
    {
        if( property.contains( "ad", Qt::CaseInsensitive ) )
        {
            m_properties.insert( 0, "AD" );
        }
        if( property.contains( "rd", Qt::CaseInsensitive ) )
        {
            m_properties.insert( 1, "RD" );
        }
        if( property.contains( "md", Qt::CaseInsensitive ) )
        {
            m_properties.insert( 2, "MD" );
        }
        if( property.contains( "fa", Qt::CaseInsensitive ) )
        {
            m_properties.insert( 3, "FA" );
        }
    }

    emit AllPropertiesUsed( m_properties );
}

void Plot::SetSubjects( const QList< QStringList >& rawDataSubMatrix )
{
    m_subjects.append( rawDataSubMatrix.first() );
}

void Plot::SetCovariates( const QList< QStringList >& data, int dataKind )
{
    QStringList firstRaw = data.first();
    int shift = 0;
    if( dataKind == Betas )
    {
        shift++;
    }
    if( dataKind == ConfidenceBands )
    {
        shift+=2;
    }

    m_allCovariates.insert( 0, "Intercept" );
    for( int i = shift + 1; i < firstRaw.size(); i++ )
    {
        m_allCovariates.insert( i - shift, firstRaw.at( i ).split( " " ).first() );
        m_covariatesNoIntercept.insert( i - shift, firstRaw.at( i ).split( " " ).first() );
        if( dataKind == ConfidenceBands )
        {
            i++;
            shift++;
        }
    }

    if( dataKind == RawData )
    {
        QStringList firstDataRaw = data.at( 1 );
        for( int i = 1; i < firstRaw.size(); i++ )
        {
            if( ( firstDataRaw .at( i ).toDouble() == 0 ) || ( firstDataRaw.at( i ).toDouble() == 1 ) )
            {
                m_binaryCovariates.insert( i, firstRaw.at( i ) );
            }
        }
    }

    emit AllCovariatesUsed( m_allCovariates );
}


void Plot::GetMeanAndStdDv( const QList< QList< double > >& binaryRawData, QList< double >& tempMean, QList< double >& tempStdDv )
{
    int nbrSubjects = binaryRawData.size();
    for( int i = 0; i < m_nbrPoints; i++ )
    {
        double currentMean = 0;
        for( int j = 0; j < nbrSubjects; j++ )
        {
            currentMean += binaryRawData.at( j ).at( i );
        }
        currentMean = currentMean / nbrSubjects;
        tempMean.append( currentMean );

        double currentStdDv = 0;
        for( int j = 0; j < nbrSubjects; j++ )
        {
            currentStdDv += std::pow( binaryRawData.at( j ).at( i ) - currentMean, 2 );
        }
        tempStdDv.append( std::sqrt( currentStdDv / ( nbrSubjects - 1 ) ) );
    }
}


void Plot::ProcessRawStats( const QList< QList< double > >& binaryRawData, QList< double >& binaryRawDataMean, QList< double >& binaryRawDataUp, QList< double >& binaryRawDataDown )
{
    QList< double > binaryRawDataStdDv;
    GetMeanAndStdDv( binaryRawData, binaryRawDataMean, binaryRawDataStdDv );
    for( int i = 0; i < m_nbrPoints; i++ )
    {
        binaryRawDataUp.append( binaryRawDataMean.at( i ) + binaryRawDataStdDv.at( i ) );
        binaryRawDataDown.append( binaryRawDataMean.at( i ) - binaryRawDataStdDv.at( i ) );
    }
}

void Plot::SetSelectionToDisplayProperties()
{
    m_lineNames.clear();
    m_lineColors.clear();

    QMap< int, QPair< QString, QPair< bool, QString > > >::ConstIterator iterSelectionToDisplay = m_selectionToDisplay.cbegin();
    while( iterSelectionToDisplay != m_selectionToDisplay.cend() )
    {
        m_lineNames.append( iterSelectionToDisplay.value().first );
        m_lineColors.append( m_allColors.value( iterSelectionToDisplay.value().second.second ) );

        ++iterSelectionToDisplay;
    }
}

QList< QList< double > > Plot::ToLog10( const QList< QList< double > >& data )
{
    QList< QList< double > > dataToLog10;
    foreach( QList< double > rowData, data )
    {
        QList< double > rowDataTempo;
        foreach( double dataToModify, rowData )
        {
            rowDataTempo.append( - std::log10( dataToModify ) );
        }
        dataToLog10.append( rowDataTempo );
    }
    return dataToLog10;
}


QList< QList< double > > Plot::LoadRawData()
{
    QList< QList< double > > rawData;
    if( !m_propertySelected.isEmpty() )
    {
        if( !m_covariateSelected.isEmpty() )
        {
            QList< QList< double > > rawData0 = m_dataBinaryRawData.value( m_propertySelected ).value( m_binaryCovariates.key( m_covariateSelected ) ).value( 0 );
            QList< QList< double > > rawData1 = m_dataBinaryRawData.value( m_propertySelected ).value( m_binaryCovariates.key( m_covariateSelected ) ).value( 1 );
            rawData.append( rawData0 );
            rawData.append( rawData1 );
        }
        else
        {
            rawData = m_dataRawData.value( m_propertySelected );
        }
    }

    return rawData;
}

QList< QList< double > > Plot::LoadRawStats()
{
    QList< QList< double > > rawStats;
    if( !m_propertySelected.isEmpty() )
    {
        if( !m_covariateSelected.isEmpty() )
        {
            QList< QList< double > > rawStats0 = m_dataBinaryRawStats.value( m_propertySelected ).value( m_binaryCovariates.key( m_covariateSelected ) ).value( 0 );
            QList< QList< double > > rawStats1 = m_dataBinaryRawStats.value( m_propertySelected ).value( m_binaryCovariates.key( m_covariateSelected ) ).value( 1 );
            rawStats.append( rawStats0 );
            rawStats.append( rawStats1 );
        }
        else
        {
            rawStats = m_dataBinaryRawStats.value( m_propertySelected ).value( -1 ).value( -1 );
        }
    }

    return rawStats;
}

QList< QList< double > > Plot::LoadBetasByProperties()
{
    QList< QList< double > > betasByProperties;
    if( !m_propertySelected.isEmpty() )
    {
        betasByProperties = m_dataBetasByProperties.value( m_propertySelected );
    }

    SetSelectionToDisplayProperties();

    return betasByProperties;
}

QList< QList< double > > Plot::LoadBetasByCovariates()
{
    QList< QList< double > > betasByCovariates;
    if( !m_covariateSelected.isEmpty() )
    {
        betasByCovariates = m_dataBetasByCovariates.value( m_allCovariates.key( m_covariateSelected ) );
    }

    SetSelectionToDisplayProperties();

    return betasByCovariates;
}

QList< QList< double > > Plot::LoadOmnibusLpvalues()
{
    m_selectionToDisplay.remove( m_selectionToDisplay.firstKey() );
    SetSelectionToDisplayProperties();

    if( m_plotSelected == "Omnibus Local pvalues" )
    {
        return ToLog10( m_dataOmnibusLpvalue );
    }
    if( m_plotSelected == "Omnibus FDR Local pvalues" )
    {
        return ToLog10( m_dataOmnibusFDRLpvalue );
    }
}

QList< QList< double > > Plot::LoadConfidenceBand()
{
    QList< QList< double > > confidenceBand;
    if( !m_propertySelected.isEmpty() && !m_covariateSelected.isEmpty() )
    {
        confidenceBand.append( m_dataConfidenceBands.value( m_propertySelected ).at( 2*m_allCovariates.key( m_covariateSelected ) + 1 ) );
        confidenceBand.append( m_dataBetasByProperties.value( m_propertySelected ).at( m_allCovariates.key( m_covariateSelected ) ) );
        confidenceBand.append( m_dataConfidenceBands.value( m_propertySelected ).at( 2*m_allCovariates.key( m_covariateSelected ) ) );
    }

    return confidenceBand;
}

QList< QList< double > > Plot::LoadPostHocFDRLpvalues()
{
    QList< QList< double > > postHocFDRLpvalues;
    if( !m_covariateSelected.isEmpty() )
    {
        postHocFDRLpvalues = m_dataPostHocFDRLpvaluesByCovariates.value( m_allCovariates.key( m_covariateSelected ) );
    }

    SetSelectionToDisplayProperties();

    return postHocFDRLpvalues;
}

QList< QList< double > > Plot::LoadSigBetasOnAverageRawData()
{
    QList< QList< double > > meanRawData;
    if( !m_propertySelected.isEmpty() )
    {
        meanRawData.append( m_processing.GetMean( m_dataRawData.value( m_propertySelected ), 0 ) );
    }

    return meanRawData;
}


bool Plot::LoadData()
{
    m_ordinate.clear();

    if( m_plotSelected == "Raw Data" )
    {
        m_ordinate = LoadRawData();
    }
    if( m_plotSelected == "Raw Stats" )
    {
        m_ordinate = LoadRawStats();
    }
    if( m_plotSelected == "Raw Betas by Properties" || m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        m_ordinate = LoadBetasByProperties();
    }
    if( m_plotSelected == "Raw Betas by Covariates" || m_plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        m_ordinate = LoadBetasByCovariates();
    }
    if( m_plotSelected == "Omnibus Local pvalues" )
    {
        m_ordinate = LoadOmnibusLpvalues();
    }
    if( m_plotSelected == "Omnibus FDR Local pvalues" )
    {
        m_ordinate = LoadOmnibusLpvalues();
    }
    if( m_plotSelected == "Omnibus Betas with Confidence Bands" )
    {
        m_ordinate = LoadConfidenceBand();
    }
    if( m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" )
    {
        m_ordinate = LoadPostHocFDRLpvalues();
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas on Average Raw Data" )
    {
        m_ordinate = LoadSigBetasOnAverageRawData();
    }

    return !( m_abscissa.isEmpty() | m_ordinate.isEmpty() );
}


void Plot::AddEntriesRawData( vtkSmartPointer< vtkTable >& table )
{
    QStringList subjects;
    if( m_covariateSelected.isEmpty() )
    {
        subjects.append( m_subjects );
    }
    else
    {
        if( !m_subjectsBinaryRawData.value( m_propertySelected ).value( m_binaryCovariates.key( m_covariateSelected ) ).value( 0 ).isEmpty() )
        {
            subjects.append( m_subjectsBinaryRawData.value( m_propertySelected ).value( m_binaryCovariates.key( m_covariateSelected ) ).value( 0 ) );
        }
        if( !m_subjectsBinaryRawData.value( m_propertySelected ).value( m_binaryCovariates.key( m_covariateSelected ) ).value( 1 ).isEmpty() )
        {
            subjects.append( m_subjectsBinaryRawData.value( m_propertySelected ).value( m_binaryCovariates.key( m_covariateSelected ) ).value( 1 ) );
        }
    }

    for( int i = 0; i < m_nbrPlots; i++ )
    {
        vtkSmartPointer< vtkDoubleArray > currentEntry = vtkSmartPointer< vtkDoubleArray >::New();
        currentEntry->SetName( subjects.at( i ).toStdString().c_str() );
        table->AddColumn( currentEntry );
    }
}

void Plot::AddEntriesRawStats( vtkSmartPointer< vtkTable >& table )
{
    if( m_covariateSelected.isEmpty() )
    {
        vtkSmartPointer< vtkDoubleArray > stdUp = vtkSmartPointer< vtkDoubleArray >::New();
        stdUp->SetName( QString( m_propertySelected + " std+" ).toStdString().c_str() );
        table->AddColumn( stdUp );
        vtkSmartPointer< vtkDoubleArray > mean = vtkSmartPointer< vtkDoubleArray >::New();
        mean->SetName( QString( m_propertySelected + " mean" ).toStdString().c_str() );
        table->AddColumn( mean );
        vtkSmartPointer< vtkDoubleArray > stdDown = vtkSmartPointer< vtkDoubleArray >::New();
        stdDown->SetName( QString( m_propertySelected + " std-" ).toStdString().c_str() );
        table->AddColumn( stdDown );
    }
    else
    {
        if( !m_dataBinaryRawStats.value( m_propertySelected ).value( m_binaryCovariates.key( m_covariateSelected ) ).value( 0 ).isEmpty() )
        {
            vtkSmartPointer< vtkDoubleArray > std0_Up = vtkSmartPointer< vtkDoubleArray >::New();
            std0_Up->SetName( QString( m_covariateSelected + " = 0 std+" ).toStdString().c_str() );
            table->AddColumn( std0_Up );
            vtkSmartPointer< vtkDoubleArray > mean0 = vtkSmartPointer< vtkDoubleArray >::New();
            mean0->SetName( QString( m_covariateSelected + " = 0 mean" ).toStdString().c_str() );
            table->AddColumn( mean0 );
            vtkSmartPointer< vtkDoubleArray > std0_Down = vtkSmartPointer< vtkDoubleArray >::New();
            std0_Down->SetName( QString( m_covariateSelected + " = 0 std-" ).toStdString().c_str() );
            table->AddColumn( std0_Down );
        }
        if( !m_dataBinaryRawStats.value( m_propertySelected ).value( m_binaryCovariates.key( m_covariateSelected ) ).value( 1 ).isEmpty() )
        {
            vtkSmartPointer< vtkDoubleArray > std1_Up = vtkSmartPointer< vtkDoubleArray >::New();
            std1_Up->SetName( QString( m_covariateSelected + " = 1 std+" ).toStdString().c_str() );
            table->AddColumn( std1_Up );
            vtkSmartPointer< vtkDoubleArray > mean1 = vtkSmartPointer< vtkDoubleArray >::New();
            mean1->SetName( QString( m_covariateSelected + " = 1 mean" ).toStdString().c_str() );
            table->AddColumn( mean1 );
            vtkSmartPointer< vtkDoubleArray > std1_Down = vtkSmartPointer< vtkDoubleArray >::New();
            std1_Down->SetName( QString( m_covariateSelected + " = 1 std-" ).toStdString().c_str() );
            table->AddColumn( std1_Down );
        }
    }
}

void Plot::AddEntriesByPropertiesOrCovariates( vtkSmartPointer< vtkTable >& table )
{
    for( int i = 0; i < m_nbrPlots; i++ )
    {
        vtkSmartPointer< vtkDoubleArray > currentEntry = vtkSmartPointer< vtkDoubleArray >::New();
        currentEntry->SetName( m_lineNames.at( i ).toStdString().c_str() );
        table->AddColumn( currentEntry );
    }
}

void Plot::AddEntriesConfidenceBands( vtkSmartPointer< vtkTable >& table )
{
    vtkSmartPointer< vtkDoubleArray > upperConfidenceBand = vtkSmartPointer< vtkDoubleArray >::New();
    upperConfidenceBand->SetName( QString( m_covariateSelected + " Upper Confidence Band" ).toStdString().c_str() );
    table->AddColumn( upperConfidenceBand );
    vtkSmartPointer< vtkDoubleArray > meanBetas = vtkSmartPointer< vtkDoubleArray >::New();
    meanBetas->SetName( QString( m_covariateSelected + " Mean Betas" ).toStdString().c_str() );
    table->AddColumn( meanBetas );
    vtkSmartPointer< vtkDoubleArray > lowerConfidenceBand = vtkSmartPointer< vtkDoubleArray >::New();
    lowerConfidenceBand->SetName( QString( m_covariateSelected + " Lower Confidence Band" ).toStdString().c_str() );
    table->AddColumn( lowerConfidenceBand );
}

void Plot::AddEntriesSigBetasOnAverageRawData( vtkSmartPointer< vtkTable >& table )
{
    vtkSmartPointer< vtkDoubleArray > currentEntry = vtkSmartPointer< vtkDoubleArray >::New();
    currentEntry->SetName( QString( "Average " + m_propertySelected ).toStdString().c_str() );
    table->AddColumn( currentEntry );
}

void Plot::AddEntries( vtkSmartPointer< vtkTable >& table )
{
    vtkSmartPointer< vtkDoubleArray > abscissa = vtkSmartPointer< vtkDoubleArray >::New();
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
    if( m_plotSelected == "Raw Betas by Properties" ||
            m_plotSelected == "Raw Betas by Covariates" ||
            m_plotSelected == "Omnibus Local pvalues" ||
            m_plotSelected == "Omnibus FDR Local pvalues" ||
            m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
            m_plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        AddEntriesByPropertiesOrCovariates( table );
    }

    if( m_plotSelected == "Post-Hoc FDR Significant Betas on Average Raw Data" )
    {
        AddEntriesSigBetasOnAverageRawData( table );
    }
    if( m_plotSelected == "Omnibus Betas with Confidence Bands" )
    {
        AddEntriesConfidenceBands( table );
    }
}


void Plot::SetData( vtkSmartPointer< vtkTable >& table )
{
    table->SetNumberOfRows( m_nbrPoints );
    for( int i = 0; i < m_nbrPoints; i++ )
    {
        // Adding abscissa
        table->SetValue( i, 0, m_abscissa.at( i ) );

        // Adding data
        for( int j = 0; j < m_nbrPlots; j++ )
        {
            table->SetValue( i, j + 1, m_ordinate.at( j ).at( i ) );
        }
    }
}


void Plot::InitLines()
{
    for( int i = 0; i < m_nbrPlots; i++ )
    {
        m_chart->AddPlot( vtkChart::LINE );
        m_chart->GetPlot( i )->SetSelectable( m_plotSelected == "Raw Data" ? true : false );
    }
}

void Plot::InitQCThresholdLines()
{
    for( int i = 0; i < m_nbrPlots; i++ )
    {
        m_chart->AddPlot( vtkChart::LINE );
        m_chart->GetPlot( i )->SetSelectable( false );
    }
}

void Plot::AddSignificantLevel( double significantLevel )
{
    vtkSmartPointer< vtkDoubleArray > abscissaSigLevel = vtkSmartPointer< vtkDoubleArray >::New();
    abscissaSigLevel->SetNumberOfComponents( 1 );
    abscissaSigLevel->SetName( "Abscissa SigLevel" );
    vtkSmartPointer< vtkDoubleArray > dataSigLevel = vtkSmartPointer< vtkDoubleArray >::New();
    dataSigLevel->SetNumberOfComponents( 1 );
    dataSigLevel->SetName( "Significant Level" );
    for( int i = 0; i < m_nbrPoints; i++ )
    {
        abscissaSigLevel->InsertNextValue( m_abscissa.at( i ) );
        dataSigLevel->InsertNextValue( significantLevel );
    }

    vtkSmartPointer< vtkTable > tableSigLevel = vtkSmartPointer< vtkTable >::New();
    tableSigLevel->SetNumberOfRows( m_nbrPoints );
    tableSigLevel->AddColumn( abscissaSigLevel );
    tableSigLevel->AddColumn( dataSigLevel );

    vtkSmartPointer< vtkPlot > sigLevelLine = m_chart->AddPlot( vtkChart::LINE );
    sigLevelLine->SetSelectable( false );
    sigLevelLine->SetInputData( tableSigLevel, 0, 1 );
    sigLevelLine->SetColor( m_allColors.value( "Black" ).first(), m_allColors.value( "Black" ).at( 1 ), m_allColors.value( "Black" ).at( 2 ), 255 );
    sigLevelLine->SetWidth( m_lineWidth / 2 );
}

void Plot::AddMean( QList< double > meanRawData )
{
    vtkSmartPointer< vtkDoubleArray > abscissaSigLevel = vtkSmartPointer< vtkDoubleArray >::New();
    abscissaSigLevel->SetNumberOfComponents( 1 );
    abscissaSigLevel->SetName( "Abscissa Mean" );
    vtkSmartPointer< vtkDoubleArray > dataSigLevel = vtkSmartPointer< vtkDoubleArray >::New();
    dataSigLevel->SetNumberOfComponents( 1 );
    dataSigLevel->SetName( "Significant Mean" );
    for( int i = 0; i < m_nbrPoints; i++ )
    {
        abscissaSigLevel->InsertNextValue( m_abscissa.at( i ) );
        dataSigLevel->InsertNextValue( meanRawData.at( i ) );
    }

    vtkSmartPointer< vtkTable > tableSigLevel = vtkSmartPointer< vtkTable >::New();
    tableSigLevel->SetNumberOfRows( m_nbrPoints );
    tableSigLevel->AddColumn( abscissaSigLevel );
    tableSigLevel->AddColumn( dataSigLevel );

    vtkSmartPointer< vtkPlot > sigLevelLine = m_chart->AddPlot( vtkChart::LINE );
    sigLevelLine->SetSelectable( false );
    sigLevelLine->SetInputData( tableSigLevel, 0, 1 );
    sigLevelLine->SetColor( m_allColors.value( "Black" ).first(), m_allColors.value( "Black" ).at( 1 ), m_allColors.value( "Black" ).at( 2 ), 255 );
    sigLevelLine->SetWidth( m_lineWidth * 2 );
}

void Plot::AddCrop( bool previousExist )
{
    if( previousExist )
    {
        m_chart->RemovePlot( m_chart->GetNumberOfPlots() - 1 );
        m_chart->RemovePlot( m_chart->GetNumberOfPlots() - 1 );
    }

    GetyMinMax();

    vtkSmartPointer< vtkDoubleArray > startCrop = vtkSmartPointer< vtkDoubleArray >::New();
    startCrop->SetNumberOfComponents( 1 );
    startCrop->SetName( "Abscissa Start Crop" );
    vtkSmartPointer< vtkDoubleArray > endCrop = vtkSmartPointer< vtkDoubleArray >::New();
    endCrop->SetNumberOfComponents( 1 );
    endCrop->SetName( "Abscissa End Crop" );
    vtkSmartPointer< vtkDoubleArray > yCropStart = vtkSmartPointer< vtkDoubleArray >::New();
    yCropStart->SetNumberOfComponents( 1 );
    yCropStart->SetName( "Y Crop Start" );
    vtkSmartPointer< vtkDoubleArray > yCropEnd = vtkSmartPointer< vtkDoubleArray >::New();
    yCropEnd->SetNumberOfComponents( 1 );
    yCropEnd->SetName( "Y Crop End" );

    startCrop->InsertNextValue( m_abscissa.at( m_arcLengthStartIndex ) );
    startCrop->InsertNextValue( m_abscissa.at( m_arcLengthStartIndex ) );
    endCrop->InsertNextValue( m_abscissa.at( m_arcLengthEndIndex ) );
    endCrop->InsertNextValue( m_abscissa.at( m_arcLengthEndIndex ) );
    yCropStart->InsertNextValue( m_yMinMax[ 0 ] );
    yCropStart->InsertNextValue( m_yMinMax[ 1 ] );
    yCropEnd->InsertNextValue( m_yMinMax[ 0 ] );
    yCropEnd->InsertNextValue( m_yMinMax[ 1 ] );

    vtkSmartPointer< vtkTable > tableCrop = vtkSmartPointer< vtkTable >::New();
    tableCrop->SetNumberOfRows( 2 );
    tableCrop->AddColumn( startCrop );
    tableCrop->AddColumn( endCrop );
    tableCrop->AddColumn( yCropStart );
    tableCrop->AddColumn( yCropEnd );

    vtkSmartPointer< vtkPlot > startCropLine = m_chart->AddPlot( vtkChart::LINE );
    startCropLine->SetSelectable( false );
    startCropLine->SetInputData( tableCrop, 0, 2 );
    startCropLine->SetColor( m_allColors.value( "Black" ).first(), m_allColors.value( "Black" ).at( 1 ), m_allColors.value( "Black" ).at( 2 ), 255 );
    startCropLine->GetPen()->SetLineType( vtkPen::DASH_DOT_LINE );
    startCropLine->SetWidth( m_lineWidth );
    startCropLine->SetVisible( m_croppingEnabled );

    vtkSmartPointer< vtkPlot > endCropLine = m_chart->AddPlot( vtkChart::LINE );
    endCropLine->SetSelectable( false );
    endCropLine->SetInputData( tableCrop, 1, 3 );
    endCropLine->SetColor( m_allColors.value( "Black" ).first(), m_allColors.value( "Black" ).at( 1 ), m_allColors.value( "Black" ).at( 2 ), 255 );
    endCropLine->GetPen()->SetLineType( vtkPen::DASH_DOT_LINE );
    endCropLine->SetWidth( m_lineWidth );
    endCropLine->SetVisible( m_croppingEnabled );
}

void Plot::AddSigBetas( const QList< double >& dataSigBetas, const QList< double >& abscissaSigBetas, int index )
{
    vtkSmartPointer< vtkDoubleArray > abscissaSigBetaTempo = vtkSmartPointer< vtkDoubleArray >::New();
    vtkSmartPointer< vtkDoubleArray > dataSigBetaTempo = vtkSmartPointer< vtkDoubleArray >::New();
    int nbrPoints = dataSigBetas.size();

    if( dataSigBetas.size() == abscissaSigBetas.size() )
    {
        abscissaSigBetaTempo->SetNumberOfComponents( 1 );
        abscissaSigBetaTempo->SetName( "Abscissa SigBeta" );
        dataSigBetaTempo->SetNumberOfComponents( 1 );
        dataSigBetaTempo->SetName( QString( m_selectionToDisplay.value( index ).first + " SigBeta" ).toStdString().c_str() );

        for( int j = 0; j < nbrPoints; j++ )
        {
            abscissaSigBetaTempo->InsertNextValue( abscissaSigBetas.at( j ) );
            dataSigBetaTempo->InsertNextValue( dataSigBetas.at( j ) );
        }
    }

    if( ( dataSigBetaTempo->GetSize() != 0 ) && ( abscissaSigBetaTempo->GetSize() != 0 ) )
    {
        vtkSmartPointer< vtkTable > tableSigBetas = vtkSmartPointer< vtkTable >::New();
        tableSigBetas->SetNumberOfRows( nbrPoints );
        tableSigBetas->AddColumn( abscissaSigBetaTempo );
        tableSigBetas->AddColumn( dataSigBetaTempo );

        vtkSmartPointer< vtkPlot > sigBetas = m_chart->AddPlot( vtkChart::POINTS );
        sigBetas->SetSelectable( false );
        sigBetas->SetInputData( tableSigBetas, 0, 1 );
        vtkPlotPoints::SafeDownCast( sigBetas )->SetMarkerStyle( m_markerType );
        vtkPlotPoints::SafeDownCast( sigBetas )->SetMarkerSize( m_markerSize );
        sigBetas->SetColor( m_lineColors.at( index ).first(), m_lineColors.at( index ).at( 1 ), m_lineColors.at( index ).at( 2 ), 255 );
        sigBetas->SetVisible( m_selectionToDisplay.value( index ).second.first );
    }
}

void Plot::AddSigBetasOnAverageRawData( const QList< double >& dataSigBetas, const QList< double >& abscissaSigBetas, const QList< int >& markerColor, QString IDLegend )
{
    vtkSmartPointer< vtkDoubleArray > abscissaSigBetaTempo = vtkSmartPointer< vtkDoubleArray >::New();
    vtkSmartPointer< vtkDoubleArray > dataSigBetaTempo = vtkSmartPointer< vtkDoubleArray >::New();
    int nbrPoints = dataSigBetas.size();

    if( dataSigBetas.size() == abscissaSigBetas.size() )
    {
        abscissaSigBetaTempo->SetNumberOfComponents( 1 );
        abscissaSigBetaTempo->SetName( "Abscissa SigBeta" );
        dataSigBetaTempo->SetNumberOfComponents( 1 );
        dataSigBetaTempo->SetName( IDLegend.toStdString().c_str() );

        for( int j = 0; j < nbrPoints; j++ )
        {
            abscissaSigBetaTempo->InsertNextValue( abscissaSigBetas.at( j ) );
            dataSigBetaTempo->InsertNextValue( dataSigBetas.at( j ) );
        }
    }

    if( ( dataSigBetaTempo->GetSize() != 0 ) && ( abscissaSigBetaTempo->GetSize() != 0 ) )
    {
        vtkSmartPointer< vtkTable > tableSigBetas = vtkSmartPointer< vtkTable >::New();
        tableSigBetas->SetNumberOfRows( nbrPoints );
        tableSigBetas->AddColumn( abscissaSigBetaTempo );
        tableSigBetas->AddColumn( dataSigBetaTempo );

        vtkSmartPointer< vtkPlot > sigBetas = m_chart->AddPlot( vtkChart::POINTS );
        sigBetas->SetSelectable( false );
        sigBetas->SetInputData( tableSigBetas, 0, 1 );
        vtkPlotPoints::SafeDownCast( sigBetas )->SetMarkerStyle( m_markerType );
        vtkPlotPoints::SafeDownCast( sigBetas )->SetMarkerSize( m_markerSize );
        sigBetas->SetColor( markerColor.first(), markerColor.at( 1 ), markerColor.at( 2 ), 255 );
    }
}

void Plot::AddLineRawData( const vtkSmartPointer< vtkTable >& table )
{
    QList< int > color = m_allColors.value( "Black" );
    double currentLineWidth = m_lineWidth / 2;
    int shift = -1;
    bool foundBinaryRawData0 = !m_dataBinaryRawData.value( m_propertySelected ).value( m_binaryCovariates.key( m_covariateSelected ) ).value( 0 ).isEmpty();
    bool foundBinaryRawData1 = !m_dataBinaryRawData.value( m_propertySelected ).value( m_binaryCovariates.key( m_covariateSelected ) ).value( 1 ).isEmpty();

    if( !m_covariateSelected.isEmpty() && !foundBinaryRawData0 && foundBinaryRawData1 )
    {
        color = m_allColors.value( m_covariatesProperties.value( m_allCovariates.key( m_covariateSelected ) ).second.second );
        currentLineWidth = m_lineWidth;
    }
    if( !m_covariateSelected.isEmpty() && foundBinaryRawData0 && foundBinaryRawData1 )
    {
        shift = m_dataBinaryRawData.value( m_propertySelected ).value( m_binaryCovariates.key( m_covariateSelected ) ).value( 0 ).size();
    }

    for( int i = 0; i < m_nbrPlots; i++ )
    {
        vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( i );
        currentLine->SetInputData( table, 0, i + 1 );

        if( shift != -1 && i >= shift )
        {
            color = m_allColors.value( m_covariatesProperties.value( m_allCovariates.key( m_covariateSelected ) ).second.second );
            currentLineWidth = m_lineWidth;
        }

        currentLine->SetColor( color.first(), color.at( 1 ), color.at( 2 ), 255 );
        currentLine->SetWidth( currentLineWidth );
    }
}

void Plot::AddLineRawStats( const vtkSmartPointer< vtkTable >& table )
{
    QList< int > color = m_allColors.value( "Black" );
    int shift = -1;
    bool foundBinaryRawStats0 = !m_dataBinaryRawStats.value( m_propertySelected ).value( m_binaryCovariates.key( m_covariateSelected ) ).value( 0 ).isEmpty();
    bool foundBinaryRawStats1 = !m_dataBinaryRawStats.value( m_propertySelected ).value( m_binaryCovariates.key( m_covariateSelected ) ).value( 1 ).isEmpty();

    if( m_covariateSelected.isEmpty() )
    {
        color = m_allColors.value( "Black" );
    }
    if( !m_covariateSelected.isEmpty() && !foundBinaryRawStats0 && foundBinaryRawStats1 )
    {
        color = m_allColors.value( m_covariatesProperties.value( m_allCovariates.key( m_covariateSelected ) ).second.second );
    }
    if( !m_covariateSelected.isEmpty() && foundBinaryRawStats0 && foundBinaryRawStats1 )
    {
        shift = 3;
    }

    for( int i = 0; i < m_nbrPlots; i++ )
    {
        vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( i );
        currentLine->SetInputData( table, 0, i + 1 );

        if( shift != -1 && i >= shift )
        {
            color = m_allColors.value( m_covariatesProperties.value( m_allCovariates.key( m_covariateSelected ) ).second.second );
        }

        currentLine->SetColor( color.first(), color.at( 1 ), color.at( 2 ), 255 );
        currentLine->SetWidth( m_lineWidth );

        if( ( i == 0 || i == 3 ) )
        {
            currentLine->GetPen()->SetLineType( vtkPen::DASH_LINE );
        }
        if( ( i == 2 || i == 5 ) )
        {
            currentLine->GetPen()->SetLineType( vtkPen::DASH_DOT_DOT_LINE );
        }
    }
}

void Plot::AddLineLPvalue( const vtkSmartPointer< vtkTable >& table, int shift )
{
    for( int i = 0; i < m_nbrPlots; i++ )
    {
        vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( i );
        currentLine->SetInputData( table, 0, i + 1 );
        currentLine->SetColor( m_lineColors.at( i ).first(), m_lineColors.at( i ).at( 1 ), m_lineColors.at( i ).at( 2 ), 255 );
        currentLine->SetWidth( m_lineWidth );
        currentLine->SetVisible( m_selectionToDisplay.value( i + shift ).second.first );
    }

    AddSignificantLevel( -log10( m_pvalueThreshold ) );
}

void Plot::AddLineBetas( const vtkSmartPointer< vtkTable >& table )
{
    for( int i = 0; i < m_nbrPlots; i++ )
    {
        vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( i );
        currentLine->SetInputData( table, 0, i + 1 );
        currentLine->SetColor( m_lineColors.at( i ).first(), m_lineColors.at( i ).at( 1 ), m_lineColors.at( i ).at( 2 ), 255 );
        currentLine->SetWidth( m_lineWidth );
        currentLine->SetVisible( m_selectionToDisplay.value( i ).second.first );

//        QList< double > dataSigBetas = data.value( IDSelected ).at( index - shift );
//        QList< double > abscissaSigBetas = abscissa.value( IDSelected ).at( index - shift );
        if( i > 0 && m_plotSelected == "Omnibus FDR Significant Betas by Properties" )
        {
            AddSigBetas( m_dataOmnibusFDRSigBetasByProperties.value( m_propertySelected ).at( i - 1 ), m_abscissaOmnibusFDRSigBetasByProperties.value( m_propertySelected ).at( i - 1 ), i );
        }
        if( m_plotSelected == "Omnibus FDR Significant Betas by Covariates" )
        {
            AddSigBetas( m_dataOmnibusFDRSigBetasByCovariates.value( m_allCovariates.key( m_covariateSelected ) ).at( i ), m_abscissaOmnibusFDRSigBetasByCovariates.value( m_allCovariates.key( m_covariateSelected ) ).at( i ), i );
        }
        if( i > 0 && m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
        {
            AddSigBetas( m_dataPostHocFDRSigBetasByProperties.value( m_propertySelected ).at( i - 1 ), m_abscissaPostHocFDRSigBetasByProperties.value( m_propertySelected ).at( i - 1 ), i );
        }
        if( m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
        {
            AddSigBetas( m_dataPostHocFDRSigBetasByCovariates.value( m_allCovariates.key( m_covariateSelected ) ).at( i ), m_abscissaPostHocFDRSigBetasByCovariates.value( m_allCovariates.key( m_covariateSelected ) ).at( i ), i );
        }
    }

    AddSignificantLevel( 0 );
}

void Plot::AddLineLConfidenceBands( const vtkSmartPointer< vtkTable >& table )
{
    for( int i = 0; i < m_nbrPlots; i++ )
    {
        vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( i );
        QList< int > color = m_allColors.value( "Black" );
        int lineType = vtkPen::SOLID_LINE;

        if( i == 0 )
        {
            lineType = vtkPen::DASH_LINE;
        }
        if( i == 1)
        {
            color = m_allColors.value( m_covariatesProperties.value( m_allCovariates.key( m_covariateSelected ) ).second.second );
        }
        if( i == 2 )
        {
            lineType = vtkPen::DASH_DOT_DOT_LINE;
        }

        currentLine->SetInputData( table, 0, i + 1 );
        currentLine->SetColor( color.first(), color.at( 1 ), color.at( 2 ), 255 );
        currentLine->SetWidth( m_lineWidth );
        currentLine->GetPen()->SetLineType( lineType );
    }

    AddSignificantLevel( 0 );
}

void Plot::AddLineSigBetasOnAverageRawData( const vtkSmartPointer< vtkTable >& table )
{
    QList< int > lineColor = m_allColors.value( "Black" );
    vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( 0 );
    currentLine->SetInputData( table, 0, 1 );
    currentLine->SetColor( lineColor.first(), lineColor.at( 1 ), lineColor.at( 2 ), 255 );
    currentLine->SetWidth( m_lineWidth );

    if( !m_covariateSelected.isEmpty() )
    {
        QList< double > negativeSigBetas = m_dataPostHocFDRSigBetasOnAverageRawData.value( m_propertySelected ).value( "Negative" ).at( m_covariatesNoIntercept.key( m_covariateSelected ) - 1 );
        QList< double > positiveSigBetas = m_dataPostHocFDRSigBetasOnAverageRawData.value( m_propertySelected ).value( "Positive" ).at( m_covariatesNoIntercept.key( m_covariateSelected ) - 1 );
        QList< double > sigBetas = QList< double >() << negativeSigBetas << positiveSigBetas;
        QList< double > abscissaNegativeSigBetas = m_abscissaPostHocFDRSigBetasOnAverageRawData.value( m_propertySelected ).value( "Negative" ).at( m_covariatesNoIntercept.key( m_covariateSelected ) - 1 );
        QList< double > abscissaPositiveSigBetas = m_abscissaPostHocFDRSigBetasOnAverageRawData.value( m_propertySelected ).value( "Positive" ).at( m_covariatesNoIntercept.key( m_covariateSelected ) - 1 );
        QList< double > abscissaSigBetas = QList< double >() << abscissaNegativeSigBetas << abscissaPositiveSigBetas;

        if( m_isPosNeg )
        {
            AddSigBetasOnAverageRawData( positiveSigBetas, abscissaPositiveSigBetas, m_allColors.value( "Lime" ), QString( m_covariateSelected + " Positive SigBetas" ) );
            AddSigBetasOnAverageRawData( negativeSigBetas, abscissaNegativeSigBetas, m_allColors.value( "Red" ), QString( m_covariateSelected + " Negative SigBetas" ) );
        }
        else
        {
            QList< int > markerColorSigBetas = m_allColors.value( m_covariatesProperties.value( m_allCovariates.key( m_covariateSelected ) ).second.second );
            AddSigBetasOnAverageRawData( sigBetas, abscissaSigBetas, markerColorSigBetas, QString( m_covariateSelected + " SigBetas" ) );
        }
    }
}

void Plot::AddLines( const vtkSmartPointer< vtkTable >& table )
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
    if( m_plotSelected == "Omnibus Local pvalues" ||
            m_plotSelected == "Omnibus FDR Local pvalues" ||
            m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" )
    {
        int shift = m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" ? 0 : 1;
        AddLineLPvalue( table, shift );
    }
    if( m_plotSelected == "Raw Betas by Properties" ||
            m_plotSelected == "Raw Betas by Covariates" ||
            m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
            m_plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        AddLineBetas( table );
    }
    if( m_plotSelected == "Omnibus Betas with Confidence Bands" )
    {
        AddLineLConfidenceBands( table );
    }

    if( m_plotSelected == "Post-Hoc FDR Significant Betas on Average Raw Data" )
    {
        AddLineSigBetasOnAverageRawData( table );
    }
}

void Plot::AddQCThresholdLines( const vtkSmartPointer< vtkTable >& table )
{
    InitQCThresholdLines();

    QList< double > atlas = m_processing.QStringListToDouble( m_atlasQCThreshold );

    QList< double > refLine = !atlas.isEmpty() ? atlas : m_processing.GetMean( m_dataRawData.value( m_propertySelected ), 0 );

    QStringList subjectsCorrelated, subjectsNotCorrelated;
    for( int i = 0; i < m_nbrPlots; i++ )
    {
        vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( i );
        currentLine->SetInputData( table, 0, i + 1 );
        QList< int > color;

        double pearsonCorrelation = m_processing.ApplyPearsonCorrelation( m_dataRawData.value( m_propertySelected ).at( i ), refLine, 0 );

        if( pearsonCorrelation < m_qcThreshold )
        {
            subjectsNotCorrelated.append( QString( currentLine->GetLabel() ) );

            color = m_allColors.value( "Red" );
            currentLine->GetPen()->SetLineType( vtkPen::DASH_LINE );
        }
        else
        {
            subjectsCorrelated.append( QString( currentLine->GetLabel() ) );

            color = m_allColors.value( "Carolina Blue" );
            currentLine->GetPen()->SetLineType( vtkPen::SOLID_LINE );
        }

        currentLine->SetColor(  color.first(), color.at( 1 ), color.at( 2 ), 255 );
        currentLine->SetWidth( m_lineWidth );
    }

    AddMean( refLine );
    AddCrop( false );

    emit UpdateSubjectsCorrelated( subjectsCorrelated, subjectsNotCorrelated );
}


void Plot::GetyMinMax()
{
    double yMin = m_yMin.first ? m_yMin.second : 1000;
    double yMax = m_yMax.first ? m_yMax.second : -1000;

    for( int i = 0; i < m_ordinate.size(); i++ )
    {
        QList< double > rowData = m_ordinate.at( i );

        if( ( m_plotSelected == "Raw Data" ||
              m_plotSelected == "Raw Stats" ||
              m_plotSelected == "Post-Hoc FDR Significant Betas on Average Raw Data" )
                || ( !( m_plotSelected == "Raw Data" ||
                        m_plotSelected == "Raw Stats" ||
                        m_plotSelected == "Post-Hoc FDR Significant Betas on Average Raw Data" )
                     && m_selectionToDisplay.value( i ).second.first ) )
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
    }

    m_yMinMax[ 0 ] = yMin;
    m_yMinMax[ 1 ] = yMax;
}

void Plot::SetAbscissaProperties( double xMin, double xMax )
{
    double deltaX = ( xMax - xMin ) / m_nbrPoints;
    xMin -= deltaX;
    xMax += deltaX;
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetMinimumLimit( xMin - deltaX );
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetMaximumLimit( xMax + deltaX );

    //    m_chart->GetAxis( vtkAxis::BOTTOM )->SetMinimumLimit( SetMinMax( xMin ) );
    //    m_chart->GetAxis( vtkAxis::BOTTOM )->SetMaximumLimit( SetMinMax( xMax ) );

    m_chart->GetAxis( vtkAxis::BOTTOM )->SetRange( xMin, xMax );

    m_chart->GetAxis( vtkAxis::BOTTOM )->GetGridPen()->SetWidth( 0.25 );
    m_chart->GetAxis( vtkAxis::BOTTOM )->GetGridPen()->SetColor( 128, 128, 128, 255 );
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetGridVisible( m_gridOn );
}

void Plot::SetOrdinateProperties( double yMin, double yMax )
{
    double deltaY = ( yMax - yMin ) / m_nbrPoints;
    yMin -= deltaY;
    yMax += deltaY;
    m_chart->GetAxis( vtkAxis::LEFT )->SetMinimumLimit( yMin - deltaY );
    m_chart->GetAxis( vtkAxis::LEFT )->SetMaximumLimit( yMax + deltaY );

    //    m_chart->GetAxis( vtkAxis::LEFT )->SetMinimumLimit( SetMinMax( yMin ) );
    //    m_chart->GetAxis( vtkAxis::LEFT )->SetMaximumLimit( SetMinMax( yMax ) );

    m_chart->GetAxis( vtkAxis::LEFT )->SetRange( yMin, yMax );
    m_chart->GetAxis( vtkAxis::LEFT )->SetNotation( m_abscissaNotation );

    m_chart->GetAxis( vtkAxis::LEFT )->GetGridPen()->SetWidth( 0.25 );
    m_chart->GetAxis( vtkAxis::LEFT )->GetGridPen()->SetColor( 128, 128, 128, 255 );
    m_chart->GetAxis( vtkAxis::LEFT )->SetGridVisible( m_gridOn );

}

void Plot::SetAxisProperties()
{
    GetyMinMax();

    SetAbscissaProperties( m_abscissa.first(), m_abscissa.last() );
    SetOrdinateProperties( m_yMinMax[ 0 ], m_yMinMax[ 1 ] );
}

void Plot::SetQCThresholdAxisProperties()
{
    double xMin = m_abscissa.first();
    double xMax = m_abscissa.last();
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetMinimumLimit( xMin );
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetMaximumLimit( xMax );
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetRange( xMin, xMax );
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetTitle( "Arc Length" );


    m_chart->SetSelectionMode( vtkContextScene::SELECTION_NONE );
    m_chart->SetForceAxesToBounds( true );
}

double Plot::SetMinMax( double minMax )
{
    bool isNegatif = minMax < 0;
    int count = 0;
    double fractpart;
    double intpart;

    fractpart = modf ( fabs( minMax ) , &intpart );

    while( double( intpart / 10 ) <= 1.0 )
    {
        count++;
        fractpart = modf( fabs( minMax )*pow( 10, count ) , &intpart );
    }

    double res = isNegatif? -round( fabs( minMax )*pow( 10, count ) + 1 ) / pow( 10, count ) : round( fabs( minMax )*pow( 10, count ) + 1 ) / pow( 10, count );
    return res;
}

int Plot::LineAlreadySelected( vtkSmartPointer< vtkPlot > line )
{
    int index = -1;

    QMap< int, struct_selectedLine >::ConstIterator iterPreviousSelectedLine = m_previousSelectedLines.selectedLines.cbegin();
    while( iterPreviousSelectedLine != m_previousSelectedLines.selectedLines.cend() )
    {
        if( iterPreviousSelectedLine.value().line == line )
        {
            return iterPreviousSelectedLine.key();
        }
        ++iterPreviousSelectedLine;
    }

    return index;
}

double* Plot::GetSelectedLineColor()
{
    return m_selectedLineColor;
}

double Plot::GetLineWidth() const
{
    return m_lineWidth;
}

void Plot::SetObservers()
{
    if( m_plotSelected == "Raw Data" )
    {
        vtkSmartPointer< vtkCallbackCommand > callback_LeftButton = vtkSmartPointer< vtkCallbackCommand >::New();
        callback_LeftButton->SetCallback( CallbackAddSelectedLine );
        callback_LeftButton->SetClientData( this );
        for( int i = 0; i < m_chart->GetNumberOfPlots(); i++ )
        {
            m_chart->GetPlot( i )->AddObserver( vtkContextMouseEvent::LEFT_BUTTON, callback_LeftButton );
        }

        vtkSmartPointer< vtkCallbackCommand > callback_RightButton = vtkSmartPointer< vtkCallbackCommand >::New();
        callback_RightButton->SetCallback( CallbackUpdateSelectedLines );
        callback_RightButton->SetClientData( this );
        m_chart->AddObserver( vtkCommand::SelectionChangedEvent, callback_RightButton );
    }

    m_chart->SetSelectionMethod( vtkChartXY::SELECTION_COLUMNS );
    m_chart->SetActionToButton( vtkChart::SELECT , vtkContextMouseEvent::LEFT_BUTTON );
    m_chart->SetActionToButton( vtkChart::PAN , vtkContextMouseEvent::MIDDLE_BUTTON );
    m_chart->SetActionToButton( vtkChart::ZOOM, vtkContextMouseEvent::RIGHT_BUTTON );
}


void Plot::SavePlot( QString filePath )
{
    vtkSmartPointer< vtkGL2PSExporter > writer = vtkSmartPointer< vtkGL2PSExporter >::New();
    writer->SetRenderWindow( m_view->GetRenderWindow() );
    writer->SetFilePrefix( filePath.split( "." ).first().toStdString().c_str() );
    writer->SetFileFormat( vtkGL2PSExporter::EPS_FILE );
    writer->SetSortToOff();
    writer->SetLineWidthFactor( 1.0 );
    writer->CompressOff();
    writer->Write();
}


void Plot::ZoomOut()
{
    SetAxisProperties();
}


/***************************************************************/
/************************ Private slots ************************/
/***************************************************************/
void Plot::OnMouseEvent( QMouseEvent* event )
{
    if( event->type() == QMouseEvent::MouseButtonDblClick && event->button() == Qt::LeftButton )
    {
        UpdateLineSelection();
    }

    if( event->type() == QMouseEvent::MouseButtonDblClick && event->button() == Qt::RightButton )
    {
        ZoomOut();
    }
}
