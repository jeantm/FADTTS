#include "FADTTSWindowConfig.h"

FADTTSWindowConfig::FADTTSWindowConfig( QWidget *parent, Qt::WindowFlags f ):
    QMainWindow( parent, f )
{
    this->setupUi( this );
    m_sync = false;
}

void FADTTSWindowConfig::SyncUiToModelStructure()
{
    if( m_sync == true )
    {
        return;
    }
    m_sync = true;

    /*1st tab: Inputs*/
    m_para_m->setpara_inputTab_adFile_lineEdit( para_inputTab_adFile_lineEdit->text() );
    m_para_m->setpara_inputTab_rdFile_lineEdit( para_inputTab_rdFile_lineEdit->text() );
    m_para_m->setpara_inputTab_mdFile_lineEdit( para_inputTab_mdFile_lineEdit->text() );
    m_para_m->setpara_inputTab_faFile_lineEdit( para_inputTab_faFile_lineEdit->text() );
    m_para_m->setpara_inputTab_covariateFile_lineEdit( para_inputTab_covariateFile_lineEdit->text() );

    m_para_m->setpara_inputTab_fiberName_lineEdit( para_inputTab_fiberName_lineEdit->text() );


    /*2nd tab: Subjects*/
    m_para_m->setpara_subjectTab_adFile_checkBox( para_subjectTab_adFile_checkBox->checkState() );
    m_para_m->setpara_subjectTab_rdFile_checkBox( para_subjectTab_rdFile_checkBox->checkState() );
    m_para_m->setpara_subjectTab_mdFile_checkBox( para_subjectTab_mdFile_checkBox->checkState() );
    m_para_m->setpara_subjectTab_faFile_checkBox( para_subjectTab_faFile_checkBox->checkState() );
    m_para_m->setpara_subjectTab_covariateFile_checkBox( para_subjectTab_covariateFile_checkBox->checkState() );

    m_para_m->setpara_subjectTab_subjectFile_lineEdit( para_subjectTab_subjectFile_lineEdit->text() );


    /*3rd tab: Parameteres*/
    std::map<std::pair<unsigned long,QString>,bool> itemList;
    itemList.clear();
    for( unsigned long i = 0; i < para_subjectTab_covariates_listWidget->count(); i++ )
    {
        QListWidgetItem *currentItem = para_subjectTab_covariates_listWidget->item( i );
        std::pair<unsigned long,QString> attribute;
        attribute = std::make_pair( i, currentItem->text() );
        itemList[ attribute ] = static_cast<bool>( currentItem->checkState() );
    }
    m_para_m->setpara_subjectTab_covariates_listWidget( itemList );


    m_para_m->setpara_executionTab_omnibus_checkBox( para_executionTab_omnibus_checkBox->checkState() );
    m_para_m->setpara_executionTab_postHoc_checkBox( para_executionTab_postHoc_checkBox->checkState() );

    m_para_m->setpara_executionTab_nbrPermutations_spinBox( para_executionTab_nbrPermutations_spinBox->value() );


    /*4th tab: Quality Control*/


    /*5th tab: Run*/
    m_para_m->setpara_executionTab_outputDir_lineEdit(para_executionTab_outputDir_lineEdit->text() );


    m_soft_m->setsoft_executionTab_matlab_lineEdit( soft_executionTab_matlab_lineEdit->text() );
    m_soft_m->setsoft_executionTab_mvcm_lineEdit( soft_executionTab_mvcm_lineEdit->text() );


    m_sync = false;
}

