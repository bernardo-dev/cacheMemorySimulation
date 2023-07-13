all: main.o cpu.o generator.o instruction.o memory.o mmu.o
	@gcc main.o cpu.o generator.o instruction.o memory.o mmu.o -o exe -lm
	@rm -r main.o cpu.o generator.o instruction.o memory.o mmu.o
main.o: main.c
	@gcc main.c -c 
cpu.o: cpu.c
	@gcc cpu.c -c 
generator.o: generator.c
	@gcc generator.c -c 
instruction.o: instruction.c
	@gcc instruction.c -c 
memory.o: memory.c
	@gcc memory.c -c 
mmu.o: mmu.c
	@gcc mmu.c -c 
run:
	@./exe
