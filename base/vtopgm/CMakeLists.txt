PROJECT(VTOPGM)

ADD_EXECUTABLE(vtopgm vtopgm.c)
TARGET_LINK_LIBRARIES(vtopgm via)

INSTALL(TARGETS vtopgm
        RUNTIME DESTINATION ${VIA_INSTALL_BIN_DIR}
        COMPONENT RuntimeLibraries)
