#ifndef __UTIL__
#define __UTIL__

void fail(const char *msg); /* Assert on a fatal error */
char *get_text(void * scanner); /* return text from the lexer */

void attach_sig_handlers(); /* attach signal handlers */


/* A macro to allocate a new exception in a sane maner */
#define set_exception(interp, msg, ... ) {       \
    char *c=0;                                   \
    int size=snprintf(0, 0, "Exception: " msg, ##__VA_ARGS__);     \
    c=(char *)alloc_string(interp, size+1);      \
    snprintf(c, size, "Exception: " msg, ##__VA_ARGS__);       \
    interp->exception=create_string(interp, c);  \
    interp->error=1;                             \
}                                                \


#endif
