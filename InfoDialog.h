#ifndef INFODIALOG_H
#define INFODIALOG_H

#include "Data.h"

#include <QDialog>
#include <QLabel>
#include <QFile>
#include <QFileInfo>


class InfoDialog : public QObject
{
    friend class TestInfoDialog; /** For unit tests **/
    friend class TestFADTTSWindow; /** For unit tests **/

    Q_OBJECT

public:
    explicit InfoDialog( QObject *parent = 0 );


    void DisplayFileInformation();

    void SetData( Data *data );

    void SetInformationLabelMap( QMap<QString, QLabel*> informationLabelMap );


private:
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
