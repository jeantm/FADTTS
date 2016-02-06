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
    friend class TestFADTTSWindow; /** For unit tests **/

    Q_OBJECT

public:
    explicit EditInputDialog( QWidget *parent = 0 );
    ~EditInputDialog();


    void SetData( Data *newData );

    void DisplayDataEdition( const QString &newLineEditPrefixID, const QString &newInputFile );

    void SetCurrentInputDir( const QString &newCurrentInputDir );

    void ResetCovariateColumnID();


signals:
    void UpdateInputFile( const QString&, const QString&  );

    void UpdateCovariateColumnID( const int& );


private slots:
    void OnDeleteRows();

    void OnDeleteColumns();


    void OnCovariateColumnIDChanged( int columnID );


    bool OnSaveFile();


private:
    static const QString m_csvSeparator;

    Ui::EditInputDialog *m_ui;

    QTableWidget *m_dataTableWidget;

    QSpinBox *m_covariateColumnIDSpinBox;

    Data *m_data;

    QString m_lineEditPrefix, m_inputFile, m_currentInputDir;

    int m_covariateColumnID;

    bool m_rowDeleted, m_columnDeleted;


    void InitEditInputDialog();

    void LoadData();

    void ResetTableWidget();


    void closeEvent( QCloseEvent *event );
};

#endif // SETINPUT_H
