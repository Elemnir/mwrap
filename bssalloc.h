#ifndef BSS_ALLOC_H
#define BSS_ALLOC_H

#define BSS_POOL_SIZE 4096

#include <stdlib.h>

typedef struct {
    char pool[BSS_POOL_SIZE];
    void *cptr;

} BSS_Alloc_Data;

static BSS_Alloc_Data bss_data;

void BSS_Alloc_Data_init(BSS_Alloc_Data *d);

int BSS_Alloc_Data_owns_d(BSS_Alloc_Data *d, void *p);
int BSS_Alloc_Data_owns(void *p);

void *bss_alloc_d(BSS_Alloc_Data *d, size_t size);
void *bss_alloc(size_t size);

#endif //BSS_ALLOC_H
