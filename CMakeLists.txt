cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(RTCHokuyoAist)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

include(${PROJECT_SOURCE_DIR}/cmake/rtcomp_utils.cmake)

# Set version and OS-specific settings
set(RTC_VERSION 2.0.0 CACHE STRING "RTC:HokuyoAist version")
DISSECT_VERSION()
GET_OS_INFO()

# Add an "uninstall" target
CONFIGURE_FILE ("${PROJECT_SOURCE_DIR}/cmake/uninstall_target.cmake.in"
    "${PROJECT_BINARY_DIR}/uninstall_target.cmake" IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET (uninstall "${CMAKE_COMMAND}" -P
    "${PROJECT_BINARY_DIR}/uninstall_target.cmake")

# Component source files
set(comp_srcs comp/rtc.cpp
              comp/ranger_impl.cpp
              comp/hokuyoaist_impl.cpp)
set(standalone_srcs comp/standalone.cpp)
set(idls ranger
         hokuyoaist)

# Compile the built-in IDL files
COMPILE_IDL_FILES(${PROJECT_SOURCE_DIR}/idl ${idls})
FILTER_LIST("${_idl_srcs}" "hh$" _installable_idl_srcs)
install(FILES ${_installable_idl_srcs}
    DESTINATION ${RTC_INCLUDE_INSTALL_DIR}/idl COMPONENT headers)

# Get necessary dependency information
GET_PKG_CONFIG_INFO(openrtm-aist REQUIRED)
GET_PKG_CONFIG_INFO(hokuyo_aist REQUIRED >=2.0.0)

# Component target
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR})
if(openrtm-aist_INCLUDE_DIRS OR hokuyo_aist_INCLUDE_DIRS)
    include_directories(${openrtm-aist_INCLUDE_DIRS}
        ${hokuyo_aist_INCLUDE_DIRS})
endif(openrtm-aist_INCLUDE_DIRS OR hokuyo_aist_INCLUDE_DIRS)
if(openrtm-aist_LIBRARY_DIRS OR hokuyo_aist_LIBRARY_DIRS)
    link_directories(${openrtm-aist_LIBRARY_DIRS}
        ${hokuyo_aist_LIBRARY_DIRS})
endif(openrtm-aist_LIBRARY_DIRS OR hokuyo_aist_LIBRARY_DIRS)
if(openrtm-aist_CFLAGS OR hokuyo_aist_CFLAGS)
    set_source_files_properties(${comp_srcs} ${proxy_srcs}
        PROPERTIES COMPILE_FLAGS
        "${openrtm-aist_CFLAGS} ${hokuyo_aist_CFLAGS}")
endif(openrtm-aist_CFLAGS OR hokuyo_aist_CFLAGS)

add_library(rtc_hokuyoaist SHARED ${comp_srcs} ${proxy_srcs} ${_idl_srcs})
add_executable(rtc_hokuyoaist_standalone ${standalone_srcs})

target_link_libraries(rtc_hokuyoaist_standalone rtc_hokuyoaist)
if(openrtm-aist_LINK_FLAGS OR hokuyo_aist_LINK_FLAGS)
    set_target_properties(rtc_hokuyoaist PROPERTIES LINK_FLAGS
        "${openrtm-aist_LINK_FLAGS} ${hokuyo_aist_LINK_FLAGS}")
endif(openrtm-aist_LINK_FLAGS OR hokuyo_aist_LINK_FLAGS)
if(openrtm-aist_LINK_LIBS OR hokuyo_aist_LINK_LIBS)
    target_link_libraries(rtc_hokuyoaist ${openrtm-aist_LINK_LIBS}
        ${hokuyo_aist_LINK_LIBS})
endif(openrtm-aist_LINK_LIBS OR hokuyo_aist_LINK_LIBS)

install(TARGETS rtc_hokuyoaist LIBRARY DESTINATION ${RTC_LIB_INSTALL_DIR}
    COMPONENT component)
install(TARGETS rtc_hokuyoaist_standalone RUNTIME DESTINATION bin
    COMPONENT component)

# Subdirectories
add_subdirectory(cmake)
add_subdirectory(doc)
add_subdirectory(examples)
add_subdirectory(idl)

# Package creation
include(InstallRequiredSystemLibraries)
set(RTC_DESCRIPTION "RT-Component for Hokuyo laser scanners.")
set(RTC_VENDOR "Geoffrey Biggs, AIST")
set(RTC_EXECUTABLES "rtc_hokuyoaist_standalone" "RTC:HokuyoAist")
configure_file("${PROJECT_SOURCE_DIR}/cmake/cpack_options.cmake.in"
    "${PROJECT_BINARY_DIR}/cpack_options.cmake" @ONLY)
set(CPACK_PROJECT_CONFIG_FILE "${PROJECT_BINARY_DIR}/cpack_options.cmake")
include(CPack)

