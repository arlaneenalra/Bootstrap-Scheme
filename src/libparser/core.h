#ifndef _CORE_
#define _CORE_

#include <stdio.h>

#include <inttypes.h>

#include "scheme.h"

/* Add in some debuggin messages */
#ifdef DEBUG

#define TRACE(x)  (void)fprintf(stderr, "%s",x);

#else

#define TRACE(x)

#endif


typedef int8_t bool;
struct interp_core;

/* The types of cells that we can represent */
typedef enum {
    FIXNUM,
    FLOATNUM,
    BOOL,
    CHAR,
    STRING,
    TUPLE,
    SYM,

    PRIM, /* primitive c functions */
    CHAIN /* internal type */
} object_type_enum;

/* Used to tell add object that we are setting a car or cdr */
typedef enum {
    CAR,
    CDR
} setting_type_enum;

struct object;

typedef struct object *(*primitive_type)
    (struct interp_core *interp, struct object *);



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
	struct {
	    char *name;
	    struct object *binding;
	} symbol;

	primitive_type primitive;

    } value;

    struct object *next;
} object_type;

typedef struct bool_global {
    object_type *true;
    object_type *false;
} bool_global_type;


/* Stores symbols for look up latter. */
typedef struct symbol_table {
    object_type *list;
} symbol_table_type;


/* Configuartion for the garbage collector */
typedef struct gc {
    object_type *active_list;
    object_type *free_list;
} gc_type;


/* Values that are required for an instance of the interpreter 
   to function properly */
typedef struct interp_core {
    bool error;
    bool running;
    
    /* Instances of #t and #f */
    bool_global_type boolean;

    /*Iinstance of the quote symbol */
    object_type *quote;

    /* object to be attached to the object graph*/
    object_type *added;

    /* Object we are currently working on */
    object_type *current;

    /* Used by the parser to generate lists */
    object_type *state_stack;

    /* Stores variable bindings */
    object_type *env_stack;
    
    /* Where the garbage collector keeps it's data */
    gc_type gc; 

    /* List of all symbols in the system */
    symbol_table_type symbols;

    void * scanner; /* an instance of the parser/lexer */
    
} interp_core_type;


#include "gc.h"

/* Function definitions */
void set(interp_core_type *interp, setting_type_enum setting);
int list_length(object_type *args); /* Find the length of a passed in list */


interp_core_type *create_interp();
void cleanup_interp(interp_core_type *interp);

object_type *parse(interp_core_type *interp, FILE *in);
object_type *eval(interp_core_type *interp, object_type *obj);
void output(interp_core_type *interp, object_type *obj);

void add_object(interp_core_type *interp, object_type *obj);
void add_char(interp_core_type *interp, char *str);
void add_number(interp_core_type *interp, char *str);
void add_float(interp_core_type *interp, char *str);
void add_string(interp_core_type *interp, char *str);
void add_quote(interp_core_type *interp);

void chain_state(interp_core_type *interp);
void push_state(interp_core_type *interp);
void pop_state(interp_core_type *interp);

object_type *create_symbol(interp_core_type *interp, char *str);

#endif
