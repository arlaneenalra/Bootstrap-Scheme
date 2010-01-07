#ifndef _CORE_
#define _CORE_

#include <stdio.h>
#include <wchar.h>

#include <stdint.h>

#include "scheme.h"
#include "lexer.h"

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
	int8_t bool_val;
    } value;

} object_type;



/* Values that are required for an instance of the interpreter 
   to function properly */
typedef struct interp_core {
    char running;
    

    yyscan_t scanner;
} interp_core_type;


/* Function definitions */
object_type *alloc_object(interp_core_type* core, object_type_enum obj_type);
interp_core_type *create_interp();
void cleanup_interp(interp_core_type *interp);

#endif
