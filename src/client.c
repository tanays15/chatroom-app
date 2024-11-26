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
    if (argc < 2) {
        fprintf(stdout, "Incorrect arguments provided\n");
        return 1;
    }

}

int create_socket(char *port) {
    struct addrinfo hints, *servinfo;
    int status, sockfd;
    memset(&hints, 0, sizeof(hints)); // set hints to 0 before modifying fields
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    status = getaddrinfo(NULL, port, &hints, &servinfo);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return -1;
    }
    if ((sockfd = connect_socket(servinfo)) == -1) {
        return -1;
    }
    fprintf(stdout, "Client connected to port: %s\n", port);
    return sockfd;
}

int connect_socket(struct addrinfo *servinfo) {
    struct addrinfo *p; // sets up call for getaddrinfo
    int sockfd;
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
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
    return sockfd;
}
