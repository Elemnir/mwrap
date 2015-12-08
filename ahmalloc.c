#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bccalloc.h"

/* Add profiling and instrumentation code to these functions */
void prof_pre_alloc(size_t size) {
    /* size is the amount of memory requested */
    fprintf(stderr, "Allocating %li", size);
}

void prof_post_alloc(size_t size, void *ptr) {
    /* size is the amount of memory requested, 
     * ptr is the pointer about to be returned */
    fprintf(stderr, " ptr=%p\n", ptr);
}

void prof_pre_free(void *ptr) {
    /* ptr is the pointer about to be freed */
}

void prof_post_free() {
}
/* End hooks */

static int ah_init_status; /* 0: Need Init, 1: Initializing, 2: Initialized */

static void *(*real_malloc)  (size_t size) = NULL;
static void *(*real_calloc)  (size_t nmemb, size_t size) = NULL;
static void  (*real_free)    (void *ptr) = NULL;
static void *(*real_realloc) (void *ptr, size_t size) = NULL;

static void ah_init() {
    ah_init_status = 1;
    real_malloc  = dlsym(RTLD_NEXT, "malloc");
    real_calloc  = dlsym(RTLD_NEXT, "calloc");
    real_free    = dlsym(RTLD_NEXT, "free");
    real_realloc = dlsym(RTLD_NEXT, "realloc");
    if (!real_malloc || !real_calloc || !real_free || !real_realloc) {
        fprintf(stderr, "Error in dlsym: %s\n", dlerror());
        exit(1);
    }
    ah_init_status = 2;
}

void *malloc(size_t size) {
    void *ptr;
    
    if (ah_init_status == 0) {
        ah_init();
    } else if (ah_init_status == 1) {
        return bcc_alloc(size);
    }
    
    prof_pre_alloc(size);
    ptr = real_malloc(size);
    prof_post_alloc(size, ptr);
    return ptr;
}

void *calloc(size_t nmemb, size_t size) {
    void *ptr;

    if (ah_init_status == 0) {
        ah_init();
    } else if (ah_init_status == 1) {
        ptr = bcc_alloc(nmemb * size);
        if (ptr) {
            memset(ptr, 0, nmemb * size);
        }
        return ptr;
    }
    
    prof_pre_alloc(size);
    ptr = real_calloc(nmemb, size);
    prof_post_alloc(size, ptr);
    return ptr;
}

void free(void *ptr) {
    if (ah_init_status == 0) {
        ah_init();
    } 
    
    if (ah_init_status == 1 || BCC_Alloc_Data_owns(ptr)) {
        return;
    }
    
    prof_pre_free(ptr);
    real_free(ptr);
    prof_post_free();
}

void *realloc(void *ptr, size_t size) {
    void *nptr;

    if (ah_init_status == 0) {
        ah_init();
    } else if (ah_init_status == 1) {
        nptr = bcc_alloc(size);
        if (nptr && ptr) {
            memmove(nptr, ptr, size);
            free(ptr);
        }
        return nptr;
    }

    prof_pre_alloc(size);
    nptr = real_realloc(ptr, size);
    prof_post_alloc(size, nptr);
    return nptr;
}
