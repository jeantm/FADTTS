#ifndef FADTTSWINDOWCONFIG_H
#define FADTTSWINDOWCONFIG_H

#include <QMainWindow>

#include "ui_FADTTSWindow.h"
#include "para_Save_FADTTS.h"
#include "para_Load_FADTTS.h"
#include "para_Model_FADTTS.h"


class FADTTSWindowConfig :public QMainWindow, public Ui::FADTTSWindow
{
    Q_OBJECT

public slots:
    void Save_Parameter_Configuration( std::string filename );

    void Load_Parameter_Configuration( std::string filename );

    void SyncUiToModelStructure();

    void SyncModelStructureToUi();


public:
    para_Model_FADTTS* m_para_m;

    para_Save_FADTTS* m_para_s;

    para_Load_FADTTS* m_para_l;


    FADTTSWindowConfig( QWidget *parent = 0, Qt::WFlags f = 0 );

    void SetParaModel( para_Model_FADTTS* para_m );

    void SetParaSave( para_Save_FADTTS* para_s );

    void SetParaLoad( para_Load_FADTTS* para_l );


private:
    bool m_sync;
};


#endif // FADTTSWINDOWCONFIG_H
