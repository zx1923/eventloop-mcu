#ifndef __USER_EL_UTIL_H__
#define __USER_EL_UTIL_H__

#include "sys_global.h"
#include <string.h>

typedef struct
{
  el_pointer_t total;
  el_pointer_t free;
  float usage;
} el_heap_info_t;

el_heap_info_t el_getHeapInfo(void);
void *el_malloc(size_t n);
void el_free(void *, size_t n);

#endif
