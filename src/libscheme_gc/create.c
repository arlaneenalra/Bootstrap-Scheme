#include<stdlib.h>
#include<string.h>

#include "core.h"
#include "util.h"
#include "scheme_funcs.h"
#include "gc_funcs.h"


/* Create a new string instance */
object_type *create_string(interp_core_type *interp, const char *str) {
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

/* create an instance of a primitive object */
object_type *create_primitive(interp_core_type *interp, fn_type primitive,
			      bool eval_first, bool eval_end) {
    object_type *obj=0;

    obj=alloc_object(interp, PRIM);
    obj->value.primitive.fn=primitive;
    obj->value.primitive.eval_first=eval_first;
    obj->value.primitive.eval_end=eval_end;
    
    return obj;
}

