#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QtCore>
#include <QtGui>

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
    
    void DisplayInfo();

    void SetData( Data *newData );


private:
    Ui::InfoDialog *m_ui;

    QLabel *m_adFileInfo_label, *m_rdFileInfo_label, *m_mdFileInfo_label,
    *m_faFileInfo_label, *m_compFileInfo_label;

    typedef QMap<QString, QLabel*> labelMapType;
    labelMapType m_fileInfoLabelMap;

    Data *m_data;


    void InitInfoDialog();

    void SetInfoFileLabelMap();

    QString GetFileInfo( const QString p );
};

#endif // INFODIALOG_H
