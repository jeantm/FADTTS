#include "soft_Save_FADTTS.h"
	void soft_Save_FADTTS::save(soft_Model_FADTTS& m , std::string filename){
		//First, address the appropriate instantiate
		QFile file(filename.c_str());
		if(!file.open(QIODevice::WriteOnly)){
			//handle file open errors here 
			 qDebug()<<file.exists();
		}

		QXmlStreamWriter writer(&file);

		//write each field stored in model into xml
		saveToXML(m,writer);

	}


	void soft_Save_FADTTS::saveToXML(soft_Model_FADTTS& m,QXmlStreamWriter& writer){

		writer.setAutoFormatting(true);

		writer.writeStartDocument();


		writer.writeStartElement("soft_Parameters");
		writer.writeEmptyElement("Parameter");
		writer.writeAttribute("type","QString");
		writer.writeAttribute("name","soft_runTab_matlab_lineEdit");
		writer.writeAttribute("value",m.getsoft_runTab_matlab_lineEdit());
		writer.writeEndElement();

		writer.writeEndDocument();

	}
