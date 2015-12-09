#include <stdlib.h>

/* Add profiling and instrumentation code to these functions */

/* preform any initialization here, occurs before resolving the first memory 
 * allocation request in the program run. */
void prof_post_init();

/* size is the amount of memory requested */
void prof_pre_alloc(size_t size);

/* size is the amount of memory requested, 
 * ptr is the pointer about to be returned */
void prof_post_alloc(size_t size, void *ptr);

/* ptr is the pointer about to be freed */
void prof_pre_free(void *ptr);

void prof_post_free();
/* End hooks */


