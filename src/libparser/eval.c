#include "core.h"
#include "util.h"
#include "scheme_funcs.h"


/* The returned object is a tail call and not a terminal one */
void set_tail(interp_core_type *interp) {
    interp->tail=1;
}

/* returned object is a terminal one */
void clear_tail(interp_core_type *interp) {
    interp->tail=0;
}

/* Are we in a tail call chain? */
bool is_tail(interp_core_type *interp) {
    return interp->tail;
}

/* Evaluate a symbol */
object_type *eval_symbol(interp_core_type *interp, object_type *obj) {
    object_type *binding=0;

    binding=get_binding(interp, obj);

    /* Check the binding */
    if(binding==0) {
	printf("\nUnound Variable: %s@%p\n", obj->value.symbol.name, obj);
	printf("env: %p=", interp->cur_env);
	output(interp, interp->cur_env);
	printf("\n");
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
    
    evaled_args=interp->empty_list;
    next=args;
    
    while(!is_empty_list(interp, next)) {
	evaled=eval(interp, car(next));
	
	if(interp->error) {
	    return 0;
	}
	
	/* first value has to be treated differently */
	if(is_empty_list(interp, evaled_args)) {
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


/* evaluate a tagged list */
object_type *eval_tagged_list(interp_core_type *interp, object_type *proc, 
			      object_type *obj) {
    object_type *evaled_args=0;
    object_type *body=0;
    object_type *result=0;

    /* Make sure that the procedure is callable */
    if(is_primitive(interp, proc)) {
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
	result=(*(proc->value.primitive.fn))(interp, evaled_args);
	
    } else {
	TRACE("Co");

	/* always evaluate arguments of compound procecdures */
	evaled_args=eval_list(interp, cdr(obj));

	push_environment(interp, proc->value.closure.env); /* enter the procedure */

	bind_argument_list(interp, proc->value.closure.param, evaled_args);

	body=proc->value.closure.body;
	
	/* loop until we have the last call */
	while(!is_empty_list(interp, cdr(body))) {
	    TRACE("Cb");
	    eval(interp, car(body));
	    body=cdr(body);
	}

	body=car(body);
	
	/* if we don't have a tuple here, we cannot have
	   a tail call */
	if(is_tuple(interp, body) && !is_empty_list(interp, body)) {
	    set_tail(interp);
	    result=body;	
	} else {
	    clear_tail(interp);
	    result=eval(interp, body);
	    pop_environment(interp);
	}	
    }

    TRACE("Ret");
    return result;
}

/* Main entry point to evaluator */
object_type *eval(interp_core_type *interp, object_type *obj) {
    object_type *proc=0;
    
    /* make sure that the interpreter is alive */
    if(has_error(interp)) {
	return 0;
    }

    while(!interp->error) {

	TRACE("E");
    
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
	if(is_tuple(interp, obj)) {

	    TRACE("p");
	    
	    /* Evaluate our object to see what we have */
	    proc=eval(interp, car(obj));
	    
	    if(is_primitive(interp, proc)) {
	
		obj=eval_tagged_list(interp, proc, obj);
		
		if(proc->value.primitive.eval_end) {
		    return obj;
		}
	    } else {	    
		obj=eval_tagged_list(interp, proc, obj);
	    }
	} else {
	    /* we don't know how to evaluate this object */
	    TRACE("?");

	    /* If we make it here the expression wasn't
	       on we could evaluate. */
	    interp->error=1;
	    return interp->boolean.false;
	}

    }

    printf("WTF!\n");
    output(interp, obj);
    return obj;
}
