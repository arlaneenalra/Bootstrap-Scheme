#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "core.h"
#include "util.h"
#include "scheme_funcs.h"
#include "parser_internal.h"


object_type * call_load(interp_core_type *interp, char *filename) {
    struct stat stat_buf;
    object_type *load_call=0;

    /* check the current directory */
    if(stat(filename, &stat_buf)) {
	fail("Unable to locate libraries");
    }

    /* build a call to load */
    load_call=cons(interp, create_symbol(interp, "load"),
		   cons(interp, create_string(interp, filename),
			interp->empty_list));


    return eval(interp, load_call);
}

/* Load the main library */
void load_main(interp_core_type *interp) {
    object_type *result=0;
    
    result=call_load(interp, "lib/main.scm");

    /* this is not a good test */
    if(result!=true) {
	fail("There was a problem loading system libraries");
    }
}

/* a very simple repl environment */
void repl(interp_core_type *interp) {

    printf("Simple Bootstrapper\n");
    printf("sizeof(object_type) %" PRIi64 "\n", sizeof(object_type));
    
    load_main(interp);

    /* while the interpreter is running */
    while(interp->running) {
    	object_type *obj=0;

    	printf(">");

    	obj=parse(interp, stdin);
	
    	if(interp->running) {
    	    obj=eval(interp, obj);
	
    	    /* if there was no error, output the result */
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
void top_level_program(interp_core_type *interp, char* filename) {
    object_type *result=0;
    
    load_main(interp);
    
    result=call_load(interp, filename);
    
    if(interp->error==1) {
	fprintf(stderr, "There was an error executing script\n");
	exit(-1);
    }
}
