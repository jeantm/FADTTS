#ifndef soft_Save_FADTTS_H
#define soft_Save_FADTTS_H
#include <QXmlStreamWriter>
#include <QFile>

#include <QString>

#include <string>

#include "soft_Model_FADTTS.h"

#include <QDebug>

class soft_Save_FADTTS{ 

public: 

	void save(soft_Model_FADTTS& m , std::string filename );
	void saveToXML(soft_Model_FADTTS& m,QXmlStreamWriter& writer);
};
#endif
