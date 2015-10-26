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
set(rd_TestFADTTS_Input DATA{${SOURCE_DIR}/rd_TestFADTTS_Input.csv})
set(md_TestFADTTS_Input DATA{${SOURCE_DIR}/md_TestFADTTS_Input.csv})
set(fa_TestFADTTS_Input DATA{${SOURCE_DIR}/fa_TestFADTTS_Input.csv})
set(COMP_0_TestFADTTS_Input DATA{${SOURCE_DIR}/COMP_0_TestFADTTS_Input.csv})
set(COMP_3_TestFADTTS_Input DATA{${SOURCE_DIR}/COMP_3_TestFADTTS_Input.csv})

set(selectedSubjectList_TestFADTTS DATA{${SOURCE_DIR}/selectedSubjectList_TestFADTTS.txt})

set(ad_TestFADTTS_MatlabInput DATA{${SOURCE_DIR}/ad_TestFADTTS_MatlabInput.csv})
set(rd_TestFADTTS_MatlabInput DATA{${SOURCE_DIR}/rd_TestFADTTS_MatlabInput.csv})
set(md_TestFADTTS_MatlabInput DATA{${SOURCE_DIR}/md_TestFADTTS_MatlabInput.csv})
set(fa_TestFADTTS_MatlabInput DATA{${SOURCE_DIR}/fa_TestFADTTS_MatlabInput.csv})
set(COMP_TestFADTTS_MatlabInput DATA{${SOURCE_DIR}/COMP_TestFADTTS_MatlabInput.csv})

set(TestFADTTS_MatlabScript DATA{${SOURCE_DIR}/TestFADTTS_FADTTSAnalysis_MatlabScript.m})


include_DIRECTORIES( ${CMAKE_SOURCE_DIR})

# Add the executable for the tests and linked the != libraries needed
link_directories(${CMAKE_BINARY_DIR}/lib)

# Add the executable for the test of EditInputDialog
file(GLOB SOURCES_TEST_EDITINPUTDIALOG "*EditInputDialog.cxx")
add_executable(FADTTS_Test_EditInputDialog ${SOURCES_TEST_EDITINPUTDIALOG})
target_link_libraries(FADTTS_Test_EditInputDialog FADTTSLib)

# Add the executable for the test of FADTTSWindow
#file(GLOB SOURCES_TEST_FADTTSWINDOW "*FADTTSWindow.cxx")
#add_executable(FADTTS_Test_FADTTSWindow ${SOURCES_TEST_FADTTSWINDOW})
#target_link_libraries(FADTTS_Test_FADTTSWindow FADTTSLib)

# Add the executable for the test of InfoDialog
file(GLOB SOURCES_TEST_INFODIALOG "*InfoDialog.cxx")
add_executable(FADTTS_Test_InfoDialog ${SOURCES_TEST_INFODIALOG})
target_link_libraries(FADTTS_Test_InfoDialog FADTTSLib)

# Add the executable for the test of MatlabScript
file(GLOB SOURCES_TEST_MATLABSCRIPT "*MatlabScript.cxx")
add_executable(FADTTS_Test_MatlabScript ${SOURCES_TEST_MATLABSCRIPT})
target_link_libraries(FADTTS_Test_MatlabScript FADTTSLib)

# Add the executable for the test of Processing
file(GLOB SOURCES_TEST_PROCESSING "*Processing.cxx")
add_executable(FADTTS_Test_Processing ${SOURCES_TEST_PROCESSING})
target_link_libraries(FADTTS_Test_Processing FADTTSLib)


# Test for EditInputDialog
ExternalData_add_test(
        MY_DATA
        NAME TestEditInputDialog
        COMMAND $<TARGET_FILE:FADTTS_Test_EditInputDialog> ${ad_TestFADTTS_Input} ${COMP_0_TestFADTTS_Input}
)

# Test for FADTTSWindow
#add_test(
#        NAME TestFADTTSWindow
#        COMMAND $<TARGET_FILE:FADTTS_Test_FADTTSWindow>
#)

# Test for InfoDialog
ExternalData_add_test(
        MY_DATA
        NAME TestInfoDialog
        COMMAND $<TARGET_FILE:FADTTS_Test_InfoDialog> ${ad_TestFADTTS_Input} ${COMP_0_TestFADTTS_Input}
)

# Test for MatlabScript
ExternalData_add_test(
        MY_DATA
        NAME TestMatlabScript
        COMMAND $<TARGET_FILE:FADTTS_Test_MatlabScript> ${TEMP_DIR} ${ad_TestFADTTS_MatlabInput} ${COMP_TestFADTTS_MatlabInput} ${TestFADTTS_MatlabScript}
)

# Test for Processing
ExternalData_add_test(
        MY_DATA
        NAME TestProcessing
        COMMAND $<TARGET_FILE:FADTTS_Test_Processing> ${TEMP_DIR} ${ad_TestFADTTS_Input} ${COMP_0_TestFADTTS_Input} ${COMP_3_TestFADTTS_Input} ${ad_TestFADTTS_MatlabInput} ${COMP_TestFADTTS_MatlabInput} ${selectedSubjectList_TestFADTTS}
)

# Test for the command --help
add_test(
        NAME PrintHelp
        COMMAND $<TARGET_FILE:FADTTS> --help
)

ExternalData_Add_Target(MY_DATA)
