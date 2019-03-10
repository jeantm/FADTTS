#ifndef QCTHRESHOLDDIALOG_H
#define QCTHRESHOLDDIALOG_H

#include "Plot.h"

#include <QDialog>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QAbstractSlider>


namespace Ui {
class QCThresholdDialog;
}

class QCThresholdDialog : public QDialog
{
    friend class TestQCThresholdDialog; /** For unit tests **/
    friend class TestFADTTSWindow; /** For unit tests **/

    Q_OBJECT
    

public slots:



public:
    explicit QCThresholdDialog(QWidget *parent = 0);
    ~QCThresholdDialog();


    bool InitPlot( const QList< QStringList >& rawData, const QStringList& atlas, const QStringList& matchedSubjects, double qcThreshold, const QString& valueStartArcLength, const QString&  valueEndArcLength ); // Not Directly Tested


signals:
    void ApplyQCThreshold( const QStringList&, const QStringList&, double qcThreshold, bool windowClosed );

    void NanSujects( const QStringList& );

    void UpdateStartArcLength( const QString& );

    void UpdateEndArcLength( const QString& );


private slots:
    void OnCroppingProfile();

    void OnResetProfileCropping();

    void OnCropProfileToggled( bool checked );

    void OnUpdatingSubjectsCorrelated( const QStringList& subjectsCorrelated, const QStringList& subjectsNotCorrelated ); // Not Directly Tested

    void OnUpdatingQCThreshold( double qcThreshold ); // Not Directly Tested

    void OnRemoveNAN();

    void OnApplyQCThreshold();

    void closeEvent( QCloseEvent *event );


private:
    Ui::QCThresholdDialog *ui;

    Processing m_processing;

    QSharedPointer< QVTKOpenGLWidget > m_qvtkWidget;

    Plot *m_qcThresholdPlot;

    QDoubleSpinBox *m_qcThresholdDoubleSpinBox;

    QAbstractSlider *m_arcLengthStartHorizontalSlider, *m_arcLengthEndHorizontalSlider;

    QList< QStringList > m_faData;

    QStringList m_matchedSubjects,
    m_subjectsCorrelated, m_subjectsNotCorrelated, m_atlas,
    m_nanSubjects;

    int m_currentSliderStartIndex, m_currentSliderEndIndex;

    bool m_updated;


    void InitQCThresholdDialog();  // Not Directly Tested

    void DisplayQCThresholdPlot( double qcThreshold ); // Not Directly Tested

    void SetProfileCropping( const QString& valueStartArcLength, const QString&  valueEndArcLength );

    void SetCroppingFeatureEnabled( bool enabled );
};

#endif // QCTHRESHOLDDIALOG_H
