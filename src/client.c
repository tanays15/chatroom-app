// c file for client
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "client.h"
#include "util.h"

#define BACKLOG 10
#define MAX_BUFF_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "incorrect arguments provided\n");
        exit(1);
    }
    char *port = argv[1];
    int status;
    struct addrinfo hints, *servinfo, *p; // sets up call for getaddrinfo
    int sockfd;
    memset(&hints, 0, sizeof(hints)); // set hints to 0 before modifying fields
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    status = getaddrinfo(NULL, port, &hints, &servinfo);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }
    if (connect_socket(sockfd, servinfo) == -1) {
        close_socket(sockfd);
    }
    char buffer[MAX_BUFF_SIZE];
    int bytes_read;
    while (1) {
        fprintf(stdout, "waiting..\n");
    }
    close_socket(sockfd);
    return 0;
}

int connect_socket(int sockfd, struct addrinfo *serv_info) {
    struct addrinfo *p;
    for (p = serv_info; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket\n");
            continue;
        }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("client connect:");
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "Could not connect to server\n");
        return -1;
    }
    return 0;
}
