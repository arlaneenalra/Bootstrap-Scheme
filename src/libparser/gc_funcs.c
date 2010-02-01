#include<stdlib.h>
#include<string.h>

#include "core.h"
#include "util.h"
#include "scheme_funcs.h"
#include "parser_internal.h"

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
