#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "util.h"
#include "scheme_funcs.h"

#include "symbol.h"

/* include the bohem gc and our gc stuff */
#include <gc.h> 
#include "gc_funcs.h"


void create_booleans(interp_core_type *interp);

/* Add a new environment on top of the current one */
void push_environment(interp_core_type *interp, object_type *env) {

    TRACE("Peu");
    
    /* replace it with the passed in stack */
    interp->cur_env=cons(interp, interp->empty_list, env);
}

/* Create an instance of the Quote symbol */
void create_quote(interp_core_type * interp) {
    object_type *obj=0;

    obj=create_symbol(interp, "quote");

    interp->quote=obj;
}

/* Create instances of the global boolean values */
void create_booleans(interp_core_type *interp) {
    object_type *obj=0;

    /* true and false are defined by macros to be 
       memvers of interp->boolean */
    obj=alloc_object(interp,BOOL);
    obj->value.bool_val=1;

    true=obj;

    
    obj=alloc_object(interp,BOOL);
    obj->value.bool_val=0;

    false=obj;
}

/* Create a marker for the empty list */
void create_empty_list(interp_core_type *interp) {
    object_type *obj=0;

    obj=alloc_object(interp,TUPLE);

    interp->empty_list=obj;
}

/* Create a marker for EOF */
void create_eof_object(interp_core_type *interp) {
    object_type *obj=0;

    obj=alloc_object(interp,CHAR);

    eof_object=obj;
}

/* setup the base environment */
void create_base_environment(interp_core_type *interp) {

    interp->cur_env=interp->empty_list;

    /* There is nothing in the environment right now
       so, this is just the empty list */
    push_environment(interp,interp->empty_list);

    bind_symbol_list(interp, primitive_list, &interp->cur_env);

    /* for cond */
    bind_symbol(interp, 
		create_symbol(interp, "else"),
		true,
		&interp->cur_env);

    interp->top_env=interp->cur_env;

}

/* Create an instance of the interpreter */
interp_core_type *create_interp() {
    interp_core_type *interp=0;

    interp=alloc_interp();
    
    /* Setup the interpreter */
    if(interp) {
	bzero(interp, sizeof(interp_core_type));


	/* create some special values */
	create_booleans(interp);

	/* create special case symbols */
	create_quote(interp);
	
	/* create the empty list */
	create_empty_list(interp);

	/* create the eof_object */
	create_eof_object(interp);

	/* setup the base environment */
	create_base_environment(interp);

	interp->running=1;

	return interp;
    }
    
    fail("Unable to create interpreter instance");
    return 0;
}

/* Clean up allocations in the interpreter. */
void cleanup_interp(interp_core_type *interp) {

    /* Clean up all allocated parsers */
    while(interp->scanner!=0) {
	pop_parse_state(interp);
    }
}

/* Find the length of a passed in list */
int list_length(interp_core_type* interp, object_type *args) {
    int count=0;
    
    while(!is_empty_list(interp, args)) {
	count++;
	args=cdr(args);
    }

    return count;
}


