#include "FADTTSWindowConfig.h"

FADTTSWindowConfig::FADTTSWindowConfig( QWidget *parent, Qt::WFlags f ):
    QMainWindow( parent, f )
{
    this->setupUi( this ) ;
    m_sync = false ;
//    m_para_m = 0;
}

void FADTTSWindowConfig::SyncUiToModelStructure()
{
    if( m_sync == 1)
    {
        return;
    }
    m_sync = true;

    /*1st tab: Inputs*/
    m_para_m->setpara_inputsTab_inputADfile_lineEdit( para_inputsTab_inputADfile_lineEdit->text() ) ;
    m_para_m->setpara_inputsTab_inputRDfile_lineEdit( para_inputsTab_inputRDfile_lineEdit->text() ) ;
    m_para_m->setpara_inputsTab_inputMDfile_lineEdit( para_inputsTab_inputMDfile_lineEdit->text() ) ;
    m_para_m->setpara_inputsTab_inputFAfile_lineEdit( para_inputsTab_inputFAfile_lineEdit->text() ) ;
    m_para_m->setpara_inputsTab_inputCOMPfile_lineEdit( para_inputsTab_inputCOMPfile_lineEdit->text() ) ;

    m_para_m->setpara_inputsTab_fiberName_lineEdit( para_inputsTab_fiberName_lineEdit->text() );


    /*2nd tab: Subjects*/
    m_para_m->setpara_subjectsTab_adInput_checkBox( para_subjectsTab_adInput_checkBox->checkState() );
    m_para_m->setpara_subjectsTab_rdInput_checkBox( para_subjectsTab_rdInput_checkBox->checkState() );
    m_para_m->setpara_subjectsTab_mdInput_checkBox( para_subjectsTab_mdInput_checkBox->checkState() );
    m_para_m->setpara_subjectsTab_faInput_checkBox( para_subjectsTab_faInput_checkBox->checkState() );
    m_para_m->setpara_subjectsTab_compInput_checkBox( para_subjectsTab_compInput_checkBox->checkState() );

    m_para_m->setpara_subjectsTab_inputSubjectList_lineEdit( para_subjectsTab_inputSubjectList_lineEdit->text() );

    /************** ????listWidget Subjects???? **************/


    /*3rd tab: Parameteres*/
    std::map<QString,bool> itemList;
    itemList.clear();
    for( int i = 0 ; i < para_parametersTab_covariates_listWidget->count() ; i++ )
    {
        QString encoding = QString( "%1" ).arg( i, 3, 10, QChar( '0' ) ).append( "_" );
        QListWidgetItem *currentItem = para_parametersTab_covariates_listWidget->item( i ) ;
        itemList[ encoding.append( currentItem->text() ) ] = static_cast<bool>( currentItem->checkState() ) ;
    }
    qDebug() << "itemList.size() Set" << itemList.size();
    m_para_m->setpara_parametersTab_covariates_listWidget( itemList ) ;

    m_para_m->setpara_parametersTab_omnibus_checkBox( para_parametersTab_omnibus_checkBox->checkState() );
    m_para_m->setpara_parametersTab_postHoc_checkBox( para_parametersTab_postHoc_checkBox->checkState() );

    m_para_m->setpara_parametersTab_nbrPermutations_spinBox( para_parametersTab_nbrPermutations_spinBox->value() );


    /*4th tab: Quality Control*/


    /*5th tab: Run*/
    m_para_m->setpara_runTab_outputDir_lineEdit(para_runTab_outputDir_lineEdit->text() );
    m_soft_m->setsoft_runTab_matlab_lineEdit( soft_runTab_matlab_lineEdit->text() );


    m_sync = false ;
}

