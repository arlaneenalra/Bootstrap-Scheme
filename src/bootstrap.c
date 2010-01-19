#include <stdlib.h>
#include <stdio.h>


#include "libparser/core.h"


int main(int argc, char **argv) {


    interp_core_type * interp=create_interp();

    printf("Simple Bootstrapper\n");
    printf("sizeof(object_type) %zu\n", sizeof(object_type));

    /* while the interpreter is running */
    while(1) {
	object_type *obj=0;

	printf(">");

	obj=parse(interp, stdin);
	
	obj=eval(interp, obj);
	
	// if there was no error, output the result
	if(!has_error(interp)) {
	    output(interp, obj);
	} else {	       
	    printf("There was an error executing the given expression");
	}
	
	printf("\n");
    }
}
