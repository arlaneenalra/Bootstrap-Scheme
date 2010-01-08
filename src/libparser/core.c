#include <stdlib.h>
#include <strings.h>

#include "core.h"
#include "util.h"

void free_list(object_type *obj);
void mark_free(interp_core_type *interp, object_type *obj);
void gc_all(interp_core_type *interp);


void create_booleans(interp_core_type *interp);
object_type *alloc_object(interp_core_type *interp, object_type_enum obj_type);

void output(interp_core_type *interp, object_type *obj);

void add_object(interp_core_type *interp, object_type *obj) {
    object_type *current=0;

    /* replace nil if this is the first object allocated */
    if(interp->root==0) {

	interp->root=alloc_object(interp, TUPLE);
	interp->current=interp->root; 
    }
    
    current=interp->current;
    
    /* make sure we have an empty car to deal with */
    if(current->value.tuple.car!=0) {	

	current->value.tuple.cdr=alloc_object(interp, TUPLE);
	current=interp->current=current->value.tuple.cdr;
    }
    
    /* set the value, finally */
    current->value.tuple.car=obj;
}

/* Save the current list off so that we can get back to it */
void push_state(interp_core_type *interp) {
    object_type *state=0;
    object_type *new_state=0;

    state=alloc_object(interp, TUPLE);
    new_state=alloc_object(interp, TUPLE);

    add_object(interp, new_state);

    /* push the current state onto the state stack */
    state->value.tuple.cdr=interp->state_stack;
    state->value.tuple.car=interp->current;
    interp->state_stack=state;
    
    interp->current=new_state;
}

/* Pop a previously saved list */
void pop_state(interp_core_type *interp) {
    object_type *state=0;
    
    
    if(interp==0) {
	fail("Attempt to pop non-existent state");
    }
    
    state=interp->state_stack;
    
    interp->current=state->value.tuple.car;
    interp->state_stack->value.tuple.cdr;
    
    mark_free(interp, state);
}

/* Parse a string */
object_type *parse(interp_core_type *interp, const char *buf) {

    interp->current=interp->root=alloc_object(interp, TUPLE);
    
    yy_scan_string(buf, interp->scanner);
    yyparse(interp, interp->scanner);
 
    return interp->root;
}

/* Output an object graph to stdout */
void output(interp_core_type *interp, object_type *obj) {

    /* make sure there is something to display */
    if(obj==0) {
	printf("nil");
	return;
    }
    
    switch(obj->type) {
    case FIXNUM:
	printf("%li", obj->value.int_val);
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
	break;
    case TUPLE:
	printf("(");
	output(interp, obj->value.tuple.car); 

	printf(" ");
	
	if(obj->value.tuple.cdr!=0) {
	    output(interp, obj->value.tuple.cdr);
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
	yylex_init(&(interp->scanner));

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
	free(obj);
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

    /* move children of obj to top of 
       active list */
    while(child!=0) {
	object_type *top=child->next;
	child->next=interp->gc.active_list;

	interp->gc.active_list=child;
	child=top;
    }
}
