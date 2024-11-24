#ifndef SERVER_H
#define SERVER_H


int create_listener(char *);
int bind_socket(int, struct addrinfo *);
int add_connection(struct pollfd *, int); // takes in the array of connections and the new capacity
int delete_connection(struct pollfd *, int);
void * resize_array(void *);
#endif
