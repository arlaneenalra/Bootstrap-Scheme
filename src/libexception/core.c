#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "core.h"
#include "scheme_funcs.h"
#include "util.h"

object_type *prim_plug_it(interp_core_type *interp, object_type *args) {
    printf("\nTesting!\n");

    return true;
}

/* primitives */
binding_type primitive_list[]={
    {"plug-it", &prim_plug_it, 1, 1}
};
