#ifndef _CORE_
#define _CORE_

#include <stdio.h>

#include <inttypes.h>

/* Add in some debugging messages */
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
    IMAG,
    BOOL,
    CHAR,
    STRING,
    VECTOR,
    TUPLE,
    SYM,
    PORT,

    PRIM, /* primitive c functions */
    CLOSURE, /* a closure */
    CHAIN /* internal type */
} object_type_enum;

/* used by the gc */
typedef enum {
    MARK_RED, /* Everything else */
    MARK_BLACK,

    MARK_PERM  /* Things that will not be deallocated while we are running */
} gc_mark_type;

struct object;

typedef	struct tuple {
    struct object *car;
    struct object *cdr;
} tuple_type;

typedef struct object *(*fn_type)
    (struct interp_core *interp, struct object *);

typedef struct primitive {
    fn_type fn;
    bool eval_first; /* evaluate arguements ? */
    bool eval_end; /* evaluate result? */
} primitive_type;

typedef struct port {
    FILE *port;
    bool input;
    bool output;
} port_type;

typedef struct closure {
    struct object *param;
    struct object *body;
    struct object *env;
} closure_type;

typedef struct vector {
    uint64_t length;
    struct object **vector;
} vector_type;

typedef struct imaginary {
    long double real;
    long double imaginary;

} imaginary_type;

/* Define a structure to represent a memory cell */
typedef struct object {
    object_type_enum type;

    union {
	int64_t int_val;
	long double float_val; /* There should be a better way to do this */
	imaginary_type imaginary;
	char char_val; 
	char *string_val;
	vector_type vector;
	bool bool_val;
	tuple_type tuple;
	primitive_type primitive;
	closure_type closure;
	port_type port_val;
    } value;

    gc_mark_type mark;
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


/* used to keep track of a stack of lexers */
typedef struct scanner_stack {
    void * scanner;
    struct scanner_stack *previous;
} scanner_stack_type;

/* Values that are required for an instance of the interpreter 
   to function properly */
typedef struct interp_core {
    bool error; /* An error has occured */
    bool running; /* The interpreter is running */
    bool parsing; /* Is the parser running */
    
    /* Instances of #t and #f */
    bool_global_type boolean;
    
    /* The empty list */
    object_type *empty_list;

    /* Instance of the quote symbol */
    object_type *quote;
    
    object_type *eof_object;

    /* Object to be attached to the object graph */
    object_type *added;

    /* Object we are currently working on */
    object_type *current;


    /* Used by the parser to generate lists */
    object_type *state_stack;

    /* Stores variable bindings */
    object_type *cur_env;
    
    /* Top level environment */
    object_type *top_env;
    
    /* List of all symbols in the system */
    symbol_table_type symbols;

    scanner_stack_type *scanner; /* an instance of the parser/lexer */
    
} interp_core_type;


interp_core_type *create_interp();
void repl(interp_core_type *interp, int scheme);
void top_level_program(interp_core_type *interp, char *filename);
void cleanup_interp(interp_core_type *interp);

object_type *parse(interp_core_type *interp, FILE *in);
object_type *parse_string(interp_core_type *interp, char *in);
object_type *parse_chain(interp_core_type *interp);
void reset(interp_core_type *interp);

void push_parse_state(interp_core_type *interp, FILE *in);
void pop_parse_state(interp_core_type *interp);

object_type *clone(interp_core_type *interp, object_type *obj);

int parse_internal(interp_core_type *interp, void *scanner);

object_type *eval(interp_core_type *interp, object_type *obj);

void output(interp_core_type *interp, object_type *obj);
void output_stream(interp_core_type *interp, object_type *obj, FILE *fout);

bool has_error(interp_core_type *interp);

void end_of_file(interp_core_type *interp);


#endif
