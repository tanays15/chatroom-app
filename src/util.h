#ifndef UTIL_H
#define UTIL_H

int send_all(int, const char *, int);
int recv_all(int, unsigned char**);
void close_socket(int);
unsigned char *create_packet(char *);
char *unpack_packet(unsigned char *);
#endif
