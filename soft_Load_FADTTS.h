#ifndef soft_Load_FADTTS_H
#define soft_Load_FADTTS_H

#include <QCoreApplication>
#include <QString>
#include <QDebug>
#include <map>
#include <QFile>
#include <QStringList>
#include <iostream>
#include "soft_Model_FADTTS.h"
#include <QXmlStreamReader>


class soft_Load_FADTTS{

  QString tmpClassName;
  QString tmpClassType;
public:

  soft_Model_FADTTS load(soft_Model_FADTTS& model , std::string filename );

};
#endif