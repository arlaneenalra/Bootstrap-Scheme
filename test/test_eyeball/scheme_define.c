#include "libtester/test.h"
#include <strings.h>
#include "../src/libparser/core.h"

interp_core_type *interp=0;

int create() {
    interp=create_interp();
    
    /* make sure that create returned something */
    if(interp==0) {
	return 1;
    }
    
    return 0;
}

int define_var() {
    object_type *ret_val=0;

    ret_val=parse_string(interp, "(define a 1)");
    ret_val=eval(interp, ret_val);
    
    /* interpreter should still be alive */
    if(interp->running==0 || interp->error==1) {
	fprintf(stderr,"Invalid return state (%i, %i)\n", 
	       interp->running, interp->error);

	return 1; /* failed */
    }
    
    /* check to make sure that the ret_val is #t */
    if(interp->boolean.true!=ret_val) {
	printf("True is not true\n");
	return 1; /* failed */
    }
    
    return 0;
}

int symbol_lookup() {
    object_type *ret_val=0;

    ret_val=parse_string(interp, "a");
    ret_val=eval(interp, ret_val);
    
    /* interpreter should still be alive */
    if(interp->running==0 || interp->error==1
       || ret_val==0) {
	fprintf(stderr,"Invalid return state (%i, %i)\n", 
	       interp->running, interp->error);

	return 1; /* failed */
    }

    if(ret_val->type==FIXNUM && ret_val->value.int_val==1) {
	return 0;
    }
    
    return 1;
}

int set_var() {
    object_type *ret_val=0;

    ret_val=parse_string(interp, "(set! a (+ 1 1 ))");
    ret_val=eval(interp, ret_val);
    
    /* interpreter should still be alive */
    if(interp->running==0 || interp->error==1) {
	fprintf(stderr,"Invalid return state (%i, %i)\n", 
	       interp->running, interp->error);

	return 1; /* failed */
    }
    
    /* check to make sure that the ret_val is #t */
    if(interp->boolean.true!=ret_val) {
	printf("True is not true\n");
	return 1; /* failed */
    }
    
    return 0;
}

int symbol_lookup2() {
    object_type *ret_val=0;

    ret_val=parse_string(interp, "a");
    ret_val=eval(interp, ret_val);
    
    /* interpreter should still be alive */
    if(interp->running==0 || interp->error==1
       || ret_val==0) {
	fprintf(stderr,"Invalid return state (%i, %i)\n", 
	       interp->running, interp->error);

	return 1; /* failed */
    }

    if(ret_val->type==FIXNUM && ret_val->value.int_val==2) {
	return 0;
    }
    
    return 1;
}

int cleanup() {
    cleanup_interp(interp);
    return 0;
}

test_case_type cases[]={
    {&create, "Creating interpreter."},
    {&define_var, "Defining variable."},
    {&symbol_lookup, "Looking up variable."},
    {&set_var, "Setting variable."},
    {&symbol_lookup2, "Looking up variable."},

    {&cleanup, "Cleaning up interpreter."},
    {0,0}
};
