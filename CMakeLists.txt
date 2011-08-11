cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(RTCHokuyoAist)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
include(${PROJECT_SOURCE_DIR}/cmake/rtcomp_utils.cmake)
set(PROJECT_VERSION 3.0.0 CACHE STRING "RTCHokuyoAIST version")
DISSECT_VERSION()
set(PROJECT_DESCRIPTION "RT-Component for Hokuyo range sensors")
set(PROJECT_VENDOR "AIST")

# Add an "uninstall" target
CONFIGURE_FILE ("${PROJECT_SOURCE_DIR}/cmake/uninstall_target.cmake.in"
    "${PROJECT_BINARY_DIR}/uninstall_target.cmake" IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET (uninstall "${CMAKE_COMMAND}" -P
    "${PROJECT_BINARY_DIR}/uninstall_target.cmake")

option(BUILD_EXAMPLES "Build and install examples" ON)
option(BUILD_DOCUMENTATION "Build the documentation" ON)

option(STATIC_LIBS "Build static libraries" OFF)
if(STATIC_LIBS)
    set(LIB_TYPE STATIC)
else(STATIC_LIBS)
    set(LIB_TYPE SHARED)
endif(STATIC_LIBS)

# Set up installation directories
set(BIN_INSTALL_DIR "bin")
set(LIB_INSTALL_DIR "lib")
set(INC_INSTALL_DIR
    "include/${PROJECT_NAME_LOWER}-${PROJECT_VERSION_MAJOR}")
set(SHARE_INSTALL_DIR
    "share/${PROJECT_NAME_LOWER}-${PROJECT_VERSION_MAJOR}")

# Get necessary dependency information
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
find_package(OpenRTM 1 REQUIRED)
find_package(HokuyoAIST 3 REQUIRED)

# Subdirectories
add_subdirectory(cmake)
if(BUILD_DOCUMENTATION)
    add_subdirectory(doc)
endif(BUILD_DOCUMENTATION)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)
add_subdirectory(idl)
add_subdirectory(include)
add_subdirectory(src)

# Package creation
include(InstallRequiredSystemLibraries)
set(PROJECT_EXECUTABLES ${PROJECT_NAME_LOWER}_standalone
    "RTC:HokuyoAIST_standalone")
set(cpack_options "${PROJECT_BINARY_DIR}/cpack_options.cmake")
configure_file("${PROJECT_SOURCE_DIR}/cmake/cpack_options.cmake.in"
    ${cpack_options} @ONLY)
set(CPACK_PROJECT_CONFIG_FILE ${cpack_options})
include(CPack)

