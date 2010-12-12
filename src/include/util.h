#ifndef __UTIL__
#define __UTIL__

void fail(const char *msg); /* Assert on a fatal error */
void cause_bt();
char *get_text(void * scanner); /* return text from the lexer */

void attach_sig_handlers();

#endif
