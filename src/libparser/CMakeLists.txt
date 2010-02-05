
# setup flex and bison
find_package(BISON)
find_package(FLEX)

BISON_TARGET(PARSER scheme.y ${CMAKE_CURRENT_BINARY_DIR}/scheme.c)
FLEX_TARGET(LEXER lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)

ADD_FLEX_BISON_DEPENDENCY(LEXER PARSER)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(libparser core.c eval.c gc_funcs.c output.c 
  scheme_funcs.c util.c ${BISON_PARSER_OUTPUTS} ${FLEX_LEXER_OUTPUTS})