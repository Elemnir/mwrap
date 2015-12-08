#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bccalloc.h"

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
        fprintf(stderr, "bcc_alloc(%li)", size);
        return bcc_alloc(size);
    }
    
    fprintf(stderr, "malloc(%li)", size);
    ptr = real_malloc(size);
    fprintf(stderr, "= %p\n", ptr);
    return ptr;
}

void *calloc(size_t nmemb, size_t size) {
    void *ptr;

    if (ah_init_status == 0) {
        ah_init();
    } else if (ah_init_status == 1) {
        fprintf(stderr, "bcc_alloc(%li)", size);
        ptr = bcc_alloc(nmemb * size);
        if (ptr) {
            memset(ptr, 0, nmemb * size);
        }
        return ptr;
    }
    
    fprintf(stderr, "calloc(%li)", size);
    ptr = real_calloc(nmemb, size);
    fprintf(stderr, "= %p\n", ptr);
    return ptr;
}

void free(void *ptr) {
    if (ah_init_status == 0) {
        ah_init();
    } 
    
    if (ah_init_status == 1 || BCC_Alloc_Data_owns(ptr)) {
        return;
    }
    
    real_free(ptr);
}

void *realloc(void *ptr, size_t size) {
    void *nptr;

    if (ah_init_status == 0) {
        ah_init();
    } else if (ah_init_status == 1) {
        fprintf(stderr, "bcc_alloc(%li)", size);
        nptr = bcc_alloc(size);
        if (nptr && ptr) {
            memmove(nptr, ptr, size);
            free(ptr);
        }
        return nptr;
    }

    nptr = real_realloc(ptr, size);
    return nptr;
}
