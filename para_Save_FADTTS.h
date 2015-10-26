#ifndef para_Save_FADTTS_H
#define para_Save_FADTTS_H
#include <QXmlStreamWriter>
#include <QFile>

#include <QString>

#include <string>

#include "para_Model_FADTTS.h"

#include <QDebug>

class para_Save_FADTTS{ 

public: 

	void save(para_Model_FADTTS& m , std::string filename );
	void saveToXML(para_Model_FADTTS& m,QXmlStreamWriter& writer);
};
#endif
