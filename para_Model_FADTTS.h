#ifndef para_Model_FADTTS_H
#define para_Model_FADTTS_H

#include <QString>
#include <map>

class para_Model_FADTTS {
  bool para_parametersTab_omnibus_checkBox;
  bool para_parametersTab_postHoc_checkBox;
  bool para_subjectsTab_adInput_checkBox;
  bool para_subjectsTab_compInput_checkBox;
  bool para_subjectsTab_faInput_checkBox;
  bool para_subjectsTab_mdInput_checkBox;
  bool para_subjectsTab_rdInput_checkBox;
  QString para_inputsTab_fiberName_lineEdit;
  QString para_inputsTab_inputADfile_lineEdit;
  QString para_inputsTab_inputCOMPfile_lineEdit;
  QString para_inputsTab_inputFAfile_lineEdit;
  QString para_inputsTab_inputMDfile_lineEdit;
  QString para_inputsTab_inputRDfile_lineEdit;
  QString para_runTab_outputDir_lineEdit;
  QString para_subjectsTab_inputSubjectList_lineEdit;
  std::map<QString,bool> para_parametersTab_covariates_listWidget;
  int para_parametersTab_nbrPermutations_spinBox;

public: 

  bool getpara_parametersTab_omnibus_checkBox();
  void setpara_parametersTab_omnibus_checkBox(bool a);

  bool getpara_parametersTab_postHoc_checkBox();
  void setpara_parametersTab_postHoc_checkBox(bool a);

  bool getpara_subjectsTab_adInput_checkBox();
  void setpara_subjectsTab_adInput_checkBox(bool a);

  bool getpara_subjectsTab_compInput_checkBox();
  void setpara_subjectsTab_compInput_checkBox(bool a);

  bool getpara_subjectsTab_faInput_checkBox();
  void setpara_subjectsTab_faInput_checkBox(bool a);

  bool getpara_subjectsTab_mdInput_checkBox();
  void setpara_subjectsTab_mdInput_checkBox(bool a);

  bool getpara_subjectsTab_rdInput_checkBox();
  void setpara_subjectsTab_rdInput_checkBox(bool a);

  QString getpara_inputsTab_fiberName_lineEdit();
  void setpara_inputsTab_fiberName_lineEdit(QString a);

  QString getpara_inputsTab_inputADfile_lineEdit();
  void setpara_inputsTab_inputADfile_lineEdit(QString a);

  QString getpara_inputsTab_inputCOMPfile_lineEdit();
  void setpara_inputsTab_inputCOMPfile_lineEdit(QString a);

  QString getpara_inputsTab_inputFAfile_lineEdit();
  void setpara_inputsTab_inputFAfile_lineEdit(QString a);

  QString getpara_inputsTab_inputMDfile_lineEdit();
  void setpara_inputsTab_inputMDfile_lineEdit(QString a);

  QString getpara_inputsTab_inputRDfile_lineEdit();
  void setpara_inputsTab_inputRDfile_lineEdit(QString a);

  QString getpara_runTab_outputDir_lineEdit();
  void setpara_runTab_outputDir_lineEdit(QString a);

  QString getpara_subjectsTab_inputSubjectList_lineEdit();
  void setpara_subjectsTab_inputSubjectList_lineEdit(QString a);

  std::map<QString,bool> getpara_parametersTab_covariates_listWidget();
  void setpara_parametersTab_covariates_listWidget(std::map<QString,bool> a);

  int getpara_parametersTab_nbrPermutations_spinBox();
  void setpara_parametersTab_nbrPermutations_spinBox(int a);

};

#endif
