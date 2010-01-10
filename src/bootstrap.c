#include <stdlib.h>
#include <stdio.h>


#include "libparser/core.h"


int main(int argc, char **argv) {


    char *buffer=0;
    size_t buffer_size=0;

    interp_core_type * interp=create_interp();

    printf("Simple Bootstrapper\n");
    printf("sizeof(object_type) %i\n", sizeof(object_type));

    while(interp->running) {
	object_type *obj=0;

	printf(">\n");

	/* clean up after the previous run */
	if(buffer_size>0){ 
	    buffer[0]='\0';
	}

	/* if the user actually entered anything, parse it */
	if(getline(&buffer, &buffer_size, stdin)) {

	    if(buffer[0]=='\n') {
		interp->running=0;

	    } else {

		obj=parse(interp, buffer);
		output(interp, obj);
	    }

	    printf("\n");
	}
	
    }

    /* clean up the input buffer */
    if(buffer_size) {
	free(buffer);
    }

    cleanup_interp(interp);

    exit(0);
}
