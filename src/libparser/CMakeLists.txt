
# setup flex and bison
find_package(BISON)
find_package(FLEX)

# flex generates a .h file, it would appear that no one handles this correctly
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/lexer.h)

BISON_TARGET(PARSER scheme.y ${CMAKE_CURRENT_BINARY_DIR}/scheme.c)
FLEX_TARGET(LEXER lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c
  COMPILE_FLAGS "--header-file=${CMAKE_CURRENT_BINARY_DIR}/lexer.h")

ADD_FLEX_BISON_DEPENDENCY(LEXER PARSER)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

add_library(libparser 
  core.c 
  output.c 
  repl.c
  scheme_funcs.c 
  util.c 
  ${BISON_PARSER_OUTPUTS} 
  ${FLEX_LEXER_OUTPUTS})

include(${PROJECT_SOURCE_DIR}/CMakeIncludes.txt)
