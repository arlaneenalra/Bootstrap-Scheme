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

%token SYMBOL


%%

dot_pair:
    object DOT object
  | object

tuple:
    OPEN_PAREN     { push_state(interp); }
    dot_pair
    CLOSE_PAREN    { pop_state(interp); }

nil:
    OPEN_PAREN CLOSE_PAREN { add_object(interp, 0); }    

boolean:
    TRUE_OBJ        { add_object(interp, interp->boolean.true); }
  | FALSE_OBJ       { add_object(interp, interp->boolean.false); }
    
object:
    boolean
  | nil
  | SYMBOL
  | tuple



%%

void yyerror(interp_core_type *interp, yyscan_t scanner, char *s) {
    (void)fprintf(stderr,"There was an error parsing %s on line %i\n", 
		       s, yyget_lineno(scanner));
}
