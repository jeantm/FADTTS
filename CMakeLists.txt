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

# find Qt5 headers

find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(QT_LIBRARIES ${Qt5Widgets_LIBRARIES})

# find VTK headers
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

# find SlicerExecutionModel headers
find_package(SlicerExecutionModel REQUIRED)
include(${SlicerExecutionModel_USE_FILE})

# Qt to c++
set(FADTTS_src
Data.cxx
Processing.cxx
MatlabThread.cxx
Plot.cxx
Log.cxx
FADTTS_noGUI.cxx
EditInputDialog.cxx
QCThresholdDialog.cxx
FADTTSWindow.cxx
FADTTSter.cxx
)

set(FADTTS_moc
Processing.h
MatlabThread.h
Plot.h
Log.h
FADTTS_noGUI.h
EditInputDialog.h
QCThresholdDialog.h
FADTTSWindow.h
)

set(FADTTS_ui
EditInputDialog.ui
QCThresholdDialog.ui
FADTTSWindow.ui
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

if(NOT INSTALL_RUNTIME_DESTINATION)
	set(INSTALL_RUNTIME_DESTINATION bin)
endif(NOT INSTALL_RUNTIME_DESTINATION)

if(NOT INSTALL_LIBRARY_DESTINATION)
	set(INSTALL_LIBRARY_DESTINATION lib)
endif(NOT INSTALL_LIBRARY_DESTINATION)

if(NOT INSTALL_ARCHIVE_DESTINATION)
	set(INSTALL_ARCHIVE_DESTINATION lib)
endif(NOT INSTALL_ARCHIVE_DESTINATION)

# generate FADTTSter executable
SEMMacroBuildCLI(
NAME FADTTSter
EXECUTABLE_ONLY
ADDITIONAL_SRCS ${FADTTS_src}
TARGET_LIBRARIES ${QT_LIBRARIES} ${VTK_LIBRARIES}
LINK_DIRECTORIES ${QT_LIBRARY_DIRS}
INCLUDE_DIRECTORIES ${QT_INCLUDE_DIR}
INSTALL_RUNTIME_DESTINATION ${INSTALL_RUNTIME_DESTINATION}
INSTALL_LIBRARY_DESTINATION ${INSTALL_LIBRARY_DESTINATION}
INSTALL_ARCHIVE_DESTINATION ${INSTALL_ARCHIVE_DESTINATION}
)

set(FADTTS_LIBRARIES ${FADTTS_LIBRARIES} ${cli_executable_libraries} )

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

# Tests
if(BUILD_TESTING)
  include(CTest)
  include(ExternalData)
  add_SUBDIRECTORY(Testing)
endif()

if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/FADTTS)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")
  
configure_file(CMake/FADTTSConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/FADTTSConfig.cmake" @ONLY)
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/FADTTSConfig.cmake"  
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)