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


    void SetData( Data *newData ); // Tested

    void ResetSubjectColumnID(); // Tested


    void DisplayDataEdition( int newDiffusionPropertyIndex ); // Tested


signals:
    void UpdateInputFile( const int&, const QString& );

    void UpdateSubjectColumnID( const int& );


private slots:
    void OnDeleteRows(); // Tested

    void OnDeleteColumns(); // Tested


    void OnRemoveDuplicates();


    void OnSubjectColumnIDChanged( int columnID ); // Tested


    bool OnSaveFile(); // Tested


    void closeEvent( QCloseEvent *event ); /// Not tested


private:
    static const QString m_csvSeparator;

    Ui::EditInputDialog *m_ui;

    QTableWidget *m_dataTableWidget;

    QSpinBox *m_subjectColumnIDSpinBox;

    Data *m_data;

    QStringList firstOccurrences;

    QList< int > indexSecondeOccurrences;

    QString m_currentDir;

    int m_diffusionPropertyIndex, m_subjectColumnID;

    bool m_rowDeleted, m_columnDeleted;


    void InitEditInputDialog(); // Not Directly Tested

    void UpdateNAN();

    void LoadData(); // Not Directly Tested

    void GetDuplicates();

    void ResetTableWidget(); // Tested


    void SaveFile( QString newFilePath ); // Tested
};

#endif // SETINPUT_H
