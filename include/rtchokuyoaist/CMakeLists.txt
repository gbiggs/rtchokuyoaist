set(hdrs hokuyoaist_impl.h
    ranger_impl.h
    rtc.h)

install(FILES ${hdrs} DESTINATION ${INC_INSTALL_DIR}/${PROJECT_NAME_LOWER}
    COMPONENT library)

