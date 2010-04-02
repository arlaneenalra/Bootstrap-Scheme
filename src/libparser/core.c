#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "util.h"
#include "scheme_funcs.h"

#include "parser_internal.h"

#include "scheme.h"
#include "lexer.h"

void clear_state_stack(interp_core_type *interp);

/* Attach an added object to our graph of objects */
void set(interp_core_type *interp, setting_type_enum setting) {
    object_type *current=0;
    object_type *obj=interp->added;
    
    
    obj=interp->added;
    current=interp->current;

    if(current !=0 && current->type!=TUPLE) {
	fail("Attempting to set on none tuple.");
    }

    switch(setting) {
    case CAR:
	TRACE("Sa");
	if(current==0) {
	    fail("Attempt to set car on null pointer");
	}

	car(current)=obj;
	break;

    case CDR:
	TRACE("Sd");
	if(current==0) {
	    fail("Attempt to set cdr on null pointer");
	}

	cdr(current)=obj;
	break;

    default:
	fail("Invalide setting state!");
    }

}

/* Put the passed object into the added buffer */
void add_object(interp_core_type *interp, object_type *obj) {
    interp->added=obj;
}

/* Quote things objects that have been added */
void add_quote(interp_core_type *interp) {
    object_type *obj=0;
 
    TRACE("Qu");

    obj=quote(interp, interp->added);

    add_object(interp,obj);
    
}

/* Create a character constant object */
void add_char(interp_core_type *interp, char *str) {
    object_type *obj=0;
    
    char c=*str;
    
    if(strcmp(str, "newline")==0) {
	c='\n';

    } else if(strcmp(str, "space")==0) {
	c=' ';

    } else if(*str=='x') { /* Hex encoded charater */
	uint64_t val=strtoul(str+1, 0, 16);
	c=0xff & val;
    }
    
    obj=alloc_object(interp, CHAR);
    obj->value.char_val=c;
    add_object(interp, obj);
}

/* Create a number */
void add_number(interp_core_type *interp, char *str) {
    object_type *obj=0;

    obj=alloc_object(interp, FIXNUM);
    obj->value.int_val=strtoll(str, 0, 10);
    
    add_object(interp, obj);
}

void add_empty_vector(interp_core_type *interp) {
    object_type *obj=0;

    obj=alloc_vector(interp, 0);
    
    interp->added=obj;
}

/* parse and add a vector */
void add_vector(interp_core_type *interp) {
    object_type *obj=0;
    object_type *next=0;
    uint64_t len=0;
    uint64_t i=0;

    /* figure out how long the list is */
    len=list_length(interp, interp->added);

    /* create a new vector */
    obj=alloc_vector(interp, len);
    
    next=interp->added;
    
    while(next!=interp->empty_list) {
	obj->value.vector.vector[i]=car(next);

	next=cdr(next);
	i++;
    }
    
    interp->added=obj;
}

/* create an instance of a string object */
void add_string(interp_core_type *interp, char *str) {
    object_type *obj=0;
    char *c_write=0;
    char *c_read=0;
    char read=0;
    uint64_t val;

    c_write=c_read=str;
    
    /* In place character interpolation */
    while(*c_read!=0) {
	
	read=*c_read;

	/* look for \ character */
	if(read=='\\') {
	    c_read++;
	    
	    switch (*c_read) {
	    case 'a': /* alarm */
		read=0x07;
		break;

	    case 'b': /* backspace */
		read=0x08;
		break;

	    case 't': /* character tab */
		read=0x09;
		break;
		
	    case 'n': /* linefeed */
		read=0x0A;
		break;

	    case 'v': /* vertical tab */
		read=0x0B;
		break;

	    case 'f': /* form feed */
		read=0x0C;
		break;

	    case 'r': /* return */
		read=0x0D;
		break;

	    case '"': /* double quote */
		read='"';
		break;
		
	    case '\\': /* backslash */
		read='\\';
		break;
		
	    case 'x': /* read a numeric character constant */
		c_read++;
		
		/* TODO: this will need to be adapted to
		   unicode */
		val=strtoul(c_read, &c_read, 16);
		read=0xff & val;
		break;
		
	    default:
		read='!';
		interp->error=1;
		break;
	    }
	    
	}
	
	/* store the read character and move
	to next location */
	*c_write=read;
	c_write++;
	c_read++;
    }

    /* add null terminator */
    if(c_write!=c_read) {
	*c_write=0;
    }

    obj=create_string(interp, str);
    add_object(interp,obj);
}

