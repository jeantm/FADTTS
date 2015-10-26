#ifndef para_Load_FADTTS_H
#define para_Load_FADTTS_H

#include <QCoreApplication>
#include <QString>
#include <QDebug>
#include <map>
#include <QFile>
#include <QStringList>
#include <iostream>
#include "para_Model_FADTTS.h"
#include <QXmlStreamReader>


class para_Load_FADTTS{

  QString tmpClassName;
  QString tmpClassType;
public:

  para_Model_FADTTS load(para_Model_FADTTS& model , std::string filename );

};
#endif