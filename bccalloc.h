#ifndef BCC_ALLOC_H
#define BCC_ALLOC_H

#define BCC_POOL_SIZE 4096

#include <stdlib.h>

typedef struct {
    char pool[BCC_POOL_SIZE];
    void *cptr;

} BCC_Alloc_Data;

static BCC_Alloc_Data bcc_data;

void BCC_Alloc_Data_init(BCC_Alloc_Data *d);

int BCC_Alloc_Data_owns(BCC_Alloc_Data *d, void *p);

void *bcc_alloc_d(size_t size, BCC_Alloc_Data *d);

void *bcc_alloc(size_t size);

#endif //BCC_ALLOC_H
