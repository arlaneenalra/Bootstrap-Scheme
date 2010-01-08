#ifndef _CORE_
#define _CORE_

#include <stdio.h>
#include <wchar.h>

#include <stdint.h>

#include "scheme.h"
#include "lexer.h"


typedef int8_t bool;

/* The types of cells that we can represent */
typedef enum {
    FIXNUM,
    BOOL,
    CHAR
} object_type_enum;

/* Define a structure to represent a memory cell */
typedef struct object {
    object_type_enum type;

    union {
	int64_t int_val;
	wchar_t char_val; 
	bool bool_val;
    } value;

    struct object_type *next;
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

    gc_type gc; /* Where the garbage collector keeps it's data */
    yyscan_t scanner; /* an instance of the parser/lexer */
} interp_core_type;


/* Function definitions */
object_type *alloc_object(interp_core_type* core, object_type_enum obj_type);

interp_core_type *create_interp();
void cleanup_interp(interp_core_type *interp);

object_type *parse(interp_core_type *interp, const char *buf);

#endif
