%define api.pure

%{
#include <stdio.h>
#include "scheme.h"
#include "lexer.h"

void yyerror(yyscan_t scanner, char *s);

%}

%parse-param {void * scanner}
%lex-param {yyscan_t scanner}

%token OPEN_PAREN
%token CLOSE_PAREN

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
  | OPEN_PAREN CLOSE_PAREN { printf("nil\n"); }


object:
    TRUE_OBJ        { printf("true\n"); }
  | FALSE_OBJ       { printf("false\n"); }
  | tuple



%%

void yyerror(yyscan_t scanner, char *s) {
    (void)fprintf(stderr,"There was an error parsing %s on line %i\n", 
		       s, yyget_lineno(scanner));
}