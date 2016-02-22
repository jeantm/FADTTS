cmake_minimum_required(VERSION 2.8)


# Add external data from midas
list(APPEND ExternalData_URL_TEMPLATES
"http://slicer.kitware.com/midas3/api/rest?method=midas.bitstream.download&checksum=%(hash)"
)


# Set source directory
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
set( DATA_DIR ${CMAKE_BINARY_DIR}/Testing/data )
set(TEMP_DIR ${CMAKE_BINARY_DIR}/Testing/Temporary)


# Set the files used for the tests
set(testGetDataFromFile_N DATA{${SOURCE_DIR}/testGetDataFromFile_N.csv})
set(testGetDataFromFile_R DATA{${SOURCE_DIR}/testGetDataFromFile_R.csv})
set(testGetDataFromFile_RN DATA{${SOURCE_DIR}/testGetDataFromFile_RN.csv})

set(adFilePath DATA{${SOURCE_DIR}/testADInput.csv})
set(newADFile DATA{${SOURCE_DIR}/newADFile.csv})
set(subMatrix0FilePath DATA{${SOURCE_DIR}/testSubMatrix0Input.csv})
set(subMatrix3FilePath DATA{${SOURCE_DIR}/testSubMatrix3Input.csv})

set(subjectsListFilePath DATA{${SOURCE_DIR}/testSubjectList.txt})

set(adMatlabFile DATA{${SOURCE_DIR}/testADMatlabInput.csv})
set(subMatrixMatlabFilePath DATA{${SOURCE_DIR}/testSubMatrixMatlabInput.csv})

set(refMatlabScript DATA{${SOURCE_DIR}/testRefMatlabScriptWithPlots.m})
set(myFDRScript DATA{${SOURCE_DIR}/myFDR.m})

set(rdRawDataToSortPath DATA{${SOURCE_DIR}/testSort_RD_RawData.csv})
set(faRawDataToSortPath DATA{${SOURCE_DIR}/testSort_FA_RawData.csv})
set(subMatrixRawDataToSortPath DATA{${SOURCE_DIR}/testSort_SUBMATRIX_RawData.csv})

set(rdRawDataPath DATA{${SOURCE_DIR}/test_RD_RawData.csv})
set(faRawDataPath DATA{${SOURCE_DIR}/test_FA_RawData.csv})
set(subMatrixRawDataPath DATA{${SOURCE_DIR}/test_SUBMATRIX_RawData.csv})
set(rdBetaPath DATA{${SOURCE_DIR}/test_Betas_RD.csv})
set(faBetaPath DATA{${SOURCE_DIR}/test_Betas_FA.csv})
set(omnibusLpvaluesPath DATA{${SOURCE_DIR}/test_Omnibus_Local_pvalues.csv})
set(omnibusFDRLpvaluesPath DATA{${SOURCE_DIR}/test_Omnibus_FDR_Local_pvalues.csv})
set(rdConfidenceBandsPath DATA{${SOURCE_DIR}/test_Omnibus_ConfidenceBands_RD.csv})
set(faConfidenceBandsPath DATA{${SOURCE_DIR}/test_Omnibus_ConfidenceBands_FA.csv})
set(rdPostHocFDRLpvaluesPath DATA{${SOURCE_DIR}/test_PostHoc_FDR_Local_pvalues_RD.csv})
set(faPostHocFDRLpvaluesPath DATA{${SOURCE_DIR}/test_PostHoc_FDR_Local_pvalues_FA.csv})

set(transposedRDRawDataPath DATA{${SOURCE_DIR}/testRDRawDataTransposed.csv})
set(transposedFARawDataPath DATA{${SOURCE_DIR}/testFARawDataTransposed.csv})
set(transposedOmnibusLpvaluesPath DATA{${SOURCE_DIR}/testOmnibusLpvaluesTransposed.csv})
set(transposedRDPostHocFDRPath DATA{${SOURCE_DIR}/testRDPostHocFDRTransposed.csv})
set(transposedFAPostHocFDRPath DATA{${SOURCE_DIR}/testFAPostHocFDRTransposed.csv})



include_directories(${CMAKE_SOURCE_DIR})

# Add the executable for the tests and linked the != libraries needed
link_directories(${CMAKE_BINARY_DIR}/lib)

