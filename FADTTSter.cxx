#include "FADTTSWindow.h"
#include "FADTTS_noGUI.h"
#include <QApplication>
#include "FADTTSterCLP.h"

#include <QDebug>

int main( int argc, char *argv[] )
{
    PARSE_ARGS;
    Q_INIT_RESOURCE( FADTTS_Resources );

    if( !noGUI )
    {
        QApplication app( argc , argv );

        FADTTSWindow fadttsWindow;
        fadttsWindow.show();

        if( !paraConfig.empty() )
        {
            fadttsWindow.LoadParaConfiguration( paraConfig.data() );
        }
        if( !softConfig.empty() )
        {
            fadttsWindow.LoadSoftConfiguration( softConfig.data() );
        }

        return app.exec();
    }
    else
    {
        std::cout << QDate::currentDate().toString( "MM/dd/yyyy" ).toStdString() << std::endl;
        std::cout << QTime::currentTime().toString( "hh:mm ap" ).toStdString() << std::endl;
        std::cout << "/***********************************/" << std::endl;
        std::cout << "/*      FADTTSter version " << FADTTS_VERSION << "      */" << std::endl;
        std::cout << "/***********************************/" << std::endl << std::endl;


        QString text;
        QFile file( noGUIConfig.data() );
        if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        {
            std::cout << "/!\\ Cannot open " << "'" << noGUIConfig.data() << "'" << std::endl;
            std::cout << "/!\\ File does not exist or is corrupted" << std::endl << std::endl;

            std::cout << "FADTTSter --noGUI will not be run" << std::endl;
            std::cout << "/***********************************/" << std::endl;

            return EXIT_FAILURE;
        }
        else
        {
            text = file.readAll();
            file.close();

            QJsonParseError jsonError;
            QJsonDocument jsonDoc = QJsonDocument::fromJson( text.toUtf8(), &jsonError );
            if( jsonError.error != QJsonParseError::NoError )
            {
                std::cout << "/!\\ An error occurred when trying to extract json data from file " << "'" << noGUIConfig.data() << "'"
                          << std::endl << "/!\\ error: " << jsonError.errorString().toUpper().toStdString() <<  std::endl << std::endl;

                std::cout << "FADTTSter --noGUI will not be run" << std::endl;
                std::cout << "/***********************************/" << std::endl;

                return EXIT_FAILURE;
            }
            else
            {
                std::cout << "FADTTSter --noGUI currently running " << "'" << noGUIConfig.data() << "'..." << std::endl << std::endl;

                FADTTS_noGUI fadtts_noGUI;
                QJsonObject jsonObject_noGUI = jsonDoc.object().value( "noGUIConfiguration" ).toObject();

                if( fadtts_noGUI.RunFADTTSter_noGUI( jsonObject_noGUI ) == EXIT_SUCCESS )
                {
                    std::cout << "FADTTSter run SUCCESSFULLY" << std::endl;
                }
                else
                {
                    std::cout << "FADTTSter FAILED to create expected outputs" << std::endl;
                    std::cout << "/***********************************/" << std::endl;

                    return EXIT_FAILURE;
                }
            }
        }
        std::cout << "/***********************************/" << std::endl;

        return EXIT_SUCCESS;
    }
}
