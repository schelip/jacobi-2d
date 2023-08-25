CC = gcc
CFLAGS = -Wall -Wextra -pthread
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = .

SRC_POLYBENCH = polybench.c
SRC_SERIAL = jacobi-2d-serial.c $(SRC_POLYBENCH)
SRC_PARALLEL = jacobi-2d-parallel.c $(SRC_POLYBENCH)
SRC_MAIN = main.c

OBJ_POLYBENCH = $(addprefix $(OBJ_DIR)/,$(notdir $(SRC_POLYBENCH:.c=.o)))
OBJ_SERIAL = $(addprefix $(OBJ_DIR)/,$(notdir $(SRC_SERIAL:.c=.o)))
OBJ_PARALLEL = $(addprefix $(OBJ_DIR)/,$(notdir $(SRC_PARALLEL:.c=.o)))
OBJ_MAIN = $(addprefix $(OBJ_DIR)/,$(notdir $(SRC_MAIN:.c=.o)))

BIN_SERIAL = $(BIN_DIR)/jacobi-2d-serial
BIN_PARALLEL = $(BIN_DIR)/jacobi-2d-parallel
BIN_MAIN = $(BIN_DIR)/main

.PHONY: all clean

all: $(BIN_MAIN)

$(BIN_MAIN): $(OBJ_MAIN) $(OBJ_SERIAL) $(OBJ_PARALLEL) $(OBJ_POLYBENCH) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