void FADTTSWindowConfig::SyncUiToModelStructure( QString prefix )
{
    if( m_sync == true )
    {
        return;
    }
    m_sync = true;
    if( prefix =="para" )
    {
        /*1st tab: Inputs*/
        m_para_m->setpara_inputTab_adFile_lineEdit( para_inputTab_adFile_lineEdit->text() );
        m_para_m->setpara_inputTab_rdFile_lineEdit( para_inputTab_rdFile_lineEdit->text() );
        m_para_m->setpara_inputTab_mdFile_lineEdit( para_inputTab_mdFile_lineEdit->text() );
        m_para_m->setpara_inputTab_faFile_lineEdit( para_inputTab_faFile_lineEdit->text() );
        m_para_m->setpara_inputTab_covariateFile_lineEdit( para_inputTab_covariateFile_lineEdit->text() );

        m_para_m->setpara_inputTab_fiberName_lineEdit( para_inputTab_fiberName_lineEdit->text() );


        /*2nd tab: Subjects*/
        m_para_m->setpara_subjectTab_adFile_checkBox( para_subjectTab_adFile_checkBox->checkState() );
        m_para_m->setpara_subjectTab_rdFile_checkBox( para_subjectTab_rdFile_checkBox->checkState() );
        m_para_m->setpara_subjectTab_mdFile_checkBox( para_subjectTab_mdFile_checkBox->checkState() );
        m_para_m->setpara_subjectTab_faFile_checkBox( para_subjectTab_faFile_checkBox->checkState() );
        m_para_m->setpara_subjectTab_covariateFile_checkBox( para_subjectTab_covariateFile_checkBox->checkState() );

        m_para_m->setpara_subjectTab_subjectFile_lineEdit( para_subjectTab_subjectFile_lineEdit->text() );


        /*3rd tab: Parameteres*/
        std::map<std::pair<unsigned long,QString>,bool> itemList;
        itemList.clear();
        for( unsigned long i = 0; i < para_subjectTab_covariates_listWidget->count(); i++ )
        {
            QListWidgetItem *currentItem = para_subjectTab_covariates_listWidget->item( i );
            std::pair<unsigned long,QString> attribute;
            attribute = std::make_pair( i, currentItem->text() );
            itemList[ attribute ] = static_cast<bool>( currentItem->checkState() );
        }
        m_para_m->setpara_subjectTab_covariates_listWidget( itemList );

        m_para_m->setpara_executionTab_omnibus_checkBox( para_executionTab_omnibus_checkBox->checkState() );
        m_para_m->setpara_executionTab_postHoc_checkBox( para_executionTab_postHoc_checkBox->checkState() );

        m_para_m->setpara_executionTab_nbrPermutations_spinBox( para_executionTab_nbrPermutations_spinBox->value() );


        /*4th tab: Quality Control*/


        /*5th tab: Run*/
        m_para_m->setpara_executionTab_outputDir_lineEdit(para_executionTab_outputDir_lineEdit->text() );
    }

    if( prefix == "soft" )
    {
        /*5th tab: Run*/
        m_soft_m->setsoft_executionTab_matlab_lineEdit( soft_executionTab_matlab_lineEdit->text() );
        m_soft_m->setsoft_executionTab_mvcm_lineEdit( soft_executionTab_mvcm_lineEdit->text() );
    }

    m_sync = false;
}


void FADTTSWindowConfig::SyncModelStructureToUi()
{
    if( m_sync == true )
    {
        return;
    }
    m_sync = true;

    /*1st tab: Inputs*/
    para_inputTab_adFile_lineEdit->setText( m_para_m->getpara_inputTab_adFile_lineEdit() );
    para_inputTab_rdFile_lineEdit->setText( m_para_m->getpara_inputTab_rdFile_lineEdit() );
    para_inputTab_mdFile_lineEdit->setText( m_para_m->getpara_inputTab_mdFile_lineEdit() );
    para_inputTab_faFile_lineEdit->setText( m_para_m->getpara_inputTab_faFile_lineEdit() );
    para_inputTab_covariateFile_lineEdit->setText( m_para_m->getpara_inputTab_covariateFile_lineEdit() );

    para_inputTab_fiberName_lineEdit->setText( m_para_m->getpara_inputTab_fiberName_lineEdit() );


    /*2nd tab: Subjects*/
    para_subjectTab_adFile_checkBox->setChecked( m_para_m->getpara_subjectTab_adFile_checkBox() );
    para_subjectTab_rdFile_checkBox->setChecked( m_para_m->getpara_subjectTab_rdFile_checkBox() );
    para_subjectTab_mdFile_checkBox->setChecked( m_para_m->getpara_subjectTab_mdFile_checkBox() );
    para_subjectTab_faFile_checkBox->setChecked( m_para_m->getpara_subjectTab_faFile_checkBox() );
    para_subjectTab_covariateFile_checkBox->setChecked( m_para_m->getpara_subjectTab_covariateFile_checkBox() );

    para_subjectTab_subjectFile_lineEdit->setText( m_para_m->getpara_subjectTab_subjectFile_lineEdit() );


    /*3rd tab: Parameteres*/
    std::map<std::pair<unsigned long,QString>,bool> itemList;
    if( !m_para_m->getpara_inputTab_covariateFile_lineEdit().isEmpty() )
    {
        itemList = m_para_m->getpara_subjectTab_covariates_listWidget();
    }
    para_subjectTab_covariates_listWidget->clear();
    for( std::map<std::pair<unsigned long,QString>,bool>::iterator it = itemList.begin(); it != itemList.end(); it++ )
    {
        QListWidgetItem *item = new QListWidgetItem( it->first.second , para_subjectTab_covariates_listWidget );
        item->setCheckState( it->second != 0 ? Qt::Checked : Qt::Unchecked );
        item->setFlags( Qt::ItemIsEnabled );
    }

    para_executionTab_omnibus_checkBox->setChecked( m_para_m->getpara_executionTab_omnibus_checkBox() );
    para_executionTab_postHoc_checkBox->setChecked( m_para_m->getpara_executionTab_postHoc_checkBox() );

    para_executionTab_nbrPermutations_spinBox->setValue( m_para_m->getpara_executionTab_nbrPermutations_spinBox() );


    /*4th tab: Quality Control*/


    /*5th tab: Run*/
    para_executionTab_outputDir_lineEdit->setText( m_para_m->getpara_executionTab_outputDir_lineEdit() );

    soft_executionTab_matlab_lineEdit->setText( m_soft_m->getsoft_executionTab_matlab_lineEdit() );
    soft_executionTab_mvcm_lineEdit->setText( m_soft_m->getsoft_executionTab_mvcm_lineEdit() );


    m_sync = false;
}

