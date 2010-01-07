%option noyywrap reentrant bison-bridge
%option yylineno

%{
#include "scheme.h"

%}

%%

"("       { return OPEN_PAREN; }

")"       { return CLOSE_PAREN; } 

%%
