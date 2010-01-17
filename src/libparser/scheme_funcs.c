#include <stdlib.h>

#include "core.h"
#include "scheme_funcs.h"
#include "util.h"

#define false interp->boolean.false
#define true interp->boolean.true

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

/* create an instance of a primitive object */
object_type *create_primitive(interp_core_type *interp, fn_type primitive,
			      bool eval_first) {
    object_type *obj=0;

    obj=alloc_object(interp, PRIM);
    obj->value.primitive.fn=primitive;
    obj->value.primitive.eval_first=eval_first;
    
    return obj;
}

/* Bind a symbol to a given value */
void bind_symbol(interp_core_type *interp, object_type *sym, object_type *value) {
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

void bind_symbol_list(interp_core_type *interp, binding_type *binding_list) {
    object_type *obj=0;
    int i=0;

    /* bind every symbol in the list of primitives */
    for(i=0; binding_list[i].symbol!=0;i++) {
	obj=create_symbol(interp, binding_list[i].symbol);
	bind_symbol(interp, obj, 
		    create_primitive(interp, binding_list[i].primitive, 
				     binding_list[i].eval_first));
    }
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

/* Is the interpreter currently in a runnable state? */
bool has_error(interp_core_type *interp) {
    return interp->error;
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

/* Check to see if the object represents truth */
bool is_true(interp_core_type *interp, object_type *obj) {
    /* anything other than false is true */
    return false!=obj;
}

/* Primitives */

/* define */
object_type *prim_define(interp_core_type *interp, object_type *args) {
    object_type *var=0;

    /* make sure we have the correct arguments */
    if(list_length(args)!=2) {
	interp->error=1;
	return false;
    }

    /* You can bind a symbol or a 
       special list */
    if(!is_symbol(interp, car(args)) &&
       !is_tagged_list(interp, car(args))) {
	interp->error=1;
	return false;
    }

    var=car(args);

    bind_symbol(interp, var, eval(interp, cdar(args)));

    return true;
}

/* set! */
object_type *prim_set(interp_core_type *interp, object_type *args) {
    object_type *binding=0;

    /* make sure we have the correct arguments */
    if(list_length(args)!=2) {
	return false;
    }

    /* You can set the value of a symbol */
    if(!is_symbol(interp, car(args))) {
	return false;
    }

    binding=get_binding(interp, car(args));
    
    if(binding==0) {
	interp->error=1;
	return false;
    }
    
    cdr(binding)=eval(interp, cdar(args));
   
    return true;
}

/* quit */
object_type *prim_quit(interp_core_type *interp, object_type *args) {
    cleanup_interp(interp);
    exit(0);
    return 0;
}

/* quote */
object_type *prim_quote(interp_core_type *interp, object_type *args) {
    /* quote just says, don't evaluate my arguments */
    if(list_length(args)<1) {
	interp->error=1;
	return false;
    }

    return car(args);
}

/* An if then constrcut */
object_type *prim_if(interp_core_type *interp, object_type *args) {
    object_type *predicate=0;
    int arg_count=0;

    /* make sure we have a predicate and at least
       a then clause */
    arg_count=list_length(args);
    if(arg_count<2 || arg_count>3) {
	interp->error=1;
	return false;
    }
    
    /* evaluate the predicate */
    predicate=eval(interp, car(args));

    /* setup a tail call */
    set_tail(interp);
    
    if(is_true(interp, predicate)) {
	return cdar(args);
    } else {
	if(arg_count==2) {
	    return false;
	} else {
	    return cddar(args);
	}
    }
}

/* Converts a fixnum into a floatnum */
void fixnum_to_floatnum(interp_core_type *interp, object_type **num) {
    object_type  *new_float=0;

    /* We don't need to change anything here */
    if((*num)->type==FLOATNUM) {
	return;
    }
    
    /* convert our int to a float */
    new_float=alloc_object(interp, FLOATNUM);
    new_float->value.float_val=(*num)->value.int_val;
    (*num)=new_float;
}

/* Convert numbers into a mutually acceptable representation */
void normalize_numbers(interp_core_type *interp, object_type **num, 
		       object_type **num2) {

    if((*num)->type!=(*num2)->type) {
	fixnum_to_floatnum(interp, num);
	fixnum_to_floatnum(interp, num);
    }
}

/* Math in Macros */
#define OPERATION(oper, name)						\
    object_type *name(interp_core_type *interp, object_type *args) {	\
	object_type *result=0;						\
	object_type *operand=0;						\
									\
	int arg_count=0;						\
									\
									\
	/* No argument means we return 0 */				\
	if(args==0) {							\
	    result=alloc_object(interp, FIXNUM);			\
	    result->value.int_val=0;					\
	    return result;						\
	}								\
									\
	/* clone the first argument into a new number for our result */	\
	result=clone(interp, car(args));				\
	args=cdr(args);							\
									\
	/* walk argument list and evaluate each one */			\
	while(args!=0) {						\
	    operand=car(args);						\
									\
	    /* Make sure that everything is the same kind of number */	\
	    normalize_numbers(interp, &result, &operand);		\
									\
									\
	    /* Make sure to operate on the right field */		\
	    switch(result->type) {					\
	    case FIXNUM:						\
		result->value.int_val oper operand->value.int_val;	\
		break;							\
	    case FLOATNUM:						\
		result->value.float_val oper operand->value.float_val;	\
		break;							\
	    }								\
									\
	    args=cdr(args);						\
	}								\
									\
	return result;							\
    }									\

/* Four basic math operations */
OPERATION(+=, prim_plus)
OPERATION(-=, prim_minus)
OPERATION(*=, prim_multi)
OPERATION(/=, prim_div)

#define TEST(test, name)						\
    object_type *name(interp_core_type *interp, object_type *args) {	\
									\
	/* accept one argument and only one argument */			\
	if(list_length(args)!=1) {					\
	    interp->error=1;						\
	    return 0;							\
	}								\
									\
	return test ? true : false;					\
    }									\


TEST(car(args)==0, prim_is_null)
TEST(car(args)!=0 && car(args)->type==BOOL, prim_is_boolean)
TEST(car(args)!=0 && car(args)->type==SYM, prim_is_symbol)
TEST(car(args)!=0 && car(args)->type==FIXNUM, prim_is_integer)
TEST(car(args)!=0 && car(args)->type==CHAR, prim_is_char)
TEST(car(args)!=0 && car(args)->type==STRING, prim_is_string)
TEST(car(args)!=0 && car(args)->type==TUPLE, prim_is_tuple)
TEST(car(args)!=0 && car(args)->type==PRIM, prim_is_prim)


/* Setup scheme primitive function bindings */
binding_type primitive_list[]={
    {"define", &prim_define, 0},
    {"set!", &prim_set, 0},
    {"quit", &prim_quit, 0},
    {"quote", &prim_quote, 0},
    {"if", &prim_if, 0},

    {"+", &prim_plus, 1},
    {"-", &prim_minus, 1},
    {"*", &prim_multi, 1},
    {"/", &prim_div, 1},

    {"null?", &prim_is_null, 1},
    {"boolean?", &prim_is_boolean, 1},
    {"symbol?", &prim_is_symbol, 1},
    {"integer?", &prim_is_integer, 1},
    {"char?", &prim_is_char, 1},
    {"string?", &prim_is_string, 1},
    {"pair?", &prim_is_tuple, 1},
    {"procedure?", &prim_is_prim, 1},

    {0,0} /* Terminate the list */
};
