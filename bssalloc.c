#include "bssalloc.h"

#include <stdio.h>

void BSS_Alloc_Data_init(BSS_Alloc_Data *d) {
    d->cptr = d->pool;
}

int BSS_Alloc_Data_owns_d(BSS_Alloc_Data *d, void *p) {
    return ((char*) p >= d->pool && (char*) p <= d->cptr);
}

int BSS_Alloc_Data_owns(void *p) {
    return BSS_Alloc_Data_owns_d(&bss_data, p);
}

void *bss_alloc_d(BSS_Alloc_Data *d, size_t size) {
    char *p = NULL;
    size_t nsize = size + ((size % 8) ? 8 - size % 8 : 0);
    
    if (d->cptr + nsize < d->pool + BSS_POOL_SIZE) {
        p = d->cptr;
        d->cptr += nsize;
    }

    return (void*) p;
}

void *bss_alloc(size_t size) {
    if (bss_data.cptr == NULL) {
        BSS_Alloc_Data_init(&bss_data);
    }
    return bss_alloc_d(&bss_data, size);
}

void bss_free_d(BSS_Alloc_Data *d, void *p) {
    
}

void bss_free(void *p) {
    bss_free_d(&bss_data, p);
}
