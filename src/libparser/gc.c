#include<stdlib.h>
#include<strings.h>

#include "core.h"
#include "util.h"

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

/* Make a copy of an object */
object_type *clone(interp_core_type *interp, object_type *obj) {
    object_type *new_obj;
    object_type *next=0;
    char *str=0;

    /* don't try to clone a null */
    if(obj==0) {
	return 0;
    }
    
    if(obj->type==STRING) {
	return create_string(interp, obj->value.string_val);
    }

    /* There should only be one instance of a given symbol */
    if(obj->type==SYM) {
	return obj;
    }

    new_obj=alloc_object(interp, obj->type);
    next=new_obj->next; /* save off the next point */

    memcpy(new_obj, obj, sizeof(object_type));
    
    new_obj->next=next;
}

/* free a list a list of objects */
void free_list(object_type *obj) {
    while(obj) {
	object_type *next=obj->next;

	/* make sure we clear out string buffers */
	switch(obj->type) {
	case STRING:
	    if(obj->value.string_val!=0) {
		free(obj->value.string_val);
	    }
	    break;

	case SYM:
	    if(obj->value.symbol.name!=0) {
		free(obj->value.symbol.name);
	    }
	    break;

	default:
	    break;
	}
	
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

    if(interp->gc.free_list) {
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
