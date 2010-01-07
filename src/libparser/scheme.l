%option noyywrap 

%%

"("       { return 1; }

")"       { return 0; } 

%%

void set_stream(FILE *fin) {
    yyin=fin;
}