#include "QCThresholdDialog.h"
#include "ui_QCThresholdDialog.h"

QCThresholdDialog::QCThresholdDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QCThresholdDialog)
{
    ui->setupUi(this);
}

QCThresholdDialog::~QCThresholdDialog()
{
    delete ui;
}
