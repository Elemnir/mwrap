#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bssalloc.h"
#include "apphooks.h"

static int mwrap_init_status; /* 0: Need Init, 1: Initializing, 2: Initialized */
static int mwrap_in_hook; /* 0: Not called by hook, 1: Called within hook */

static void *(*real_malloc)  (size_t size) = NULL;
static void *(*real_calloc)  (size_t nmemb, size_t size) = NULL;
static void  (*real_free)    (void *ptr) = NULL;
static void *(*real_realloc) (void *ptr, size_t size) = NULL;

static void mwrap_init() {
    mwrap_init_status = 1;
    real_malloc  = dlsym(RTLD_NEXT, "malloc");
    real_calloc  = dlsym(RTLD_NEXT, "calloc");
    real_free    = dlsym(RTLD_NEXT, "free");
    real_realloc = dlsym(RTLD_NEXT, "realloc");
    if (!real_malloc || !real_calloc || !real_free || !real_realloc) {
        fprintf(stderr, "Error in dlsym: %s\n", dlerror());
        exit(1);
    }
    mwrap_init_status = 2;
    mwrap_in_hook = 1;
    prof_post_init();
    mwrap_in_hook = 0;
}

void *malloc(size_t size) {
    void *ptr;
    
    if (mwrap_init_status == 0) {
        mwrap_init();
    } else if (mwrap_in_hook == 1 || mwrap_init_status == 1) {
        return bss_alloc(size);
    }
    
    mwrap_in_hook = 1;
    prof_pre_alloc(size);
    mwrap_in_hook = 0;

    ptr = real_malloc(size);
    
    mwrap_in_hook = 1;
    prof_post_alloc(size, ptr);
    mwrap_in_hook = 0;

    return ptr;
}

void *calloc(size_t nmemb, size_t size) {
    void *ptr;

    if (mwrap_init_status == 0) {
        mwrap_init();
    } else if (mwrap_in_hook || mwrap_init_status == 1) {
        ptr = bss_alloc(nmemb * size);
        if (ptr) {
            memset(ptr, 0, nmemb * size);
        }
        return ptr;
    }
    
    mwrap_in_hook = 1;
    prof_pre_alloc(size);
    mwrap_in_hook = 0;

    ptr = real_calloc(nmemb, size);
    
    mwrap_in_hook = 1;
    prof_post_alloc(size, ptr);
    mwrap_in_hook = 0;
    
    return ptr;
}

void free(void *ptr) {
    if (mwrap_init_status == 0) {
        mwrap_init();
    } 
    
    if (BSS_Alloc_Data_owns(ptr)) {
        return;
    }
    
    prof_pre_free(ptr);
    real_free(ptr);
    prof_post_free();
}

void *realloc(void *ptr, size_t size) {
    void *nptr;

    if (mwrap_init_status == 0) {
        mwrap_init();
    } else if (mwrap_in_hook || mwrap_init_status == 1) {
        nptr = bss_alloc(size);
        if (nptr && ptr) {
            memmove(nptr, ptr, size);
            free(ptr);
        }
        return nptr;
    }

    mwrap_in_hook = 1;
    prof_pre_alloc(size);
    mwrap_in_hook = 0;
    
    nptr = real_realloc(ptr, size);
    
    mwrap_in_hook = 1;
    prof_post_alloc(size, nptr);
    mwrap_in_hook = 0;
    
    return nptr;
}
