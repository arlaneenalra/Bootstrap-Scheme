#include "core.h"
#include "util.h"
#include "scheme_funcs.h"


/* The returned object is a tail call and not a terminal one */
void set_tail(interp_core_type *interp) {
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
	
	if(interp->error) {
	    return 0;
	}
	
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
    object_type *proc=0;
    object_type *body=0;

    /* binding=get_binding(interp, car(obj)); */
	
    /* /\* If the symbol is unbound,  */
    /*    we have an error *\/ */
    /* if(binding==0) { */
    /* 	TRACE("e"); */
    /* 	interp->error=1; */
    /* 	return 0; */
    /* } */

    /* proc=cdr(binding); */

    /* Evaluate our object to see what we have */
    proc=eval(interp, car(obj));



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
	printf("\n");
	output(interp, cdr(obj));
	printf("\n");
	output(interp, evaled_args);
	printf("\n");
	output(interp, proc);
	printf("\n");

	/* Call the primitive and return, make sure to skip the
	   symbol ref at the start of the list */
	return (*(proc->value.primitive.fn))(interp, evaled_args);
    } else {

	/* always evaluate arguments of compound procecdures */
	evaled_args=eval_list(interp, cdr(obj));

	push_environment(interp, proc->value.closure.env); /* enter the procedure */
	
	bind_argument_list(interp, proc->value.closure.param, evaled_args);

	body=proc->value.closure.body;
	
	/* loop until we have the last call */
	while(cdr(body)!=0) {
	    eval(interp, car(body));
	    body=cdr(body);
	}
	
	set_tail(interp);
	return car(body);	
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
	if(is_tuple(interp, obj)) {

	    TRACE("p");

	    /* handle tail calls clean */
	    obj=eval_tagged_list(interp, obj);
	
	    /* If this was not a tail call, 
	       return the result */
	    if(!interp->tail) {
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

    } while(interp->tail && !interp->error);

    return obj;
}
