# Variables

CC = clang
CFLAGS = -Wall -Werror -g
SRC_DIR = src
OBJ_DIR = $(SRC_DIR)

all: $(OBJ_DIR)/server #$(OBJ_DIR)/client
# target to create server executable

$(OBJ_DIR)/server: $(OBJ_DIR)/server.o $(OBJ_DIR)/parser.o $(OBJ_DIR)/util.o
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/server $(OBJ_DIR)/server.o $(OBJ_DIR)/parser.o $(OBJ_DIR)/util.o

$(OBJ_DIR)/server.o: $(SRC_DIR)/server.c $(SRC_DIR)/server.h $(SRC_DIR)/parser.h $(SRC_DIR)/util.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/server.c -o $(OBJ_DIR)/server.o

#$(OBJ_DIR)/client: $(OBJ_DIR)/client.o
#	$(CC) $(CFLAGS) -o $(OBJ_DIR)/client $(SRC_DIR)/client.o

#$(OBJ_DIR)/client.o: $(SRC_DIR)/client.c
#	$(CC) $(CFLAGS) -c $(SRC_DIR)/client.c -o $(OBJ_DIR)/client.o

$(OBJ_DIR)/parser.o: $(SRC_DIR)/parser.c $(SRC_DIR)/parser.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/parser.c -o $(OBJ_DIR)/parser.o

$(OBJ_DIR)/util.o: $(SRC_DIR)/util.c $(SRC_DIR)/util.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/util.c -o $(OBJ_DIR)/util.o
clean:
	rm -f $(OBJ_DIR)/server $(OBJ_DIR)/server.o $(OBJ_DIR)/client $(OBJ_DIR)/client.o $(OBJ_DIR)/parser.o $(OBJ_DIR)/util.o
