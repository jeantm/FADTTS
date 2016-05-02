#include "QCThresholdDialog.h"
#include "ui_QCThresholdDialog.h"

#include <QDebug>

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
    m_rawData = rawData;
    m_matchedSubjects = matchedSubjects;
    m_atlas = atlas;

    m_qcThresholdDoubleSpinBox->setValue( qcThreshold );

    bool qcThresholdDialogReady = m_qcThresholdPlot->InitQCThresholdPlot( m_rawData, m_matchedSubjects );
    DisplayQCThresholdPlot( m_qcThresholdDoubleSpinBox->value(), m_atlas );

    return qcThresholdDialogReady;
}



/***************************************************************/
/************************ Private slots ************************/
/***************************************************************/
void QCThresholdDialog::OnUpdatingSubjectsCorrelated( const QStringList& subjectsCorrelated, const QStringList& subjectsNotCorrelated )
{
    m_subjectsCorrelated = subjectsCorrelated;
    m_subjectsNotCorrelated = subjectsNotCorrelated;

    QString info = !m_subjectsNotCorrelated.isEmpty() ? QString( QString::number( m_subjectsNotCorrelated.size() ) + " subject(s) not satisfying the threshold" ) : "";
    ui->QCThresholdDialog_info_label->setText( tr( qPrintable( info ) ) );
}

void QCThresholdDialog::OnUpdatingQCThreshold( double qcThreshold )
{
    DisplayQCThresholdPlot( qcThreshold, m_atlas );
}

void QCThresholdDialog::OnApplyQCThreshold()
{
    emit ApplyQCThreshold( m_subjectsCorrelated, m_subjectsNotCorrelated, m_qcThresholdDoubleSpinBox->value() );

    m_subjectsCorrelated.clear();
    m_subjectsNotCorrelated.clear();
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

    connect( ui->QCThresholdDialog_applyQCThreshold_pushButton, SIGNAL( clicked() ), this, SLOT( OnApplyQCThreshold() ) );
}


void QCThresholdDialog::DisplayQCThresholdPlot( double qcThreshold, QStringList altas )
{
    m_qcThresholdPlot->ClearPlot();

    m_qcThresholdPlot->SetQCThreshold() = qcThreshold;
    m_qcThresholdPlot->SetAtlasQCThreshold() = altas;
    m_qcThresholdPlot->DisplayQCThresholdPlot();
}

void QCThresholdDialog::closeEvent( QCloseEvent *event )
{
    event->accept();
}
