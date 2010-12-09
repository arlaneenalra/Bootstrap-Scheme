
include(${PROJECT_SOURCE_DIR}/CMakeIncludes.txt)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(eval eval.c core.c)

target_link_libraries(eval)

