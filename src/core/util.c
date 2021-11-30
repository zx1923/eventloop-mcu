#include "util.h"

static int FreeSize = DF_HEAP_SIZE;

el_heap_info_t el_getHeapInfo(void)
{
  el_heap_info_t heapInfo;
  heapInfo.total = DF_HEAP_SIZE;
  heapInfo.free = FreeSize;
  heapInfo.usage = 1 - FreeSize * 1.0 / DF_HEAP_SIZE;
  return heapInfo;;
}

void *el_malloc(size_t n)
{
  void *p = malloc(n);
  if (n <= 8) FreeSize -= 8;
  FreeSize -= (n / 8 * 8) + (n % 8 > 0 ? 8 : 0);
  return p;
}

void el_free(void *p, size_t n)
{
  free(p);
  if (n <= 8) FreeSize += 8;
  FreeSize += (n / 8 * 8) + (n % 8 > 0 ? 8 : 0);
}
