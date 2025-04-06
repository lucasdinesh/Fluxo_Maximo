# Compilador a ser usado
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -std=c99 -g -O2

# Nome do executável
EXECUTABLE = makegraph

# Arquivos fonte
SOURCES = new_washington.c

# Arquivos objeto (serão gerados)
OBJECTS = $(SOURCES:.c=.o)

# Regra padrão: compilar e linkar para gerar o executável
all: $(EXECUTABLE)

# Regra para criar o executável
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

# Regra para compilar arquivos .c para .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos gerados
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Regra para limpar tudo e recompilar
rebuild: clean all
