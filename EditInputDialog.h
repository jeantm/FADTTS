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

    void DisplayFileData();

    void SetData( Data *newData );

    void SetLineEditPrefix( const QString &newLineEditPrefixID );

    void SetInputFile( const QString &newInputFile );

    void SetCurrentInputDir( const QString &newCurrentInputDir );

    void ResetCovariateFileSubjectColumnID();


signals:
    void FilePathChanged( const QString&, const QString&  );

    void CovariateMapChanged( const QMap<int, QString>&  );

    void CovariateFileSubjectColumnIDChanged( const int& );


private slots:
    void DeleteRows();

    void DeleteColumns();

    void ChangeCovariateFileSubjectColumnID( int idColumn );

    bool SaveFile();


private:
    static const QString m_csvSeparator;

    Ui::EditInputDialog *m_ui;

    QTableWidget *m_dataTableWidget;

    QSpinBox *m_covariateFileSubjectColumnIDSpinBox;

    Data *m_data;

    QString m_lineEditPrefix, m_inputFile, m_currentInputDir;

    int m_covariateFileSubjectColumnID;

    bool m_rowDeleted, m_columnDeleted, m_isCovariateFile;


    void InitEditInputDialog();

    void UploadFileData();

    void ResetTableWidget();

    void closeEvent( QCloseEvent *event );
};

#endif // SETINPUT_H
