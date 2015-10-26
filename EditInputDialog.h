#ifndef EDITINPUTDIALOG_H
#define EDITINPUTDIALOG_H

#include <QtCore>
#include <QtGui>

#include "Data.h"
#include "Processing.h"

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

    void DisplayData();

    void SetData( Data *newData );

    void SetPrefix( const QString &prefixID );

    void SetInputFile( const QString &inputFile );

    void SetCurrentInputDir( const QString &newCurrentInputDir );

    void ResetSubjectColumnID();


signals:
    void FilePathChanged( const QString&, const QString&  );

    void CovariatesChanged( const QMap<int, QString>&  );

    void SubjectColumnIDChanged( const int& );


private slots:
    void DeleteRows();

    void DeleteColumns();

    void ChangeSubjectColumnID( int idColumn );

    void Save();


private:
    static const QString m_csvSeparator;

    Ui::EditInputDialog *m_ui;

    QTableWidget *m_dataTableWidget;

    QSpinBox *m_subjectColumnSpinBox;

    QLabel *m_fileInformationLabel, *m_subjectColumnLabel;

    QString m_prefix, m_inputFile, m_currentInputDir;

    QMap<int, QString> m_covariates;

    bool m_rowDeleted, m_columnDeleted, m_isCOMP;

    int m_subjectColumnID;

    Data *m_data;


    void InitEditInputDialog();

    void UploadData();

    void RefreshFileInfo();

    bool SaveCSVFile();

    void ResetTableWidget();

    void closeEvent( QCloseEvent *event );
};

#endif // SETINPUT_H
