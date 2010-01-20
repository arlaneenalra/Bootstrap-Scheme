#include <stdlib.h>

#include "core.h"
#include "scheme_funcs.h"
#include "util.h"
#include "gc.h"

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
object_type *quote(interp_core_type *interp, object_type *obj) {

    object_type *ret_val=0;
    
    /* Create a (quote ...) list */
    /* and now (quote ( ... )) */

    ret_val=cons(interp, interp->quote,
		 cons(interp, obj, interp->empty_list));
    
    return ret_val;
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
	fixnum_to_floatnum(interp, num2);
    }
}

/* create an instance of a primitive object */
object_type *create_primitive(interp_core_type *interp, fn_type primitive,
			      bool eval_first, bool eval_end) {
    object_type *obj=0;

    obj=alloc_object(interp, PRIM);
    obj->value.primitive.fn=primitive;
    obj->value.primitive.eval_first=eval_first;
    obj->value.primitive.eval_end=eval_end;
    
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
    car(interp->cur_env)=cons(interp, binding, 
			  car(interp->cur_env));
}

/* bind a parallel list of symbols and arguments */
void bind_argument_list(interp_core_type *interp, object_type *sym_list, 
			object_type *value_list) {

    object_type *binding=0;

    while(!is_empty_list(interp, sym_list)) {

	/* create a new binding as we didn't find one */
	binding=cons(interp, car(sym_list), car(value_list));
    
	/* add our new binding to the list of bindings */
	car(interp->cur_env)=cons(interp, binding, 
				  car(interp->cur_env));

	sym_list=cdr(sym_list);
	value_list=cdr(value_list);
    }
}

void bind_symbol_list(interp_core_type *interp, binding_type *binding_list) {
    object_type *obj=0;
    int i=0;

    /* bind every symbol in the list of primitives */
    for(i=0; binding_list[i].symbol!=0;i++) {
	obj=create_symbol(interp, binding_list[i].symbol);
	bind_symbol(interp, obj, 
		    create_primitive(interp, binding_list[i].primitive, 
				     binding_list[i].eval_first, 
				     binding_list[i].eval_end));
    }
}

