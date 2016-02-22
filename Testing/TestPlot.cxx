#include "TestPlot.h"

#include <QDebug>
#include <vtkContextMapper2D.h>

TestPlot::TestPlot()
{
}


/**********************************************************************/
/*************************** Tests ************************************/
/**********************************************************************/
bool TestPlot::Test_SetQVTKWidget()
{
    Plot plot;
    QSharedPointer< QVTKWidget > qvtkWidget = QSharedPointer< QVTKWidget >( new QVTKWidget() );


    bool testSetQVTKWidget_Passed = plot.m_qvtkWidget.isNull();
    plot.SetQVTKWidget( qvtkWidget );
    testSetQVTKWidget_Passed = testSetQVTKWidget_Passed && !plot.m_qvtkWidget.isNull();


    if( !testSetQVTKWidget_Passed )
    {
        std::cerr << "/!\\ Test_SetQVTKWidget() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetQVTKWidget( QSharedPointer< QVTKWidget > qvtkWidget )" << std::endl;
        std::cerr << "\t+ m_qvtkWidget not initialized correctly" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetQVTKWidget() PASSED" << std::endl;
    }

    return testSetQVTKWidget_Passed;
}



bool TestPlot::Test_QStringListToDouble()
{
    Plot plot;
    QStringList stringList = QStringList() << "1.11" << "2.22" << "3.33" << "4.44" << "5.55";
    QList< double > doubleList = QList< double >() << 1.11 << 2.22 << 3.33 << 4.44 << 5.55;


    bool testQStringListToDouble_Passed = doubleList == plot.QStringListToDouble( stringList );
    if( !testQStringListToDouble_Passed )
    {
        std::cerr << "/!\\ Test_QStringListToDouble() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with QStringListToDouble( QStringList rowData )" << std::endl;
        std::cerr << "\t+ convertion from QStringList to QList < double > failed" << std::endl;
    }
    else
    {
        std::cerr << "Test_QStringListToDouble() PASSED" << std::endl;
    }

    return testQStringListToDouble_Passed;
}

bool TestPlot::Test_DataToDouble()
{
    Plot plot;
    QStringList stringList1 = QStringList() << "1.11" << "2.22" << "3.33" << "4.44" << "5.55";
    QStringList stringList2 = QStringList() << "6.66" << "7.77" << "8.88" << "9.99" << "10.1010";
    QList< QStringList > stringData = QList< QStringList >() << stringList1 << stringList2;
    QList< double > doubleList1 = QList< double >() << 1.11 << 2.22 << 3.33 << 4.44 << 5.55;
    QList< double > doubleList2 = QList< double >() << 6.66 << 7.77 << 8.88 << 9.99 << 10.1010;
    QList< QList< double > > doubleData = QList< QList< double > >() << doubleList1 << doubleList2;


    bool testDataToDouble_Passed = doubleData == plot.DataToDouble( stringData );
    if( !testDataToDouble_Passed )
    {
        std::cerr << "/!\\ Test_DataToDouble() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with DataToDouble( QList < QStringList > data )" << std::endl;
        std::cerr << "\t+ convertion from QList < QStringList > to QList < QList < double > > failed" << std::endl;
    }
    else
    {
        std::cerr << "Test_DataToDouble() PASSED" << std::endl;
    }

    return testDataToDouble_Passed;
}

