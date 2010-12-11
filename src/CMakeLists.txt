
add_subdirectory(libparser)
add_subdirectory(libeval)
add_subdirectory(libscheme_gc)
add_subdirectory(libexception)

include_directories(libparser libeval libscheme_gc)

link_directories(${libparser_BIN_DIR}/libparser
  ${libeval_BIN_DIR}/libeval
  ${libscheme_gc_BIN_DIR}/libscheme_gc)


add_executable(bootstrap bootstrap.c)

target_link_libraries(bootstrap parser eval scheme_gc dl)


include(${PROJECT_SOURCE_DIR}/CMakeIncludes.txt)
