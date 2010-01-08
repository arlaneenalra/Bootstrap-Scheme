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
  | object DOT object
  | object list

tuple:
    OPEN_PAREN     { add_object(interp, alloc_object(interp, TUPLE)); }
    list
    CLOSE_PAREN    { /* end of current chain . . . */ }
  | OPEN_PAREN CLOSE_PAREN { add_object(interp, 0); }


object:
    TRUE_OBJ        { add_object(interp, interp->boolean.true); }
  | FALSE_OBJ       { add_object(interp, interp->boolean.false); }
  | tuple           { /* shouldn't be anything to do here */ }



%%

void yyerror(interp_core_type *interp, yyscan_t scanner, char *s) {
    (void)fprintf(stderr,"There was an error parsing %s on line %i\n", 
		       s, yyget_lineno(scanner));
}