#include "FADTTSWindow.h"
#include <QApplication>
#include "FADTTSterCLP.h"

int main( int argc, char *argv[] )
{
    PARSE_ARGS;
    Q_INIT_RESOURCE( FADTTS_Resources );

    para_Model_FADTTS para_m;
    para_Save_FADTTS para_s;
    para_Load_FADTTS para_l;
    soft_Model_FADTTS soft_m;
    soft_Save_FADTTS soft_s;
    soft_Load_FADTTS soft_l;

    QApplication app( argc , argv );

    FADTTSWindow window;
    window.SetParaLoad( &para_l );
    window.SetParaSave( &para_s );
    window.SetParaModel( &para_m );
    window.SetSoftLoad( &soft_l );
    window.SetSoftModel( &soft_m );
    window.SetSoftSave( &soft_s );
    window.show() ;
    window.SyncUiToModelStructure();

    return app.exec() ;
}
