#include "MainWindow.h"
#include <QApplication>
#include "FADTTSCLP.h"


int main( int argc, char *argv[] )
{
    PARSE_ARGS;
    QApplication app( argc, argv );
    MainWindow w;
    w.show();

    return app.exec();
}
