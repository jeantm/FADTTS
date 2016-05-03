#include "FADTTSWindow.h"
#include "FADTTS_noGUI.h"
#include "FADTTSterCLP.h"

#include <QApplication>

//#include <QDebug>

int main( int argc, char *argv[] )
{
    PARSE_ARGS;
    Q_INIT_RESOURCE( FADTTS_Resources );

    QString dir = QString( directory.data() ).isEmpty() ? QDir::currentPath() : directory.data();
    QStringList paraFilter = QStringList() << "*para*.json" << "*Para*.json";
    QStringList softFilter = QStringList() << "*soft*.json" << "*Soft*.json";
    QStringList noGUIFilter = QStringList() << "*noGUI*.json" << "*NoGUI*.json";

    QString defaultParaConfigurationFile = !QDir( dir ).entryList( paraFilter, QDir::Files ).isEmpty() ?
                QString( dir + "/" + QDir( dir ).entryList( paraFilter, QDir::Files ).first() ) :
                QString();
    QString defaultSoftConfigurationFile = !QDir( dir ).entryList( softFilter, QDir::Files ).isEmpty() ?
                QString( dir + "/" + QDir( dir ).entryList( softFilter, QDir::Files ).first() ) :
                QString();
    QString defaultNoGUIConfigurationFile = !QDir( dir ).entryList( noGUIFilter, QDir::Files ).isEmpty() ?
                QString( dir + "/" + QDir( dir ).entryList( noGUIFilter, QDir::Files ).first() ) :
                QString();

    QString paraConfigurationFile = QString( paraConfig.data() ).isEmpty() ? defaultParaConfigurationFile : paraConfig.data();
    QString softConfigurationFile = QString( softConfig.data() ).isEmpty() ? defaultSoftConfigurationFile : softConfig.data();
    QString noGUIConfigurationFile = QString( noGUIConfig.data() ).isEmpty() ? defaultNoGUIConfigurationFile : noGUIConfig.data();

    if( noGUI )
    {
        std::cout << QDate::currentDate().toString( "MM/dd/yyyy" ).toStdString() << std::endl;
        std::cout << QTime::currentTime().toString( "hh:mm ap" ).toStdString() << std::endl;
        std::cout << "/***********************************/" << std::endl;
        std::cout << "/*      FADTTSter version " << FADTTS_VERSION << "      */" << std::endl;
        std::cout << "/***********************************/" << std::endl << std::endl;


        QString text;
        QFile file( noGUIConfigurationFile );
        if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        {
            std::cout << "/!\\ Cannot open " << "'" << noGUIConfigurationFile.toStdString() << "'" << std::endl;
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
                std::cout << "/!\\ An error occurred when trying to extract json data from file " << "'" << noGUIConfigurationFile.toStdString() << "'"
                          << std::endl << "/!\\ error: " << jsonError.errorString().toUpper().toStdString() <<  std::endl << std::endl;

                std::cout << "FADTTSter --noGUI will not be run" << std::endl;
                std::cout << "/***********************************/" << std::endl;

                return EXIT_FAILURE;
            }
            else
            {
                std::cout << "FADTTSter --noGUI currently running " << "'" << noGUIConfigurationFile.toStdString() << "'..." << std::endl << std::endl;

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
    else
    {
        QApplication app( argc , argv );

        FADTTSWindow fadttsWindow;
        fadttsWindow.show();

        if( !paraConfigurationFile.isEmpty() )
        {
            fadttsWindow.LoadParaConfiguration( paraConfigurationFile );
        }
        if( !softConfigurationFile.isEmpty() )
        {
            fadttsWindow.LoadSoftConfiguration( softConfigurationFile );
        }

        return app.exec();
    }
}
