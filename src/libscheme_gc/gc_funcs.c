#include<stdlib.h>
#include<string.h>

#include "core.h"
#include "util.h"
#include "scheme_funcs.h"

#define GC_PRE_ALLOC_BLOCK 100

void free_all(object_type *list);
void free_internal(object_type *obj);
void set_next_mark_objects(gc_core_type * gc);
void mark_objects(gc_mark_type mark, object_type *obj);
uint64_t count_list(object_type *list);

/* return the number of objects in a list of objects */
uint64_t count_list(object_type *list) {
    uint64_t count=0;
    
    while(list) {
	count++;
	list=list->next;
    }
    return count;
}

/* Output some useful statistics about the garbage collector */
void gc_stats(gc_core_type * gc) {
    uint64_t active=count_list(gc->active_list);
    uint64_t dead=count_list(gc->dead_list);
    uint64_t protected=count_list(gc->protected_list);
    uint64_t perm=count_list(gc->perm_list);
    uint64_t total=active+dead+protected+perm;
    
    printf("GC:Active: %" PRIi64 ", Dead: %" PRIi64 ", Protected: %" PRIi64 ", "
	   "Permenant: %" PRIi64 ", Total Objects: %" PRIi64  ", Roots: %i, Depth: %" PRIi64 "\n", 
	   active, dead, protected, perm, total, gc->root_number, gc->protect_count);
}

/* figure out what the next mark is going to be */
void set_next_mark_objects(gc_core_type * gc) {
    
    /* we only have two marks */
    gc->mark=gc->mark==RED ? BLACK : RED;
}

/* tree through objects and mark them */
void mark_objects(gc_mark_type mark, object_type *obj) {
    
    /* Walk until we run out of obects or see one that is
       already marked. Objects marked permenant are treated 
       as marked. */
    while(obj && obj->mark != mark && obj->mark != PERM) {
	obj->mark=mark;

	switch (obj->type) {
	case TUPLE:
	case CHAIN:
	    mark_objects(mark, cdr(obj)); /* walk the other branch */	    
	    obj=car(obj);
	    break;
            
        case CLOSURE: /* walk the internals of a closure */
            mark_objects(mark, obj->value.closure.param);
            mark_objects(mark, obj->value.closure.body);
            mark_objects(mark, obj->value.closure.env);
            break;

	case VECTOR:
	    /* walk all elements of the vector */
	    for(int i=0; i<obj->value.vector.length;i++) {
		mark_objects(mark, obj->value.vector.vector[i]);
	    }
	    obj=0;

	    break;

	default:
	    obj=0; /* nothing more to do here */
	}
    }
}

/* allocate a block of objects that can be used for gc_alloc_object */
void alloc_block(gc_core_type *gc, int num) {
    object_type * obj=0;
    
    for(int i=num; i>0; i--) {
	obj=(object_type *)calloc(1, sizeof(object_type));
	obj->next=gc->dead_list;
	gc->dead_list=obj;
    }

    printf("Stats");
    gc_stats(gc);
}

/* run a manual sweep */
void gc_sweep(gc_core_type *gc) {
    gc_root_count_type i=0;

    object_type *obj=0;
    object_type *obj_next=0;
    
    printf("\nSweep:");
    gc_stats(gc);

    /* if we are under protection, don't sweep */
    if(gc->protect_count>0) {
	printf("Protected\n");
	return;
    } else {
        cause_bt();
    }
    
    /* setup the next mark */
    set_next_mark_objects(gc);

    /* Walk all of our root pointers and tree through their 
       referenced objects.  Mark the reachable objects with 
       the new mark. */

    for(i=0;i<gc->root_number;i++) {
	obj=*(gc->roots[i]); /* get the object pointed to by this root */

	/* tree down and mark all reachable objects */
	mark_objects(gc->mark,obj);
    }

    /* now move all objects unmarked objects to the top of the dead list */
    
    obj=gc->active_list;
    gc->active_list=0; /* clear the active list */
    
    while(obj) {
	obj_next=obj->next;
	
	/* if our object is marked, add it to the live list */
	if(obj->mark == gc->mark) {
	    obj->next=gc->active_list;
	    gc->active_list=obj;

	} else if(obj->mark == PERM) {
	    /* move object out the list of active objects and
	       into our permenant object list. */
	    obj->next=gc->perm_list;
	    gc->perm_list=obj;
	    
	} else {
	    /* otherwise, add it to the dead list */

            free_internal(obj); /* make sure we don't leave strings, etc allocated. */

	    obj->next=gc->dead_list;
	    gc->dead_list=obj;
	}
	
	/* move on to the next object */
	obj=obj_next;
    }
    
    gc_stats(gc);
}

/* Turn allocated object protection on */
void gc_protect(interp_core_type *interp) {
    interp->gc->protect_count++;
    printf("PR:");
    gc_stats(interp->gc);

}

/* Turn off allocated object protection */
void gc_unprotect(interp_core_type *interp) {
    object_type *obj=0;
    gc_core_type *gc=interp->gc;

    gc->protect_count--;
    
    /* Something went wrong with protection counting */
    if(gc->protect_count<0) {
	fail("Too many unprotects!");
    }

    /* we can safely run the gc */
    if(gc->protect_count==0) {
	
	/* move protected objects into the active_list */
	obj=gc->protected_list;
	
	while(obj) {
	    gc->protected_list=obj->next;
	    
	    /* we need to remark them since there could have
	       been a gc since protection was turned on */
	    if(obj->mark!=PERM) {
		obj->mark=gc->mark;
                /* attach to active list */
                obj->next=gc->active_list;
                gc->active_list=obj;
                
	    } else {
                /* attach object to the perm list */
                obj->next=gc->perm_list;
                gc->perm_list=obj;
            }
	    
	    obj=gc->protected_list;
	}

	/* if there are no dead objects, let's do a collections */
        gc_sweep(gc);    
    }

    printf("DN:");
    gc_stats(gc);
}

