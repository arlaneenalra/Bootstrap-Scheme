%option noyywrap reentrant bison-bridge
%option yylineno

%{
#include "scheme.h"

%}

DIGITS    [0-9]+
EOL     \n|\r\n|\n\r|\r
WS	{EOL}|[[:blank:]]

%%

"("       { return OPEN_PAREN; }
")"       { return CLOSE_PAREN; }

"#t"      { return TRUE_OBJ; }
"#f"      { return FALSE_OBJ; }

{WS}*     /* Do nothing with white space */

%%
