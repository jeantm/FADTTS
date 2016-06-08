#ifndef QCTHRESHOLDDIALOG_H
#define QCTHRESHOLDDIALOG_H

#include "Plot.h"

#include <QDialog>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>


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


    bool InitPlot( const QList< QStringList >& rawData, const QStringList& atlas, const QStringList& matchedSubjects, double qcThreshold ); // Not Directly Tested


signals:
    void ApplyQCThreshold( const QStringList&, const QStringList&, double qcThreshold, bool windowClosed );

    void NanSujects( const QStringList& );


private slots:
    void OnUpdatingSubjectsCorrelated( const QStringList& subjectsCorrelated, const QStringList& subjectsNotCorrelated ); // Not Directly Tested

    void OnUpdatingQCThreshold( double qcThreshold ); // Not Directly Tested

    void OnRemoveNAN();

    void OnApplyQCThreshold();

    void closeEvent( QCloseEvent *event );


private:
    Ui::QCThresholdDialog *ui;

    Processing m_processing;

    QSharedPointer< QVTKWidget > m_qvtkWidget;

    Plot *m_qcThresholdPlot;

    QDoubleSpinBox *m_qcThresholdDoubleSpinBox;

    QList< QStringList > m_faData;

    QStringList m_matchedSubjects,
    m_subjectsCorrelated, m_subjectsNotCorrelated, m_atlas,
    m_nanSubjects;

    bool m_updated;


    void InitQCThresholdDialog();  // Not Directly Tested

    void DisplayQCThresholdPlot( double qcThreshold, const QStringList& atlas ); // Not Directly Tested
};

#endif // QCTHRESHOLDDIALOG_H
