#include <stdio.h>
#include <stdlib.h>

#include <test.h>


int main(int argv, char** argc) {
    int count=0;
    int failed=0;

    for(count=0;cases[count].test!=0;count++) {

	printf("%s:", cases[count].message);

	if((*cases[count].test)()) {
	    printf("FAILED\n");
	    failed++;
	} else {
	    printf("PASSED\n");
	}
    }

    printf("================\n");
    printf("Ran %i tests, %i failed.\n", count, failed);

    if(failed) {
	return 1;
    }

    return 0;
}
