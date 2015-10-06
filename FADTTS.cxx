#include "FADTTSWindow.h"
#include <QApplication>
#include "FADTTSCLP.h"


int main( int argc, char *argv[] )
{

//    PARSE_ARGS;
//    QApplication app( argc, argv );
//    Q_INIT_RESOURCE( icons );
//    FADTTSWindow w;
//    w.show();

//    return app.exec();


//    PARSE_ARGS;
//    para_Model_FADTTS para_m;
//    para_Save_FADTTS para_s;
//    para_Load_FADTTS para_l;
//    soft_Model_FADTTS soft_m;
//    soft_Save_FADTTS soft_s;
//    soft_Load_FADTTS soft_l;
//    if(!executables.empty())
//    {
//        soft_l.load( soft_m, executables );
//    }
//    if(!parameters.empty())
//    {
//        para_l.load( para_m, parameters );
//    }

//    if(noGUI == false)
//    {
//        QApplication app( argc , argv );
//        FADTTSWindow window;
//        window.SetParaLoad(&para_l);
//        window.SetParaSave(&para_s);
//        window.SetParaModel(&para_m);
//        window.SetSoftLoad(&soft_l);
//        window.SetSoftModel(&soft_m);
//        window.SetSoftSave(&soft_s);
//        window.show() ;
//        if(parameters.empty() && executables.empty())
//        {
//            window.initSoftware();
//            window.SyncModelStructureToUi("soft");
//        }
//        if(parameters.empty() && !executables.empty())
//        {
//            window.SyncModelStructureToUi("soft");

//        }
//        if(!parameters.empty() && executables.empty())
//        {
//            window.initSoftware();
//            window.SyncModelStructureToUi();
//        }
//        if(!parameters.empty() && !executables.empty())
//        {
//            window.SyncModelStructureToUi();
//        }
//        window.SyncUiToModelStructure();
//        return app.exec() ;
//    }
//    else
//    {
//        Pipeline* pipeline;
//        pipeline = new Pipeline();
//        pipeline->setPipelineParameters(&para_m);
//        pipeline->setPipelineSoftwares(&soft_m);
//        pipeline->writePipeline();
//        pipeline->runPipeline();
//        delete( pipeline );
//        return 0;
//    }


    PARSE_ARGS;
    Q_INIT_RESOURCE( icons );

    para_Model_FADTTS para_m;
    para_Save_FADTTS para_s;
    para_Load_FADTTS para_l;
    soft_Model_FADTTS soft_m;
    soft_Save_FADTTS soft_s;
    soft_Load_FADTTS soft_l;

    QApplication app( argc , argv );
    FADTTSWindow window;
    window.SetParaLoad(&para_l);
    window.SetParaSave(&para_s);
    window.SetParaModel(&para_m);
    window.SetSoftLoad(&soft_l);
    window.SetSoftModel(&soft_m);
    window.SetSoftSave(&soft_s);
    window.show() ;

    window.SyncUiToModelStructure();
    return app.exec() ;
}
