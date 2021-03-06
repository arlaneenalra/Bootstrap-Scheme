%define api.pure

%{
#include <stdio.h>
#include "core.h"
#include "util.h"
#include "parser_internal.h"

#include "scheme.h"
#include "lexer.h"

void yyerror(interp_core_type *interp, void *scanner, char *s);

%}

%parse-param {interp_core_type *interp}
%parse-param {void *scanner}
%lex-param {void *scanner}

%token OPEN_PAREN
%token START_VECTOR
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

%token END_OF_FILE

%%

expression:
    object         { YYACCEPT; }
  | END_OF_FILE    { end_of_file(interp); YYACCEPT; }

vector_body:
    object         { chain_state(interp); }
  | object         { chain_state(interp); }
    vector_body

vector_end:
    CLOSE_PAREN    { pop_state(interp); add_empty_vector(interp); }
  | vector_body
    CLOSE_PAREN    { pop_state(interp); add_vector(interp); }

vector:
    START_VECTOR   { push_state(interp); }
    vector_end
   
list_end:
    list_next
    CLOSE_PAREN    { pop_state(interp); }
  | CLOSE_PAREN    { pop_state(interp); add_object(interp, interp->empty_list);}

list:
    OPEN_PAREN     { push_state(interp); }
    list_end       

list_next:
    object         { chain_state(interp); }
  | object         { chain_state(interp); }
    list_next
  | object         { chain_state(interp); }
    DOT
    object         { set(interp, CDR); }

quoted_list:
    QUOTE          
    object         { add_quote(interp); }
    
boolean:
    TRUE_OBJ        { add_object(interp, interp->boolean.true); }
  | FALSE_OBJ       { add_object(interp, interp->boolean.false); }

number:
    FIXED_NUMBER    { add_number(interp, get_text(scanner)); }
  | FLOAT_NUMBER    { add_float(interp, get_text(scanner)); }

string_end:
    STRING_CONSTANT { add_string(interp, get_text(scanner)); }
    DOUBLE_QUOTE
  | DOUBLE_QUOTE    { add_string(interp, ""); }

string:
    DOUBLE_QUOTE
    string_end
    
object:
    boolean
  | CHAR_CONSTANT   { add_char(interp, get_text(scanner)); }
  | string 
  | SYMBOL          { add_symbol(interp, get_text(scanner)); }
  | number
  | list
  | quoted_list
  | vector


%%

void yyerror(interp_core_type *interp, void *scanner, char *s) {
/*    (void)fprintf(stderr,"There was an error parsing %s on line %i\n", 
		       s, yyget_lineno(scanner));*/
      interp->error=1;
}


int parse_internal(interp_core_type *interp, void *scanner) {
    int ret_val=0;
    int parsing_flag=0;
    
    /* save off the flag */
    parsing_flag=interp->parsing;

    interp->parsing=1;

    ret_val=yyparse(interp, scanner);

    /* put it back to what it was before we started */
    interp->parsing=parsing_flag;

    return ret_val;
}
