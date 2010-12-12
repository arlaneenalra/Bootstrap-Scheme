#ifndef __UTIL__
#define __UTIL__

void fail(const char *msg); /* Assert on a fatal error */
char *get_text(void * scanner); /* return text from the lexer */

void attach_sig_handlers(); /* attach signal handlers */


/* A macro to allocate a new exception in a sane maner */
#define set_exception_private(interp, prefix_msg, msg, ... ) {     \
    char *c=0;                                                     \
    int size=snprintf(0, 0, prefix_msg msg, ##__VA_ARGS__);        \
    c=(char *)alloc_string(interp, size+1);                        \
    snprintf(c, size, prefix_msg msg, ##__VA_ARGS__);              \
    interp->exception=create_string(interp, c);                    \
    interp->error=1;                                               \
}                                                                  \

#define set_exception(interp, msg, ...)                              \
    set_exception_private(interp, "Exception: ", msg, ##__VA_ARGS__) \


/* Wrap an already existing exception */
#define wrap_exception(interp, msg, ...) {                         \
    object_type *obj=interp->exception;                            \
    set_exception_private(interp, "", msg, ##__VA_ARGS__);         \
    set_exception(interp, "%s\nWrapping:\n%s",                     \
        interp->exception->value.string_val,                       \
        obj->value.string_val);                                    \
}                                                                  \


#endif
