#ifndef UTIL_H
#define UTIL_H

int send_all(int, const char *, int);
int recv_all(int, char *, int);
void close_socket(int);

#endif
