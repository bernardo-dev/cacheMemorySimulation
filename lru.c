#include "lru.h"
#include "constants.h"
#include "memory.h"
#include "mmu.h"

void LRUIncrement(Cache *cache) {
  // Incrementa todas as posicoes
  // das linhas da cache
  for (int i = 0; i < cache->size; i++)
    if (cache->lines[i].tag != INVALID_ADD)
      cache->lines[i].queuePlace += 1;
}

int LRUlineWhichWillLeave(Cache *cache) {
  // Retorna a tag de uma linha da cache que esta
  // vazia ou um linha que esta a mais tempo
  // na cache
  int greater = 0;
  int tag;
  for (int i = 0; i < cache->size; i++) {
    if (cache->lines[i].tag == INVALID_ADD){
      return i;
    }
  }
  for (int i = 0; i < cache->size; i++) {
    if (cache->lines[i].tag != INVALID_ADD && !(cache->lines[i].updated)) {
      return i;
    }
  }
  for (int i = 0; i < cache->size; i++){
    if (cache->lines[i].queuePlace >= greater){
      greater = cache->lines[i].queuePlace;
      tag = cache->lines[i].tag;
    }
  }
  return tag;
}
