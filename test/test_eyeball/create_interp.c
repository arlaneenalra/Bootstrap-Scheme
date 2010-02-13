#include "libtester/test.h"
#include <strings.h>
#include "../src/libparser/core.h"


int create() {
    interp_core_type *interp=0;

    interp=create_interp();
    
    /* make sure that create returned something */
    if(interp==0) {
	return 1;
    }

    cleanup_interp(interp);
    
    return 0;
}


test_case_type cases[]={
    {&create, "Unable to create interpreter"},
    {0,0}
};
