CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
CMAKE_POLICY( VERSION 2.8 )

PROJECT( FADTTS )
OPTION( BUILD_TESTING "tests" ON )


SET( CMAKE_MODULE_PATH
${${PROJECT_NAME}_SOURCE_DIR}/CMake
${${PROJECT_NAME}_BINARY_DIR}/CMake
${CMAKE_MODULE_PATH}
)

SET( CMAKE_CXX_FLAGS "-std=c++0x" )
#add_definitions(-std=c++0x) # CMake 2.8.11 or older

FIND_PACKAGE( QtToCppXML REQUIRED )
INCLUDE( ${QtToCppXML_USE_FILE} )

FIND_PACKAGE( Qt4 COMPONENTS QtCore QtGui REQUIRED )
INCLUDE( ${QT_USE_FILE} )

FIND_PACKAGE( SlicerExecutionModel REQUIRED )
INCLUDE( ${SlicerExecutionModel_USE_FILE} )

SET( FADTTS_SOURCES
FADTTSWindow.cxx
FADTTSWindowConfig.cxx
EditInputDialog.cxx
InfoDialog.cxx
Data.cxx
Processing.cxx
MatlabScript.cxx
)

SET( FADTTS_HEADERS
FADTTSWindow.h
FADTTSWindowConfig.h
EditInputDialog.h
InfoDialog.h
Data.h
MatlabScript.h
)

SET( FADTTS_FORMS
FADTTSWindow.ui
EditInputDialog.ui
InfoDialog.ui
)

SET( FADTTS_RESOURCES
FADTTS_Resources.qrc
)

QT4_WRAP_CPP( FADTTS_HEADERS_MOC ${FADTTS_HEADERS} )
QT4_WRAP_UI( FADTTS_FORMS_HEADERS ${FADTTS_FORMS} )
QT4_ADD_RESOURCES( FADTTS_RESOURCES_RCC ${FADTTS_RESOURCES} )

# xml info in GUI
FILE(READ FADTTS.xml var)

string( REGEX MATCH "<version>.*</version>" ext "${var}" )
string( REPLACE "<version>" "" version ${ext} )
string( REPLACE "</version>" "" version ${version} )

ADD_DEFINITIONS( -DFADTTS_VERSION="${version}" )

string( REGEX MATCH "<contributor>.*</contributor>" ext "${var}" )
string( REPLACE "<contributor>" "" contributors ${ext} )
string( REPLACE "</contributor>" "" contributors ${contributors} )

ADD_DEFINITIONS( -DFADTTS_CONTRIBUTOR="${CONTRIBUTORS}" )

LIST( APPEND
FADTTS_SOURCES
${FADTTS_HEADERS_MOC}
${FADTTS_FORMS_HEADERS}
${FADTTS_RESOURCES_RCC}
)

QtToCppXML( SOURCES FADTTS_SOURCES NAME FADTTS UIXML_FILE FADTTSWindow.ui PREFIXES soft_ para_ )

SEMMacroBuildCLI(
NAME FADTTS
EXECUTABLE_ONLY
ADDITIONAL_SRCS ${FADTTS_SOURCES}
TARGET_LIBRARIES ${QT_LIBRARIES}
LINK_DIRECTORIES ${QT_LIBRARY_DIRS}
INCLUDE_DIRECTORIES ${QT_INCLUDE_DIR}
)

if(BUILD_TESTING)
  include(CTest)
  include(ExternalData)
  ADD_SUBDIRECTORY(Testing)
endif()
