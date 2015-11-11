#ifndef FADTTSWINDOWCONFIG_H
#define FADTTSWINDOWCONFIG_H

#include <QMainWindow>

#include "ui_FADTTSWindow.h"
#include "para_Save_FADTTS.h"
#include "para_Load_FADTTS.h"
#include "para_Model_FADTTS.h"
#include "soft_Save_FADTTS.h"
#include "soft_Load_FADTTS.h"
#include "soft_Model_FADTTS.h"


class FADTTSWindowConfig :public QMainWindow, public Ui::FADTTSWindow
{
    Q_OBJECT

public slots:
    void Save_Parameter_Configuration( std::string filename );

    void Load_Parameter_Configuration( std::string filename );

    void Save_Software_Configuration( std::string filename );

    void Load_Software_Configuration( std::string filename );


    void SyncUiToModelStructure();

    void SyncModelStructureToUi();

    void SyncUiToModelStructure( QString prefix );

    void SyncModelStructureToUi( QString prefix );


public:
    para_Model_FADTTS* m_para_m;

    para_Save_FADTTS* m_para_s;

    para_Load_FADTTS* m_para_l;

    soft_Model_FADTTS* m_soft_m;

    soft_Save_FADTTS* m_soft_s;

    soft_Load_FADTTS* m_soft_l;


    FADTTSWindowConfig( QWidget *parent = 0, Qt::WindowFlags f = 0 );

    void SetParaModel( para_Model_FADTTS* para_m );

    void SetParaSave( para_Save_FADTTS* para_s );

    void SetParaLoad( para_Load_FADTTS* para_l );

    void SetSoftModel( soft_Model_FADTTS* soft_m );

    void SetSoftSave( soft_Save_FADTTS* soft_s );

    void SetSoftLoad( soft_Load_FADTTS* soft_l );


private:
    bool m_sync;
};


#endif // FADTTSWINDOWCONFIG_H
