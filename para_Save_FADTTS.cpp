#include "para_Save_FADTTS.h"
	void para_Save_FADTTS::save(para_Model_FADTTS& m , std::string filename){
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


	void para_Save_FADTTS::saveToXML(para_Model_FADTTS& m,QXmlStreamWriter& writer){

		writer.setAutoFormatting(true);

		writer.writeStartDocument();


		writer.writeStartElement("para_Parameters");
		writer.writeEmptyElement("Parameter");
		writer.writeAttribute("type","bool");
		writer.writeAttribute("name","para_parametersTab_omnibus_checkBox");
		writer.writeAttribute("value",QString::number(m.getpara_parametersTab_omnibus_checkBox()));
		writer.writeEmptyElement("Parameter");
		writer.writeAttribute("type","bool");
		writer.writeAttribute("name","para_parametersTab_postHoc_checkBox");
		writer.writeAttribute("value",QString::number(m.getpara_parametersTab_postHoc_checkBox()));
		writer.writeEmptyElement("Parameter");
		writer.writeAttribute("type","bool");
		writer.writeAttribute("name","para_subjectsTab_adInput_checkBox");
		writer.writeAttribute("value",QString::number(m.getpara_subjectsTab_adInput_checkBox()));
		writer.writeEmptyElement("Parameter");
		writer.writeAttribute("type","bool");
		writer.writeAttribute("name","para_subjectsTab_compInput_checkBox");
		writer.writeAttribute("value",QString::number(m.getpara_subjectsTab_compInput_checkBox()));
		writer.writeEmptyElement("Parameter");
		writer.writeAttribute("type","bool");
		writer.writeAttribute("name","para_subjectsTab_faInput_checkBox");
		writer.writeAttribute("value",QString::number(m.getpara_subjectsTab_faInput_checkBox()));
		writer.writeEmptyElement("Parameter");
		writer.writeAttribute("type","bool");
		writer.writeAttribute("name","para_subjectsTab_mdInput_checkBox");
		writer.writeAttribute("value",QString::number(m.getpara_subjectsTab_mdInput_checkBox()));
		writer.writeEmptyElement("Parameter");
		writer.writeAttribute("type","bool");
		writer.writeAttribute("name","para_subjectsTab_rdInput_checkBox");
		writer.writeAttribute("value",QString::number(m.getpara_subjectsTab_rdInput_checkBox()));
		writer.writeEmptyElement("Parameter");
		writer.writeAttribute("type","QString");
		writer.writeAttribute("name","para_inputsTab_fiberName_lineEdit");
		writer.writeAttribute("value",m.getpara_inputsTab_fiberName_lineEdit());
		writer.writeEmptyElement("Parameter");
		writer.writeAttribute("type","QString");
		writer.writeAttribute("name","para_inputsTab_inputADfile_lineEdit");
		writer.writeAttribute("value",m.getpara_inputsTab_inputADfile_lineEdit());
		writer.writeEmptyElement("Parameter");
		writer.writeAttribute("type","QString");
		writer.writeAttribute("name","para_inputsTab_inputCOMPfile_lineEdit");
		writer.writeAttribute("value",m.getpara_inputsTab_inputCOMPfile_lineEdit());
		writer.writeEmptyElement("Parameter");
		writer.writeAttribute("type","QString");
		writer.writeAttribute("name","para_inputsTab_inputFAfile_lineEdit");
		writer.writeAttribute("value",m.getpara_inputsTab_inputFAfile_lineEdit());
		writer.writeEmptyElement("Parameter");
		writer.writeAttribute("type","QString");
		writer.writeAttribute("name","para_inputsTab_inputMDfile_lineEdit");
		writer.writeAttribute("value",m.getpara_inputsTab_inputMDfile_lineEdit());
		writer.writeEmptyElement("Parameter");
		writer.writeAttribute("type","QString");
		writer.writeAttribute("name","para_inputsTab_inputRDfile_lineEdit");
		writer.writeAttribute("value",m.getpara_inputsTab_inputRDfile_lineEdit());
		writer.writeEmptyElement("Parameter");
		writer.writeAttribute("type","QString");
		writer.writeAttribute("name","para_runTab_outputDir_lineEdit");
		writer.writeAttribute("value",m.getpara_runTab_outputDir_lineEdit());
		writer.writeEmptyElement("Parameter");
		writer.writeAttribute("type","QString");
		writer.writeAttribute("name","para_subjectsTab_inputSubjectList_lineEdit");
		writer.writeAttribute("value",m.getpara_subjectsTab_inputSubjectList_lineEdit());
		writer.writeEmptyElement("Parameter");
		writer.writeAttribute("type","std::map<QString,bool>");
		writer.writeAttribute("name","para_parametersTab_covariates_listWidget");
		std::map<QString,bool> map = m.getpara_parametersTab_covariates_listWidget();
		std::map<QString,bool>::iterator it = map.begin();
		for( int count = 0 ; it != map.end() ; count++ , it++ )
		{
			std::string item = "item" + QString::number(count).toStdString() ;
			std::string itemName = item+"Name" ;
			writer.writeAttribute(itemName.c_str(),it->first);
			std::string itemState = item+"isChecked" ;
			writer.writeAttribute(itemState.c_str(),QString::number(it->second));
		}
		writer.writeEmptyElement("Parameter");
		writer.writeAttribute("type","int");
		writer.writeAttribute("name","para_parametersTab_nbrPermutations_spinBox");
		writer.writeAttribute("value",QString::number(m.getpara_parametersTab_nbrPermutations_spinBox()));
		writer.writeEndElement();

		writer.writeEndDocument();

	}
