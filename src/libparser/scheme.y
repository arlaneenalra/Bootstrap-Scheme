%define api.pure

%{
#include <stdio.h>
#include "core.h"

void yyerror(interp_core_type *interp, yyscan_t scanner, char *s);

%}

%parse-param {interp_core_type *interp}
%parse-param {void * scanner}
%lex-param {yyscan_t scanner}

%token OPEN_PAREN
%token CLOSE_PAREN
%token DOT

%token TRUE_OBJ
%token FALSE_OBJ


%%

list:
    object
  | object list

tuple:
    OPEN_PAREN
    list
    CLOSE_PAREN
  | OPEN_PAREN
    object
    DOT
    object
    CLOSE_PAREN
  | OPEN_PAREN CLOSE_PAREN { printf("nil\n"); }


object:
    TRUE_OBJ        {  }
  | FALSE_OBJ       {  }
  | tuple



%%

void yyerror(interp_core_type *interp, yyscan_t scanner, char *s) {
    (void)fprintf(stderr,"There was an error parsing %s on line %i\n", 
		       s, yyget_lineno(scanner));
}