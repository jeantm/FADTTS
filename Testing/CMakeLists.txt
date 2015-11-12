cmake_minimum_required(VERSION 2.8)

# Add external data from midas
list(APPEND ExternalData_URL_TEMPLATES
"http://slicer.kitware.com/midas3/api/rest?method=midas.bitstream.download&checksum=%(hash)"
)

# Set source directory
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
set(TEMP_DIR ${CMAKE_BINARY_DIR}/Testing/Temporary)

# Set the files used for the tests
set(ad_TestFADTTS_Input DATA{${SOURCE_DIR}/ad_TestFADTTS_Input.csv})
set(COMP_0_TestFADTTS_Input DATA{${SOURCE_DIR}/COMP_0_TestFADTTS_Input.csv})
set(COMP_3_TestFADTTS_Input DATA{${SOURCE_DIR}/COMP_3_TestFADTTS_Input.csv})

set(selectedSubjectList_TestFADTTS DATA{${SOURCE_DIR}/selectedSubjectList_TestFADTTS.txt})

set(ad_TestFADTTS_MatlabInput DATA{${SOURCE_DIR}/ad_TestFADTTS_MatlabInput.csv})
set(COMP_TestFADTTS_MatlabInput DATA{${SOURCE_DIR}/COMP_TestFADTTS_MatlabInput.csv})

set(matlabScript_TestFADTTS_RunInput DATA{${SOURCE_DIR}/FADTTSAnalysis_MatlabScript_TestFADTTS_259perm.m})

include_directories(${CMAKE_SOURCE_DIR})

# Add the executable for the tests and linked the != libraries needed
link_directories(${CMAKE_BINARY_DIR}/lib)

# Add the executable for the test(s) of the InfoDialog class
file(GLOB SOURCES_TEST_INFODIALOG "*InfoDialog.cxx")
add_executable(FADTTS_Test_InfoDialog ${SOURCES_TEST_INFODIALOG})
target_link_libraries(FADTTS_Test_InfoDialog FADTTSterLib)

# Add the executable for the test(s) of the MatlabScript class
file(GLOB SOURCES_TEST_MATLABSCRIPT "*MatlabScript.cxx")
add_executable(FADTTS_Test_MatlabScript ${SOURCES_TEST_MATLABSCRIPT})
target_link_libraries(FADTTS_Test_MatlabScript FADTTSterLib)

# Add the executable for the test(s) of the Processing class
file(GLOB SOURCES_TEST_PROCESSING "*Processing.cxx")
add_executable(FADTTS_Test_Processing ${SOURCES_TEST_PROCESSING})
target_link_libraries(FADTTS_Test_Processing FADTTSterLib)

# Add the executable for the test(s) of the EditInputDialog class
file(GLOB SOURCES_TEST_EDITINPUTDIALOG "*EditInputDialog.cxx")
add_executable(FADTTS_Test_EditInputDialog ${SOURCES_TEST_EDITINPUTDIALOG})
target_link_libraries(FADTTS_Test_EditInputDialog FADTTSterLib)

# Add the executable for the test(s) of the FADTTSWindow class
file(GLOB SOURCES_TEST_FADTTSWINDOW "*FADTTSWindow.cxx")
add_executable(FADTTS_Test_FADTTSWindow ${SOURCES_TEST_FADTTSWINDOW})
target_link_libraries(FADTTS_Test_FADTTSWindow FADTTSterLib)


# Test for InfoDialog class
ExternalData_add_test(
        MY_DATA
        NAME TestInfoDialog
        COMMAND $<TARGET_FILE:FADTTS_Test_InfoDialog> ${ad_TestFADTTS_Input} ${COMP_0_TestFADTTS_Input}
)

# Test for MatlabScript class
ExternalData_add_test(
        MY_DATA
        NAME TestMatlabScript
        COMMAND $<TARGET_FILE:FADTTS_Test_MatlabScript> ${TEMP_DIR} ${ad_TestFADTTS_MatlabInput} ${COMP_TestFADTTS_MatlabInput} ${matlabScript_TestFADTTS_RunInput}
)

# Test for Processing class
ExternalData_add_test(
        MY_DATA
        NAME TestProcessing
        COMMAND $<TARGET_FILE:FADTTS_Test_Processing> ${TEMP_DIR} ${ad_TestFADTTS_Input} ${COMP_0_TestFADTTS_Input} ${COMP_3_TestFADTTS_Input} ${ad_TestFADTTS_MatlabInput} ${COMP_TestFADTTS_MatlabInput} ${selectedSubjectList_TestFADTTS}
)

# Test for EditInputDialog class
ExternalData_add_test(
        MY_DATA
        NAME TestEditInputDialog
        COMMAND $<TARGET_FILE:FADTTS_Test_EditInputDialog> ${ad_TestFADTTS_Input} ${COMP_0_TestFADTTS_Input}
)

# Test for FADTTSWindow class except final running function
ExternalData_add_test(
        MY_DATA
        NAME TestFADTTSWindow
        COMMAND $<TARGET_FILE:FADTTS_Test_FADTTSWindow> ${ad_TestFADTTS_Input} ${COMP_0_TestFADTTS_Input}
)

# Test for the command --help
add_test(
        NAME PrintHelp
        COMMAND $<TARGET_FILE:FADTTSter> --help
)

ExternalData_add_target(MY_DATA)
