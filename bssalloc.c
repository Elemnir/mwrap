#include "bssalloc.h"

void BSS_Alloc_Data_init(BSS_Alloc_Data *d) {
    d->cptr = (void*) d->pool;
}

int BSS_Alloc_Data_owns_d(BSS_Alloc_Data *d, void *p) {
    return (p >= (void*) d->pool && p <= d->cptr);
}

int BSS_Alloc_Data_owns(void *p) {
    return BSS_Alloc_Data_owns_d(&bss_data, p);
}

void *bss_alloc_d(BSS_Alloc_Data *d, size_t size) {
    void *p = NULL;
    size_t nsize = size + ((size % 8) ? 8 - size % 8 : 0);
    
    if (d->cptr + nsize < ((void*) d->pool) + BSS_POOL_SIZE) {
        p = d->cptr;
        d->cptr += nsize;
    }
    return p;
}

void *bss_alloc(size_t size) {
    if (bss_data.cptr == NULL) {
        BSS_Alloc_Data_init(&bss_data);
    }
    return bss_alloc_d(&bss_data, size);
}
