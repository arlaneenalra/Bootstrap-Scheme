#include "core.h"
#include "util.h"
#include "scheme_funcs.h"


object_type *eval(interp_core_type *interp, object_type *obj) {
    
    TRACE("E");

    /* Check for the empty list */
    if(obj==0) {
	return 0;
    }

    /* Check for self evaluating */
    if(is_self_evaluating(obj)) {
	TRACE("s");
	return obj;
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
    if(is_procedure_call(interp, obj)) {
	object_type *binding=0;
	object_type *args=0;
	object_type *evaled_args=0;
	object_type *next=0;

	TRACE("p");

	binding=get_binding(interp, car(obj));
	
	/* If the symbol is unbound, 
	   we have an error */
	if(binding==0) {
	    TRACE("e");
	    interp->error=1;
	    return 0;
	}

	/* Evaluate each argument and pass the
	   resulting list to the function */
	
	args=cdr(obj);
	
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

	/* Make sure that the procedure is callable */
	if(is_primitive(interp, cdr(binding))) {
	    TRACE("Pi");
	    /* call the primitive and return */
	    return (*(cdr(binding)->value.primitive))(interp, evaled_args);
	}
	TRACE("?");
    }

    /* If we make it here the expression wasn't
       on we could evaluate. */
    interp->error=1;

    return 0;
}
