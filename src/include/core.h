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
    bool eval_first;
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

#include "interp.h"

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

int parse_internal(interp_core_type *interp, void *scanner);

object_type *eval(interp_core_type *interp, object_type *obj);

void output(interp_core_type *interp, object_type *obj);
void output_stream(interp_core_type *interp, object_type *obj, FILE *fout);

bool has_error(interp_core_type *interp);
void set_exception(interp_core_type *interp, const char *msg);

void end_of_file(interp_core_type *interp);


#endif
