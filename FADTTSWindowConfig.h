#ifndef FADTTSWINDOWCONFIG_H
#define FADTTSWINDOWCONFIG_H

#include "ui_FADTTSWindow.h"

#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

class FADTTSWindowConfig :public QMainWindow, public Ui::FADTTSWindow
{
    Q_OBJECT

public:

    FADTTSWindowConfig( QWidget *parent = 0, Qt::WindowFlags f = 0 );



};


#endif // FADTTSWINDOWCONFIG_H
