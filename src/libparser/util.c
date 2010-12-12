#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include <execinfo.h>

#include "util.h"


/* Deal with fatal errrors */
void fail(const char *msg) {
    fprintf(stderr,"%s\n", msg);
    assert(0);
}

/* Dump a backtrace */
void cause_bt() {
    void *frames[20];
    size_t size=0;
    
    size=backtrace(frames, 20);

    /* print a stack trace to stderr */
    fprintf(stderr, "Backtrace:");
    backtrace_symbols_fd(frames, size, 2);
}

void sig_segv_handler(int sig) {
    
    fprintf(stderr, "Caught signal %d :\n", sig);
    cause_bt();
    
    /* Dump the core */
    abort();
}


void attach_sig_handlers() {
    signal(SIGSEGV, sig_segv_handler);
}