bool TestPlot::Test_SortFilesByProperties( QString rdRawData, QString faRawData, QString subMatrixRawData )
{
    Plot plot;
    QStringList wrongFiles  = QStringList() << "./path/wrongFile1.csv" << "./path/wrongFile2.csv";
    QStringList files = QStringList() << faRawData << rdRawData;
    QList<double> rdDataRow0 = QList<double>() << QString( "Arc_length vs Data" ).toDouble() << QString( "F02-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble()
                                               << QString( "F05-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble() << QString( "F06-1_42_DWI_QCed_VC_DTI_embed" ).toDouble()
                                               << QString( "F07-1_42_DWI_QCed_VC_DTI_embed" ).toDouble() << QString( "F09-1_42_DWI_QCed_VC_DTI_embed" ).toDouble();
    QList<double> rdDataRow1 = QList<double>() << -39.1846 << 0.00117111 << 0.00118478 << 0.00131709 << 0.00121005 << 0.00141595;
    QList<double> rdDataRow2 = QList<double>() << -38.1846 << 0.00118018 << 0.00117143 << 0.0013476 << 0.00124804 << 0.00137842;
    QList<double> rdDataRow3 = QList<double>() << -37.1846 << 0.00119539 << 0.00116328 << 0.00137232 << 0.00127349 << 0.00135757;
    QList<double> rdDataRow4 = QList<double>() << -36.1846 << 0.00120302 << 0.00116113 << 0.00138538 << 0.0012901 << 0.00133806;
    QList<double> rdDataRow5 = QList<double>() << -35.1846 << 0.00119349 << 0.00115982 << 0.00138459 << 0.00130296 << 0.00131466;
    QList<QList<double> > rdData = QList<QList<double> > () << rdDataRow0 << rdDataRow1 << rdDataRow2 << rdDataRow3 << rdDataRow4 << rdDataRow5;
    QList<double> faDataRow0 = QList<double>() << QString( "Arc_length vs Data" ).toDouble() << QString( "F02-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble()
                                               << QString( "F05-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble() << QString( "F06-1_42_DWI_QCed_VC_DTI_embed" ).toDouble()
                                               << QString( "F07-1_42_DWI_QCed_VC_DTI_embed" ).toDouble() << QString( "F09-1_42_DWI_QCed_VC_DTI_embed" ).toDouble();
    QList<double> faDataRow1 = QList<double>() << -39.1846 << 0.101391 << 0.100461 << 0.127887 << 0.135817 << 0.100568;
    QList<double> faDataRow2 = QList<double>() << -38.1846 << 0.089874 << 0.121609 << 0.120668 << 0.140971 << 0.109092;
    QList<double> faDataRow3 = QList<double>() << -37.1846 << 0.0899232 << 0.140557 << 0.116923 << 0.145793 << 0.120203;
    QList<double> faDataRow4 = QList<double>() << -36.1846 << 0.106037 << 0.154739 << 0.12252 << 0.153154 << 0.13125;
    QList<double> faDataRow5 = QList<double>() << -35.1846 << 0.130884 << 0.165439 << 0.13898 << 0.163628 << 0.140868;
    QList<QList<double> > faData = QList<QList<double> > () << faDataRow0 << faDataRow1 << faDataRow2 << faDataRow3 << faDataRow4 << faDataRow5;
    QStringList subMatrixDataRow0 = QStringList() << "Case_orig_Shaili" << "GENDER" << "DaysPostBirth" << "ICV_y0";
    QStringList subMatrixDataRow1 = QStringList() << "F02-1_dwi_35_all_QCed_VC_DTI_embed" << "0" << "16" << "494341.5";
    QStringList subMatrixDataRow2 = QStringList() << "F05-1_dwi_35_all_QCed_VC_DTI_embed" << "1" << "54" << "483068";
    QStringList subMatrixDataRow3 = QStringList() << "F06-1_42_DWI_QCed_VC_DTI_embed" << "0" << "31" << "471322.4";
    QStringList subMatrixDataRow4 = QStringList() << "F07-1_42_DWI_QCed_VC_DTI_embed" << "1" << "18" << "432712.4";
    QStringList subMatrixDataRow5 = QStringList() << "F09-1_42_DWI_QCed_VC_DTI_embed" << "1" << "38" << "449280.2";
    QList< QStringList > expectedDataSubMatrix = QList< QStringList >() << subMatrixDataRow0 << subMatrixDataRow1 << subMatrixDataRow2 << subMatrixDataRow3 << subMatrixDataRow4 << subMatrixDataRow5;
    QMap< QString, QList<QList<double> > > expectedData;
    expectedData.insert( "FA", faData );
    expectedData.insert( "RD", rdData );
    QMap< QString, QList<QList<double> > > data;


    plot.SortFilesByProperties( "", wrongFiles, data );
    bool testWrongFiles = data.isEmpty();

    data.clear();
    plot.SortFilesByProperties( "", files, data );
    bool testFilesNoSubMatrix = data == expectedData;

    data.clear();
    files.append( subMatrixRawData );
    plot.SortFilesByProperties( "", files, data );
    bool testFilesSubMatrix = data == expectedData;
    bool testSubMatrix = plot.m_dataSubMatrix == expectedDataSubMatrix;


    bool testSortFilesByProperties_Passed = testWrongFiles && testFilesNoSubMatrix && testFilesSubMatrix && testSubMatrix;
    if( !testSortFilesByProperties_Passed )
    {
        std::cerr << "/!\\ Test_SortFilesByProperties() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SortFilesByProperties( QString directory, QStringList files,"
                     " QMap< QString, QList<QList<double> > > &data )" << std::endl;
        if( !testWrongFiles )
        {
            std::cerr << "\t+ no data should be sorted and extracted" << std::endl;
        }
        if( !testFilesNoSubMatrix )
        {
            std::cerr << "\t+ data not sorted and extracted (without subMatrix file)" << std::endl;
        }
        if( !testFilesSubMatrix )
        {
            std::cerr << "\t+ data not sorted and extracted (with subMatrix file)" << std::endl;
        }
        if( !testSubMatrix )
        {
            std::cerr << "\t+ subMatrix data not extracted" << std::endl;
        }
    }
    else
    {
        std::cerr << "Test_SortFilesByProperties() PASSED" << std::endl;
    }

    return testSortFilesByProperties_Passed;
}

bool TestPlot::Test_TransposeData()
{
    Plot plot;
    QList<double> rdDataRow0 = QList<double>() << QString( "Arc_length vs Data" ).toDouble() << QString( "F02-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble()
                                               << QString( "F05-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble() << QString( "F06-1_42_DWI_QCed_VC_DTI_embed" ).toDouble()
                                               << QString( "F07-1_42_DWI_QCed_VC_DTI_embed" ).toDouble() << QString( "F09-1_42_DWI_QCed_VC_DTI_embed" ).toDouble();
    QList<double> rdDataRow1 = QList<double>() << -39.1846 << 0.00117111 << 0.00118478 << 0.00131709 << 0.00121005 << 0.00141595;
    QList<double> rdDataRow2 = QList<double>() << -38.1846 << 0.00118018 << 0.00117143 << 0.0013476 << 0.00124804 << 0.00137842;
    QList<double> rdDataRow3 = QList<double>() << -37.1846 << 0.00119539 << 0.00116328 << 0.00137232 << 0.00127349 << 0.00135757;
    QList<double> rdDataRow4 = QList<double>() << -36.1846 << 0.00120302 << 0.00116113 << 0.00138538 << 0.0012901 << 0.00133806;
    QList<double> rdDataRow5 = QList<double>() << -35.1846 << 0.00119349 << 0.00115982 << 0.00138459 << 0.00130296 << 0.00131466;
    QList<QList<double> > rdData = QList<QList<double> > () << rdDataRow0 << rdDataRow1 << rdDataRow2 << rdDataRow3 << rdDataRow4 << rdDataRow5;
    QList<double> rdDataTranspRow0 = QList<double>() << -39.1846 << -38.1846 << -37.1846 << -36.1846 << -35.1846;
    QList<double> rdDataTranspRow1 = QList<double>() << 0.00117111 << 0.00118018 << 0.00119539 << 0.00120302 << 0.00119349;
    QList<double> rdDataTranspRow2 = QList<double>() << 0.00118478 << 0.00117143 << 0.00116328 << 0.00116113 << 0.00115982;
    QList<double> rdDataTranspRow3 = QList<double>() << 0.00131709 << 0.0013476 << 0.00137232 << 0.00138538 << 0.00138459;
    QList<double> rdDataTranspRow4 = QList<double>() << 0.00121005 << 0.00124804 << 0.00127349 << 0.0012901 << 0.00130296;
    QList<double> rdDataTranspRow5 = QList<double>() << 0.00141595 << 0.00137842 << 0.00135757 << 0.00133806 << 0.00131466;
    QList<QList<double> > expectedRDDataTransp = QList<QList<double> > () << rdDataTranspRow0 << rdDataTranspRow1 << rdDataTranspRow2 << rdDataTranspRow3 << rdDataTranspRow4 << rdDataTranspRow5;


    plot.TransposeData( rdData, 1, 0 );


    bool testTransposeData_Passed = rdData == expectedRDDataTransp;
    if( !testTransposeData_Passed )
    {
        std::cerr << "/!\\ Test_TransposeData() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with TransposeData( QList < QList < double > > &data, int firstRow, int firstColumn )" << std::endl;
    }
    else
    {
        std::cerr << "Test_TransposeData() PASSED" << std::endl;
    }

    return testTransposeData_Passed;
}

bool TestPlot::Test_TransposeDataInQMap()
{
    Plot plot;
    QList<double> rdDataRow0 = QList<double>() << QString( "Arc_length vs Data" ).toDouble() << QString( "F02-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble()
                                               << QString( "F05-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble() << QString( "F06-1_42_DWI_QCed_VC_DTI_embed" ).toDouble()
                                               << QString( "F07-1_42_DWI_QCed_VC_DTI_embed" ).toDouble() << QString( "F09-1_42_DWI_QCed_VC_DTI_embed" ).toDouble();
    QList<double> rdDataRow1 = QList<double>() << -39.1846 << 0.00117111 << 0.00118478 << 0.00131709 << 0.00121005 << 0.00141595;
    QList<double> rdDataRow2 = QList<double>() << -38.1846 << 0.00118018 << 0.00117143 << 0.0013476 << 0.00124804 << 0.00137842;
    QList<double> rdDataRow3 = QList<double>() << -37.1846 << 0.00119539 << 0.00116328 << 0.00137232 << 0.00127349 << 0.00135757;
    QList<double> rdDataRow4 = QList<double>() << -36.1846 << 0.00120302 << 0.00116113 << 0.00138538 << 0.0012901 << 0.00133806;
    QList<double> rdDataRow5 = QList<double>() << -35.1846 << 0.00119349 << 0.00115982 << 0.00138459 << 0.00130296 << 0.00131466;
    QList<QList<double> > rdData = QList<QList<double> > () << rdDataRow0 << rdDataRow1 << rdDataRow2 << rdDataRow3 << rdDataRow4 << rdDataRow5;
    QList<double> rdDataTranspRow0 = QList<double>() << -39.1846 << -38.1846 << -37.1846 << -36.1846 << -35.1846;
    QList<double> rdDataTranspRow1 = QList<double>() << 0.00117111 << 0.00118018 << 0.00119539 << 0.00120302 << 0.00119349;
    QList<double> rdDataTranspRow2 = QList<double>() << 0.00118478 << 0.00117143 << 0.00116328 << 0.00116113 << 0.00115982;
    QList<double> rdDataTranspRow3 = QList<double>() << 0.00131709 << 0.0013476 << 0.00137232 << 0.00138538 << 0.00138459;
    QList<double> rdDataTranspRow4 = QList<double>() << 0.00121005 << 0.00124804 << 0.00127349 << 0.0012901 << 0.00130296;
    QList<double> rdDataTranspRow5 = QList<double>() << 0.00141595 << 0.00137842 << 0.00135757 << 0.00133806 << 0.00131466;
    QList<QList<double> > expectedRDDataTransp = QList<QList<double> > () << rdDataTranspRow0 << rdDataTranspRow1 << rdDataTranspRow2 << rdDataTranspRow3 << rdDataTranspRow4 << rdDataTranspRow5;
    QList<double> faDataRow0 = QList<double>() << QString( "Arc_length vs Data" ).toDouble() << QString( "F02-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble()
                                               << QString( "F05-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble() << QString( "F06-1_42_DWI_QCed_VC_DTI_embed" ).toDouble()
                                               << QString( "F07-1_42_DWI_QCed_VC_DTI_embed" ).toDouble() << QString( "F09-1_42_DWI_QCed_VC_DTI_embed" ).toDouble();
    QList<double> faDataRow1 = QList<double>() << -39.1846 << 0.101391 << 0.100461 << 0.127887 << 0.135817 << 0.100568;
    QList<double> faDataRow2 = QList<double>() << -38.1846 << 0.089874 << 0.121609 << 0.120668 << 0.140971 << 0.109092;
    QList<double> faDataRow3 = QList<double>() << -37.1846 << 0.0899232 << 0.140557 << 0.116923 << 0.145793 << 0.120203;
    QList<double> faDataRow4 = QList<double>() << -36.1846 << 0.106037 << 0.154739 << 0.12252 << 0.153154 << 0.13125;
    QList<double> faDataRow5 = QList<double>() << -35.1846 << 0.130884 << 0.165439 << 0.13898 << 0.163628 << 0.140868;
    QList<QList<double> > faData = QList<QList<double> > () << faDataRow0 << faDataRow1 << faDataRow2 << faDataRow3 << faDataRow4 << faDataRow5;
    QList<double> faDataTranspRow0 = QList<double>() << -39.1846 << -38.1846 << -37.1846 << -36.1846 << -35.1846;
    QList<double> faDataTranspRow1 = QList<double>() << 0.101391 << 0.089874 << 0.0899232 << 0.106037 << 0.130884;
    QList<double> faDataTranspRow2 = QList<double>() << 0.100461 << 0.121609 << 0.140557 << 0.154739 << 0.165439;
    QList<double> faDataTranspRow3 = QList<double>() << 0.127887 << 0.120668 << 0.116923 << 0.12252 << 0.13898;
    QList<double> faDataTranspRow4 = QList<double>() << 0.135817 << 0.140971 << 0.145793 << 0.153154 << 0.163628;
    QList<double> faDataTranspRow5 = QList<double>() << 0.100568 << 0.109092 << 0.120203 << 0.13125 << 0.140868;
    QList<QList<double> > expectedFADataTransp = QList<QList<double> > () << faDataTranspRow0 << faDataTranspRow1 << faDataTranspRow2 << faDataTranspRow3 << faDataTranspRow4 << faDataTranspRow5;
    QMap< QString, QList<QList<double> > > data;
    data.insert( "FA", faData );
    data.insert( "RD", rdData );
    QMap< QString, QList<QList<double> > > expectedTranspDataMap;
    expectedTranspDataMap.insert( "RD", expectedRDDataTransp );
    expectedTranspDataMap.insert( "FA", expectedFADataTransp );


    plot.TransposeDataInQMap( data, 1, 0 );


    bool testTransposeDataInQMap_Passed = data == expectedTranspDataMap;
    if( !testTransposeDataInQMap_Passed )
    {
        std::cerr << "/!\\ Test_TransposeDataInQMap() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with TransposeDataInQMap( QMap< QString, QList<QList<double> > > &data, int firstRow, int firstColumn )" << std::endl;
    }
    else
    {
        std::cerr << "Test_TransposeDataInQMap() PASSED" << std::endl;
    }

    return testTransposeDataInQMap_Passed;
}


bool TestPlot::Test_SetRawData( QString rdFilePath, QString faFilePath, QString subMatrixFilePath,
                                QString rdTransposeFilePath, QString faTransposeFilePath, QString dataDir )
{
    Plot plot;
    Processing processing;
    QList < QList < double > > rdTransposedData = plot.DataToDouble( processing.GetDataFromFile( rdTransposeFilePath ) );
    QList < QList < double > > faTransposedData = plot.DataToDouble( processing.GetDataFromFile( faTransposeFilePath ) );
    QMap< QString, QList < QList < double > > > expectedDataRawData;
    expectedDataRawData.insert( "RD", rdTransposedData );
    expectedDataRawData.insert( "FA", faTransposedData );
    QList< QStringList > expectedDataSubMatrix = processing.GetDataFromFile( subMatrixFilePath );


    plot.m_directory = dataDir;
    plot.m_csvRawDataFiles = QStringList() << QFileInfo( QFile( rdFilePath ) ).fileName() << QFileInfo( QFile( faFilePath ) ).fileName() << QFileInfo( QFile( subMatrixFilePath ) ).fileName();
    plot.SetRawData();

    bool testProperties = plot.m_dataRawData == expectedDataRawData;
    bool testSubMatrix= plot.m_dataSubMatrix == expectedDataSubMatrix;


    bool testSetRawData_Passed = testProperties && testSubMatrix;
    if( !testSetRawData_Passed )
    {
        std::cerr << "/!\\ Test_SetRawData() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetRawData()" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetRawData() PASSED" << std::endl;
    }

    return testSetRawData_Passed;
}

bool TestPlot::Test_SetBeta( QString rdBetaFilePath, QString faBetaFilePath, QString dataDir )
{
    Plot plot;
    Processing processing;
    QList < QList < double > > rdBetaData = plot.DataToDouble( processing.GetDataFromFile( rdBetaFilePath ) );
    QList < QList < double > > faBetaData = plot.DataToDouble( processing.GetDataFromFile( faBetaFilePath ) );
    QMap< QString, QList < QList < double > > > expectedBetaData;
    expectedBetaData.insert( "RD", rdBetaData );
    expectedBetaData.insert( "FA", faBetaData );


    plot.m_matlabDirectory = dataDir;
    plot.m_csvBetaFiles = QStringList() << QFileInfo( QFile( rdBetaFilePath ) ).fileName() << QFileInfo( QFile( faBetaFilePath ) ).fileName();
    plot.SetBeta();


    bool testSetBeta_Passed = plot.m_dataBeta == expectedBetaData;
    if( !testSetBeta_Passed )
    {
        std::cerr << "/!\\ Test_SetBeta() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetBeta()" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetBeta() PASSED" << std::endl;
    }

    return testSetBeta_Passed;
}

bool TestPlot::Test_SetOmnibusLpvalue( QString omnibusLpvalueFilePath, QString omnibusLpvalueTransposedFilePath, QString dataDir )
{
    Plot plot;
    Processing processing;
    QList < QList < double > > omnibusLpvaluesData;
    QList < QList < double > > expectedOmnibusLpvaluesData = plot.DataToDouble( processing.GetDataFromFile( omnibusLpvalueTransposedFilePath ) );


    plot.m_matlabDirectory = dataDir;
    plot.SetOmnibusLpvalue( QStringList() << QFileInfo( QFile( omnibusLpvalueFilePath ) ).fileName(), omnibusLpvaluesData );


    bool testSetOmnibusLpvalue_Passed = omnibusLpvaluesData == expectedOmnibusLpvaluesData;
    if( !testSetOmnibusLpvalue_Passed )
    {
        std::cerr << "/!\\ Test_SetOmnibusLpvalue() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetOmnibusLpvalue(QStringList omnibusLpvalueFiles, QList < QList < double > > &omnibusLpvaluesData )" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetOmnibusLpvalue() PASSED" << std::endl;
    }

    return testSetOmnibusLpvalue_Passed;
}

bool TestPlot::Test_SetConfidenceBands( QString rdConfidenceBandsFilePath, QString faConfidenceBandsFilePath, QString dataDir )
{
    Plot plot;
    Processing processing;
    QList < QList < double > > rdConfidenceBandsData = plot.DataToDouble( processing.GetDataFromFile( rdConfidenceBandsFilePath ) );
    QList < QList < double > > faConfidenceBandsData = plot.DataToDouble( processing.GetDataFromFile( faConfidenceBandsFilePath ) );
    QMap< QString, QList < QList < double > > > expectedConfidenceBandsData;
    expectedConfidenceBandsData.insert( "RD", rdConfidenceBandsData );
    expectedConfidenceBandsData.insert( "FA", faConfidenceBandsData );


    plot.m_matlabDirectory = dataDir;
    plot.m_csvConfidenceBandsFiles = QStringList() << QFileInfo( QFile( rdConfidenceBandsFilePath ) ).fileName() << QFileInfo( QFile( faConfidenceBandsFilePath ) ).fileName();
    plot.SetConfidenceBands();


    bool testSetConfidenceBands_Passed = plot.m_dataConfidenceBands == expectedConfidenceBandsData;
    if( !testSetConfidenceBands_Passed )
    {
        std::cerr << "/!\\ Test_SetConfidenceBands() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetConfidenceBands()" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetConfidenceBands() PASSED" << std::endl;
    }

    return testSetConfidenceBands_Passed;
}

bool TestPlot::Test_SetPostHocFDRLpvalue( QString rdPostHocFDRLpvalueFilePath, QString faPostHocFDRLpvalueFilePath,
                                          QString rdTransposePostHocFDRLpvalueFilePath, QString faTransposePostHocFDRLpvalueFilePath, QString dataDir )
{
    Plot plot;
    Processing processing;
    QList < QList < double > > rdTransposedData = plot.DataToDouble( processing.GetDataFromFile( rdTransposePostHocFDRLpvalueFilePath ) );
    QList < QList < double > > faTransposedData = plot.DataToDouble( processing.GetDataFromFile( faTransposePostHocFDRLpvalueFilePath ) );
    QMap< QString, QList < QList < double > > > expectedPostHocFDRLpvalueData;
    expectedPostHocFDRLpvalueData.insert( "RD", rdTransposedData );
    expectedPostHocFDRLpvalueData.insert( "FA", faTransposedData );


    plot.m_matlabDirectory = dataDir;
    plot.m_csvPostHocFDRLpvalueFiles = QStringList() << QFileInfo( QFile( rdPostHocFDRLpvalueFilePath ) ).fileName() << QFileInfo( QFile( faPostHocFDRLpvalueFilePath ) ).fileName();
    plot.SetPostHocFDRLpvalue();


    bool testSetPostHocFDRLpvalue_Passed = plot.m_dataPostHocFDRLpvalue == expectedPostHocFDRLpvalueData;
    if( !testSetPostHocFDRLpvalue_Passed )
    {
        std::cerr << "/!\\ Test_SetPostHocFDRLpvalue() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetPostHocFDRLpvalue()" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetPostHocFDRLpvalue() PASSED" << std::endl;
    }

    return testSetPostHocFDRLpvalue_Passed;
}




bool TestPlot::Test_GetRawDataFiles( QString dataDir )
{
    Plot plot;
    QStringList expectedRawDataFiles = QStringList() << "test_FA_RawData.csv" << "test_RD_RawData.csv" << "test_SUBMATRIX_RawData.csv" << "testSort_FA_RawData.csv" << "testSort_RD_RawData.csv" << "testSort_SUBMATRIX_RawData.csv";
    expectedRawDataFiles.sort();


    plot.m_directory = dataDir;
    plot.GetRawDataFiles();


    bool testGetRawDataFiles = plot.m_csvRawDataFiles == expectedRawDataFiles;
    if( !testGetRawDataFiles )
    {
        std::cerr << "/!\\ Test_GetRawDataFiles() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with GetRawDataFiles()" << std::endl;
        DisplayError_FileList( expectedRawDataFiles, plot.m_csvRawDataFiles );
    }
    else
    {
        std::cerr << "Test_GetRawDataFiles() PASSED" << std::endl;
    }

    return testGetRawDataFiles;
}

bool TestPlot::Test_GetBetaFiles( QString dataDir )
{
    Plot plot;
    QStringList expectedBetaFiles = QStringList() << "test_Betas_FA.csv" << "test_Betas_RD.csv";
    expectedBetaFiles.sort();


    plot.m_matlabDirectory = dataDir;
    plot.GetBetaFiles();


    bool testGetBetaFiles = plot.m_csvBetaFiles == expectedBetaFiles;
    if( !testGetBetaFiles )
    {
        std::cerr << "/!\\ Test_GetBetaFiles() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with GetBetaFiles()" << std::endl;
        DisplayError_FileList( expectedBetaFiles, plot.m_csvBetaFiles );
    }
    else
    {
        std::cerr << "Test_GetBetaFiles() PASSED" << std::endl;
    }

    return testGetBetaFiles;
}

bool TestPlot::Test_GetOmnibusLpvalueFiles( QString dataDir )
{
    Plot plot;
    QStringList expectedOmnibusLpvalueFiles = QStringList() << "test_Omnibus_Local_pvalues.csv";
    expectedOmnibusLpvalueFiles.sort();


    plot.m_matlabDirectory = dataDir;
    plot.GetOmnibusLpvalueFiles();


    bool testGetOmnibusLpvalueFiles = plot.m_csvOmnibusLpvalueFiles == expectedOmnibusLpvalueFiles;
    if( !testGetOmnibusLpvalueFiles )
    {
        std::cerr << "/!\\ Test_GetOmnibusLpvalueFiles() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with GetOmnibusLpvalueFiles()" << std::endl;
        DisplayError_FileList( expectedOmnibusLpvalueFiles, plot.m_csvOmnibusLpvalueFiles );
    }
    else
    {
        std::cerr << "Test_GetOmnibusLpvalueFiles() PASSED" << std::endl;
    }

    return testGetOmnibusLpvalueFiles;
}

bool TestPlot::Test_GetOmnibusFDRLpvalueFiles( QString dataDir )
{
    Plot plot;
    QStringList expectedOmnibusFDRLpvalueFiles = QStringList() << "test_Omnibus_FDR_Local_pvalues.csv";
    expectedOmnibusFDRLpvalueFiles.sort();


    plot.m_matlabDirectory = dataDir;
    plot.GetOmnibusFDRLpvalueFiles();


    bool testGetOmnibusFDRLpvalueFiles = plot.m_csvOmnibusFDRLpvalueFiles == expectedOmnibusFDRLpvalueFiles;
    if( !testGetOmnibusFDRLpvalueFiles )
    {
        std::cerr << "/!\\ Test_GetOmnibusFDRLpvalueFiles() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with GetOmnibusFDRLpvalueFiles()" << std::endl;
        DisplayError_FileList( expectedOmnibusFDRLpvalueFiles, plot.m_csvOmnibusFDRLpvalueFiles );
    }
    else
    {
        std::cerr << "Test_GetOmnibusFDRLpvalueFiles() PASSED" << std::endl;
    }

    return testGetOmnibusFDRLpvalueFiles;
}

bool TestPlot::Test_GetConfidenceBandsFiles( QString dataDir )
{
    Plot plot;
    QStringList expectedConfidenceBandsFiles = QStringList() << "test_Omnibus_ConfidenceBands_FA.csv" << "test_Omnibus_ConfidenceBands_RD.csv";
    expectedConfidenceBandsFiles.sort();


    plot.m_matlabDirectory = dataDir;
    plot.GetConfidenceBandsFiles();


    bool testGetConfidenceBandsFiles = plot.m_csvConfidenceBandsFiles == expectedConfidenceBandsFiles;
    if( !testGetConfidenceBandsFiles )
    {
        std::cerr << "/!\\ Test_GetConfidenceBandsFiles() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with GetConfidenceBandsFiles()" << std::endl;
        DisplayError_FileList( expectedConfidenceBandsFiles, plot.m_csvConfidenceBandsFiles );
    }
    else
    {
        std::cerr << "Test_GetConfidenceBandsFiles() PASSED" << std::endl;
    }

    return testGetConfidenceBandsFiles;
}

bool TestPlot::Test_GetPostHocFDRLpvalueFiles( QString dataDir )
{
    Plot plot;
    QStringList expectedGetPostHocFDRLpvalueFiles = QStringList() << "test_PostHoc_FDR_Local_pvalues_FA.csv" << "test_PostHoc_FDR_Local_pvalues_RD.csv";
    expectedGetPostHocFDRLpvalueFiles.sort();


    plot.m_matlabDirectory = dataDir;
    plot.GetPostHocFDRLpvalueFiles();


    bool testGetPostHocFDRLpvalueFiles = plot.m_csvPostHocFDRLpvalueFiles == expectedGetPostHocFDRLpvalueFiles;
    if( !testGetPostHocFDRLpvalueFiles )
    {
        std::cerr << "/!\\ Test_GetPostHocFDRLpvalueFiles() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with GetPostHocFDRLpvalueFiles()" << std::endl;
        DisplayError_FileList( expectedGetPostHocFDRLpvalueFiles, plot.m_csvPostHocFDRLpvalueFiles );
    }
    else
    {
        std::cerr << "Test_GetPostHocFDRLpvalueFiles() PASSED" << std::endl;
    }
    return testGetPostHocFDRLpvalueFiles;
}



bool TestPlot::Test_SetPlots( QString dataDir )
{
    Plot plot;
    QStringList expectedPlotsUsedRawData = QStringList() << "Raw Data" << "Raw Stats";
    QStringList expectedPlotsUsedBetas = QStringList() << "Raw Betas by Properties" << "Raw Betas by Covariates";
    QStringList expectedPlotsUsedOmnibusLpvalues = QStringList() << "Omnibus Local pvalues";
    QStringList expectedPlotsUsedOmnibusFDRLpvalues = QStringList() << "Omnibus FDR Local pvalues";
    QStringList expectedPlotsUsedOmnibusFDRSigBetas = QStringList() << "Raw Betas by Properties" << "Raw Betas by Covariates"
                                                                    << "Omnibus FDR Local pvalues"
                                                                    << "Omnibus FDR Significant Betas by Properties"
                                                                    << "Omnibus FDR Significant Betas by Covariates";
    QStringList expectedPlotsUsedConfidenceBands = QStringList() << "Raw Betas by Properties" << "Raw Betas by Covariates"
                                                                 << "Betas with Omnibus Confidence Bands";
    QStringList expectedPlotsUsedPostHocFDRLpvalue = QStringList() << "Post-Hoc FDR Local pvalues by Covariates";
    QStringList expectedPlotsUsedPostHocFDRSigBetas = QStringList() << "Raw Betas by Properties" << "Raw Betas by Covariates"
                                                                    << "Post-Hoc FDR Local pvalues by Covariates"
                                                                    << "Post-Hoc FDR Significant Betas by Properties"
                                                                    << "Post-Hoc FDR Significant Betas by Covariates";


    plot.m_directory = dataDir;
    plot.m_matlabDirectory = dataDir;

    plot.GetRawDataFiles();
    plot.SetPlots();
    QStringList displayedPlotsUsedRawData = plot.m_plotsUsed;
    bool testRawData = expectedPlotsUsedRawData == displayedPlotsUsedRawData;

    plot.ResetPlotData();
    plot.GetBetaFiles();
    plot.SetPlots();
    QStringList displayedPlotsUsedBetas = plot.m_plotsUsed;
    bool testRawBeta = expectedPlotsUsedBetas == displayedPlotsUsedBetas;

    plot.ResetPlotData();
    plot.GetOmnibusLpvalueFiles();
    plot.SetPlots();
    QStringList displayedPlotsUsedOmnibusLpvalues = plot.m_plotsUsed;
    bool testOmnibusLocalPvalues = expectedPlotsUsedOmnibusLpvalues == displayedPlotsUsedOmnibusLpvalues;

    plot.ResetPlotData();
    plot.GetOmnibusFDRLpvalueFiles();
    plot.SetPlots();
    QStringList displayedPlotsUsedOmnibusFDRLpvalues = plot.m_plotsUsed;
    bool testOmnibusFDRLocalPvalues = expectedPlotsUsedOmnibusFDRLpvalues == displayedPlotsUsedOmnibusFDRLpvalues;

    plot.ResetPlotData();
    plot.GetBetaFiles();
    plot.GetOmnibusFDRLpvalueFiles();
    plot.SetPlots();
    QStringList displayedPlotsUsedOmnibusFDRSigBetas = plot.m_plotsUsed;
    bool testOmnibusFDRSignificantBetas = expectedPlotsUsedOmnibusFDRSigBetas == displayedPlotsUsedOmnibusFDRSigBetas;

    plot.ResetPlotData();
    plot.GetBetaFiles();
    plot.GetConfidenceBandsFiles();
    plot.SetPlots();
    QStringList displayedPlotsUsedConfidenceBands = plot.m_plotsUsed;
    bool testConfidenceBands = expectedPlotsUsedConfidenceBands == displayedPlotsUsedConfidenceBands;

    plot.ResetPlotData();
    plot.GetPostHocFDRLpvalueFiles();
    plot.SetPlots();
    QStringList displayedPlotsUsedPostHocFDRLpvalue = plot.m_plotsUsed;
    bool testPostHocFDRLocalPvalues = expectedPlotsUsedPostHocFDRLpvalue == displayedPlotsUsedPostHocFDRLpvalue;

    plot.ResetPlotData();
    plot.GetBetaFiles();
    plot.GetPostHocFDRLpvalueFiles();
    plot.SetPlots();
    QStringList displayedPlotsUsedPostHocFDRSigBetas = plot.m_plotsUsed;
    bool testPostHocFDRSignificantBetas = expectedPlotsUsedPostHocFDRSigBetas == displayedPlotsUsedPostHocFDRSigBetas;


    bool testSetPlots = testRawData && testRawBeta && testOmnibusLocalPvalues && testOmnibusFDRLocalPvalues
            && testOmnibusFDRSignificantBetas && testConfidenceBands && testPostHocFDRLocalPvalues && testPostHocFDRSignificantBetas;
    if( !testSetPlots )
    {
        std::cerr << "/!\\ Test_SetPlots() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetPlots()" << std::endl;
        if( !testRawData )
        {
            std::cerr << "\t+ when getting Raw Data" << std::endl;
            DisplayError_PlotList( expectedPlotsUsedRawData, displayedPlotsUsedRawData );
        }
        if( !testRawBeta )
        {
            std::cerr << "\t+ when getting Betas" << std::endl;
            DisplayError_PlotList( expectedPlotsUsedBetas, displayedPlotsUsedBetas );
        }
        if( !testOmnibusLocalPvalues )
        {
            std::cerr << "\t+ when getting Omnibus Local Pvalues" << std::endl;
            DisplayError_PlotList( expectedPlotsUsedOmnibusLpvalues, displayedPlotsUsedOmnibusLpvalues );
        }
        if( !testOmnibusFDRLocalPvalues )
        {
            std::cerr << "\t+ when getting Omnibus FDR Local Pvalues" << std::endl;
            DisplayError_PlotList( expectedPlotsUsedOmnibusFDRLpvalues, displayedPlotsUsedOmnibusFDRLpvalues );
        }
        if( !testOmnibusFDRSignificantBetas )
        {
            std::cerr << "\t+ when getting Omnibus FDR Significant Betas" << std::endl;
            DisplayError_PlotList( expectedPlotsUsedOmnibusFDRSigBetas, displayedPlotsUsedOmnibusFDRSigBetas );
        }
        if( !testConfidenceBands )
        {
            std::cerr << "\t+ when getting Confidence Bands" << std::endl;
            DisplayError_PlotList( expectedPlotsUsedConfidenceBands, displayedPlotsUsedConfidenceBands );
        }
        if( !testPostHocFDRLocalPvalues )
        {
            std::cerr << "\t+ when getting PostHoc FDR Local Pvalues" << std::endl;
            DisplayError_PlotList( expectedPlotsUsedPostHocFDRLpvalue, displayedPlotsUsedPostHocFDRLpvalue );
        }
        if( !testPostHocFDRSignificantBetas )
        {
            std::cerr << "\t+ when getting PostHoc FDR Significant Betas" << std::endl;
            DisplayError_PlotList( expectedPlotsUsedPostHocFDRSigBetas, displayedPlotsUsedPostHocFDRSigBetas );
        }
    }
    else
    {
        std::cerr << "Test_SetPlots() PASSED" << std::endl;
    }
    return testSetPlots;
}

bool TestPlot::Test_SetProperties( QString dataDir )
{
    Plot plot;
    QMap<int, QString> expectedProperties;
    expectedProperties.insert( 1, "RD" );
    expectedProperties.insert( 3, "FA" );


    plot.m_directory = dataDir;
    plot.GetRawDataFiles();
    plot.SetProperties();


    bool testSetProperties = plot.m_properties == expectedProperties;
    if( !testSetProperties )
    {
        std::cerr << "/!\\ Test_SetProperties() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetProperties()" << std::endl;
        DisplayError_Properties( expectedProperties, plot.m_properties );
    }
    else
    {
        std::cerr << "Test_SetProperties() PASSED" << std::endl;
    }
    return testSetProperties;
}

bool TestPlot::Test_SetSubjects( QString dataDir )
{
    Plot plot;
    QStringList expectedSubjects = QStringList() << "F02-1_dwi_35_all_QCed_VC_DTI_embed" << "F05-1_dwi_35_all_QCed_VC_DTI_embed" << "F06-1_42_DWI_QCed_VC_DTI_embed"
                                                 << "F07-1_42_DWI_QCed_VC_DTI_embed" << "F09-1_42_DWI_QCed_VC_DTI_embed";


    plot.m_directory = dataDir;
    plot.GetRawDataFiles();
    plot.SetSubjects();


    bool testSetSubjects = plot.m_subjects == expectedSubjects;
    if( !testSetSubjects )
    {
        std::cerr << "/!\\ Test_SetSubjects() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetSubjects()" << std::endl;
        DisplayError_SubjectList( expectedSubjects, plot.m_subjects );
    }
    else
    {
        std::cerr << "Test_SetSubjects() PASSED" << std::endl;
    }
    return testSetSubjects;
}

bool TestPlot::Test_SetCovariates( QString dataDir )
{
    Plot plot;
    QMap<int, QString> expectedAllCovariates;
    expectedAllCovariates.insert( 0, "Intercept" );
    expectedAllCovariates.insert( 1, "GENDER" );
    expectedAllCovariates.insert( 2, "DaysPostBirth" );
    expectedAllCovariates.insert( 3, "ICV_y0" );
    QMap<int, QString> expectedCovariatesNoIntercept;
    expectedCovariatesNoIntercept.insert( 1, "GENDER" );
    expectedCovariatesNoIntercept.insert( 2, "DaysPostBirth" );
    expectedCovariatesNoIntercept.insert( 3, "ICV_y0" );
    QMap<int, QString> expectedBinaryCovariates;
    expectedBinaryCovariates.insert( 1, "GENDER" );


    plot.m_directory = dataDir;
    plot.GetRawDataFiles();
    plot.SetCovariates();

    bool testAllCovariates = plot.m_allCovariates == expectedAllCovariates;
    bool testCovariatesNoIntercept = plot.m_covariatesNoIntercept == expectedCovariatesNoIntercept;
    bool testBinaryCovariates = plot.m_binaryCovariates == expectedBinaryCovariates;


    bool testSetCovariates = testAllCovariates && testCovariatesNoIntercept && testBinaryCovariates;
    if( !testSetCovariates )
    {
        std::cerr << "/!\\ Test_SetCovariates() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetCovariates()" << std::endl;
        if( !testAllCovariates )
        {
            std::cerr << "\t+ pb with m_allCovariates" << std::endl;
            DisplayError_Covariates( expectedAllCovariates, plot.m_allCovariates );
        }
        if( !testCovariatesNoIntercept )
        {
            std::cerr << "\t+ pb with m_covariatesNoIntercept" << std::endl;
            DisplayError_Covariates( expectedCovariatesNoIntercept, plot.m_covariatesNoIntercept );
        }
        if( !testBinaryCovariates )
        {
            std::cerr << "\t+ pb with m_binaryCovariates" << std::endl;
            DisplayError_Covariates( expectedBinaryCovariates, plot.m_binaryCovariates );
        }
    }
    else
    {
        std::cerr << "Test_SetCovariates() PASSED" << std::endl;
    }
    return testSetCovariates;
}

bool TestPlot::Test_SetAbscissa( QString dataDir )
{
    Plot plot;
    QList< double > expectedAbscissa = QList< double >() << -39.1846 << -38.1846 << -37.1846 << -36.1846 << -35.1846;


    plot.m_directory = dataDir;
    plot.GetRawDataFiles();
    plot.SetAbscissa();


    bool testSetAbscissa = plot.m_abscissa == expectedAbscissa;
    if( !testSetAbscissa )
    {
        std::cerr << "/!\\ Test_SetAbscissa() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetAbscissa()" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetAbscissa() PASSED" << std::endl;
    }
    return testSetAbscissa;
}



bool TestPlot::Test_SetSelectedPlot( QString dataDir )
{
    Plot plot;


    plot.m_directory = dataDir;
    plot.GetRawDataFiles();
    plot.SetCovariates();

    plot.SetSelectedPlot( "Raw Data" );
    bool testBinaryCovariates = plot.m_isBinaryCovariatesSent && !plot.m_isAllCovariatesSent && !plot.m_isCovariatesNoInterceptSent;
    plot.m_isBinaryCovariatesSent = false;
    plot.SetSelectedPlot( "Raw Data" );
    testBinaryCovariates = testBinaryCovariates && plot.m_isBinaryCovariatesSent && !plot.m_isAllCovariatesSent && !plot.m_isCovariatesNoInterceptSent;

    plot.SetSelectedPlot( "Raw Betas by Properties" );
    bool testAllCovariates = !plot.m_isBinaryCovariatesSent && plot.m_isAllCovariatesSent && !plot.m_isCovariatesNoInterceptSent;
    plot.m_isAllCovariatesSent = false;
    plot.SetSelectedPlot( "Omnibus FDR Significant Betas by Properties" );
    testAllCovariates = testAllCovariates && !plot.m_isBinaryCovariatesSent && plot.m_isAllCovariatesSent && !plot.m_isCovariatesNoInterceptSent;
    plot.m_isAllCovariatesSent = false;
    plot.SetSelectedPlot( "Betas with Omnibus Confidence Bands" );
    testAllCovariates = testAllCovariates && !plot.m_isBinaryCovariatesSent && plot.m_isAllCovariatesSent && !plot.m_isCovariatesNoInterceptSent;
    plot.m_isAllCovariatesSent = false;
    plot.SetSelectedPlot( "Post-Hoc FDR Significant Betas by Properties" );
    testAllCovariates = testAllCovariates && !plot.m_isBinaryCovariatesSent && plot.m_isAllCovariatesSent && !plot.m_isCovariatesNoInterceptSent;

    plot.SetSelectedPlot( "Raw Betas by Covariates" );
    bool testCovariatesNoIntercept = !plot.m_isBinaryCovariatesSent && !plot.m_isAllCovariatesSent && plot.m_isCovariatesNoInterceptSent;
    plot.m_isCovariatesNoInterceptSent = false;
    plot.SetSelectedPlot( "Omnibus FDR Significant Betas by Covariates" );
    testCovariatesNoIntercept = testCovariatesNoIntercept && !plot.m_isBinaryCovariatesSent && !plot.m_isAllCovariatesSent && plot.m_isCovariatesNoInterceptSent;
    plot.m_isCovariatesNoInterceptSent = false;
    plot.SetSelectedPlot( "Post-Hoc FDR Local pvalues by Covariates" );
    testCovariatesNoIntercept = testCovariatesNoIntercept && !plot.m_isBinaryCovariatesSent && !plot.m_isAllCovariatesSent && plot.m_isCovariatesNoInterceptSent;
    plot.m_isCovariatesNoInterceptSent = false;
    plot.SetSelectedPlot( "Post-Hoc FDR Significant Betas by Covariates" );
    testCovariatesNoIntercept = testCovariatesNoIntercept && !plot.m_isBinaryCovariatesSent && !plot.m_isAllCovariatesSent && plot.m_isCovariatesNoInterceptSent;


    bool testSetSelectedPlot = testAllCovariates && testCovariatesNoIntercept && testBinaryCovariates;
    if( !testSetSelectedPlot )
    {
        std::cerr << "/!\\ Test_SetSelectedPlot() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetSelectedPlot( QString plotSelected )" << std::endl;
        if( !testAllCovariates )
        {
            std::cerr << "\t+ pb when sending m_allCovariates" << std::endl;
        }
        if( !testCovariatesNoIntercept )
        {
            std::cerr << "\t+ pb when sending m_covariatesNoIntercept" << std::endl;
        }
        if( !testBinaryCovariates )
        {
            std::cerr << "\t+ pb when sending m_binaryCovariates" << std::endl;
        }
    }
    else
    {
        std::cerr << "Test_SetSelectedPlot() PASSED" << std::endl;
    }
    return testSetSelectedPlot;
}



bool TestPlot::Test_SetTitle()
{
    Plot plot;
    QString title = "Test Title";
    bool bold = true;
    bool italic = false;
    double fontSize = 24;


    plot.m_chart = vtkSmartPointer<vtkChartXY>::New();
    plot.SetTitle( title, bold, italic, fontSize );

    bool testTitle = title == plot.m_chart->GetTitle();
    bool testBold = bold == plot.m_chart->GetTitleProperties()->GetBold();
    bool testItalic = italic == plot.m_chart->GetTitleProperties()->GetItalic();
    bool testFontSize = fontSize == plot.m_chart->GetTitleProperties()->GetFontSize();


    bool testSetTitle = testTitle && testBold && testItalic && testFontSize;
    if( !testSetTitle )
    {
        std::cerr << "/!\\ Test_SetTitle() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetTitle( QString title, bool isBold, bool isItalic, double fontSize )" << std::endl;
        if( !testTitle )
        {
            std::cerr << "\t   title expected: " << title.toStdString() << " | title displayed: " << plot.m_chart->GetTitle() << std::endl;
        }
        if( !testBold )
        {
            std::cerr << "\t   bold status expected: " << ( bold == 1 ? "TRUE" : "FALSE" ) << " | bold status displayed: " << ( plot.m_chart->GetTitleProperties()->GetBold() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        }
        if( !testItalic )
        {
            std::cerr << "\t   italic status expected: " << ( italic == 1 ? "TRUE" : "FALSE" ) << " | italic status displayed: " << ( plot.m_chart->GetTitleProperties()->GetItalic() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        }
        if( !testFontSize )
        {
            std::cerr << "\t   fontSize expected: " << fontSize << " | fontSize displayed: " << plot.m_chart->GetTitleProperties()->GetFontSize() << std::endl;
        }
    }
    else
    {
        std::cerr << "Test_SetTitle() PASSED" << std::endl;
    }
    return testSetTitle;
}

bool TestPlot::Test_SetDefaultTitle()
{
    Plot plot;


    plot.m_chart = vtkSmartPointer<vtkChartXY>::New();
    plot.m_fibername = "Test";
    plot.m_propertySelected = "RD";
    plot.m_covariateSelected = "GENDER";
    plot.m_pvalueThreshold = 0.05;

    plot.m_plotSelected = "Raw Data";
    plot.SetDefaultTitle();
    bool testTitle = plot.m_chart->GetTitle() == "Test Raw Data GENDER (RD)";
    plot.m_plotSelected = "Raw Stats";
    plot.SetDefaultTitle();
    testTitle = testTitle && plot.m_chart->GetTitle() == "Test Average and Standard Deviation GENDER (RD)";
    plot.m_plotSelected = "Raw Betas by Properties";
    plot.SetDefaultTitle();
    testTitle = testTitle && plot.m_chart->GetTitle() == "Test RD Beta Values";
    plot.m_plotSelected = "Raw Betas by Covariates";
    plot.SetDefaultTitle();
    testTitle = testTitle && plot.m_chart->GetTitle() == "Test GENDER Beta Values";
    plot.m_plotSelected = "Omnibus Local pvalues";
    plot.SetDefaultTitle();
    testTitle = testTitle && plot.m_chart->GetTitle() == "Test Omnibus Local pvalues";
    plot.m_plotSelected = "Omnibus FDR Significant Betas by Properties";
    plot.SetDefaultTitle();
    testTitle = testTitle && plot.m_chart->GetTitle() == "Test Omnibus FDR Significant Beta Values RD alpha=0.05";
    plot.m_plotSelected = "Omnibus FDR Significant Betas by Covariates";
    plot.SetDefaultTitle();
    testTitle = testTitle && plot.m_chart->GetTitle() == "Test Omnibus FDR Significant Beta Values GENDER alpha=0.05";
    plot.m_plotSelected = "Betas with Omnibus Confidence Bands";
    plot.SetDefaultTitle();
    testTitle = testTitle && plot.m_chart->GetTitle() == "Test Beta Values with Omnibus Confidence Bands GENDER (RD)";
    plot.m_plotSelected = "Post-Hoc FDR Local pvalues by Covariates";
    plot.SetDefaultTitle();
    testTitle = testTitle && plot.m_chart->GetTitle() == "Test Post-Hoc FDR Local pvalues GENDER";
    plot.m_plotSelected = "Post-Hoc FDR Significant Betas by Properties";
    plot.SetDefaultTitle();
    testTitle = testTitle && plot.m_chart->GetTitle() == "Test Post-Hoc FDR Significant Beta Values RD alpha=0.05";
    plot.m_plotSelected = "Post-Hoc FDR Significant Betas by Covariates";
    plot.SetDefaultTitle();
    testTitle = testTitle && plot.m_chart->GetTitle() == "Test Post-Hoc FDR Significant Beta Values GENDER alpha=0.05";

    bool testBold = plot.m_chart->GetTitleProperties()->GetBold() == false;
    bool testItalic = plot.m_chart->GetTitleProperties()->GetItalic() == false;
    bool testFontSize = plot.m_chart->GetTitleProperties()->GetFontSize() == 12.0;


    bool testSetDefaultTitle = testTitle && testBold && testItalic && testFontSize;
    if( !testSetDefaultTitle )
    {
        std::cerr << "/!\\ Test_SetDefaultTitle() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetDefaultTitle()" << std::endl;
        if( !testTitle )
        {
            std::cerr << "\t   wrong title displayed" << std::endl;
        }
        if( !testBold )
        {
            std::cerr << "\t   bold status expected: FALSE | bold status displayed: " << ( plot.m_chart->GetTitleProperties()->GetBold() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        }
        if( !testItalic )
        {
            std::cerr << "\t   italic status expected: FALSE | italic status displayed: " << ( plot.m_chart->GetTitleProperties()->GetItalic() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        }
        if( !testFontSize )
        {
            std::cerr << "\t   fontSize expected: 12.0 | fontSize displayed: " << plot.m_chart->GetTitleProperties()->GetFontSize() << std::endl;
        }
    }
    else
    {
        std::cerr << "Test_SetDefaultTitle() PASSED" << std::endl;
    }
    return testSetDefaultTitle;
}

bool TestPlot::Test_SetAxis()
{
    Plot plot;
    QString xName = "Title Abscissa";
    QString yName = "Title Ordinate";
    bool bold = true;
    bool italic = false;
    bool yMinSet = false;
    double yMin = -5;
    bool yMaxSet = true;
    double yMax = 5;


    plot.m_chart = vtkSmartPointer<vtkChartXY>::New();
    plot.SetAxis( xName, yName, bold, italic, yMinSet, yMin, yMaxSet, yMax );

    bool testXName = xName == plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitle();
    bool testYName = yName == plot.m_chart->GetAxis( vtkAxis::LEFT )->GetTitle();
    bool testBold = ( bold == plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->GetBold()
                      && bold == plot.m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->GetBold() );
    bool testItalic = ( italic == plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->GetItalic()
                        && italic == plot.m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->GetItalic() );
    bool testYMinSet = yMinSet == plot.m_yMin.first;
    bool testYMinValue = 0 == plot.m_yMin.second;
    bool testYMaxSet = yMaxSet == plot.m_yMax.first;
    bool testYMaxValue = yMax == plot.m_yMax.second;


    bool testSetAxis = testXName && testYName && testBold && testItalic && testYMinSet && testYMinValue && testYMaxSet && testYMaxValue;
    if( !testSetAxis )
    {
        std::cerr << "/!\\ Test_SetAxis() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetAxis( QString xName, QString yName, bool isBold, bool isItalic,"
                     "bool isYMinSet, double yMin, bool isYMaxSet, double yMax )" << std::endl;
        if( !testXName )
        {
            std::cerr << "\t   xName expected: " << xName.toStdString() << " | xName displayed: " << plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitle() << std::endl;
        }
        if( !testYName )
        {
            std::cerr << "\t   yName expected: " << yName.toStdString() << " | yName displayed: " << plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitle() << std::endl;
        }
        if( !testBold )
        {
            std::cerr << "\t   bold vtkAxis::BOTTOM status expected: " << ( bold == 1 ? "TRUE" : "FALSE" ) << " | bold status displayed: " << ( plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->GetBold() == 1 ? "TRUE" : "FALSE" ) << std::endl;
            std::cerr << "\t   bold vtkAxis::LEFT status expected: " << ( bold == 1 ? "TRUE" : "FALSE" ) << " | bold status displayed: " << ( plot.m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->GetBold() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        }
        if( !testItalic )
        {
            std::cerr << "\t   italic vtkAxis::BOTTOM status expected: " << ( italic == 1 ? "TRUE" : "FALSE" ) << " | italic status displayed: " << ( plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->GetItalic() == 1 ? "TRUE" : "FALSE" ) << std::endl;
            std::cerr << "\t   italic vtkAxis::LEFT status expected: " << ( italic == 1 ? "TRUE" : "FALSE" ) << " | italic status displayed: " << ( plot.m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->GetItalic() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        }
        if( !testYMinSet )
        {
            std::cerr << "\t   yMinSet status expected: " << ( yMinSet == 1 ? "TRUE" : "FALSE" ) << " | yMinSet displayed: " << ( plot.m_yMin.first == 1 ? "TRUE" : "FALSE" ) << std::endl;
        }

        if( !testYMinValue )
        {
            std::cerr << "\t   yMin expected: " << 0 << " | yMin displayed: " << plot.m_yMin.second << std::endl;
        }
        if( !testYMaxSet )
        {
            std::cerr << "\t   yMaxSet status expected: " << ( yMaxSet == 1 ? "TRUE" : "FALSE" ) << " | yMaxSet displayed: " << ( plot.m_yMax.first == 1 ? "TRUE" : "FALSE" ) << std::endl;
        }
        if( !testYMaxValue )
        {
            std::cerr << "\t   yMax expected: " << yMax << " | yMax displayed: " << plot.m_yMax.second << std::endl;
        }
    }
    else
    {
        std::cerr << "Test_SetAxis() PASSED" << std::endl;
    }
    return testSetAxis;
}

bool TestPlot::Test_SetDefaultAxis()
{
    Plot plot;


    plot.m_plotSelected = "Omnibus Local pvalues";
    plot.m_chart = vtkSmartPointer<vtkChartXY>::New();
    plot.SetDefaultAxis();

    bool testXName = plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitle() == "Arc Length";
    bool testYName = plot.m_chart->GetAxis( vtkAxis::LEFT )->GetTitle() == "-log10";
    bool testBold = ( plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->GetBold() == false
                      && plot.m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->GetBold() == false );
    bool testItalic = ( plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->GetItalic() == false
                        && plot.m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->GetItalic() == false );
    bool testYMinSet = plot.m_yMin.first == false;
    bool testYMinValue = plot.m_yMin.second == 0;
    bool testYMaxSet = plot.m_yMax.first == false;
    bool testYMaxValue = plot.m_yMax.second == 0;


    bool testSetDefaultAxis = testXName && testYName && testBold && testItalic && testYMinSet && testYMinValue && testYMaxSet && testYMaxValue;
    if( !testSetDefaultAxis )
    {
        std::cerr << "/!\\ Test_SetDefaultAxis() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetDefaultAxis()" << std::endl;
        if( !testXName )
        {
            std::cerr << "\t   xName expected: Arc Length | xName displayed: " << plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitle() << std::endl;
        }
        if( !testYName )
        {
            std::cerr << "\t   yName expected: -log10 | yName displayed: " << plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitle() << std::endl;
        }
        if( !testBold )
        {
            std::cerr << "\t   bold vtkAxis::BOTTOM status expected: FALSE | bold status displayed: " << ( plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->GetBold() == 1 ? "TRUE" : "FALSE" ) << std::endl;
            std::cerr << "\t   bold vtkAxis::LEFT status expected: FALSE | bold status displayed: " << ( plot.m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->GetBold() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        }
        if( !testItalic )
        {
            std::cerr << "\t   italic vtkAxis::BOTTOM status expected: FALSE | italic status displayed: " << ( plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->GetItalic() == 1 ? "TRUE" : "FALSE" ) << std::endl;
            std::cerr << "\t   italic vtkAxis::LEFT status expected: FALSE | italic status displayed: " << ( plot.m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->GetItalic() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        }
        if( !testYMinSet )
        {
            std::cerr << "\t   yMinSet status expected: FALSE | yMinSet displayed: " << ( plot.m_yMin.first == 1 ? "TRUE" : "FALSE" ) << std::endl;
        }
        if( !testYMinValue )
        {
            std::cerr << "\t   yMin expected: " << 0 << " | yMin displayed: " << plot.m_yMin.second << std::endl;
        }
        if( !testYMaxSet )
        {
            std::cerr << "\t   yMaxSet status expected: FALSE | yMaxSet displayed: " << ( plot.m_yMax.first == 1 ? "TRUE" : "FALSE" ) << std::endl;
        }
        if( !testYMaxValue )
        {
            std::cerr << "\t   yMax expected: " << 0 << " | yMax displayed: " << plot.m_yMax.second << std::endl;
        }
    }
    else
    {
        std::cerr << "Test_SetDefaultAxis() PASSED" << std::endl;
    }
    return testSetDefaultAxis;
}

bool TestPlot::Test_SetLegend()
{
    Plot plot;


    plot.m_chart = vtkSmartPointer<vtkChartXY>::New();

    plot.m_plotSelected = "Raw Data";
    plot.SetLegend( "Top Left" );
    bool testLegendAlignment1 = ( plot.m_chart->GetLegend()->GetVerticalAlignment() == vtkChartLegend::TOP
                                  && plot.m_chart->GetLegend()->GetHorizontalAlignment() == vtkChartLegend::LEFT );
    bool testLegendHidden1  = plot.m_chart->GetShowLegend() == false;

    plot.m_plotSelected = "Omnibus Local pvalues";
    plot.SetLegend( "Middle Center" );
    bool testLegendAlignment2 = ( plot.m_chart->GetLegend()->GetVerticalAlignment() == vtkChartLegend::CENTER
                                  && plot.m_chart->GetLegend()->GetHorizontalAlignment() == vtkChartLegend::CENTER );
    bool testLegendHidden2  = plot.m_chart->GetShowLegend() == true;

    plot.SetLegend( "Bottom Right" );
    bool testLegendAlignment3 = ( plot.m_chart->GetLegend()->GetVerticalAlignment() == vtkChartLegend::BOTTOM
                                  && plot.m_chart->GetLegend()->GetHorizontalAlignment() == vtkChartLegend::RIGHT );


    bool testSetLegend = testLegendAlignment1 && testLegendAlignment2 && testLegendAlignment3 && testLegendHidden1 && testLegendHidden2;
    if( !testSetLegend )
    {
        std::cerr << "/!\\ Test_SetLegend() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetLegend( QString position )" << std::endl;
        if( !testLegendAlignment1 )
        {
            std::cerr << "\t   when Top Left --> wrong legend alignment" << std::endl;
        }
        if( !testLegendAlignment2 )
        {
            std::cerr << "\t   when Middle Center --> wrong legend alignment" << std::endl;
        }
        if( !testLegendAlignment3 )
        {
            std::cerr << "\t   when Bottom Right --> wrong legend alignment" << std::endl;
        }
        if( !testLegendHidden1 )
        {
            std::cerr << "\t   when m_plotSelected == Raw Data --> legend displayed" << std::endl;
        }
        if( !testLegendHidden2 )
        {
            std::cerr << "\t   when m_plotSelected != Raw Data --> legend not displayed" << std::endl;
        }
    }
    else
    {
        std::cerr << "Test_SetLegend() PASSED" << std::endl;
    }
    return testSetLegend;
}



bool TestPlot::Test_SetMarkerType()
{
    Plot plot;


    plot.SetMarkerType( "Cross" );
    bool testMarkerType1 = plot.m_markerType == vtkPlotPoints::CROSS;

    plot.SetMarkerType( "Plus" );
    bool testMarkerType2 = plot.m_markerType == vtkPlotPoints::PLUS;

    plot.SetMarkerType( "Square" );
    bool testMarkerType3 = plot.m_markerType == vtkPlotPoints::SQUARE;

    plot.SetMarkerType( "Circle" );
    bool testMarkerType4 = plot.m_markerType == vtkPlotPoints::CIRCLE;

    plot.SetMarkerType( "Diamond" );
    bool testMarkerType5 = plot.m_markerType == vtkPlotPoints::DIAMOND;


    bool testSetMarkerType = testMarkerType1 && testMarkerType2 && testMarkerType3 && testMarkerType4 && testMarkerType5;
    if( !testSetMarkerType )
    {
        std::cerr << "/!\\ Test_SetMarkerType() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetMarkerType( QString markerType )" << std::endl;
        if( !testMarkerType1 )
        {
            std::cerr << "\t   vtkPlotPoints::CROSS not set" << std::endl;
        }
        if( !testMarkerType2 )
        {
            std::cerr << "\t   vtkPlotPoints::PLUS not set" << std::endl;
        }
        if( !testMarkerType3 )
        {
            std::cerr << "\t   vtkPlotPoints::SQUARE not set" << std::endl;
        }
        if( !testMarkerType4 )
        {
            std::cerr << "\t   vtkPlotPoints::CIRCLE not set" << std::endl;
        }
        if( !testMarkerType5 )
        {
            std::cerr << "\t   vtkPlotPoints::DIAMOND not set" << std::endl;
        }
    }
    else
    {
        std::cerr << "Test_SetMarkerType() PASSED" << std::endl;
    }
    return testSetMarkerType;
}



bool TestPlot::Test_UpdateCovariatesNames( QString dataDir )
{
    Plot plot;
    QMap<int, QString> expectedChangedAllCovariates;
    expectedChangedAllCovariates.insert( 0, "Intercept" );
    expectedChangedAllCovariates.insert( 1, "Sex" );
    expectedChangedAllCovariates.insert( 2, "DaysPostBirth" );
    expectedChangedAllCovariates.insert( 3, "ICV" );
    QMap<int, QString> expectedChangedCovariatesNoIntercept;
    expectedChangedCovariatesNoIntercept.insert( 1, "Sex" );
    expectedChangedCovariatesNoIntercept.insert( 2, "DaysPostBirth" );
    expectedChangedCovariatesNoIntercept.insert( 3, "ICV" );
    QMap<int, QString> expectedChangedBinaryCovariates;
    expectedChangedBinaryCovariates.insert( 1, "Sex" );


    plot.m_directory = dataDir;
    plot.GetRawDataFiles();
    plot.SetCovariates();
    plot.UpdateCovariatesNames( expectedChangedBinaryCovariates );
    plot.UpdateCovariatesNames( expectedChangedCovariatesNoIntercept );

    bool testChangeAllCovariates = plot.m_allCovariates == expectedChangedAllCovariates;
    bool testChangeCovariatesNoIntercept = plot.m_covariatesNoIntercept == expectedChangedCovariatesNoIntercept;
    bool testChangeBinaryCovariates = plot.m_binaryCovariates == expectedChangedBinaryCovariates;


    bool testUpdateCovariatesNames = testChangeAllCovariates && testChangeCovariatesNoIntercept && testChangeBinaryCovariates;
    if( !testUpdateCovariatesNames )
    {
        std::cerr << "/!\\ Test_UpdateCovariatesNames() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with UpdateCovariatesNames( QMap< int, QString > newCovariateName )" << std::endl;
        if( !testChangeAllCovariates )
        {
            std::cerr << "\t   m_allCovariates not correctly changed" << std::endl;
        }
        if( !testChangeCovariatesNoIntercept )
        {
            std::cerr << "\t   m_covariatesNoIntercept not correctly changed" << std::endl;
        }
        if( !testChangeBinaryCovariates )
        {
            std::cerr << "\t   m_binaryCovariates not correctly changed" << std::endl;
        }
    }
    else
    {
        std::cerr << "Test_UpdateCovariatesNames() PASSED" << std::endl;
    }
    return testUpdateCovariatesNames;
}



bool TestPlot::Test_SeparateBinary( QString faFilePath, QString dataDir )
{
    Plot plot;
    Processing processing;
    QList< QList < double > > expectedTemp0bin = QList< QList < double > >()
            << QList < double >( QList < double >() << 0.101391 << 0.089874 << 0.0899232 << 0.106037 << 0.130884 )
            << QList < double >( QList < double >() << 0.127887 << 0.120668 << 0.116923 << 0.12252 << 0.13898 );
    QList< QList < double > > expectedTemp1bin = QList< QList < double > >()
            << QList < double >( QList < double >() << 0.100461 << 0.121609 << 0.140557 << 0.154739 << 0.165439 )
            << QList < double >( QList < double >() << 0.135817 << 0.140971 << 0.145793 << 0.153154 << 0.163628 )
            << QList < double >( QList < double >() << 0.100568 << 0.109092 << 0.120203 << 0.13125 << 0.140868 );
    QList< QList < double > > temp0Bin;
    QList< QList < double > > temp1Bin;


    plot.m_directory = dataDir;
    plot.GetRawDataFiles();
    plot.SetCovariates();
    plot.m_propertySelected = "FA";
    plot.m_covariateSelected = "GENDER";
    plot.m_ordinate = plot.DataToDouble( processing.GetDataFromFile( faFilePath ) );
    plot.TransposeData( plot.m_ordinate, 1 ,0 );
    plot.m_ordinate.removeFirst();
    plot.SeparateBinary( temp0Bin, temp1Bin );

    bool testTemp0 = temp0Bin == expectedTemp0bin;
    bool testTemp1 = temp1Bin == expectedTemp1bin;


    bool testSeparateBinary_Passed = testTemp0 && testTemp1;
    if( !testSeparateBinary_Passed )
    {
        std::cerr << "/!\\ Test_SeparateBinary() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SeparateBinary( QList< QList < double > > &temp0Bin, QList< QList < double > > &temp1Bin )" << std::endl;
        if( !testTemp0 )
        {
            std::cerr << "\t+ 0 data not separated correctly" << std::endl;
        }
        if( !testTemp1 )
        {
            std::cerr << "\t+ 1 data not separated correctly" << std::endl;
        }
    }
    else
    {
        std::cerr << "Test_SeparateBinary() PASSED" << std::endl;
    }
    return testSeparateBinary_Passed;
}

bool TestPlot::Test_GetMeanAndStdDv()
{
    Plot plot;
    QList < double > expectedMean = QList < double >() << 2 << 2 << 3 << 8;
    QList < double > expectedStdDv = QList < double >() << 0 << 4 << 6 << 16;
    QList< QList < double > > temp0Bin = QList< QList < double > >()
            << ( QList < double >() << 2 << 8 << 0 << 0 )
            << ( QList < double >() << 2 << 0 << 0 << 32 )
            << ( QList < double >() << 2 << 0 << 12 << 0 )
            << ( QList < double >() << 2 << 0 << 0 << 0 );
    QList < double > tempMean;
    QList < double > tempStdDv;


    plot.m_nbrPoint = expectedMean.size();
    plot.GetMeanAndStdDv( temp0Bin, tempMean, tempStdDv );

    bool testMean = tempMean == expectedMean;
    bool testStdDv = tempStdDv == expectedStdDv;


    bool testGetMeanAndStdDv_Passed = testMean && testStdDv;
    if( !testGetMeanAndStdDv_Passed )
    {
        std::cerr << "/!\\ Test_GetMeanAndStdDv() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with GetMeanAndStdDv( QList< QList < double > > tempBin, QList < double > &tempMean,"
                     " QList < double > &tempStdDv )" << std::endl;
        if( !testMean )
        {
            std::cerr << "\t+ wrong mean generated" << std::endl;
        }
        if( !testStdDv )
        {
            std::cerr << "\t+ wrong standard deviation generated" << std::endl;
        }
    }
    else
    {
        std::cerr << "Test_GetMeanAndStdDv() PASSED" << std::endl;
    }
    return testGetMeanAndStdDv_Passed;
}

bool TestPlot::Test_ProcessRawStats()
{
    Plot plot;
    QList < double > expectedMean = QList < double >() << 2 << 2 << 3 << 8;
    QList < double > expectedUp = QList < double >() << 2 << 6 << 9 << 24;
    QList < double > expectedDown = QList < double >() << 2 << -2 << -3 << -8;
    QList < double > tempMean;
    QList < double > tempUp;
    QList < double > tempDown;

    QList< QList < double > > temp0Bin = QList< QList < double > >()
            << ( QList < double >() << 2 << 8 << 0 << 0 )
            << ( QList < double >() << 2 << 0 << 0 << 32 )
            << ( QList < double >() << 2 << 0 << 12 << 0 )
            << ( QList < double >() << 2 << 0 << 0 << 0 );


    plot.m_nbrPoint = expectedMean.size();
    plot.ProcessRawStats( temp0Bin, tempMean, tempUp, tempDown );

    bool testMean = tempMean == expectedMean;
    bool testUp = tempUp == expectedUp;
    bool testDown = tempDown == expectedDown;


    bool testProcessRawStats_Passed = testMean && testUp && testDown;
    if( !testProcessRawStats_Passed )
    {
        std::cerr << "/!\\ Test_ProcessRawStats() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with ProcessRawStats( QList< QList < double > > tempBin, QList < double > &tempBinMean,"
                     " QList < double > &tempBinUp, QList < double > &tempBinDown )" << std::endl;
        if( !testMean )
        {
            std::cerr << "\t+ wrong mean generated" << std::endl;
        }
        if( !testUp )
        {
            std::cerr << "\t+ wrong upper limit generated" << std::endl;
        }
        if( !testDown )
        {
            std::cerr << "\t+ wrong lower limit generated" << std::endl;
        }
    }
    else
    {
        std::cerr << "Test_ProcessRawStats() PASSED" << std::endl;
    }
    return testProcessRawStats_Passed;
}

bool TestPlot::Test_UpdateOrdinate( QString omnibusLpvalueFilePath )
{
    Plot plot;
    Processing processing;
    QMap< int, QPair< QString, QPair< bool, QString > > > selectionToDisplay;
    selectionToDisplay.insert( 0, QPair< QString, QPair< bool, QString > >( "Intercept", QPair< bool, QString >( true, "Red" ) ) );
    selectionToDisplay.insert( 1, QPair< QString, QPair< bool, QString > >( "GENDER", QPair< bool, QString >( false, "Lime" ) ) );
    selectionToDisplay.insert( 2, QPair< QString, QPair< bool, QString > >( "DaysPostBirth", QPair< bool, QString >( true, "Blue" ) ) );
    selectionToDisplay.insert( 3, QPair< QString, QPair< bool, QString > >( "ICV_y0", QPair< bool, QString >( false, "Yellow" ) ) );
    QList< QList < double > > expectedOrdinate = QList< QList < double > >()
            << ( QList < double >() << 0.42433 << 0.022155 << 0.0051664 << 0.011978 << 0.017748 << 0.020539 << 0.053934 << 0.19373 << 0.18608 << 0.22124
                 << 0.3546 << 0.49296 << 0.60709 << 0.67003 << 0.65688 << 0.53028 << 0.28052 << 0.066579 << 0.0066949 << 0.00154 << 0.003504 << 0.014889
                 << 0.046893 << 0.10867 << 0.15115 << 0.12687 << 0.098 << 0.064371 << 0.033962 << 0.01373 << 0.0038786 << 0.00080909 << 0.00016981
                 << 5.3551e-05 << 5.5402e-05 << 0.00043792 << 0.0058056 << 0.030523 << 0.081162 << 0.16288 << 0.26543 << 0.36584 << 0.46101 << 0.54396
                 << 0.57806 << 0.53984 << 0.47443 << 0.43421 << 0.41056 << 0.36538 << 0.27477 << 0.17113 << 0.11706 << 0.11631 << 0.14407 << 0.18715
                 << 0.24997 << 0.33176 << 0.41012 << 0.46284 << 0.48603 << 0.49578 << 0.53769 << 0.60997 << 0.62247 << 0.5635 << 0.43685 << 0.26143
                 << 0.18669 << 0.18562 << 0.20461 << 0.18628 << 0.12946 << 0.11408 << 0.14275 << 0.11017 << 0.044633 << 0.041554 << 0.22627);
    QStringList expectedLineNames = QStringList() << "DaysPostBirth";
    QList< QList< int > > expectedLineColors = QList< QList< int > >() << ( QList< int >() << 0 << 0 << 255 );


    plot.m_ordinate = plot.DataToDouble( processing.GetDataFromFile( omnibusLpvalueFilePath ) );
    plot.TransposeData( plot.m_ordinate, 0, 0 );
    plot.m_selectionToDisplay = selectionToDisplay;
    plot.m_selectionToDisplay.remove( plot.m_selectionToDisplay.firstKey() );
    plot.UpdateOrdinate( plot.m_ordinate );

    bool testOrdinate = expectedOrdinate == plot.m_ordinate;
    bool testLineNames = expectedLineNames == plot.m_lineNames;
    bool testLineColors = expectedLineColors == plot.m_lineColors;


    bool testUpdateOrdinate_Passed = testOrdinate && testLineNames && testLineColors;
    if( !testUpdateOrdinate_Passed )
    {
        std::cerr << "/!\\ Test_UpdateOrdinate() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with UpdateOrdinate( QList< QList < double > > &ordinate, int shift )" << std::endl;
        if( !testOrdinate )
        {
            std::cerr << "\t+ wrong data kept after updating m_ordinate with m_selectionToDisplay" << std::endl;
            DisplayError_QListQList( expectedOrdinate, plot.m_ordinate, "ordinate" );
        }
        if( !testLineNames )
        {
            std::cerr << "\t+ wrong data kept after updating m_ordinate with m_selectionToDisplay" << std::endl;
            DisplayError_LineNameList( expectedLineNames, plot.m_lineNames );
        }
        if( !testLineColors )
        {
            std::cerr << "\t+ wrong data kept after updating m_ordinate with m_selectionToDisplay" << std::endl;
            DisplayError_QListQList( expectedLineColors, plot.m_lineColors, "line colors" );
        }
    }
    else
    {
        std::cerr << "Test_UpdateOrdinate() PASSED" << std::endl;
    }
    return testUpdateOrdinate_Passed;
}

bool TestPlot::Test_Tolog10()
{
    Plot plot;
    QList < double > rowData1 = QList < double >() << 1 << 10 << 100 << 1000 << 10000;
    QList < double > rowData2 = QList < double >() << 10000 << 1000 << 100 << 10 << 1;
    QList< QList < double > > data = QList< QList < double > >() << rowData1 << rowData2 ;
    QList< QList < double > > dataTolog10 = plot.ToLog10( data );
    QList < double > expedtedRowDataTolog10_1 = QList < double >() << 0 << -1 << -2 << -3 << -4;
    QList < double > expedtedRowDataTolog10_2 = QList < double >() << -4 << -3 << -2 << -1 << 0;
    QList< QList < double > > expedtedDataTolog10 = QList< QList < double > >() << expedtedRowDataTolog10_1 << expedtedRowDataTolog10_2 ;


    bool testToLog10 = dataTolog10 == expedtedDataTolog10;
    if( !testToLog10 )
    {
        std::cerr << "/!\\ Test_Tolog10() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with ToLog10( QList< QList < double > > data )" << std::endl;
    }
    else
    {
        std::cerr << "Test_Tolog10() PASSED" << std::endl;
    }
    return testToLog10;
}



bool TestPlot::Test_LoadRawData( QString dataDir )
{
    Plot plot;
    QList< QList < double > > expectedOrdinate = QList< QList < double > >()
            << ( QList < double >() << 0.101391 << 0.089874 << 0.0899232 << 0.106037 << 0.130884 )
            << ( QList < double >() << 0.100461 << 0.121609 << 0.140557 << 0.154739 << 0.165439 )
            << ( QList < double >() << 0.127887 << 0.120668 << 0.116923 << 0.12252 << 0.13898 )
            << ( QList < double >() << 0.135817 << 0.140971 << 0.145793 << 0.153154 << 0.163628 )
            << ( QList < double >() << 0.100568 << 0.109092 << 0.120203 << 0.13125 << 0.140868 );


    plot.m_directory = dataDir;
    plot.GetRawDataFiles();
    plot.m_propertySelected = "FA";
    plot.m_ordinate = plot.LoadRawData();


    bool testLoadRawData = plot.m_ordinate == expectedOrdinate;
    if( !testLoadRawData )
    {
        std::cerr << "/!\\ Test_LoadRawData() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with LoadRawData()" << std::endl;
        DisplayError_QListQList( expectedOrdinate, plot.m_ordinate, "raw data loaded" );
    }
    else
    {
        std::cerr << "Test_LoadRawData() PASSED" << std::endl;
    }
    return testLoadRawData;
}

bool TestPlot::Test_LoadRawStats( QString dataDir )
{
    Plot plot;
    QList< QList < double > > expectedOrdinate;
    QList< QList < double > > temp0Bin;
    QList< QList < double > > temp1Bin;
    QList < double > temp0BinUp;
    QList < double > temp0BinMean;
    QList < double > temp0BinDown;
    QList < double > temp1BinUp;
    QList < double > temp1BinMean;
    QList < double > temp1BinDown;


    plot.m_directory = dataDir;
    plot.GetRawDataFiles();
    plot.SetCovariates();
    plot.SetAbscissa();
    plot.m_propertySelected = "FA";
    plot.m_covariateSelected = "GENDER";
    plot.m_ordinate = plot.LoadRawData();

    expectedOrdinate = plot.m_ordinate;
    plot.SeparateBinary( temp0Bin, temp1Bin );
    plot.ProcessRawStats( temp0Bin, temp0BinMean, temp0BinUp, temp0BinDown );
    plot.ProcessRawStats( temp1Bin, temp1BinMean, temp1BinUp, temp1BinDown );
    expectedOrdinate.clear();
    expectedOrdinate.append( temp0BinUp );
    expectedOrdinate.append( temp0BinMean );
    expectedOrdinate.append( temp0BinDown );
    expectedOrdinate.append( temp1BinUp );
    expectedOrdinate.append( temp1BinMean );
    expectedOrdinate.append( temp1BinDown);

    plot.m_ordinate = plot.LoadRawStats();


    bool testLoadRawStats = plot.m_ordinate == expectedOrdinate;
    if( !testLoadRawStats )
    {
        std::cerr << "/!\\ Test_LoadRawStats() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with LoadRawStats()" << std::endl;
        DisplayError_QListQList( expectedOrdinate, plot.m_ordinate, "raw stats loaded" );
    }
    else
    {
        std::cerr << "Test_LoadRawStats() PASSED" << std::endl;
    }
    return testLoadRawStats;
}

bool TestPlot::Test_LoadBetas( QString dataDir )
{
    Plot plot;
    QMap< int, QPair< QString, QPair< bool, QString > > > selectionToDisplay;
    selectionToDisplay.insert( 0, QPair< QString, QPair< bool, QString > >( "Intercept", QPair< bool, QString >( true, "Red" ) ) );
    selectionToDisplay.insert( 1, QPair< QString, QPair< bool, QString > >( "GENDER", QPair< bool, QString >( false, "Lime" ) ) );
    selectionToDisplay.insert( 2, QPair< QString, QPair< bool, QString > >( "DaysPostBirth", QPair< bool, QString >( true, "Blue" ) ) );
    selectionToDisplay.insert( 3, QPair< QString, QPair< bool, QString > >( "ICV_y0", QPair< bool, QString >( false, "Yellow" ) ) );
    QList< QList < double > > expectedOrdinate = QList< QList < double > >()
            << ( QList < double >() << 0.05691 << 0.060803 << 0.06531 << 0.070412 << 0.076043 << 0.082126 << 0.088623 << 0.095532 << 0.10283 << 0.11034
                 << 0.11774 << 0.12448 << 0.13002 << 0.13382 << 0.13549 << 0.13481 << 0.13175 << 0.1265 << 0.11944 << 0.11116
                 << 0.10241 << 0.094048 << 0.086846 << 0.081379 << 0.077899 << 0.076351 << 0.076481 << 0.07796 << 0.080441 << 0.083556
                 << 0.08693 << 0.09029 << 0.09366 << 0.097488 << 0.10259 << 0.10981 << 0.11963 << 0.1317 << 0.14488 << 0.15758
                 << 0.16838 << 0.1766 << 0.18248 << 0.18686 << 0.19069 << 0.19462 << 0.19879 << 0.20295 << 0.20667 << 0.20951
                 << 0.21109 << 0.21111 << 0.20949 << 0.20635 << 0.2021 << 0.19723 << 0.19216 << 0.18702 << 0.18159 << 0.17546
                 << 0.16828 << 0.16009 << 0.15129 << 0.14252 << 0.1343 << 0.12683 << 0.12 << 0.11352 << 0.10714 << 0.10076
                 << 0.094387 << 0.088147 << 0.082266 << 0.077046 << 0.072797 << 0.06971 << 0.067772 << 0.066745 << 0.06624 )
            << ( QList < double >() << 0.0020391 << 0.004916 << 0.0066412 << 0.0072202 << 0.0068241 << 0.0057568 << 0.0043697 << 0.0029498 << 0.0016436 << 0.00047481
                 << -0.00056549 << -0.0014505 << -0.0021154 << -0.002479 << -0.0024539 << -0.0019674 << -0.0010076 << 0.00034182 << 0.0019112 << 0.0034908
                 << 0.0048857 << 0.0059693 << 0.0067164 << 0.0072016 << 0.0075721 << 0.0080139 << 0.0087013 << 0.0097065 << 0.010886 << 0.011829
                 << 0.011975 << 0.010896 << 0.0085982 << 0.0055898 << 0.0026212 << 0.00024992 << -0.0014404 << -0.0027416 << -0.0039521 << -0.0050873
                 << -0.0058968 << -0.006083 << -0.0054818 << -0.0041217 << -0.0022256 << -0.00016911 << 0.0016491 << 0.0030008 << 0.0039446 << 0.0047553
                 << 0.0057167 << 0.0069428 << 0.0083482 << 0.0097426 << 0.010931 << 0.011754 << 0.01208 << 0.011822 << 0.010968 << 0.0096046
                 << 0.0079188 << 0.006182 << 0.0047188 << 0.0038556 << 0.0038345 << 0.0047113 << 0.0062915 << 0.0081613 << 0.0098252 << 0.010898
                 << 0.011251 << 0.01104 << 0.010609 << 0.010337 << 0.010522 << 0.011324 << 0.012793 << 0.014898 << 0.017575 );


    plot.m_matlabDirectory = dataDir;
    plot.GetBetaFiles();
    plot.m_propertySelected = "FA";
    plot.m_selectionToDisplay = selectionToDisplay;
    plot.m_ordinate = plot.LoadBetas();


    bool testLoadBetas = plot.m_ordinate == expectedOrdinate;
    if( !testLoadBetas )
    {
        std::cerr << "/!\\ Test_LoadBetas() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with LoadBetas()" << std::endl;
        DisplayError_QListQList( expectedOrdinate, plot.m_ordinate, "betas loaded" );
    }
    else
    {
        std::cerr << "Test_LoadBetas() PASSED" << std::endl;
    }
    return testLoadBetas;
}

bool TestPlot::Test_LoadBetaByCovariate( QString dataDir )
{
    Plot plot;
    QMap< int, QPair< QString, QPair< bool, QString > > > selectionToDisplay;
    selectionToDisplay.insert( 1, QPair< QString, QPair< bool, QString > >( "RD", QPair< bool, QString >( true, "Lime" ) ) );
    selectionToDisplay.insert( 3, QPair< QString, QPair< bool, QString > >( "FA", QPair< bool, QString >( false, "Yellow" ) ) );
    QList< QList < double > > expectedOrdinate;


    plot.m_matlabDirectory = dataDir;
    plot.GetBetaFiles();
    plot.m_properties.insert( 1, "RD" );
    plot.m_properties.insert( 3, "FA" );
    plot.m_allCovariates.insert( 0, "Intercept" );
    plot.m_allCovariates.insert( 1, "GENDER" );
    plot.m_allCovariates.insert( 2, "DaysPostBirth" );
    plot.m_allCovariates.insert( 3, "ICV_y0" );
    plot.m_covariateSelected = "DaysPostBirth";

    expectedOrdinate.append( plot.m_dataBeta.value( "RD" ).at( plot.m_allCovariates.key( plot.m_covariateSelected ) ) );

    plot.m_selectionToDisplay = selectionToDisplay;
    plot.m_ordinate = plot.LoadBetaByCovariate();


    bool testLoadBetaByCovariate = plot.m_ordinate == expectedOrdinate;
    if( !testLoadBetaByCovariate )
    {
        std::cerr << "/!\\ Test_LoadBetaByCovariate() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with LoadBetaByCovariate()" << std::endl;
        DisplayError_QListQList( expectedOrdinate, plot.m_ordinate, "beta by covariates loaded" );
    }
    else
    {
        std::cerr << "Test_LoadBetaByCovariate() PASSED" << std::endl;
    }
    return testLoadBetaByCovariate;
}

bool TestPlot::Test_LoadOmnibusLpvalues( QString dataDir )
{
    Plot plot;
    QMap< int, QPair< QString, QPair< bool, QString > > > selectionToDisplay;
    selectionToDisplay.insert( 0, QPair< QString, QPair< bool, QString > >( "Intercept", QPair< bool, QString >( true, "Red" ) ) );
    selectionToDisplay.insert( 1, QPair< QString, QPair< bool, QString > >( "GENDER", QPair< bool, QString >( false, "Lime" ) ) );
    selectionToDisplay.insert( 2, QPair< QString, QPair< bool, QString > >( "DaysPostBirth", QPair< bool, QString >( true, "Blue" ) ) );
    selectionToDisplay.insert( 3, QPair< QString, QPair< bool, QString > >( "ICV_y0", QPair< bool, QString >( false, "Yellow" ) ) );
    QList< QList < double > > expectedOrdinate;


    plot.m_matlabDirectory = dataDir;
    plot.GetOmnibusLpvalueFiles();
    plot.m_selectionToDisplay = selectionToDisplay;
    plot.m_ordinate = plot.LoadOmnibusLpvalues( plot.m_dataOmnibusLpvalue );
    expectedOrdinate = plot.m_dataOmnibusLpvalue;
    expectedOrdinate.removeAt( 0 );
    expectedOrdinate.removeAt( 1 );
    expectedOrdinate.removeAt( 3 );
    expectedOrdinate = plot.ToLog10( expectedOrdinate );


    bool testLoadOmnibusLpvalues = plot.m_ordinate == expectedOrdinate;
    if( !testLoadOmnibusLpvalues )
    {
        std::cerr << "/!\\ Test_LoadOmnibusLpvalues() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with LoadOmnibusLpvalues( QList< QList < double > > omnibusLpvalues )" << std::endl;
        DisplayError_QListQList( expectedOrdinate, plot.m_ordinate, "omnibus Lpvalues loaded" );
    }
    else
    {
        std::cerr << "Test_LoadOmnibusLpvalues() PASSED" << std::endl;
    }
    return testLoadOmnibusLpvalues;
}

bool TestPlot::Test_LoadConfidenceBand( QString dataDir )
{
    Plot plot;
    QList< QList < double > > expectedOrdinate = QList< QList < double > >()
            << ( QList < double >() << 0.031504 << 0.034381 << 0.036106 << 0.036685 << 0.036289 << 0.035222 << 0.033835 << 0.032415 << 0.031109 << 0.02994
                 << 0.0289 << 0.028015 << 0.02735 << 0.026986 << 0.027011 << 0.027498 << 0.028458 << 0.029807 << 0.031376 << 0.032956
                 << 0.034351 << 0.035434 << 0.036182 << 0.036667 << 0.037037 << 0.037479 << 0.038166 << 0.039172 << 0.040351 << 0.041295
                 << 0.04144 << 0.040361 << 0.038063 << 0.035055 << 0.032086 << 0.029715 << 0.028025 << 0.026724 << 0.025513 << 0.024378
                 << 0.023568 << 0.023382 << 0.023983 << 0.025343 << 0.02724 << 0.029296 << 0.031114 << 0.032466 << 0.03341 << 0.034221
                 << 0.035182 << 0.036408 << 0.037813 << 0.039208 << 0.040397 << 0.041219 << 0.041545 << 0.041288 << 0.040434 << 0.03907
                 << 0.037384 << 0.035647 << 0.034184 << 0.033321 << 0.0333 << 0.034176 << 0.035757 << 0.037626 << 0.03929 << 0.040363
                 << 0.040716 << 0.040505 << 0.040074 << 0.039803 << 0.039987 << 0.04079 << 0.042258 << 0.044363 << 0.04704 )
            << ( QList < double >() << 0.0020391 << 0.004916 << 0.0066412 << 0.0072202 << 0.0068241 << 0.0057568 << 0.0043697 << 0.0029498 << 0.0016436 << 0.00047481
                 << -0.00056549 << -0.0014505 << -0.0021154 << -0.002479 << -0.0024539 << -0.0019674 << -0.0010076 << 0.00034182 << 0.0019112 << 0.0034908
                 << 0.0048857 << 0.0059693 << 0.0067164 << 0.0072016 << 0.0075721 << 0.0080139 << 0.0087013 << 0.0097065 << 0.010886 << 0.011829
                 << 0.011975 << 0.010896 << 0.0085982 << 0.0055898 << 0.0026212 << 0.00024992 << -0.0014404 << -0.0027416 << -0.0039521 << -0.0050873
                 << -0.0058968 << -0.006083 << -0.0054818 << -0.0041217 << -0.0022256 << -0.00016911 << 0.0016491 << 0.0030008 << 0.0039446 << 0.0047553
                 << 0.0057167 << 0.0069428 << 0.0083482 << 0.0097426 << 0.010931 << 0.011754 << 0.01208 << 0.011822 << 0.010968 << 0.0096046 << 0.0079188
                 << 0.006182 << 0.0047188 << 0.0038556 << 0.0038345 << 0.0047113 << 0.0062915 << 0.0081613 << 0.0098252 << 0.010898 << 0.011251 << 0.01104
                 << 0.010609 << 0.010337 << 0.010522 << 0.011324 << 0.012793 << 0.014898 << 0.017575 )
            << ( QList < double >() << -0.027426 << -0.024549 << -0.022824 << -0.022245 << -0.022641 << -0.023708 << -0.025096 << -0.026515 << -0.027822 << -0.02899
                 << -0.030031 << -0.030916 << -0.031581 << -0.031944 << -0.031919 << -0.031433 << -0.030473 << -0.029123 << -0.027554 << -0.025974
                 << -0.02458 << -0.023496 << -0.022749 << -0.022264 << -0.021893 << -0.021451 << -0.020764 << -0.019759 << -0.018579 << -0.017636
                 << -0.01749 << -0.018569 << -0.020867 << -0.023875 << -0.026844 << -0.029215 << -0.030906 << -0.032207 << -0.033417 << -0.034553
                 << -0.035362 << -0.035548 << -0.034947 << -0.033587 << -0.031691 << -0.029634 << -0.027816 << -0.026464 << -0.025521 << -0.02471
                 << -0.023748 << -0.022522 << -0.021117 << -0.019723 << -0.018534 << -0.017711 << -0.017385 << -0.017643 << -0.018497 << -0.019861
                 << -0.021546 << -0.023283 << -0.024746 << -0.02561 << -0.025631 << -0.024754 << -0.023174 << -0.021304 << -0.01964 << -0.018567
                 << -0.018214 << -0.018425 << -0.018856 << -0.019128 << -0.018944 << -0.018141 << -0.016673 << -0.014567 << -0.01189 );


    plot.m_matlabDirectory = dataDir;
    plot.GetBetaFiles();
    plot.GetConfidenceBandsFiles();
    plot.m_allCovariates.insert( 0, "Intercept" );
    plot.m_allCovariates.insert( 1, "GENDER" );
    plot.m_allCovariates.insert( 2, "DaysPostBirth" );
    plot.m_allCovariates.insert( 3, "ICV_y0" );
    plot.m_propertySelected = "FA";
    plot.m_covariateSelected = "DaysPostBirth";
    plot.m_ordinate = plot.LoadConfidenceBand();


    bool testLoadConfidenceBand = plot.m_ordinate == expectedOrdinate;
    if( !testLoadConfidenceBand )
    {
        std::cerr << "/!\\ Test_LoadConfidenceBand() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with LoadConfidenceBand()" << std::endl;
        DisplayError_QListQList( expectedOrdinate, plot.m_ordinate, "confidence bands loaded" );
    }
    else
    {
        std::cerr << "LoadConfidenceBand() PASSED" << std::endl;
    }
    return testLoadConfidenceBand;
}

bool TestPlot::Test_LoadPostHocFDRLpvalues( QString dataDir )
{
    Plot plot;
    QMap< int, QPair< QString, QPair< bool, QString > > > selectionToDisplay;
    selectionToDisplay.insert( 1, QPair< QString, QPair< bool, QString > >( "RD", QPair< bool, QString >( false, "Lime" ) ) );
    selectionToDisplay.insert( 3, QPair< QString, QPair< bool, QString > >( "FA", QPair< bool, QString >( true, "Yellow" ) ) );
    QList< QList < double > > expectedOrdinate;


    plot.m_matlabDirectory = dataDir;
    plot.GetPostHocFDRLpvalueFiles();
    plot.m_properties.insert( 1, "RD" );
    plot.m_properties.insert( 3, "FA" );
    plot.m_covariatesNoIntercept.insert( 1, "GENDER" );
    plot.m_covariatesNoIntercept.insert( 2, "DaysPostBirth" );
    plot.m_covariatesNoIntercept.insert( 3, "ICV_y0" );
    plot.m_covariateSelected = "DaysPostBirth";

//    expectedOrdinate.append( plot.m_dataPostHocFDRLpvalue.value( "RD" ).at( plot.m_covariatesNoIntercept.key( plot.m_covariateSelected ) - 1 ) );
    expectedOrdinate.append( plot.m_dataPostHocFDRLpvalue.value( "FA" ).at( plot.m_covariatesNoIntercept.key( plot.m_covariateSelected ) - 1 ) );
    expectedOrdinate = plot.ToLog10( expectedOrdinate );

    plot.m_selectionToDisplay = selectionToDisplay;
    plot.m_ordinate = plot.LoadPostHocFDRLpvalues();


    bool testLoadPostHocFDRLpvalues = plot.m_ordinate == expectedOrdinate;
    if( !testLoadPostHocFDRLpvalues )
    {
        std::cerr << "/!\\ Test_LoadPostHocFDRLpvalues() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with LoadPostHocFDRLpvalues()" << std::endl;
        DisplayError_QListQList( expectedOrdinate, plot.m_ordinate, "post-hoc FDR Lpvalues loaded" );
    }
    else
    {
        std::cerr << "Test_LoadPostHocFDRLpvalues() PASSED" << std::endl;
    }
    return testLoadPostHocFDRLpvalues;
}



bool TestPlot::Test_AddEntriesRawData()
{
    Plot plot;
    bool testAddEntriesRawData = true;
    vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();
    QStringList tableEntries;


    plot.m_subjects = QStringList() << "Stan" << "Kyle" << "Kenny" << "Cartman" << "Butters";
    plot.m_nbrPlot = plot.m_subjects.size();
    plot.AddEntriesRawData( table );

    for( int i = 0; i < table->GetNumberOfColumns(); i++ )
    {
        testAddEntriesRawData = testAddEntriesRawData && ( table->GetColumn( i )->GetName() == plot.m_subjects.at( i ) );
        tableEntries.append( table->GetColumn( i )->GetName() );
    }


    if( !testAddEntriesRawData )
    {
        std::cerr << "/!\\ Test_AddEntriesRawData() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with AddEntriesRawData( vtkSmartPointer<vtkTable> &table )" << std::endl;
        DisplayError_EntryList( plot.m_subjects, tableEntries );
    }
    else
    {
        std::cerr << "Test_AddEntriesRawData() PASSED" << std::endl;
    }
    return testAddEntriesRawData;
}

bool TestPlot::Test_AddEntriesRawStats()
{
    Plot plot;
    bool testAddEntriesRawStats = true;
    vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();
    QStringList expectedEntries = QStringList() << "GENDER = 0 std+" << "GENDER = 0 mean" << "GENDER = 0 std-" << "GENDER = 1 std+" << "GENDER = 1 mean" << "GENDER = 1 std-";
    QStringList tableEntries;


    plot.m_covariateSelected = "GENDER";
    plot.AddEntriesRawStats( table );

    for( int i = 0; i < table->GetNumberOfColumns(); i++ )
    {
        testAddEntriesRawStats = testAddEntriesRawStats && ( table->GetColumn( i )->GetName() == expectedEntries.at( i ) );
        tableEntries.append( table->GetColumn( i )->GetName() );
    }


    if( !testAddEntriesRawStats )
    {
        std::cerr << "/!\\ Test_AddEntriesRawStats() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with AddEntriesRawStats( vtkSmartPointer<vtkTable> &table )" << std::endl;
        DisplayError_EntryList( expectedEntries, tableEntries );
    }
    else
    {
        std::cerr << "Test_AddEntriesRawStats() PASSED" << std::endl;
    }
    return testAddEntriesRawStats;
}

bool TestPlot::Test_AddEntriesByPropertiesOrCovariates()
{
    Plot plot;
    bool testAddEntriesByPropertiesOrCovariates = true;
    vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();
    QStringList tableEntries;


    plot.m_lineNames = QStringList() << "Intercept" << "GENDER" << "DaysPostBirth" << "ICV_y0";
    plot.m_nbrPlot = plot.m_lineNames.size();
    plot.AddEntriesByPropertiesOrCovariates( table );

    for( int i = 0; i < table->GetNumberOfColumns(); i++ )
    {
        testAddEntriesByPropertiesOrCovariates = testAddEntriesByPropertiesOrCovariates && ( table->GetColumn( i )->GetName() == plot.m_lineNames.at( i ) );
        tableEntries.append( table->GetColumn( i )->GetName() );
    }


    if( !testAddEntriesByPropertiesOrCovariates )
    {
        std::cerr << "/!\\ Test_AddEntriesByPropertiesOrCovariates() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with AddEntriesByPropertiesOrCovariates( vtkSmartPointer<vtkTable> &table )" << std::endl;
        DisplayError_EntryList( plot.m_lineNames, tableEntries );
    }
    else
    {
        std::cerr << "Test_AddEntriesByPropertiesOrCovariates() PASSED" << std::endl;
    }
    return testAddEntriesByPropertiesOrCovariates;
}

bool TestPlot::Test_AddEntriesCovariatesBands()
{
    Plot plot;
    bool testAddEntriesCovariatesBands = true;
    vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();
    QStringList expectedEntries = QStringList() << "Upper Confidence Band" << "Betas" << "Lower Confidence Band";
    QStringList tableEntries;


    plot.AddEntriesCovariatesBands( table );

    for( int i = 0; i < table->GetNumberOfColumns(); i++ )
    {
        testAddEntriesCovariatesBands = testAddEntriesCovariatesBands && ( table->GetColumn( i )->GetName() == expectedEntries.at( i ) );
        tableEntries.append( table->GetColumn( i )->GetName() );
    }


    if( !testAddEntriesCovariatesBands )
    {
        std::cerr << "/!\\ Test_AddEntriesCovariatesBands() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with AddEntriesCovariatesBands( vtkSmartPointer<vtkTable> &table )" << std::endl;
        DisplayError_EntryList( expectedEntries, tableEntries );
    }
    else
    {
        std::cerr << "Test_AddEntriesCovariatesBands() PASSED" << std::endl;
    }
    return testAddEntriesCovariatesBands;
}



bool TestPlot::Test_SetData()
{
    Plot plot;
    bool testSetData = true;
    vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();
    QList< QList< double > > dataTable;


    plot.m_subjects = QStringList() << "Stan" << "Kyle" << "Kenny";
    plot.m_abscissa = QList< double >() << -3 << -2 << -1;
    plot.m_ordinate = QList< QList< double > >()
            << ( QList< double >() << 1 << 2 << 3 )
            << ( QList< double >() << 4 << 5 << 6 )
            << ( QList< double >() << 7 << 8 << 9 );
    QList< QList< double > > expectedTable = QList< QList< double > >() << plot.m_abscissa << plot.m_ordinate;
    plot.m_nbrPoint = plot.m_abscissa.size();
    plot.m_nbrPlot = plot.m_ordinate.size();
    plot.m_plotSelected = "Raw Data";
    plot.AddEntries( table );
    plot.SetData( table );

    for( int i = 0; i < table->GetNumberOfColumns(); i++ )
    {
        QList< double > currentDataRow;
        for( int j = 0; j < table->GetNumberOfRows(); j++ )
        {
            currentDataRow.append( table->GetValue( j, i ).ToDouble() );
            if( i == 0 )
            {
                testSetData = testSetData && ( table->GetValue( j, i ).ToDouble() == plot.m_abscissa.at( j ) );
            }
            else
            {
                testSetData = testSetData && ( table->GetValue( j, i ).ToDouble() == plot.m_ordinate.at( i - 1 ).at( j ) );
            }
        }
        dataTable.append( currentDataRow );
    }


    if( !testSetData )
    {
        std::cerr << "/!\\ Test_SetData() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetData( vtkSmartPointer<vtkTable> &table )" << std::endl;
        DisplayError_QListQList( expectedTable, dataTable, "table" );
    }
    else
    {
        std::cerr << "Test_SetData() PASSED" << std::endl;
    }
    return testSetData;
}



bool TestPlot::Test_InitLines()
{
    Plot plot;
    int nbrPlot = 5;

    plot.m_nbrPlot = nbrPlot;
    plot.m_chart = vtkSmartPointer<vtkChartXY>::New();
    plot.InitLines();


    bool testInitLines = plot.m_chart->GetNumberOfPlots() == plot.m_nbrPlot;
    if( !testInitLines )
    {
        std::cerr << "/!\\ Test_InitLines() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with InitLines()" << std::endl;
        std::cerr << "\t+ initialization failed: " << plot.m_nbrPlot << " plots expected | " << plot.m_chart->GetNumberOfPlots() << " plots set" << std::endl;
    }
    else
    {
        std::cerr << "Test_InitLines() PASSED" << std::endl;
    }
    return testInitLines;
}

bool TestPlot::Test_AddSignificantLevel()
{
    Plot plot;
    double significantLevel = 5.5;
    double lineWidth = 20;
    int indexLastPlot;
    QList< QList< double > > dataTable;


    plot.m_abscissa = QList< double >() << -3 << -2 << -1;
    QList< QList< double > > expectedTable = QList< QList< double > >() << plot.m_abscissa << ( QList< double >() << significantLevel << significantLevel << significantLevel );
    plot.m_nbrPoint = plot.m_abscissa.size();
    plot.m_lineWidth = lineWidth;
    plot.m_chart = vtkSmartPointer<vtkChartXY>::New();
    plot.AddSignificantLevel( significantLevel );
    indexLastPlot = plot.m_chart->GetNumberOfPlots() - 1;

    for( int i = 0; i < plot.m_chart->GetPlot( indexLastPlot )->GetInput()->GetNumberOfColumns(); i++ )
    {
        QList< double > currentDataRow;
        for( int j = 0; j < plot.m_chart->GetPlot( indexLastPlot )->GetInput()->GetNumberOfRows(); j++ )
        {
            currentDataRow.append( plot.m_chart->GetPlot( indexLastPlot )->GetInput()->GetValue( j, i ).ToDouble() );
        }
        dataTable.append( currentDataRow );
    }

    bool testNbrPlots = plot.m_chart->GetNumberOfPlots() == 1;
    bool testSignificantLevel = dataTable.last().last() == expectedTable.last().last();
    bool testLineWidth = plot.m_chart->GetPlot( indexLastPlot )->GetWidth() == lineWidth / 2;


    bool testAddSignificantLevel = testNbrPlots && testSignificantLevel && testLineWidth;
    if( !testAddSignificantLevel )
    {
        std::cerr << "/!\\ Test_AddSignificantLevel() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with AddSignificantLevel( double significantLevel )" << std::endl;
        if( !testNbrPlots )
        {
            std::cerr << "\t+ significant level not added to m_chart" << std::endl;
        }
        if( !testSignificantLevel )
        {
            std::cerr << "\t+ wrong significant level added to m_chart" << std::endl;
            std::cerr << "\t   significant level expected: " << significantLevel << " | significant level set: " << dataTable.last().last() << std::endl;
        }
        if( !testLineWidth )
        {
            std::cerr << "\t+ wrong line width" << std::endl;
            std::cerr << "\t   line width expected: " << lineWidth / 2 << " | line width set: " << plot.m_chart->GetPlot( indexLastPlot )->GetWidth() << std::endl;
        }
    }
    else
    {
        std::cerr << "Test_AddSignificantLevel() PASSED" << std::endl;
    }
    return testAddSignificantLevel;
}

bool TestPlot::Test_AddLineSigBetas()
{
    Plot plot;





    bool testAddLineSigBetas = false;
    if( !testAddLineSigBetas )
    {
        std::cerr << "/!\\ Test_AddLineSigBetas() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with AddLineSigBetas( vtkSmartPointer<vtkTable> table, bool betaDisplayedByProperties, bool isOmnibus, int i )" << std::endl;
    }
    else
    {
        std::cerr << "Test_AddLineSigBetas() PASSED" << std::endl;
    }
    return testAddLineSigBetas;
}










bool TestPlot::Test_GetyMinMax()
{
    Plot plot;
    QList< QList< double > > ordinate = QList< QList< double > >()
            << ( QList< double >() << -10 << -9 << -8 << -7 << 6 )
            << ( QList< double >() << -5 << -4 << -3 << -2 << -1 )
            << ( QList< double >() << 0 << 1 << 2 << 3 << 4 )
            << ( QList< double >() << 5 << 6 << 7 << 8 << 9 )
            << ( QList< double >() << 10 << 11 << 12 << 13 << 14 );
    QList< double > expectedYMinMax = QList< double >() << -10 << 5.5;


    plot.m_yMin.first = false;
    plot.m_yMin.second = -5.5;
    plot.m_yMax.first = true;
    plot.m_yMax.second = 5.5;
    plot.m_ordinate = ordinate;
    QList< double > yMinMax = plot.GetyMinMax();


    bool testGetyMinMax = yMinMax == expectedYMinMax;
    if( !testGetyMinMax )
    {
        std::cerr << "/!\\ Test_GetyMinMax() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with GetyMinMax()" << std::endl;
        std::cerr << "\t+ yMinMax expected: {" << yMinMax.first() << "," << yMinMax.at( 1 ) << "} | "
                  << "yMinMax set: {" << expectedYMinMax.first() << "," << expectedYMinMax.at( 1 ) << "}" << std::endl;
    }
    else
    {
        std::cerr << "Test_GetyMinMax() PASSED" << std::endl;
    }
    return testGetyMinMax;
}

bool TestPlot::Test_SetyMinMax()
{
    Plot plot;



    bool testSetyMinMax = false;
    if( !testSetyMinMax )
    {
        std::cerr << "/!\\ Test_SetyMinMax() FAILED /!\\" << std::endl;
        std::cerr << "\t+ pb with SetyMinMax()" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetyMinMax() PASSED" << std::endl;
    }
    return testSetyMinMax;
}


















/**********************************************************************/
/********************** Functions Used For Testing ********************/
/**********************************************************************/
void TestPlot::DisplayError_FileList( QStringList fileListExpected, QStringList fileListDisplayed )
{
    DisplayError_QStringList( fileListExpected, fileListDisplayed, "files" );
}

void TestPlot::DisplayError_SubjectList( QStringList subjectListExpected, QStringList subjectListDisplayed )
{
    DisplayError_QStringList( subjectListExpected, subjectListDisplayed, "subjects" );
}

void TestPlot::DisplayError_PlotList( QStringList plotListExpected, QStringList plotListDisplayed )
{
    DisplayError_QStringList( plotListExpected, plotListDisplayed, "plots" );
}

void TestPlot::DisplayError_LineNameList( QStringList lineNameListExpected, QStringList lineNameListDisplayed )
{
    DisplayError_QStringList( lineNameListExpected, lineNameListDisplayed, "line names" );
}

void TestPlot::DisplayError_EntryList( QStringList entryListExpected, QStringList entryListDisplayed )
{
    DisplayError_QStringList( entryListExpected, entryListDisplayed, "entries" );
}

void TestPlot::DisplayError_QStringList( QStringList qStringListExpected, QStringList qStringListDisplayed, QString type )
{
    std::cerr << "\t   - " << type.toStdString() << " expected" << std::endl;
    for( int i = 0; i < qStringListExpected.size(); i++ )
    {
        std::cerr << "\t\t  \"" << qStringListExpected.at( i ).toStdString() << "\"" << std::endl;
    }
    std::cerr << "\t   - " << type.toStdString() << " displayed" << std::endl;
    for( int i = 0; i < qStringListDisplayed.size(); i++ )
    {
        std::cerr << "\t\t  \"" << qStringListDisplayed.at( i ).toStdString() << "\"" << std::endl;
    }
}


void TestPlot::DisplayError_Properties( QMap<int, QString> propertiesExpected, QMap<int, QString> propertiesDisplayed )
{
    DisplayError_QMapIntQString( propertiesExpected, propertiesDisplayed, "properties" );
}

void TestPlot::DisplayError_Covariates( QMap<int, QString> covariatesExpected, QMap<int, QString> covariatesDisplayed )
{
    DisplayError_QMapIntQString( covariatesExpected, covariatesDisplayed, "covariates" );
}

void TestPlot::DisplayError_QMapIntQString( QMap<int, QString> qMapExpected, QMap<int, QString> qMapDisplayed, QString type )
{
    std::cerr << "\t   - " << type.toStdString() << " expected" << std::endl;
    QMap<int, QString>::ConstIterator iterQMapExpected = qMapExpected.cbegin();
    while( iterQMapExpected != qMapExpected.cend() )
    {
        std::cerr << "\t\t  \"" << iterQMapExpected.value().toStdString() << "\" at " << iterQMapExpected.key() << std::endl;
        ++iterQMapExpected;
    }
    std::cerr << "\t   - " << type.toStdString() << " displayed" << std::endl;
    QMap<int, QString>::ConstIterator iterQMapDisplayed = qMapDisplayed.cbegin();
    while( iterQMapDisplayed != qMapDisplayed.cend() )
    {
        std::cerr << "\t\t  \"" << iterQMapDisplayed.value().toStdString() << "\" at " << iterQMapDisplayed.key() << std::endl;
        ++iterQMapDisplayed;
    }
}


template <typename T>
void TestPlot::DisplayError_QListQList( T qListQListExpected, T qListQListDisplayed, QString type )
{
    std::cerr << "\t   - " << type.toStdString() << " expected" << std::endl;
    for( int i = 0; i < qListQListExpected.size(); i++ )
    {
        std::cerr << "\t\t  ";
        for( int j = 0; j < qListQListExpected.first().size(); j++ )
        {
            std::cerr << qListQListExpected.at( i ).at( j ) << ", ";
        }
        std::cerr << std::endl;
    }
    std::cerr << "\t   - " << type.toStdString() << " displayed" << std::endl;
    for( int i = 0; i < qListQListDisplayed.size(); i++ )
    {
        std::cerr << "\t\t  ";
        for( int j = 0; j < qListQListDisplayed.first().size(); j++ )
        {
            std::cerr << qListQListDisplayed.at( i ).at( j ) << ", ";
        }
        std::cerr << std::endl;
    }
}
