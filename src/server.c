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
        exit(1);
    }
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server : socket\n");
            continue;
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            close(sockfd);
            perror("setsockopt");
            exit(1);
        }
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("bind");
            continue;
        }
        break;
    }
    freeaddrinfo(servinfo);
    if (p == NULL) {
        close(sockfd);
        fprintf(stderr, "server: failed to bind\n");
        return 1;
    }

    if (listen(sockfd, BACKLOG) == -1) {
        close(sockfd);
        perror("listen");
        return 1;
    } else {
        fprintf(stdout, "server: listening on port: %s\n", port);
    }
    return sockfd;
}

int send_all(char *resp, int sockfd) {
    int bytes_sent = 0;
    int buffer_len = strlen(resp);
    int sent;
    while (bytes_sent < buffer_len) {
       if((sent = send(sockfd, resp + bytes_sent, buffer_len, 0)) == -1) {
            perror("send");
            break;
        } 
        bytes_sent += sent;
        buffer_len -= sent;
    }
    return sent == -1 ? -1 : 0;
}