void FADTTSWindowConfig::SyncUiToModelStructure( QString prefix )
{
    if( m_sync == 1)
    {
        return;
    }
    m_sync = true;
    if(prefix =="para")
    {
        /*1st tab: Inputs*/
        m_para_m->setpara_inputsTab_inputADfile_lineEdit( para_inputsTab_inputADfile_lineEdit->text() ) ;
        m_para_m->setpara_inputsTab_inputRDfile_lineEdit( para_inputsTab_inputRDfile_lineEdit->text() ) ;
        m_para_m->setpara_inputsTab_inputMDfile_lineEdit( para_inputsTab_inputMDfile_lineEdit->text() ) ;
        m_para_m->setpara_inputsTab_inputFAfile_lineEdit( para_inputsTab_inputFAfile_lineEdit->text() ) ;
        m_para_m->setpara_inputsTab_inputCOMPfile_lineEdit( para_inputsTab_inputCOMPfile_lineEdit->text() ) ;

        m_para_m->setpara_inputsTab_fiberName_lineEdit( para_inputsTab_fiberName_lineEdit->text() );


        /*2nd tab: Subjects*/
        m_para_m->setpara_subjectsTab_adInput_checkBox( para_subjectsTab_adInput_checkBox->checkState() );
        m_para_m->setpara_subjectsTab_rdInput_checkBox( para_subjectsTab_rdInput_checkBox->checkState() );
        m_para_m->setpara_subjectsTab_mdInput_checkBox( para_subjectsTab_mdInput_checkBox->checkState() );
        m_para_m->setpara_subjectsTab_faInput_checkBox( para_subjectsTab_faInput_checkBox->checkState() );
        m_para_m->setpara_subjectsTab_compInput_checkBox( para_subjectsTab_compInput_checkBox->checkState() );

        m_para_m->setpara_subjectsTab_inputSubjectList_lineEdit( para_subjectsTab_inputSubjectList_lineEdit->text() );

        /************** ????listWidget Subjects???? **************/
//        std::map<QString,bool> itemList ;
//        for( int i = 0 ; i < para_ref_tracts_listWidget->count() ; i++ )
//        {
//            QListWidgetItem *currentItem = para_ref_tracts_listWidget->item(i) ;
//            itemList[ currentItem->text() ] = static_cast<bool>(currentItem->checkState() ) ;
//        }
//        m_para_m->setpara_ref_tracts_listWidget(itemList) ;


        /*3rd tab: Parameteres*/
        std::map<QString,bool> itemList;
        itemList.clear();
        for( int i = 0 ; i < para_parametersTab_covariates_listWidget->count() ; i++ )
        {
            QString encoding = QString( "%1" ).arg( i, 3, 10, QChar( '0' ) ).append( "_" );
            QListWidgetItem *currentItem = para_parametersTab_covariates_listWidget->item( i ) ;
            itemList[ encoding.append( currentItem->text() ) ] = static_cast<bool>( currentItem->checkState() ) ;
        }
        m_para_m->setpara_parametersTab_covariates_listWidget( itemList ) ;

        m_para_m->setpara_parametersTab_omnibus_checkBox( para_parametersTab_omnibus_checkBox->checkState() );
        m_para_m->setpara_parametersTab_postHoc_checkBox( para_parametersTab_postHoc_checkBox->checkState() );

        m_para_m->setpara_parametersTab_nbrPermutations_spinBox( para_parametersTab_nbrPermutations_spinBox->value() );


        /*4th tab: Quality Control*/


        /*5th tab: Run*/
        m_para_m->setpara_runTab_outputDir_lineEdit(para_runTab_outputDir_lineEdit->text() );
    }

    if(prefix == "soft")
    {
        /*5th tab: Run*/
        m_soft_m->setsoft_runTab_matlab_lineEdit( soft_runTab_matlab_lineEdit->text() );
    }

    m_sync = false ;
}

