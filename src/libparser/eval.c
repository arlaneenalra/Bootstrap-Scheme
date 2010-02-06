#include "core.h"
#include "util.h"
#include "scheme_funcs.h"
#include "parser_internal.h"

/* Evaluate a symbol */
object_type *eval_symbol(interp_core_type *interp, object_type *obj) {
    object_type *binding=0;

    binding=get_binding(interp, obj);

    /* Check the binding */
    if(binding==0) {
	printf("\nUnound Variable: %s@%p\n", obj->value.string_val, obj);

	/* Unbound variable */
	interp->error=1;
	return false;
    }
    return cdr(binding);
}

/* Walk a list of values and return an object result */
object_type *eval_list(interp_core_type *interp, object_type *args) {
    object_type *next=0;
    object_type *evaled_args=0;
    object_type *next_val=0;
    object_type *evaled=0;
    
    evaled_args=interp->empty_list;
    next=args;
    
    while(!is_empty_list(interp, next)) {
	evaled=eval(interp, car(next));
	
	if(interp->error) {
	    return 0;
	}
	
	/* first value has to be treated differently */
	if(is_empty_list(interp, evaled_args)) {
	    /* make sure we have a record of the first value */
	    next_val=evaled_args=cons(interp, evaled, interp->empty_list);
	} else {
	    /* build a list of the evaled arguments keeping 
	       the same order */
	    cdr(next_val)=cons(interp, evaled, interp->empty_list);
	    next_val=cdr(next_val);
	}
	
	next=cdr(next);
    }

    return evaled_args;
}

/* evaluate primitive functions */
object_type *eval_prim(interp_core_type *interp, object_type *proc, 
		       object_type *obj) {
    object_type *evaled_args=0;

    TRACE("Pi");

    /* do we evaluate the arguments before passing them? */
    if(proc->value.primitive.eval_first) {
	evaled_args=eval_list(interp, cdr(obj));

	/* propagate errors */
	if(interp->error) {
	    return 0;
	}
    } else {
	evaled_args=cdr(obj);
    }

    TRACE("Cpi");
	
    /* Call the primitive and return, make sure to skip the
       symbol ref at the start of the list */
    return (*(proc->value.primitive.fn))(interp, evaled_args);
}

/* evaluate a tagged list */
object_type *eval_tagged_list(interp_core_type *interp, object_type *proc, 
			      object_type *obj) {

    object_type *body=0;

    TRACE("Co");

    body=proc->value.closure.body;
	
    /* loop until we have the last call */
    while(!is_empty_list(interp, cdr(body))) {
	TRACE("Cb");
	eval(interp, car(body));
	body=cdr(body);
    }

    body=car(body);
	
    return body;
}

/* Main entry point to evaluator */
object_type *eval(interp_core_type *interp, object_type *obj) {
    object_type *proc=0;
    object_type *evaled_args=0;
    object_type *cur_env=0;

    bool loop=1;

    cur_env=interp->cur_env;
    
    /* make sure that the interpreter is alive */
    if(has_error(interp)) {
	return 0;
    }

    while(!has_error(interp) && loop) {
	TRACE("E");
    
	/* Check for self evaluating */
	if(is_self_evaluating(interp, obj)) {

	    TRACE("s");
	    loop=0;
	    
	} else if(is_symbol(interp, obj)) { 	/* Check for a symbol */

	    TRACE("v");

	    obj=eval_symbol(interp, obj);
	    loop=0;

	} else if(is_tuple(interp, obj)) { 	/* This should give us a means 
						   of executing primitives */
	    TRACE("p");

	    /* Evaluate our object to see what we have */
	    proc=eval(interp, car(obj));

	    /* make sure that we actually have a proc to work with */
	    if(has_error(interp) || proc==0) {
		interp->error=1;
		loop=0;
		obj=false;

	    } else {
		
		if(is_primitive(interp, proc)) {
		    /* We have a primitive */
		    obj=eval_prim(interp, proc, obj);
		
		    /* Do we need to evaluate the result? */
		    if(proc->value.primitive.eval_end) {
			loop=0;
		    }

		} else if(is_closure(interp, proc)) {
		    /* always evaluate arguments of compound procecdures */
		    evaled_args=eval_list(interp, cdr(obj));

		    /* check for error evaluating arguments */
		    if(has_error(interp)) {
			printf("\nfailed evaluating:");
			output(interp, obj);
			printf("\nwith args:");
			output(interp, cdr(obj));
			printf("\n");
		    
			return false;
		    }
		    
		    push_environment(interp, proc->value.closure.env);

		    bind_argument_list(interp, proc->value.closure.param, evaled_args);

		    obj=eval_tagged_list(interp, proc, obj);

		    /* if we don't have a tuple here, we cannot have
		       a tail call */
		    if(!is_tuple(interp, obj) || is_empty_list(interp, obj)) {
			obj=eval(interp, obj);
			loop=0;
		    }
		} else {
		    interp->error=1;
		    return false;
		}
	    }

	    
	} else {
	    /* we don't know how to evaluate this object */
	    TRACE("?");

	    /* If we make it here the expression wasn't
	       on we could evaluate. */
	    interp->error=1;
	    return false;
	}
    }

    interp->cur_env=cur_env;

    return obj;
}
