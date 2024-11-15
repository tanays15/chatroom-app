# Variables

C = clang
CFLAGS = -Wall -Werror
SRC_DIR = src
OBJ_DIR = $(SRC_DIR)

all: $(OBJ_DIR)/server $(OBJ_DIR)/client
# target to create server executable

$(OBJ_DIR)/server: $(SRC_DIR)/server.o
	$(C) $(CFLAGS) -o $(OBJ_DIR)/server $(SRC_DIR)/server.o

$(OBJ_DIR)/server.o: $(SRC_DIR)/server.c
	$(C) $(CFLAGS) -c $(SRC_DIR)/server.c -o $(OBJ_DIR)/server.o

$(OBJ_DIR)/client: $(SRC_DIR)/client.o
	$(C) $(CFLAGS) -o $(OBJ_DIR)/client $(SRC_DIR)/client.o

$(OBJ_DIR)/client.o: $(SRC_DIR)/client.c
	$(C) $(CFLAGS) -c $(SRC_DIR)/client.c -o $(OBJ_DIR)/client.o
clean:
	rm -f $(OBJ_DIR)/server $(OBJ_DIR)/server.o $(OBJ_DIR)/client $(OBJ_DIR)/client.o 
