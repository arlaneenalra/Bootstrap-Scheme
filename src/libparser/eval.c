#include "core.h"
#include "util.h"
#include "scheme_funcs.h"


/* The returned object is a tail call and not a terminal one */
void set_tail(interp_core_type *interp, object_type *obj) {
    interp->tail=1;
}

/* Evaluate a symbol */
object_type *eval_symbol(interp_core_type *interp, object_type *obj) {
    object_type *binding=0;

    binding=get_binding(interp, obj);

    /* Check the binding */
    if(binding==0) {
	/* Unbound variable */
	interp->error=1;
	return 0;
    }
    return cdr(binding);
}

/* evaluate a tagged list */
object_type *eval_tagged_list(interp_core_type *interp, object_type *obj) {
    object_type *evaled_args=0;
    object_type *binding=0;

    binding=get_binding(interp, car(obj));
	
    /* If the symbol is unbound, 
       we have an error */
    if(binding==0) {
	TRACE("e");
	interp->error=1;
	return 0;
    }	

    /* Make sure that the procedure is callable */
    if(is_primitive(interp, cdr(binding))) {
	TRACE("Pi");
	/* Call the primitive and return, make sure to skip the
	   symbol ref at the start of the list */
	return (*(cdr(binding)->value.primitive))(interp, cdr(obj));
    }
}

/* Main entry point to evaluator */
object_type *eval(interp_core_type *interp, object_type *obj) {

    do {
	interp->tail=0; /* reset the tail call indicator */

	TRACE("E");
    
	/* check for the empty list */
	if(obj==0) {
	    return 0;
	}

	/* Check for self evaluating */
	if(is_self_evaluating(interp, obj)) {
	    TRACE("s");
	    return obj;
	}

	/* Check for a symbol */
	if(is_symbol(interp, obj)) {
	    TRACE("v");
	    return eval_symbol(interp, obj);
	}
    
	/* This should give us a means of executing 
	   primitives */
	if(is_tagged_list(interp, obj)) {
	    TRACE("p");
	    /* handle tail calls clean */
	    obj=eval_tagged_list(interp, obj);
	
	    /* If this was not a tail call, 
	       return the result */
	    if(!interp->tail) {
		interp->tail=0;
		return obj;
	    }
	} else {
	    /* we don't know how to evaluate this object */
	    TRACE("?");

	    /* If we make it here the expression wasn't
	       on we could evaluate. */
	    interp->error=1;
	    return interp->boolean.false;
	}

    } while(interp->tail);

    return obj;
}
