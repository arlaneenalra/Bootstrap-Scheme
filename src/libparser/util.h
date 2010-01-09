#ifndef __UTIL__
#define __UTIL__

void fail(const char *msg); /* Assert on a fatal error */
char *get_text(void * scanner); /* return text from the lexer */

#endif
