#include<stdlib.h>
#include<string.h>

#include "core.h"
#include "util.h"
#include "scheme_funcs.h"
//#include "parser_internal.h"

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

/* allocate string memory */
char *alloc_string(interp_core_type *interp, size_t len) {

    char *c=(char *)GC_malloc(len+1);
    
    if(!c) {
	fail("Unable to allocate string!");
    }

    
    return c;
}

