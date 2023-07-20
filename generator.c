#include "generator.h"
#include <stdio.h>
#include <stdlib.h>

Instruction* generateRandomInstructions(int ramSize) {
    // 01|22|13|45 => this is an instruction
    // 02|33|12|01 => this is another instruction
            
    // 0 => opcode => move
    // 1 => opcode => sum
    // 2 => opcode => subtract
    //-1 => halt
    
    int n = 10;

    Instruction* instructions = (Instruction*) malloc((n + 1) * sizeof(Instruction));

    for (int i=0; i<n; i++) {
        instructions[i].add1.block = rand() % ramSize;
        instructions[i].add1.word = rand() % WORDS_SIZE;
        instructions[i].add2.block = rand() % ramSize;
        instructions[i].add2.word = rand() % WORDS_SIZE;
        instructions[i].add3.block = rand() % ramSize;
        instructions[i].add3.word = rand() % WORDS_SIZE;
        instructions[i].opcode = rand() % 3;
    }
    instructions[n].opcode = -1;
    
    return instructions;
}

Instruction *generateMultiplicationInstructions(int add1, int add2,
                                                int *ramSize) {

  // Identifica o menor numero para uma lista de instrucoes menor
  int maior, menor;

  *ramSize = 2;

  if (add1 > add2) {
    maior = add1;
    menor = add2;
  } else {
    maior = add2;
    menor = add1;
  }

  if (menor == 0) {
    int aux = menor;
    menor = maior;
    maior = aux;
  }

  Instruction *instructions = (Instruction *)malloc(
      sizeof(Instruction) *
      (menor + 2)); // 2 instrucoes MOV + menor - 1 + instrucao de saida

  // Duas instrucoes de mover
  int i;
  for (i = 0; i < 2; i++) {
    instructions[i].opcode = 0;
    instructions[i].add1.block = maior;
    instructions[i].add2.block = i;
  }

  // Somas consecutivas
  for (; i <= menor; i++) {
    instructions[i].opcode = 1;
    instructions[i].add1.block = 0;
    instructions[i].add2.block = 1;
    instructions[i].add3.block = 1;
  }

  // Encerra
  instructions[i].opcode = -1;
  instructions[i].add1.block = -1;
  instructions[i].add2.block = -1;
  instructions[i].add3.block = -1;

  return instructions;
}

Instruction *generateDivisionInstructions(int add1, int add2, int *ramSize) {
  // Divide apenas numeros inteiros maiores que zero
  int dividendo = add1;
  int divisor = add2;

  *ramSize = 4;

  int quociente = dividendo / divisor;

  Instruction *instructions =
      (Instruction *)malloc(sizeof(Instruction) * ((quociente * 2) + 5));

  // Move
  instructions[0].opcode = 0;
  instructions[0].add1.block = dividendo;
  instructions[0].add2.block = 0;

  // Move
  instructions[1].opcode = 0;
  instructions[1].add1.block = divisor;
  instructions[1].add2.block = 1;

  // Move //resultado da divisao aqui
  instructions[2].opcode = 0;
  instructions[2].add1.block = 0;
  instructions[2].add2.block = 2;

  // para somar + 1
  instructions[3].opcode = 0;
  instructions[3].add1.block = 1;
  instructions[3].add2.block = 3;

  int i;
  for (i = 4; dividendo >= divisor; i += 2, dividendo -= divisor) {
    instructions[i].opcode = 2;
    instructions[i].add1.block = 0;
    instructions[i].add2.block = 1;
    instructions[i].add3.block = 0;
    instructions[i + 1].opcode = 1;
    instructions[i + 1].add1.block = 2;
    instructions[i + 1].add2.block = 3;
    instructions[i + 1].add3.block = 2;
  }

  // Encerra
  instructions[i].opcode = -1;
  instructions[i].add1.block = -1;
  instructions[i].add2.block = -1;
  instructions[i].add3.block = -1;

  return instructions;
}

