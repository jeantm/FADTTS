#include "TestPlot.h"

#include <QDebug>

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
        std::cerr << "/!\\/!\\ Test_SetQVTKWidget() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetQVTKWidget( QSharedPointer< QVTKWidget > qvtkWidget )" << std::endl;
        //        std::cerr << "\t  m_qvtkWidget not initialized correctly" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetQVTKWidget() PASSED";
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
        std::cerr << "/!\\/!\\ Test_QStringListToDouble() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with QStringListToDouble( QStringList rowData )" << std::endl;
        //        std::cerr << "\t  convertion from QStringList to QList < double > failed" << std::endl;
    }
    else
    {
        std::cerr << "Test_QStringListToDouble() PASSED";
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
        std::cerr << "/!\\/!\\ Test_DataToDouble() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with DataToDouble( QList < QStringList > data )" << std::endl;
        //        std::cerr << "\t  convertion from QList < QStringList > to QList < QList < double > > failed" << std::endl;
    }
    else
    {
        std::cerr << "Test_DataToDouble() PASSED";
    }

    return testDataToDouble_Passed;
}

bool TestPlot::Test_SortFilesByProperties( QString rdRawData, QString faRawData, QString subMatrixRawData )
{
    Plot plot;
    QStringList wrongFiles  = QStringList() << "./path/wrongFile1.csv" << "./path/wrongFile2.csv";
    QStringList files = QStringList() << faRawData << rdRawData;
    QList< double > rdDataRow0 = QList< double >() << QString( "Arc_length vs Data" ).toDouble() << QString( "F02-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble()
                                                   << QString( "F05-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble() << QString( "F06-1_42_DWI_QCed_VC_DTI_embed" ).toDouble()
                                                   << QString( "F07-1_42_DWI_QCed_VC_DTI_embed" ).toDouble() << QString( "F09-1_42_DWI_QCed_VC_DTI_embed" ).toDouble();
    QList< double > rdDataRow1 = QList< double >() << -39.1846 << 0.00117111 << 0.00118478 << 0.00131709 << 0.00121005 << 0.00141595;
    QList< double > rdDataRow2 = QList< double >() << -38.1846 << 0.00118018 << 0.00117143 << 0.0013476 << 0.00124804 << 0.00137842;
    QList< double > rdDataRow3 = QList< double >() << -37.1846 << 0.00119539 << 0.00116328 << 0.00137232 << 0.00127349 << 0.00135757;
    QList< double > rdDataRow4 = QList< double >() << -36.1846 << 0.00120302 << 0.00116113 << 0.00138538 << 0.0012901 << 0.00133806;
    QList< double > rdDataRow5 = QList< double >() << -35.1846 << 0.00119349 << 0.00115982 << 0.00138459 << 0.00130296 << 0.00131466;
    QList< QList< double > > rdData = QList< QList< double > > () << rdDataRow0 << rdDataRow1 << rdDataRow2 << rdDataRow3 << rdDataRow4 << rdDataRow5;
    QList< double > faDataRow0 = QList< double >() << QString( "Arc_length vs Data" ).toDouble() << QString( "F02-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble()
                                                   << QString( "F05-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble() << QString( "F06-1_42_DWI_QCed_VC_DTI_embed" ).toDouble()
                                                   << QString( "F07-1_42_DWI_QCed_VC_DTI_embed" ).toDouble() << QString( "F09-1_42_DWI_QCed_VC_DTI_embed" ).toDouble();
    QList< double > faDataRow1 = QList< double >() << -39.1846 << 0.101391 << 0.100461 << 0.127887 << 0.135817 << 0.100568;
    QList< double > faDataRow2 = QList< double >() << -38.1846 << 0.089874 << 0.121609 << 0.120668 << 0.140971 << 0.109092;
    QList< double > faDataRow3 = QList< double >() << -37.1846 << 0.0899232 << 0.140557 << 0.116923 << 0.145793 << 0.120203;
    QList< double > faDataRow4 = QList< double >() << -36.1846 << 0.106037 << 0.154739 << 0.12252 << 0.153154 << 0.13125;
    QList< double > faDataRow5 = QList< double >() << -35.1846 << 0.130884 << 0.165439 << 0.13898 << 0.163628 << 0.140868;
    QList< QList< double > > faData = QList< QList< double > > () << faDataRow0 << faDataRow1 << faDataRow2 << faDataRow3 << faDataRow4 << faDataRow5;
    QStringList subMatrixDataRow0 = QStringList() << "Case_orig_Shaili" << "GENDER" << "DaysPostBirth" << "ICV_y0";
    QStringList subMatrixDataRow1 = QStringList() << "F02-1_dwi_35_all_QCed_VC_DTI_embed" << "0" << "16" << "494341.5";
    QStringList subMatrixDataRow2 = QStringList() << "F05-1_dwi_35_all_QCed_VC_DTI_embed" << "1" << "54" << "483068";
    QStringList subMatrixDataRow3 = QStringList() << "F06-1_42_DWI_QCed_VC_DTI_embed" << "0" << "31" << "471322.4";
    QStringList subMatrixDataRow4 = QStringList() << "F07-1_42_DWI_QCed_VC_DTI_embed" << "1" << "18" << "432712.4";
    QStringList subMatrixDataRow5 = QStringList() << "F09-1_42_DWI_QCed_VC_DTI_embed" << "1" << "38" << "449280.2";
    QList< QStringList > expectedDataSubMatrix = QList< QStringList >() << subMatrixDataRow0 << subMatrixDataRow1 << subMatrixDataRow2 << subMatrixDataRow3 << subMatrixDataRow4 << subMatrixDataRow5;
    QMap< QString, QList< QList< double > > > expectedData;
    expectedData.insert( "FA", faData );
    expectedData.insert( "RD", rdData );
    QMap< QString, QList< QList< double > > > data;


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
        std::cerr << "/!\\/!\\ Test_SortFilesByProperties() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SortFilesByProperties( QString directory, QStringList files,"
        //                     " QMap< QString, QList< QList< double > > > &data )" << std::endl;
        //        if( !testWrongFiles )
        //        {
        //            std::cerr << "\t  no data should be sorted and extracted" << std::endl;
        //        }
        //        if( !testFilesNoSubMatrix )
        //        {
        //            std::cerr << "\t  data not sorted and extracted (without subMatrix file)" << std::endl;
        //        }
        //        if( !testFilesSubMatrix )
        //        {
        //            std::cerr << "\t  data not sorted and extracted (with subMatrix file)" << std::endl;
        //        }
        //        if( !testSubMatrix )
        //        {
        //            std::cerr << "\t  subMatrix data not extracted" << std::endl;
        //        }
    }
    else
    {
        std::cerr << "Test_SortFilesByProperties() PASSED";
    }

    return testSortFilesByProperties_Passed;
}

bool TestPlot::Test_TransposeData()
{
    Plot plot;
    QList< double > rdDataRow0 = QList< double >() << QString( "Arc_length vs Data" ).toDouble() << QString( "F02-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble()
                                                   << QString( "F05-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble() << QString( "F06-1_42_DWI_QCed_VC_DTI_embed" ).toDouble()
                                                   << QString( "F07-1_42_DWI_QCed_VC_DTI_embed" ).toDouble() << QString( "F09-1_42_DWI_QCed_VC_DTI_embed" ).toDouble();
    QList< double > rdDataRow1 = QList< double >() << -39.1846 << 0.00117111 << 0.00118478 << 0.00131709 << 0.00121005 << 0.00141595;
    QList< double > rdDataRow2 = QList< double >() << -38.1846 << 0.00118018 << 0.00117143 << 0.0013476 << 0.00124804 << 0.00137842;
    QList< double > rdDataRow3 = QList< double >() << -37.1846 << 0.00119539 << 0.00116328 << 0.00137232 << 0.00127349 << 0.00135757;
    QList< double > rdDataRow4 = QList< double >() << -36.1846 << 0.00120302 << 0.00116113 << 0.00138538 << 0.0012901 << 0.00133806;
    QList< double > rdDataRow5 = QList< double >() << -35.1846 << 0.00119349 << 0.00115982 << 0.00138459 << 0.00130296 << 0.00131466;
    QList< QList< double > > rdData = QList< QList< double > > () << rdDataRow0 << rdDataRow1 << rdDataRow2 << rdDataRow3 << rdDataRow4 << rdDataRow5;
    QList< double > rdDataTranspRow0 = QList< double >() << -39.1846 << -38.1846 << -37.1846 << -36.1846 << -35.1846;
    QList< double > rdDataTranspRow1 = QList< double >() << 0.00117111 << 0.00118018 << 0.00119539 << 0.00120302 << 0.00119349;
    QList< double > rdDataTranspRow2 = QList< double >() << 0.00118478 << 0.00117143 << 0.00116328 << 0.00116113 << 0.00115982;
    QList< double > rdDataTranspRow3 = QList< double >() << 0.00131709 << 0.0013476 << 0.00137232 << 0.00138538 << 0.00138459;
    QList< double > rdDataTranspRow4 = QList< double >() << 0.00121005 << 0.00124804 << 0.00127349 << 0.0012901 << 0.00130296;
    QList< double > rdDataTranspRow5 = QList< double >() << 0.00141595 << 0.00137842 << 0.00135757 << 0.00133806 << 0.00131466;
    QList< QList< double > > expectedRDDataTransp = QList< QList< double > > () << rdDataTranspRow0 << rdDataTranspRow1 << rdDataTranspRow2 << rdDataTranspRow3 << rdDataTranspRow4 << rdDataTranspRow5;


    plot.TransposeData( rdData, 1, 0 );


    bool testTransposeData_Passed = rdData == expectedRDDataTransp;
    if( !testTransposeData_Passed )
    {
        std::cerr << "/!\\/!\\ Test_TransposeData() FAILED /!\\/!\\";
        std::cerr << "\t+ pb with TransposeData( QList < QList < double > > &data, int firstRow, int firstColumn )" << std::endl;
    }
    else
    {
        std::cerr << "Test_TransposeData() PASSED";
    }

    return testTransposeData_Passed;
}

bool TestPlot::Test_TransposeDataInQMap()
{
    Plot plot;
    QList< double > rdDataRow0 = QList< double >() << QString( "Arc_length vs Data" ).toDouble() << QString( "F02-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble()
                                                   << QString( "F05-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble() << QString( "F06-1_42_DWI_QCed_VC_DTI_embed" ).toDouble()
                                                   << QString( "F07-1_42_DWI_QCed_VC_DTI_embed" ).toDouble() << QString( "F09-1_42_DWI_QCed_VC_DTI_embed" ).toDouble();
    QList< double > rdDataRow1 = QList< double >() << -39.1846 << 0.00117111 << 0.00118478 << 0.00131709 << 0.00121005 << 0.00141595;
    QList< double > rdDataRow2 = QList< double >() << -38.1846 << 0.00118018 << 0.00117143 << 0.0013476 << 0.00124804 << 0.00137842;
    QList< double > rdDataRow3 = QList< double >() << -37.1846 << 0.00119539 << 0.00116328 << 0.00137232 << 0.00127349 << 0.00135757;
    QList< double > rdDataRow4 = QList< double >() << -36.1846 << 0.00120302 << 0.00116113 << 0.00138538 << 0.0012901 << 0.00133806;
    QList< double > rdDataRow5 = QList< double >() << -35.1846 << 0.00119349 << 0.00115982 << 0.00138459 << 0.00130296 << 0.00131466;
    QList< QList< double > > rdData = QList< QList< double > > () << rdDataRow0 << rdDataRow1 << rdDataRow2 << rdDataRow3 << rdDataRow4 << rdDataRow5;
    QList< double > rdDataTranspRow0 = QList< double >() << -39.1846 << -38.1846 << -37.1846 << -36.1846 << -35.1846;
    QList< double > rdDataTranspRow1 = QList< double >() << 0.00117111 << 0.00118018 << 0.00119539 << 0.00120302 << 0.00119349;
    QList< double > rdDataTranspRow2 = QList< double >() << 0.00118478 << 0.00117143 << 0.00116328 << 0.00116113 << 0.00115982;
    QList< double > rdDataTranspRow3 = QList< double >() << 0.00131709 << 0.0013476 << 0.00137232 << 0.00138538 << 0.00138459;
    QList< double > rdDataTranspRow4 = QList< double >() << 0.00121005 << 0.00124804 << 0.00127349 << 0.0012901 << 0.00130296;
    QList< double > rdDataTranspRow5 = QList< double >() << 0.00141595 << 0.00137842 << 0.00135757 << 0.00133806 << 0.00131466;
    QList< QList< double > > expectedRDDataTransp = QList< QList< double > > () << rdDataTranspRow0 << rdDataTranspRow1 << rdDataTranspRow2 << rdDataTranspRow3 << rdDataTranspRow4 << rdDataTranspRow5;
    QList< double > faDataRow0 = QList< double >() << QString( "Arc_length vs Data" ).toDouble() << QString( "F02-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble()
                                                   << QString( "F05-1_dwi_35_all_QCed_VC_DTI_embed" ).toDouble() << QString( "F06-1_42_DWI_QCed_VC_DTI_embed" ).toDouble()
                                                   << QString( "F07-1_42_DWI_QCed_VC_DTI_embed" ).toDouble() << QString( "F09-1_42_DWI_QCed_VC_DTI_embed" ).toDouble();
    QList< double > faDataRow1 = QList< double >() << -39.1846 << 0.101391 << 0.100461 << 0.127887 << 0.135817 << 0.100568;
    QList< double > faDataRow2 = QList< double >() << -38.1846 << 0.089874 << 0.121609 << 0.120668 << 0.140971 << 0.109092;
    QList< double > faDataRow3 = QList< double >() << -37.1846 << 0.0899232 << 0.140557 << 0.116923 << 0.145793 << 0.120203;
    QList< double > faDataRow4 = QList< double >() << -36.1846 << 0.106037 << 0.154739 << 0.12252 << 0.153154 << 0.13125;
    QList< double > faDataRow5 = QList< double >() << -35.1846 << 0.130884 << 0.165439 << 0.13898 << 0.163628 << 0.140868;
    QList< QList< double > > faData = QList< QList< double > > () << faDataRow0 << faDataRow1 << faDataRow2 << faDataRow3 << faDataRow4 << faDataRow5;
    QList< double > faDataTranspRow0 = QList< double >() << -39.1846 << -38.1846 << -37.1846 << -36.1846 << -35.1846;
    QList< double > faDataTranspRow1 = QList< double >() << 0.101391 << 0.089874 << 0.0899232 << 0.106037 << 0.130884;
    QList< double > faDataTranspRow2 = QList< double >() << 0.100461 << 0.121609 << 0.140557 << 0.154739 << 0.165439;
    QList< double > faDataTranspRow3 = QList< double >() << 0.127887 << 0.120668 << 0.116923 << 0.12252 << 0.13898;
    QList< double > faDataTranspRow4 = QList< double >() << 0.135817 << 0.140971 << 0.145793 << 0.153154 << 0.163628;
    QList< double > faDataTranspRow5 = QList< double >() << 0.100568 << 0.109092 << 0.120203 << 0.13125 << 0.140868;
    QList< QList< double > > expectedFADataTransp = QList< QList< double > > () << faDataTranspRow0 << faDataTranspRow1 << faDataTranspRow2 << faDataTranspRow3 << faDataTranspRow4 << faDataTranspRow5;
    QMap< QString, QList< QList< double > > > data;
    data.insert( "FA", faData );
    data.insert( "RD", rdData );
    QMap< QString, QList< QList< double > > > expectedTranspDataMap;
    expectedTranspDataMap.insert( "RD", expectedRDDataTransp );
    expectedTranspDataMap.insert( "FA", expectedFADataTransp );


    plot.TransposeDataInQMap( data, 1, 0 );


    bool testTransposeDataInQMap_Passed = data == expectedTranspDataMap;
    if( !testTransposeDataInQMap_Passed )
    {
        std::cerr << "/!\\/!\\ Test_TransposeDataInQMap() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with TransposeDataInQMap( QMap< QString, QList< QList< double > > > &data, int firstRow, int firstColumn )" << std::endl;
    }
    else
    {
        std::cerr << "Test_TransposeDataInQMap() PASSED";
    }

    return testTransposeDataInQMap_Passed;
}

bool TestPlot::Test_RemoveUnmatchedSubjects( QString faFilePath )
{
    Plot plot;
    Processing processing;
    QList< QStringList > rawData = processing.GetDataFromFile( faFilePath );
    QList< QStringList > rawDataExpected;
    for( int i = 0; i < rawData.size(); i++ )
    {
        QStringList tempoFArow = QStringList() << rawData.value( i ).first() << rawData.value( i ).at( 1 );
        rawDataExpected.append( tempoFArow );
    }


    plot.m_matchedSubjects = QStringList() << processing.GetSubjectsFromData( rawData, -1 ).first();
    plot.m_subjects = processing.GetSubjectsFromData( rawData, -1 );
    plot.RemoveUnmatchedSubjects( rawData );

    bool testSubjects = plot.m_subjects == plot.m_matchedSubjects;
    bool testData = rawData == rawDataExpected;


    bool testRemoveUnmatchedSubjects_Passed = testSubjects && testData;
    if( !testRemoveUnmatchedSubjects_Passed )
    {
        std::cerr << "/!\\/!\\ Test_RemoveUnmatchedSubjects() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with RemoveUnmatchedSubjects( QList<QStringList> &rawData )" << std::endl;
//        if( !testSubjects )
//        {
//            std::cerr << "\t+ wrong subjects kept" << std::endl;
//            DisplayError_QStringList( plot.m_matchedSubjects, plot.m_subjects, "files" );
//        }
//        if( !testData )
//        {
//            std::cerr << "\t+ wrong raw data values kept" << std::endl;
//        }
    }
    else
    {
        std::cerr << "Test_RemoveUnmatchedSubjects() PASSED";
    }

    return testRemoveUnmatchedSubjects_Passed;
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
        std::cerr << "/!\\/!\\ Test_SetRawData() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetRawData()" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetRawData() PASSED";
    }

    return testSetRawData_Passed;
}

bool TestPlot::Test_SetRawDataQCThreshold( QString faFilePath )
{
    Plot plot;
    Processing processing;
    QList< QStringList > rawData = processing.GetDataFromFile( faFilePath );
    QList< QList< double > > faRawData;
    faRawData = QList< QList< double > >() << ( QList< double >() << -39.1846 << -38.1846 << -37.1846 << -36.1846 << -35.1846 << -34.1846 << -33.1846 << -32.1846 << -31.1846 << -30.1846 << -29.1846
                                                << -28.1846 << -27.1846 << -26.1846 << -25.1846 << -24.1846 << -23.1846 << -22.1846 << -21.1846 << -20.1846 << -19.1846 << -18.1846
                                                << -17.1846 << -16.1846 << -15.1846 << -14.1846 << -13.1846 << -12.1846 << -11.1846 << -10.1846 << -9.18462 << -8.18462 << -7.18462
                                                << -6.18462 << -5.18462 << -4.18462 << -3.18462 << -2.18462 << -1.18462 << -0.184619 << 0.815381 << 1.81538 << 2.81538 << 3.81538
                                                << 4.81538 << 5.81538 << 6.81538 << 7.81538 << 8.81538 << 9.81538 << 10.8154 << 11.8154 << 12.8154 << 13.8154 << 14.8154 << 15.8154
                                                << 16.8154 << 17.8154 << 18.8154 << 19.8154 << 20.8154 << 21.8154 << 22.8154 << 23.8154 << 24.8154 << 25.8154 << 26.8154 << 27.8154
                                                << 28.8154 << 29.8154 << 30.8154 << 31.8154 << 32.8154 << 33.8154 << 34.8154 << 35.8154 << 36.8154 << 37.8154 << 38.8154 )
                                           << ( QList< double >() << 0.101391 << 0.089874 << 0.0899232 << 0.106037 << 0.130884 << 0.15669 << 0.180479 << 0.199196 << 0.213933 << 0.228779 << 0.244781
                                                << 0.258981 << 0.266899 << 0.267612 << 0.262843 << 0.250976 << 0.231375 << 0.209352 << 0.189447 << 0.169195 << 0.14859 << 0.132038
                                                << 0.12105 << 0.116202 << 0.118984 << 0.126646 << 0.128749 << 0.130604 << 0.138109 << 0.144723 << 0.152909 << 0.162929 << 0.175347
                                                << 0.190291 << 0.200493 << 0.208809 << 0.222114 << 0.241194 << 0.274804 << 0.33242 << 0.365297 << 0.366184 << 0.356722 << 0.350173
                                                << 0.346578 << 0.351808 << 0.361099 << 0.367868 << 0.376094 << 0.380544 << 0.377181 << 0.372649 << 0.365934 << 0.354917 << 0.342584
                                                << 0.333111 << 0.328198 << 0.325226 << 0.322341 << 0.318701 << 0.313947 << 0.303292 << 0.285643 << 0.267017 << 0.251371 << 0.237579
                                                << 0.223202 << 0.206765 << 0.189013 << 0.172983 << 0.160959 << 0.153894 << 0.145722 << 0.13097 << 0.114278 << 0.106507 << 0.105277
                                                << 0.0902092 << 0.056156 );
    QList< QList< double > > rawDataExpected = faRawData;


    plot.m_matchedSubjects = QStringList() << processing.GetSubjectsFromData( rawData, -1 ).first();
    plot.m_subjects = processing.GetSubjectsFromData( rawData, -1 );
    plot.SetRawDataQCThreshold( rawData );


    bool testSetRawDataQCThreshold_Passed = plot.m_dataRawData.value( "FA" ) == rawDataExpected;
    if( !testSetRawDataQCThreshold_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetRawDataQCThreshold() FAILED /!\\/!\\";
//        std::cerr << std::endl << "\t+ pb with SetRawDataQCThreshold( QMap< QString, QList< QStringList > >& rawData )" << std::endl;
//        std::cerr << "\t+ wrong raw data" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetRawDataQCThreshold() PASSED";
    }

    return testSetRawDataQCThreshold_Passed;
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
        std::cerr << "/!\\/!\\ Test_SetBeta() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetBeta()" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetBeta() PASSED";
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
        std::cerr << "/!\\/!\\ Test_SetOmnibusLpvalue() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetOmnibusLpvalue( QStringList omnibusLpvalueFiles, QList < QList < double > > &omnibusLpvaluesData )" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetOmnibusLpvalue() PASSED";
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
        std::cerr << "/!\\/!\\ Test_SetConfidenceBands() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetConfidenceBands()" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetConfidenceBands() PASSED";
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
        std::cerr << "/!\\/!\\ Test_SetPostHocFDRLpvalue() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetPostHocFDRLpvalue()" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetPostHocFDRLpvalue() PASSED";
    }

    return testSetPostHocFDRLpvalue_Passed;
}




bool TestPlot::Test_GetRawDataFiles( QString dataDir, QString tempoDir )
{
    Plot plot;
    QString dirTest = tempoDir + "/TestPlot/Test_GetRawDataFiles";
    QDir().mkpath( dirTest );
    QFile::copy( dataDir + "/test_RawData_FA.csv", dirTest + "/test_RawData_FA.csv" );
    QFile::copy( dataDir + "/test_RawData_RD.csv", dirTest + "/test_RawData_RD.csv" );
    QFile::copy( dataDir + "/test_RawData_SUBMATRIX.csv", dirTest + "/test_RawData_SUBMATRIX.csv" );
    QStringList expectedRawDataFiles = QStringList() << "test_RawData_FA.csv" << "test_RawData_RD.csv" << "test_RawData_SUBMATRIX.csv";


    plot.m_directory = dirTest;
    plot.GetRawDataFiles();


    bool testGetRawDataFiles_Passed = plot.m_csvRawDataFiles == expectedRawDataFiles;
    if( !testGetRawDataFiles_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetRawDataFiles() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with GetRawDataFiles()" << std::endl;
        //        DisplayError_QStringList( expectedRawDataFiles, plot.m_csvRawDataFiles, "files" );
    }
    else
    {
        std::cerr << "Test_GetRawDataFiles() PASSED";
    }

    return testGetRawDataFiles_Passed;
}

bool TestPlot::Test_GetBetaFiles( QString dataDir, QString tempoDir )
{
    Plot plot;
    QString dirTest = tempoDir + "/TestPlot/Test_GetBetaFiles";
    QDir().mkpath( dirTest );
    QFile::copy( dataDir + "/test_Betas_FA.csv", dirTest + "/test_Betas_FA.csv" );
    QFile::copy( dataDir + "/test_Betas_RD.csv", dirTest + "/test_Betas_RD.csv" );
    QStringList expectedBetaFiles = QStringList() << "test_Betas_FA.csv" << "test_Betas_RD.csv";
    expectedBetaFiles.sort();


    plot.m_matlabDirectory = dirTest;
    plot.GetBetaFiles();


    bool testGetBetaFiles_Passed = plot.m_csvBetaFiles == expectedBetaFiles;
    if( !testGetBetaFiles_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetBetaFiles() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with GetBetaFiles()" << std::endl;
        //        DisplayError_QStringList( expectedBetaFiles, plot.m_csvBetaFiles, "files" );
    }
    else
    {
        std::cerr << "Test_GetBetaFiles() PASSED";
    }

    return testGetBetaFiles_Passed;
}

bool TestPlot::Test_GetOmnibusLpvalueFiles( QString dataDir, QString tempoDir )
{
    Plot plot;
    QString dirTest = tempoDir + "/TestPlot/Test_GetOmnibusLpvalueFiles";
    QDir().mkpath( dirTest );
    QFile::copy( dataDir + "/test_Omnibus_Local_pvalues.csv", dirTest + "/test_Omnibus_Local_pvalues.csv" );
    QStringList expectedOmnibusLpvalueFiles = QStringList() << "test_Omnibus_Local_pvalues.csv";
    expectedOmnibusLpvalueFiles.sort();


    plot.m_matlabDirectory = dirTest;
    plot.GetOmnibusLpvalueFiles();


    bool testGetOmnibusLpvalueFiles_Passed = plot.m_csvOmnibusLpvalueFiles == expectedOmnibusLpvalueFiles;
    if( !testGetOmnibusLpvalueFiles_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetOmnibusLpvalueFiles() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with GetOmnibusLpvalueFiles()" << std::endl;
        //        DisplayError_QStringList( expectedOmnibusLpvalueFiles, plot.m_csvOmnibusLpvalueFiles, "files" );
    }
    else
    {
        std::cerr << "Test_GetOmnibusLpvalueFiles() PASSED";
    }

    return testGetOmnibusLpvalueFiles_Passed;
}

bool TestPlot::Test_GetOmnibusFDRLpvalueFiles( QString dataDir, QString tempoDir )
{
    Plot plot;
    QString dirTest = tempoDir + "/TestPlot/Test_GetOmnibusFDRLpvalueFiles";
    QDir().mkpath( dirTest );
    QFile::copy( dataDir + "/test_Omnibus_FDR_Local_pvalues.csv", dirTest + "/test_Omnibus_FDR_Local_pvalues.csv" );
    QStringList expectedOmnibusFDRLpvalueFiles = QStringList() << "test_Omnibus_FDR_Local_pvalues.csv";
    expectedOmnibusFDRLpvalueFiles.sort();


    plot.m_matlabDirectory = dirTest;
    plot.GetOmnibusFDRLpvalueFiles();


    bool testGetOmnibusFDRLpvalueFiles_Passed = plot.m_csvOmnibusFDRLpvalueFiles == expectedOmnibusFDRLpvalueFiles;
    if( !testGetOmnibusFDRLpvalueFiles_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetOmnibusFDRLpvalueFiles() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with GetOmnibusFDRLpvalueFiles()" << std::endl;
        //        DisplayError_QStringList( expectedOmnibusFDRLpvalueFiles, plot.m_csvOmnibusFDRLpvalueFiles, "files" );
    }
    else
    {
        std::cerr << "Test_GetOmnibusFDRLpvalueFiles() PASSED";
    }

    return testGetOmnibusFDRLpvalueFiles_Passed;
}

bool TestPlot::Test_GetConfidenceBandsFiles( QString dataDir, QString tempoDir )
{
    Plot plot;
    QString dirTest = tempoDir + "/TestPlot/Test_GetConfidenceBandsFiles";
    QDir().mkpath( dirTest );
    QFile::copy( dataDir + "/test_Omnibus_ConfidenceBands_FA.csv", dirTest + "/test_Omnibus_ConfidenceBands_FA.csv" );
    QFile::copy( dataDir + "/test_Omnibus_ConfidenceBands_RD.csv", dirTest + "/test_Omnibus_ConfidenceBands_RD.csv" );
    QStringList expectedConfidenceBandsFiles = QStringList() << "test_Omnibus_ConfidenceBands_FA.csv" << "test_Omnibus_ConfidenceBands_RD.csv";
    expectedConfidenceBandsFiles.sort();


    plot.m_matlabDirectory = dirTest;
    plot.GetConfidenceBandsFiles();


    bool testGetConfidenceBandsFiles_Passed = plot.m_csvConfidenceBandsFiles == expectedConfidenceBandsFiles;
    if( !testGetConfidenceBandsFiles_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetConfidenceBandsFiles() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with GetConfidenceBandsFiles()" << std::endl;
        //        DisplayError_QStringList( expectedConfidenceBandsFiles, plot.m_csvConfidenceBandsFiles, "files" );
    }
    else
    {
        std::cerr << "Test_GetConfidenceBandsFiles() PASSED";
    }

    return testGetConfidenceBandsFiles_Passed;
}

bool TestPlot::Test_GetPostHocFDRLpvalueFiles( QString dataDir, QString tempoDir )
{
    Plot plot;
    QString dirTest = tempoDir + "/TestPlot/Test_GetPostHocFDRLpvalueFiles";
    QDir().mkpath( dirTest );
    QFile::copy( dataDir + "/test_PostHoc_FDR_Local_pvalues_FA.csv", dirTest + "/test_PostHoc_FDR_Local_pvalues_FA.csv" );
    QFile::copy( dataDir + "/test_PostHoc_FDR_Local_pvalues_RD.csv", dirTest + "/test_PostHoc_FDR_Local_pvalues_RD.csv" );
    QStringList expectedGetPostHocFDRLpvalueFiles = QStringList() << "test_PostHoc_FDR_Local_pvalues_FA.csv" << "test_PostHoc_FDR_Local_pvalues_RD.csv";
    expectedGetPostHocFDRLpvalueFiles.sort();


    plot.m_matlabDirectory = dirTest;
    plot.GetPostHocFDRLpvalueFiles();


    bool testGetPostHocFDRLpvalueFiles_Passed = plot.m_csvPostHocFDRLpvalueFiles == expectedGetPostHocFDRLpvalueFiles;
    if( !testGetPostHocFDRLpvalueFiles_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetPostHocFDRLpvalueFiles() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with GetPostHocFDRLpvalueFiles()" << std::endl;
        //        DisplayError_QStringList( expectedGetPostHocFDRLpvalueFiles, plot.m_csvPostHocFDRLpvalueFiles, "files" );
    }
    else
    {
        std::cerr << "Test_GetPostHocFDRLpvalueFiles() PASSED";
    }
    return testGetPostHocFDRLpvalueFiles_Passed;
}



bool TestPlot::Test_SetPlots( QString dataDir, QString tempoDir )
{
    Plot plot;
    QString dirTest = tempoDir + "/TestPlot/Test_SetPlots";
    QString matlabDir = dirTest + "/MatlabOutputs";
    QDir().mkpath( dirTest );
    QDir().mkpath( matlabDir );
    QFile::copy( dataDir + "/test_RawData_FA.csv", dirTest + "/test_RawData_FA.csv" );
    QFile::copy( dataDir + "/test_RawData_RD.csv", dirTest + "/test_RawData_RD.csv" );
    QFile::copy( dataDir + "/test_RawData_SUBMATRIX.csv", dirTest + "/test_RawData_SUBMATRIX.csv" );
    QFile::copy( dataDir + "/test_Betas_FA.csv", matlabDir + "/test_Betas_FA.csv" );
    QFile::copy( dataDir + "/test_Betas_RD.csv", matlabDir + "/test_Betas_RD.csv" );
    QFile::copy( dataDir + "/test_Omnibus_Local_pvalues.csv", matlabDir + "/test_Omnibus_Local_pvalues.csv" );
    QFile::copy( dataDir + "/test_Omnibus_FDR_Local_pvalues.csv", matlabDir + "/test_Omnibus_FDR_Local_pvalues.csv" );
    QFile::copy( dataDir + "/test_Omnibus_ConfidenceBands_FA.csv", matlabDir + "/test_Omnibus_ConfidenceBands_FA.csv" );
    QFile::copy( dataDir + "/test_Omnibus_ConfidenceBands_RD.csv", matlabDir + "/test_Omnibus_ConfidenceBands_RD.csv" );
    QFile::copy( dataDir + "/test_PostHoc_FDR_Local_pvalues_FA.csv", matlabDir + "/test_PostHoc_FDR_Local_pvalues_FA.csv" );
    QFile::copy( dataDir + "/test_PostHoc_FDR_Local_pvalues_RD.csv", matlabDir + "/test_PostHoc_FDR_Local_pvalues_RD.csv" );
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


    plot.m_directory = dirTest;
    plot.m_matlabDirectory = matlabDir;
    plot.GetRawDataFiles();
    plot.SetPlots();
    QStringList displayedPlotsUsedRawData = plot.m_plotsUsed;
    bool testRawData = expectedPlotsUsedRawData == displayedPlotsUsedRawData;

    plot.ResetPlotData();
    plot.m_directory = dirTest;
    plot.m_matlabDirectory = matlabDir;
    plot.GetBetaFiles();
    plot.SetPlots();
    QStringList displayedPlotsUsedBetas = plot.m_plotsUsed;
    bool testRawBeta = expectedPlotsUsedBetas == displayedPlotsUsedBetas;

    plot.ResetPlotData();
    plot.m_directory = dirTest;
    plot.m_matlabDirectory = matlabDir;
    plot.GetOmnibusLpvalueFiles();
    plot.SetPlots();
    QStringList displayedPlotsUsedOmnibusLpvalues = plot.m_plotsUsed;
    bool testOmnibusLocalPvalues = expectedPlotsUsedOmnibusLpvalues == displayedPlotsUsedOmnibusLpvalues;

    plot.ResetPlotData();
    plot.m_directory = dirTest;
    plot.m_matlabDirectory = matlabDir;
    plot.GetOmnibusFDRLpvalueFiles();
    plot.SetPlots();
    QStringList displayedPlotsUsedOmnibusFDRLpvalues = plot.m_plotsUsed;
    bool testOmnibusFDRLocalPvalues = expectedPlotsUsedOmnibusFDRLpvalues == displayedPlotsUsedOmnibusFDRLpvalues;

    plot.ResetPlotData();
    plot.m_directory = dirTest;
    plot.m_matlabDirectory = matlabDir;
    plot.GetBetaFiles();
    plot.GetOmnibusFDRLpvalueFiles();
    plot.SetPlots();
    QStringList displayedPlotsUsedOmnibusFDRSigBetas = plot.m_plotsUsed;
    bool testOmnibusFDRSignificantBetas = expectedPlotsUsedOmnibusFDRSigBetas == displayedPlotsUsedOmnibusFDRSigBetas;

    plot.ResetPlotData();
    plot.m_directory = dirTest;
    plot.m_matlabDirectory = matlabDir;
    plot.GetBetaFiles();
    plot.GetConfidenceBandsFiles();
    plot.SetPlots();
    QStringList displayedPlotsUsedConfidenceBands = plot.m_plotsUsed;
    bool testConfidenceBands = expectedPlotsUsedConfidenceBands == displayedPlotsUsedConfidenceBands;

    plot.ResetPlotData();
    plot.m_directory = dirTest;
    plot.m_matlabDirectory = matlabDir;
    plot.GetPostHocFDRLpvalueFiles();
    plot.SetPlots();
    QStringList displayedPlotsUsedPostHocFDRLpvalue = plot.m_plotsUsed;
    bool testPostHocFDRLocalPvalues = expectedPlotsUsedPostHocFDRLpvalue == displayedPlotsUsedPostHocFDRLpvalue;

    plot.ResetPlotData();
    plot.m_directory = dirTest;
    plot.m_matlabDirectory = matlabDir;
    plot.GetBetaFiles();
    plot.GetPostHocFDRLpvalueFiles();
    plot.SetPlots();
    QStringList displayedPlotsUsedPostHocFDRSigBetas = plot.m_plotsUsed;
    bool testPostHocFDRSignificantBetas = expectedPlotsUsedPostHocFDRSigBetas == displayedPlotsUsedPostHocFDRSigBetas;


    bool testSetPlots_Passed = testRawData && testRawBeta && testOmnibusLocalPvalues && testOmnibusFDRLocalPvalues
            && testOmnibusFDRSignificantBetas && testConfidenceBands && testPostHocFDRLocalPvalues && testPostHocFDRSignificantBetas;
    if( !testSetPlots_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetPlots() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetPlots()" << std::endl;
        //        if( !testRawData )
        //        {
        //            std::cerr << "\t+ when getting Raw Data" << std::endl;
        //            DisplayError_QStringList( expectedPlotsUsedRawData, expectedPlotsUsedRawData, "plots" );
        //        }
        //        if( !testRawBeta )
        //        {
        //            std::cerr << "\t+ when getting Betas" << std::endl;
        //            DisplayError_QStringList( expectedPlotsUsedBetas, displayedPlotsUsedBetas, "plots" );
        //        }
        //        if( !testOmnibusLocalPvalues )
        //        {
        //            std::cerr << "\t+ when getting Omnibus Local Pvalues" << std::endl;
        //            DisplayError_QStringList( expectedPlotsUsedOmnibusLpvalues, displayedPlotsUsedOmnibusLpvalues, "plots" );
        //        }
        //        if( !testOmnibusFDRLocalPvalues )
        //        {
        //            std::cerr << "\t+ when getting Omnibus FDR Local Pvalues" << std::endl;
        //            DisplayError_QStringList( expectedPlotsUsedOmnibusFDRLpvalues, displayedPlotsUsedOmnibusFDRLpvalues, "plots" );
        //        }
        //        if( !testOmnibusFDRSignificantBetas )
        //        {
        //            std::cerr << "\t+ when getting Omnibus FDR Significant Betas" << std::endl;
        //            DisplayError_QStringList( expectedPlotsUsedOmnibusFDRSigBetas, displayedPlotsUsedOmnibusFDRSigBetas, "plots" );
        //        }
        //        if( !testConfidenceBands )
        //        {
        //            std::cerr << "\t+ when getting Confidence Bands" << std::endl;
        //            DisplayError_QStringList( expectedPlotsUsedConfidenceBands, displayedPlotsUsedConfidenceBands, "plots" );
        //        }
        //        if( !testPostHocFDRLocalPvalues )
        //        {
        //            std::cerr << "\t+ when getting PostHoc FDR Local Pvalues" << std::endl;
        //            DisplayError_QStringList( expectedPlotsUsedPostHocFDRLpvalue, displayedPlotsUsedPostHocFDRLpvalue, "plots" );
        //        }
        //        if( !testPostHocFDRSignificantBetas )
        //        {
        //            std::cerr << "\t+ when getting PostHoc FDR Significant Betas" << std::endl;
        //            DisplayError_QStringList( expectedPlotsUsedPostHocFDRSigBetas, displayedPlotsUsedPostHocFDRSigBetas, "plots" );
        //        }
    }
    else
    {
        std::cerr << "Test_SetPlots() PASSED";
    }
    return testSetPlots_Passed;
}

bool TestPlot::Test_SetProperties( QString dataDir, QString tempoDir )
{
    Plot plot;
    QString dirTest = tempoDir + "/TestPlot/Test_SetProperties";
    QDir().mkpath( dirTest );
    QFile::copy( dataDir + "/test_RawData_FA.csv", dirTest + "/test_RawData_FA.csv" );
    QFile::copy( dataDir + "/test_RawData_RD.csv", dirTest + "/test_RawData_RD.csv" );
    QFile::copy( dataDir + "/test_RawData_SUBMATRIX.csv", dirTest + "/test_RawData_SUBMATRIX.csv" );
    QMap< int, QString > expectedProperties;
    expectedProperties.insert( 1, "RD" );
    expectedProperties.insert( 3, "FA" );


    plot.m_directory = dirTest;
    plot.GetRawDataFiles();
    plot.SetProperties();


    bool testSetProperties_Passed = plot.m_properties == expectedProperties;
    if( !testSetProperties_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetProperties() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetProperties()" << std::endl;
        //        DisplayError_QMapIntQString( expectedProperties, plot.m_properties, "properties" );
    }
    else
    {
        std::cerr << "Test_SetProperties() PASSED";
    }
    return testSetProperties_Passed;
}

bool TestPlot::Test_SetSubjects( QString dataDir, QString tempoDir )
{
    Plot plot;
    QString dirTest = tempoDir + "/TestPlot/Test_SetSubjects";
    QDir().mkpath( dirTest );
    QFile::copy( dataDir + "/test_RawData_FA.csv", dirTest + "/test_RawData_FA.csv" );
    QFile::copy( dataDir + "/test_RawData_RD.csv", dirTest + "/test_RawData_RD.csv" );
    QFile::copy( dataDir + "/test_RawData_SUBMATRIX.csv", dirTest + "/test_RawData_SUBMATRIX.csv" );
    QStringList expectedSubjects = QStringList() << "F02-1_dwi_35_all_QCed_VC_DTI_embed" << "F05-1_dwi_35_all_QCed_VC_DTI_embed" << "F06-1_42_DWI_QCed_VC_DTI_embed"
                                                 << "F07-1_42_DWI_QCed_VC_DTI_embed" << "F09-1_42_DWI_QCed_VC_DTI_embed" << "F12-1_42_DWI_QCed_VC_DTI_embed"
                                                 << "F21-1_42_DWI_QCed_VC_DTI_embed" << "F22-1_42_DWI_QCed_VC_DTI_embed" << "F23-1_42_DWI_QCed_VC_DTI_embed"
                                                 << "F25-1_42_DWI_QCed_VC_DTI_embed";


    plot.m_directory = dirTest;
    plot.GetRawDataFiles();
    plot.SetSubjects();


    bool testSetSubjects_Passed = plot.m_subjects == expectedSubjects;
    if( !testSetSubjects_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetSubjects() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetSubjects()" << std::endl;
        //        DisplayError_QStringList( expectedSubjects, plot.m_subjects, "subjects" );
    }
    else
    {
        std::cerr << "Test_SetSubjects() PASSED";
    }
    return testSetSubjects_Passed;
}

bool TestPlot::Test_SetCovariates( QString dataDir, QString tempoDir )
{
    Plot plot;
    QString dirTest = tempoDir + "/TestPlot/Test_SetCovariates";
    QDir().mkpath( dirTest );
    QFile::copy( dataDir + "/test_RawData_FA.csv", dirTest + "/test_RawData_FA.csv" );
    QFile::copy( dataDir + "/test_RawData_RD.csv", dirTest + "/test_RawData_RD.csv" );
    QFile::copy( dataDir + "/test_RawData_SUBMATRIX.csv", dirTest + "/test_RawData_SUBMATRIX.csv" );
    QMap< int, QString > expectedAllCovariates;
    expectedAllCovariates.insert( 0, "Intercept" );
    expectedAllCovariates.insert( 1, "ngroup" );
    expectedAllCovariates.insert( 2, "GENDER" );
    expectedAllCovariates.insert( 3, "GestAgeAtBirth" );
    QMap< int, QString > expectedCovariatesNoIntercept;
    expectedCovariatesNoIntercept.insert( 1, "ngroup" );
    expectedCovariatesNoIntercept.insert( 2, "GENDER" );
    expectedCovariatesNoIntercept.insert( 3, "GestAgeAtBirth" );
    QMap< int, QString > expectedBinaryCovariates;
    expectedBinaryCovariates.insert( 1, "ngroup" );
    expectedBinaryCovariates.insert( 2, "GENDER" );


    plot.m_directory = dirTest;
    plot.GetRawDataFiles();
    plot.SetCovariates();

    bool testAllCovariates = plot.m_allCovariates == expectedAllCovariates;
    bool testCovariatesNoIntercept = plot.m_covariatesNoIntercept == expectedCovariatesNoIntercept;
    bool testBinaryCovariates = plot.m_binaryCovariates == expectedBinaryCovariates;


    bool testSetCovariates_Passed = testAllCovariates && testCovariatesNoIntercept && testBinaryCovariates;
    if( !testSetCovariates_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetCovariates() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetCovariates()" << std::endl;
        //        if( !testAllCovariates )
        //        {
        //            std::cerr << "\t+ pb with m_allCovariates" << std::endl;
        //            DisplayError_QMapIntQString( expectedAllCovariates, plot.m_allCovariates, "covariates" );
        //        }
        //        if( !testCovariatesNoIntercept )
        //        {
        //            std::cerr << "\t+ pb with m_covariatesNoIntercept" << std::endl;
        //            DisplayError_QMapIntQString( expectedCovariatesNoIntercept, plot.m_covariatesNoIntercept, "covariates" );
        //        }
        //        if( !testBinaryCovariates )
        //        {
        //            std::cerr << "\t+ pb with m_binaryCovariates" << std::endl;
        //            DisplayError_QMapIntQString( expectedBinaryCovariates, plot.m_binaryCovariates, "covariates" );
        //        }
    }
    else
    {
        std::cerr << "Test_SetCovariates() PASSED";
    }
    return testSetCovariates_Passed;
}

bool TestPlot::Test_SetAbscissa( QString dataDir, QString tempoDir )
{
    Plot plot;
    QString dirTest = tempoDir + "/TestPlot/Test_SetAbscissa";
    QDir().mkpath( dirTest );
    QFile::copy( dataDir + "/test_RawData_FA.csv", dirTest + "/test_RawData_FA.csv" );
    QFile::copy( dataDir + "/test_RawData_RD.csv", dirTest + "/test_RawData_RD.csv" );
    QFile::copy( dataDir + "/test_RawData_SUBMATRIX.csv", dirTest + "/test_RawData_SUBMATRIX.csv" );
    QList< double > expectedAbscissa = QList< double >() << -39.1846 << -38.1846 << -37.1846 << -36.1846 << -35.1846 << -34.1846 << -33.1846 << -32.1846 << -31.1846 << -30.1846
                                                         << -29.1846 << -28.1846 << -27.1846 << -26.1846 << -25.1846 << -24.1846 << -23.1846 << -22.1846 << -21.1846 << -20.1846
                                                         << -19.1846 << -18.1846 << -17.1846 << -16.1846 << -15.1846 << -14.1846 << -13.1846 << -12.1846 << -11.1846 << -10.1846
                                                         << -9.18462 << -8.18462 << -7.18462 << -6.18462 << -5.18462 << -4.18462 << -3.18462 << -2.18462 << -1.18462 << -0.184619
                                                         << 0.815381 << 1.81538 << 2.81538 << 3.81538 << 4.81538 << 5.81538 << 6.81538 << 7.81538 << 8.81538 << 9.81538
                                                         << 10.8154 << 11.8154 << 12.8154 << 13.8154 << 14.8154 << 15.8154 << 16.8154 << 17.8154 << 18.8154 << 19.8154
                                                         << 20.8154 << 21.8154 << 22.8154 << 23.8154 << 24.8154 << 25.8154 << 26.8154 << 27.8154 << 28.8154 << 29.8154
                                                         << 30.8154 << 31.8154 << 32.8154 << 33.8154 << 34.8154 << 35.8154 << 36.8154 << 37.8154 << 38.8154;


    plot.m_directory = dirTest;
    plot.GetRawDataFiles();
    plot.SetAbscissa();


    bool testSetAbscissa_Passed = plot.m_abscissa == expectedAbscissa;
    if( !testSetAbscissa_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetAbscissa() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetAbscissa()" << std::endl;
    }
    else
    {
        std::cerr << "Test_SetAbscissa() PASSED";
    }
    return testSetAbscissa_Passed;
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


    bool testSetSelectedPlot_Passed = testAllCovariates && testCovariatesNoIntercept && testBinaryCovariates;
    if( !testSetSelectedPlot_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetSelectedPlot() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetSelectedPlot( QString plotSelected )" << std::endl;
        //        if( !testAllCovariates )
        //        {
        //            std::cerr << "\t  when sending m_allCovariates" << std::endl;
        //        }
        //        if( !testCovariatesNoIntercept )
        //        {
        //            std::cerr << "\t  when sending m_covariatesNoIntercept" << std::endl;
        //        }
        //        if( !testBinaryCovariates )
        //        {
        //            std::cerr << "\t  when sending m_binaryCovariates" << std::endl;
        //        }
    }
    else
    {
        std::cerr << "Test_SetSelectedPlot() PASSED";
    }
    return testSetSelectedPlot_Passed;
}



bool TestPlot::Test_SetCustomizedTitle()
{
    Plot plot;
    QSharedPointer< QVTKWidget > qvtkWidget = QSharedPointer< QVTKWidget >( new QVTKWidget() );
    QString title = "Test Title";
    bool bold = true;
    bool italic = false;
    double fontSize = 24;


    plot.SetQVTKWidget( qvtkWidget );
    plot.m_chart = vtkSmartPointer< vtkChartXY >::New();
    plot.SetCustomizedTitle( title, bold, italic, fontSize );

    bool testTitle = title == plot.m_chart->GetTitle();
    bool testBold = bold == plot.m_chart->GetTitleProperties()->GetBold();
    bool testItalic = italic == plot.m_chart->GetTitleProperties()->GetItalic();
    bool testFontSize = fontSize == plot.m_chart->GetTitleProperties()->GetFontSize();


    bool testSetCustomizedTitle_Passed = testTitle && testBold && testItalic && testFontSize;
    if( !testSetCustomizedTitle_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetCustomizedTitle() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetCustomizedTitle( QString title, bool isBold, bool isItalic, double fontSize )" << std::endl;
        //        if( !testTitle )
        //        {
        //            std::cerr << "\t  title expected: " << title.toStdString() << " | title displayed: " << plot.m_chart->GetTitle() << std::endl;
        //        }
        //        if( !testBold )
        //        {
        //            std::cerr << "\t  bold status expected: " << ( bold == 1 ? "TRUE" : "FALSE" ) << " | bold status displayed: " << ( plot.m_chart->GetTitleProperties()->GetBold() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        //        }
        //        if( !testItalic )
        //        {
        //            std::cerr << "\t  italic status expected: " << ( italic == 1 ? "TRUE" : "FALSE" ) << " | italic status displayed: " << ( plot.m_chart->GetTitleProperties()->GetItalic() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        //        }
        //        if( !testFontSize )
        //        {
        //            std::cerr << "\t  fontSize expected: " << fontSize << " | fontSize displayed: " << plot.m_chart->GetTitleProperties()->GetFontSize() << std::endl;
        //        }
    }
    else
    {
        std::cerr << "Test_SetCustomizedTitle() PASSED";
    }
    return testSetCustomizedTitle_Passed;
}

bool TestPlot::Test_SetDefaultTitle()
{
    Plot plot;
    QSharedPointer< QVTKWidget > qvtkWidget = QSharedPointer< QVTKWidget >( new QVTKWidget() );


    plot.SetQVTKWidget( qvtkWidget );
    plot.m_chart = vtkSmartPointer< vtkChartXY >::New();
    plot.m_fibername = "Test";
    plot.SetSelectedProperty() = "RD";
    plot.SetSelectedCovariate() = "GENDER";
    plot.SetPvalueThreshold() = 0.05;

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


    bool testSetDefaultTitle_Passed = testTitle && testBold && testItalic && testFontSize;
    if( !testSetDefaultTitle_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetDefaultTitle() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetDefaultTitle()" << std::endl;
        //        if( !testTitle )
        //        {
        //            std::cerr << "\t  wrong title displayed" << std::endl;
        //        }
        //        if( !testBold )
        //        {
        //            std::cerr << "\t  bold status expected: FALSE | bold status displayed: " << ( plot.m_chart->GetTitleProperties()->GetBold() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        //        }
        //        if( !testItalic )
        //        {
        //            std::cerr << "\t  italic status expected: FALSE | italic status displayed: " << ( plot.m_chart->GetTitleProperties()->GetItalic() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        //        }
        //        if( !testFontSize )
        //        {
        //            std::cerr << "\t  fontSize expected: 12.0 | fontSize displayed: " << plot.m_chart->GetTitleProperties()->GetFontSize() << std::endl;
        //        }
    }
    else
    {
        std::cerr << "Test_SetDefaultTitle() PASSED";
    }
    return testSetDefaultTitle_Passed;
}

bool TestPlot::Test_SetCustomizedAxis()
{
    Plot plot;
    QSharedPointer< QVTKWidget > qvtkWidget = QSharedPointer< QVTKWidget >( new QVTKWidget() );
    QString xName = "Title Abscissa";
    QString yName = "Title Ordinate";
    bool bold = true;
    bool italic = false;
    bool yMinSet = false;
    double yMin = -5;
    bool yMaxSet = true;
    double yMax = 5;


    plot.SetQVTKWidget( qvtkWidget );
    plot.m_chart = vtkSmartPointer< vtkChartXY >::New();
    plot.SetCustomizedAxis( 12, xName, yName, 12, bold, italic, yMinSet, yMin, yMaxSet, yMax );

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


    bool testSetCustomizedAxis_Passed = testXName && testYName && testBold && testItalic && testYMinSet && testYMinValue && testYMaxSet && testYMaxValue;
    if( !testSetCustomizedAxis_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetCustomizedAxis() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetCustomizedAxis( QString xName, QString yName, bool isBold, bool isItalic,"
        //                     "bool isYMinSet, double yMin, bool isYMaxSet, double yMax )" << std::endl;
        //        if( !testXName )
        //        {
        //            std::cerr << "\t  xName expected: " << xName.toStdString() << " | xName displayed: " << plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitle() << std::endl;
        //        }
        //        if( !testYName )
        //        {
        //            std::cerr << "\t  yName expected: " << yName.toStdString() << " | yName displayed: " << plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitle() << std::endl;
        //        }
        //        if( !testBold )
        //        {
        //            std::cerr << "\t  bold vtkAxis::BOTTOM status expected: " << ( bold == 1 ? "TRUE" : "FALSE" ) << " | bold status displayed: " << ( plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->GetBold() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        //            std::cerr << "\t  bold vtkAxis::LEFT status expected: " << ( bold == 1 ? "TRUE" : "FALSE" ) << " | bold status displayed: " << ( plot.m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->GetBold() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        //        }
        //        if( !testItalic )
        //        {
        //            std::cerr << "\t  italic vtkAxis::BOTTOM status expected: " << ( italic == 1 ? "TRUE" : "FALSE" ) << " | italic status displayed: " << ( plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->GetItalic() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        //            std::cerr << "\t  italic vtkAxis::LEFT status expected: " << ( italic == 1 ? "TRUE" : "FALSE" ) << " | italic status displayed: " << ( plot.m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->GetItalic() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        //        }
        //        if( !testYMinSet )
        //        {
        //            std::cerr << "\t  yMinSet status expected: " << ( yMinSet == 1 ? "TRUE" : "FALSE" ) << " | yMinSet displayed: " << ( plot.m_yMin.first == 1 ? "TRUE" : "FALSE" ) << std::endl;
        //        }
        //        if( !testYMinValue )
        //        {
        //            std::cerr << "\t  yMin expected: " << 0 << " | yMin displayed: " << plot.m_yMin.second << std::endl;
        //        }
        //        if( !testYMaxSet )
        //        {
        //            std::cerr << "\t  yMaxSet status expected: " << ( yMaxSet == 1 ? "TRUE" : "FALSE" ) << " | yMaxSet displayed: " << ( plot.m_yMax.first == 1 ? "TRUE" : "FALSE" ) << std::endl;
        //        }
        //        if( !testYMaxValue )
        //        {
        //            std::cerr << "\t  yMax expected: " << yMax << " | yMax displayed: " << plot.m_yMax.second << std::endl;
        //        }
    }
    else
    {
        std::cerr << "Test_SetCustomizedAxis() PASSED";
    }
    return testSetCustomizedAxis_Passed;
}

bool TestPlot::Test_SetDefaultAxis()
{
    Plot plot;
    QSharedPointer< QVTKWidget > qvtkWidget = QSharedPointer< QVTKWidget >( new QVTKWidget() );


    plot.SetQVTKWidget( qvtkWidget );
    plot.m_plotSelected = "Omnibus Local pvalues";
    plot.m_chart = vtkSmartPointer< vtkChartXY >::New();
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


    bool testSetDefaultAxis_Passed = testXName && testYName && testBold && testItalic && testYMinSet && testYMinValue && testYMaxSet && testYMaxValue;
    if( !testSetDefaultAxis_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetDefaultAxis() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetDefaultAxis()" << std::endl;
        //        if( !testXName )
        //        {
        //            std::cerr << "\t  xName expected: Arc Length | xName displayed: " << plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitle() << std::endl;
        //        }
        //        if( !testYName )
        //        {
        //            std::cerr << "\t  yName expected: -log10 | yName displayed: " << plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitle() << std::endl;
        //        }
        //        if( !testBold )
        //        {
        //            std::cerr << "\t  bold vtkAxis::BOTTOM status expected: FALSE | bold status displayed: " << ( plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->GetBold() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        //            std::cerr << "\t  bold vtkAxis::LEFT status expected: FALSE | bold status displayed: " << ( plot.m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->GetBold() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        //        }
        //        if( !testItalic )
        //        {
        //            std::cerr << "\t  italic vtkAxis::BOTTOM status expected: FALSE | italic status displayed: " << ( plot.m_chart->GetAxis( vtkAxis::BOTTOM )->GetTitleProperties()->GetItalic() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        //            std::cerr << "\t  italic vtkAxis::LEFT status expected: FALSE | italic status displayed: " << ( plot.m_chart->GetAxis( vtkAxis::LEFT )->GetTitleProperties()->GetItalic() == 1 ? "TRUE" : "FALSE" ) << std::endl;
        //        }
        //        if( !testYMinSet )
        //        {
        //            std::cerr << "\t  yMinSet status expected: FALSE | yMinSet displayed: " << ( plot.m_yMin.first == 1 ? "TRUE" : "FALSE" ) << std::endl;
        //        }
        //        if( !testYMinValue )
        //        {
        //            std::cerr << "\t  yMin expected: " << 0 << " | yMin displayed: " << plot.m_yMin.second << std::endl;
        //        }
        //        if( !testYMaxSet )
        //        {
        //            std::cerr << "\t  yMaxSet status expected: FALSE | yMaxSet displayed: " << ( plot.m_yMax.first == 1 ? "TRUE" : "FALSE" ) << std::endl;
        //        }
        //        if( !testYMaxValue )
        //        {
        //            std::cerr << "\t  yMax expected: " << 0 << " | yMax displayed: " << plot.m_yMax.second << std::endl;
        //        }
    }
    else
    {
        std::cerr << "Test_SetDefaultAxis() PASSED";
    }
    return testSetDefaultAxis_Passed;
}

bool TestPlot::Test_SetLegend()
{
    Plot plot;
    QSharedPointer< QVTKWidget > qvtkWidget = QSharedPointer< QVTKWidget >( new QVTKWidget() );


    plot.SetQVTKWidget( qvtkWidget );
    plot.m_chart = vtkSmartPointer< vtkChartXY >::New();

    plot.m_plotSelected = "Raw Data";
    plot.SetLegend( true, "Top Left" );
    bool testLegendAlignment1 = ( plot.m_chart->GetLegend()->GetVerticalAlignment() == vtkChartLegend::TOP
                                  && plot.m_chart->GetLegend()->GetHorizontalAlignment() == vtkChartLegend::LEFT );
    bool testLegendHidden1  = plot.m_chart->GetShowLegend() == false;

    plot.m_plotSelected = "Omnibus Local pvalues";
    plot.SetLegend( true, "Middle Center" );
    bool testLegendAlignment2 = ( plot.m_chart->GetLegend()->GetVerticalAlignment() == vtkChartLegend::CENTER
                                  && plot.m_chart->GetLegend()->GetHorizontalAlignment() == vtkChartLegend::CENTER );
    bool testLegendHidden2  = plot.m_chart->GetShowLegend() == true;

    plot.SetLegend( true, "Bottom Right" );
    bool testLegendAlignment3 = ( plot.m_chart->GetLegend()->GetVerticalAlignment() == vtkChartLegend::BOTTOM
                                  && plot.m_chart->GetLegend()->GetHorizontalAlignment() == vtkChartLegend::RIGHT );


    bool testSetLegend_Passed = testLegendAlignment1 && testLegendAlignment2 && testLegendAlignment3 && testLegendHidden1 && testLegendHidden2;
    if( !testSetLegend_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetLegend() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetLegend( QString position )" << std::endl;
        //        if( !testLegendAlignment1 )
        //        {
        //            std::cerr << "\t  when Top Left --> wrong legend alignment" << std::endl;
        //        }
        //        if( !testLegendAlignment2 )
        //        {
        //            std::cerr << "\t  when Middle Center --> wrong legend alignment" << std::endl;
        //        }
        //        if( !testLegendAlignment3 )
        //        {
        //            std::cerr << "\t  when Bottom Right --> wrong legend alignment" << std::endl;
        //        }
        //        if( !testLegendHidden1 )
        //        {
        //            std::cerr << "\t  when m_plotSelected == Raw Data --> legend displayed" << std::endl;
        //        }
        //        if( !testLegendHidden2 )
        //        {
        //            std::cerr << "\t  when m_plotSelected != Raw Data --> legend not displayed" << std::endl;
        //        }
    }
    else
    {
        std::cerr << "Test_SetLegend() PASSED";
    }
    return testSetLegend_Passed;
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


    bool testSetMarkerType_Passed = testMarkerType1 && testMarkerType2 && testMarkerType3 && testMarkerType4 && testMarkerType5;
    if( !testSetMarkerType_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetMarkerType() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetMarkerType( QString markerType )" << std::endl;
        //        if( !testMarkerType1 )
        //        {
        //            std::cerr << "\t  vtkPlotPoints::CROSS not set" << std::endl;
        //        }
        //        if( !testMarkerType2 )
        //        {
        //            std::cerr << "\t  vtkPlotPoints::PLUS not set" << std::endl;
        //        }
        //        if( !testMarkerType3 )
        //        {
        //            std::cerr << "\t  vtkPlotPoints::SQUARE not set" << std::endl;
        //        }
        //        if( !testMarkerType4 )
        //        {
        //            std::cerr << "\t  vtkPlotPoints::CIRCLE not set" << std::endl;
        //        }
        //        if( !testMarkerType5 )
        //        {
        //            std::cerr << "\t  vtkPlotPoints::DIAMOND not set" << std::endl;
        //        }
    }
    else
    {
        std::cerr << "Test_SetMarkerType() PASSED";
    }
    return testSetMarkerType_Passed;
}



bool TestPlot::Test_UpdateCovariatesNames( QString dataDir, QString tempoDir )
{
    Plot plot;
    QString dirTest = tempoDir + "/TestPlot/Test_UpdateCovariatesNames";
    QDir().mkpath( dirTest );
    QFile::copy( dataDir + "/test_RawData_SUBMATRIX.csv", dirTest + "/test_RawData_SUBMATRIX.csv" );
    QMap< int, QString > expectedChangedAllCovariates;
    expectedChangedAllCovariates.insert( 0, "Intercept" );
    expectedChangedAllCovariates.insert( 1, "group" );
    expectedChangedAllCovariates.insert( 2, "Sex" );
    expectedChangedAllCovariates.insert( 3, "GestAgeAtBrith" );
    QMap< int, QString > expectedChangedCovariatesNoIntercept;
    expectedChangedCovariatesNoIntercept.insert( 1, "group" );
    expectedChangedCovariatesNoIntercept.insert( 2, "Sex" );
    expectedChangedCovariatesNoIntercept.insert( 3, "GestAgeAtBrith" );
    QMap< int, QString > expectedChangedBinaryCovariates;
    expectedChangedBinaryCovariates.insert( 1, "group" );
    expectedChangedBinaryCovariates.insert( 2, "Sex" );


    plot.m_directory = dirTest;
    plot.GetRawDataFiles();
    plot.SetCovariates();
    plot.UpdateCovariatesNames( expectedChangedBinaryCovariates );
    plot.UpdateCovariatesNames( expectedChangedCovariatesNoIntercept );

    bool testChangeAllCovariates = plot.m_allCovariates == expectedChangedAllCovariates;
    bool testChangeCovariatesNoIntercept = plot.m_covariatesNoIntercept == expectedChangedCovariatesNoIntercept;
    bool testChangeBinaryCovariates = plot.m_binaryCovariates == expectedChangedBinaryCovariates;


    bool testUpdateCovariatesNames_Passed = testChangeAllCovariates && testChangeCovariatesNoIntercept && testChangeBinaryCovariates;
    if( !testUpdateCovariatesNames_Passed )
    {
        std::cerr << "/!\\/!\\ Test_UpdateCovariatesNames() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with UpdateCovariatesNames( QMap< int, QString > newCovariateName )" << std::endl;
        //        if( !testChangeAllCovariates )
        //        {
        //            std::cerr << "\t  m_allCovariates not correctly changed" << std::endl;
        //        }
        //        if( !testChangeCovariatesNoIntercept )
        //        {
        //            std::cerr << "\t  m_covariatesNoIntercept not correctly changed" << std::endl;
        //        }
        //        if( !testChangeBinaryCovariates )
        //        {
        //            std::cerr << "\t  m_binaryCovariates not correctly changed" << std::endl;
        //        }
    }
    else
    {
        std::cerr << "Test_UpdateCovariatesNames() PASSED";
    }
    return testUpdateCovariatesNames_Passed;
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
    plot.SetSelectedProperty() = "FA";
    plot.SetSelectedCovariate() = "GENDER";
    plot.m_ordinate = plot.DataToDouble( processing.GetDataFromFile( faFilePath ) );
    plot.TransposeData( plot.m_ordinate, 1 ,0 );
    plot.m_ordinate.removeFirst();
    plot.SeparateBinary( temp0Bin, temp1Bin );

    bool testTemp0 = temp0Bin == expectedTemp0bin;
    bool testTemp1 = temp1Bin == expectedTemp1bin;


    bool testSeparateBinary_Passed = testTemp0 && testTemp1;
    if( !testSeparateBinary_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SeparateBinary() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SeparateBinary( QList< QList < double > > &temp0Bin, QList< QList < double > > &temp1Bin )" << std::endl;
        //        if( !testTemp0 )
        //        {
        //            std::cerr << "\t  0 data not separated correctly" << std::endl;
        //        }
        //        if( !testTemp1 )
        //        {
        //            std::cerr << "\t  1 data not separated correctly" << std::endl;
        //        }
    }
    else
    {
        std::cerr << "Test_SeparateBinary() PASSED";
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


    plot.m_nbrPoints = expectedMean.size();
    plot.GetMeanAndStdDv( temp0Bin, tempMean, tempStdDv );

    bool testMean = tempMean == expectedMean;
    bool testStdDv = tempStdDv == expectedStdDv;


    bool testGetMeanAndStdDv_Passed = testMean && testStdDv;
    if( !testGetMeanAndStdDv_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetMeanAndStdDv() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with GetMeanAndStdDv( QList< QList < double > > tempBin, QList < double > &tempMean,"
        //                     " QList < double > &tempStdDv )" << std::endl;
        //        if( !testMean )
        //        {
        //            std::cerr << "\t  wrong mean generated" << std::endl;
        //        }
        //        if( !testStdDv )
        //        {
        //            std::cerr << "\t  wrong standard deviation generated" << std::endl;
        //        }
    }
    else
    {
        std::cerr << "Test_GetMeanAndStdDv() PASSED";
    }
    return testGetMeanAndStdDv_Passed;
}

bool TestPlot::Test_GetMean()
{
    Plot plot;
    QList< QList < double > > rawData = QList< QList < double > >()
            << ( QList < double >() << 2 << 8 << 0 << 0 )
            << ( QList < double >() << 2 << 0 << 0 << 32 )
            << ( QList < double >() << 2 << 0 << 12 << 0 )
            << ( QList < double >() << 2 << 0 << 0 << 0 );
    QList< double >  mean;
    QList< double > expectedMean = QList< double >() << 2 << 2 << 3 << 8;


    plot.m_nbrPoints = rawData.first().size();
    mean = plot.GetMean( rawData );


    bool testGetMean_Passed = mean == expectedMean;
    if( !testGetMean_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetMean() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with GetMean( const QList< QList< double > >& rawData )" << std::endl;
    }
    else
    {
        std::cerr << "Test_GetMean() PASSED";
    }
    return testGetMean_Passed;
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


    plot.m_nbrPoints = expectedMean.size();
    plot.ProcessRawStats( temp0Bin, tempMean, tempUp, tempDown );

    bool testMean = tempMean == expectedMean;
    bool testUp = tempUp == expectedUp;
    bool testDown = tempDown == expectedDown;


    bool testProcessRawStats_Passed = testMean && testUp && testDown;
    if( !testProcessRawStats_Passed )
    {
        std::cerr << "/!\\/!\\ Test_ProcessRawStats() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with ProcessRawStats( QList< QList < double > > tempBin, QList < double > &tempBinMean,"
        //                     " QList < double > &tempBinUp, QList < double > &tempBinDown )" << std::endl;
        //        if( !testMean )
        //        {
        //            std::cerr << "\t  wrong mean generated" << std::endl;
        //        }
        //        if( !testUp )
        //        {
        //            std::cerr << "\t  wrong upper limit generated" << std::endl;
        //        }
        //        if( !testDown )
        //        {
        //            std::cerr << "\t  wrong lower limit generated" << std::endl;
        //        }
    }
    else
    {
        std::cerr << "Test_ProcessRawStats() PASSED";
    }
    return testProcessRawStats_Passed;
}

bool TestPlot::Test_SetSelectionToDisplayProperties( QString omnibusLpvalueFilePath )
{
    Plot plot;
    Processing processing;
    QMap< int, QPair< QString, QPair< bool, QString > > > selectionToDisplay;
    selectionToDisplay.insert( 0, QPair< QString, QPair< bool, QString > >( "Intercept", QPair< bool, QString >( true, "Red" ) ) );
    selectionToDisplay.insert( 1, QPair< QString, QPair< bool, QString > >( "GENDER", QPair< bool, QString >( false, "Lime" ) ) );
    selectionToDisplay.insert( 2, QPair< QString, QPair< bool, QString > >( "DaysPostBirth", QPair< bool, QString >( true, "Blue" ) ) );
    selectionToDisplay.insert( 3, QPair< QString, QPair< bool, QString > >( "ICV_y0", QPair< bool, QString >( false, "Yellow" ) ) );
    QList< QList < double > > expectedOrdinate = QList< QList < double > >()
            << (  QList < double >() << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0
                  << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0
                  << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0
                  << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 )
            << ( QList < double >() << 0.42433 << 0.022155 << 0.0051664 << 0.011978 << 0.017748 << 0.020539 << 0.053934 << 0.19373 << 0.18608 << 0.22124
                 << 0.3546 << 0.49296 << 0.60709 << 0.67003 << 0.65688 << 0.53028 << 0.28052 << 0.066579 << 0.0066949 << 0.00154 << 0.003504 << 0.014889
                 << 0.046893 << 0.10867 << 0.15115 << 0.12687 << 0.098 << 0.064371 << 0.033962 << 0.01373 << 0.0038786 << 0.00080909 << 0.00016981
                 << 5.3551e-05 << 5.5402e-05 << 0.00043792 << 0.0058056 << 0.030523 << 0.081162 << 0.16288 << 0.26543 << 0.36584 << 0.46101 << 0.54396
                 << 0.57806 << 0.53984 << 0.47443 << 0.43421 << 0.41056 << 0.36538 << 0.27477 << 0.17113 << 0.11706 << 0.11631 << 0.14407 << 0.18715
                 << 0.24997 << 0.33176 << 0.41012 << 0.46284 << 0.48603 << 0.49578 << 0.53769 << 0.60997 << 0.62247 << 0.5635 << 0.43685 << 0.26143
                 << 0.18669 << 0.18562 << 0.20461 << 0.18628 << 0.12946 << 0.11408 << 0.14275 << 0.11017 << 0.044633 << 0.041554 << 0.22627)
            << ( QList < double >() << 0.94004 << 0.82655 << 0.73728 << 0.67154 << 0.58057 << 0.45893 << 0.32605 << 0.29475 << 0.32767 << 0.30357
                 << 0.19631 << 0.15309 << 0.20123 << 0.31519 << 0.48187 << 0.69268 << 0.89998 << 0.98593 << 0.92225 << 0.87452 << 0.87551 << 0.79905
                 << 0.60172 << 0.2903 << 0.083271 << 0.05703 << 0.12427 << 0.19786 << 0.19729 << 0.20125 << 0.27525 << 0.36389 << 0.3789 << 0.30557
                 << 0.26575 << 0.419 << 0.58063 << 0.3731 << 0.15021 << 0.095776 << 0.080784 << 0.069636 << 0.070261 << 0.088405 << 0.14262 << 0.25082
                 << 0.43045 << 0.60261 << 0.66147 << 0.63367 << 0.56695 << 0.48756 << 0.44893 << 0.49061 << 0.58033 << 0.66452 << 0.7171 << 0.73483
                 << 0.73022 << 0.72955 << 0.75837 << 0.79045 << 0.75875 << 0.7239 << 0.73807 << 0.80359 << 0.88687 << 0.96278 << 0.99407 << 0.97285
                 << 0.92298 << 0.86497 << 0.83197 << 0.82812 << 0.84262 << 0.84291 << 0.64372 << 0.31529 << 0.33202 );
    QStringList expectedLineNames = QStringList() << "GENDER" << "DaysPostBirth" << "ICV_y0";
    QList< QList< int > > expectedLineColors = QList< QList< int > >() << ( QList< int >() << 0 << 255 << 0 ) << ( QList< int >() << 0 << 0 << 255 ) << ( QList< int >() << 255 << 200 << 0 );


    plot.m_ordinate = plot.DataToDouble( processing.GetDataFromFile( omnibusLpvalueFilePath ) );
    plot.TransposeData( plot.m_ordinate, 0, 0 );
    plot.SetSelectionToDisPlay() = selectionToDisplay;
    plot.m_selectionToDisplay.remove( plot.m_selectionToDisplay.firstKey() );
    plot.SetSelectionToDisplayProperties();

    bool testOrdinate = expectedOrdinate == plot.m_ordinate;
    bool testLineNames = expectedLineNames == plot.m_lineNames;
    bool testLineColors = expectedLineColors == plot.m_lineColors;


    bool testSetSelectionToDisplayProperties_Passed = testOrdinate && testLineNames && testLineColors;
    if( !testSetSelectionToDisplayProperties_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetSelectionToDisplayProperties() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetSelectionToDisplayProperties()" << std::endl;
        //        if( !testOrdinate )
        //        {
        //            std::cerr << "\t+ wrong data kept after updating m_ordinate with m_selectionToDisplay" << std::endl;
        //            DisplayError_QListQList( expectedOrdinate, plot.m_ordinate, "ordinate" );
        //        }
        //        if( !testLineNames )
        //        {
        //            std::cerr << "\t+ wrong data kept after updating m_ordinate with m_selectionToDisplay" << std::endl;
        //            DisplayError_QStringList( expectedLineNames, plot.m_lineNames, "line names" );
        //        }
        //        if( !testLineColors )
        //        {
        //            std::cerr << "\t+ wrong data kept after updating m_ordinate with m_selectionToDisplay" << std::endl;
        //            DisplayError_QListQList( expectedLineColors, plot.m_lineColors, "line colors" );
        //        }
    }
    else
    {
        std::cerr << "Test_SetSelectionToDisplayProperties() PASSED";
    }
    return testSetSelectionToDisplayProperties_Passed;
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


    bool testToLog10_Passed = dataTolog10 == expedtedDataTolog10;
    if( !testToLog10_Passed )
    {
        std::cerr << "/!\\/!\\ Test_Tolog10() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with ToLog10( QList< QList < double > > data )" << std::endl;
    }
    else
    {
        std::cerr << "Test_Tolog10() PASSED";
    }
    return testToLog10_Passed;
}



bool TestPlot::Test_LoadRawData( QString dataDir, QString tempoDir )
{
    Plot plot;
    QString dirTest = tempoDir + "/TestPlot/Test_LoadRawData";
    QDir().mkpath( dirTest );
    QFile::copy( dataDir + "/testSort_RawData_FA.csv", dirTest + "/testSort_RawData_FA.csv" );
    QList< QList < double > > expectedOrdinate = QList< QList < double > >()
            << ( QList < double >() << 0.101391 << 0.089874 << 0.0899232 << 0.106037 << 0.130884 )
            << ( QList < double >() << 0.100461 << 0.121609 << 0.140557 << 0.154739 << 0.165439 )
            << ( QList < double >() << 0.127887 << 0.120668 << 0.116923 << 0.12252 << 0.13898 )
            << ( QList < double >() << 0.135817 << 0.140971 << 0.145793 << 0.153154 << 0.163628 )
            << ( QList < double >() << 0.100568 << 0.109092 << 0.120203 << 0.13125 << 0.140868 );


    plot.m_directory = dirTest;
    plot.GetRawDataFiles();
    plot.SetSelectedProperty() = "FA";
    plot.m_ordinate = plot.LoadRawData();


    bool testLoadRawData_Passed = plot.m_ordinate == expectedOrdinate;
    if( !testLoadRawData_Passed )
    {
        std::cerr << "/!\\/!\\ Test_LoadRawData() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with LoadRawData()" << std::endl;
        //        DisplayError_QListQList( expectedOrdinate, plot.m_ordinate, "raw data loaded" );
    }
    else
    {
        std::cerr << "Test_LoadRawData() PASSED";
    }
    return testLoadRawData_Passed;
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
    plot.SetSelectedProperty() = "FA";
    plot.SetSelectedCovariate() = "GENDER";
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


    bool testLoadRawStats_Passed = plot.m_ordinate == expectedOrdinate;
    if( !testLoadRawStats_Passed )
    {
        std::cerr << "/!\\/!\\ Test_LoadRawStats() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with LoadRawStats()" << std::endl;
        //        DisplayError_QListQList( expectedOrdinate, plot.m_ordinate, "raw stats loaded" );
    }
    else
    {
        std::cerr << "Test_LoadRawStats() PASSED";
    }
    return testLoadRawStats_Passed;
}

bool TestPlot::Test_LoadBetas( QString dataDir, QString tempoDir )
{
    Plot plot;
    QString dirTest = tempoDir + "/TestPlot/Test_LoadBetas";
    QDir().mkpath( dirTest );
    QFile::copy( dataDir + "/test_Betas_FA.csv", dirTest + "/test_Betas_FA.csv" );
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
            << ( QList < double >() << 0.05691 << 0.060803 << 0.06531 << 0.070412 << 0.076043 << 0.082126 << 0.088623 << 0.095532 << 0.10283
                 << 0.11034 << 0.11774 << 0.12448 << 0.13002 << 0.13382 << 0.13549 << 0.13481 << 0.13175 << 0.1265 << 0.11944 << 0.11116 << 0.10241
                 << 0.094048 << 0.086846 << 0.081379 << 0.077899 << 0.076351 << 0.076481 << 0.07796 << 0.080441 << 0.083556 << 0.08693 << 0.09029
                 << 0.09366 << 0.097488 << 0.10259 << 0.10981 << 0.11963 << 0.1317 << 0.14488 << 0.15758 << 0.16838 << 0.1766 << 0.18248 << 0.18686
                 << 0.19069 << 0.19462 << 0.19879 << 0.20295 << 0.20667 << 0.20951 << 0.21109 << 0.21111 << 0.20949 << 0.20635 << 0.2021 << 0.19723
                 << 0.19216 << 0.18702 << 0.18159 << 0.17546 << 0.16828 << 0.16009 << 0.15129 << 0.14252 << 0.1343 << 0.12683 << 0.12 << 0.11352
                 << 0.10714 << 0.10076 << 0.094387 << 0.088147 << 0.082266 << 0.077046 << 0.072797 << 0.06971 << 0.067772 << 0.066745 << 0.06624 )
            << ( QList < double >() << 0.0020391 << 0.004916 << 0.0066412 << 0.0072202 << 0.0068241 << 0.0057568 << 0.0043697 << 0.0029498 << 0.0016436 << 0.00047481
                 << -0.00056549 << -0.0014505 << -0.0021154 << -0.002479 << -0.0024539 << -0.0019674 << -0.0010076 << 0.00034182 << 0.0019112 << 0.0034908
                 << 0.0048857 << 0.0059693 << 0.0067164 << 0.0072016 << 0.0075721 << 0.0080139 << 0.0087013 << 0.0097065 << 0.010886 << 0.011829
                 << 0.011975 << 0.010896 << 0.0085982 << 0.0055898 << 0.0026212 << 0.00024992 << -0.0014404 << -0.0027416 << -0.0039521 << -0.0050873
                 << -0.0058968 << -0.006083 << -0.0054818 << -0.0041217 << -0.0022256 << -0.00016911 << 0.0016491 << 0.0030008 << 0.0039446 << 0.0047553
                 << 0.0057167 << 0.0069428 << 0.0083482 << 0.0097426 << 0.010931 << 0.011754 << 0.01208 << 0.011822 << 0.010968 << 0.0096046
                 << 0.0079188 << 0.006182 << 0.0047188 << 0.0038556 << 0.0038345 << 0.0047113 << 0.0062915 << 0.0081613 << 0.0098252 << 0.010898
                 << 0.011251 << 0.01104 << 0.010609 << 0.010337 << 0.010522 << 0.011324 << 0.012793 << 0.014898 << 0.017575 )
            << ( QList < double >() << -0.0011137 << -0.0020195 << -0.0025759 << -0.0026584 << -0.0021895 << -0.0011818 << 0.00023219
                 << 0.0018069 << 0.0032472 << 0.0043166 << 0.0049365 << 0.0052121 << 0.0053511 << 0.0055006 << 0.0056043 << 0.0053937
                 << 0.0045306 << 0.0028062 << 0.00027067 << -0.0027614 << -0.0058085 << -0.0083778 << -0.010134 << -0.011027 << -0.011313
                 << -0.011464 << -0.011985 << -0.013206 << -0.015119 << -0.017318 << -0.019088 << -0.019627 << -0.01831 << -0.014896
                 << -0.0096305 << -0.0032148 << 0.0033922 << 0.0092868 << 0.013866 << 0.016855 << 0.018172 << 0.017843 << 0.016047 << 0.013194
                 << 0.0098876 << 0.0067644 << 0.0043081 << 0.0027264 << 0.0019289 << 0.0016116 << 0.0014205 << 0.0011236 << 0.00070222
                 << 0.00031744 << 0.00017271 << 0.00035411 << 0.00074522 << 0.0010643 << 0.00099883 << 0.00036933 << -0.0007551 << -0.002061
                 << -0.0031315 << -0.0036463 << -0.0035073 << -0.0028209 << -0.0017922 << -0.00063317 << 0.0004757 << 0.0014047 << 0.0021019
                 << 0.0026113 << 0.0030566 << 0.0036038 << 0.0044325 << 0.0057314 << 0.0077012 << 0.010542 << 0.014414 );


    plot.m_matlabDirectory = dirTest;
    plot.GetBetaFiles();
    plot.SetSelectedProperty() = "FA";
    plot.SetSelectionToDisPlay() = selectionToDisplay;
    plot.m_ordinate = plot.LoadBetas();


    bool testLoadBetas_Passed = plot.m_ordinate == expectedOrdinate;
    if( !testLoadBetas_Passed )
    {
        std::cerr << "/!\\/!\\ Test_LoadBetas() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with LoadBetas()" << std::endl;
        //        DisplayError_QListQList( expectedOrdinate, plot.m_ordinate, "betas loaded" );
    }
    else
    {
        std::cerr << "Test_LoadBetas() PASSED";
    }
    return testLoadBetas_Passed;
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
    plot.SetSelectedCovariate() = "DaysPostBirth";

    expectedOrdinate.append( plot.m_dataBeta.value( "RD" ).at( plot.m_allCovariates.key( plot.m_covariateSelected ) ) );
    expectedOrdinate.append( plot.m_dataBeta.value( "FA" ).at( plot.m_allCovariates.key( plot.m_covariateSelected ) ) );

    plot.SetSelectionToDisPlay() = selectionToDisplay;
    plot.m_ordinate = plot.LoadBetaByCovariate();


    bool testLoadBetaByCovariate_Passed = plot.m_ordinate == expectedOrdinate;
    if( !testLoadBetaByCovariate_Passed )
    {
        std::cerr << "/!\\/!\\ Test_LoadBetaByCovariate() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with LoadBetaByCovariate()" << std::endl;
        //        DisplayError_QListQList( expectedOrdinate, plot.m_ordinate, "beta by covariates loaded" );
    }
    else
    {
        std::cerr << "Test_LoadBetaByCovariate() PASSED";
    }
    return testLoadBetaByCovariate_Passed;
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
    plot.SetSelectionToDisPlay() = selectionToDisplay;
    plot.m_ordinate = plot.LoadOmnibusLpvalues( plot.m_dataOmnibusLpvalue );
    expectedOrdinate = plot.m_dataOmnibusLpvalue;
    expectedOrdinate = plot.ToLog10( expectedOrdinate );


    bool testLoadOmnibusLpvalues_Passed = plot.m_ordinate == expectedOrdinate;
    if( !testLoadOmnibusLpvalues_Passed )
    {
        std::cerr << "/!\\/!\\ Test_LoadOmnibusLpvalues() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with LoadOmnibusLpvalues( QList< QList < double > > omnibusLpvalues )" << std::endl;
        //        DisplayError_QListQList( expectedOrdinate, plot.m_ordinate, "omnibus Lpvalues loaded" );
    }
    else
    {
        std::cerr << "Test_LoadOmnibusLpvalues() PASSED";
    }
    return testLoadOmnibusLpvalues_Passed;
}

bool TestPlot::Test_LoadConfidenceBand( QString dataDir, QString tempoDir )
{
    Plot plot;
    QString dirTest = tempoDir + "/TestPlot/Test_LoadConfidenceBand";
    QDir().mkpath( dirTest );
    QFile::copy( dataDir + "/test_Betas_FA.csv", dirTest + "/test_Betas_FA.csv" );
    QFile::copy( dataDir + "/test_Omnibus_ConfidenceBands_FA.csv", dirTest + "/test_Omnibus_ConfidenceBands_FA.csv" );
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


    plot.m_matlabDirectory = dirTest;
    plot.GetBetaFiles();
    plot.GetConfidenceBandsFiles();
    plot.m_allCovariates.insert( 0, "Intercept" );
    plot.m_allCovariates.insert( 1, "GENDER" );
    plot.m_allCovariates.insert( 2, "DaysPostBirth" );
    plot.m_allCovariates.insert( 3, "ICV_y0" );
    plot.SetSelectedProperty() = "FA";
    plot.SetSelectedCovariate() = "DaysPostBirth";
    plot.m_ordinate = plot.LoadConfidenceBand();


    bool testLoadConfidenceBand_Passed = plot.m_ordinate == expectedOrdinate;
    if( !testLoadConfidenceBand_Passed )
    {
        std::cerr << "/!\\/!\\ Test_LoadConfidenceBand() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with LoadConfidenceBand()" << std::endl;
        //        DisplayError_QListQList( expectedOrdinate, plot.m_ordinate, "confidence bands loaded" );
    }
    else
    {
        std::cerr << "LoadConfidenceBand() PASSED";
    }
    return testLoadConfidenceBand_Passed;
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
    plot.SetSelectedCovariate() = "DaysPostBirth";

    expectedOrdinate.append( plot.m_dataPostHocFDRLpvalue.value( "RD" ).at( plot.m_covariatesNoIntercept.key( plot.m_covariateSelected ) - 1 ) );
    expectedOrdinate.append( plot.m_dataPostHocFDRLpvalue.value( "FA" ).at( plot.m_covariatesNoIntercept.key( plot.m_covariateSelected ) - 1 ) );
    expectedOrdinate = plot.ToLog10( expectedOrdinate );

    plot.SetSelectionToDisPlay() = selectionToDisplay;
    plot.m_ordinate = plot.LoadPostHocFDRLpvalues();


    bool testLoadPostHocFDRLpvalues_Passed = plot.m_ordinate == expectedOrdinate;
    if( !testLoadPostHocFDRLpvalues_Passed )
    {
        std::cerr << "/!\\/!\\ Test_LoadPostHocFDRLpvalues() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with LoadPostHocFDRLpvalues()" << std::endl;
        //        DisplayError_QListQList( expectedOrdinate, plot.m_ordinate, "post-hoc FDR Lpvalues loaded" );
    }
    else
    {
        std::cerr << "Test_LoadPostHocFDRLpvalues() PASSED";
    }
    return testLoadPostHocFDRLpvalues_Passed;
}



bool TestPlot::Test_AddEntriesRawData()
{
    Plot plot;
    bool testAddEntriesRawData_Passed = true;
    vtkSmartPointer< vtkTable > table = vtkSmartPointer< vtkTable >::New();
    QStringList tableEntries;


    plot.m_subjects = QStringList() << "Stan" << "Kyle" << "Kenny" << "Cartman" << "Butters";
    plot.m_nbrPlots = plot.m_subjects.size();
    plot.AddEntriesRawData( table );

    for( int i = 0; i < table->GetNumberOfColumns(); i++ )
    {
        testAddEntriesRawData_Passed = testAddEntriesRawData_Passed && ( table->GetColumn( i )->GetName() == plot.m_subjects.at( i ) );
        tableEntries.append( table->GetColumn( i )->GetName() );
    }


    if( !testAddEntriesRawData_Passed )
    {
        std::cerr << "/!\\/!\\ Test_AddEntriesRawData() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with AddEntriesRawData( vtkSmartPointer< vtkTable > &table )" << std::endl;
        //        DisplayError_QStringList( plot.m_subjects, tableEntries, "entries" );
    }
    else
    {
        std::cerr << "Test_AddEntriesRawData() PASSED";
    }
    return testAddEntriesRawData_Passed;
}

bool TestPlot::Test_AddEntriesRawStats()
{
    Plot plot;
    bool testAddEntriesRawStats_Passed = true;
    vtkSmartPointer< vtkTable > table = vtkSmartPointer< vtkTable >::New();
    QStringList expectedEntries = QStringList() << "GENDER = 0 std+" << "GENDER = 0 mean" << "GENDER = 0 std-" << "GENDER = 1 std+" << "GENDER = 1 mean" << "GENDER = 1 std-";
    QStringList tableEntries;


    plot.SetSelectedCovariate() = "GENDER";
    plot.AddEntriesRawStats( table );

    for( int i = 0; i < table->GetNumberOfColumns(); i++ )
    {
        testAddEntriesRawStats_Passed = testAddEntriesRawStats_Passed && ( table->GetColumn( i )->GetName() == expectedEntries.at( i ) );
        tableEntries.append( table->GetColumn( i )->GetName() );
    }


    if( !testAddEntriesRawStats_Passed )
    {
        std::cerr << "/!\\/!\\ Test_AddEntriesRawStats() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with AddEntriesRawStats( vtkSmartPointer< vtkTable > &table )" << std::endl;
        //        DisplayError_QStringList( expectedEntries, tableEntries, "entries" );
    }
    else
    {
        std::cerr << "Test_AddEntriesRawStats() PASSED";
    }
    return testAddEntriesRawStats_Passed;
}

bool TestPlot::Test_AddEntriesByPropertiesOrCovariates()
{
    Plot plot;
    bool testAddEntriesByPropertiesOrCovariates_Passed = true;
    vtkSmartPointer< vtkTable > table = vtkSmartPointer< vtkTable >::New();
    QStringList tableEntries;


    plot.m_lineNames = QStringList() << "Intercept" << "GENDER" << "DaysPostBirth" << "ICV_y0";
    plot.m_nbrPlots = plot.m_lineNames.size();
    plot.AddEntriesByPropertiesOrCovariates( table );

    for( int i = 0; i < table->GetNumberOfColumns(); i++ )
    {
        testAddEntriesByPropertiesOrCovariates_Passed = testAddEntriesByPropertiesOrCovariates_Passed && ( table->GetColumn( i )->GetName() == plot.m_lineNames.at( i ) );
        tableEntries.append( table->GetColumn( i )->GetName() );
    }


    if( !testAddEntriesByPropertiesOrCovariates_Passed )
    {
        std::cerr << "/!\\/!\\ Test_AddEntriesByPropertiesOrCovariates() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with AddEntriesByPropertiesOrCovariates( vtkSmartPointer< vtkTable > &table )" << std::endl;
        //        DisplayError_QStringList( plot.m_lineNames, tableEntries, "entries" );
    }
    else
    {
        std::cerr << "Test_AddEntriesByPropertiesOrCovariates() PASSED";
    }
    return testAddEntriesByPropertiesOrCovariates_Passed;
}

bool TestPlot::Test_AddEntriesConfidenceBands()
{
    Plot plot;
    bool testAddEntriesConfidenceBands_Passed = true;
    vtkSmartPointer< vtkTable > table = vtkSmartPointer< vtkTable >::New();
    QStringList expectedEntries = QStringList() << "Upper Confidence Band" << "Mean Betas" << "Lower Confidence Band";
    QStringList tableEntries;


    plot.AddEntriesConfidenceBands( table );

    for( int i = 0; i < table->GetNumberOfColumns(); i++ )
    {
        testAddEntriesConfidenceBands_Passed = testAddEntriesConfidenceBands_Passed && ( table->GetColumn( i )->GetName() == expectedEntries.at( i ) );
        tableEntries.append( table->GetColumn( i )->GetName() );
    }


    if( !testAddEntriesConfidenceBands_Passed )
    {
        std::cerr << "/!\\/!\\ Test_AddEntriesConfidenceBands() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with AddEntriesConfidenceBands( vtkSmartPointer< vtkTable > &table )" << std::endl;
        //        DisplayError_QStringList( expectedEntries, tableEntries, "entries" );
    }
    else
    {
        std::cerr << "Test_AddEntriesConfidenceBands() PASSED";
    }
    return testAddEntriesConfidenceBands_Passed;
}



bool TestPlot::Test_SetData()
{
    Plot plot;
    bool testSetData_Passed = true;
    vtkSmartPointer< vtkTable > table = vtkSmartPointer< vtkTable >::New();
    QList< QList< double > > dataTable;


    plot.m_subjects = QStringList() << "Stan" << "Kyle" << "Kenny";
    plot.m_abscissa = QList< double >() << -3 << -2 << -1;
    plot.m_ordinate = QList< QList< double > >()
            << ( QList< double >() << 1 << 2 << 3 )
            << ( QList< double >() << 4 << 5 << 6 )
            << ( QList< double >() << 7 << 8 << 9 );
    QList< QList< double > > expectedTable = QList< QList< double > >() << plot.m_abscissa << plot.m_ordinate;
    plot.m_nbrPoints = plot.m_abscissa.size();
    plot.m_nbrPlots = plot.m_ordinate.size();
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
                testSetData_Passed = testSetData_Passed && ( table->GetValue( j, i ).ToDouble() == plot.m_abscissa.at( j ) );
            }
            else
            {
                testSetData_Passed = testSetData_Passed && ( table->GetValue( j, i ).ToDouble() == plot.m_ordinate.at( i - 1 ).at( j ) );
            }
        }
        dataTable.append( currentDataRow );
    }


    if( !testSetData_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SetData() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with SetData( vtkSmartPointer< vtkTable > &table )" << std::endl;
        //        DisplayError_QListQList( expectedTable, dataTable, "table" );
    }
    else
    {
        std::cerr << "Test_SetData() PASSED";
    }
    return testSetData_Passed;
}



bool TestPlot::Test_ApplyPearsonCorrelation()
{
    Plot plot;
    QList< QList < double > > rawData = QList< QList < double > >()
            << ( QList < double >() << 2 << 2 << 3 << 8 << 0 )
            << ( QList < double >() << 0 << 0 << 3 << 0 << 0 );
    QList< double > coefficientsExpected = QList< double >() << 1 << 0;
    QList< double > mean = QList< double >() << 2 << 2 << 3 << 8;
    QList< double > coefficients;


    plot.m_dataRawData.insert( "FA", rawData );
    plot.m_propertySelected = "FA";
    plot.m_nbrPoints = rawData.first().size();

    for( int i = 0; i < rawData.size(); i++ )
    {
        coefficients.append( plot.ApplyPearsonCorrelation( i, mean ) );
    }


    bool testApplyPearsonCorrelation_Passed = coefficients == coefficientsExpected;
    if( !testApplyPearsonCorrelation_Passed )
    {
        std::cerr << "/!\\/!\\ Test_ApplyPearsonCorrelation() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with ApplyPearsonCorrelation( int indexLine, QList< double > meanRawData ))" << std::endl;
    }
    else
    {
        std::cerr << "Test_ApplyPearsonCorrelation() PASSED";
    }
    return testApplyPearsonCorrelation_Passed;
}

bool TestPlot::Test_InitLines()
{
    Plot plot;
    int nbrPlot = 5;

    plot.m_nbrPlots = nbrPlot;
    plot.m_chart = vtkSmartPointer< vtkChartXY >::New();
    plot.InitLines();


    bool testInitLines_Passed = plot.m_chart->GetNumberOfPlots() == plot.m_nbrPlots;
    if( !testInitLines_Passed )
    {
        std::cerr << "/!\\/!\\ Test_InitLines() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with InitLines()" << std::endl;
        //        std::cerr << "\t  " << plot.m_nbrPlots << " plots expected | " << plot.m_chart->GetNumberOfPlots() << " plots set" << std::endl;
    }
    else
    {
        std::cerr << "Test_InitLines() PASSED";
    }
    return testInitLines_Passed;
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
    plot.m_nbrPoints = plot.m_abscissa.size();
    plot.SetLineWidth() = lineWidth;
    plot.m_chart = vtkSmartPointer< vtkChartXY >::New();
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


    bool testAddSignificantLevel_Passed = testNbrPlots && testSignificantLevel && testLineWidth;
    if( !testAddSignificantLevel_Passed )
    {
        std::cerr << "/!\\/!\\ Test_AddSignificantLevel() FAILED /!\\/!\\";
        //        std::cerr << std::endl << "\t+ pb with AddSignificantLevel( double significantLevel )" << std::endl;
        //        if( !testNbrPlots )
        //        {
        //            std::cerr << "\t+ significant level not added to m_chart" << std::endl;
        //        }
        //        if( !testSignificantLevel )
        //        {
        //            std::cerr << "\t+ wrong significant level added to m_chart" << std::endl;
        //            std::cerr << "\t  significant level expected: " << significantLevel << " | significant level set: " << dataTable.last().last() << std::endl;
        //        }
        //        if( !testLineWidth )
        //        {
        //            std::cerr << "\t+ wrong line width" << std::endl;
        //            std::cerr << "\t  line width expected: " << lineWidth / 2 << " | line width set: " << plot.m_chart->GetPlot( indexLastPlot )->GetWidth() << std::endl;
        //        }
    }
    else
    {
        std::cerr << "Test_AddSignificantLevel() PASSED";
    }
    return testAddSignificantLevel_Passed;
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
    double expectedYMinMax[ 2 ] = { -10, 5.5 };

    plot.m_plotSelected = "Raw Data";
    plot.m_yMin.first = false;
    plot.m_yMin.second = -5.5;
    plot.m_yMax.first = true;
    plot.m_yMax.second = 5.5;
    plot.m_ordinate = ordinate;
    plot.GetyMinMax();


    bool testGetyMinMax_Passed = plot.m_yMinMax[ 0 ] == expectedYMinMax[ 0 ] && plot.m_yMinMax[ 1 ] == expectedYMinMax[ 1 ];
    if( !testGetyMinMax_Passed )
    {
        std::cerr << "/!\\/!\\ Test_GetyMinMax() FAILED /!\\/!\\";
        std::cerr << std::endl << "\t+ pb with GetyMinMax()" << std::endl;
        std::cerr << "\t  yMinMax expected: {" << plot.m_yMinMax[ 0 ] << "," << plot.m_yMinMax[ 1 ] << "} | "
                  << "yMinMax set: {" << expectedYMinMax[ 0 ] << "," << expectedYMinMax[ 1 ] << "}" << std::endl;
    }
    else
    {
        std::cerr << "Test_GetyMinMax() PASSED";
    }
    return testGetyMinMax_Passed;
}



bool TestPlot::Test_SavePlot( QString plotPath, QString dataDir, QString tempoDir )
{
    Plot plot;
    QSharedPointer< QVTKWidget > qvtkWidget = QSharedPointer< QVTKWidget >( new QVTKWidget() );
    QString dirTest = tempoDir + "/TestPlot/Test_SavePlot/FADTTSter_test";
    QString matlabDir = dirTest + "/MatlabOutputs";
    QDir().mkpath( dirTest );
    QDir().mkpath( matlabDir );
    QFile::copy( dataDir + "/test_RawData_FA.csv", dirTest + "/test_RawData_FA.csv" );
    QFile::copy( dataDir + "/test_RawData_RD.csv", dirTest + "/test_RawData_RD.csv" );
    QFile::copy( dataDir + "/test_RawData_SUBMATRIX.csv", dirTest + "/test_RawData_SUBMATRIX.csv" );
    QFile::copy( dataDir + "/test_Betas_FA.csv", matlabDir + "/test_Betas_FA.csv" );
    QFile::copy( dataDir + "/test_Betas_RD.csv", matlabDir + "/test_Betas_RD.csv" );
    QFile::copy( dataDir + "/test_Omnibus_FDR_Local_pvalues.csv", matlabDir + "/test_Omnibus_FDR_Local_pvalues.csv" );
    QFile::copy( dataDir + "/test_PostHoc_FDR_Local_pvalues_FA.csv", matlabDir + "/test_PostHoc_FDR_Local_pvalues_FA.csv" );
    QFile::copy( dataDir + "/test_PostHoc_FDR_Local_pvalues_RD.csv", matlabDir + "/test_PostHoc_FDR_Local_pvalues_RD.csv" );
    QMap<int, QPair< QString, QPair< bool, QString > > > linesToDisplay;
    linesToDisplay.insert( 0, QPair< QString, QPair< bool, QString > >( "Intercept", QPair< bool, QString >( true, "Red" ) ) );
    linesToDisplay.insert( 1, QPair< QString, QPair< bool, QString > >( "ngroup", QPair< bool, QString >( true, "Lime" ) ) );
    linesToDisplay.insert( 2, QPair< QString, QPair< bool, QString > >( "GENDER", QPair< bool, QString >( true, "Blue" ) ) );
    linesToDisplay.insert( 3, QPair< QString, QPair< bool, QString > >( "GestAgeAtBirth", QPair< bool, QString >( true, "Yellow" ) ) );


    plot.SetQVTKWidget( qvtkWidget );
    plot.InitPlot( dirTest, "test" );
    plot.SetSelectedPlot( "Raw Betas by Properties" );
    plot.SetSelectedProperty() = "FA";
    plot.SetSelectionToDisPlay() = linesToDisplay;
    plot.SetDefaultTitle();
    plot.SetDefaultAxis();
    plot.SetLegend( true, "Top Right" );
    plot.SetPvalueThreshold() = 0.05;
    plot.SetLineWidth() = 1.50;
    plot.SetMarkerType( "Circle" );
    plot.SetMarkerSize() = 8.00;
    plot.DisplayPlot();
    plot.SavePlot( matlabDir + "/test_Betas_FA.eps" );

    //    qDebug() << dirTest + "/test_Betas_FA.eps" << endl << GetHashFile( dirTest + "/test_Betas_FA.eps" );
    //    qDebug() << plotPath << endl << GetHashFile( plotPath );


    bool testSavePlot_Passed = CompareFile( matlabDir + "/test_Betas_FA.eps", plotPath );
    if( !testSavePlot_Passed )
    {
        std::cerr << "/!\\/!\\ Test_SavePlot() FAILED /!\\/!\\";
        //        std::cerr << endl << "\t+ pb with SavePlot( QString filePath )" << std::endl;
    }
    else
    {
        std::cerr << "Test_SavePlot() PASSED";
    }
    return testSavePlot_Passed;
}


/**********************************************************************/
/********************** Functions Used For Testing ********************/
/**********************************************************************/
void TestPlot::DisplayError_QStringList( QStringList qStringListExpected, QStringList qStringListDisplayed, QString type )
{
    std::cerr << "\t  - " << type.toStdString() << " expected:" << std::endl;
    for( int i = 0; i < qStringListExpected.size(); i++ )
    {
        std::cerr << "\t      " << qStringListExpected.at( i ).toStdString() << std::endl;
    }
    std::cerr << "\t  - " << type.toStdString() << " displayed:" << std::endl;
    for( int i = 0; i < qStringListDisplayed.size(); i++ )
    {
        std::cerr << "\t      " << qStringListDisplayed.at( i ).toStdString() << std::endl;
    }
}

void TestPlot::DisplayError_QMapIntQString( QMap< int, QString > qMapExpected, QMap< int, QString > qMapDisplayed, QString type )
{
    std::cerr << "\t  - " << type.toStdString() << " expected:" << std::endl;
    QMap< int, QString >::ConstIterator iterQMapExpected = qMapExpected.cbegin();
    while( iterQMapExpected != qMapExpected.cend() )
    {
        std::cerr << "\t       " << iterQMapExpected.value().toStdString() << " at " << iterQMapExpected.key() << std::endl;
        ++iterQMapExpected;
    }
    std::cerr << "\t  - " << type.toStdString() << " displayed:" << std::endl;
    QMap< int, QString >::ConstIterator iterQMapDisplayed = qMapDisplayed.cbegin();
    while( iterQMapDisplayed != qMapDisplayed.cend() )
    {
        std::cerr << "\t       " << iterQMapDisplayed.value().toStdString() << " at " << iterQMapDisplayed.key() << std::endl;
        ++iterQMapDisplayed;
    }
}

template <typename T>
void TestPlot::DisplayError_QListQList( T qListQListExpected, T qListQListDisplayed, QString type )
{
    std::cerr << "\t  - " << type.toStdString() << " expected:" << std::endl;
    for( int i = 0; i < qListQListExpected.size(); i++ )
    {
        std::cerr << "\t      ";
        for( int j = 0; j < qListQListExpected.first().size(); j++ )
        {
            std::cerr << qListQListExpected.at( i ).at( j ) << ", ";
        }
        std::cerr << std::endl;
    }
    std::cerr << "\t  - " << type.toStdString() << " displayed:" << std::endl;
    for( int i = 0; i < qListQListDisplayed.size(); i++ )
    {
        std::cerr << "\t      ";
        for( int j = 0; j < qListQListDisplayed.first().size(); j++ )
        {
            std::cerr << qListQListDisplayed.at( i ).at( j ) << ", ";
        }
        std::cerr << std::endl;
    }
}


QByteArray TestPlot::GetHashFile( QString filePath )
{
    QCryptographicHash hash( QCryptographicHash::Sha1 );
    QFile file( filePath );

    if( file.open( QIODevice::ReadOnly ) )
    {
        hash.addData( file.readAll() );
        file.close();
    }
    else
    {
        std::cerr << "Cannot open file: " << filePath.toStdString() << std::endl;
    }

    return hash.result().toHex();
}

bool TestPlot::CompareFile( QString filePath1, QString filePath2 )
{
    QByteArray sig1 = GetHashFile( filePath1 );
    QByteArray sig2 = GetHashFile( filePath2 );

    if( sig1 == sig2 )
    {
        return true;
    }
    else
    {
        return false;
    }
}
