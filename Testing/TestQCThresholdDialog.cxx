#include "TestQCThresholdDialog.h"

TestQCThresholdDialog::TestQCThresholdDialog()
{
}

/**********************************************************************/
/*************************** Tests ************************************/
/**********************************************************************/
bool TestQCThresholdDialog::Test_OnApplyQCThreshold()
{
    QSharedPointer< QCThresholdDialog > qcThresholdDialog = QSharedPointer< QCThresholdDialog >( new QCThresholdDialog );

    qcThresholdDialog->m_subjectsCorrelated = QStringList() << "Marcus Paige" << "Joel Berry II" << "Kennedy Meeks" << "Brice Johnson" << "Justin Jackson";
    qcThresholdDialog->m_subjectsNotCorrelated = QStringList() << "None";
    qcThresholdDialog->m_qcThresholdDoubleSpinBox->setValue() = 0.11;


    qcThresholdDialog->OnApplyQCThreshold();


    bool testOnApplyQCThreshold_Passed = false;
    if( !testOnApplyQCThreshold_Passed )
    {
        std::cerr << "/!\\/!\\ Test_OnApplyQCThreshold() FAILED /!\\ /!\\";
        std::cerr << std::endl;
    }
    else
    {
        std::cerr << "Test_OnApplyQCThreshold() PASSED";
    }

    return testOnApplyQCThreshold_Passed;
}
