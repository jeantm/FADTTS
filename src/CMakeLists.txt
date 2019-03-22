
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

if(UNIX AND NOT APPLE AND CREATE_BUNDLE)
  # the RPATH to be used when installing
  SET(CMAKE_INSTALL_RPATH "$ORIGIN/../lib;../lib")
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

# Generate a bundle
if(CREATE_BUNDLE)

  set(myApp_ICON ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Icons/logoFADTTSter_small.icns)
  set(CPACK_PACKAGE_ICON "${myApp_ICON}")

  string(REPLACE "." ";" VERSION_LIST ${version})
  list(GET VERSION_LIST 0 CPACK_PACKAGE_VERSION_MAJOR)
  list(GET VERSION_LIST 1 CPACK_PACKAGE_VERSION_MINOR)
  list(GET VERSION_LIST 2 CPACK_PACKAGE_VERSION_PATCH)

  set(bundle_name FADTTSter${version})

  if(APPLE)
    set(OS_BUNDLE MACOSX_BUNDLE)
    #--------------------------------------------------------------------------------
    SET(qtconf_dest_dir bin)
    SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/${bundle_name}")
  
    SET(qtconf_dest_dir ${bundle_name}.app/Contents/Resources)
    SET(APPS "\${CMAKE_INSTALL_PREFIX}/${bundle_name}.app")

    set(MACOSX_BUNDLE_BUNDLE_NAME "${bundle_name}")
    set(MACOSX_BUNDLE_INFO_STRING "FADTTS: functional analysis of diffusion tensor tract statistics.")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "niral.unc.edu.${bundle_name}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "FADTTSter version - ${version}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${version}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${version}")
    set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2016 University of North Carolina , Chapel Hill.")
    
    set_source_files_properties(
      ${myApp_ICON}
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
      )

    get_filename_component(apple_icon_filename ${myApp_ICON} NAME)
    set(MACOSX_BUNDLE_ICON_FILE ${apple_icon_filename})

    add_executable(${bundle_name} ${OS_BUNDLE}
      ${myApp_ICON} ${FADTTS_src}
    )
    target_link_libraries(${bundle_name} ${QT_LIBRARIES} ${VTK_LIBRARIES})

    #--------------------------------------------------------------------------------
    # Install the QtTest application, on Apple, the bundle is at the root of the
    # install tree, and on other platforms it'll go into the bin directory.
    INSTALL(TARGETS ${bundle_name}
      DESTINATION . COMPONENT Runtime
      RUNTIME DESTINATION bin COMPONENT Runtime
    )

    macro(install_qt5_plugin _qt_plugin_name _qt_plugins_var)
      get_target_property(_qt_plugin_path "${_qt_plugin_name}" LOCATION)
      if(EXISTS "${_qt_plugin_path}")
        get_filename_component(_qt_plugin_file "${_qt_plugin_path}" NAME)
        get_filename_component(_qt_plugin_type "${_qt_plugin_path}" PATH)
        get_filename_component(_qt_plugin_type "${_qt_plugin_type}" NAME)
        set(_qt_plugin_dest "${bundle_name}.app/Contents/PlugIns/${_qt_plugin_type}")
        install(FILES "${_qt_plugin_path}"
          DESTINATION "${_qt_plugin_dest}"
          COMPONENT Runtime)
        set(${_qt_plugins_var}
          "${${_qt_plugins_var}};\${CMAKE_INSTALL_PREFIX}/${_qt_plugin_dest}/${_qt_plugin_file}")
      else()
        message(FATAL_ERROR "QT plugin ${_qt_plugin_name} not found")
      endif()
    endmacro()
    
    install_qt5_plugin("Qt5::QCocoaIntegrationPlugin" QT_PLUGINS)

    # To Create a package, one can run "cpack -G DragNDrop CPackConfig.cmake" on Mac OS X
    # where CPackConfig.cmake is created by including CPack
    # And then there's ways to customize this as well
    set(CPACK_BINARY_DRAGNDROP ON)
    #--------------------------------------------------------------------------------
    # install a qt.conf file
    # this inserts some cmake code into the install script to write the file
    INSTALL(CODE "
        file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/qt.conf\" \"[Paths]\nPlugins = PlugIns\n\")
        " COMPONENT Runtime)

    # Install the license
    INSTALL(FILES 
      ${CMAKE_SOURCE_DIR}/LICENSE
      DESTINATION "${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}"
      COMPONENT Runtime)
   
    get_target_property(Qt5_location Qt5::Widgets LOCATION)
    string(FIND ${Qt5_location} "/QtWidgets" length)
    string(SUBSTRING ${Qt5_location} 0 ${length} Qt5_location)
    #Fix the bundle
    install(CODE "
      include(BundleUtilities)
      fixup_bundle(\"${APPS}\" \"${QT_PLUGINS};\" \"${Qt5_location}\")
     "
      COMPONENT Runtime)
  ENDIF(APPLE)

  get_target_property(QtWidgets_location Qt5::Widgets LOCATION)
  get_target_property(QtOpenGL_location Qt5::OpenGL LOCATION)
  
  get_target_property(QtWidgets_SOFT Qt5::Widgets IMPORTED_SONAME_RELEASE)
  get_target_property(QtOpenGL_SOFT Qt5::OpenGL IMPORTED_SONAME_RELEASE)

  get_target_property(Qt5_location Qt5::Widgets LOCATION)  
  string(FIND ${Qt5_location} "libQt5Widgets" length)
  string(SUBSTRING ${Qt5_location} 0 ${length} Qt5_location)
  
  #install(FILES ${QtWidgets_location} ${QtOpenGL_location} ${Qt5_location}/${QtWidgets_SOFT} ${Qt5_location}/${QtOpenGL_SOFT}
  #    DESTINATION lib
  #    COMPONENT Runtime)

  install(FILES ${QtWidgets_location} ${QtOpenGL_location}
      DESTINATION lib
      COMPONENT Runtime)

  if(UNIX AND NOT APPLE)

    get_target_property(QtSql_location Qt5::Sql LOCATION)
    get_target_property(QtX11Extras_location Qt5::X11Extras LOCATION)
    get_target_property(QCore_location Qt5::Core LOCATION)
    get_target_property(QGui_location Qt5::Gui LOCATION)
    
    get_target_property(QtSql_SOFT Qt5::Sql IMPORTED_SONAME_RELEASE)
    get_target_property(QtX11Extras_SOFT Qt5::X11Extras IMPORTED_SONAME_RELEASE)
    get_target_property(QCore_SOFT Qt5::Core IMPORTED_SONAME_RELEASE)
    get_target_property(QGui_SOFT Qt5::Gui IMPORTED_SONAME_RELEASE)
    

    install(FILES ${QtSql_location} ${QtX11Extras_location} ${QCore_location} ${QGui_location} ${Qt5_location}/${QtWidgets_SOFT} ${Qt5_location}/${QtSql_SOFT} ${Qt5_location}/${QtX11Extras_SOFT} ${Qt5_location}/${QCore_SOFT} ${Qt5_location}/${QGui_SOFT} 
      DESTINATION lib
      COMPONENT Runtime)
  endif()
endif()

# To Create a package, one can run "cpack -G DragNDrop CPackConfig.cmake" on Mac OS X
# where CPackConfig.cmake is created by including CPack
# And then there's ways to customize this as well  
include(InstallRequiredSystemLibraries)
include(CPack) 


configure_file(CMake/FADTTSConfig.cmake.in
  "${PROJECT_BINARY_DIR}/FADTTSConfig.cmake" @ONLY)

if(WIN32 AND NOT CYGWIN)
  set(INSTALL_CMAKE_DIR cmake)
else()
  set(INSTALL_CMAKE_DIR lib/cmake/FADTTS)
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