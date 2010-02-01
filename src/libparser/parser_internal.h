#ifndef _PARSER_INTERNAL_
#define _PARSER_INTERNAL_

/* Add in some debuggin messages */
#ifdef DEBUG
#define TRACE(x)  (void)fprintf(stderr, "%s",x);
#else
#define TRACE(x)
#endif

/* include the bohem gc and our gc stuff */
#include <gc.h> 
#include "gc_funcs.h"

/* Used to tell add object that we are setting a car or cdr */
typedef enum {
    CAR,
    CDR
} setting_type_enum;

/* Function definitions */
void set(interp_core_type *interp, setting_type_enum setting);
int list_length(interp_core_type * interp, object_type *args); /* Find the length of a passed in list */

void add_object(interp_core_type *interp, object_type *obj);
void add_char(interp_core_type *interp, char *str);
void add_number(interp_core_type *interp, char *str);
void add_float(interp_core_type *interp, char *str);
void add_string(interp_core_type *interp, char *str);
void add_quote(interp_core_type *interp);
void add_symbol(interp_core_type *interp, char *str);


object_type *create_string(interp_core_type *interp, char *str);
object_type *create_symbol(interp_core_type *interp, char *str);

void chain_state(interp_core_type *interp);
void push_state(interp_core_type *interp);
void pop_state(interp_core_type *interp);

void push_environment(interp_core_type *interp, object_type *env);


#endif