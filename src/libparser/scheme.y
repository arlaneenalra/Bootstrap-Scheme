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

%%

s_expression:
    OPEN_PAREN
    CLOSE_PAREN


%%

void yyerror(yyscan_t scanner, char *s) {
    (void)frprintf(stderr,"There was an error parsing %s on line %i\n", 
		       s, yyget_lineno(scanner));
}