void FADTTSWindowConfig::SyncModelStructureToUi()
{
    if( m_sync == 1)
    {
        return;
    }
    m_sync = true;

    /*1st tab: Inputs*/
    para_inputsTab_inputADfile_lineEdit->setText( m_para_m->getpara_inputsTab_inputADfile_lineEdit() );
    para_inputsTab_inputRDfile_lineEdit->setText( m_para_m->getpara_inputsTab_inputRDfile_lineEdit() );
    para_inputsTab_inputMDfile_lineEdit->setText( m_para_m->getpara_inputsTab_inputMDfile_lineEdit() );
    para_inputsTab_inputFAfile_lineEdit->setText( m_para_m->getpara_inputsTab_inputFAfile_lineEdit() );
    para_inputsTab_inputCOMPfile_lineEdit->setText( m_para_m->getpara_inputsTab_inputCOMPfile_lineEdit() );

    para_inputsTab_fiberName_lineEdit->setText( m_para_m->getpara_inputsTab_fiberName_lineEdit() );


    /*2nd tab: Subjects*/
    para_subjectsTab_adInput_checkBox->setChecked( m_para_m->getpara_subjectsTab_adInput_checkBox() );
    para_subjectsTab_rdInput_checkBox->setChecked( m_para_m->getpara_subjectsTab_rdInput_checkBox() );
    para_subjectsTab_mdInput_checkBox->setChecked( m_para_m->getpara_subjectsTab_mdInput_checkBox() );
    para_subjectsTab_faInput_checkBox->setChecked( m_para_m->getpara_subjectsTab_faInput_checkBox() );
    para_subjectsTab_compInput_checkBox->setChecked( m_para_m->getpara_subjectsTab_compInput_checkBox() );

    para_subjectsTab_inputSubjectList_lineEdit->setText( m_para_m->getpara_subjectsTab_inputSubjectList_lineEdit() );

    /************** ????listWidget Subjects???? **************/


    /*3rd tab: Parameteres*/
    std::map<QString,bool> itemList = m_para_m->getpara_parametersTab_covariates_listWidget();
    qDebug() << "itemList.size() Get" << itemList.size();
    para_parametersTab_covariates_listWidget->clear();
    for( std::map<QString,bool>::iterator it = itemList.begin() ; it != itemList.end() ; it++ )
    {
        QListWidgetItem *item = new QListWidgetItem( it->first.split( "_" ).last() , para_parametersTab_covariates_listWidget ) ;
        item->setCheckState( it->second != false ? Qt::Checked : Qt::Unchecked ) ;
        item->setFlags( Qt::ItemIsEnabled );
        para_parametersTab_covariates_listWidget->addItem( item );
    }

    para_parametersTab_omnibus_checkBox->setChecked( m_para_m->getpara_parametersTab_omnibus_checkBox() );
    para_parametersTab_postHoc_checkBox->setChecked( m_para_m->getpara_parametersTab_postHoc_checkBox() );

    para_parametersTab_nbrPermutations_spinBox->setValue( m_para_m->getpara_parametersTab_nbrPermutations_spinBox() );


    /*4th tab: Quality Control*/


    /*5th tab: Run*/
    para_runTab_outputDir_lineEdit->setText( m_para_m->getpara_runTab_outputDir_lineEdit() );

    soft_runTab_matlab_lineEdit->setText( m_soft_m->getsoft_runTab_matlab_lineEdit() );


    m_sync = false ;
}