Instruction *generateExponentialInstructions(int base, int exponencial,
                                             int *ramSize) {
  *ramSize = 2;

  Instruction *instructions = (Instruction *)malloc(
      sizeof(Instruction) *
      (((exponencial - 1) * base) + 3 + (2 * (exponencial - 1))));
  // 3 *

  // Move
  instructions[0].opcode = 0;
  instructions[0].add1.block = 0;
  instructions[0].add2.block = 0;

  // Move
  instructions[1].opcode = 0;
  instructions[1].add1.block = base;
  instructions[1].add2.block = 1;

  int k = 2;
  for (int j = 0; j < exponencial - 1; j++) {
    // Multiplicacao
    for (int i = 0; i < base; i++, k++) {
      instructions[k].opcode = 1;
      instructions[k].add1.block = 0;
      instructions[k].add2.block = 1;
      instructions[k].add3.block = 0;
    }
    // Move informacao
    instructions[k].opcode = 3;
    instructions[k].add1.block = 0;
    instructions[k].add2.block = 1;
    k++;
    // Move
    instructions[k].opcode = 0;
    instructions[k].add1.block = 0;
    instructions[k].add2.block = 0;
    k++;
  }

  // Encerra
  instructions[k].opcode = -1;
  instructions[k].add1.block = -1;
  instructions[k].add2.block = -1;
  instructions[k].add3.block = -1;

  return instructions;
}

Instruction *generateFatorialInstructions(int numero, int *ramSize) {

  if (numero == 0 || numero == 1 || numero < 0) {
    *ramSize = 1;
    Instruction *instructions = (Instruction *)malloc(sizeof(Instruction) * 2);

    instructions[0].opcode = 0;
    instructions[0].add1.block = 1;
    instructions[0].add2.block = 0;

    instructions[1].opcode = -1;
    instructions[1].add1.block = -1;
    instructions[1].add2.block = -1;
    instructions[1].add3.block = -1;

    return instructions;
  }

  *ramSize = 2;

  int instructionSize = ((numero - 1) * numero / 2) + (2 * numero) + 1;

  Instruction *instructions =
      (Instruction *)malloc(sizeof(Instruction) * instructionSize);
  int k = 0;
  instructions[k].opcode = 0;
  instructions[k].add1.block = numero;
  instructions[k].add2.block = 0;
  k++;

  // Zera o endereco 2 para o resulado
  instructions[k].opcode = 0;
  instructions[k].add1.block = 0;
  instructions[k].add2.block = 1;
  k++;

  // Subtrai 1 do endereço 1 e salva no 1
  // instructions[k].opcode = 2;
  // instructions[k].add1.block = 1;
  // instructions[k].add2.block = 2;
  // instructions[k].add3.block = 1;
  // k++;
  // 5!
  // n = #0 //5
  // resultado = 0//#1
  numero--;
  int multiplicando = numero;
  for (int i = 0; i < numero; i++) {
    for (int j = 0; j < multiplicando; j++) {
      instructions[k].opcode = 1;
      instructions[k].add1.block = 0;
      instructions[k].add2.block = 1;
      instructions[k].add3.block = 1;
      k++;
    }
    // Move o resultado da multiplicacao
    instructions[k].opcode = 3;
    instructions[k].add1.block = 1;
    instructions[k].add2.block = 0;
    k++;
    // Zera o endereco do resultado
    instructions[k].opcode = 0;
    instructions[k].add1.block = 0;
    instructions[k].add2.block = 1;
    k++;
    multiplicando--;
  }

  instructions[k].opcode = -1;
  instructions[k].add1.block = -1;
  instructions[k].add2.block = -1;
  instructions[k].add3.block = -1;

  return instructions;
}

Instruction *readInstructions(char *fileName, int *ramSize) {
  printf("FILE -> %s\n", fileName);
  FILE *file = fopen(fileName, "r"); // Abrindo arquivo no modo leitura

  if (file == NULL) {
    printf("Arquivo nao pode ser aberto.\n");
    exit(1);
  }

  int n, i = 0;
  fscanf(file, "%d %d", ramSize, &n);
  Instruction *instructions = (Instruction *)malloc(n * sizeof(Instruction));
  while (i < n) {
    fscanf(file, "%d %d %d %d", &instructions[i].opcode, &instructions[i].add1.block,
           &instructions[i].add2.block, &instructions[i].add3.block);
    i++;
  }
  fclose(file); // Fechando o arquivo

  return instructions;
}
