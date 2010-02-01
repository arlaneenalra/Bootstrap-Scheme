#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "util.h"
#include "scheme_funcs.h"

#include "parser_internal.h"

#include "scheme.h"
#include "lexer.h"


void create_booleans(interp_core_type *interp);

void clear_state_stack(interp_core_type *interp);

/* Attach an added object to our graph of objects */
void set(interp_core_type *interp, setting_type_enum setting) {
    object_type *current=0;
    object_type *obj=interp->added;
    
    
    obj=interp->added;
    current=interp->current;

    if(current !=0 && current->type!=TUPLE) {
	fail("Attempting to set on none tuple.");
    }

    switch(setting) {
    case CAR:
	TRACE("Sa");
	if(current==0) {
	    fail("Attempt to set car on null pointer");
	}

	car(current)=obj;
	break;

    case CDR:
	TRACE("Sd");
	if(current==0) {
	    fail("Attempt to set cdr on null pointer");
	}

	cdr(current)=obj;
	break;

    default:
	fail("Invalide setting state!");
    }

}

/* Put the passed object into the added buffer */
void add_object(interp_core_type *interp, object_type *obj) {
    interp->added=obj;
}

/* Quote things objects that have been added */
void add_quote(interp_core_type *interp) {
    object_type *obj=0;
 
    TRACE("Qu");

    obj=quote(interp, interp->added);

    add_object(interp,obj);
    
}

/* Create a character constant object */
void add_char(interp_core_type *interp, char *str) {
    object_type *obj=0;
    
    char c=*str;
    
    if(strcmp(str, "newline")==0) {
	c='\n';
    } else if(strcmp(str, "space")==0) {
	c=' ';
    }
    
    obj=alloc_object(interp, CHAR);
    obj->value.char_val=c;
    add_object(interp, obj);
}

/* Create a number */
void add_number(interp_core_type *interp, char *str) {
    object_type *obj=0;

    obj=alloc_object(interp, FIXNUM);
    obj->value.int_val=strtoll(str, 0, 10);
    
    add_object(interp, obj);
}

/* create an instance of a string object */
void add_string(interp_core_type *interp, char *str) {
    object_type *obj=0;

    obj=create_string(interp, str);
    add_object(interp,obj);
}


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

/* Create a new string instance */
object_type *create_string(interp_core_type *interp, char *str) {
    object_type *obj=0;

    /* create a new buffer for the string value */
    char *c=(char *)GC_malloc(strlen(str)+1);
    strcpy(c, str);

    obj=alloc_object(interp, STRING);
    obj->value.string_val=c;
	
    return obj;
}


/* Create a new symbol instance */
object_type *create_symbol(interp_core_type *interp, char *str) {
    object_type *list=0;
    object_type *obj=0;

    obj=get_symbol(interp, str);
    
    /* does the symbol already exist? */
    if(obj) {
	return obj;
    }

    /* create a new buffer for the string value */
    char *c=(char *)GC_malloc(strlen(str)+1);
    strcpy(c, str);

    obj=alloc_object(interp, SYM);
    obj->value.string_val=c;
	
    /* add our new symbol to the symbol list */
    list=cons(interp, obj, interp->symbols.list);

    interp->symbols.list=list;

    return obj;
}

/* create an instance of a string object */
void add_symbol(interp_core_type *interp, char *str) {
    object_type *obj=0;

    TRACE("Sy");

    obj=create_symbol(interp, str);
    
    add_object(interp,obj);
}

/* Create an instance of a floating point number */
void add_float(interp_core_type *interp, char *str) {
    object_type *obj=0;
    
    obj=alloc_object(interp, FLOATNUM);
    obj->value.float_val=strtold(str, 0);

    add_object(interp, obj);
}

/* Add a new state to the current chain of states without
   pushing it onto the state stack */
void chain_state(interp_core_type *interp) {
    object_type *new_state=interp->added;

    TRACE("C")

    /* handle the first list entry correctly */
      if(car(interp->current)==0) {
	/* we don't need to do anythin else here */
	set(interp, CAR);
	return;
    } 

    /* Before throwing current, we 
       need to make sure that it has been added */

    push_state(interp);
    interp->state_stack->type=CHAIN;
    
    add_object(interp, new_state);
    set(interp, CAR);

}

/* Save the current list off so that we can get back to it */
void push_state(interp_core_type *interp) {
    object_type *state=0;
    object_type *new_state=alloc_object(interp, TUPLE);

    TRACE("Pu");

    /* push the current state onto the state stack */
    
    state=cons(interp, interp->current, interp->state_stack);

    interp->state_stack=state;
    
    interp->current=new_state;
    
}