/* create an instance of our garbage collector */
gc_core_type *gc_create() {
    gc_core_type *gc=0;
    
    gc=(gc_core_type *)calloc(1, sizeof(gc_core_type));

    if(!gc) {
        fail("Unable to allocate gc");
    }

    gc->mark=RED;

    alloc_block(gc, 100);

    return gc;
}

/* clean up garbage collector instance */
void gc_destroy(gc_core_type *gc) {
    
    if(!gc) {
        return;
    }
    
    gc_stats(gc);

    /* free our lists of objects */
    free_all(gc->active_list);
    free_all(gc->dead_list);
    free_all(gc->perm_list);

    /* free our array of root pointers */
    free(gc->roots);
    
    free(gc);
}

/* Register a root pointer with the gc */
void gc_register_root(interp_core_type *interp, object_type **root_ptr) {
    
    object_type ***new_roots=0;
    gc_core_type *gc=interp->gc;
    
    /* allocate a new root array with one more element*/
    new_roots=calloc(gc->root_number+1,sizeof(object_type*));
    
    if(!new_roots) {
        fail("Unable to allocate new root array");
    }

    if(gc->roots) {

	/* copy old list of roots to the new array */
	memcpy(new_roots, gc->roots, (gc->root_number * sizeof(object_type*)));
	free(gc->roots);
    }
    
    new_roots[gc->root_number]=root_ptr;

    gc->roots=new_roots;
    gc->root_number++;
}

/* mark an object as permenant */
void gc_mark_perm(interp_core_type *interp, object_type *obj) {
    obj->mark=PERM;
}

/* free all objects in a list */
/* TODO: complete this! */
void free_all(object_type *list) {
    object_type *next=list;

    while(list) {
	next=list->next;
        
        free_internal(list);

	free(list);
	list=next;
    }
}

/* free the internals of an object */
void free_internal(object_type *obj) {
    /* handle any internal objects */
    switch(obj->type) {
    case STRING:
    case SYM:
        free(obj->value.string_val);
        break;
	    
    case VECTOR:
        if(obj->value.vector.vector) {
            free(obj->value.vector.vector);
        }
        break;

    default:
        break;
    }
}

/* Allocate and return an new object instance */
object_type *alloc_object(interp_core_type *interp, object_type_enum obj_type) {
    object_type *obj=0;
    gc_core_type *gc=interp->gc;

    
    /* check to see if we have any dead objects */
    if(!gc->dead_list) {
        gc_sweep(gc);
    }

    /* Are there any dead objects out there now? */
    if(gc->dead_list) {
       
	/* pop an object off the top of the dead list */
	obj=gc->dead_list;
	gc->dead_list=obj->next;
	/* zero out the block */
	memset(obj, 0, sizeof(object_type));

    } else {
	alloc_block(gc, GC_PRE_ALLOC_BLOCK); /* allocate a block of objects for next time */
	obj=(object_type *)calloc(1, sizeof(object_type));	
    }

    /* check to make sure there was memory to allocate and then
       make sure that we have a properly typed and zeroed object */
    if(!obj) {
        fail("Unable to allocate new object");
        return 0;
    }

    obj->type=obj_type;

    /* do some special purpose init */
    switch(obj_type) {
    case TUPLE:
        cdr(obj)=interp->empty_list;
        break;

    default:
        break;
    }

    /* Attach to garbage collector */
    if(gc->protect_count==0) {
	obj->next=gc->active_list;
	gc->active_list=obj;
    } else {
	obj->next=gc->protected_list;
	gc->protected_list=obj;
    }

    printf("AL:");
    gc_stats(gc);

    return obj;    
}

/* allocate a vector object */
object_type *alloc_vector(interp_core_type *interp, uint64_t len) {
    object_type *obj=0;
    
    obj=alloc_object(interp, VECTOR);
    
    if(len!=0) {
	/* allocate an array of object_type pointers */
	obj->value.vector.vector=
	    (object_type **)calloc(1, sizeof(object_type *) * len);
    }
    
    obj->value.vector.length=len;

    return obj;
}

/* allocate string memory */
char *alloc_string(interp_core_type *interp, size_t len) {

    char *c=(char *)calloc(1, len+1);
    
    if(!c) {
	fail("Unable to allocate string!");
    }

    
    return c;
}

/* allocate and instance of the interpreter state */
interp_core_type *alloc_interp() {

    interp_core_type *interp=
	(interp_core_type *)calloc(1, sizeof(interp_core_type));

    if(!interp) {
	fail("Unable to allocate interpreter instance!");
    }

    /* attach the garbage collector */
    interp->gc=gc_create();
    
    return interp;
}


/* allocate an instance of the lexer */
scanner_stack_type *alloc_scanner() {

    scanner_stack_type *scanner=
	(scanner_stack_type *)calloc(1, sizeof(scanner_stack_type));

    return scanner;
}

/* Create a new string instance */
object_type *create_string(interp_core_type *interp, char *str) {
    object_type *obj=0;
    char *c=0;

    gc_protect(interp);

    /* create a new buffer for the string value */
    c=alloc_string(interp, strlen(str));
    strcpy(c, str);

    obj=alloc_object(interp, STRING);
    obj->value.string_val=c;

    gc_unprotect(interp);
	
    return obj;
}


/* Make a copy of an object */
object_type *clone(interp_core_type *interp, object_type *obj) {
    object_type *new_obj;
    object_type *next=0;

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

    return new_obj;
}
