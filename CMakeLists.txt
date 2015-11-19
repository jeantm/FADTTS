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

# find QtToCppXML headers
find_package(QtToCppXML REQUIRED)
include(${QtToCppXML_USE_FILE})

# find VTK headers
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

# find Qt5 headers
find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(QT_LIBRARIES ${Qt5Widgets_LIBRARIES})

# find SlicerExecutionModel headers
find_package(SlicerExecutionModel REQUIRED)
include(${SlicerExecutionModel_USE_FILE})

# Qt to c++
set(FADTTS_src
FADTTSter.cxx
FADTTSWindowConfig.cxx
FADTTSWindow.cxx
EditInputDialog.cxx
InfoDialog.cxx
Data.cxx
Processing.cxx
MatlabScript.cxx
MatlabThread.cxx
Plot.cxx
)

set(FADTTS_moc
FADTTSWindowConfig.h
FADTTSWindow.h
EditInputDialog.h
InfoDialog.h
MatlabThread.h
Plot.h
)

set(FADTTS_ui
FADTTSWindow.ui
EditInputDialog.ui
InfoDialog.ui
)

set(FADTTS_rcc
FADTTS_Resources.qrc
)

qt5_wrap_cpp(FADTTS_generated_moc ${FADTTS_moc})
qt5_wrap_ui(FADTTS_generated_ui ${FADTTS_ui})
qt5_add_resources(FADTTS_generated_rcc ${FADTTS_rcc})

list(APPEND FADTTS_src
${FADTTS_generated_moc}
${FADTTS_generated_ui}
${FADTTS_generated_rcc}
)
QtToCppXML(SOURCES FADTTS_src NAME FADTTS UIXML_FILE FADTTSWindow.ui PREFIXES soft_ para_)

# generate FADTTSter executable
SEMMacroBuildCLI(
NAME FADTTSter
EXECUTABLE_ONLY
ADDITIONAL_SRCS ${FADTTS_src}
TARGET_LIBRARIES ${QT_LIBRARIES} ${VTK_LIBRARIES}
LINK_DIRECTORIES ${QT_LIBRARY_DIRS}
INCLUDE_DIRECTORIES ${QT_INCLUDE_DIR}
)

# get FADTTSter info
FILE(READ FADTTSter.xml var)

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
