#include "libtester/test.h"
#include <strings.h>

int succeed_test() {
    return 0;
}

int fail_test() {
    return 1;
}

int malloc_fail() {
    malloc(1000000);
    return 0;
}

test_case_type cases[]={
    {&succeed_test, "Successful test failed!"},
    {&succeed_test, "Successful test failed!"},
    {&succeed_test, "Successful test failed!"},
    {&fail_test, "Expected Failure!"},
    {&succeed_test, "Successful test failed!"},
    {&fail_test, "Expected Failure!"},
    {&succeed_test, "Successful test failed!"},
    {&malloc_fail, "malloc without free!"},
    {&succeed_test, "Successful test failed!"},
    {&succeed_test, "Successful test failed!"},
    {0,0}
};
