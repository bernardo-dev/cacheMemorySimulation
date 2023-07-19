#include "lru.h"
#include "memory.h"
#include "mmu.h"
#include "constants.h"

void LRUIncrement(Cache *cache) {
  for (int i = 0; i < cache->size; i++)
    if (cache->lines[i].tag != INVALID_ADD)
      cache->lines[i].queuePlace += 1;
}

int LRUlineWhichWillLeave(Cache *cache) {
  for (int i = 0; i < cache->size; i++) {
    if (cache->lines[i].tag != INVALID_ADD)
      
  }
}
