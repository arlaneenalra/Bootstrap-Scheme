#include <stdlib.h>
#include <stdio.h>
#include <gc.h>

#include "core.h"


int main(int argc, char **argv) {

    
    GC_INIT();

    interp_core_type * interp=create_interp();
    
    if(argc==1) {
	repl(interp);
    } else {
	top_level_program(interp, argv[1]);
    }

    return 0;
}
