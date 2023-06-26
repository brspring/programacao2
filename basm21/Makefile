CC = gcc

SRC_DIR = src
BIN_DIR = .

# arquivos fonte
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

# nome do executavel
EXEC = $(BIN_DIR)/vina++

# regra principal
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $^ -o $@

# compilar os arquivos fonte
$(OBJS): %.o: %.c
	$(CC) -c $< -o $@

# limpa os arquivos temporários e gerados
clean:
	rm -f $(OBJS) $(EXEC)