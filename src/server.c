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
#include "server.h"
#include "parser.h"

#define BACKLOG 10
#define MAX_BUFF_SIZE 1024

int main(int argc, char *argv[]) {
    int new_fd = 0;
    char *port = argc != 2 ? "8080" : argv[1];
    struct sockaddr_storage inc_addr;
    socklen_t addr_size = sizeof (struct sockaddr);
    int sockfd = createListener(port);
    while (1) {
        if ((new_fd = accept(sockfd, (struct sockaddr *) &inc_addr, (socklen_t *) &addr_size)) < 0) {
            perror("accept");
        }
        while (1) {
            int bytes_read;
            char buffer[MAX_BUFF_SIZE];
            if ((bytes_read = recv(new_fd, buffer, MAX_BUFF_SIZE, 0)) < 0) {
                perror("recv");
                continue;
            }
            fprintf(stdout, "LOG: %s\n", buffer);
            msg_t resp = parseMessage(buffer);
            send(new_fd, resp.msg, strlen(resp.msg), 0);
        }
    }
    return 0;
}

int createListener(char *port) {
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
