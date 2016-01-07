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
    m_table = vtkSmartPointer<vtkTable>::New();
    m_qvtkWidget->SetRenderWindow( m_view->GetRenderWindow() );
}

void Plot::SetDirectory( QString directory )
{
    m_directory = directory;
    m_matlabOutputDir = directory + "/MatlabOutputs";

    GetRawDataFiles();
    GetBetaFiles();
    GetOmnibusLpvalueFiles();
    GetOmnibusFDRLpvalueFiles();
    GetPostHocLpvalueFiles();
    GetPostHocFDRLpvalueFiles();

    SetOutcomes();
    SetCovariates();
    SetAbscissa();
}

void Plot::ResetDataFile()
{
    m_csvRawDataFile.clear();
    m_csvBetaFile.clear();
    m_csvOmnibusLpvalueFile.clear();
    m_csvOmnibusFDRLpvalueFile.clear();
    m_csvPostHocLpvalueFile.clear();
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
        emit CovariateUsed( m_binaryCovariate );
    }
    else
    {
        if( m_plotSelected == "Raw Betas" )
        {
            emit CovariateUsed( m_allCovariate );
        }
        else
        {
            emit CovariateUsed( m_covariateNoIntercept );
        }
    }

}

void Plot::SelectOutcome( QString outcome )
{
    m_outcomeSelected = outcome;
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

        // Creating a table with 2 named arrays
        qDebug() << endl << "Adding axis";
        AddEntry();

        // Adding data
        qDebug() << endl << "Adding data";
        SetData();

        // Adding plots
        qDebug() << "Adding plots";
        AddLines();

//        // Setting chart properties
//        qDebug() << "Setting chart properties";
//        SetChartProperties();
    }
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

void Plot::SetOmnibusLpvalue( QStringList files )
{
    QString currentPath = m_matlabOutputDir + "/" + files.first();
    QList < QList < double > > dataTempo = ToDouble( m_process.GetDataFromFile( currentPath ) );
    TransposeData( dataTempo, 0, 0 );
    m_csvOmnibusLpvalue = dataTempo;
}

void Plot::SetOmnibusFDRLpvalue( QStringList files )
{
    QString currentPath = m_matlabOutputDir + "/" + files.first();
    QList < QList < double > > dataTempo = ToDouble( m_process.GetDataFromFile( currentPath ) );
    TransposeData( dataTempo, 0, 0 );
    m_csvOmnibusFDRLpvalue = dataTempo;
}

void Plot::SetPostHocLpvalue( QStringList files, QMap< QString, QList < QList < double > > > &data )
{
    SortFiles( m_matlabOutputDir, files, data );
    Transpose( data, 0, 0 );
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
    qDebug()  << endl << "m_csvRawDataFile" << ": " << m_csvRawDataFile;
    SetRawData( m_csvRawDataFile, m_csvRawData );

    QMap< QString, QList < QList < double > > >::ConstIterator iterRawData = m_csvRawData.begin();
    while( iterRawData != m_csvRawData.end() )
    {
        qDebug() << iterRawData.value().size() << "x" << iterRawData.value().first().size();
        ++iterRawData;
    }
    qDebug() << m_csvCovariate.size() << "x" << m_csvCovariate.first().size();
}

void Plot::GetBetaFiles()
{
    QStringList nameFilterBeta( "*Betas.csv" );
    m_csvBetaFile = QDir( m_matlabOutputDir ).entryList( nameFilterBeta );
    qDebug()  << endl << "m_csvBetaFile" << ": " << m_csvBetaFile;
    SetBeta( m_csvBetaFile, m_csvBeta );

    QMap< QString, QList < QList < double > > >::ConstIterator iterRawData = m_csvBeta.begin();
    while( iterRawData != m_csvBeta.end() )
    {
        qDebug() << iterRawData.value().size() << "x" << iterRawData.value().first().size();
        ++iterRawData;
    }
}

void Plot::GetOmnibusLpvalueFiles()
{
    QStringList nameFilterOmnibusLpvalue( "*Omnibus_Local_pvalues.csv" );
    m_csvOmnibusLpvalueFile = QDir( m_matlabOutputDir ).entryList( nameFilterOmnibusLpvalue );
    qDebug() << endl << "m_csvOmnibusLpvalueFile" << ": " << m_csvOmnibusLpvalueFile;
    SetOmnibusLpvalue( m_csvOmnibusLpvalueFile );

    qDebug() << m_csvOmnibusLpvalue.size() << "x" << m_csvOmnibusLpvalue.first().size();
}

