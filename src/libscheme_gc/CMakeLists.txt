
include(${PROJECT_SOURCE_DIR}/CMakeIncludes.txt)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(scheme_gc gc_funcs.c create.c)

target_link_libraries(gc)

