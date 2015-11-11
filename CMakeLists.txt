cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.8)

project(FADTTS)
option(BUILD_TESTING "tests" ON)


# Setting paths
set(CMAKE_MODULE_PATH
${${PROJECT_NAME}_SOURCE_DIR}/CMake
${${PROJECT_NAME}_BINARY_DIR}/CMake
${CMAKE_MODULE_PATH}
)


# Setting QtToCppXML
find_package(QtToCppXML REQUIRED)
include(${QtToCppXML_USE_FILE})


# Setting Qt5
find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(QT_LIBRARIES ${Qt5Widgets_LIBRARIES})


# Setting SlicerExecutionModel
find_package(SlicerExecutionModel REQUIRED)
include(${SlicerExecutionModel_USE_FILE})


# Qt to c++
set(FADTTS_Sources
FADTTSWindow.cxx
FADTTSWindowConfig.cxx
EditInputDialog.cxx
InfoDialog.cxx
Data.cxx
Processing.cxx
MatlabScript.cxx
)

set(FADTTS_Headers
FADTTSWindow.h
FADTTSWindowConfig.h
EditInputDialog.h
InfoDialog.h
Data.h
MatlabScript.h
)

set(FADTTS_Forms
FADTTSWindow.ui
EditInputDialog.ui
InfoDialog.ui
)

set(FADTTS_Resources
FADTTS_Resources.qrc
)

qt5_wrap_cpp(FADTTS_Headers_Moc ${FADTTS_Headers})
qt5_wrap_ui(FADTTS_Headers_Form ${FADTTS_Forms})
qt5_add_resources(FADTTS_Resources_rcc ${FADTTS_Resources})

list(APPEND
FADTTS_Sources
${FADTTS_Headers_Moc}
${FADTTS_Headers_Form}
${FADTTS_Resources_rcc}
)

QtToCppXML(SOURCES FADTTS_Sources NAME FADTTS UIXML_FILE FADTTSWindow.ui PREFIXES soft_ para_)


# Generating FADTTS executable
SEMMacroBuildCLI(
NAME FADTTS
EXECUTABLE_ONLY
ADDITIONAL_SRCS ${FADTTS_Sources}
TARGET_LIBRARIES ${QT_LIBRARIES}
LINK_DIRECTORIES ${QT_LIBRARY_DIRS}
INCLUDE_DIRECTORIES ${QT_INCLUDE_DIR}
)


# Getting xml info
FILE(READ FADTTS.xml var)

string(REGEX MATCH "<title>.*</title>" ext "${var}")
string(REPLACE "<title>" "" title ${ext})
string(REPLACE "</title>" "" title ${title})
add_definitions(-DFADTTS_TITLE="${title}")

string(REGEX MATCH "<version>.*</version>" ext "${var}")
string(REPLACE "<version>" "" version ${ext})
string(REPLACE "</version>" "" version ${version})
add_definitions(-DFADTTS_VERSION="${version}")

string(REGEX MATCH "<contributor>.*</contributor>" ext "${var}")
string(REPLACE "<contributor>" "" contributors ${ext})
string(REPLACE "</contributor>" "" contributors ${contributors})
add_definitions(-DFADTTS_CONTRIBUTORS="${contributors}")

string(REGEX MATCH "<description>.*</description>" ext "${var}")
string(REPLACE "<description>" "" description ${ext})
string(REPLACE "</description>" "" description ${description})
add_definitions(-DFADTTS_DESCRIPTION="${description}")


# Tests
if(BUILD_TESTING)
  include(CTest)
  include(ExternalData)
  add_SUBDIRECTORY(Testing)
endif()
