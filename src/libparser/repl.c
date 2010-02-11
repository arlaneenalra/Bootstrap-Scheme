#include "core.h"


/* a very simple repl environment */
void repl(interp_core_type *interp) {

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

/* execution of a top-level program */
void top_level_program(interp_core_type *interp, char* file) {
    /* TODO */

}
