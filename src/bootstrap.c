#include <stdlib.h>
#include <stdio.h>
#include <gc.h>

#include "core.h"


int main(int argc, char **argv) {

    
    GC_INIT();

    interp_core_type * interp=create_interp();
    
    repl(interp);

    return 0;
}
