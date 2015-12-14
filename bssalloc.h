#ifndef BSS_ALLOC_H
#define BSS_ALLOC_H

#define BSS_POOL_SIZE 1024 * 1024

#include <stdlib.h>

typedef struct {
    char pool[BSS_POOL_SIZE];
    char *cptr;

} BSS_Alloc_Data;

static BSS_Alloc_Data bss_data;

void BSS_Alloc_Data_init(BSS_Alloc_Data *d);

int BSS_Alloc_Data_owns_d(BSS_Alloc_Data *d, void *p);
int BSS_Alloc_Data_owns(void *p);

void *bss_alloc_d(BSS_Alloc_Data *d, size_t size);
void *bss_alloc(size_t size);

void bss_free_d(BSS_Alloc_Data *d, void *p);
void bss_free(void *p);

#endif //BSS_ALLOC_H
