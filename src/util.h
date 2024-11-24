#ifndef UTIL_H
#define UTIL_H

int send_all(int sockfd, const char *buffer, int buf_len);
int recv_all(int sockfd, char *buffer, int buf_len);

#endif
