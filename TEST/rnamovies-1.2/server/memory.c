#include "memory.h"

void *
resize(void * ptr, size_t size, size_t type) {

  void *tmpptr;

  tmpptr = realloc(ptr, size * type);
  if(tmpptr == NULL)
    dbError(DB_TRUE, "failed to allocate memory");

  return tmpptr;
}

void *
chunk_resize(void * ptr, size_t oldsize, size_t type, size_t * newsize) {

  void *tmpptr;

  dbDebug(DB_DEBUG, "->chunk_resize()");

  if(oldsize<= *newsize) {
    *newsize = (*newsize - oldsize)/2 + *newsize;
    dbDebug(DB_DEBUG, "mem size set to %d", newsize);
    tmpptr = resize(ptr, *newsize, type);
  }
  else
    tmpptr = ptr;

  dbDebug(DB_DEBUG, "<-chunk_resize()");

  return tmpptr;
}
