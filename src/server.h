#ifndef SERVER_H
#define SERVER_H


int create_listener(char *);
int bind_socket(struct addrinfo *);
void add_connection(struct pollfd *[], int, int *, int *); // takes in the array of connections and the new capacity
void delete_connection(struct pollfd[], int, int *);
#endif
