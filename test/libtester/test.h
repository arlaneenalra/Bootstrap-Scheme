#ifndef __TEST__
#define __TEST__

typedef struct test_case {
    int (*test)(); /* the test case to run */
    char *fail_message;
} test_case_type;

extern test_case_type cases[]; /* define with a list of test cases */

#endif
