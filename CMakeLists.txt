cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.8)

project(FADTTS)
option(BUILD_TESTING "tests" ON)
option(CREATE_BUNDLE "Create MACOSX_BUNDLE" OFF)

# Setting paths
set(CMAKE_MODULE_PATH
${${PROJECT_NAME}_SOURCE_DIR}/CMake
${${PROJECT_NAME}_BINARY_DIR}/CMake
${CMAKE_MODULE_PATH}
)

# find Qt5 headers
if(UNIX AND NOT APPLE)
  find_package(Qt5 COMPONENTS Widgets OpenGL X11Extras DBus REQUIRED)
else()
  find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)
endif()

include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(QT_LIBRARIES ${Qt5Widgets_LIBRARIES})

# find VTK headers
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

string(FIND "${VTK_MODULES_ENABLED}" "GL2PS" index)

if( NOT ${index} EQUAL -1)
  add_definitions("-DGL2PSENABLED")
endif()

# find SlicerExecutionModel headers
find_package(SlicerExecutionModel REQUIRED)
include(${SlicerExecutionModel_USE_FILE})

add_subdirectory(src)

