#include<stdlib.h>
#include<string.h>

#include "core.h"
#include "util.h"
#include "scheme_funcs.h"

#include <gc.h>

/* Allocate and return an new object instance */
object_type *alloc_object(interp_core_type *interp, object_type_enum obj_type) {
    object_type *obj=0;
	
    obj=(object_type *)GC_malloc(sizeof(object_type));

    /* check to make sure there was memory to allocate and then
       make sure that we have a properly typed and zeroed object */
    if(obj) {

	obj->type=obj_type;

	/* do some special purpose init */
	switch(obj_type) {
	case TUPLE:
	    cdr(obj)=interp->empty_list;
	    break;

	default:
	    break;
	}

	return obj;
    }
    
    fail("Unable to new object");
    
    return 0;
}

/* allocate a vector object */
object_type *alloc_vector(interp_core_type *interp, uint64_t len) {
    object_type *obj=0;
    
    obj=alloc_object(interp, VECTOR);
    
    if(len!=0) {
	/* allocate an array of object_type pointers */
	obj->value.vector.vector=
	    (object_type **)GC_malloc(sizeof(object_type *) * len);
    }
    
    obj->value.vector.length=len;

    return obj;
}

/* allocate string memory */
char *alloc_string(interp_core_type *interp, size_t len) {

    char *c=(char *)GC_malloc(len+1);
    
    if(!c) {
	fail("Unable to allocate string!");
    }

    
    return c;
}

/* allocate and instance of the interpreter state */
interp_core_type *alloc_interp() {

    interp_core_type *interp=
	(interp_core_type *)GC_malloc(sizeof(interp_core_type));

    if(!interp) {
	fail("Unable to allocate interpreter instance!");
    }
    
    return interp;
}


/* allocate an instance of the lexer */
scanner_stack_type *alloc_scanner() {

    scanner_stack_type *scanner=
	(scanner_stack_type *)GC_malloc(sizeof(scanner_stack_type));

    return scanner;
}

/* Create a new string instance */
object_type *create_string(interp_core_type *interp, char *str) {
    object_type *obj=0;

    /* create a new buffer for the string value */
    char *c=alloc_string(interp, strlen(str));
    strcpy(c, str);

    obj=alloc_object(interp, STRING);
    obj->value.string_val=c;
	
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
