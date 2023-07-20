#ifndef LFU_H
#define LFU_H

#include "memory.h"

void LFUIncrement(Line *);

int LFUlineWhichWillLeave(Cache *);

#endif // LFU_H