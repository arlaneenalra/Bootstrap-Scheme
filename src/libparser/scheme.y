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

%token QUOTE
%token DOUBLE_QUOTE

%token STRING_CONSTANT
%token CHAR_CONSTANT

%token FIXED_NUMBER
%token FLOAT_NUMBER

%token SYMBOL


%%

expression:
    object         { set(interp, BARE); }

tuple:
    OPEN_PAREN CLOSE_PAREN  { add_object(interp, 0); }
  | OPEN_PAREN     { add_object(interp, alloc_object(interp, TUPLE)); set(interp, NONE);}
    object         { set(interp, CAR);}
    DOT 
    object         { set(interp, CDR);}
    CLOSE_PAREN    { pop_state(interp); }


boolean:
    TRUE_OBJ        { add_object(interp, interp->boolean.true); }
  | FALSE_OBJ       { add_object(interp, interp->boolean.false); }

number:
    FIXED_NUMBER    { add_number(interp, yyget_text(scanner)); }
  | FLOAT_NUMBER    { add_float(interp, yyget_text(scanner)); }

string:
    DOUBLE_QUOTE
    STRING_CONSTANT { add_string(interp, yyget_text(scanner)); }
    DOUBLE_QUOTE
    
object:
    boolean
  | CHAR_CONSTANT   { add_char(interp, yyget_text(scanner)); }
  | string 
  | number
  | tuple



%%

void yyerror(interp_core_type *interp, yyscan_t scanner, char *s) {
    (void)fprintf(stderr,"There was an error parsing %s on line %i\n", 
		       s, yyget_lineno(scanner));
}