void FADTTSWindowConfig::SyncModelStructureToUi( QString prefix )
{
    if( m_sync == true )
    {
        return;
    }
    m_sync = true;

    if( prefix == "para" )
    {
        /*1st tab: Inputs*/
        para_inputTab_adFile_lineEdit->setText( m_para_m->getpara_inputTab_adFile_lineEdit() );
        para_inputTab_rdFile_lineEdit->setText( m_para_m->getpara_inputTab_rdFile_lineEdit() );
        para_inputTab_mdFile_lineEdit->setText( m_para_m->getpara_inputTab_mdFile_lineEdit() );
        para_inputTab_faFile_lineEdit->setText( m_para_m->getpara_inputTab_faFile_lineEdit() );
        para_inputTab_covariateFile_lineEdit->setText( m_para_m->getpara_inputTab_covariateFile_lineEdit() );

        para_inputTab_fiberName_lineEdit->setText( m_para_m->getpara_inputTab_fiberName_lineEdit() );


        /*2nd tab: Subjects*/
        para_subjectTab_adFile_checkBox->setChecked( m_para_m->getpara_subjectTab_adFile_checkBox() );
        para_subjectTab_rdFile_checkBox->setChecked( m_para_m->getpara_subjectTab_rdFile_checkBox() );
        para_subjectTab_mdFile_checkBox->setChecked( m_para_m->getpara_subjectTab_mdFile_checkBox() );
        para_subjectTab_faFile_checkBox->setChecked( m_para_m->getpara_subjectTab_faFile_checkBox() );
        para_subjectTab_covariateFile_checkBox->setChecked( m_para_m->getpara_subjectTab_covariateFile_checkBox() );

        para_subjectTab_subjectFile_lineEdit->setText( m_para_m->getpara_subjectTab_subjectFile_lineEdit() );


        /*3rd tab: Parameteres*/
        std::map<std::pair<unsigned long,QString>,bool> itemList = m_para_m->getpara_subjectTab_covariates_listWidget();
        para_subjectTab_covariates_listWidget->clear();
        for( std::map<std::pair<unsigned long,QString>,bool>::iterator it = itemList.begin(); it != itemList.end(); it++ )
        {
            QListWidgetItem *item = new QListWidgetItem( it->first.second , para_subjectTab_covariates_listWidget );
            item->setCheckState( it->second != 0 ? Qt::Checked : Qt::Unchecked );
            item->setFlags( Qt::ItemIsEnabled );
        }

        para_executionTab_omnibus_checkBox->setChecked( m_para_m->getpara_executionTab_omnibus_checkBox() );
        para_executionTab_postHoc_checkBox->setChecked( m_para_m->getpara_executionTab_postHoc_checkBox() );

        para_executionTab_nbrPermutations_spinBox->setValue( m_para_m->getpara_executionTab_nbrPermutations_spinBox() );


        /*4th tab: Quality Control*/


        /*5th tab: Run*/
        para_executionTab_outputDir_lineEdit->setText( m_para_m->getpara_executionTab_outputDir_lineEdit() );
    }

    if( prefix == "soft" )
    {
        /*5th tab: Run*/
        soft_executionTab_matlab_lineEdit->setText( m_soft_m->getsoft_executionTab_matlab_lineEdit() );
        soft_executionTab_mvcm_lineEdit->setText( m_soft_m->getsoft_executionTab_mvcm_lineEdit() );
    }

    m_sync = false;
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
