#ifndef MMU_H
#define MMU_H

#include "memory.h"
#include "instruction.h"
#include "cpu.h"

typedef enum {
    L1Hit = 1,
    L2Hit,
    L3Hit,
    RAMHit
} WhereWasHit;

char* convertToString(WhereWasHit);

Line* MMUSearchOnMemorys(Address, Machine*, WhereWasHit*);

bool canOnlyReplaceBlock(Line);

int memoryCacheMapping(int, Cache *);

int lineWhichWillLeave(int, Cache *);

void updateMachineInfos(Machine *, WhereWasHit *, int);

bool isOnCache(int, Cache *);

void moveLine();

#endif // !MMU_H