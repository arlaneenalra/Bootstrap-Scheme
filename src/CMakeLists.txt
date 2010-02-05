
add_subdirectory(libparser)

include_directories(libparser)

link_directories(${libparser_BIN_DIR}/libparser)


add_executable(bootstrap bootstrap.c)

target_link_libraries(bootstrap libparser gc)


include(${PROJECT_SOURCE_DIR}/CMakeIncludes.txt)