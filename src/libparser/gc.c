#include<stdlib.h>
#include<string.h>

#include "core.h"
#include "util.h"
#include "scheme_funcs.h"

/* Allocate and return an new object instance */
object_type *alloc_object(interp_core_type *interp, object_type_enum obj_type) {
    object_type *obj=0;
	
    /* Do we have any objects already allocate? */
    /* if(interp->gc.free_list==0) { */
    /* 	obj=malloc(sizeof(object_type)); */
    /* 	printf("$"); */
    /* } else { */
    /* 	printf("R"); */
    /* 	/\* Use a previously allocated object *\/ */
    /* 	obj=interp->gc.free_list; */
    /* 	interp->gc.free_list=obj->next; */
    /* } */

    obj=malloc(sizeof(object_type));

    /* check to make sure there was memory to allocate and then
       make sure that we have a properly typed and zeroed object */
    if(obj) {
	bzero(obj, sizeof(object_type));

	/* Add this object to the active list */
	obj->next=interp->gc.active_list;
	interp->gc.active_list=obj;

	obj->type=obj_type;
	obj->mark=interp->gc.cur_mark;

	/* do some special purpose init */
	switch(obj_type) {
	case TUPLE:
	    cdr(obj)=interp->empty_list;
	    break;

	case SYM: /* symbols are not gc'd */
	    mark_perm(interp, obj);
	    break;
	default:
	    break;
	}

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

/* free strings and such */
void free_internal(object_type *obj) {
	switch(obj->type) {
	case STRING:
	    if(obj->value.string_val!=0) {
		free(obj->value.string_val);
		obj->value.string_val=0;
	    }
	    break;

	case SYM:
	    if(obj->value.symbol.name!=0) {
		free(obj->value.symbol.name);
		obj->value.symbol.name=0;
	    }
	    break;

	default:
	    break;
	}
}

/* free a list a list of objects */
void free_list(object_type *obj) {
    while(obj) {
	object_type *next=obj->next;

	/* make sure we clear out string buffers */
	free_internal(obj);
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

void free_marked(interp_core_type *interp, gc_mark_type mark) {
    object_type *active=0;
    object_type *new_active=0;
    object_type *temp_next=0;
    
    active=interp->gc.active_list;
    
    while(active!=0) {
	if(active->mark==mark) {
	    temp_next=active->next;
	    
	    /* clean up the stuff that isn't an object */
	    //	    free_internal(active);
	    
	    /* add to the top of the free list */
	    active->next=interp->gc.free_list;
	    interp->gc.free_list=active;

	    /* move on to the next object */
	    active=temp_next;
	} else {
	    temp_next=active->next;

	    active->next=new_active;
	    new_active=active;

	    printf("Marked:");
	    output(interp, active);
	    printf("\n");
	    
	    active=temp_next;
	}
    }
    
    interp->gc.active_list=new_active;
}


/* get the next mark value */
gc_mark_type next_mark(interp_core_type *interp) {
    return interp->gc.cur_mark==MARK_RED ? MARK_BLACK : MARK_RED;
}

/* mark an object with the opposite of the current mark */
void mark_obj(interp_core_type *interp, object_type *obj, gc_mark_type mark) {
    if(obj->mark==MARK_PERM) {
	return;
    }

    obj->mark=mark;
}

/* Is the passed in object marked */
bool is_marked(interp_core_type *interp, object_type *obj, gc_mark_type mark) {
    return obj->mark==mark;
}

/* Mark all reachable objects in the given object graph*/
void mark_reachable(interp_core_type *interp, object_type *obj, gc_mark_type mark) {
    bool looping=1;

    /* Return immediately on null */
    if(obj==0) {
	return;
    }

    /* Walk every object in our object graph 
       and mark the ones we can see */
    while(looping) {
	/* if the object is marked, we're done with 
	   this branch */
	if(is_marked(interp, obj, mark)) {
	    return;
	}
	
	mark_obj(interp, obj, mark);

	/* special types that need marking */
	switch(obj->type) {
	case CHAIN:
	case TUPLE:

	    /* Mark any referenced objects */
	    mark_reachable(interp, car(obj), mark);

	    obj=cdr(obj); /* move on to the next object */
	    if(is_empty_list(interp, obj)) {
		looping=0;
	    }

	    break;

	case CLOSURE:
	    
	    /* mark the components of the closure */
	    mark_reachable(interp, obj->value.closure.param, mark);
	    mark_reachable(interp, obj->value.closure.body, mark);
	    mark_reachable(interp, obj->value.closure.env, mark);
	    
	    looping=0;
	    break;

	default:
	    looping=0;
	    break;
	}
    }

}

/* Used to mark objects that will not be deallocated 
   while the interpreter is running */
void mark_perm(interp_core_type *interp, object_type *obj) {
    mark_obj(interp, obj, MARK_PERM);
}
