cmake_minimum_required(VERSION 2.8)

# Set source directory
SET(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
SET(TEMP_DIR ${CMAKE_BINARY_DIR}/Testing/Temporary )

# Set the images used for the tests
SET(referenceYinYang DATA{${SOURCE_DIR}/reverseYinYang.png})
SET(Input DATA{${SOURCE_DIR}/basicYinYang.png})
SET(ResultYinYang ${TEMP_DIR}/testYinYang.png)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# Add the executable for the tests and linked the != libraries needed
ADD_EXECUTABLE(ownFilterTests ownFilterTests.cxx)
TARGET_LINK_LIBRARIES(ownFilterTests multiThreadedOwnFilterLib)

# Add the executable for the tests and linked the != libraries needed -- Exceptions
ADD_EXECUTABLE(ownFilterExceptions ownFilterExceptions.cxx)
TARGET_LINK_LIBRARIES(ownFilterExceptions multiThreadedOwnFilterLib)


# Test checking that the program cannot run without arguments
ExternalData_add_test(
        MY_DATA
        NAME OpenFileTest
        COMMAND $<TARGET_FILE:multiThreadedOwnFilter>)
SET_TESTS_PROPERTIES(OpenFileTest PROPERTIES WILL_FAIL true )

#Test checking that a wrong input will prevent the program from running
ExternalData_add_test(
        MY_DATA
        NAME WrongInputFileName
        COMMAND $<TARGET_FILE:ownFilterExceptions>
    ModuleEntryPoint
        WrongInput
        ${ResultYinYang}

)
#set_tests_properties( WrongInputFileName PROPERTIES WILL_FAIL true )

# Tests checking that the output image is what we expect
ExternalData_add_test(
        MY_DATA
        NAME multiThreadedOwnFilterOK
        COMMAND $<TARGET_FILE:ownFilterTests>
    --compare
        ${referenceYinYang}
        ${ResultYinYang}
    ModuleEntryPoint
        ${Input}
        ${ResultYinYang}
)

# Test for the command --help
ADD_TEST(NAME PrintHelp COMMAND $<TARGET_FILE:multiThreadedOwnFilter> --help)

ExternalData_Add_Target(MY_DATA)
