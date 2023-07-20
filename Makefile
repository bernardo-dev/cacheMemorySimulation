# Variáveis para os arquivos fonte e de objetos
SRCS := main.c cpu.c generator.c instruction.c memory.c mmu.c lru.c lfu.c
OBJS := $(SRCS:.c=.o)

# Nome do executável
EXEC := exe

# Flags do compilador
CFLAGS := -Wall -Wextra -std=c99
LDFLAGS := -lm

# Regra principal: construir o executável
all: $(EXEC)

# Regra implícita para compilar arquivos .c em objetos .o
%.o: %.c
	@gcc $(CFLAGS) -c $<

# Regra para o executável
$(EXEC): $(OBJS)
	@gcc $(OBJS) -o $(EXEC) $(LDFLAGS)
	@rm -f $(OBJS)

valgrind: $(OBJS)
	@gcc $(OBJS) -o $(EXEC) $(LDFLAGS) -g

# Regra para executar o programa
run:
	@./$(EXEC)

# Regra para limpar os arquivos gerados pela compilação
clean:
	@rm -f $(OBJS) $(EXEC)
