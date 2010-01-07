#include <stdlib.h>
#include <strings.h>
#include <assert.h>

#include "libparser/core.h"


int main(int argc, char **argv) {


    interp_core_type * interp=create_interp();

    printf("Simple Bootstrapper\n");
    printf("sizeof(object_type) %li\n", sizeof(object_type));

    yyset_in(stdin, interp->scanner);
    yyparse(interp->scanner);
    
    printf(">\n");

    cleanup_interp(interp);

    exit(0);
}
