

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(libeval eval.c core.c)

target_link_libraries(eval)

include(${PROJECT_SOURCE_DIR}/CMakeIncludes.txt)
