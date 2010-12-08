%option noyywrap reentrant bison-bridge
%option always-interactive
%option yylineno
%option nounput noinput

%{
#include "scheme.h"

/* avoid a compiler warning */
#define ECHO /* do nothing */

%}

DIGITS    [0-9]+
EOL       \n|\r\n|\n\r|\r
WS	  {EOL}|[[:blank:]]
IDENT     [!$%&*+\-./:<=>?@^_~[:alnum:]]
SIGNS     [-+]

CHAR_VAL      (newline|space|[^{WS}]|x[0-9A-F]+)

%x EXPECT_CHAR
%x EXPECT_STRING

%%
"("|"["   { return OPEN_PAREN; }
")"|"]"   { return CLOSE_PAREN; }
"#("      { return START_VECTOR; }
"."       { return DOT; }
"'"       { return QUOTE; }

"#t"      { return TRUE_OBJ; }
"#f"      { return FALSE_OBJ; }

{WS}*     /* Do nothing with white space */

{SIGNS}?{DIGITS}                       { return FIXED_NUMBER; }
{SIGNS}?({DIGITS}|nan|inf)?"."{DIGITS} { return FLOAT_NUMBER; }
{SIGNS}?{DIGITS}[/]{DIGITS}              { return FLOAT_NUMBER; }



#\\       { BEGIN(EXPECT_CHAR); }

<EXPECT_CHAR>{CHAR_VAL} {	BEGIN(INITIAL); 
				return CHAR_CONSTANT; }

\"        { BEGIN(EXPECT_STRING); return DOUBLE_QUOTE;}

<EXPECT_STRING>{

	([^\"]|\\\")*  { return STRING_CONSTANT; }
	\"             { BEGIN(INITIAL); return DOUBLE_QUOTE; }
}


{IDENT}+  { return SYMBOL; }

[;].*     /* Eat comments */
#!r6rs    /* Treat this as a comment */      

<<EOF>>   { return END_OF_FILE; }
%%

char * get_text(void * scanner) {
	return yyget_text(scanner);
}

