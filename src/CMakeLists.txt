
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
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

if(APPLE)
  SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

# generate FADTTSter executable
SEMMacroBuildCLI(
NAME FADTTSter
EXECUTABLE_ONLY
ADDITIONAL_SRCS ${FADTTS_src}
TARGET_LIBRARIES ${QT_LIBRARIES} ${VTK_LIBRARIES}
LINK_DIRECTORIES ${QT_LIBRARY_DIRS}
INCLUDE_DIRECTORIES ${QT_INCLUDE_DIR}
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
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

configure_file(CMake/FADTTSConfig.cmake.in
  "${PROJECT_BINARY_DIR}/FADTTSConfig.cmake" @ONLY)

if(WIN32 AND NOT CYGWIN)
  set(INSTALL_CMAKE_DIR cmake)
else()
  set(INSTALL_CMAKE_DIR lib/cmake/FADTTS-${version})
endif()

install(FILES
  "${PROJECT_BINARY_DIR}/FADTTSConfig.cmake"  
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)


file(GLOB FADTTS_HEADERS "*.h")
install(FILES ${FADTTS_HEADERS} 
  DESTINATION include)

# Tests
if(BUILD_TESTING)
  include(CTest)
  include(ExternalData)
  add_SUBDIRECTORY(Testing)
endif()