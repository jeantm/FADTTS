#include "QCThresholdDialog.h"
#include "ui_QCThresholdDialog.h"

//#include <QDebug>

QCThresholdDialog::QCThresholdDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QCThresholdDialog)
{
    ui->setupUi(this);

    InitQCThresholdDialog();
}

QCThresholdDialog::~QCThresholdDialog()
{
    delete ui;
}


/***************************************************************/
/************************  Public slots ************************/
/***************************************************************/


/***************************************************************/
/********************** Public functions ***********************/
/***************************************************************/
bool QCThresholdDialog::InitPlot( const QList< QStringList >& rawData, const QStringList& atlas, const QStringList& matchedSubjects, double qcThreshold )
{
    m_faData = rawData;
    m_matchedSubjects = matchedSubjects;
    m_atlas = atlas;
    m_nanSubjects = m_processing.GetNANSubjects( m_faData, m_matchedSubjects );

    ui->QCThresholdDialog_removeNAN_pushButton->setHidden( m_nanSubjects.isEmpty() );
    ui->QCThresholdDialog_removeNAN_label->setHidden( m_nanSubjects.isEmpty() );

    m_qcThresholdDoubleSpinBox->setValue( qcThreshold );

    bool qcThresholdDialogReady = m_qcThresholdPlot->InitQCThresholdPlot( m_faData, m_matchedSubjects );
    DisplayQCThresholdPlot( m_qcThresholdDoubleSpinBox->value(), m_atlas );
    m_updated = false;

    return qcThresholdDialogReady;
}



/***************************************************************/
/************************ Private slots ************************/
/***************************************************************/
void QCThresholdDialog::OnUpdatingSubjectsCorrelated( const QStringList& subjectsCorrelated, const QStringList& subjectsNotCorrelated )
{
    m_subjectsCorrelated = subjectsCorrelated;
    m_subjectsNotCorrelated = subjectsNotCorrelated;

    QString info = !m_subjectsNotCorrelated.isEmpty() ? QString( "<i><span style=""font-size:8pt;"">" + QString::number( m_subjectsNotCorrelated.size() ) + " subject(s) not satisfying the threshold</span></i>" ) : "";
    ui->QCThresholdDialog_info_label->setText( tr( qPrintable( info ) ) );
}

void QCThresholdDialog::OnUpdatingQCThreshold( double qcThreshold )
{
    DisplayQCThresholdPlot( qcThreshold, m_atlas );
    m_updated = false;
}

void QCThresholdDialog::OnRemoveNAN()
{
    foreach( QString subject, m_nanSubjects )
    {
        if( m_matchedSubjects.contains( subject ) )
        {
            m_matchedSubjects.removeAll( subject );
        }
    }

    m_qcThresholdPlot->InitQCThresholdPlot( m_faData, m_matchedSubjects );
    DisplayQCThresholdPlot( m_qcThresholdDoubleSpinBox->value(), m_atlas );

    ui->QCThresholdDialog_removeNAN_pushButton->hide();
    ui->QCThresholdDialog_removeNAN_label->hide();

    emit NanSujects( m_nanSubjects );

    m_nanSubjects.clear();
}

void QCThresholdDialog::OnApplyQCThreshold()
{
    emit ApplyQCThreshold( m_subjectsCorrelated, m_subjectsNotCorrelated, m_qcThresholdDoubleSpinBox->value(), false );
    m_updated = true;
}

void QCThresholdDialog::closeEvent( QCloseEvent *event )
{
    if( m_updated )
    {
        emit ApplyQCThreshold( m_subjectsCorrelated, m_subjectsNotCorrelated, m_qcThresholdDoubleSpinBox->value(), true );

        m_subjectsCorrelated.clear();
        m_subjectsNotCorrelated.clear();
    }

    event->accept();
}




/***************************************************************/
/********************** Private functions **********************/
/***************************************************************/
void QCThresholdDialog::InitQCThresholdDialog()
{
    m_qvtkWidget = QSharedPointer< QVTKWidget >( ui->QCThresholdDialog_plot_qvtkWidget );

    m_qcThresholdPlot = new Plot();
    m_qcThresholdPlot->SetQVTKWidget( m_qvtkWidget );
    connect( m_qcThresholdPlot, SIGNAL( UpdateSubjectsCorrelated( const QStringList&, const QStringList& ) ), this, SLOT( OnUpdatingSubjectsCorrelated( const QStringList&, const QStringList& ) ) );

    m_qcThresholdDoubleSpinBox = new QDoubleSpinBox;
    m_qcThresholdDoubleSpinBox = ui->QCThresholdDialog_qcThreshold_doubleSpinBox;
    connect( m_qcThresholdDoubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnUpdatingQCThreshold( double ) ) );

    connect( ui->QCThresholdDialog_removeNAN_pushButton, SIGNAL( clicked() ), this, SLOT( OnRemoveNAN() ) );

    connect( ui->QCThresholdDialog_applyQCThreshold_pushButton, SIGNAL( clicked() ), this, SLOT( OnApplyQCThreshold() ) );
}


void QCThresholdDialog::DisplayQCThresholdPlot( double qcThreshold, const QStringList& altas )
{
    m_qcThresholdPlot->ClearPlot();

    m_qcThresholdPlot->SetQCThreshold() = qcThreshold;
    m_qcThresholdPlot->SetAtlasQCThreshold() = altas;
    m_qcThresholdPlot->DisplayQCThresholdPlot();
}