# Add the executable for the test(s) of the Data class
file(GLOB SOURCES_TEST_DATA "*Data.cxx")
add_executable(FADTTS_Test_Data ${SOURCES_TEST_DATA})
target_link_libraries(FADTTS_Test_Data FADTTSterLib)

# Add the executable for the test(s) of the Processing class
file(GLOB SOURCES_TEST_PROCESSING "*Processing.cxx")
add_executable(FADTTS_Test_Processing ${SOURCES_TEST_PROCESSING})
target_link_libraries(FADTTS_Test_Processing FADTTSterLib)

# Add the executable for the test(s) of the MatlabThread class
file(GLOB SOURCES_TEST_MATLABTHREAD "*MatlabThread.cxx")
add_executable(FADTTS_Test_MatlabThread ${SOURCES_TEST_MATLABTHREAD})
target_link_libraries(FADTTS_Test_MatlabThread FADTTSterLib)

# Add the executable for the test(s) of the Plot class
file(GLOB SOURCES_TEST_PLOT "*Plot.cxx")
add_executable(FADTTS_Test_Plot ${SOURCES_TEST_PLOT})
target_link_libraries(FADTTS_Test_Plot FADTTSterLib)

# Add the executable for the test(s) of the FADTTSWindow class
file(GLOB SOURCES_TEST_FADTTSWINDOW "*FADTTSWindow.cxx")
add_executable(FADTTS_Test_FADTTSWindow ${SOURCES_TEST_FADTTSWINDOW})
target_link_libraries(FADTTS_Test_FADTTSWindow FADTTSterLib)

# Add the executable for the test(s) of the EditInputDialog class
file(GLOB SOURCES_TEST_EDITINPUTDIALOG "*EditInputDialog.cxx")
add_executable(FADTTS_Test_EditInputDialog ${SOURCES_TEST_EDITINPUTDIALOG})
target_link_libraries(FADTTS_Test_EditInputDialog FADTTSterLib)



# Test for Data class
add_test(
        NAME TestData
        COMMAND $<TARGET_FILE:FADTTS_Test_Data>
)

# Test for Processing class
ExternalData_add_test(
        MY_DATA
        NAME TestProcessing
        COMMAND $<TARGET_FILE:FADTTS_Test_Processing> ${testGetDataFromFile_N} ${testGetDataFromFile_N} ${testGetDataFromFile_N}
                                                      ${adFilePath} ${subMatrix0FilePath} ${subMatrix3FilePath}
                                                      ${adMatlabFile} ${subMatrixMatlabFilePath} ${subjectsListFilePath}
                                                      ${TEMP_DIR}
)

# Test for MatlabThread class
ExternalData_add_test(
        MY_DATA
        NAME TestMatlabThread
        COMMAND $<TARGET_FILE:FADTTS_Test_MatlabThread> ${refMatlabScript} ${myFDRScript} ${TEMP_DIR}
)

# Test for EditInputDialog class
ExternalData_add_test(
        MY_DATA
        NAME TestEditInputDialog
        COMMAND $<TARGET_FILE:FADTTS_Test_EditInputDialog> ${adFilePath} ${subMatrix0FilePath} ${newADFile} ${TEMP_DIR}
)

# Test for Plot class
ExternalData_add_test(
        MY_DATA
        NAME TestPlot
        COMMAND $<TARGET_FILE:FADTTS_Test_Plot> ${rdRawDataToSortPath} ${faRawDataToSortPath} ${subMatrixRawDataToSortPath}
                                                ${rdRawDataPath} ${faRawDataPath} ${subMatrixRawDataPath}
                                                ${rdBetaPath} ${faBetaPath}
                                                ${omnibusLpvaluesPath} ${omnibusFDRLpvaluesPath}
                                                ${rdConfidenceBandsPath} ${faConfidenceBandsPath}
                                                ${rdPostHocFDRLpvaluesPath} ${faPostHocFDRLpvaluesPath}
                                                ${transposedRDRawDataPath} ${transposedFARawDataPath}
                                                ${transposedOmnibusLpvaluesPath}
                                                ${transposedRDPostHocFDRPath} ${transposedFAPostHocFDRPath}
                                                ${DATA_DIR}
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
