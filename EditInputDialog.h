#ifndef EDITINPUTDIALOG_H
#define EDITINPUTDIALOG_H

#include "Data.h"
#include "Processing.h"

#include <QDialog>
#include <QTableWidget>
#include <QSpinBox>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>


namespace Ui {
class EditInputDialog;
}

class EditInputDialog : public QDialog
{
    friend class TestEditInputDialog; /** For unit tests **/
//    friend class TestFADTTSWindow; /** For unit tests **/

    Q_OBJECT

public:
    explicit EditInputDialog( QWidget *parent = 0 );
    ~EditInputDialog();


    void SetData( Data *newData );

    QString& SetCurrentInputDir(); // Tested

    void ResetCovariateColumnID(); // Tested


    void DisplayDataEdition( const int &newDiffusionPropertyIndex ); // Tested


signals:
    void UpdateInputFile( const int&, const QString&  );

    void UpdateCovariateColumnID( const int& );


private slots:
    void OnDeleteRows(); // Tested

    void OnDeleteColumns(); // Tested


    void OnCovariateColumnIDChanged( int columnID ); // Tested


    bool OnSaveFile(); // Tested


private:
    static const QString m_csvSeparator;

    Ui::EditInputDialog *m_ui;

    QTableWidget *m_dataTableWidget;

    QSpinBox *m_covariateColumnIDSpinBox;

    Data *m_data;

    QString m_currentDir;

    int m_diffusionPropertyIndex, m_covariateColumnID;

    bool m_rowDeleted, m_columnDeleted;


    void InitEditInputDialog();

    void LoadData(); // Tested

    void ResetTableWidget(); // Tested


    void closeEvent( QCloseEvent *event );
};

#endif // SETINPUT_H
