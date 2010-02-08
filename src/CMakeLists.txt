
add_subdirectory(libparser)
add_subdirectory(libscheme_gc)

include_directories(libparser libscheme_gc)

link_directories(${libparser_BIN_DIR}/libparser
	${libscheme_gc_BIN_DIR}/libscheme_gc)


add_executable(bootstrap bootstrap.c)

target_link_libraries(bootstrap libparser libscheme_gc gc)


include(${PROJECT_SOURCE_DIR}/CMakeIncludes.txt)
