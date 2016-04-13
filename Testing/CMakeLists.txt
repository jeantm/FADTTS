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

set(rdRawDataToSortPath DATA{${SOURCE_DIR}/testSort_RawData_RD.csv})
set(faRawDataToSortPath DATA{${SOURCE_DIR}/testSort_RawData_FA.csv})
set(subMatrixRawDataToSortPath DATA{${SOURCE_DIR}/testSort_RawData_SUBMATRIX.csv})

set(rdRawDataPath DATA{${SOURCE_DIR}/test_RawData_RD.csv})
set(faRawDataPath DATA{${SOURCE_DIR}/test_RawData_FA.csv})
set(subMatrixRawDataPath DATA{${SOURCE_DIR}/test_RawData_SUBMATRIX.csv})
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

set(testParaSettings DATA{${SOURCE_DIR}/testParaSettings.xml})
set(testParaNoSettings DATA{${SOURCE_DIR}/testParaNoSettings.xml})
set(testSoftSettings DATA{${SOURCE_DIR}/testSoftSettings.xml})
set(testSoftNoSettings DATA{${SOURCE_DIR}/testSoftNoSettings.xml})

set(okIcon DATA{${SOURCE_DIR}/okIconOut.xpm})
set(koIcon DATA{${SOURCE_DIR}/koIconOut.xpm})
set(warningIcon DATA{${SOURCE_DIR}/warningIconOut.xpm})

set(mdRawDataPath1 DATA{${SOURCE_DIR}/testPlot1_RawData_MD.csv})
set(rdRawDataPath1 DATA{${SOURCE_DIR}/testPlot1_RawData_RD.csv})
set(subMatrixRawDataPath1 DATA{${SOURCE_DIR}/testPlot1_RawData_SUBMATRIX.csv})
set(mdBetaPath1 DATA{${SOURCE_DIR}/testPlot1_Betas_MD.csv})
set(rdBetaPath1 DATA{${SOURCE_DIR}/testPlot1_Betas_RD.csv})
set(mdConfidenceBandsPath1 DATA{${SOURCE_DIR}/testPlot1_Omnibus_ConfidenceBands_MD.csv})
set(rdConfidenceBandsPath1 DATA{${SOURCE_DIR}/testPlot1_Omnibus_ConfidenceBands_RD.csv})
set(mdPostHocFDRLpvaluesPath1 DATA{${SOURCE_DIR}/testPlot1_PostHoc_FDR_Local_pvalues_MD.csv})
set(rdPostHocFDRLpvaluesPath1 DATA{${SOURCE_DIR}/testPlot1_PostHoc_FDR_Local_pvalues_RD.csv})

set(rdRawDataPath2 DATA{${SOURCE_DIR}/testPlot2_RawData_AD.csv})
set(faRawDataPath2 DATA{${SOURCE_DIR}/testPlot2_RawData_FA.csv})
set(subMatrixRawDataPath2 DATA{${SOURCE_DIR}/testPlot2_RawData_SUBMATRIX.csv})
set(adBetaPath2 DATA{${SOURCE_DIR}/testPlot2_Betas_AD.csv})
set(faBetaPath2 DATA{${SOURCE_DIR}/testPlot2_Betas_FA.csv})
set(omnibusFDRLpvaluesPath2 DATA{${SOURCE_DIR}/testPlot2_Omnibus_FDR_Local_pvalues.csv})

set(plotSettingsPath DATA{${SOURCE_DIR}/plotSettings.csv})
set(plotPath DATA{${SOURCE_DIR}/test_Betas_FA.eps})

set(failedQCThreshold DATA{${SOURCE_DIR}/test_failed_QCThreshold_SubjectList.txt})


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

# Add the executable for the test(s) of the EditInputDialog class
file(GLOB SOURCES_TEST_EDITINPUTDIALOG "*EditInputDialog.cxx")
add_executable(FADTTS_Test_EditInputDialog ${SOURCES_TEST_EDITINPUTDIALOG})
target_link_libraries(FADTTS_Test_EditInputDialog FADTTSterLib)

# Add the executable for the test(s) of the Plot class
file(GLOB SOURCES_TEST_PLOT "*Plot.cxx")
add_executable(FADTTS_Test_Plot ${SOURCES_TEST_PLOT})
target_link_libraries(FADTTS_Test_Plot FADTTSterLib)

# Add the executable for the test(s) of the FADTTSWindow class
file(GLOB SOURCES_TEST_FADTTSWINDOW "*FADTTSWindow.cxx")
add_executable(FADTTS_Test_FADTTSWindow ${SOURCES_TEST_FADTTSWINDOW})
target_link_libraries(FADTTS_Test_FADTTSWindow FADTTSterLib)



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

                                                ${plotPath}

                                                ${DATA_DIR} ${TEMP_DIR}
)

# Test for FADTTSWindow class except final running function
ExternalData_add_test(
        MY_DATA
        NAME TestFADTTSWindow
        COMMAND $<TARGET_FILE:FADTTS_Test_FADTTSWindow> ${testParaSettings} ${testParaNoSettings} ${testSoftSettings} ${testSoftNoSettings}
                                                        ${rdRawDataPath} ${faRawDataPath} ${subMatrixRawDataPath} ${subjectsListFilePath}

                                                        ${mdRawDataPath1} ${rdRawDataPath1} ${subMatrixRawDataPath1}
                                                        ${mdBetaPath1} ${rdBetaPath1} ${mdConfidenceBandsPath1} ${rdConfidenceBandsPath1}
                                                        ${mdPostHocFDRLpvaluesPath1} ${rdPostHocFDRLpvaluesPath1}

                                                        ${rdRawDataPath2} ${faRawDataPath2} ${subMatrixRawDataPath2}
                                                        ${adBetaPath2} ${faBetaPath2} ${omnibusFDRLpvaluesPath2}

                                                        ${plotSettingsPath}

                                                        ${failedQCThreshold}

                                                        ${okIcon} ${koIcon} ${warningIcon}

                                                        ${DATA_DIR} ${TEMP_DIR}
)

# Test for the command --help
add_test(
        NAME PrintHelp
        COMMAND $<TARGET_FILE:FADTTSter> --help
)

ExternalData_add_target(MY_DATA)