void Plot::GetOmnibusFDRLpvalueFiles()
{
    QStringList nameFilterOmnibusFDRLpvalue( "*Omnibus_FDR_Local_pvalues.csv" );
    m_csvOmnibusFDRLpvalueFile = QDir( m_matlabOutputDir ).entryList( nameFilterOmnibusFDRLpvalue );
    qDebug() << endl << "m_csvOmnibusFDRLpvalueFile" << ": " << m_csvOmnibusFDRLpvalueFile;
    SetOmnibusFDRLpvalue( m_csvOmnibusFDRLpvalueFile );

    qDebug() << m_csvOmnibusFDRLpvalue.size() << "x" << m_csvOmnibusFDRLpvalue.first().size();
}

void Plot::GetPostHocLpvalueFiles()
{
    QStringList nameFilterPostHocLpvalue( "*PostHoc_Local_pvalues.csv" );
    m_csvPostHocLpvalueFile = QDir( m_matlabOutputDir ).entryList( nameFilterPostHocLpvalue );
    qDebug() << endl << "m_csvPostHocLpvalueFile" << ": " << m_csvPostHocLpvalueFile << endl;
    SetPostHocLpvalue( m_csvPostHocLpvalueFile, m_csvPostHocLpvalue );

    QMap< QString, QList < QList < double > > >::ConstIterator iterPostHocLpvalue = m_csvPostHocLpvalue.begin();
    while( iterPostHocLpvalue != m_csvPostHocLpvalue.end() )
    {
        qDebug() << iterPostHocLpvalue.value().size() << "x" << iterPostHocLpvalue.value().first().size();
        ++iterPostHocLpvalue;
    }
}

void Plot::GetPostHocFDRLpvalueFiles()
{
    QStringList nameFilterPostHocFDRLpvalue( "*PostHoc_FDR_Local_pvalues.csv" );
    m_csvPostHocFDRLpvalueFile = QDir( m_matlabOutputDir ).entryList( nameFilterPostHocFDRLpvalue );
    qDebug() << endl << "m_csvPostHocFDRLpvalueFile" << ": " << m_csvPostHocFDRLpvalueFile << endl;
    SetPostHocFDRLpvalue( m_csvPostHocFDRLpvalueFile, m_csvPostHocFDRLpvalue );

    QMap< QString, QList < QList < double > > >::ConstIterator iterPostHocFDRLpvalue = m_csvPostHocFDRLpvalue.begin();
    while( iterPostHocFDRLpvalue != m_csvPostHocFDRLpvalue.end() )
    {
        qDebug() << iterPostHocFDRLpvalue.value().size() << "x" << iterPostHocFDRLpvalue.value().first().size();
        ++iterPostHocFDRLpvalue;
    }
}


void Plot::SetOutcomes()
{
    QMap< int, QString > tempoOutcome;
    foreach( QString path, m_csvRawDataFile )
    {
        if( path.contains( "_ad_", Qt::CaseInsensitive ) )
        {
            tempoOutcome.insert( 0, "AD" );
        }
        if( path.contains( "_md_", Qt::CaseInsensitive ) )
        {
            tempoOutcome.insert( 1, "RD" );
        }
        if( path.contains( "_rd_", Qt::CaseInsensitive ) )
        {
            tempoOutcome.insert( 2, "MD" );
        }
        if( path.contains( "_fa_", Qt::CaseInsensitive ) )
        {
            tempoOutcome.insert( 3, "FA" );
        }
    }
    m_outcomeUsed = tempoOutcome.values();
    emit OutcomeUsed( m_outcomeUsed );
    qDebug() << endl << "m_outcomeUsed: " << m_outcomeUsed;
}

void Plot::SetCovariates()
{
    QStringList firstRaw = m_csvCovariate.first();
    QStringList firstDataRaw = m_csvCovariate.at( 1 );

    m_allCovariate.insert( 0, "Intercept" );
    for( int i = 1; i < firstRaw.size(); i++ )
    {
        m_allCovariate.insert( i, firstRaw.at( i ) );
        m_covariateNoIntercept.insert( i, firstRaw.at( i ) );
        if( ( firstDataRaw .at( i ).toInt() == 0 ) || ( firstDataRaw.at( i ).toInt() == 1 ) )
        {
           m_binaryCovariate.insert( i, firstRaw.at( i ) );
        }
    }
    qDebug() << endl << "m_allCovariate: " << m_allCovariate;
    qDebug() << "m_covariateNoIntercept: " << m_covariateNoIntercept;
    qDebug() << "m_binaryCovariate: " << m_binaryCovariate;
}

