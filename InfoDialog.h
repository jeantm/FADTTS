#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QtWidgets>

#include "Data.h"

namespace Ui {
class InfoDialog;
}

class InfoDialog : public QDialog
{
    friend class TestInfoDialog; /** For unit tests **/
    friend class TestFADTTSWindow; /** For unit tests **/

    Q_OBJECT    

public:
    explicit InfoDialog( QWidget *parent = 0 );
    ~InfoDialog();
    
    void DisplayFileInformation();

    void SetData( Data *newData );


private:
    Ui::InfoDialog *m_ui;

    QLabel *m_adFileInfo_label, *m_rdFileInfo_label, *m_mdFileInfo_label,
    *m_faFileInfo_label, *m_compFileInfo_label;

    typedef QMap<QString, QLabel*> labelMapType;
    labelMapType m_inputFileInformationLabelMap;

    Data *m_data;


    void InitInfoDialog();

    void SetFileInformationLabelMap();

    QString GetInputFileInformation( const QString prefID );
};

#endif // INFODIALOG_H
