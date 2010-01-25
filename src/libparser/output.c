#include <stdlib.h>

#include "core.h"
#include "util.h"
#include "scheme_funcs.h"

/* Output an object graph to stdout */
void output(interp_core_type *interp, object_type *obj) {
    static object_type *last_closure=0;
    object_type *car=0;
    object_type *cdr=0;


    /* make sure there is something to display */
    if(obj==interp->empty_list) {
	printf("()");
	return;
    }
    
    if(obj==0) {
	printf("nil");
	return;
    }
    
    switch(obj->type) {
    case FIXNUM:
	printf("%" PRIi64, obj->value.int_val);
	break;
	
    case FLOATNUM:
	printf("%Lg", obj->value.float_val);
	break;

    case BOOL:
	if(obj==true) {
	    printf("#t");
	} else if (obj==false) {
	    printf("#f");
	} else {
	    fail("BOOL is not a boolean");
	}
	break;

    case CHAR:
	printf("#\\");
	if(obj->value.char_val=='\n') {
	    printf("newline");
	} else if(obj->value.char_val==' ') {
	    printf("space");
	} else {
	    printf("%c", obj->value.char_val);
	}

	break;

    case STRING:
	printf("\"%s\"", obj->value.string_val);
	break;

    case TUPLE:
	car=car(obj);
	cdr=cdr(obj);
	
	/* TODO: This could probably be simplified */
	if(is_quoted(interp, obj)) {
	    printf("'");
	    if(cdr==0) {
		interp->error=1;
		return;
	    }
	    output(interp, car(cdr));

	} else {
	    printf("(");
	    output(interp, car);

	    if(!is_empty_list(interp, cdr)) {
		if(cdr->type==TUPLE) {

		    while(!is_empty_list(interp, cdr) && is_tuple(interp, cdr)) {
			printf(" ");

			car=car(cdr);
			cdr=cdr(cdr);

			output(interp, car);
		    
			/* if the next element is not the empty list
			   end with . cdr */
			if(!is_empty_list(interp, cdr) && !is_tuple(interp,cdr)) {
			    printf(" . ");
			    output(interp, cdr);
			}
		    
		    }

		} else {
		    printf(" . ");
		    output(interp, cdr);
		}
	    }

	    printf(")");

	}
	
	break;
    case SYM:
	printf("%s", obj->value.string_val);
	break;
    case PRIM:
	printf("#<primitive procedure:@%p>", obj->value.primitive.fn);
	break;
    case CLOSURE:

	/* avoid infinite recursion */
	if(last_closure==obj) {
	    printf("#<closure:current>");
	    return;
	}

	last_closure=obj;

	printf("#<closure: params:");
	output(interp, obj->value.closure.param);
	printf(" body:");
	output(interp, obj->value.closure.body);
	printf(">");
	last_closure=0;
	break;

    default:
	break;
    }
}
