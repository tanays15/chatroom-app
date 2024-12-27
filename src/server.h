#ifndef SERVER_H
#define SERVER_H


/*
 * user struct
 * socket will store the socket fd of the connected client
 * room will store the chatroom the client is connected to
 */

typedef struct user {
    int socket;
    int room;
} user_t;

int create_listener(char *);
int bind_socket(struct addrinfo *);
void add_connection(struct pollfd *[], int, int *, int *); // takes in the array of connections and the new capacity
void delete_connection(struct pollfd[], int, int *);
void add_user(user_t*[], int, int*, int*);
void delete_user(user_t [], int, int *);
#endif
