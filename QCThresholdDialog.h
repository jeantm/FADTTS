#ifndef QCTHRESHOLDDIALOG_H
#define QCTHRESHOLDDIALOG_H

#include <QDialog>

namespace Ui {
class QCThresholdDialog;
}

class QCThresholdDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit QCThresholdDialog(QWidget *parent = 0);
    ~QCThresholdDialog();
    
private:
    Ui::QCThresholdDialog *ui;
};

#endif // QCTHRESHOLDDIALOG_H
