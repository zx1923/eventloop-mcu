#include "util.h"
#include "emiter.h"

static int FreeSize = DF_HEAP_SIZE;
static el_heap_map_t HeapMap = {0, 0};

void _heapPush(void *p, size_t n)
{
  HeapMap.p[HeapMap.wp] = p;
  HeapMap.size[HeapMap.wp++] = n;
  if (HeapMap.wp >= DF_HEAP_BUF_LEN)
  {
    HeapMap.wp = 0;
  }
}

uint16_t _heapFind(void *p)
{
  uint16_t size = 0;
  for (uint16_t i = 0; i < DF_HEAP_BUF_LEN; i++)
  {
    if (p == HeapMap.p[i])
    {
      size = HeapMap.size[i];
      HeapMap.p[i] = NULL;
      HeapMap.size[i] = 0;
      return size;
    }
  }
  return 0;
}

el_heap_info_t el_getHeapInfo(void)
{
  el_heap_info_t heapInfo;
  heapInfo.total = DF_HEAP_SIZE;
  heapInfo.free = FreeSize;
  heapInfo.usage = FreeSize <= 0 ? 1.0 : (1 - FreeSize * 1.0 / DF_HEAP_SIZE);
  return heapInfo;
}

void *el_malloc(size_t n)
{
  uint16_t realSize = 0;
  void *p = malloc(n);
  if (p == NULL)
  {
    el_emitEvent(EVENT_HEAP_INVALID, NULL);
    return NULL;
  }
  realSize = (n / 8 * 8) + 8;
  FreeSize -= realSize;
  _heapPush(p, realSize);
  return p;
}

void el_free(void *p)
{
  FreeSize += _heapFind(p);
  free(p);
}
