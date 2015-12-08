#include "bccalloc.h"

void BCC_Alloc_Data_init(BCC_Alloc_Data *d) {
    d->cptr = (void*) d->pool;
}

int BCC_Alloc_Data_owns_d(BCC_Alloc_Data *d, void *p) {
    return (p >= (void*) d->pool && p <= d->cptr);
}

int BCC_Alloc_Data_owns(void *p) {
    return BCC_Alloc_Data_owns_d(&bcc_data, p);
}

void *bcc_alloc_d(BCC_Alloc_Data *d, size_t size) {
    void *p = NULL;
    size_t nsize = size + ((size % 8) ? 8 - size % 8 : 0);
    
    if (d->cptr + nsize < ((void*) d->pool) + BCC_POOL_SIZE) {
        p = d->cptr;
        d->cptr += nsize;
    }
    return p;
}

void *bcc_alloc(size_t size) {
    if (bcc_data.cptr == NULL) {
        BCC_Alloc_Data_init(&bcc_data);
    }
    return bcc_alloc_d(&bcc_data, size);
}
