# Compiler
CC = gcc
MPICC = mpicc

# Flags
CFLAGS = -Wall -O3
LFLAGS = -lm

# Directories
SRC_DIR = src
BIN_DIR = bin

# Source files
SRC_SERIAL = $(SRC_DIR)/jacobi-2d-serial.c $(SRC_DIR)/common.c
SRC_PTHREAD = $(SRC_DIR)/jacobi-2d-pthread.c $(SRC_DIR)/common.c
SRC_MPI = $(SRC_DIR)/jacobi-2d-mpi.c $(SRC_DIR)/common.c
SRC_MPI_PTHREAD = $(SRC_DIR)/jacobi-2d-mpi-pthread.c $(SRC_DIR)/common.c

# Executables
EXEC_SERIAL = $(BIN_DIR)/jacobi-2d-serial
EXEC_PTHREAD = $(BIN_DIR)/jacobi-2d-pthread
EXEC_MPI = $(BIN_DIR)/jacobi-2d-mpi
EXEC_MPI_PTHREAD = $(BIN_DIR)/jacobi-2d-mpi-pthread

# Create bin directory
$(shell mkdir -p $(BIN_DIR))

all: $(EXEC_SERIAL) $(EXEC_PTHREAD) $(EXEC_MPI) $(EXEC_MPI_PTHREAD)

$(EXEC_SERIAL): $(SRC_SERIAL)
	$(CC) -I $(SRC_DIR) $(CFLAGS) -o $@ $^ $(LFLAGS)

$(EXEC_PTHREAD): $(SRC_PTHREAD)
	$(CC) -I $(SRC_DIR) $(CFLAGS) -pthread -o $@ $^ $(LFLAGS)

$(EXEC_MPI): $(SRC_MPI)
	$(MPICC) -I $(SRC_DIR) $(CFLAGS) -o $@ $^ $(LFLAGS)

$(EXEC_MPI_PTHREAD): $(SRC_MPI_PTHREAD)
	$(MPICC) -I $(SRC_DIR) $(CFLAGS) -pthread -o $@ $^ $(LFLAGS)

clean:
	rm -rf $(BIN_DIR)/*.o $(EXEC_SERIAL) $(EXEC_PTHREAD) $(EXEC_MPI) $(EXEC_MPI_PTHREAD)

.PHONY: all clean
