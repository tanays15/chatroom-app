/*
 * EXPANSION: make simple parser -> allow multiple clients to connect to the
 * same server -> expand API to move clients into "chatrooms" -> allow clients
 * to chat within their rooms
 */

/*
 * FIXES:
 * allow spaces to be sent/read?
 * inputs should be: HEADER COMMAND -> text to echo?
 */


#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>
#include "server.h"
#include "util.h"
//#include "parser.h"

#define BACKLOG 10
#define MAX_BUFF_SIZE 1024
#define INIT_SIZE 5

int main(int argc, char *argv[]) {
    int new_fd = 0;
    struct pollfd pfds[INIT_SIZE];
    char *port = argc != 2 ? "8080" : argv[1];
    struct sockaddr_storage inc_addr;
    socklen_t addr_size = sizeof (struct sockaddr);
    pfds[0].fd = create_listener(port);
    pfds[0].events = POLL_IN;
    while (1) {
        
    }
    return 0;
}

int create_listener(char *port) {
    struct addrinfo hints, *servinfo, *p; // sets up call for getaddrinfo
    int sockfd, status, yes;
    memset(&hints, 0, sizeof(hints)); // set hints to 0 before modifying fields
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if ((status = getaddrinfo(NULL, port, &hints, &servinfo)) < 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return -1;
    }
    if (bind_socket(sockfd, servinfo) == -1) {
        close_socket(sockfd);
        return -1;
    }
    if (listen(sockfd, BACKLOG) == -1) {
        close_socket(sockfd);
        return -1;
    }
    fprintf(stdout, "server: listening on port: %s\n", port);
    return sockfd;
}

int bind_socket(int sockfd, struct addrinfo *serv_info) {
    struct addrinfo *p;
    int yes;
    for (p = serv_info; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server : socket\n");
            continue;
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            return -1;
        }
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("bind");
            continue;
        }
        break;
    }
    freeaddrinfo(serv_info);
    if (p == NULL) {
        perror("bind");
        return -1;
    }
    return 0;
}
