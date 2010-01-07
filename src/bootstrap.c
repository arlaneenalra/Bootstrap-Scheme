#include <stdlib.h>
#include <strings.h>
#include <stdio.h>


#include "libparser/core.h"


int main(int argc, char **argv) {


    char *buffer=0;
    size_t buffer_size=0;

    interp_core_type * interp=create_interp();

    printf("Simple Bootstrapper\n");
    printf("sizeof(object_type) %li\n", sizeof(object_type));

    while(interp->running) {

	printf(">\n");

	/* clean up after the previous run */
	if(buffer_size>0){ 
	    buffer[0]='\0';
	}

	(void)getline(&buffer, &buffer_size, stdin);
    
	yy_scan_string(buffer, interp->scanner);
	yyparse(interp, interp->scanner);
	
    }

    /*yyset_in(stdin, interp->scanner);
      yyparse(interp->scanner);*/
    


    if(buffer_size) {
	free(buffer);
    }

    cleanup_interp(interp);

    exit(0);
}
