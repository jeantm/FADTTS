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


void Plot::SetQVTKWidget( QSharedPointer< QVTKWidget > qvtkWidget )
{
    m_qvtkWidget = QSharedPointer< QVTKWidget >( qvtkWidget );
    m_view = vtkSmartPointer< vtkContextView >::New();
    m_chart = vtkSmartPointer< vtkChartXY >::New();

    m_view->SetInteractor( m_qvtkWidget->GetInteractor() );
    m_qvtkWidget->SetRenderWindow( m_view->GetRenderWindow() );

    connect( m_qvtkWidget.data(), SIGNAL( mouseEvent( QMouseEvent * ) ), this, SLOT( OnMouseEvent( QMouseEvent * ) ) );
}

bool Plot::InitPlot( QString directory, QString fibername )
{
    m_directory = directory;
    m_matlabDirectory = directory + "/MatlabOutputs";
    m_fibername = fibername;

    m_isBinaryCovariatesSent = false;
    m_isAllCovariatesSent = false;
    m_isCovariatesNoInterceptSent = false;

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
        SetSubjects();
        SetCovariates();
        SetAbscissa();
    }

    return isPlottingEnabled;
}

bool Plot::InitQCThresholdPlot( QList< QStringList > rawData, QStringList matchedSubjects )
{
    ClearPlot();
    ResetPlotData();

    m_matchedSubjects = matchedSubjects;
    m_plotSelected = "Raw Data";

    QStringList tempoSubjects = rawData.first();
    tempoSubjects.removeFirst();
    m_subjects = tempoSubjects;

    bool isPlottingEnabled = SetRawDataQCThreshold( rawData );
    if( isPlottingEnabled )
    {
        SetProperties();
        SetAbscissa();
    }

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
    m_propertySelected = "FA";

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
        QList< double > atlas = QStringListToDouble( m_atlasQCThreshold );
        AddQCThresholdLines( table, atlas );

        // Setting axis properties and observers
        SetQCThresholdAxisProperties();

        m_chart->SetInteractive( false );
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
    m_dataSubMatrix.clear();
    m_dataBeta.clear();
    m_dataOmnibusLpvalue.clear();
    m_dataOmnibusFDRLpvalue.clear();
    m_dataConfidenceBands.clear();
    m_dataPostHocFDRLpvalue.clear();

    m_plotsUsed.clear();
    m_properties.clear();
    m_subjects.clear();
    m_covariatesNoIntercept.clear();
    m_allCovariates.clear();
    m_binaryCovariates.clear();
    m_abscissa.clear();
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
    if( m_plotSelected == "Raw Data" || m_plotSelected == "Raw Stats" )
    {
        if( !m_isBinaryCovariatesSent )
        {
            emit CovariatesAvailableForPlotting( m_binaryCovariates );
            m_isBinaryCovariatesSent = true;
            m_isAllCovariatesSent = false;
            m_isCovariatesNoInterceptSent = false;
        }
    }
    if( m_plotSelected.contains( "by Properties" ) || m_plotSelected.contains( "Confidence Bands" ) || m_plotSelected.contains( "Average" ) )
    {
        if( !m_isAllCovariatesSent )
        {
            emit CovariatesAvailableForPlotting( m_allCovariates );
            m_isBinaryCovariatesSent = false;
            m_isAllCovariatesSent = true;
            m_isCovariatesNoInterceptSent = false;
        }
    }
    if( m_plotSelected.contains( "by Covariates" ) || m_plotSelected.contains( "Average" ) )
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

    if( m_plotSelected == "Omnibus Local pvalues" || m_plotSelected == "Omnibus FDR Local pvalues" || m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" )
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

void Plot::UpdatePvalueThreshold()
{
    if( m_plotSelected.contains( "Significant Betas" ) )
    {
        const vtkSmartPointer< vtkTable >& table = m_chart->GetPlot( 0 )->GetInput();
        bool betaDisplayedByProperties = m_plotSelected.contains( "by Properties" );
        bool isOmnibus = m_plotSelected.contains( "Omnibus" );

        for( int indexPlot = m_chart->GetNumberOfPlots(); indexPlot > m_nbrPlots ; indexPlot-- )
        {
            m_chart->RemovePlot( indexPlot - 1 );
        }

        for( int indexPlot = 0; indexPlot < m_nbrPlots; indexPlot++ )
        {
            if( ( betaDisplayedByProperties && ( m_lineNames.at( indexPlot ) != m_allCovariates.first() ) ) || !betaDisplayedByProperties )
            {
                AddLineSigBetas( table, betaDisplayedByProperties, isOmnibus, indexPlot );
            }
        }

        AddSignificantLevel( 0 );
    }

    if( m_plotSelected.contains( "Local pvalues" ) )
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
            if( m_covariateSelected.isEmpty() || m_dataSubMatrix.at( i + 1 ).at( indexCovariate ).toDouble() == 0 )
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
            if( m_plotSelected != "Raw Stats" && ( i == m_chart->GetNumberOfPlots() - 1 ) )
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
    if( m_plotSelected.contains( "Significant Betas" ) )
    {
        for( int i = m_nbrPlots; i < m_chart->GetNumberOfPlots() - 1; i++ )
        {
            vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( i );
            vtkPlotPoints::SafeDownCast( currentLine )->SetMarkerStyle( m_markerType );
            vtkPlotPoints::SafeDownCast( currentLine )->SetMarkerSize( m_markerSize );
        }
    }

    if( m_plotSelected.contains( "Average" ) )
    {
        if( m_chart->GetNumberOfPlots() == 2 )
        {
            vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( 1 );
            vtkPlotPoints::SafeDownCast( currentLine )->SetMarkerStyle( m_markerType );
            vtkPlotPoints::SafeDownCast( currentLine )->SetMarkerSize( m_markerSize );
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

    if( m_plotSelected == "Raw Data" || m_plotSelected == "Raw Stats" )
    {
        emit CovariatesAvailableForPlotting( m_binaryCovariates );
    }
    if( m_plotSelected == "Raw Betas by Properties" || m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
            m_plotSelected == "Omnibus Betas with Confidence Bands" || m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        emit CovariatesAvailableForPlotting( m_allCovariates );
    }
    if( m_plotSelected == "Raw Betas by Covariates" || m_plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" || m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" ||
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
    if( m_plotSelected == "Omnibus Local pvalues" || m_plotSelected == "Omnibus FDR Local pvalues" )
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
QList< double > Plot::QStringListToDouble( const QStringList& rowData )
{
    QList< double > rowDataDouble;
    foreach ( QString data, rowData )
    {
        rowDataDouble.append( data.toDouble() );
    }
    return rowDataDouble;
}

QList< QList< double > > Plot::DataToDouble( const QList< QStringList >& data )
{
    QList< QList< double > > dataDouble;
    foreach ( QStringList rowData, data )
    {
        dataDouble.append( QStringListToDouble( rowData ) );
    }
    return dataDouble;
}

void Plot::SortFilesByProperties( QString directory, const QStringList& files, QMap< QString, QList< QList< double > > >& data )
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
            m_dataSubMatrix = m_processing.GetDataFromFile( currentPath );
        }

        if( !index.isNull() )
        {
            data.insert( index, DataToDouble( m_processing.GetDataFromFile( currentPath ) ) );
        }
    }
}

void Plot::TransposeData( QList< QList< double > >& data, int firstRow, int firstColumn )
{
    QList< QList< double > > dataTransposed;
    for( int j = firstColumn; j < data.first().size(); j++ )
    {
        QList< double > rowData;
        for( int i = firstRow; i < data.size(); i++ )
        {
            rowData.append( data.at( i ).at( j ) );
        }
        dataTransposed.append( rowData );
    }
    data.clear();
    data = dataTransposed;
}

void Plot::TransposeDataInQMap( QMap< QString, QList< QList< double > > >& data, int firstRow, int firstColumn )
{
    QMap< QString, QList< QList< double > > >::Iterator iterData = data.begin();
    while( iterData != data.end() )
    {
        TransposeData( iterData.value(), firstRow, firstColumn );
        ++iterData;
    }
}

void Plot::RemoveUnmatchedSubjects( QList< QStringList >& rawData )
{
    QStringList currentFirstRow = rawData.first();

    foreach( QString subject, currentFirstRow )
    {
        if( !m_matchedSubjects.contains( subject ) && ( subject != currentFirstRow.first() ) )
        {
            m_subjects.removeAll( subject );

            int indexSubjectToDelete = currentFirstRow.indexOf( subject );
            currentFirstRow.removeAt( indexSubjectToDelete );
            for( int row = 0 ; row < rawData.size(); row++ )
            {
                rawData[ row ].removeAt( indexSubjectToDelete );
            }
        }
    }
}



void Plot::SetRawData()
{
    SortFilesByProperties( m_directory, m_csvRawDataFiles, m_dataRawData );
    TransposeDataInQMap( m_dataRawData, 1, 0 );
}

bool Plot::SetRawDataQCThreshold( QList< QStringList >& rawData )
{
    m_dataRawData.clear();

    if( !m_matchedSubjects.isEmpty() )
    {
        RemoveUnmatchedSubjects( rawData );

        m_dataRawData.insert( "FA", DataToDouble( rawData ) );

        TransposeDataInQMap( m_dataRawData, 1, 0 );
    }

    return !m_dataRawData.isEmpty();
}

void Plot::SetBeta()
{
    SortFilesByProperties( m_matlabDirectory, m_csvBetaFiles, m_dataBeta );
}

void Plot::SetOmnibusLpvalue( const QStringList& omnibusLpvalueFiles, QList< QList< double > >& omnibusLpvaluesData )
{
    QString currentPath = m_matlabDirectory + "/" + omnibusLpvalueFiles.first();
    QList< QList< double > > dataTempo = DataToDouble( m_processing.GetDataFromFile( currentPath ) );
    TransposeData( dataTempo, 0, 0 );
    omnibusLpvaluesData = dataTempo;
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
    QStringList nameFilterRawData = QStringList() << "*_RawData_*.csv" << "*_RawData.csv";
    m_csvRawDataFiles = QDir( m_directory ).entryList( nameFilterRawData, QDir::Files );
    m_csvRawDataFiles.sort();
    SetRawData();

    return( !m_dataRawData.isEmpty() && !m_dataSubMatrix.isEmpty() );
}

void Plot::GetBetaFiles()
{
    QStringList nameFilterBeta = QStringList() << "*_Betas_*.csv" << "*_Betas.csv";
    m_csvBetaFiles = QDir( m_matlabDirectory ).entryList( nameFilterBeta, QDir::Files );
    m_csvBetaFiles.sort();
    SetBeta();
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
}

void Plot::GetConfidenceBandsFiles()
{
    QStringList nameFilterConfidenceBands( "*_Omnibus_ConfidenceBands_*.csv" );
    m_csvConfidenceBandsFiles = QDir( m_matlabDirectory ).entryList( nameFilterConfidenceBands, QDir::Files );
    m_csvConfidenceBandsFiles.sort();
    SetConfidenceBands();
}

void Plot::GetPostHocFDRLpvalueFiles()
{
    QStringList nameFilterPostHocFDRLpvalue = QStringList() << "*_PostHoc_FDR_Local_pvalues_*.csv" << "*_PostHoc_FDR_Local_pvalues.csv";
    m_csvPostHocFDRLpvalueFiles = QDir( m_matlabDirectory ).entryList( nameFilterPostHocFDRLpvalue, QDir::Files );
    m_csvPostHocFDRLpvalueFiles.sort();
    SetPostHocFDRLpvalue();
}


void Plot::SetPlots()
{
    m_plotsUsed.clear();
    if( !m_dataRawData.isEmpty() )
    {
        m_plotsUsed.append( QStringList() << "Raw Data" << "Raw Stats" );
    }
    if( !m_dataBeta.isEmpty() )
    {
        m_plotsUsed.append( QStringList() << "Raw Betas by Properties" << "Raw Betas by Covariates" );
    }
    if( !m_dataOmnibusLpvalue.isEmpty() )
    {
        m_plotsUsed.append( QStringList() << "Omnibus Local pvalues" );
    }
    if( !m_dataOmnibusFDRLpvalue.isEmpty() )
    {
        m_plotsUsed.append( QStringList() << "Omnibus FDR Local pvalues" );
    }
    if( !m_dataBeta.isEmpty() && !m_dataOmnibusFDRLpvalue.isEmpty() )
    {
        m_plotsUsed.append( QStringList() << "Omnibus FDR Significant Betas by Properties" <<
                            "Omnibus FDR Significant Betas by Covariates" );
    }
    if( !m_dataBeta.isEmpty() && !m_dataConfidenceBands.isEmpty() )
    {
        m_plotsUsed.append( QStringList() << "Omnibus Betas with Confidence Bands" );
    }
    if( !m_dataPostHocFDRLpvalue.isEmpty() )
    {
        m_plotsUsed.append( QStringList() << "Post-Hoc FDR Local pvalues by Covariates" );
    }
    if( !m_dataRawData.isEmpty() && !m_dataPostHocFDRLpvalue.isEmpty() )
    {
        m_plotsUsed.append( QStringList() << "Post-Hoc FDR Significant Betas on Average Raw Data" );
    }
    if( !m_dataBeta.isEmpty() && !m_dataPostHocFDRLpvalue.isEmpty() )
    {
        m_plotsUsed.append( QStringList() << "Post-Hoc FDR Significant Betas by Properties" <<
                            "Post-Hoc FDR Significant Betas by Covariates" );
    }
    emit PlotsUsed( m_plotsUsed );
}

void Plot::SetProperties()
{
    foreach( QString property, m_dataRawData.keys() )
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

void Plot::SetSubjects()
{
    for( int i = 1; i < m_dataSubMatrix.size(); i++ )
    {
        m_subjects.append( m_dataSubMatrix.at( i ).first() );
    }
}

void Plot::SetCovariates()
{
    QStringList firstRaw = m_dataSubMatrix.first();
    QStringList firstDataRaw = m_dataSubMatrix.at( 1 );

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
    m_nbrPoints = m_abscissa.size();
}


void Plot::SeparateBinary( QList< QList< double > >& temp0Bin, QList< QList< double > >& temp1Bin )
{
    int indexCovariate = m_binaryCovariates.key( m_covariateSelected );
    for( int i = 0; i < m_ordinate.size(); i++ )
    {
        if( m_dataSubMatrix.at( i + 1 ).at( indexCovariate ).toDouble() == 0 )
        {
            temp0Bin.append( m_ordinate.at( i ) );
        }
        else
        {
            temp1Bin.append( m_ordinate.at( i ) );
        }
    }
}

void Plot::GetMeanAndStdDv( const QList< QList< double > >& tempBin, QList< double >& tempMean, QList< double >& tempStdDv )
{
    int nbrSubjects = tempBin.size();
    for( int i = 0; i < m_nbrPoints; i++ )
    {
        double currentMean = 0;
        for( int j = 0; j < nbrSubjects; j++ )
        {
            currentMean += tempBin.at( j ).at( i );
        }
        currentMean = currentMean / nbrSubjects;
        tempMean.append( currentMean );

        double currentStdDv = 0;
        for( int j = 0; j < nbrSubjects; j++ )
        {
            currentStdDv += std::pow( tempBin.at( j ).at( i ) - currentMean, 2 );
        }
        tempStdDv.append( std::sqrt( currentStdDv / ( nbrSubjects - 1 ) ) );
    }
}

QList< double > Plot::GetMean( const QList< QList< double > >& rawData )
{
    int nbrSubjects = rawData.size();
    QList< double > mean;

    for( int i = 0; i < m_nbrPoints; i++ )
    {
        double currentMean = 0;
        for( int j = 0; j < nbrSubjects; j++ )
        {
            currentMean += rawData.at( j ).at( i );
        }
        currentMean = currentMean / nbrSubjects;
        mean.append( currentMean );
    }

    return mean;
}


void Plot::ProcessRawStats( const QList< QList< double > >& tempBin, QList< double >& tempBinMean, QList< double >& tempBinUp, QList< double >& tempBinDown )
{
    QList< double > tempBinStdDv;
    GetMeanAndStdDv( tempBin, tempBinMean, tempBinStdDv );
    for( int i = 0; i < m_nbrPoints; i++ )
    {
        tempBinUp.append( tempBinMean.at( i ) + tempBinStdDv.at( i ) );
        tempBinDown.append( tempBinMean.at( i ) - tempBinStdDv.at( i ) );
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
    QList< QList< double > > ordinate = m_dataRawData.value( m_propertySelected );
    ordinate.removeFirst();
    return ordinate;
}

QList< QList< double > > Plot::LoadRawStats()
{
    QList< QList< double > > temp0Bin;
    QList< QList< double > > temp1Bin;
    SeparateBinary( temp0Bin, temp1Bin );

    QList< double > temp0BinUp;
    QList< double > temp0BinMean;
    QList< double > temp0BinDown;
    ProcessRawStats( temp0Bin, temp0BinMean, temp0BinUp, temp0BinDown );

    QList< double > temp1BinUp;
    QList< double > temp1BinMean;
    QList< double > temp1BinDown;
    ProcessRawStats( temp1Bin, temp1BinMean, temp1BinUp, temp1BinDown );

    QList< QList< double > > ordinate;
    ordinate.append( temp0BinUp );
    ordinate.append( temp0BinMean );
    ordinate.append( temp0BinDown );
    ordinate.append( temp1BinUp );
    ordinate.append( temp1BinMean );
    ordinate.append( temp1BinDown);

    for( int i = ordinate.size() - 1; i >= 0; i-- )
    {
        if( std::isnan( ordinate.at( i ).first() ) )
        {
            ordinate.removeAt( i );
        }
    }

    m_ordinate.clear();

    return ordinate;
}

QList< QList< double > > Plot::LoadSigBetasOnAverageRawData()
{
    QList< QList< double > > meanOrdinate = QList< QList< double > >() << GetMean( LoadRawData() );

    return meanOrdinate;
}

QList< QList< double > > Plot::LoadBetas()
{
    QList< QList< double > > ordinate;
    ordinate = m_dataBeta.value( m_propertySelected );
    SetSelectionToDisplayProperties();

    return ordinate;
}

QList< QList< double > > Plot::LoadBetaByCovariate()
{
    QList< QList< double > > ordinate;
    QMap< int, QString >::ConstIterator iterProperties = m_properties.cbegin();
    while( iterProperties != m_properties.cend() )
    {
        ordinate.append( m_dataBeta.value( iterProperties.value() ).at( m_allCovariates.key( m_covariateSelected ) ) );
        ++iterProperties;
    }
    SetSelectionToDisplayProperties();

    return ordinate;
}

QList< QList< double > > Plot::LoadOmnibusLpvalues( QList< QList< double > > omnibusLpvalues )
{
    QList< QList< double > > ordinate = omnibusLpvalues;
    m_selectionToDisplay.remove( m_selectionToDisplay.firstKey() );
    SetSelectionToDisplayProperties();

    return ToLog10( ordinate );
}

QList< QList< double > > Plot::LoadConfidenceBand()
{
    QList< QList< double > > ordinate;
    ordinate.append( m_dataConfidenceBands.value( m_propertySelected ).at( 2*m_allCovariates.key( m_covariateSelected ) + 1 ) );
    ordinate.append( m_dataBeta.value( m_propertySelected ).at( m_allCovariates.key( m_covariateSelected ) ) );
    ordinate.append( m_dataConfidenceBands.value( m_propertySelected ).at( 2*m_allCovariates.key( m_covariateSelected ) ) );

    return ordinate;
}

QList< QList< double > > Plot::LoadPostHocFDRLpvalues()
{
    QList< QList< double > > ordinate;
    QMap< int, QString >::ConstIterator iterProperties = m_properties.cbegin();
    while( iterProperties != m_properties.cend() )
    {
        ordinate.append( m_dataPostHocFDRLpvalue.value( iterProperties.value() ).at( m_covariatesNoIntercept.key( m_covariateSelected ) - 1 ) );
        ++iterProperties;
    }
    SetSelectionToDisplayProperties();

    return ToLog10( ordinate );
}

bool Plot::LoadData()
{
    m_ordinate.clear();
    if( ( m_plotSelected == "Raw Data" ) || ( m_plotSelected == "Raw Stats" ) )
    {
        if( !m_propertySelected.isEmpty() )
        {
            m_ordinate = LoadRawData();

            if( m_plotSelected == "Raw Stats" )
            {
                m_ordinate = LoadRawStats();
            }
        }
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas on Average Raw Data" )
    {
        if( !m_propertySelected.isEmpty() )
        {
            m_ordinate = LoadSigBetasOnAverageRawData();
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
    if( m_plotSelected == "Omnibus Betas with Confidence Bands" )
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


void Plot::AddEntriesRawData( vtkSmartPointer< vtkTable >& table )
{
    for( int i = 0; i < m_nbrPlots; i++ )
    {
        vtkSmartPointer< vtkDoubleArray > currentEntry = vtkSmartPointer< vtkDoubleArray >::New();
        currentEntry->SetName( m_subjects.at( i ).toStdString().c_str() );
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
        if( m_nbrPlots == 3 )
        {
            int indexCovariate = m_binaryCovariates.key( m_covariateSelected );
            QString name = m_dataSubMatrix.at( 1 ).at( indexCovariate ).toDouble() == 0 ?
                        QString( m_covariateSelected + " = 0 " ) : QString(  m_covariateSelected + " = 1 " );

            vtkSmartPointer< vtkDoubleArray > stdUp = vtkSmartPointer< vtkDoubleArray >::New();
            stdUp->SetName( QString( name + "std+" ).toStdString().c_str() );
            table->AddColumn( stdUp );
            vtkSmartPointer< vtkDoubleArray > mean = vtkSmartPointer< vtkDoubleArray >::New();
            mean->SetName( QString( name + "mean" ).toStdString().c_str() );
            table->AddColumn( mean );
            vtkSmartPointer< vtkDoubleArray > stdDown = vtkSmartPointer< vtkDoubleArray >::New();
            stdDown->SetName( QString( name + "std-" ).toStdString().c_str() );
            table->AddColumn( stdDown );
        }
        else
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

void Plot::AddEntriesSigBetasOnAverageRawData( vtkSmartPointer< vtkTable >& table )
{
    vtkSmartPointer< vtkDoubleArray > currentEntry = vtkSmartPointer< vtkDoubleArray >::New();
    currentEntry->SetName( QString( "Average " + m_propertySelected ).toStdString().c_str() );
    table->AddColumn( currentEntry );
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
    upperConfidenceBand->SetName( "Upper Confidence Band" );
    table->AddColumn( upperConfidenceBand );
    vtkSmartPointer< vtkDoubleArray > meanBetas = vtkSmartPointer< vtkDoubleArray >::New();
    meanBetas->SetName( "Mean Betas" );
    table->AddColumn( meanBetas );
    vtkSmartPointer< vtkDoubleArray > lowerConfidenceBand = vtkSmartPointer< vtkDoubleArray >::New();
    lowerConfidenceBand->SetName( "Lower Confidence Band" );
    table->AddColumn( lowerConfidenceBand );
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
    if( m_plotSelected == "Post-Hoc FDR Significant Betas on Average Raw Data" )
    {
        AddEntriesSigBetasOnAverageRawData( table );
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


double Plot::ApplyPearsonCorrelation( int indexLine, const QList< double >& meanRawData )
{
    double pearsonCorrelation = 1.0;

    if( m_nbrPoints > 1)
    {
        QList< double > currentLine = m_dataRawData.value( m_propertySelected ).at( indexLine );

        int indexMax = meanRawData.size();

        double sumMean = 0;
        for( int i = 0; i < indexMax; i++ )
        {
            sumMean += meanRawData.at( i );
        }

        double sumMeanSquare = 0;
        for( int i = 0; i < indexMax; i++ )
        {
            sumMeanSquare += meanRawData.at( i ) * meanRawData.at( i );
        }

        double sumCurrentLine = 0;
        for( int i = 0; i < indexMax; i++ )
        {
            sumCurrentLine += currentLine.at( i );
        }

        double sumCurrentLineSquare = 0;
        for( int i = 0; i < indexMax; i++ )
        {
            sumCurrentLineSquare += currentLine.at( i ) * currentLine.at( i );
        }

        double meanXcurrentLine = 0;
        for( int i = 0; i < indexMax; i++ )
        {
            meanXcurrentLine += meanRawData.at( i ) * currentLine.at( i );
        }

        pearsonCorrelation = ( meanXcurrentLine - ( sumMean * sumCurrentLine ) / m_nbrPoints ) /
                std::sqrt( ( sumMeanSquare - std::pow( sumMean, 2 ) / m_nbrPoints ) * ( sumCurrentLineSquare - std::pow( sumCurrentLine, 2 ) / m_nbrPoints ) );
    }

    return pearsonCorrelation;
}

void Plot::InitLines()
{
    for( int i = 0; i < m_nbrPlots; i++ )
    {
        m_chart->AddPlot( vtkChart::LINE );
        m_chart->GetPlot( i )->SetSelectable( m_plotSelected == "Raw Data" ? true : false );
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

void Plot::AddLineSigBetas( const vtkSmartPointer< vtkTable >& table, bool betaDisplayedByProperties, bool isOmnibus, int i )
{
    vtkSmartPointer< vtkDoubleArray > abscissaSigBetaTempo = vtkSmartPointer< vtkDoubleArray >::New();
    abscissaSigBetaTempo->SetNumberOfComponents( 1 );
    abscissaSigBetaTempo->SetName( "Abscissa SigBeta" );
    vtkSmartPointer< vtkDoubleArray > dataSigBetaTempo = vtkSmartPointer< vtkDoubleArray >::New();
    dataSigBetaTempo->SetNumberOfComponents( 1 );
    dataSigBetaTempo->SetName( QString( m_lineNames.at( i ) + " SigBeta" ).toStdString().c_str() );
    int index = 0;
    for( int j = 0; j < m_nbrPoints; j++ )
    {
        if( betaDisplayedByProperties )
        {
            if( isOmnibus )
            {
                if( m_dataOmnibusFDRLpvalue.at( m_allCovariates.key( m_lineNames.at( i ) ) - 1 ).at( j ) <= m_pvalueThreshold )
                {
                    abscissaSigBetaTempo->InsertNextValue( table->GetValue( j, 0 ).ToDouble() );
                    dataSigBetaTempo->InsertNextValue( table->GetValue( j, i + 1 ).ToDouble() );
                    index++;
                }
            }
            else
            {
                if( m_dataPostHocFDRLpvalue.value( m_propertySelected ).at( m_allCovariates.key( m_lineNames.at( i ) ) - 1 ).at( j ) <= m_pvalueThreshold )
                {
                    abscissaSigBetaTempo->InsertNextValue( table->GetValue( j, 0 ).ToDouble() );
                    dataSigBetaTempo->InsertNextValue( table->GetValue( j, i + 1 ).ToDouble() );
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
                    abscissaSigBetaTempo->InsertNextValue( table->GetValue( j, 0 ).ToDouble() );
                    dataSigBetaTempo->InsertNextValue( table->GetValue( j, i + 1 ).ToDouble() );
                    index++;
                }
            }
            else
            {
                if( m_dataPostHocFDRLpvalue.value( m_lineNames.at( i ) ).at( m_allCovariates.key( m_covariateSelected ) - 1 ).at( j ) <= m_pvalueThreshold )
                {
                    abscissaSigBetaTempo->InsertNextValue( table->GetValue( j, 0 ).ToDouble() );
                    dataSigBetaTempo->InsertNextValue( table->GetValue( j, i + 1 ).ToDouble() );
                    index++;
                }
            }
        }
    }

    if( ( dataSigBetaTempo->GetSize() != 0 ) && ( abscissaSigBetaTempo->GetSize() != 0 ) )
    {
        vtkSmartPointer< vtkTable > tableSigBetas = vtkSmartPointer< vtkTable >::New();
        tableSigBetas->SetNumberOfRows( index );
        tableSigBetas->AddColumn( abscissaSigBetaTempo );
        tableSigBetas->AddColumn( dataSigBetaTempo );

        vtkSmartPointer< vtkPlot > sigBetas = m_chart->AddPlot( vtkChart::POINTS );
        sigBetas->SetSelectable( false );
        sigBetas->SetInputData( tableSigBetas, 0, 1 );
        vtkPlotPoints::SafeDownCast( sigBetas )->SetMarkerStyle( m_markerType );
        vtkPlotPoints::SafeDownCast( sigBetas )->SetMarkerSize( m_markerSize );
        sigBetas->SetColor( m_lineColors.at( i ).first(), m_lineColors.at( i ).at( 1 ), m_lineColors.at( i ).at( 2 ), 255 );
        sigBetas->SetVisible( m_selectionToDisplay.value( i ).second.first );
    }
}

void Plot::AddLineRawData( const vtkSmartPointer< vtkTable >& table )
{
    for( int i = 0; i < m_nbrPlots; i++ )
    {
        vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( i );
        currentLine->SetInputData( table, 0, i + 1 );


        QList< int > color;
        double currentLineWidth = 0;
        if( m_covariateSelected.isEmpty() )
        {
            color = m_allColors.value( "Black" );
            currentLineWidth = m_lineWidth / 2;
        }
        else
        {
            int indexCovariate = m_binaryCovariates.key( m_covariateSelected );
            if( m_dataSubMatrix.at( i + 1 ).at( indexCovariate ).toDouble() == 0 )
            {
                color = m_allColors.value( "Black" );
                currentLineWidth = m_lineWidth / 2;
            }
            else
            {
                color = m_allColors.value( m_covariatesProperties.value( m_binaryCovariates.key( m_covariateSelected ) ).second.second );
                currentLineWidth = m_lineWidth;
            }
        }

        currentLine->SetColor( color.first(), color.at( 1 ), color.at( 2 ), 255 );
        currentLine->SetWidth( currentLineWidth );
    }
}

void Plot::AddLineRawStats( const vtkSmartPointer< vtkTable >& table )
{
    for( int i = 0; i < m_nbrPlots; i++ )
    {
        vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( i );
        currentLine->SetInputData( table, 0, i + 1 );

        QList< int > color;
        if( m_covariateSelected.isEmpty() )
        {
            color = m_allColors.value( "Black" );
        }
        else
        {
            if( m_nbrPlots == 3 )
            {
                int indexCovariate = m_binaryCovariates.key( m_covariateSelected );
                if( m_dataSubMatrix.at( 1 ).at( indexCovariate ).toDouble() == 0 )
                {
                    color = m_allColors.value( "Black" );
                }
                else
                {
                    color = m_allColors.value( m_covariatesProperties.value( m_binaryCovariates.key( m_covariateSelected ) ).second.second );
                }
            }
            else
            {
                if( i < 3 )
                {
                    color = m_allColors.value( "Black" );
                }
                else
                {
                    color = m_allColors.value( m_covariatesProperties.value( m_binaryCovariates.key( m_covariateSelected ) ).second.second );
                }
            }
        }
        currentLine->SetColor( color.first(), color.at( 1 ), color.at( 2 ), 255 );


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

void Plot::AddLineSigBetasOnAverageRawData( const vtkSmartPointer< vtkTable >& table )
{
    QList< int > lineColor = m_allColors.value( "Black" );
    vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( 0 );
    currentLine->SetInputData( table, 0, 1 );
    currentLine->SetColor( lineColor.first(), lineColor.at( 1 ), lineColor.at( 2 ), 255 );
    currentLine->SetWidth( m_lineWidth );

    if( !m_covariateSelected.isEmpty() )
    {
        QList< int > markerColor = m_allColors.value( m_covariatesProperties.value( m_covariatesNoIntercept.key( m_covariateSelected ) ).second.second );

        vtkSmartPointer< vtkDoubleArray > abscissaSigBetaTempo = vtkSmartPointer< vtkDoubleArray >::New();
        abscissaSigBetaTempo->SetNumberOfComponents( 1 );
        abscissaSigBetaTempo->SetName( "Abscissa SigBeta" );
        vtkSmartPointer< vtkDoubleArray > dataSigBetaTempo = vtkSmartPointer< vtkDoubleArray >::New();
        dataSigBetaTempo->SetNumberOfComponents( 1 );
        dataSigBetaTempo->SetName( QString( m_covariateSelected + " SigBeta" ).toStdString().c_str() );
        int index = 0;
        for( int j = 0; j < m_nbrPoints; j++ )
        {
            if( m_dataPostHocFDRLpvalue.value( m_propertySelected ).at( m_allCovariates.key( m_covariateSelected ) - 1 ).at( j ) <= m_pvalueThreshold )
            {
                abscissaSigBetaTempo->InsertNextValue( table->GetValue( j, 0 ).ToDouble() );
                dataSigBetaTempo->InsertNextValue( table->GetValue( j, 1 ).ToDouble() );
                index++;
            }
        }

        if( ( dataSigBetaTempo->GetSize() != 0 ) && ( abscissaSigBetaTempo->GetSize() != 0 ) )
        {
            vtkSmartPointer< vtkTable > tableSigBetas = vtkSmartPointer< vtkTable >::New();
            tableSigBetas->SetNumberOfRows( index );
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
}

void Plot::AddLineBetas( const vtkSmartPointer< vtkTable >& table )
{
    for( int i = 0; i < m_nbrPlots; i++ )
    {
        vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( i );
        currentLine->SetInputData( table, 0, i + 1 );
        currentLine->SetColor( m_lineColors.at( i ).first(), m_lineColors.at( i ).at( 1 ), m_lineColors.at( i ).at( 2 ), 255 );
        currentLine->SetWidth( m_lineWidth );

        if( m_plotSelected.contains( "Significant Betas" ) )
        {
            bool betaDisplayedByProperties = m_plotSelected.contains( "by Properties" );
            bool isOmnibus = m_plotSelected.contains( "Omnibus" );

            if( ( betaDisplayedByProperties && ( m_lineNames.at( i ) != m_allCovariates.first() ) ) || !betaDisplayedByProperties )
            {
                AddLineSigBetas( table, betaDisplayedByProperties, isOmnibus, i );
            }
        }

        currentLine->SetVisible( m_selectionToDisplay.value( i ).second.first );
    }

    AddSignificantLevel( 0 );
}

void Plot::AddLineLPvalue( const vtkSmartPointer< vtkTable >& table )
{
    int shift = ( m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" ) ? 0 : 1;

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

void Plot::AddLineLConfidenceBands( const vtkSmartPointer< vtkTable >& table )
{
    for( int i = 0; i < m_nbrPlots; i++ )
    {
        vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( i );
        currentLine->SetInputData( table, 0, i + 1 );
        if( i%2 )
        {
            QList< int > color = m_allColors.value( m_covariatesProperties.value( m_binaryCovariates.key( m_covariateSelected ) ).second.second );
            currentLine->SetColor( color.first(), color.at( 1 ), color.at( 2 ), 255 );
        }
        else
        {
            currentLine->SetColor( 0, 0, 0, 255 );
            currentLine->GetPen()->SetLineType( vtkPen::DASH_LINE );
        }
        currentLine->SetWidth( m_lineWidth );
    }

    AddSignificantLevel( 0 );
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
    if( m_plotSelected.contains( "Average" ) )
    {
        AddLineSigBetasOnAverageRawData( table );
    }
    if( ( m_plotSelected.contains( "Raw Betas" ) || m_plotSelected.contains( "Significant Betas" ) ) && !m_plotSelected.contains( "Average" ) )
    {
        AddLineBetas( table );
    }
    if( m_plotSelected == "Omnibus Local pvalues" || m_plotSelected == "Omnibus FDR Local pvalues" ||
            m_plotSelected == "Post-Hoc FDR Local pvalues by Covariates" )
    {
        AddLineLPvalue( table );
    }
    if( m_plotSelected == "Omnibus Betas with Confidence Bands" )
    {
        AddLineLConfidenceBands( table );
    }
}

void Plot::AddQCThresholdLines( const vtkSmartPointer< vtkTable >& table, const QList< double >& atlas )
{
    InitLines();

    QList< QList< double > > tempoRawData = m_dataRawData.value( m_propertySelected );
    tempoRawData.removeFirst();
    QList< double > meanRawData = !atlas.isEmpty() ? atlas : GetMean( tempoRawData );

    QStringList subjectsCorrelated, subjectsNotCorrelated;
    for( int i = 0; i < m_nbrPlots; i++ )
    {
        vtkSmartPointer< vtkPlot > currentLine = m_chart->GetPlot( i );
        currentLine->SetInputData( table, 0, i + 1 );
        QList< int > color;

        double pearsonCorrelation = ApplyPearsonCorrelation( i + 1, meanRawData );

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

    AddMean( meanRawData );

    emit UpdateSubjectsCorrelated( subjectsCorrelated, subjectsNotCorrelated );
}


void Plot::GetyMinMax()
{
    double yMin = m_yMin.first ? m_yMin.second : 1000;
    double yMax = m_yMax.first ? m_yMax.second : -1000;

    for( int i = 0; i < m_ordinate.size(); i++ )
    {
        QList< double > rowData = m_ordinate.at( i );

        if( ( m_plotSelected == "Raw Data" || m_plotSelected == "Raw Stats" || m_plotSelected.contains( "Average" ) )
                || ( !( m_plotSelected == "Raw Data" || m_plotSelected == "Raw Stats" || m_plotSelected.contains( "Average" ) ) && m_selectionToDisplay.value( i ).second.first ) )
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
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetMinimumLimit( xMin );
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetMaximumLimit( xMax );
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetRange( xMin, xMax );

    m_chart->GetAxis( vtkAxis::BOTTOM )->GetGridPen()->SetWidth( 0.25 );
    m_chart->GetAxis( vtkAxis::BOTTOM )->GetGridPen()->SetColor( 128, 128, 128, 255 );
    m_chart->GetAxis( vtkAxis::BOTTOM )->SetGridVisible( m_gridOn );
}

void Plot::SetOrdinateProperties( double yMin, double yMax )
{
    m_chart->GetAxis( vtkAxis::LEFT )->SetMinimumLimit( yMin );
    m_chart->GetAxis( vtkAxis::LEFT )->SetMaximumLimit( yMax );
    m_chart->GetAxis( vtkAxis::LEFT )->SetRange( yMin, yMax );
    m_chart->GetAxis( vtkAxis::LEFT )->SetNotation( m_abscissaNotation );

    m_chart->GetAxis( vtkAxis::LEFT )->GetGridPen()->SetWidth( 0.25 );
    m_chart->GetAxis( vtkAxis::LEFT )->GetGridPen()->SetColor( 128, 128, 128, 255 );
    m_chart->GetAxis( vtkAxis::LEFT )->SetGridVisible( m_gridOn );

}

void Plot::SetAxisProperties()
{
    if( !m_abscissa.isEmpty() )
    {
        SetAbscissaProperties( m_abscissa.first(), m_abscissa.last() );
    }

    GetyMinMax();
    SetOrdinateProperties( m_yMinMax[ 0 ], m_yMinMax[ 1 ] );
}

void Plot::SetQCThresholdAxisProperties()
{
    if( !m_abscissa.isEmpty() )
    {
        double xMin = m_abscissa.first();
        double xMax = m_abscissa.last();
        m_chart->GetAxis( vtkAxis::BOTTOM )->SetMinimumLimit( xMin );
        m_chart->GetAxis( vtkAxis::BOTTOM )->SetMaximumLimit( xMax );
        m_chart->GetAxis( vtkAxis::BOTTOM )->SetRange( xMin, xMax );
        m_chart->GetAxis( vtkAxis::BOTTOM )->SetLabelsVisible( false );
        m_chart->GetAxis( vtkAxis::BOTTOM )->SetTitle( "Arc Length" );
    }

    GetyMinMax();
    double yMin = m_yMinMax[ 0 ];
    double yMax = m_yMinMax[ 1 ];
    m_chart->GetAxis( vtkAxis::LEFT )->SetMinimumLimit( yMin );
    m_chart->GetAxis( vtkAxis::LEFT )->SetMaximumLimit( yMax );
    m_chart->GetAxis( vtkAxis::LEFT )->SetRange( yMin, yMax );
    m_chart->GetAxis( vtkAxis::LEFT )->SetLabelsVisible( false );
    m_chart->GetAxis( vtkAxis::LEFT )->SetTitle( "" );
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
    writer->SetSortToBSP();
    writer->SetLineWidthFactor( 1.0 );
    writer->CompressOff();
    writer->LandscapeOff();
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
