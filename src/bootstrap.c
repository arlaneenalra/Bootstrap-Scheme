#include <stdlib.h>
#include <strings.h>
#include <assert.h>

#include "core.h"

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
    
    fprintf(stderr, "Unable to allocate memory\n");
    assert(0);
}


int main(int argc, char **argv) {


    interp_core_type interp;

    printf("Simple Bootstrapper\n");
    printf("sizeof(object_type) %li\n", sizeof(object_type));

    object_type *obj=alloc_object(&interp, INTEGER);

    printf(">\n");

    exit(0);
}
