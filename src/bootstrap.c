#include <stdlib.h>
#include <stdio.h>
#include <gc.h>

#include "libparser/core.h"


int main(int argc, char **argv) {

    
    GC_INIT();

    interp_core_type * interp=create_interp();

    printf("Simple Bootstrapper\n");
    printf("sizeof(object_type) %zu\n", sizeof(object_type));

    /* while the interpreter is running */
    while(interp->running) {
	object_type *obj=0;

	printf(">");

	obj=parse(interp, stdin);
	
	if(interp->running) {
	    obj=eval(interp, obj);
	
	    // if there was no error, output the result
	    if(!has_error(interp)) {
		output(interp, obj);
	    } else {	       
		printf("There was an error executing the given expression");
	    }
	}
	printf("\n");
    }
}
