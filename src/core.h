#ifndef _CORE_
#define _CORE_

#include <stdio.h>
#include <wchar.h>

#include <stdint.h>

/* The types of cells that we can represent */
typedef enum {
    INTEGER,
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
    
} interp_core_type;


#endif
