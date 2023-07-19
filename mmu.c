#include "mmu.h"

#include <stdbool.h>
#include <stdio.h>

char *convertToString(WhereWasHit whereWasHit) {
  switch (whereWasHit) {
  case L1Hit:
    return "CL1";
  case L2Hit:
    return "CL2";
  case L3Hit:
    return "CL3";
  case RAMHit:
    return "RAM";
  default:
    return "ERR MMU";
  }
}

Line *MMUSearchOnMemorys(Address add, Machine *machine,
                         WhereWasHit *whereWasHit) {
  // Strategy => write back

  // Direct memory map
  int l1pos = lineWhichWillLeave(add.block, &machine->l1);
  int l2pos = lineWhichWillLeave(add.block, &machine->l2);
  int l3pos = lineWhichWillLeave(add.block, &machine->l3);

  Line *cache1 = machine->l1.lines;
  Line *cache2 = machine->l2.lines;
  Line *cache3 = machine->l3.lines;

  MemoryBlock *RAM = machine->ram.blocks;

  int cost = 0;

  if (isOnCache(add.block, &machine->l1)) {
    /* Block is in memory cache L1 */
    cost = COST_ACCESS_L1;
    *whereWasHit = L1Hit;
  } else if (isOnCache(add.block, &machine->l2)) {
    /* Block is in memory cache L2 */
    // cache2[l2pos].tag = add.block;
    cost = COST_ACCESS_L1 + COST_ACCESS_L2;
    *whereWasHit = L2Hit;
    // Just works for Direct Mapping
    {
      Line tmp = cache1[l1pos];
      cache1[l1pos] = cache2[l2pos];

      int newL2pos = lineWhichWillLeave(tmp.tag, &machine->l2);
      int newL3pos = lineWhichWillLeave(tmp.tag, &machine->l3);

      if (!canOnlyReplaceBlock(cache2[newL2pos])) {
        if (!canOnlyReplaceBlock(cache3[newL3pos])) {
          RAM[cache3[newL3pos].tag] = cache3[newL3pos].block;
        }
        cache3[newL3pos] = cache2[newL2pos];
      }
      cache2[newL2pos] = tmp;
    }
  } else if (isOnCache(add.block, &machine->l3)) {
    /* Block is memory cache L3 */
    // cache3[l3pos].tag = add.block;
    cost = COST_ACCESS_L1 + COST_ACCESS_L2 + COST_ACCESS_L3;
    *whereWasHit = L3Hit;
    // Just works for Direct Mapping
    {
      Line tmp = cache2[l2pos];
      cache2[l2pos] = cache3[l3pos];

      int newL1pos = lineWhichWillLeave(tmp.tag, &machine->l1);
      int newL2pos = lineWhichWillLeave(tmp.tag, &machine->l2);
      int newL3pos = lineWhichWillLeave(tmp.tag, &machine->l3);

      if (!canOnlyReplaceBlock(cache3[newL3pos])) {
        RAM[cache3[newL3pos].tag] = cache3[newL3pos].block;
      }
      cache3[newL3pos] = tmp;
    }
  } else {
    /* Block only in memory RAM, need to bring it to cache and manipulate the
     * blocks */
    int newL2pos = lineWhichWillLeave(
        cache1[l1pos].tag, &machine->l2); /* Need to check the position of the
                                             block that will leave the L1 */
    int newL3pos = lineWhichWillLeave(cache2[l2pos].tag, &machine->l3);

    if (!canOnlyReplaceBlock(cache1[l1pos])) {
      /* The block on cache L1 cannot only be replaced, the memories must be
       * updated */
      if (!canOnlyReplaceBlock(cache2[l2pos])) {
        /* The block on cache L2 cannot only be replaced, the memories must be
         * updated */
        if (!canOnlyReplaceBlock(cache3[l3pos])) {
          /* The block on cache L2 cannot only be replaced, the memories must be
           * updated */
          RAM[cache3[newL3pos].tag] = cache3[newL3pos].block;
        }
        cache3[newL3pos] = cache2[l2pos];
      }
      cache2[newL2pos] = cache1[l1pos];
    }
    cache1[l1pos].block = RAM[add.block];
    cache1[l1pos].tag = add.block;
    cache1[l1pos].updated = false;
    cost = COST_ACCESS_L1 + COST_ACCESS_L2 + COST_ACCESS_L3 + COST_ACCESS_RAM;
    *whereWasHit = RAMHit;
  }
  updateMachineInfos(machine, whereWasHit, cost);
  return &(cache1[l1pos]);
}

bool isOnCache(int address, Cache *cache) {
  for (int i = 0; i < cache->size; i++) {
    if (cache->lines[i].tag == address) {
      return true;
    }
  }
  return false;
}

bool canOnlyReplaceBlock(Line line) {
  // Or the block is empty or
  // the block is equal to the one in memory RAM and can be replaced
  if (line.tag == INVALID_ADD || (line.tag != INVALID_ADD && !line.updated))
    return true;
  return false;
}

int memoryCacheMapping(int address, Cache *cache) {
  return address % cache->size;
}

int lineWhichWillLeave(int address, Cache *cache) {
  // return address % cache->size;
  for (int i = 0; i < cache->size; i++) {
    if (cache->lines[i].tag == INVALID_ADD ||
        (cache->lines[i].tag && !cache->lines[i].updated)) {
      return i;
    }
  }
  return 0;
}

void updateMachineInfos(Machine *machine, WhereWasHit *whereWasHit, int cost) {
  switch (*whereWasHit) {
  case L1Hit:
    machine->hitL1 += 1;
    break;

  case L2Hit:
    machine->hitL2 += 1;
    machine->missL1 += 1;
    break;

  case L3Hit:
    machine->hitL3 += 1;
    machine->missL1 += 1;
    machine->missL2 += 1;
    break;

  case RAMHit:
    machine->hitRAM += 1;
    machine->missL1 += 1;
    machine->missL2 += 1;
    machine->missL3 += 1;
    break;
  }
  machine->totalCost += cost;
}