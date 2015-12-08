#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "bccalloc.h"

static void *(*real_malloc)  (size_t size) = NULL;
static void *(*real_calloc)  (size_t nmemb, size_t size) = NULL;
static void  (*real_free)    (size_t) = NULL;
static void *(*real_realloc) (void *ptr, size_t size) = NULL;


static void ah_init() {
    real_malloc = dlsym(RTLD_NEXT, "malloc");
    if (real_malloc = NULL) {
        fprintf(stderr, "Error in dlsym: %s\n", dlerror());
    }
}

void *malloc(size_t size) {
    if (real_malloc == NULL) {
        ah_init();
    }

    fprintf(stderr, "malloc(%li)", size);
    void *p = real_malloc(size);
    fprintf(stderr, "= %p\n", p);
    return p;
}

