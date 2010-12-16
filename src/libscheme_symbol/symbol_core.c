#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dlfcn.h>

#include "core.h"
#include "util.h"
#include "scheme_funcs.h"
#include "symbol.h"
#include "gc_funcs.h"

/* Return an instance of a given symbol from the current symbol
   list. */
object_type *get_symbol(interp_core_type *interp, char *str) {
    object_type *top=0;
    object_type *sym=0;
    
    top=interp->symbols.list;
    
    while(top!=0) {
        sym=car(top);

	/* Is this the right symbol? */
	if(strcmp(sym->value.string_val, str)==0) {
	    return sym;
	}
	
	/* move on to the next entry in the list */
	top=cdr(top);
    }

    return 0;
}

/* Create a new symbol instance */
object_type *create_symbol(interp_core_type *interp, char *str) {
    object_type *list=0;
    object_type *obj=0;
    char *c=0;

    obj=get_symbol(interp, str);
    
    /* does the symbol already exist? */
    if(obj) {
	return obj;
    }

    /* create a new buffer for the string value */
    c=alloc_string(interp, strlen(str));
    strcpy(c, str);

    obj=alloc_object(interp, SYM);
    obj->value.string_val=c;
	
    /* add our new symbol to the symbol list */
    list=cons(interp, obj, interp->symbols.list);

    interp->symbols.list=list;

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