void Plot::SetAbscissa()
{
    m_abscissa.clear();
    m_abscissa.append( m_csvRawData.first().first() );
    m_nbrPoint = m_abscissa.size();
    qDebug() << endl << "m_abscissa (" << m_nbrPoint << "): " << m_abscissa;
}


void Plot::ResetLoadData()
{
    m_yMin = 0;
    m_yMax = 0;
    m_ordinate.clear();
    m_line.clear();
    m_table->SetNumberOfRows( 0 );
}


void Plot::SeparateBinary( QList< QList < double > > &temp0Bin, QList< QList < double > > &temp1Bin )
{
    int indexCovariate = m_binaryCovariate.key( m_covariateSelected );
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
    qDebug() << endl << "SeparateBinary";
    qDebug() << "temp0Bin: " << temp0Bin.size() << "x" << temp0Bin.first().size();
    qDebug() << "temp1Bin: " << temp1Bin.size() << "x" << temp1Bin.first().size();
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


QList< QList < double > > Plot::LoadRawData()
{
    QList< QList < double > > ordinate = m_csvRawData.value( m_outcomeSelected );
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
    ordinate = m_csvBeta.value( m_outcomeSelected );
    return ordinate;
}

QList< QList < double > > Plot::LoadBetaByCovariate( QString covariate )
{
    QList< QList < double > > ordinate;
    foreach( QString outcome, m_outcomeUsed )
    {
        ordinate.append( m_csvBeta.value( outcome ).at( m_allCovariate.key( covariate ) ) );
    }
    return ordinate;
}

QList< QList < double > > Plot::LoadOmnibusLpvalues()
{
    QList< QList < double > > ordinate;
    ordinate = m_csvOmnibusLpvalue;
    return ordinate;
}

QList< QList < double > > Plot::LoadOmnibusFDRLpvalues()
{
    QList< QList < double > > ordinate;
    ordinate = m_csvOmnibusFDRLpvalue;
    return ordinate;
}

QList< QList < double > > Plot::LoadPostHocLpvalues()
{
    QList< QList < double > > ordinate;
    ordinate = m_csvPostHocLpvalue.value( m_outcomeSelected );
    return ordinate;
}

QList< QList < double > > Plot::LoadPostHocFDRLpvalues()
{
    QList< QList < double > > ordinate;
    ordinate = m_csvPostHocFDRLpvalue.value( m_outcomeSelected );
    return ordinate;
}

bool Plot::LoadData()
{
    if( ( m_plotSelected == "Raw Data" ) || ( m_plotSelected == "Raw Stats" ) )
    {
        if( !m_outcomeSelected.isEmpty() && !m_covariateSelected.isEmpty() )
        {
            m_ordinate = LoadRawData();
            qDebug() << endl << "Load Raw Data (" << m_outcomeSelected << "/" << m_covariateSelected << "): " << m_ordinate.size() << "x" << m_ordinate.first().size();

            if( m_plotSelected == "Raw Stats" )
            {
                m_ordinate = LoadRawStats();
                qDebug() << endl << "Load Raw Stats (" << m_outcomeSelected << "/" << m_covariateSelected << "): " << m_ordinate.size() << "x" << m_ordinate.first().size();

            }
        }
    }
    if( m_plotSelected == "Raw Betas" )
    {
        if( !m_outcomeSelected.isEmpty() )
        {
            m_ordinate = LoadBetas();
            qDebug() << endl << "Load Betas (" << m_outcomeSelected << "): " << m_ordinate.size() << "x" << m_ordinate.first().size();

        }
    }
    if( m_plotSelected == "Omnibus Local pvalues" )
    {
        m_ordinate = LoadOmnibusLpvalues();
        qDebug() << endl << "Load Omnibus Local pvalues: " << m_ordinate.size() << "x" << m_ordinate.first().size();

    }

    if( m_plotSelected == "Omnibus FDR Local pvalues" )
    {
        m_ordinate = LoadOmnibusFDRLpvalues();
        qDebug() << endl << "Load Omnibus FDR Local pvalues: " << m_ordinate.size() << "x" << m_ordinate.first().size();

    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Properties" )
    {
        if( !m_outcomeSelected.isEmpty() )
        {
            m_ordinate = LoadBetas();
            m_ordinate.removeFirst();
            qDebug() << endl << "Load Omnibus FDR Significant Betas by Properties: (" << m_outcomeSelected << "): " << m_ordinate.size() << "x" << m_ordinate.first().size();

        }
    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Covariates" )
    {
        if( !m_covariateSelected.isEmpty() )
        {
            m_ordinate = LoadBetaByCovariate( m_covariateSelected );
            qDebug() << endl << "Load Omnibus FDR Significant Betas by Covariates: (" << m_covariateSelected << "): " << m_ordinate.size() << "x" << m_ordinate.first().size();

        }
    }
    if( m_plotSelected == "Post-Hoc Local pvalues" )
    {
        if( !m_outcomeSelected.isEmpty() )
        {
            m_ordinate = LoadPostHocLpvalues();
            qDebug() << endl << "Load Post-Hoc Local pvalues: (" << m_outcomeSelected << "): " << m_ordinate.size() << "x" << m_ordinate.first().size();

        }
    }
    if( m_plotSelected == "Post-Hoc FDR Local pvalues" )
    {
        if( !m_outcomeSelected.isEmpty() )
        {
            m_ordinate = LoadPostHocFDRLpvalues();
            qDebug() << endl << "Load Post-Hoc FDR Local pvalues: (" << m_outcomeSelected << "): " << m_ordinate.size() << "x" << m_ordinate.first().size();

        }
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        if( !m_outcomeSelected.isEmpty() )
        {
            m_ordinate = LoadBetas();
            m_ordinate.removeFirst();
            qDebug() << endl << "Load Post-Hoc FDR Significant Betas by Properties: (" << m_outcomeSelected << "): " << m_ordinate.size() << "x" << m_ordinate.first().size();

        }
    }
    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        if( !m_covariateSelected.isEmpty() )
        {
            m_ordinate = LoadBetaByCovariate( m_covariateSelected );
            qDebug() << endl << "Load Post-Hoc FDR Significant Betas by Covariates: (" << m_covariateSelected << "): " << m_ordinate.size() << "x" << m_ordinate.first().size();

        }
    }
    return !( m_abscissa.isEmpty() | m_ordinate.isEmpty() );
}


void Plot::AddEntryRawData()
{
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkSmartPointer<vtkFloatArray> currentEntry = vtkSmartPointer<vtkFloatArray>::New();

        currentEntry->SetName( QString( "Subject " + QString::number( i + 1 ) ).toStdString().c_str() );
        m_table->AddColumn( currentEntry );
    }
}

void Plot::AddEntryRawStats()
{
    vtkSmartPointer<vtkFloatArray> std0up = vtkSmartPointer<vtkFloatArray>::New();
    std0up->SetName( QString( m_covariateSelected + " = 0 std+" ).toStdString().c_str() );
    m_table->AddColumn( std0up );
    vtkSmartPointer<vtkFloatArray> mean0 = vtkSmartPointer<vtkFloatArray>::New();
    mean0->SetName( QString( m_covariateSelected + " = 0 mean" ).toStdString().c_str() );
    m_table->AddColumn( mean0 );
    vtkSmartPointer<vtkFloatArray> std0down = vtkSmartPointer<vtkFloatArray>::New();
    std0down->SetName( QString( m_covariateSelected + " = 0 std-" ).toStdString().c_str() );
    m_table->AddColumn( std0down );
    vtkSmartPointer<vtkFloatArray> std1up = vtkSmartPointer<vtkFloatArray>::New();
    std1up->SetName( QString( m_covariateSelected + " = 1 std+" ).toStdString().c_str() );
    m_table->AddColumn( std1up );
    vtkSmartPointer<vtkFloatArray> mean1 = vtkSmartPointer<vtkFloatArray>::New();
    mean1->SetName( QString( m_covariateSelected + " = 1 mean" ).toStdString().c_str() );
    m_table->AddColumn( mean1 );
    vtkSmartPointer<vtkFloatArray> std1down = vtkSmartPointer<vtkFloatArray>::New();
    std1down->SetName( QString( m_covariateSelected + " = 1 std-" ).toStdString().c_str() );
    m_table->AddColumn( std1down );
}

void Plot::AddEntryCovariate( QMap< int, QString > covariates )
{
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkSmartPointer<vtkFloatArray> currentEntry = vtkSmartPointer<vtkFloatArray>::New();
        currentEntry->SetName( covariates.value( i ).toStdString().c_str() );
        m_table->AddColumn( currentEntry );
    }
}

