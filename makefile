# Diretórios
SRCDIR = src
INCDIR = include

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -I$(INCDIR)

# Fontes e objetos
dir_SRCS = $(wildcard $(SRCDIR)/*.c)
SRCS = $(notdir $(dir_SRCS))
OBJS = $(SRCS:.c=.o)

# Executável
target = main

.PHONY: all clean

all: $(target)

$(target): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Regra genérica para objetos, procurando o .c em src/
%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	
# Executar o programa
run: $(target)
ifeq ($(OS),Windows_NT)
	@.\$(target).exe
else
	@./$(target)
endif


# Limpeza de arquivos gerados
clean:
ifeq ($(OS),Windows_NT)
	@del /Q $(OBJS) $(target).exe 2>nul || @echo "Nenhum arquivo para limpar"
else
	@rm -f $(OBJS) $(target)
endif
