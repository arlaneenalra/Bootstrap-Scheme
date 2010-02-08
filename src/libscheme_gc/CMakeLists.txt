

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(libscheme_gc gc_funcs.c)

target_link_libraries(gc)

include(${PROJECT_SOURCE_DIR}/CMakeIncludes.txt)
