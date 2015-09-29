#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "EditInputDialog.h"
#include "InfoDialog.h"
#include "Data.h"
#include "Processing.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow( QWidget *parent = 0 );
    ~MainWindow();


private slots:
    /*************** Input Tab ***************/
    void on_inputAddDataFiles_pushButton_clicked();

    void on_inputInfo_pushButton_clicked();

    void on_outputDir_pushButton_clicked();

    void on_para_outputDir_lineEdit_textChanged( const QString& textLineEdit );


    void UpdateLineEditAfterFileEdition( const QString& newFilePathAfterFileEdition, const QString& prefID );

    void UpdateCovariatesAfterFileEdition( const QStringList& newCovariatesAfterFileEdition );

    void UpdateSubjectColumnIDAfterFileEdition( const int& newSubjectColumnIDAfterFileEdition );

    void AddFile( const QString& );

    void EditFile( const QString& );

    void UpdateInputLineEdit( const QString& );


    /*************** Subjects Tab ***************/
    void on_loadInputSubjectList_PushButton_clicked();

    void on_reset_pushButton_clicked();

    void on_para_inputSubjectList_lineEdit_textChanged( const QString& textLineEdit );

    void on_saveCheckedSubjectList_pushButton_clicked();


    void on_checkAllVisible_pushButton_clicked();

    void on_unCheckAllVisible_pushButton_clicked();

    void on_sortedSubjects_listWidget_itemClicked( QListWidgetItem *item );

    void on_para_search_lineEdit_textEdited();

    void on_caseSensitive_checkBox_toggled( bool checked );


    void SortSubjects();


    /***************** Parameters  Tab *****************/
    void on_covariates_listWidget_itemClicked( QListWidgetItem *item );

    void on_covariatesCheckAll_pushButton_clicked();

    void on_covariatesUncheckAll_pushButton_clicked();


    /******************** Run  Tab ********************/
    void on_run_pushButton_clicked();


private:
    static const QString m_csvSeparator;

    static const QColor m_green;
    static const QColor m_red;
    static const QColor m_grey;
    static const QColor m_yellow;
    static const QColor m_lightBlack;

    static const int m_IconSize;

    Ui::MainWindow *m_mainUi;

    EditInputDialog *m_editInputDialog;

    InfoDialog *m_infoDialog;

    QListWidget *m_sortedSubjectListWidget, *m_covariatesListWidget;

    QPixmap m_okPixmap;
    QPixmap m_koPixmap;
    QPixmap m_warningPixmap;

    typedef QMap<QString, QLabel*> labelMapType;
    labelMapType m_paramTabFileSizeLabelMap, m_inputTabIconLabelMap;

    typedef QMap<QString, QCheckBox*> checkBoxMapType;
    checkBoxMapType m_paramTabFileCheckBoxMap;

    typedef QMap<QString, QLineEdit*> lineEditMapType;
    lineEditMapType m_inputTabFilePathLineEditMap;

    typedef QMap<QString, QPushButton*> pushButtonMapType;
    pushButtonMapType m_inputTabAddFilePushButtonMap, m_inputTabEditFilePushButtonMap;

    Qt::CaseSensitivity caseSensitivity;

    QString m_currentFileInputDir, m_currentSubjectListInputDir;

    Data m_data;

    Processing m_processing;


    /***************** Other *****************/
    void Init();

    void InitInputTab();

    void InitSubjectTab();

    void UpdateCurrentDirEditInputDialog(const QString newfilePath, QString& currentDir );

    QDir UpdateCurrentDir(const QString newfilePath, QString& currentDir );

    void WarningPopUp( const QString warningText );

    void CriticalPopUp( const QString criticalText );

    void DisplayIcon( QLabel *label , const QPixmap icon );


    /*************** Input Tab ***************/
    void AddFiles( const QStringList fileList );

    void UpdateFileInformation( const QString prefID );

    bool IsMatrixDimensionOK( const QList<QStringList> data ); // Move

    void DisplayIcon( const QString prefID , const QPixmap icon );

    void SetInfoSubjectColumnID();

    void LaunchEditInputWindow( QString prefID );


    /*************** Subjects Tab ***************/
    void UpdateAvailableFileParamTab();

    QStringList GetRefSubjectList( const QString subjectListFilePath ); // Move

    QMap<QString, QStringList> GetAllSubjectsFromSelectedInputFiles(); // Move

    void DisplaySortedSubjectList( const QStringList subjectListRef, const QStringList matchedSubjectList, const QMap<QString, QStringList > unMatchedSubjectList );

    void Search();


    /*************** Parameters Tab ***************/
    void DisplayCovariatesList();


    /*************** Run Tab ***************/
    QString GenerateSelectedSubjectList();

    QStringList GetSelectedInputFiles();

    QList<int> GetSelectedCovariates();
};

#endif // MAINWINDOW_H
