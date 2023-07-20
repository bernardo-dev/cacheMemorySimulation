#include "lfu.h"
#include "constants.h"
#include "memory.h"
#include "mmu.h"
// #include <limits.h>

void LFUIncrement(Line *line) {
  // Incrementa a queuePlace da linha
  // da cache
  line->queuePlace++;
}

int LFUlineWhichWillLeave(Cache *cache) {
  // Retorna a tag de uma linha da cache que
  // foi usada menos vezes
  for (int i = 0; i < cache->size; i++) {
    if (cache->lines[i].tag == INVALID_ADD) {
      return i;
    }
  }
  for (int i = 0; i < cache->size; i++) {
    if (cache->lines[i].tag != INVALID_ADD && !(cache->lines[i].updated)) {
      return i;
    }
  }

  int smaller = 1000;
  int tag = 0;

  for (int i = 0; i < cache->size; i++) {
    if (cache->lines[i].queuePlace <= smaller) {
      smaller = cache->lines[i].queuePlace;
      tag = cache->lines[i].tag;
    }
  }
  return tag;
}