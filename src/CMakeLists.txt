include_directories(libparser)

link_directories(${libparser_BIN_DIR}/libparser)


add_executable(bootstrap bootstrap.c)