object_type *get_binding(interp_core_type *interp, 
			 object_type *sym) {
    object_type *binding=0;
    object_type *env=0;
    object_type *list=0;

    env=interp->cur_env;

    /* Walk each environment */
    while(!is_empty_list(interp, env)) {
	list=car(env);
	
	/* walk every binding */
	while(!is_empty_list(interp, list)) {
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

/* Is this the empty list? */
bool is_empty_list(interp_core_type *interp,object_type *obj) {
    return obj==interp->empty_list;
}

/* Does this list start with a symbol? */
bool is_tagged_list(interp_core_type *interp, object_type *obj) {
    return obj!=0 && obj->type==TUPLE
	&& car(obj)!=0 && car(obj)->type==SYM;
}

/* Is this object a tuple? */
bool is_tuple(interp_core_type *interp, object_type *obj) {
    return obj!=0 && obj->type==TUPLE;
}

/* Is this is a primitive? */
bool is_primitive(interp_core_type *interp, object_type *obj) {
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

/* Check to see if the object represents a closure */
bool is_closure(interp_core_type *interp, object_type *obj) {
    /* anything other than false is true */
    return obj!=0 && obj->type==CLOSURE;
}


/* Primitives */

/* car */
object_type *prim_car(interp_core_type *interp, object_type *args) {
    if(args==0 || car(args)==0 || car(args)->type != TUPLE) {
	interp->error=1;
	return false;
    }

    return caar(args);
}

/* set-car! */
object_type *prim_set_car(interp_core_type *interp, object_type *args) {

    /* make sure we have values to work with */
    if(list_length(interp, args)!=2 && car(args)->type != TUPLE) {
	interp->error=1;
	return false;
    }

    caar(args)=cadr(args);
    return true;
}

/* cdr */
object_type *prim_cdr(interp_core_type *interp, object_type *args) {
    if(args==0 || car(args)==0 || car(args)->type != TUPLE) {
	interp->error=1;
	return false;
    }

    return cdar(args);
}

/* set-cdr! */
object_type *prim_set_cdr(interp_core_type *interp, object_type *args) {

    /* make sure we have values to work with */
    if(list_length(interp, args)!=2 && car(args)->type != TUPLE) {
	interp->error=1;
	return false;
    }

    cadr(args)=cadr(args);
    return true;
}


/* cons */
object_type *prim_cons(interp_core_type *interp, object_type *args) {
    object_type *obj=0;

    /* make sure we have the correct arguments */
    if(list_length(interp, args)!=2) {
	interp->error=1;
	return false;
    }
    
    obj=cons(interp, car(args), cadr(args));
    
    return obj;
}

/* lambda */
object_type *prim_lambda(interp_core_type *interp, object_type *args) {
    object_type *obj=0;
    
    obj=alloc_object(interp, CLOSURE);
    
    obj->value.closure.env=interp->cur_env;
    obj->value.closure.param=car(args);
    obj->value.closure.body=cdr(args);

    return obj;
}

/* begin */
object_type *prim_begin(interp_core_type *interp, object_type *args) {
    
    /* If we don't have any arguments, return an 
       empty list */
    if(is_empty_list(interp, args)) {
	return quote(interp, interp->empty_list);
    }
    
    /* evalueate each argument */
    while(!is_empty_list(interp, cdr(args))) {
	eval(interp, car(args));
	args=cdr(args);
    }
    
    /* return the last argument */
    return car(args);
}

/* define */
object_type *prim_define(interp_core_type *interp, object_type *args) {

    /* make sure we have the correct arguments */
    if(list_length(interp, args)<2) {
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

    if(is_symbol(interp, car(args))) {
	bind_symbol(interp, car(args), /* Symbol */
		    eval(interp, cadr(args))); /* Value */
    } else {
	/* It can only be a tagged list then */

	bind_symbol(interp, caar(args), /* Symbol */
		    prim_lambda(interp, 
				cons(interp, cdar(args), /* Arguments */
				     cdr(args)))); /* Body */
    }

    return true;
}

/* set! */
object_type *prim_set(interp_core_type *interp, object_type *args) {
    object_type *binding=0;

    /* make sure we have the correct arguments */
    if(list_length(interp, args)!=2) {
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
    
    cdr(binding)=eval(interp, cadr(args));
   
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
    if(list_length(interp, args)<1) {
	interp->error=1;
	return false;
    }

    return car(args);
}

/* An cond then constrcut */
object_type *prim_cond(interp_core_type *interp, object_type *args) {
    object_type *result=false;
    object_type *test=0;

    /* We need to have some arguments */
    if(is_empty_list(interp, args)) {
	return false;
    }

    while(!is_empty_list(interp, args)) {
	/* arguments should be in the form of a 
	   list of lists */

	if(!is_tuple(interp, car(args))) {
	    interp->error=1;
	    return false;
	}

    	test=caar(args);
	
	/* evaluate the test expression */
	result=eval(interp, test);
	
	/* check for an interpreter error */
	if(interp->error) {
	    return false;
	}

	if(result!=false) {
	    return cadar(args);
	}	
	args=cdr(args);
    }

    return false;
}

/* An if then constrcut */
object_type *prim_if(interp_core_type *interp, object_type *args) {
    object_type *predicate=0;
    object_type *result=0;
    int arg_count=0;

    /* make sure we have a predicate and at least
       a then clause */
    arg_count=list_length(interp, args);
    if(arg_count<2 || arg_count>3) {
	interp->error=1;
	return false;
    }
    
    /* evaluate the predicate */
    predicate=eval(interp, car(args));

    if(is_true(interp, predicate)) {
	result=cadr(args);
    } else {
	if(arg_count==2) {
	    result=false;
	} else {
	    result=caddr(args);
	}
    }

    return result;
}

/* Symbol to String */
object_type *prim_sym_to_string(interp_core_type *interp, object_type *args) {
    object_type *obj=0;
    char *name=0;
    
    /* make sure we have enough arguments */
    if(list_length(interp, args)!=1) {
	interp->error=1;
	return false;
    }
    
    if(car(args)==0 || car(args)->type!=SYM) {
	interp->error=1;
	return false;	
    }

    obj=create_string(interp, car(args)->value.symbol.name);
    
    return obj;
}

/* String to Symbol */
object_type *prim_string_to_sym(interp_core_type *interp, object_type *args) {
    object_type *obj=0;
    char *name=0;
    
    /* make sure we have enough arguments */
    if(list_length(interp, args)!=1) {
	interp->error=1;
	return false;
    }
    
    if(car(args)==0 || car(args)->type!=STRING) {
	interp->error=1;
	return false;	
    }

    obj=create_symbol(interp, car(args)->value.string_val);
    
    return obj;
}

/* Character to Integer */
object_type *prim_char_to_int(interp_core_type *interp, object_type *args) {
    object_type *obj=0;
    
    /* make sure we have enough arguments */
    if(list_length(interp, args)!=1) {
	interp->error=1;
	return false;
    }
    
    if(car(args)==0 || car(args)->type!=CHAR) {
	interp->error=1;
	return false;	
    }

    /* convert our char to a string */
    obj=alloc_object(interp, FIXNUM);
    obj->value.int_val=car(args)->value.char_val;
    
    return obj;
}


/* Integer to Character */
object_type *prim_int_to_char(interp_core_type *interp, object_type *args) {
    object_type *obj=0;
    
    /* make sure we have enough arguments */
    if(list_length(interp, args)!=1) {
	interp->error=1;
	return false;
    }
    
    if(car(args)==0 || car(args)->type!=FIXNUM) {
	interp->error=1;
	return false;	
    }

    /* convert our char to a string */
    obj=alloc_object(interp, CHAR);
    obj->value.char_val=car(args)->value.int_val;
    
    return obj;
}

/* Number to String */
object_type *prim_num_to_string(interp_core_type *interp, object_type *args) {
    object_type *obj=0;
    char buf[2048];
    
    /* make sure we have enough arguments */
    if(list_length(interp, args)!=1) {
	interp->error=1;
	return false;
    }
    
    obj=car(args);

    switch(obj->type) {
    case FIXNUM:
	sprintf(buf, "%" PRIi64, obj->value.int_val);
	break;

    case FLOATNUM:
	sprintf(buf, "%Lg", obj->value.float_val);
	break;

    default:
	interp->error=1;
	return false;
    }
    
    return create_string(interp, buf);
}


/* String to Number */
object_type *prim_string_to_num(interp_core_type *interp, object_type *args) {
    object_type *obj=0;
    char *str=0;
    char *walk=0;
    
    /* make sure we have enough arguments */
    if(list_length(interp, args)!=1) {
	interp->error=1;
	return false;
    }
    
    if(car(args)==0 || car(args)->type!=STRING) {
	interp->error=1;
	return false;	
    }
    
    walk=str=car(args)->value.string_val;

    /* check for an initial negative */
    if(*walk=='-') {
	walk++;
    }
    
    /* assume we have an integer until otherwise determined */
    obj=alloc_object(interp, FIXNUM);

    /*scan the string and determine if we have an integer or a float */
    while(walk!=0 && (*walk)!=0) {
	
	if(*walk=='.') {
	    obj->type=FLOATNUM;
	    walk=0;
	} else if(!isdigit(*walk)) {
	    walk=0;
	}else {
	    walk++;
	}
    }

    /* do the actual conversion */
    switch(obj->type) {
    case FIXNUM:
	obj->value.int_val=strtoll(str,0,10);
	break;
    case FLOATNUM:
	obj->value.float_val=strtold(str, 0);
	break;
    }
    

    return obj;
}


/* Calculate the remainder of a division */
object_type *prim_mod(interp_core_type *interp, object_type *args) {
    object_type *dividend=0;
    object_type *divisor=0;
    object_type *result=0;

    /* make sure we have the correct arguments */
    if(list_length(interp, args)!=2) {
	interp->error=1;
	return false;
    }

    dividend=car(args);
    divisor=cadr(args);

    /* You can set the value of a symbol */
    if(dividend==0 || divisor==0 ||
       dividend->type!=FIXNUM || divisor->type !=FIXNUM) {
	interp->error=1;
	return false;
    }

    result=clone(interp, dividend);
    result->value.int_val%=divisor->value.int_val;

    return result;
}

/* Calculate the quotient of a division */
object_type *prim_div_int(interp_core_type *interp, object_type *args) {
    object_type *dividend=0;
    object_type *divisor=0;
    object_type *result=0;

    /* make sure we have the correct arguments */
    if(list_length(interp, args)!=2) {
	interp->error=1;
	return false;
    }

    dividend=car(args);
    divisor=cadr(args);

    /* You can set the value of a symbol */
    if(dividend==0 || divisor==0 ||
       dividend->type!=FIXNUM || divisor->type !=FIXNUM) {
	interp->error=1;
	return false;
    }

    result=clone(interp, dividend);
    result->value.int_val/=divisor->value.int_val;

    return result;
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
	if(is_empty_list(interp, args)) {				\
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
	while(!is_empty_list(interp, args)) {				\
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
	if(list_length(interp, args)!=1) {				\
	    interp->error=1;						\
	    return 0;							\
	}								\
									\
	return test ? true : false;					\
    }			 						\


TEST(car(args)==0, prim_is_null)
TEST(car(args)!=0 && car(args)->type==BOOL, prim_is_boolean)
TEST(car(args)!=0 && car(args)->type==SYM, prim_is_symbol)
TEST(car(args)!=0 && car(args)->type==FIXNUM, prim_is_integer)
TEST(car(args)!=0 && car(args)->type==CHAR, prim_is_char)
TEST(car(args)!=0 && car(args)->type==STRING, prim_is_string)
TEST(car(args)!=0 && car(args)->type==TUPLE, prim_is_tuple)
TEST(car(args)!=0 && car(args)->type==PRIM, prim_is_prim)

#define NUMERIC_TEST(test, name)					\
    object_type *name(interp_core_type *interp, object_type *args) {	\
	object_type *first=0;						\
	object_type *next=0;						\
	/* Make sure we have at least 2 arguments */			\
	if(list_length(interp, args)<2) {				\
	    interp->error=1;						\
	    return false;						\
	}								\
	first=car(args);						\
									\
	args=cdr(args);							\
									\
	while(!is_empty_list(interp, args)) {				\
	    next=car(args);						\
	    normalize_numbers(interp, &first, &next);			\
									\
	    switch(first->type) {					\
	    case FIXNUM:						\
		if(!(first->value.int_val test next->value.int_val)) {	\
		    return false;					\
		}							\
		break;							\
	    case FLOATNUM:						\
		if(!(first->value.float_val test next->value.float_val)) { \
		    return false;					\
		}							\
		break;							\
	    }								\
	    first=next;							\
	    args=cdr(args);						\
	}								\
									\
	/* default to true */						\
	return true;							\
    }									\
 
NUMERIC_TEST(==, prim_equal)
NUMERIC_TEST(<, prim_less)
NUMERIC_TEST(>, prim_greater)

object_type *prim_dump_env(interp_core_type *interp, object_type *args) {
    printf("\nenv: %p=", interp->cur_env);
    output(interp, interp->cur_env);
    printf("\n");
    return true;
}

/* Setup scheme primitive function bindings */
binding_type primitive_list[]={
    {"define", &prim_define, 0, 0},
    {"set!", &prim_set, 0, 0},
    {"quit", &prim_quit, 0, 0},
    {"quote", &prim_quote, 0, 1},
    {"if", &prim_if, 0, 0},
    {"cond", &prim_cond, 0, 0},
    {"lambda", &prim_lambda, 0, 1},
    {"begin", &prim_begin, 0, 0},

    {"cons", &prim_cons, 1, 1},
    {"car", &prim_car, 1, 0},
    {"cdr", &prim_cdr, 1, 0},
    {"set-car!", &prim_set_car, 1, 0},
    {"set-cdr!", &prim_set_cdr, 1, 0},

    {"+", &prim_plus, 1, 0},
    {"-", &prim_minus, 1, 0},
    {"*", &prim_multi, 1, 0},
    {"/", &prim_div, 1, 0},
    {"quotient", &prim_div_int, 1, 0},
    {"remainder", &prim_mod, 1, 0},

    {"=", &prim_equal, 1, 0},
    {"<", &prim_less, 1, 0},
    {">", &prim_greater, 1, 0},

    {"null?", &prim_is_null, 1, 0},
    {"boolean?", &prim_is_boolean, 1, 0},
    {"symbol?", &prim_is_symbol, 1, 0},
    {"integer?", &prim_is_integer, 1, 0},
    {"char?", &prim_is_char, 1, 0},
    {"string?", &prim_is_string, 1, 0},
    {"pair?", &prim_is_tuple, 1, 0},
    {"procedure?", &prim_is_prim, 1, 0},
    
    {"char->integer", &prim_char_to_int, 1, 0},
    {"integer->char", &prim_int_to_char, 1, 0},
    {"number->string", &prim_num_to_string, 1, 0},
    {"string->number", &prim_string_to_num, 1, 0},
    {"symbol->string", &prim_sym_to_string, 1, 0},
    {"string->symbol", &prim_string_to_sym, 1, 0},

    {"dump_env", &prim_dump_env, 1, 0},

    {0,0} /* Terminate the list */
};
