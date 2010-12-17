#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "core.h"
#include "scheme_funcs.h"
#include "gc_funcs.h"
#include "util.h"


/* with-exception-handler */
object_type *prim_with_exception_handler(interp_core_type *interp, object_type *args) {
    int arg_count=list_length(interp, args);
    object_type *result=0;
    object_type *cur_env=0;

    /* we have to have two arguments */
    if(arg_count!=2) {
        interp->error=1;
        return false;
    }
        
    /* save off some things we need in event of an error */
    cur_env=interp->cur_env;

    /* attempt to eval */
    result=eval(interp,
                cons(interp, 
                     cadr(args),
                     interp->empty_list));

    /* we had an error */
    if(has_error(interp)) {

        result=interp->exception; /* extract the exception object if any */
        
        /* check for no exception object */
        if(!result) {
            result=create_string(interp, "Undefined Exception!");
        }

        /* clean this up so we can run code */
        reset(interp);
        
        /* restore the environment stack */
        interp->cur_env=cur_env;

        /* attempt to run the exception handler */
        result=cons(interp, 
                         car(args), 
                         cons(interp, 
                              result, interp->empty_list));
        result=eval(interp,result);
    }

    return result;
}


/* primitives */
binding_type primitive_list[]={
    {"with-exception-handler", &prim_with_exception_handler, 1, 1}
};
