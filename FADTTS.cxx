#include "FADTTSWindow.h"
#include <QApplication>
#include "FADTTSCLP.h"


int main( int argc, char *argv[] )
{
    PARSE_ARGS;
    Q_INIT_RESOURCE( FADTTS_Resources );

    para_Model_FADTTS para_m;
    para_Save_FADTTS para_s;
    para_Load_FADTTS para_l;

    QApplication app( argc , argv );

    FADTTSWindow window;
    window.SetParaLoad( &para_l );
    window.SetParaSave( &para_s );
    window.SetParaModel( &para_m );
    window.show() ;
    window.SyncUiToModelStructure();

    return app.exec() ;
}
