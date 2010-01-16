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

/* Walk a list of values and return an object result */
object_type *eval_list(interp_core_type *interp, object_type *args) {
    object_type *next=0;
    object_type *evaled_args=0;
    object_type *next_val=0;
    object_type *evaled=0;
    
    next=args;
    
    while(next!=0) {
	evaled=eval(interp, car(next));
	
	/* first value has to be treated differently */
	if(evaled_args==0) {
	    next_val=evaled_args=cons(interp, evaled, 0);
	} else {
	    /* build a list of the evaled arguments keeping 
	       the same order */
	    cdr(next_val)=cons(interp, evaled, 0);
	    next_val=cdr(next_val);
	}
	
	next=cdr(next);
    }

    return evaled_args;
}


/* evaluate a tagged list */
object_type *eval_tagged_list(interp_core_type *interp, object_type *obj) {
    object_type *evaled_args=0;
    object_type *binding=0;
    object_type *primitive=0;

    binding=get_binding(interp, car(obj));
	
    /* If the symbol is unbound, 
       we have an error */
    if(binding==0) {
	TRACE("e");
	interp->error=1;
	return 0;
    }

    primitive=cdr(binding);

    /* do we evaluate the arguments before passing them? */
    if(primitive->value.primitive.eval_first) {
	evaled_args=eval_list(interp, cdr(obj));
    } else {
	evaled_args=cdr(obj);
    }

    /* Make sure that the procedure is callable */
    if(is_primitive(interp, cdr(binding))) {
	TRACE("Pi");
	/* Call the primitive and return, make sure to skip the
	   symbol ref at the start of the list */
	return (*(primitive->value.primitive.fn))(interp, evaled_args);
    }
}

/* Main entry point to evaluator */
object_type *eval(interp_core_type *interp, object_type *obj) {
    
    /* make sure that the interpreter is alive */
    if(has_error(interp)) {
	return 0;
    }

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
