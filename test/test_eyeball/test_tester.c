#include "libtester/test.h"

int succeed_test() {
    return 0;
}

test_case_type cases[]={
    {&succeed_test, "Successful test failed!"},
    {&succeed_test, "Successful test failed!"},
    {&succeed_test, "Successful test failed!"},
    {&succeed_test, "Successful test failed!"},
    {&succeed_test, "Successful test failed!"},
    {&succeed_test, "Successful test failed!"},
    {&succeed_test, "Successful test failed!"},
    {&succeed_test, "Successful test failed!"},
    {&succeed_test, "Successful test failed!"},
    {&succeed_test, "Successful test failed!"},
    {0,0}
};