/* create an instance of a string object */
void add_symbol(interp_core_type *interp, char *str) {
    object_type *obj=0;

    TRACE("Sy");

    obj=create_symbol(interp, str);
    
    add_object(interp,obj);
}

/* Create an instance of a floating point number */
void add_float(interp_core_type *interp, char *str) {
    object_type *obj=0;
    char *c=str;
    
    /* check for / */
    while(*c!=0 && *c!='/') {
	c++;
    }
   
    obj=alloc_object(interp, FLOATNUM);

    /* if we found a / then do the division 
       otherwise we convert it as any other real */
    if(*c=='/') {
	long double x,y;
	x=strtold(str,0);
	c++;
	y=strtold(c,0);
	
	/* check for div by 0 */
	if(y==0.0) {
	    interp->error=1;
	    obj->value.float_val=0;
	} else {
	    obj->value.float_val=x/y;
	}
	
    } else {
	obj->value.float_val=strtold(str, 0);
    }

    add_object(interp, obj);
}

/* Add a new state to the current chain of states without
   pushing it onto the state stack */
void chain_state(interp_core_type *interp) {
    object_type *new_state=interp->added;

    TRACE("C")

    /* handle the first list entry correctly */
      if(car(interp->current)==0) {
	/* we don't need to do anythin else here */
	set(interp, CAR);
	return;
    } 

    /* Before throwing current, we 
       need to make sure that it has been added */

    push_state(interp);
    interp->state_stack->type=CHAIN;
    
    add_object(interp, new_state);
    set(interp, CAR);

}

/* Save the current list off so that we can get back to it */
void push_state(interp_core_type *interp) {
    object_type *state=0;
    object_type *new_state=alloc_object(interp, TUPLE);

    TRACE("Pu");

    /* push the current state onto the state stack */
    
    state=cons(interp, interp->current, interp->state_stack);

    interp->state_stack=state;
    
    interp->current=new_state;
    
}

/* Pop a previously saved list */
void pop_state(interp_core_type *interp) {
    object_type *state=0;

    TRACE("Po")
    
    if(interp->state_stack==0) {
	fail("Attempt to pop non-existent state");
    }

    state=interp->state_stack;
    
    interp->added=interp->current;
    interp->current=car(state);
    interp->state_stack=cdr(state);

    /* we are in the depths of a chain, pop until 
       we're out of it */
    if(state->type==CHAIN) {
	set(interp, CDR);
	pop_state(interp);
    }
}

/* Make sure that there are no loose state's sitting on 
   the stack */
void clear_state_stack(interp_core_type *interp) {
    while(interp->state_stack!=0) {
	pop_state(interp);
    }
}

void end_of_file(interp_core_type *interp) {
    interp->running=0;
}

void reset(interp_core_type *interp) {
    interp->current=interp->empty_list;
    clear_state_stack(interp);
    interp->error=0;

    TRACE("RESET\n");
}


/* Parse a file */
object_type *parse(interp_core_type *interp, FILE *in) {
    reset(interp);
    
    yyset_in(in, peek_scanner);
    
    if(parse_internal(interp, peek_scanner)) {
	return 0;
    }

    TRACE("\n")
    
    return interp->added;
}

/* Parse a string */
object_type *parse_string(interp_core_type *interp, char *in) {
    reset(interp);
    
    yy_scan_string(in, peek_scanner);
    
    if(parse_internal(interp, peek_scanner)) {
	return 0;
    }

    TRACE("\n")
    
    return interp->added;
}

/* Parse used by load */
object_type *parse_chain(interp_core_type *interp) {
    reset(interp);
    

    if(parse_internal(interp, peek_scanner)) {
	return 0;
    }

    return interp->added;
}

void push_parse_state(interp_core_type *interp, FILE *fin) {
    
    scanner_stack_type *scanner=alloc_scanner();

    yylex_init_extra(interp, &(scanner->scanner));
    yyset_in(fin, scanner->scanner);

    scanner->previous=interp->scanner;
    interp->scanner=scanner;
}

void pop_parse_state(interp_core_type *interp) {

    yylex_destroy(peek_scanner);
    interp->scanner=peek_previous;
}

void create_parser(interp_core_type *interp) {
    
    interp->scanner=alloc_scanner();
    
    yylex_init_extra(interp, &(peek_scanner));
}
