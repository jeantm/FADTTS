#include "QCThresholdDialog.h"
#include "ui_QCThresholdDialog.h"

// #include <QDebug>

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
bool QCThresholdDialog::InitPlot( const QList< QStringList >& rawData, const QStringList& atlas, const QStringList& matchedSubjects, double qcThreshold, const QString& valueStartArcLength, const QString&  valueEndArcLength )
{
    m_faData = rawData;
    m_atlas = atlas;
    m_matchedSubjects = matchedSubjects;

    m_nanSubjects = m_processing.GetNANSubjects( m_faData, m_matchedSubjects );

    ui->QCThresholdDialog_removeNAN_pushButton->setHidden( m_nanSubjects.isEmpty() );
    ui->QCThresholdDialog_removeNAN_label->setHidden( m_nanSubjects.isEmpty() );

    m_qcThresholdDoubleSpinBox->blockSignals( true );
    m_qcThresholdDoubleSpinBox->setValue( qcThreshold );
    m_qcThresholdDoubleSpinBox->blockSignals( false );

    m_arcLengthStartHorizontalSlider->blockSignals( true );
    m_arcLengthEndHorizontalSlider->blockSignals( true );
    SetProfileCropping( valueStartArcLength, valueEndArcLength );
    m_arcLengthStartHorizontalSlider->blockSignals( false );
    m_arcLengthEndHorizontalSlider->blockSignals( false );

    bool qcThresholdDialogReady = m_qcThresholdPlot->InitQCThresholdPlot( m_faData, m_matchedSubjects );
    DisplayQCThresholdPlot( m_qcThresholdDoubleSpinBox->value() );

    m_updated = false;

    return qcThresholdDialogReady;
}



/***************************************************************/
/************************ Private slots ************************/
/***************************************************************/
void QCThresholdDialog::OnCroppingProfile()
{
    int arcLengthStartIndex = m_arcLengthStartHorizontalSlider->value();
    int arcLengthEndIndex = m_arcLengthEndHorizontalSlider->maximum() - m_arcLengthEndHorizontalSlider->value();
    bool startInfEnd = arcLengthStartIndex < arcLengthEndIndex;

    if( startInfEnd )
    {
        m_qcThresholdPlot->SetArcLengthStartIndex() = arcLengthStartIndex;
        m_qcThresholdPlot->SetArcLengthEndIndex() = arcLengthEndIndex;

        m_qcThresholdPlot->UpdateCropping();

        m_currentSliderStartIndex = arcLengthStartIndex;
        m_currentSliderEndIndex = m_arcLengthEndHorizontalSlider->value();
    }
    else
    {
        m_arcLengthStartHorizontalSlider->setValue( m_currentSliderStartIndex );
        m_arcLengthEndHorizontalSlider->setValue( m_currentSliderEndIndex );
    }
}

void QCThresholdDialog::OnResetProfileCropping()
{
    m_arcLengthStartHorizontalSlider->setValue( m_arcLengthStartHorizontalSlider->minimum() );
    m_arcLengthEndHorizontalSlider->setValue( m_arcLengthStartHorizontalSlider->minimum() );
}

void QCThresholdDialog::OnCropProfileToggled( bool checked )
{
    SetCroppingFeatureEnabled( checked );
    m_qcThresholdPlot->SetCroppingEnabled() = checked;
    m_qcThresholdPlot->ShowHideProfileCropping( checked );
}

void QCThresholdDialog::OnUpdatingSubjectsCorrelated( const QStringList& subjectsCorrelated, const QStringList& subjectsNotCorrelated )
{
    m_subjectsCorrelated = subjectsCorrelated;
    m_subjectsNotCorrelated = subjectsNotCorrelated;

    QString info = !m_subjectsNotCorrelated.isEmpty() ? QString( "<i><span style=""font-size:8pt;"">" + QString::number( m_subjectsNotCorrelated.size() ) + " subject(s) not satisfying the threshold</span></i>" ) : "";
    ui->QCThresholdDialog_info_label->setText( tr( qPrintable( info ) ) );
}

