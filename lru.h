#ifndef LRU_H
#define LRU_H

#include "memory.h"

void LRUIncrement(Cache *);

int LRUlineWhichWillLeave(Cache *);

#endif // LRU_H