void Plot::AddEntryOutcome()
{
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkSmartPointer<vtkFloatArray> currentEntry = vtkSmartPointer<vtkFloatArray>::New();
        currentEntry->SetName( m_outcomeUsed.value( i ).toStdString().c_str() );
        m_table->AddColumn( currentEntry );
    }
}

void Plot::AddEntry()
{
    vtkSmartPointer<vtkFloatArray> abscissa = vtkSmartPointer<vtkFloatArray>::New();
    abscissa->SetName( "Arc Length" );
    m_table->AddColumn( abscissa );

    if( m_plotSelected == "Raw Data" )
    {
        AddEntryRawData();
    }
    if( m_plotSelected == "Raw Stats" )
    {
        AddEntryRawStats();
    }
    if( m_plotSelected == "Raw Betas" )
    {
        AddEntryCovariate( m_allCovariate );
    }
    if( m_plotSelected == "Omnibus Local pvalues" || m_plotSelected == "Omnibus FDR Local pvalues" ||
            m_plotSelected == "Omnibus FDR Significant Betas by Properties" ||
            m_plotSelected == "Post-Hoc Local pvalues" || m_plotSelected == "Post-Hoc FDR Local pvalues" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {
        AddEntryCovariate( m_covariateNoIntercept );
    }
    if( m_plotSelected == "Omnibus FDR Significant Betas by Covariates" ||
            m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {
        AddEntryOutcome();
    }
}


void Plot::SetData()
{
    m_table->SetNumberOfRows( m_nbrPoint );
    for( int i = 0; i < m_nbrPoint; i++ )
    {
        // Adding abscissa
        m_table->SetValue( i, 0, m_abscissa.at( i ) );

        // Adding data
        for( int j = 0; j < m_nbrPlot; j++ )
        {
            m_table->SetValue( i, j + 1, m_ordinate.at( j ).at( i ) );
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

void Plot::AddLineRawData()
{
    int indexCovariate = m_binaryCovariate.key( m_covariateSelected );
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkPlot *currentLine = m_line.value( i );
        currentLine->SetInputData( m_table, 0, i + 1 );
        if( m_csvCovariate.at( i + 1 ).at( indexCovariate ).toInt() == 0 )
        {
            currentLine->SetColor( 0, 0, 255, 255 );
        }
        else
        {
            currentLine->SetColor( 255, 0, 0, 255 );
        }
        currentLine->SetWidth( 1.0 );
    }
}

void Plot::AddLineRawStats()
{
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkPlot *currentLine = m_line.value( i );
        currentLine->SetInputData( m_table, 0, i + 1 );
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

        currentLine->SetWidth( 1.0 );
    }
}

void Plot::AddLineRawBeta()
{
    for( int i = 0; i < m_nbrPlot; i++ )
    {
        vtkPlot *currentLine = m_line.value( i );
        vtkPlotPoints::SafeDownCast( currentLine )->SetMarkerStyle( vtkPlotPoints::CIRCLE );
        currentLine->SetColor( 255*0.3*i, 255*0.2*i, 0, 255 );
        currentLine->SetWidth( 1.0 );
    }
}







void Plot::AddLines()
{
    InitLines();

    if( m_plotSelected == "Raw Data" )
    {
        AddLineRawData();
    }


    if( m_plotSelected == "Raw Stats" )
    {
        AddLineRawStats();
    }


    if( m_plotSelected == "Raw Betas" )
    {
        AddLineRawBeta();
    }


    if( m_plotSelected == "Omnibus Local pvalues" )
    {

    }


    if( m_plotSelected == "Omnibus FDR Local pvalues" )
    {

    }


    if( m_plotSelected == "Omnibus FDR Significant Betas by Properties" )
    {

    }


    if( m_plotSelected == "Omnibus FDR Significant Betas by Covariates" )
    {

    }


    if( m_plotSelected == "Post-Hoc Local pvalues" )
    {

    }


    if( m_plotSelected == "Post-Hoc FDR Local pvalues" )
    {

    }


    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Properties" )
    {

    }


    if( m_plotSelected == "Post-Hoc FDR Significant Betas by Covariates" )
    {

    }
}













//void Plot::AddPlots( float red, float green, float blue, float opacity )
//{
//    if( m_plotSelected == "Raw Data" )
//    {
//        for( int i = 0; i < m_nbrPlot; i++ )
//        {
//            QPair< int, QString > currentPair;
//            currentPair.first = i;
//            currentPair.second = "Subject " + QString::number( i + 1 );
//            m_line.insert( currentPair, m_chart->AddPlot( vtkChart::LINE ) );
//        }

//        QMap< QPair< int, QString >, vtkPlot* >::ConstIterator iterPlot = m_line.begin();
//        int i = 1;
//        while( iterPlot != m_line.end() )
//        {
//            iterPlot.value()->SetInputData( m_table.GetPointer(), 0, i );

//            if( m_isCovariateBinary && ( m_csvRawData.value( 4 ).at( i ).at( m_indexColumn ).toInt() == 0 ) )
//            {
//                iterPlot.value()->SetColor( 0, 0, 255, 255 );
//            }
//            else
//            {
//                iterPlot.value()->SetColor( 255, 0, 0, 255 );
//            }
//            iterPlot.value()->SetWidth( 1.0 );
//            ++iterPlot;
//            i++;
//        }
//    }

//    if( m_plotSelected == "Raw Stats" )
//    {
//        if( m_nbrPlot == 6 )
//        {
//            QPair< int, QString > up0Pair;
//            up0Pair.first = 0;
//            up0Pair.second = m_covariateSelected + " up : 0";
//            m_line.insert( up0Pair, m_chart->AddPlot( vtkChart::LINE ) );

//            QPair< int, QString > mean0Pair;
//            mean0Pair.first = 1;
//            mean0Pair.second = m_covariateSelected + " mean : 0";
//            m_line.insert( mean0Pair, m_chart->AddPlot( vtkChart::LINE ) );

//            QPair< int, QString > down0Pair;
//            down0Pair.first = 2;
//            down0Pair.second = m_covariateSelected + " down : 0";
//            m_line.insert( down0Pair, m_chart->AddPlot( vtkChart::LINE ) );

//            QPair< int, QString > up1Pair;
//            up1Pair.first = 3;
//            up1Pair.second = m_covariateSelected + " up : 1";
//            m_line.insert( up1Pair, m_chart->AddPlot( vtkChart::LINE ) );

//            QPair< int, QString > mean1Pair;
//            mean1Pair.first = 4;
//            mean1Pair.second = m_covariateSelected + " mean : 1";
//            m_line.insert( mean1Pair, m_chart->AddPlot( vtkChart::LINE ) );

//            QPair< int, QString > down1Pair;
//            down1Pair.first = 5;
//            down1Pair.second = m_covariateSelected + " down : 1";
//            m_line.insert( down1Pair, m_chart->AddPlot( vtkChart::LINE ) );
//        }
//        else
//        {
//            QPair< int, QString > upPair;
//            upPair.first = 0;
//            upPair.second = m_covariateSelected + " up";
//            m_line.insert( upPair, m_chart->AddPlot( vtkChart::LINE ) );

//            QPair< int, QString > meanPair;
//            meanPair.first = 1;
//            meanPair.second = m_covariateSelected + " mean";
//            m_line.insert( meanPair, m_chart->AddPlot( vtkChart::LINE ) );

//            QPair< int, QString > downPair;
//            downPair.first = 2;
//            downPair.second = m_covariateSelected + " down";
//            m_line.insert( downPair, m_chart->AddPlot( vtkChart::LINE ) );
//        }


//    }

//    if( m_plotSelected == "Omnibus" )
//    {
//        QMap< QPair < int, QString >, bool >::ConstIterator iterCovariates = m_covariates.begin();
//        while( iterCovariates != m_covariates.end() )
//        {
//            QPair< int, QString > currentPair;
//            currentPair.first = iterCovariates.key().first + 1;
//            currentPair.second = iterCovariates.key().second;
//            m_line.insert( currentPair, m_chart->AddPlot( vtkChart::LINE ) );
//            ++iterCovariates;
//        }


//    }

//    if( m_plotSelected == "Post-Hoc" )
//    {

//    }

////    m_line.insert( "Intercept", m_chart->AddPlot( vtkChart::LINE ) );
////    m_line.insert( "SSRIExposure", m_chart->AddPlot( vtkChart::LINE ) );
////    m_line.insert( "Sex", m_chart->AddPlot( vtkChart::LINE ) );
////    m_line.insert( "GestAgeBirth", m_chart->AddPlot( vtkChart::LINE ) );
////    m_line.insert( "DaysSinceBirth", m_chart->AddPlot( vtkChart::LINE ) );
////    m_line.insert( "DTIDirection", m_chart->AddPlot( vtkChart::LINE ) );
////    QMap< QString, vtkPlot* >::ConstIterator iterPlot = m_line.begin();
////    int i = 1;
////    while( iterPlot != m_line.end() )
////    {
////        iterPlot.value()->SetInputData( table.GetPointer(), 0, i );
////        vtkPlotPoints::SafeDownCast( iterPlot.value() )->SetMarkerStyle( vtkPlotPoints::CIRCLE );
////        iterPlot.value()->SetColor( 255*0.3*i, 255*0.2*i, 0, 255 );
////        iterPlot.value()->SetWidth( 1.0 );
////        ++iterPlot;
////        i++;
////    }
//}












void Plot::FindyMinMax()
{
//    foreach( QList < double > rowData, m_ordinate )
//    {
//        foreach( double data, rowData )
//        {
//            if( m_yMin == 0 )
//            {
//                m_yMin = data;
//            }
//            else
//            {
//                if( data < m_yMin )
//                {
//                    m_yMin = data;
//                }
//            }
//            if( m_yMax == 0 )
//            {
//                m_yMax = data;
//            }
//            else
//            {
//                if( data > m_yMax )
//                {
//                    m_yMax = data;
//                }
//            }
//        }
//    }
//    qDebug() << "m_yMin: " << m_yMin;
//    qDebug() << "m_yMax: " << m_yMax;
}

void Plot::SetChartProperties()
{
//    FindyMinMax();
//    if( m_title.isEmpty() )
//    {
////        m_title = "Title";
//        m_title =  m_plotSelected + " " + m_outcomeSelected + " " + m_covariateSelected;
//    }
//    if( m_xName.isEmpty() )
//    {
//        m_xName = "Arc Length";
//    }
//    if( m_yName.isEmpty() )
//    {
//        m_yName = "";
//    }
//    if( m_yMinChecked )
//    {
//        m_yMin = m_yMinGiven;
//    }
//    if( m_yMaxChecked )
//    {
//        m_yMax = m_yMaxGiven;
//    }
//    qDebug() << "m_yMin: " << m_yMin << " | m_yMax: " << m_yMax ;


//    m_chart->GetAxis( vtkAxis::BOTTOM )->SetTitle( m_xName.toStdString() );
//    m_chart->GetAxis( vtkAxis::BOTTOM )->SetRange( m_abscissa.first(), m_abscissa.last() );
//    m_chart->GetAxis( vtkAxis::BOTTOM )->SetMinimumLimit( m_abscissa.first() );
//    m_chart->GetAxis( vtkAxis::BOTTOM )->SetMaximumLimit( m_abscissa.last() );
////    m_chart->GetAxis( vtkAxis::BOTTOM )->SetRange( std::floor( m_abscissa.first() ), std::ceil( m_abscissa.last() ) );
////    m_chart->GetAxis( vtkAxis::BOTTOM )->SetMinimumLimit( std::floor( m_abscissa.first() ) );
////    m_chart->GetAxis( vtkAxis::BOTTOM )->SetMaximumLimit( std::ceil( m_abscissa.last() ) );
//    m_chart->GetAxis( vtkAxis::BOTTOM )->GetGridPen()->SetColor( 127, 127, 127, 255 );

//    m_chart->GetAxis( vtkAxis::LEFT )->SetTitle( m_yName.toStdString() );
//    m_chart->GetAxis( vtkAxis::LEFT )->SetRange( m_yMin, m_yMax );
//    m_chart->GetAxis( vtkAxis::LEFT )->SetMinimumLimit( m_yMin );
//    m_chart->GetAxis( vtkAxis::LEFT )->SetMaximumLimit( m_yMax );
//    m_chart->GetAxis( vtkAxis::LEFT )->GetGridPen()->SetColor( 127, 127, 127, 255 );

//    m_chart->SetTitle( m_title.toStdString() );
//    m_chart->GetTitleProperties()->SetBold( 1 );

////    m_chart->SetShowLegend( true );
}
