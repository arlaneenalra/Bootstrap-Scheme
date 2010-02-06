#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "core.h"
#include "util.h"
#include "scheme_funcs.h"
#include "parser_internal.h"

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
void bind_symbol(interp_core_type *interp, object_type *sym, object_type *value,
		 object_type **env) {
    object_type *binding=0;
    
    /* create a new binding as we didn't find one */
    binding=cons(interp, sym, value);
    
    /* add our new binding to the list of bindings */
    car((*env))=cons(interp, binding, 
		     car((*env)));
}

/* bind a parallel list of symbols and arguments */
void bind_argument_list(interp_core_type *interp, object_type *sym_list, 
			object_type *value_list) {

    /* we have a list of symbols */
    while(!is_empty_list(interp, sym_list) && !is_empty_list(interp, value_list)
	  && !is_symbol(interp, sym_list)) {

	bind_symbol(interp, car(sym_list), car(value_list), &interp->cur_env);

	sym_list=cdr(sym_list);
	value_list=cdr(value_list);
    }

    /* handle single, variadic argument lists */
    if(is_symbol(interp, sym_list)) {
	bind_symbol(interp, sym_list, value_list, &interp->cur_env);
	return;
    }

    /* make sure that we have the same number of arguments
       as we have symbols */
    if(!is_empty_list(interp, sym_list) &&
       !is_empty_list(interp, value_list)) {
	interp->error=1;
    }
}

