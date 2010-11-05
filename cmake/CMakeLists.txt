set(PKG_DESC "RT Component for Hokuyo laser scanners.")
set(PKG_EXTERNAL_DEPS "openrtm-aist gearbox")
set(pkg_conf_file ${CMAKE_CURRENT_BINARY_DIR}/rtc_hokuyo_aist.pc)
configure_file(rtc_hokuyo_aist.pc.in ${pkg_conf_file} @ONLY)
install(FILES ${pkg_conf_file}
    DESTINATION ${RTC_LIB_INSTALL_DIR}/pkgconfig/ COMPONENT pkgconfig)

