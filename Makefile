# Variables

C = clang
CFLAGS = -Wall -Werror
SRC_DIR = src
OBJ_DIR = $(SRC_DIR)

# target to create server executable

$(OBJ_DIR)/server: $(SRC_DIR)/server.o
	$(C) $(CFLAGS) -o $(OBJ_DIR)/server $(SRC_DIR)/server.o

# target to create server.o oject file

$(OBJ_DIR)/server.o: $(SRC_DIR)/server.c
	$(C) $(CFLAGS) -c $(SRC_DIR)/server.c -o $(OBJ_DIR)/server.o


clean:
	rm -f $(OBJ_DIR)/server $(OBJ_DIR)/server.o