void QCThresholdDialog::OnUpdatingQCThreshold( double qcThreshold )
{
    m_qcThresholdPlot->UpdateQCThreshold( qcThreshold, true );
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
        if( m_subjectsCorrelated.contains( subject ) )
        {
            m_subjectsCorrelated.removeAll( subject );
        }
        if( m_subjectsNotCorrelated.contains( subject ) )
        {
            m_subjectsNotCorrelated.removeAll( subject );
        }
    }

    m_qcThresholdPlot->UpdateNAN( m_nanSubjects );

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

    if( ui->QCThresholdDialog_cropProfile_checkBox->isChecked() )
    {
        QStringList arcLength = m_processing.Transpose( m_faData ).first();
        arcLength.removeFirst();

        emit UpdateStartArcLength( arcLength.at( m_arcLengthStartHorizontalSlider->value() ) );
        emit UpdateEndArcLength( arcLength.at( m_arcLengthEndHorizontalSlider->maximum() - m_arcLengthEndHorizontalSlider->value() ) );
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
    m_qcThresholdPlot->SetQVTKWidget( m_qvtkWidget, true );
    connect( m_qcThresholdPlot, SIGNAL( UpdateSubjectsCorrelated( const QStringList&, const QStringList& ) ), this, SLOT( OnUpdatingSubjectsCorrelated( const QStringList&, const QStringList& ) ) );

    m_qcThresholdDoubleSpinBox = new QDoubleSpinBox;
    m_qcThresholdDoubleSpinBox = ui->QCThresholdDialog_qcThreshold_doubleSpinBox;
    connect( m_qcThresholdDoubleSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( OnUpdatingQCThreshold( double ) ) );
    connect( ui->QCThresholdDialog_applyQCThreshold_pushButton, SIGNAL( clicked() ), this, SLOT( OnApplyQCThreshold() ) );

    connect( ui->QCThresholdDialog_removeNAN_pushButton, SIGNAL( clicked() ), this, SLOT( OnRemoveNAN() ) );

    m_arcLengthStartHorizontalSlider = new QAbstractSlider;
    m_arcLengthStartHorizontalSlider = ui->QCThresholdDialog_arcLengthStart_horizontalSlider;
    m_arcLengthEndHorizontalSlider = new QAbstractSlider;
    m_arcLengthEndHorizontalSlider = ui->QCThresholdDialog_arcLengthEnd_horizontalSlider;
    connect( m_arcLengthStartHorizontalSlider, SIGNAL( valueChanged( int ) ), this, SLOT( OnCroppingProfile() ) );
    connect( m_arcLengthEndHorizontalSlider, SIGNAL( valueChanged( int ) ), this, SLOT( OnCroppingProfile() ) );
    connect( ui->QCThresholdDialog_resetArcLength_pushButton, SIGNAL( clicked() ), this, SLOT( OnResetProfileCropping() ) );
    connect( ui->QCThresholdDialog_cropProfile_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( OnCropProfileToggled( bool ) ) );

    SetCroppingFeatureEnabled( false );
}


void QCThresholdDialog::DisplayQCThresholdPlot( double qcThreshold )
{
    int arcLengthStartIndex = m_arcLengthStartHorizontalSlider->value();
    int arcLengthEndIndex = m_arcLengthEndHorizontalSlider->maximum() - m_arcLengthEndHorizontalSlider->value();

    m_qcThresholdPlot->ClearPlot();
    m_qcThresholdPlot->SetQCThreshold() = qcThreshold;
    m_qcThresholdPlot->SetAtlasQCThreshold() = m_atlas;
    m_qcThresholdPlot->SetCroppingEnabled() = ui->QCThresholdDialog_cropProfile_checkBox->isChecked();
    if( arcLengthStartIndex < arcLengthEndIndex )
    {
        m_qcThresholdPlot->SetArcLengthStartIndex() = arcLengthStartIndex;
        m_qcThresholdPlot->SetArcLengthEndIndex() = arcLengthEndIndex;
    }

    m_qcThresholdPlot->DisplayQCThresholdPlot();
}


void QCThresholdDialog::SetProfileCropping( const QString& valueStartArcLength, const QString&  valueEndArcLength )
{
    QStringList arcLength = m_processing.Transpose( m_faData ).first();
    arcLength.removeFirst();

    m_currentSliderStartIndex = arcLength.indexOf( valueStartArcLength, 0 );
    m_currentSliderEndIndex = arcLength.indexOf( valueEndArcLength, 0 );

    m_arcLengthStartHorizontalSlider->setRange( 0, arcLength.size() - 1 );
    m_arcLengthStartHorizontalSlider->setValue( m_currentSliderStartIndex );

    m_arcLengthEndHorizontalSlider->setRange( 0, arcLength.size() - 1 );
    m_arcLengthEndHorizontalSlider->setValue( m_arcLengthEndHorizontalSlider->maximum() - m_currentSliderEndIndex );
}

void QCThresholdDialog::SetCroppingFeatureEnabled( bool enabled )
{
    m_arcLengthStartHorizontalSlider->setEnabled( enabled );
    m_arcLengthEndHorizontalSlider->setEnabled( enabled );
    ui->QCThresholdDialog_resetArcLength_pushButton->setEnabled( enabled );
}
