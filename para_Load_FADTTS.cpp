#include "para_Load_FADTTS.h"

  para_Model_FADTTS para_Load_FADTTS::load(para_Model_FADTTS& model , std::string filename)
  {
    std::map<std::pair<QString, QString>, QString> xmlTokens;
    typedef std::map<std::pair<QString, QString>, QString> Dict;
    typedef Dict::const_iterator It;

    QFile* data = new QFile(filename.c_str());
    if (!data->open(QIODevice::ReadOnly | QIODevice::Text)) 
    {
      qDebug() << "Not read only";
    }

    QXmlStreamReader readXML(data);
    std::map<QString , QList< std::pair< QString , QString > > > attributes ;
    while(!readXML.atEnd() && !readXML.hasError()) 
    {
      QXmlStreamReader::TokenType token = readXML.readNext();
      if(token == QXmlStreamReader::StartDocument) 
      {
        continue;
      }
      else if (readXML.isEndElement() != true) 
      {
        if(readXML.attributes().size()>2)
        {
          QList< std::pair< QString , QString > > currentAttributes;
          for( int i = 0 ; i < readXML.attributes().size() ; i++ )
          {
            std::pair< QString , QString > xmlAttribute ;
            xmlAttribute = std::make_pair( readXML.attributes()[i].name().toString() , readXML.attributes()[i].value().toString() ) ;
            currentAttributes.append( xmlAttribute ) ;
          }
          attributes[ readXML.attributes().value("name").toString() ] = currentAttributes ;
          xmlTokens[std::make_pair(readXML.attributes().value("type").toString(), readXML.attributes().value("name").toString())] = readXML.attributes().value("value").toString();
        }
      }
    }

    tmpClassType = "bool";
    tmpClassName = "para_parametersTab_omnibus_checkBox";

    for (It it= xmlTokens.begin(); it != xmlTokens.end(); ++it) 
    {
      if((it -> first.second).compare(tmpClassName) == 0) 
      {
          int boolValue = ((QString)(it->second)).toInt();
          model.setpara_parametersTab_omnibus_checkBox(static_cast<bool>(boolValue));
      }
    }

    tmpClassType = "bool";
    tmpClassName = "para_parametersTab_postHoc_checkBox";

    for (It it= xmlTokens.begin(); it != xmlTokens.end(); ++it) 
    {
      if((it -> first.second).compare(tmpClassName) == 0) 
      {
          int boolValue = ((QString)(it->second)).toInt();
          model.setpara_parametersTab_postHoc_checkBox(static_cast<bool>(boolValue));
      }
    }

    tmpClassType = "bool";
    tmpClassName = "para_subjectsTab_adInput_checkBox";

    for (It it= xmlTokens.begin(); it != xmlTokens.end(); ++it) 
    {
      if((it -> first.second).compare(tmpClassName) == 0) 
      {
          int boolValue = ((QString)(it->second)).toInt();
          model.setpara_subjectsTab_adInput_checkBox(static_cast<bool>(boolValue));
      }
    }

    tmpClassType = "bool";
    tmpClassName = "para_subjectsTab_compInput_checkBox";

    for (It it= xmlTokens.begin(); it != xmlTokens.end(); ++it) 
    {
      if((it -> first.second).compare(tmpClassName) == 0) 
      {
          int boolValue = ((QString)(it->second)).toInt();
          model.setpara_subjectsTab_compInput_checkBox(static_cast<bool>(boolValue));
      }
    }

    tmpClassType = "bool";
    tmpClassName = "para_subjectsTab_faInput_checkBox";

    for (It it= xmlTokens.begin(); it != xmlTokens.end(); ++it) 
    {
      if((it -> first.second).compare(tmpClassName) == 0) 
      {
          int boolValue = ((QString)(it->second)).toInt();
          model.setpara_subjectsTab_faInput_checkBox(static_cast<bool>(boolValue));
      }
    }

    tmpClassType = "bool";
    tmpClassName = "para_subjectsTab_mdInput_checkBox";

    for (It it= xmlTokens.begin(); it != xmlTokens.end(); ++it) 
    {
      if((it -> first.second).compare(tmpClassName) == 0) 
      {
          int boolValue = ((QString)(it->second)).toInt();
          model.setpara_subjectsTab_mdInput_checkBox(static_cast<bool>(boolValue));
      }
    }

    tmpClassType = "bool";
    tmpClassName = "para_subjectsTab_rdInput_checkBox";

    for (It it= xmlTokens.begin(); it != xmlTokens.end(); ++it) 
    {
      if((it -> first.second).compare(tmpClassName) == 0) 
      {
          int boolValue = ((QString)(it->second)).toInt();
          model.setpara_subjectsTab_rdInput_checkBox(static_cast<bool>(boolValue));
      }
    }

    tmpClassType = "QString";
    tmpClassName = "para_inputsTab_fiberName_lineEdit";

    for (It it= xmlTokens.begin(); it != xmlTokens.end(); ++it) 
    {
      if((it -> first.second).compare(tmpClassName) == 0) 
      {
          model.setpara_inputsTab_fiberName_lineEdit((QString)(it->second));
      }
    }

    tmpClassType = "QString";
    tmpClassName = "para_inputsTab_inputADfile_lineEdit";

    for (It it= xmlTokens.begin(); it != xmlTokens.end(); ++it) 
    {
      if((it -> first.second).compare(tmpClassName) == 0) 
      {
          model.setpara_inputsTab_inputADfile_lineEdit((QString)(it->second));
      }
    }

    tmpClassType = "QString";
    tmpClassName = "para_inputsTab_inputCOMPfile_lineEdit";

    for (It it= xmlTokens.begin(); it != xmlTokens.end(); ++it) 
    {
      if((it -> first.second).compare(tmpClassName) == 0) 
      {
          model.setpara_inputsTab_inputCOMPfile_lineEdit((QString)(it->second));
      }
    }

    tmpClassType = "QString";
    tmpClassName = "para_inputsTab_inputFAfile_lineEdit";

    for (It it= xmlTokens.begin(); it != xmlTokens.end(); ++it) 
    {
      if((it -> first.second).compare(tmpClassName) == 0) 
      {
          model.setpara_inputsTab_inputFAfile_lineEdit((QString)(it->second));
      }
    }

    tmpClassType = "QString";
    tmpClassName = "para_inputsTab_inputMDfile_lineEdit";

    for (It it= xmlTokens.begin(); it != xmlTokens.end(); ++it) 
    {
      if((it -> first.second).compare(tmpClassName) == 0) 
      {
          model.setpara_inputsTab_inputMDfile_lineEdit((QString)(it->second));
      }
    }

    tmpClassType = "QString";
    tmpClassName = "para_inputsTab_inputRDfile_lineEdit";

    for (It it= xmlTokens.begin(); it != xmlTokens.end(); ++it) 
    {
      if((it -> first.second).compare(tmpClassName) == 0) 
      {
          model.setpara_inputsTab_inputRDfile_lineEdit((QString)(it->second));
      }
    }

    tmpClassType = "QString";
    tmpClassName = "para_runTab_outputDir_lineEdit";

    for (It it= xmlTokens.begin(); it != xmlTokens.end(); ++it) 
    {
      if((it -> first.second).compare(tmpClassName) == 0) 
      {
          model.setpara_runTab_outputDir_lineEdit((QString)(it->second));
      }
    }

    tmpClassType = "QString";
    tmpClassName = "para_subjectsTab_inputSubjectList_lineEdit";

    for (It it= xmlTokens.begin(); it != xmlTokens.end(); ++it) 
    {
      if((it -> first.second).compare(tmpClassName) == 0) 
      {
          model.setpara_subjectsTab_inputSubjectList_lineEdit((QString)(it->second));
      }
    }

    tmpClassType = "std::map<QString,bool>";
    tmpClassName = "para_parametersTab_covariates_listWidget";

    for (It it= xmlTokens.begin(); it != xmlTokens.end(); ++it) 
    {
      if((it -> first.second).compare(tmpClassName) == 0) 
      {
          std::map<QString,bool> para_parametersTab_covariates_listWidgetMap ;
          QList< std::pair< QString , QString > > currentAttributes = attributes[ "para_parametersTab_covariates_listWidget" ];
          if( currentAttributes.size() % 2 == 1 || currentAttributes.size() < 4 )
          {
            std::cerr << "Wrong number of attributes for "<< it->first.second.toStdString() << ":" << currentAttributes.size() << std::endl ;
            continue ;
          }
          currentAttributes.removeFirst();
          currentAttributes.removeFirst();
          int itemNb = 0 ;
          while( currentAttributes.size() > 1 )
          {
            std::pair<QString , QString> attributeName = currentAttributes[ 0 ];
            QString name = QString("item") + QString::number(itemNb) + "Name";
            std::pair<QString , QString> attributeIsChecked = currentAttributes[ 1 ];
            QString isChecked = QString("item") + QString::number(itemNb) + "isChecked";
            if( !attributeName.first.compare(name) && !attributeIsChecked.first.compare(isChecked))
            {
              para_parametersTab_covariates_listWidgetMap[ attributeName.second ] = ( attributeIsChecked.second.toInt() != 0 ? true : false ) ;
            }
            currentAttributes.removeFirst();
            currentAttributes.removeFirst();
            itemNb++;
          }
          model.setpara_parametersTab_covariates_listWidget(para_parametersTab_covariates_listWidgetMap) ;
       }
    }

    tmpClassType = "int";
    tmpClassName = "para_parametersTab_nbrPermutations_spinBox";

    for (It it= xmlTokens.begin(); it != xmlTokens.end(); ++it) 
    {
      if((it -> first.second).compare(tmpClassName) == 0) 
      {
          int intValue = ((QString)(it->second)).toInt();
          model.setpara_parametersTab_nbrPermutations_spinBox(intValue);
      }
    }

    return model;
  }