#include "TestInfoDialog.h"


TestInfoDialog::TestInfoDialog()
{
}

/**********************************************************************/
/*************************** Test Functions ***************************/
/**********************************************************************/
bool TestInfoDialog::Test_DisplayInfo( QString inputADFile, QString inputCOMPFile )
{
    int argc = 0;
    char **argv = 0;
    QApplication *app = new QApplication( argc, argv );
    Processing process;

    QList<QStringList> inputADData = process.GetDataFromFile( inputADFile );
    QList<QStringList> inputCOMPData = process.GetDataFromFile( inputCOMPFile );

    Data data;
    data.InitData();
    data.SetFilename( "ad" ) = inputADFile;
    data.SetNbrRows( "ad" ) = inputADData.size()-1;
    data.SetNbrColumns( "ad" ) = inputADData.at( 0 ).size();
    data.SetNbrSubjects( "ad" ) = inputADData.at( 0 ).size()-1;

    data.SetFilename( "COMP" ) = inputCOMPFile;
    data.SetNbrRows( "COMP" ) = inputCOMPData.size()-1;
    data.SetNbrColumns( "COMP" ) = inputCOMPData.at( 0 ).size()-1;
    data.SetNbrSubjects( "COMP" ) = inputCOMPData.size()-1;
    for( int c = 0; c < inputCOMPData.at( 0 ).size(); c++ )
    {
        if( c != data.GetSubjectColumnID() )
        {
            data.AddCovariate( c, inputCOMPData.at( 0 ).at( c ) );
        }
    }
    data.AddCovariate( -1, "Intercept" );

    QSharedPointer<InfoDialog> infoDialog = QSharedPointer<InfoDialog>( new InfoDialog );
    infoDialog->SetData( &data );
    infoDialog->DisplayInfo();

    QString expectedADFileLabel;
    expectedADFileLabel.append( "<center><b>AD File</b></center><br>" );
    expectedADFileLabel.append( "<b>Filename</b> " + QFileInfo( QFile( inputADFile ) ).fileName() + "<br>"  );
    expectedADFileLabel.append( "<b>Number of test subjects</b>  " + QString::number( inputADData.at( 0 ).size()-1 ) + "<br>" );
    expectedADFileLabel.append( "<b>Data matrix</b>  " + QString::number( inputADData.size()-1 ) + "x" + QString::number( inputADData.at( 0 ).size() ) + "<br>" );

    bool testADFileLabel = ( expectedADFileLabel == infoDialog->m_adFileInfo_label->text() );


    QString expectedNoFileLabel;
    expectedNoFileLabel.append( "<center><b>RD File</b></center><br>" );
    expectedNoFileLabel.append( "<i>No File Information.<br>Please select a correct data file</i>" );

    bool testNoFileLabel = ( expectedNoFileLabel ==  infoDialog->m_rdFileInfo_label->text() );


    QString expectedCOMPFileLabel;
    expectedCOMPFileLabel.append( "<center><b>COMP File</b></center><br>" );
    expectedCOMPFileLabel.append( "<b>Filename</b> " + QFileInfo( QFile( inputCOMPFile ) ).fileName() + "<br>"  );
    expectedCOMPFileLabel.append( "<b>Number of test subjects</b>  " + QString::number( inputCOMPData.size()-1 ) + "<br>" );
    expectedCOMPFileLabel.append( "<b>Data matrix</b>  " + QString::number( inputCOMPData.size()-1 ) + "x" + QString::number( inputCOMPData.at( 0 ).size()-1 ) + "<br>" );
    expectedCOMPFileLabel.append( "<b>Number of covariates</b>  " + QString::number( inputCOMPData.at( 0 ).size()-1 ) );
    for( int c = 0; c < inputCOMPData.at( 0 ).size(); c++ )
    {
        if( c != data.GetSubjectColumnID() )
        {
            expectedCOMPFileLabel.append ("<br>-  " + inputCOMPData.at( 0 ).at( c ) );
        }
    }

    bool testCOMPFileLabel = expectedCOMPFileLabel ==  infoDialog->m_compFileInfo_label->text();


    if( !testADFileLabel || !testNoFileLabel || !testCOMPFileLabel )
    {
        std::cerr << std::endl << "Test_DisplayInfo() FAILED:" << std::endl;
        if( !testADFileLabel )
        {
            std::cerr << "\t   Wrong info displayed for AD file" << std::endl;
            DisplayErrorFileInfoDisplayed( expectedADFileLabel, infoDialog->m_adFileInfo_label->text() );
        }
        if( !testNoFileLabel )
        {
            std::cerr << "\t   Wrong info displayed when no file" << std::endl;
            DisplayErrorFileInfoDisplayed( expectedNoFileLabel, infoDialog->m_rdFileInfo_label->text() );
        }
        if( !testCOMPFileLabel )
        {
            std::cerr << "\t   Wrong info displayed for covariate file" << std::endl;
            DisplayErrorFileInfoDisplayed( expectedCOMPFileLabel, infoDialog->m_compFileInfo_label->text() );
        }
    }
    else
    {
        std::cout << std::endl << "Test_DisplayInfo() PASSED" << std::endl;
    }

    app->exit();
    return ( testADFileLabel & testNoFileLabel & testCOMPFileLabel );
}


/**********************************************************************/
/********************** Functions Used For Testing ********************/
/**********************************************************************/
void TestInfoDialog::DisplayErrorFileInfoDisplayed( QString infoExpected, QString infoDisplayed )
{
    std::cerr << "\t\t- Info expected" << std::endl;
    std::cerr << "\t\t  " << infoExpected.toStdString() << std::endl;
    std::cerr << "\t\t- Info displayed" << std::endl;
    std::cerr << "\t\t  " << infoDisplayed.toStdString() << std::endl;
}
