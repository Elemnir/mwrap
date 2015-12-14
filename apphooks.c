#include "apphooks.h"

#include <execinfo.h>
#include <stdio.h>
#include <string.h>

#include "defs.h"

/* Add profiling and instrumentation code to these functions */
FILE *cfp;

void prof_post_init() {
    /* preform any initialization here, occurs before resolving the first 
     * memory allocation request in the program run. */
    cfp = fdopen(MWRAPCOMMFD, "w");
}

void prof_pre_alloc(size_t size) {
    /* size is the amount of memory requested */
    int bt_size;
    void *bt_buf[5];
    fprintf(cfp, "AllocStart\nSize: %li\n", size);
    fflush(cfp);
    
    bt_size = backtrace(bt_buf, 5);
    backtrace_symbols_fd(bt_buf, bt_size, MWRAPCOMMFD);
}

void prof_post_alloc(size_t size, void *ptr) {
    /* size is the amount of memory requested, 
     * ptr is the pointer about to be returned */
    fprintf(cfp, "Ptr: %p\nAllocEnd\n", ptr);
    fflush(cfp);
}

void prof_pre_free(void *ptr) {
    /* ptr is the pointer about to be freed */
}

void prof_post_free() {
}