/* Pop a previously saved list */
void pop_state(interp_core_type *interp) {
    object_type *state=0;

    TRACE("Po")
    
    if(interp->state_stack==0) {
	fail("Attempt to pop non-existent state");
    }

    state=interp->state_stack;
    
    interp->added=interp->current;
    interp->current=car(state);
    interp->state_stack=cdr(state);

    /* we are in the depths of a chain, pop until 
       we're out of it */
    if(state->type==CHAIN) {
	set(interp, CDR);
	pop_state(interp);
    }
}

/* Make sure that there are no loose state's sitting on 
   the stack */
void clear_state_stack(interp_core_type *interp) {
    while(interp->state_stack!=0) {
	pop_state(interp);
    }
}

/* Add a new environment on top of the current one */
void push_environment(interp_core_type *interp, object_type *env) {

    TRACE("Peu");
    
    /* replace it with the passed in stack */
    interp->cur_env=cons(interp, interp->empty_list, env);
}

void end_of_file(interp_core_type *interp) {
    interp->running=0;
}

void reset(interp_core_type *interp) {
    interp->current=interp->empty_list;
    clear_state_stack(interp);
    interp->error=0;

    TRACE("RESET\n");
}

/* Parse a file */
object_type *parse(interp_core_type *interp, FILE *in) {
    reset(interp);
    
    yyset_in(in, interp->scanner);
    
    if(yyparse(interp, interp->scanner)) {
	return 0;
    }

    TRACE("\n")
    
    return interp->added;
}

/* Parse a string */
object_type *parse_string(interp_core_type *interp, char *in) {
    reset(interp);
    
    yy_scan_string(in, interp->scanner);
    
    if(yyparse(interp, interp->scanner)) {
	return 0;
    }

    TRACE("\n")
    
    return interp->added;
}

/* Parse used by load */
object_type *parse_chain(interp_core_type *interp) {
    reset(interp);
    

    if(yyparse(interp, interp->scanner)) {
	return 0;
    }

    return interp->added;
}

void push_parse_state(interp_core_type *interp, FILE *fin) {
    yypush_buffer_state(
			yy_create_buffer(fin, YY_BUF_SIZE, interp->scanner),
			interp->scanner);
}

void pop_parse_state(interp_core_type *interp) {
    yypop_buffer_state(interp->scanner);
}

/* Create an instance of the Quote symbol */
void create_quote(interp_core_type * interp) {
    object_type *obj=0;

    obj=create_symbol(interp, "quote");

    interp->quote=obj;
}

/* Create instances of the global boolean values */
void create_booleans(interp_core_type *interp) {
    object_type *obj=0;

    /* true and false are defined by macros to be 
       memvers of interp->boolean */
    obj=alloc_object(interp,BOOL);
    obj->value.bool_val=1;

    true=obj;

    
    obj=alloc_object(interp,BOOL);
    obj->value.bool_val=0;

    false=obj;
}

/* Create a marker for the empty list */
void create_empty_list(interp_core_type *interp) {
    object_type *obj=0;

    obj=alloc_object(interp,TUPLE);

    interp->empty_list=obj;
}

/* Create a marker for EOF */
void create_eof_object(interp_core_type *interp) {
    object_type *obj=0;

    obj=alloc_object(interp,CHAR);

    eof_object=obj;
}

/* setup the base environment */
void create_base_environment(interp_core_type *interp) {

    interp->cur_env=interp->empty_list;

    /* There is nothing in the environment right now
       so, this is just the empty list */
    push_environment(interp,interp->empty_list);

    bind_symbol_list(interp, primitive_list, &interp->cur_env);

    /* for cond */
    bind_symbol(interp, 
		create_symbol(interp, "else"),
		true,
		&interp->cur_env);

 }

/* Create an instance of the interpreter */
interp_core_type *create_interp() {
    interp_core_type *interp=0;

    interp=(interp_core_type *)GC_malloc(sizeof(interp_core_type));
    
    /* Setup the interpreter */
    if(interp) {
	bzero(interp, sizeof(interp_core_type));


	/* create some special values */
	create_booleans(interp);

	/* create special case symbols */
	create_quote(interp);
	
	/* create the empty list */
	create_empty_list(interp);

	/* create the eof_object */
	create_eof_object(interp);

	/* setup the base environment */
	create_base_environment(interp);

	/* create an instance of the parser/lexer */
	yylex_init_extra(interp, &(interp->scanner));

	interp->running=1;

	return interp;
    }
    
    fail("Unable to create interpreter instance");
    return 0;
}

/* Clean up allocations in the interpreter. */
void cleanup_interp(interp_core_type *interp) {
    yylex_destroy(interp->scanner);
}

/* Find the length of a passed in list */
int list_length(interp_core_type* interp, object_type *args) {
    int count=0;
    
    while(!is_empty_list(interp, args)) {
	count++;
	args=cdr(args);
    }

    return count;
}
