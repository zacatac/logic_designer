#ifndef _MEMORY_H_
#define _MEMORY_H_


#include <stdio.h>
#include <stdlib.h>

#include "dberror.h"

#define MEMORYCHUNK 512

void *
resize(void * ptr, size_t size, size_t type);
void *
chunk_resize(void * ptr, size_t oldsize, size_t type, size_t * newsize);

#endif /* _MEMORY_H_ */
