#ifndef _INTERP_
#define _INTERP_

typedef struct bool_global {
    object_type *true;
    object_type *false;
} bool_global_type;

/* used to keep track of a stack of lexers */
typedef struct scanner_stack {
    void * scanner;
    struct scanner_stack *previous;
} scanner_stack_type;

/* Stores symbols for look up latter. */
typedef struct symbol_table {
    object_type *list;
} symbol_table_type;

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
    
    /* Do we have an exception ? */
    object_type *exception;

    /* List of all symbols in the system */
    symbol_table_type symbols;

    scanner_stack_type *scanner; /* an instance of the parser/lexer */
    
} interp_core_type;

#endif
