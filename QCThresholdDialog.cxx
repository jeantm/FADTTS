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
bool QCThresholdDialog::InitPlot( const QMap< QString, QList< QStringList > >& rawData, const QStringList& matchedSubjects, double qcThreshold )
{
    m_rawData.clear();
    m_matchedSubjects.clear();

    m_rawData = rawData;
    m_matchedSubjects = matchedSubjects;

    m_qcThresholdDoubleSpinBox->setValue( qcThreshold );

    bool qcThresholdDialogReady = m_qcThresholdPlot->InitQCThresholdPlot( m_rawData, m_matchedSubjects );
    if( rawData.keys().contains( "FA" ) )
    {
        ui->QCThresholdDialog_properies_comboBox->setCurrentText( "FA" );
    }
    else
    {
        DisplayQCThresholdPlot( m_qcThresholdDoubleSpinBox->value(), ui->QCThresholdDialog_properies_comboBox->currentText() );
    }

    return qcThresholdDialogReady;
}



/***************************************************************/
/************************ Private slots ************************/
/***************************************************************/
void QCThresholdDialog::OnSettingAllPropertiesUsed( const QMap< int, QString >& allPropertiesUsed )
{
    if( !allPropertiesUsed.isEmpty() )
    {
        ui->QCThresholdDialog_properies_comboBox->clear();
        ui->QCThresholdDialog_properies_comboBox->addItems( allPropertiesUsed.values() );
    }
}

void QCThresholdDialog::OnUpdatingSubjectsCorrelated( const QStringList& subjectsCorrelated, const QStringList& subjectsNotCorrelated )
{
    m_subjectsCorrelated = subjectsCorrelated;
    m_subjectsNotCorrelated = subjectsNotCorrelated;

    QString info = !m_subjectsNotCorrelated.isEmpty() ? QString( QString::number( m_subjectsNotCorrelated.size() ) + " subject(s) not satisfying the threshold" ) : "";
    ui->QCThresholdDialog_info_label->setText( tr( qPrintable( info ) ) );
}

void QCThresholdDialog::OnPropertySelection( const QString& propertySelected )
{
    DisplayQCThresholdPlot( m_qcThresholdDoubleSpinBox->value(), propertySelected );
}

void QCThresholdDialog::OnUpdatingQCThreshold( double qcThreshold )
{
    DisplayQCThresholdPlot( qcThreshold, ui->QCThresholdDialog_properies_comboBox->currentText() );
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
    connect( m_qcThresholdPlot, SIGNAL( AllPropertiesUsed( const QMap< int, QString >& ) ), this, SLOT( OnSettingAllPropertiesUsed( const QMap< int, QString >& ) ) );
    connect( m_qcThresholdPlot, SIGNAL( UpdateSubjectsCorrelated( const QStringList&, const QStringList& ) ), this, SLOT( OnUpdatingSubjectsCorrelated( const QStringList&, const QStringList& ) ) );

    connect( ui->QCThresholdDialog_properies_comboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( OnPropertySelection( const QString& ) ) );

    m_qcThresholdDoubleSpinBox = new QDoubleSpinBox;
    m_qcThresholdDoubleSpinBox = ui->QCThresholdDialog_qcThreshold_doubleSpinBox;
    connect( m_qcThresholdDoubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnUpdatingQCThreshold( double ) ) );

    connect( ui->QCThresholdDialog_applyQCThreshold_pushButton, SIGNAL( clicked() ), this, SLOT( OnApplyQCThreshold() ) );
}


void QCThresholdDialog::DisplayQCThresholdPlot( double qcThreshold, QString property )
{
    m_qcThresholdPlot->ClearPlot();

    m_qcThresholdPlot->SetQCThreshold() = qcThreshold;
    m_qcThresholdPlot->DisplayQCThresholdPlot( property );
}

void QCThresholdDialog::closeEvent( QCloseEvent *event )
{
    event->accept();
}
