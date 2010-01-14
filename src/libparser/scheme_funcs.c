
#include "core.h"
#include "scheme_funcs.h"


/* Create a new tuple object with a
   given car and cdr */
object_type *cons(interp_core_type *interp, object_type *car,
		  object_type *cdr) {
    object_type *tuple=0;
    
    tuple=alloc_object(interp, TUPLE);
    car(tuple)=car;
    cdr(tuple)=cdr;
    
    return tuple;
}

/* Applies the quote symbol to an object */
object_type *quote(interp_core_type *interp, 
		   object_type *obj) {

    object_type *ret_val=0;
    
    /* Create a (quote ...) list */
    /* and now (quote ( ... )) */

    ret_val=cons(interp, interp->quote,
		 cons(interp, obj, 0));
    
    return ret_val;
}

/* Bind a symbol to a given value */
void bind_symbol(interp_core_type *interp, object_type *sym,
		 object_type *value) {
    object_type *binding=0;
    
    /* Check to see if a binding exists, 
       and replace it if it does */
    binding=get_binding(interp, sym);
    
    if(binding) {
	cdr(binding)=value;
	return;
    }

    /* create a new binding as we didn't find one */
    binding=cons(interp, sym, value);
    
    /* add our new binding to the list of bindings */
    car(interp->env_stack)=cons(interp, binding, 
			  car(interp->env_stack));
}

object_type *get_binding(interp_core_type *interp, 
			 object_type *sym) {
    object_type *binding=0;
    object_type *env=0;
    object_type *list=0;

    env=interp->env_stack;

    /* Walk each environment */
    while(env) {
	list=car(env);
	
	/* walk every binding */
	while(list) {
	    binding=car(list);
	    
	    /* we found the binding! */
	    if(car(binding)==sym) {
		return binding;
	    }
	    
	    list=cdr(list);
	}
	/* move to the next environment */
	env=cdr(env);
    }

    return 0;
}


/* Is this object self evaluating */
bool is_self_evaluating(interp_core_type *interp, object_type *obj) {
    object_type_enum type=0;
    type=obj->type;
    
    return type==FIXNUM || type==FLOATNUM 
	|| type==CHAR || type==BOOL
	|| type==STRING;
}

/* Is the object a quoted list? */
bool is_quoted(interp_core_type *interp,object_type *obj) {
    
    return obj!=0 && obj->type==TUPLE 
	&& car(obj)==interp->quote;
}

/* Is this list a procedure call */
bool is_tagged_list(interp_core_type *interp, 
		       object_type *obj) {
    return obj!=0 && obj->type==TUPLE
	&& car(obj)!=0 && car(obj)->type==SYM;
}

/* Is this is a primitive? */
bool is_primitive(interp_core_type *interp,
		  object_type *obj) {
    return obj!=0 && obj->type==PRIM;
}

/* Is this object a symbol */
bool is_symbol(interp_core_type *interp, object_type *obj) {
    return obj!=0 && obj->type==SYM;
}


/* Primitives */

/* define */
object_type *prim_define(interp_core_type *interp, object_type *args) {
    object_type *var=0;

    /* make sure we have the correct arguments */
    if(car(args)==0 || cdr(args)==0) {
	return interp->boolean.false;
    }

    /* You can bind a symbol or a 
       special list */
    if(!is_symbol(interp, car(args)) &&
       !is_tagged_list(interp, car(args))) {
	return interp->boolean.false;
    }

    var=car(args);

    bind_symbol(interp, var, eval(interp, cdar(args)));

    output(interp, interp->env_stack);

    return interp->boolean.true;
}

/* quit */
object_type *prim_quit(interp_core_type *interp, 
			 object_type *args) {
    interp->running=0;
    return 0;
}
