#include "core.h"
#include "util.h"
#include "scheme_funcs.h"


/* Evaluate every object in a list of objects */
object_type *eval_list(interp_core_type *interp, object_type *obj) {
    object_type *args=0;
    object_type *next=0;
    object_type *evaled_args=0;

    args=obj;

    /* Walk and evaluate argument list 
       and maintain the same order */
    while(args) {
	object_type *evaled=
	    cons(interp, eval(interp, car(args)),0);

	/* Deal with the first element */
	if(evaled_args==0) {
	    next=evaled_args=evaled;
	} else {
	    cdr(next)=evaled;
	    next=evaled;
	}

	args=cdr(args);
    }
    return evaled_args;
}

object_type *eval(interp_core_type *interp, object_type *obj) {
    object_type *binding=0;
    
    TRACE("E");

    /* Check for the empty list */
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
	binding=get_binding(interp, obj);

	/* Check the binding */
	if(binding==0) {
	    /* Unbound variable */
	    interp->error=1;
	    return 0;
      	}
	return cdr(binding);
    }
    
    /* This should be done in a function 
       bound to the quote symbol rather than 
       here. */
    if(is_quoted(interp, obj)) {
	TRACE("q");
	return cdar(obj);
    }

    /* This should give us a means of executing 
       primitives */
    if(is_tagged_list(interp, obj)) {
	object_type *evaled_args=0;


	TRACE("p");

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

	/* Evaluate each argument and pass the
	   resulting list to the function */
        //evaled_args=eval_list(interp, cdr(obj));
	    
	    
	
	TRACE("?");
    }

    /* If we make it here the expression wasn't
       on we could evaluate. */
    interp->error=1;

    return 0;
}
