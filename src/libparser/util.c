#include <stdio.h>
#include <assert.h>
#include "util.h"

/* Deal with fatal errrors */
void fail(const char *msg) {
    fprintf(stderr,"%s\n", msg);
    assert(0);
}
