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
    Q_OBJECT
    

public:
    explicit InfoDialog( QWidget *parent = 0 );
    ~InfoDialog();
    
    void DisplayInfo();

    void SetData( Data *newData );


private:
    Ui::InfoDialog *m_ui;

    typedef QMap<QString, QLabel*> labelMapType;
    labelMapType m_fileInfoLabelMap;

    Data *m_data;


    QString GetFileInfo( const QString p );

    void SetFilePrefix();

    void SetInfoFileLabelMap();
};

#endif // INFODIALOG_H
