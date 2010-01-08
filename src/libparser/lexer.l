%option noyywrap reentrant bison-bridge
%option yylineno

%{
#include "scheme.h"

%}

DIGITS    [0-9]+
EOL       \n|\r\n|\n\r|\r
WS	  {EOL}|[[:blank:]]
IDENT     (.)

CHAR_VAL      (newline|space|[^{WS}])

%x EXPECT_CHAR

%%

"("       { return OPEN_PAREN; }
")"       { return CLOSE_PAREN; }
"."       { return DOT; }

"#t"      { return TRUE_OBJ; }
"#f"      { return FALSE_OBJ; }

{WS}*     /* Do nothing with white space */

[-]?{DIGITS} { return FIXED_NUMBER; }
[-]?{DIGITS}"."{DIGITS} { return FLOAT_NUMBER; }


#\\       { BEGIN(EXPECT_CHAR); }

<EXPECT_CHAR>{CHAR_VAL} {	BEGIN(INITIAL); 
				return CHAR_CONSTANT; }

[^0-9\(\)#'".[:blank:]]{IDENT}+  { return SYMBOL; }

%%