void FADTTSWindowConfig::SyncModelStructureToUi( QString prefix )
{
    if( m_sync == 1)
    {
        return;
    }
    m_sync = true;

    if(prefix == "para")
    {
        /*1st tab: Inputs*/
        para_inputsTab_inputADfile_lineEdit->setText( m_para_m->getpara_inputsTab_inputADfile_lineEdit() );
        para_inputsTab_inputRDfile_lineEdit->setText( m_para_m->getpara_inputsTab_inputRDfile_lineEdit() );
        para_inputsTab_inputMDfile_lineEdit->setText( m_para_m->getpara_inputsTab_inputMDfile_lineEdit() );
        para_inputsTab_inputFAfile_lineEdit->setText( m_para_m->getpara_inputsTab_inputFAfile_lineEdit() );
        para_inputsTab_inputCOMPfile_lineEdit->setText( m_para_m->getpara_inputsTab_inputCOMPfile_lineEdit() );

        para_inputsTab_fiberName_lineEdit->setText( m_para_m->getpara_inputsTab_fiberName_lineEdit() );


        /*2nd tab: Subjects*/
        para_subjectsTab_adInput_checkBox->setChecked( m_para_m->getpara_subjectsTab_adInput_checkBox() );
        para_subjectsTab_rdInput_checkBox->setChecked( m_para_m->getpara_subjectsTab_rdInput_checkBox() );
        para_subjectsTab_mdInput_checkBox->setChecked( m_para_m->getpara_subjectsTab_mdInput_checkBox() );
        para_subjectsTab_faInput_checkBox->setChecked( m_para_m->getpara_subjectsTab_faInput_checkBox() );
        para_subjectsTab_compInput_checkBox->setChecked( m_para_m->getpara_subjectsTab_compInput_checkBox() );

        para_subjectsTab_inputSubjectList_lineEdit->setText( m_para_m->getpara_subjectsTab_inputSubjectList_lineEdit() );

        /************** ????listWidget Subjects???? **************/


        /*3rd tab: Parameteres*/
        std::map<QString,bool> itemList = m_para_m->getpara_parametersTab_covariates_listWidget();
        qDebug() << "itemList.size() Get" << itemList.size();

        para_parametersTab_covariates_listWidget->clear();
        for( std::map<QString,bool>::iterator it = itemList.begin() ; it != itemList.end() ; it++ )
        {
            QListWidgetItem *item = new QListWidgetItem( it->first.split( "_" ).last() , para_parametersTab_covariates_listWidget ) ;
            item->setCheckState( it->second != false ? Qt::Checked : Qt::Unchecked ) ;
            item->setFlags( Qt::ItemIsEnabled );
            para_parametersTab_covariates_listWidget->addItem( item );
        }

        para_parametersTab_omnibus_checkBox->setChecked( m_para_m->getpara_parametersTab_omnibus_checkBox() );
        para_parametersTab_postHoc_checkBox->setChecked( m_para_m->getpara_parametersTab_postHoc_checkBox() );

        para_parametersTab_nbrPermutations_spinBox->setValue( m_para_m->getpara_parametersTab_nbrPermutations_spinBox() );


        /*4th tab: Quality Control*/


        /*5th tab: Run*/
        para_runTab_outputDir_lineEdit->setText( m_para_m->getpara_runTab_outputDir_lineEdit() );
    }

    if(prefix == "soft")
    {
        /*5th tab: Run*/
        soft_runTab_matlab_lineEdit->setText( m_soft_m->getsoft_runTab_matlab_lineEdit() );
    }

    m_sync = false ;
}

void FADTTSWindowConfig::Save_Parameter_Configuration( std::string filename )
{
    SyncUiToModelStructure();
    m_para_s->save( *m_para_m, filename );
}

void FADTTSWindowConfig::Load_Parameter_Configuration( std::string filename )
{
    m_para_l->load( *m_para_m, filename );
    SyncModelStructureToUi();
}

void FADTTSWindowConfig::Save_Software_Configuration( std::string filename )
{
    SyncUiToModelStructure();
    m_soft_s->save( *m_soft_m, filename );
}
void FADTTSWindowConfig::Load_Software_Configuration( std::string filename )
{
    m_soft_l->load( *m_soft_m, filename );
    SyncModelStructureToUi();
}

void FADTTSWindowConfig::SetParaModel( para_Model_FADTTS* para_m )
{
    m_para_m = para_m;
}

void FADTTSWindowConfig::SetParaSave( para_Save_FADTTS* para_s )
{
    m_para_s = para_s;
}

void FADTTSWindowConfig::SetParaLoad( para_Load_FADTTS* para_l )
{
    m_para_l = para_l;
}

void FADTTSWindowConfig::SetSoftModel( soft_Model_FADTTS* soft_m )
{
    m_soft_m = soft_m;
}

void FADTTSWindowConfig::SetSoftSave( soft_Save_FADTTS* soft_s)
{
    m_soft_s = soft_s;
}

void FADTTSWindowConfig::SetSoftLoad( soft_Load_FADTTS* soft_l)
{
    m_soft_l = soft_l;
}
