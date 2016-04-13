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
    void OnApplyQCThreshold();


public:
    explicit QCThresholdDialog(QWidget *parent = 0);
    ~QCThresholdDialog();


    bool InitPlot( const QMap< QString, QList< QStringList > >& rawData, const QStringList& matchedSubjects, double qcThreshold );

signals:
    void ApplyQCThreshold( const QStringList&, const QStringList&, double qcThreshold );


private slots:
    void OnSettingAllPropertiesUsed( const QMap< int, QString >& allPropertiesUsed );

    void OnUpdatingSubjectsCorrelated( const QStringList& subjectsCorrelated, const QStringList& subjectsNotCorrelated );

    void OnPropertySelection( const QString& propertySelected );

    void OnUpdatingQCThreshold( double qcThreshold );


private:
    Ui::QCThresholdDialog *ui;

    QSharedPointer< QVTKWidget > m_qvtkWidget;

    Plot *m_qcThresholdPlot;

    QDoubleSpinBox *m_qcThresholdDoubleSpinBox;

    QMap< QString, QList< QStringList > > m_rawData;

    QStringList m_matchedSubjects, m_subjectsCorrelated, m_subjectsNotCorrelated;


    void InitQCThresholdDialog();

    void DisplayQCThresholdPlot( double qcThreshold, QString property );


    void closeEvent( QCloseEvent *event );
};

#endif // QCTHRESHOLDDIALOG_H
