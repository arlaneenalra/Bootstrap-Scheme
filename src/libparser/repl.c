#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "core.h"
#include "util.h"
#include "scheme_funcs.h"
#include "parser_internal.h"

/* Load the main library */
void load_main(interp_core_type *interp) {
    struct stat stat_buf;
    object_type *load_call=0;
    object_type *result=0;
    
    /* check the current directory */
    if(stat("lib/main.scm", &stat_buf)) {
	fail("Unable to locate libraries");
    }
    
    load_call=cons(interp, create_symbol(interp, "load"),
		   cons(interp, create_string(interp, "lib/main.scm"),
			interp->empty_list));


    result=eval(interp, load_call);

    /* this is not a good test */
    if(result!=true) {
	fail("There was a problem loading libraries");
    }
}

/* a very simple repl environment */
void repl(interp_core_type *interp) {

    printf("Simple Bootstrapper\n");
    printf("sizeof(object_type) %zu\n", sizeof(object_type));
    
    load_main(interp);

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