void bind_symbol_list(interp_core_type *interp, binding_type *binding_list,
		      object_type **env) {
    object_type *obj=0;
    int i=0;

    /* bind every symbol in the list of primitives */
    for(i=0; binding_list[i].symbol!=0;i++) {
	obj=create_symbol(interp, binding_list[i].symbol);
	bind_symbol(interp, obj, 
		    create_primitive(interp, binding_list[i].primitive, 
				     binding_list[i].eval_first, 
				     binding_list[i].eval_end), env);
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

    /* check for null */
    if(obj==0) {
	return 0;
    }

    type=obj->type;
    
    return type==FIXNUM || type==FLOATNUM 
	|| type==CHAR || type==BOOL
	|| type==STRING || type==PRIM
	|| type==CLOSURE;
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

    cdar(args)=cadr(args);
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

/* let */
object_type *prim_let(interp_core_type *interp, object_type *args) {
    object_type *next=0;
    object_type *body=0;
    object_type *arg_list=0;
    object_type *sym_list=0;

    /* make sure that we have at least 2 arguments */
    if(list_length(interp, args)<2) {
	interp->error=1;
	return false;
    }

    /* Split first argument into symbols and 
       values */
    arg_list=interp->empty_list;
    sym_list=interp->empty_list;
    
    /* let takes a list of pairs as it's first argument */
    next=car(args);
    
    /* make sure we have a tuple */
    if(!is_tuple(interp, next)) {
	interp->error=1;
	return false;
    }

    /* split pairs into parallel lists */
    while(!is_empty_list(interp, next)) {

	/* avoid segfaults */
	/* if(!is_tuple(interp, car(next)) || caar(next)==0 || */
	/*    !is_tuple(interp, cdar(next)) || !cadar(next)==0) { */
	/*     interp->error=1; */

	/*     return false; */
	/*     } */

	sym_list=cons(interp, caar(next), sym_list);
	arg_list=cons(interp, cadar(next), arg_list);

	next=cdr(next);
    }
    


    body=cdr(args);

    
    /* construct the lambda version and return it */
    next=cons(interp, 
	      cons(interp, create_symbol(interp, "lambda"),
		   cons(interp, sym_list, body)),
	      arg_list);

    /*    output(interp, next);
	  printf("\n");*/

    return next;

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


/* apply */
object_type *prim_apply(interp_core_type *interp, object_type *args) {
    object_type *call=0;
    object_type *tail=0;
    int arg_count=list_length(interp, args);

    if(arg_count<2) {
	interp->error=1;
	return false;
    }

    tail=call=cons(interp, car(args), interp->empty_list);

    args=cdr(args);
    
    /* walk everything but the last argument  */
    while(!is_empty_list(interp, args) && !is_empty_list(interp, cdr(args))) {
	tail=cdr(tail)=cons(interp, car(args), interp->empty_list);
	args=cdr(args);
    }
    
    /* make sure we don't blow up on no arguments */
    if(!is_empty_list(interp, args)) {
	if(is_tuple(interp, car(args))) {
	    args=car(args);

	    /* clone containers but not contents */
	    while(!is_empty_list(interp, args)) {
		tail=cdr(tail)=cons(interp, car(args), interp->empty_list);
		args=cdr(args);
	    }	    

	    /*cdr(tail)=car(args);*/
	} else {
	    cdr(tail)=cons(interp, car(args), interp->empty_list);
	}
    }
    

    /* We don't want to re-evaluate arguments, so we 
       quote them all individiually */
    tail=cdr(call);
    
    while(!is_empty_list(interp, tail)) {
	car(tail)=quote(interp, car(tail));
	tail=cdr(tail);
    }

    return call;
}

/* list */
object_type *prim_list(interp_core_type *interp, object_type *args) {
    return args;
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
		    eval(interp, cadr(args)),
		    &interp->cur_env); /* Value */
    } else {
	/* It can only be a tagged list then */

	bind_symbol(interp, caar(args), /* Symbol */
		    prim_lambda(interp, 
				cons(interp, cdar(args), /* Arguments */
				     cdr(args))),
		    &interp->cur_env); /* Body */
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

/* make-string */
object_type *prim_make_string(interp_core_type *interp, object_type * args) {
    
    object_type *obj=0;
    int64_t len=0;
    char *c=0;
    
    int arg_len=list_length(interp, args);

    /* we can accept one or two arguments */
    if(arg_len <1 || arg_len > 2) {
	interp->error=1;
	return false;
    }
    
    len=car(args)->value.int_val;
    
    c=alloc_string(interp, len);
    
    /* fill our string */
    if(arg_len==2) {
	memset(c, cadr(args)->value.char_val, len);
    }


    obj=alloc_object(interp, STRING);
    obj->value.string_val=c;
    
    return obj;
}

/* string-length */
object_type *prim_string_length(interp_core_type *interp, object_type *args) {
    
    object_type *obj=0;
    
    if(list_length(interp, args)!=1) {
	interp->error=1;
	return false;
    }

    /* use strlen to find the length of our string */
    obj=alloc_object(interp, FIXNUM);
    obj->value.int_val=strlen(car(args)->value.string_val);

    
    return obj;    
}

/* string-set! */
object_type *prim_string_set(interp_core_type *interp, object_type *args) {
    
    int64_t index=0;
    char *c=0;
    
    if(list_length(interp, args)!=3) {
	interp->error=1;
	return false;
    }

    c=car(args)->value.string_val;
    index=cadr(args)->value.int_val;

    c[index]=caddr(args)->value.char_val;
    
    
    return true;
    
}

/* string-ref */
object_type *prim_string_ref(interp_core_type *interp, object_type *args) {
    
    object_type *obj=0;
    int64_t index=0;
    char *c=0;
    
    if(list_length(interp, args)!=2) {
	interp->error=1;
	return false;
    }

    c=car(args)->value.string_val;
    index=cadr(args)->value.int_val;

    obj=alloc_object(interp, CHAR);
    obj->value.char_val=c[index];
    
    
    return obj;
    
}


/* /\* String to List *\/ */
/* object_type *prim_string_to_list(interp_core_type *interp, object_type *args) { */
    
/*     object_type *obj=0; */
/*     object_type *head=0; */
/*     object_type *tail=0; */
/*     char *c=0; */

/*     /\* we have to have one argument and it must */
/*        be a string *\/ */
/*     if(list_length(interp, args)!=1 */
/*        || car(args)->type!=STRING) { */
/* 	interp->error=1; */
/* 	return false; */
/*     } */
    
/*     c=car(args)->value.string_val; */

/*     /\* Convert our string to a list of characters *\/ */
/*     while(*c!=0) { */

/* 	/\* allocate our character *\/ */
/* 	obj=alloc_object(interp, CHAR); */
/* 	obj->value.char_val=*c; */
	
/* 	if(head==0) { */
/* 	    head=tail=cons(interp, obj, interp->empty_list); */
/* 	} else { */
/* 	    tail=cdr(tail)=cons(interp, obj, interp->empty_list); */
/* 	} */

/* 	/\* move to the next character *\/ */
/* 	c++; */
/*     } */

/*     return head; */
/* } */

/* object_type *prim_string_to_list(interp_core_type *interp, object_type *args) { */

/* } */

/* Symbol to String */
object_type *prim_sym_to_string(interp_core_type *interp, object_type *args) {
    object_type *obj=0;
    
    /* make sure we have enough arguments */
    if(list_length(interp, args)!=1) {
	interp->error=1;
	return false;
    }
    
    if(car(args)==0 || car(args)->type!=SYM) {
	interp->error=1;
	return false;	
    }

    obj=create_string(interp, car(args)->value.string_val);
    
    return obj;
}

/* String to Symbol */
object_type *prim_string_to_sym(interp_core_type *interp, object_type *args) {
    object_type *obj=0;
    
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

    default:
	interp->error=1;
	return false;
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
            default:							\
		interp->error=1;					\
		return false;						\
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


TEST(is_empty_list(interp, car(args)), prim_is_null)
TEST(car(args)!=0 && car(args)->type==BOOL, prim_is_boolean)
TEST(car(args)!=0 && car(args)->type==SYM, prim_is_symbol)
TEST(car(args)!=0 && car(args)->type==FIXNUM, prim_is_integer)
TEST(car(args)!=0 && car(args)->type==CHAR, prim_is_char)
TEST(car(args)!=0 && car(args)->type==STRING, prim_is_string)
TEST(is_tuple(interp, car(args)) && car(args)!=interp->empty_list, prim_is_tuple)
TEST(car(args)!=0 && car(args)->type==PRIM, prim_is_prim)
TEST(car(args)!=0 && car(args)->type==CHAR && car(args)==eof_object, prim_is_eof_object)

TEST(car(args)!=0 && car(args)->type==PORT && car(args)->value.port_val.input, prim_is_input_port)
TEST(car(args)!=0 && car(args)->type==PORT && car(args)->value.port_val.output, prim_is_output_port)

#define NUMERIC_TEST(test, name)					\
    object_type *name(interp_core_type *interp, object_type *args) {	\
	object_type *first=0;						\
	object_type *next=0;						\
									\
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
            default:							\
		interp->error=1;					\
		return false;						\
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
NUMERIC_TEST(>=, prim_greater_equal)
NUMERIC_TEST(<=, prim_less_equal)


/* eq */
object_type *prim_eq(interp_core_type *interp, object_type *args) {
    object_type *obj1=0;
    object_type *obj2=0;
    
    /* Make sure we have at least 2 arguments */			
    if(list_length(interp, args)!=2) {				
	interp->error=1;						
	return false;						
    }

    obj1=car(args);
    obj2=cadr(args);

    /* avoid segfaults */
    if(obj1==0 || obj2==0) {
	interp->error=1;
	return false;
    }

    /* objects of different types cannot be equal */
    if(obj1->type != obj2->type) {
	return false;
    }

    /* internal comparisons */
    switch(obj1->type) {
    case FIXNUM:
    case FLOATNUM:
	return prim_equal(interp, args);
	break;

    case CHAR:
	return obj1->value.char_val==obj2->value.char_val ?
	    true : false;
	break;

    case STRING:
	return strcmp(obj1->value.string_val, obj2->value.string_val)==0 ?
	    true : false;
	break;

    default:
	return obj1==obj2 ?
	    true : false;
	break;
    }

}

/* and */
object_type *prim_and(interp_core_type *interp, object_type *args) {
    object_type *result=0;

    /* evalueate each argument */
    while(!is_empty_list(interp, args)) {
	result=eval(interp, car(args));

	/* If the result is false, return false
	   otherwise keep evaluating */
	if(!is_true(interp, result)) {
	    return false;
	}

	args=cdr(args);
    }
    
    /* return the last argument */
    return true;

}

/* or */
object_type *prim_or(interp_core_type *interp, object_type *args) {
    object_type *result=0;

    /* evalueate each argument */
    while(!is_empty_list(interp, args)) {
	result=eval(interp, car(args));
	
	/* If the result is false, return false
	   otherwise keep evaluating */
	if(is_true(interp, result)) {
	    return true;
	}

	args=cdr(args);
    }
    
    /* return the last argument */
    return false;

}

/* return the current environment */
object_type *prim_interaction_environment(interp_core_type *interp, object_type *args) {
    return interp->cur_env;
}

/* populate a new base environment */
object_type *prim_null_environment(interp_core_type *interp, object_type *args) {
    object_type *env=0;
    
    env=cons(interp, interp->empty_list, interp->empty_list);

    return env;
}
/* populate a new base environment */
object_type *prim_environment(interp_core_type *interp, object_type *args) {
    object_type *env=0;
    
    env=cons(interp, interp->empty_list, interp->empty_list);

    /* bind default primitive symbols */
    bind_symbol_list(interp, primitive_list, &env);

    /* for cond */
    bind_symbol(interp, 
		create_symbol(interp, "else"),
		true,
		&env);

    return env;
}

/* eval */
object_type *prim_eval(interp_core_type *interp, object_type *args) {
    int arg_count=list_length(interp, args);

    /* we have to some arguments */
    if(arg_count==0) {
	interp->error=1;
	return false;

    } else if(arg_count==1) {
	/* evaluate in the current environment */
	return car(args);

    } else  {
	/* replace the current environment 
	   with the passed in one */
	interp->cur_env=cadr(args);
	return car(args);
    }
}

/* load primitive */
object_type *prim_load(interp_core_type *interp, object_type *args) {
    FILE *fin=0;
    char *filename=0;
    
    object_type *parse_result=interp->empty_list;
    object_type *current=0;
    object_type *parsed=0;
    
    /* make sure that we have one argument */
    if(list_length(interp, args)!=1) {
	interp->error=1;
	return false;
    }
    
    filename=car(args)->value.string_val;
    
    /* open the file */
    fin=fopen(filename, "r");

    /* make sure we could open the file */
    if(!fin) {
	interp->error=1;
	return false;
    }
    
    push_parse_state(interp, fin);

    /* create our implicit begin form */
    current=parse_result=cons(interp,
		      create_symbol(interp, "begin"),interp->empty_list);

    /* parse all objects in the file 
       until eof or error */
    while(interp->running==1 && interp->error==0) {

	parsed=parse_chain(interp);

	if(interp->running) {
	    /* parse the file and attach it to our begin form */
	    cdr(current)=cons(interp,
			      parsed, interp->empty_list);

	    current=cdr(current);
	}

    }

    pop_parse_state(interp);

    interp->running=1;

    fclose(fin);

    return parse_result;
    
}

/* read */
object_type *prim_read(interp_core_type *interp, object_type *args) {
    FILE *fin=0;
    
    object_type *parsed=0;
    
    int len=list_length(interp, args);

    if(len==0) {
	fin=stdin;
    } else {
	fin=car(args)->value.port_val.port;
    }
    
    push_parse_state(interp, fin);

    parsed=parse_chain(interp);

    if(!interp->running) {
	parsed=eof_object;
    }

    pop_parse_state(interp);

    interp->running=1;

    return parsed;
    
}

/* write */
object_type *prim_write(interp_core_type *interp, object_type *args) {
    object_type *obj=0;
    FILE *fout=0;
    
    int len=list_length(interp, args);

    if(len==1) {
	/* writing to stdout */
	fout=stdout;

    } else if( len==2) {
	/* writing to specified port */
	fout=cadr(args)->value.port_val.port;

    } else {
	/* we have to have something to write */
	interp->error=1;
	return false;
    }

    /* object graph to write */
    obj=car(args);
    
    output_stream(interp, obj, fout);
    
    return true;
}

/* write-char */
object_type *prim_write_char(interp_core_type *interp, object_type *args) {
    object_type *obj=0;
    FILE *fout=0;
    
    int len=list_length(interp, args);

    if(len==1) {
	/* writing to stdout */
	fout=stdout;

    } else if( len==2) {
	/* writing to specified port */
	fout=cadr(args)->value.port_val.port;

    } else {
	/* we have to have something to write */
	interp->error=1;
	return false;
    }

    /* object graph to write */
    obj=car(args);
    
    /* TODO: add type checking here */
    fputc(obj->value.char_val, fout);
    fflush(fout);

    return true;
}

/* read-char */
object_type *prim_read_char(interp_core_type *interp, object_type *args) {
    object_type *obj=0;
    char c=0;

    FILE *fin=0;
    
    int len=list_length(interp, args);

    if(len==0) {
	fin=stdin;
    } else {
	fin=car(args)->value.port_val.port;
    }
    
    
    c=fgetc(fin);

    /* check for eof */
    if(c==EOF) {
	return eof_object;
    }


    obj=alloc_object(interp, CHAR);
    obj->value.char_val=c;

    return obj;
}

/* peek-char */
object_type *prim_peek_char(interp_core_type *interp, object_type *args) {
    object_type *obj=0;
    char c=0;

    FILE *fin=0;
    
    int len=list_length(interp, args);

    if(len==0) {
	fin=stdin;
    } else {
	fin=car(args)->value.port_val.port;
    }
    
    
    /* undo the read */
    c=fgetc(fin);
    ungetc(c, fin);

    /* check for eof */
    if(c==EOF) {
	return eof_object;
    }

    obj=alloc_object(interp, CHAR);
    obj->value.char_val=c;

    return obj;
}

/* open-input-file */
object_type *prim_open_input_file(interp_core_type *interp, object_type *args) {
    object_type *obj=0;
    char *filename=0;
    FILE *fin=0;
    
    /* make sure we have a file name */
    if(list_length(interp, args)!=1) {
	interp->error=1;
	return false;
    }

    /* TODO: add type checking */
    filename=car(args)->value.string_val;

    fin=fopen(filename, "r");
    
    /* make sure that we could open the file */
    if(!fin) {
	interp->error=1;
	return false;
    }

    obj=alloc_object(interp, PORT);
    obj->value.port_val.port=fin;
    obj->value.port_val.input=1;
    
    return obj;
}

/* open-output-file */
object_type *prim_open_output_file(interp_core_type *interp, object_type *args) {
    object_type *obj=0;
    char *filename=0;
    FILE *fin=0;
    
    /* make sure we have a file name */
    if(list_length(interp, args)!=1) {
	interp->error=1;
	return false;
    }

    /* TODO: add type checking */
    filename=car(args)->value.string_val;

    fin=fopen(filename, "w");
    
    /* make sure that we could open the file */
    if(!fin) {
	interp->error=1;
	return false;
    }

    obj=alloc_object(interp, PORT);
    obj->value.port_val.port=fin;
    obj->value.port_val.output=1;
    
    return obj;
}


/* close-input-file close-output-file */
object_type *prim_close(interp_core_type *interp, object_type *args) {
    object_type *obj=0;

    if(list_length(interp, args)!=1) {
	interp->error=1;
	return false;
    }
    
    obj=car(args);

    /* don't want to close something that is
       not a port */
    if(obj->type != PORT) {
	interp->error=1;
	return false;
    }

    /* attempt to close the port, checking for errors */
    if(fclose(obj->value.port_val.port)==EOF) {
	/* Something went wrong here */
	interp->error=1;
	return false;
    }

    
    return true;
}

/* error */
object_type *prim_error(interp_core_type *interp, object_type *args) {
    
    /* If we don't have any arguments, return an 
       empty list */
    if(is_empty_list(interp, args)) {
	return quote(interp, interp->empty_list);
    }
    
    /* evalueate each argument */
    while(!is_empty_list(interp, args)) {
	output_stream(interp, car(args), stderr);
	fprintf(stderr, " ");
	args=cdr(args);
    }

    return prim_quit(interp, interp->empty_list);
}


/* Setup scheme primitive function bindings */
binding_type primitive_list[]={
    {"define", &prim_define, 0, 1},
    {"set!", &prim_set, 0, 1},
    {"quit", &prim_quit, 0, 1},
    {"quote", &prim_quote, 0, 1},
    {"if", &prim_if, 0, 0},
    {"cond", &prim_cond, 0, 0},
    {"lambda", &prim_lambda, 0, 1},
    {"begin", &prim_begin, 0, 0},
    {"let", &prim_let, 0, 0},

    {"apply", &prim_apply, 1, 0},

    {"error",&prim_error, 1, 1},

    {"cons", &prim_cons, 1, 1},
    {"car", &prim_car, 1, 1},
    {"cdr", &prim_cdr, 1, 1},
    {"set-car!", &prim_set_car, 1, 1},
    {"set-cdr!", &prim_set_cdr, 1, 1},
    {"list", &prim_list, 1, 1},

    {"+", &prim_plus, 1, 1},
    {"-", &prim_minus, 1, 1},
    {"*", &prim_multi, 1, 1},
    {"/", &prim_div, 1, 1},
    {"quotient", &prim_div_int, 1, 1},
    {"remainder", &prim_mod, 1, 1},

    {"=", &prim_equal, 1, 1},
    {"<", &prim_less, 1, 1},
    {">", &prim_greater, 1, 1},
    {">=", &prim_greater_equal, 1, 1},
    {"<=", &prim_less_equal, 1, 1},

    {"null?", &prim_is_null, 1, 1},
    {"boolean?", &prim_is_boolean, 1, 1},
    {"symbol?", &prim_is_symbol, 1, 1},
    {"integer?", &prim_is_integer, 1, 1},
    {"char?", &prim_is_char, 1, 1},
    {"string?", &prim_is_string, 1, 1},
    {"pair?", &prim_is_tuple, 1, 1},
    {"procedure?", &prim_is_prim, 1, 1},

    {"eq?", &prim_eq, 1, 1}, /* TODO: I need a good test suite for eq and eqv */
    {"eqv?", &prim_eq, 1, 1}, 

    {"and", &prim_and, 0, 1},
    {"or", &prim_or, 0, 1},
    
    {"char->integer", &prim_char_to_int, 1, 1},
    {"integer->char", &prim_int_to_char, 1, 1},
    {"number->string", &prim_num_to_string, 1, 1},
    {"string->number", &prim_string_to_num, 1, 1},
    {"symbol->string", &prim_sym_to_string, 1, 1},
    {"string->symbol", &prim_string_to_sym, 1, 1},

    /* {"string->list", &prim_string_to_list, 1, 1}, */
    /* {"list->string", &prim_list_to_string, 1, 1}, */

    {"make-string", &prim_make_string, 1, 1},
    {"string-length", &prim_string_length, 1, 1},
    {"string-set!", &prim_string_set, 1, 1},
    {"string-ref", &prim_string_ref, 1, 1},

    {"interaction-environment", &prim_interaction_environment, 1, 1},
    {"null-environment", &prim_null_environment, 1, 1},
    {"environment", &prim_environment, 1, 1},
    {"eval", &prim_eval, 1, 0},

    {"load", &prim_load, 1, 0},
    {"read", &prim_read, 1, 1},
    {"write", &prim_write, 1, 1},

    {"read-char", &prim_read_char, 1, 1},
    {"write-char", &prim_write_char, 1, 1},
    {"peek-char", &prim_peek_char, 1, 1},

    {"open-input-port", &prim_open_input_file, 1, 1},
    {"open-input-file", &prim_open_input_file, 1, 1},

    {"open-output-port", &prim_open_output_file, 1, 1},
    {"open-output-file", &prim_open_output_file, 1, 1},

    {"close-input-port", &prim_close, 1, 1},
    {"close-output-port", &prim_close, 1, 1},

    {"eof-object?", &prim_is_eof_object, 1, 1},
    {"input-port?", &prim_is_input_port, 1, 1},
    {"output-port?", &prim_is_output_port, 1, 1},
    
    {0,0} /* Terminate the list */
};
