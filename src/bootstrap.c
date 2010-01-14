#include <stdlib.h>
#include <stdio.h>


#include "libparser/core.h"


int main(int argc, char **argv) {


    int running=1;

    interp_core_type * interp=create_interp();

    printf("Simple Bootstrapper\n");
    printf("sizeof(object_type) %zu\n", sizeof(object_type));

    while(running) {
	object_type *obj=0;

	printf(">\n");


	obj=parse(interp, stin);

	if(interp->error==0) {
	    obj=eval(interp, obj);
	}

	if(interp->error==0) {
	    output(interp, obj);
	}
		
	if(interp->error!=0) {
	    printf("There was an error executing the given expression");
	}

	printf("\n");
	
    }


    cleanup_interp(interp);

    exit(0);
}
