#include "hooks.h"

#include <stdio.h>
#include <string.h>

#include "defs.h"

/* Add profiling and instrumentation code to these functions */
void prof_post_init() {
    /* preform any initialization here, occurs before resolving the first 
     * memory allocation request in the program run. */
    char *message = "Communication works!\n";
    write(MWRAPCOMMFD, message, strlen(message));
}

void prof_pre_alloc(size_t size) {
    /* size is the amount of memory requested */
    //fprintf(stderr, "Allocating %li", size);
}

void prof_post_alloc(size_t size, void *ptr) {
    /* size is the amount of memory requested, 
     * ptr is the pointer about to be returned */
    //fprintf(stderr, " ptr=%p\n", ptr);
}

void prof_pre_free(void *ptr) {
    /* ptr is the pointer about to be freed */
}

void prof_post_free() {
}
/* End hooks */


