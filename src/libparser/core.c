#include <stdlib.h>
#include <strings.h>

#include "core.h"
#include "util.h"

void free_list(object_type *obj);
void gc_all(interp_core_type *interp);

void create_booleans(interp_core_type *interp);
object_type *alloc_object(interp_core_type *interp, object_type_enum obj_type);

void add_object(interp_core_type *interp, object_type *obj) {
    object_type *current=0;

    /* replace nil if this is the first object allocated */
    if(interp->root==0) {

	interp->root=alloc_object(interp, TUPLE);
	interp->current=interp->root; 
    }
    
    current=interp->current;
    
    /* make sure we have an empty car to deal with */
    if(current->value.car!=0) {	

	current->next=alloc_object(interp, TUPLE);
	current=interp->current=current->next;
    }
    
    /* set the value, finally */
    current->value.car=obj;
}

/* Parse a string */
object_type *parse(interp_core_type *interp, const char *buf) {

    interp->current=interp->root=alloc_object(interp, TUPLE);
    
    yy_scan_string(buf, interp->scanner);
    yyparse(interp, interp->scanner);
    
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

