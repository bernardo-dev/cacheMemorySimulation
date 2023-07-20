#ifndef GENERATOR_H
#define GENERATOR_H

#include "cpu.h"

Instruction* generateRandomInstructions(int);
Instruction *generateMultiplicationInstructions(int, int, int *);
Instruction *generateDivisionInstructions(int, int, int *);
Instruction *generateExponentialInstructions(int, int, int*);
Instruction *generateFatorialInstructions(int, int*);
Instruction* readInstructions(char*, int*);

#endif // !GENERATOR_H