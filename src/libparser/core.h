#ifndef _CORE_
#define _CORE_

#include <stdio.h>

#include <stdint.h>

#include "scheme.h"
#include "lexer.h"

/* Add in some debuggin messages */
#ifdef DEBUG

#define TRACE(x)  (void)fprintf(stderr, "%s",x);

#else

#define TRACE(x)

#endif


typedef int8_t bool;

/* The types of cells that we can represent */
typedef enum {
    FIXNUM,
    FLOATNUM,
    BOOL,
    CHAR,
    STRING,
    TUPLE,
    
    CHAIN /* internal type */
} object_type_enum;

/* Used to tell add object that we are setting a car or cdr */
typedef enum {
    NONE,
    BARE,
    CAR,
    CDR
} setting_type_enum;

/* Define a structure to represent a memory cell */
typedef struct object {
    object_type_enum type;

    union {
	int64_t int_val;
	long double float_val; /* There should be a better way to do this */
	char char_val; 
	char *string_val;
	bool bool_val;
	struct {
	    struct object *car;
	    struct object *cdr;
	} tuple;
    } value;

    struct object *next;
} object_type;

typedef struct bool_global {
    object_type *true;
    object_type *false;
} bool_global_type;

/* Configuartion for the garbage collector */
typedef struct gc {
    object_type *active_list;
    object_type *free_list;
} gc_type;


/* Values that are required for an instance of the interpreter 
   to function properly */
typedef struct interp_core {
    bool running;
    
    bool_global_type boolean;

    //setting_type_enum setting;
    /* last added object */
    object_type *added;

    /* root of the current object graph */
    object_type *root;

    /* object we are currently working on */
    object_type *current;


    
    object_type *state_stack;

    gc_type gc; /* Where the garbage collector keeps it's data */
    yyscan_t scanner; /* an instance of the parser/lexer */
} interp_core_type;


/* Function definitions */
void set(interp_core_type *interp, setting_type_enum setting);
object_type *alloc_object(interp_core_type *interp, object_type_enum obj_type);

interp_core_type *create_interp();
void cleanup_interp(interp_core_type *interp);

object_type *parse(interp_core_type *interp, const char *buf);
void add_object(interp_core_type *interp, object_type *obj);
void add_char(interp_core_type *interp, char *str);
void add_number(interp_core_type *interp, char *str);
void add_float(interp_core_type *interp, char *str);
void add_string(interp_core_type *interp, char *str);

void chain_state(interp_core_type *interp);
void push_state(interp_core_type *interp);
void pop_state(interp_core_type *interp);

#endif
