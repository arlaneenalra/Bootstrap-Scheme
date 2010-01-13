#include <stdlib.h>
#include <stdio.h>


#include "libparser/core.h"


int main(int argc, char **argv) {


    char *buffer=0;
    size_t buffer_size=0;
    int running=1;

    interp_core_type * interp=create_interp();

    printf("Simple Bootstrapper\n");
    printf("sizeof(object_type) %zu\n", sizeof(object_type));

    while(running) {
	object_type *obj=0;

	printf(">\n");

	/* clean up after the previous run */
	if(buffer_size>0){ 
	    buffer[0]='\0';
	}

	/* if the user actually entered anything, parse it */
	if(getline(&buffer, &buffer_size, stdin)) {

	    if(buffer[0]=='\n') {
		running=0;

	    } else {

		obj=parse(interp, buffer);

		if(interp->error==0) {
		    obj=eval(interp, obj);
		}

		if(interp->error==0) {
		    output(interp, obj);
		}
		
		if(interp->error!=0) {
		    printf("There was an error executing the given expression");
		}
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
