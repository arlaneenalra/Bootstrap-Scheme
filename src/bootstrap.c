#include <stdlib.h>
#include <stdio.h>

#include "core.h"
#include "util.h"

int main(int argc, char **argv) {
    interp_core_type * interp=0;
    
    attach_sig_handlers();

    //GC_INIT();

    interp=create_interp();
    
    if(argc==1) {
	repl(interp, 0); /* c repl */
    } else if(argc==2 && argv[1][0]=='s') {
        repl(interp, 1); /* scheme repl */
    } else {
	top_level_program(interp, argv[1]);
    }

    return 0;
}
