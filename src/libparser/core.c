#include <stdlib.h>
#include <strings.h>

#include "core.h"
#include "util.h"

/* Allocate and return an new object instance */
object_type *alloc_object(interp_core_type* core, object_type_enum obj_type) {

    object_type *obj=malloc(sizeof(object_type));

    /* check to make sure there was memory to allocate and then
       make sure that we have a properly typed and zeroed object */
    if(obj) {
	bzero(obj, sizeof(object_type));

	obj->type=obj_type;
	return obj;
    }
    
    fail("Unable to new object");
}


interp_core_type *create_interp() {
    interp_core_type *interp=0;
    
    interp=malloc(sizeof(interp_core_type));
    
    /* Setup the interpreter */
    if(interp) {
	bzero(interp, sizeof(interp_core_type));

	interp->running=1;
	

	/* create an instance of the parser/lexer */
	yylex_init(&(interp->scanner));

	return interp;
    }
    
    fail("Unable to create interpreter instance");
}

void cleanup_interp(interp_core_type *interp) {
    yylex_destroy(interp->scanner);
    free(interp);
}

