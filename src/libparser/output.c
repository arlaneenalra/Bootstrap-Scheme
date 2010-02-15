#include <stdlib.h>
#include <stdio.h>

#include "core.h"
#include "util.h"
#include "scheme_funcs.h"

/* Output an object graph  */
void output_stream(interp_core_type *interp, object_type *obj, FILE *fout) {
    static object_type *last_closure=0;
    object_type *car=0;
    object_type *cdr=0;


    /* make sure there is something to display */
    if(obj==interp->empty_list) {
	fprintf(fout,"()");
	return;
    }
    
    if(obj==0) {
	fprintf(fout,"nil");
	return;
    }
    
    switch(obj->type) {
    case FIXNUM:
	fprintf(fout,"%" PRIi64, obj->value.int_val);
	break;
	
    case FLOATNUM:
	fprintf(fout,"%Lg", obj->value.float_val);
	break;

    case BOOL:
	if(obj==true) {
	    fprintf(fout,"#t");
	} else if (obj==false) {
	    fprintf(fout,"#f");
	} else {
	    fail("BOOL is not a boolean");
	}
	break;

    case CHAR:
	fprintf(fout,"#\\");
	if(obj->value.char_val=='\n') {
	    fprintf(fout,"newline");
	} else if(obj->value.char_val==' ') {
	    fprintf(fout,"space");
	} else {
	    fprintf(fout,"%c", obj->value.char_val);
	}

	break;

    case STRING:
	fprintf(fout,"\"%s\"", obj->value.string_val);
	break;

    case TUPLE:
	car=car(obj);
	cdr=cdr(obj);
	
	/* TODO: This could probably be simplified */
	if(is_quoted(interp, obj)) {
	    fprintf(fout,"'");
	    if(cdr==0) {
		interp->error=1;
		return;
	    }
	    output_stream(interp, car(cdr), fout);

	} else {
	    fprintf(fout,"(");
	    output_stream(interp, car, fout);

	    if(!is_empty_list(interp, cdr)) {
		if(cdr->type==TUPLE) {

		    while(!is_empty_list(interp, cdr) && is_tuple(interp, cdr)) {
			fprintf(fout," ");

			car=car(cdr);
			cdr=cdr(cdr);

			output_stream(interp, car, fout);
		    
			/* if the next element is not the empty list
			   end with . cdr */
			if(!is_empty_list(interp, cdr) && !is_tuple(interp,cdr)) {
			    fprintf(fout," . ");
			    output_stream(interp, cdr, fout);
			}
		    
		    }

		} else {
		    fprintf(fout," . ");
		    output_stream(interp, cdr, fout);
		}
	    }

	    fprintf(fout,")");

	}
	
	break;
    case SYM:
	fprintf(fout,"%s", obj->value.string_val);
	break;
    case PRIM:
	fprintf(fout,"#<primitive procedure:@%p>", (void *)obj);
	break;
    case PORT:
	fprintf(fout,"#<port:@%p in:%i out:%i>", (void *)obj->value.port_val.port,
	       obj->value.port_val.input, obj->value.port_val.output);
	break;
    case CLOSURE:

	/* avoid infinite recursion */
	if(last_closure==obj) {
	    fprintf(fout,"#<closure:current>");
	    return;
	}

	last_closure=obj;

	fprintf(fout,"#<closure: params:");
	output_stream(interp, obj->value.closure.param, fout);
	fprintf(fout," body:");
	output_stream(interp, obj->value.closure.body, fout);
	fprintf(fout,">");
	last_closure=0;
	break;

    default:
	break;
    }
}

/* output to the standard output */
void output(interp_core_type *interp, object_type *obj) {
    output_stream(interp, obj, stdout);
}
