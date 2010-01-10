#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "util.h"

void free_list(object_type *obj);
void mark_free(interp_core_type *interp, object_type *obj);
void gc_all(interp_core_type *interp);


void create_booleans(interp_core_type *interp);
object_type *alloc_object(interp_core_type *interp, object_type_enum obj_type);
void clear_state_stack(interp_core_type *interp);

void output(interp_core_type *interp, object_type *obj);

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

	if(current->value.tuple.car!=0) {
	    fail("Car is already set!");
	}
	current->value.tuple.car=obj;
	break;

    case CDR:
	TRACE("Sd");
	if(current==0) {
	    fail("Attempt to set cdr on null pointer");
	}

	if(current->value.tuple.cdr!=0) {
	    fail("Cdr is already set!");
	}
	current->value.tuple.cdr=obj;
	break;

    case BARE:
	TRACE("B");

 	if(interp->root==0) {
	    interp->root=obj;
	} 

	break;

    default:
	fail("Invalide setting state!");
    }

}

/* Put the passed object into the added buffer */
void add_object(interp_core_type *interp, object_type *obj) {
    interp->added=obj;
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

    /* create a new buffer for the string value */
    char *c=malloc(strlen(str));
    strcpy(c, str);

    obj=alloc_object(interp, STRING);
    obj->value.string_val=c;

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
    object_type *state=0;
    object_type *new_state=interp->added;

    TRACE("C")

    /* handle the first list entry correctly */
    if(interp->current->value.tuple.car==0) {
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

    TRACE("Pu")

    state=alloc_object(interp, TUPLE);

    /* push the current state onto the state stack */
    state->value.tuple.cdr=interp->state_stack;
    state->value.tuple.car=interp->current;
    interp->state_stack=state;
    
    interp->current=new_state;
    
    /* if this is the first object, add it as root */
    if(interp->root==0) {
	interp->root=new_state;
    }
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
    interp->current=state->value.tuple.car;
    interp->state_stack=state->value.tuple.cdr;

    /* we are in the depths of a chain, pop until 
       we're out of it */
    if(state->type==CHAIN) {
	set(interp, CDR);
	pop_state(interp);
    }

    mark_free(interp, state);
    
}

/* Make sure that there are no loose state's sitting on 
   the stack */
void clear_state_stack(interp_core_type *interp) {
    while(interp->state_stack!=0) {
	pop_state(interp);
    }
}

/* Parse a string */
object_type *parse(interp_core_type *interp, const char *buf) {
    
    interp->current=interp->root=0;
    clear_state_stack(interp);

    TRACE("RESET\n");
    
    yy_scan_string(buf, interp->scanner);
    
    yyparse(interp, interp->scanner);

    TRACE("\n")
    
    return interp->root;
}

object_type * eval(interp_core_type *interp) {
    return interp->root;
}

/* Output an object graph to stdout */
void output(interp_core_type *interp, object_type *obj) {
    object_type *car=0;
    object_type *cdr=0;


    /* make sure there is something to display */
    if(obj==0) {
	printf("nil");
	return;
    }
    
    switch(obj->type) {
    case FIXNUM:
	printf("%" PRIi64, obj->value.int_val);
	break;
	
    case FLOATNUM:
	printf("%Lg", obj->value.float_val);
	break;

    case BOOL:
	if(obj==interp->boolean.true) {
	    printf("#t");
	} else if (obj==interp->boolean.false) {
	    printf("#f");
	} else {
	    fail("BOOL is not a boolean");
	}
	break;

    case CHAR:
	printf("#\\");
	if(obj->value.char_val=='\n') {
	    printf("newline");
	} else if(obj->value.char_val==' ') {
	    printf("space");
	} else {
	    printf("%c", obj->value.char_val);
	}

	break;

    case STRING:
	printf("\"%s\"", obj->value.string_val);
	break;

    case TUPLE:
	car=obj->value.tuple.car;
	cdr=obj->value.tuple.cdr;

      	printf("(");
	output(interp, car);

	if(cdr!=0) {
	    if(cdr->type==TUPLE && 
	       (cdr->value.tuple.cdr==0 || cdr->value.tuple.cdr->type==TUPLE)) {

		while(cdr!=0) {
		    printf(" ");

		    car=cdr->value.tuple.car;
		    cdr=cdr->value.tuple.cdr;

		    output(interp, car);
		    
		    /* if the next element is not a tupple,
		       for output it and set cdr to 0 */
		    if(cdr!=0 && cdr->type!=TUPLE) {
			printf(" . ");
			output(interp, cdr);
			cdr=0;
		    }
		    
		}

	    } else {
		printf(" . ");
		output(interp, cdr);
	    }
	}

	printf(")");
	
       
	break;

    default:
	break;
    }
}


/* Create instances of the global boolean values */
void create_booleans(interp_core_type *interp) {
    object_type *obj=0;

    obj=alloc_object(interp,BOOL);
    obj->value.bool_val=1;
    interp->boolean.true=obj;

    
    obj=alloc_object(interp,BOOL);
    obj->value.bool_val=0;
    interp->boolean.false=obj;
}

/* Create an instance of the interpreter */
interp_core_type *create_interp() {
    interp_core_type *interp=0;
    
    interp=malloc(sizeof(interp_core_type));
    
    /* Setup the interpreter */
    if(interp) {
	bzero(interp, sizeof(interp_core_type));

	interp->running=1;

	/* create some special values */
	create_booleans(interp);

	/* create an instance of the parser/lexer */
	yylex_init_extra(interp, &(interp->scanner));

	return interp;
    }
    
    fail("Unable to create interpreter instance");
}

/* Clean up allocations in the interpreter. */
void cleanup_interp(interp_core_type *interp) {
    yylex_destroy(interp->scanner);

    gc_all(interp);

    free(interp);
}



/* Allocate and return an new object instance */
object_type *alloc_object(interp_core_type *interp, object_type_enum obj_type) {

    object_type *obj=malloc(sizeof(object_type));

    /* check to make sure there was memory to allocate and then
       make sure that we have a properly typed and zeroed object */
    if(obj) {
	bzero(obj, sizeof(object_type));

	/* Add this object to the active list */
	obj->next=interp->gc.active_list;
	interp->gc.active_list=obj;

	obj->type=obj_type;

	return obj;
    }
    
    fail("Unable to new object");
}

/* free a list a list of objects */
void free_list(object_type *obj) {
    while(obj) {
	object_type *next=obj->next;

	/* make sure we clear out string buffers */
	if(obj->type==STRING && obj->value.string_val!=0) {
	    free(obj->value.string_val);
	}
	
	free(obj);
	printf("freed:%ju\n", (uintmax_t)obj);
	obj=next;
    }
}

/* clear out the garbage collector lists */
void gc_all(interp_core_type *interp) {

    if(interp->gc.active_list) {
	free_list(interp->gc.active_list);
	interp->gc.active_list=0;
    }

    if(interp->gc.active_list) {
	free_list(interp->gc.free_list);
	interp->gc.free_list=0;
    }
}

void mark_free(interp_core_type *interp, object_type *obj) {
    object_type *active_list=0;
    object_type *child=0;
    
    active_list=interp->gc.active_list;
    child=obj->next;


    /* move obj to top of free list */
    obj->next=interp->gc.free_list;
    interp->gc.free_list=obj;

    /* Invert the active_list onto the top 
       of the child list */

    while(active_list!=obj && active_list !=0) {
	object_type *top=active_list->next;

	/* Put the current active node on
	   top of the child list */
	active_list->next=child;
	child=active_list;

	active_list=top;
    }
    
    /* save the child list as the new active_list */
    interp->gc.active_list=child;
    
}
