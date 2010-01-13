%define api.pure

%{
#include <stdio.h>
#include "core.h"
#include "util.h"

void yyerror(interp_core_type *interp, void *scanner, char *s);

%}

%parse-param {interp_core_type *interp}
%parse-param {void *scanner}
%lex-param {void *scanner}

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
    object         { YYACCEPT; }

/* list processing */
list:
    OPEN_PAREN CLOSE_PAREN  { add_object(interp, 0); }
  | OPEN_PAREN     { push_state(interp); }
    object_pair_list
    CLOSE_PAREN    { pop_state(interp); }

object_pair_list:
    object         { set(interp, CAR); }
  | list_next
  
list_next:
    object         { chain_state(interp); }
  | object         { chain_state(interp); }
    list_next
  | object         { chain_state(interp); }
    DOT
    object         { set(interp, CDR); }

quoted_list:
    QUOTE          { /*push_state(interp);*/ }
    object         { quote(interp); }
    
boolean:
    TRUE_OBJ        { add_object(interp, interp->boolean.true); }
  | FALSE_OBJ       { add_object(interp, interp->boolean.false); }

number:
    FIXED_NUMBER    { add_number(interp, get_text(scanner)); }
  | FLOAT_NUMBER    { add_float(interp, get_text(scanner)); }

string:
    DOUBLE_QUOTE
    STRING_CONSTANT { add_string(interp, get_text(scanner)); }
    DOUBLE_QUOTE
    
object:
    boolean
  | CHAR_CONSTANT   { add_char(interp, get_text(scanner)); }
  | string 
  | SYMBOL          { add_symbol(interp, get_text(scanner)); }
  | number
  | list
  | quoted_list


%%

void yyerror(interp_core_type *interp, void *scanner, char *s) {
/*    (void)fprintf(stderr,"There was an error parsing %s on line %i\n", 
		       s, yyget_lineno(scanner));*/
      interp->error=1;
}
