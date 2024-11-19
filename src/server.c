/*
 * EXPANSION: make simple parser -> allow multiple clients to connect to the same server -> expand API to move clients into "chatrooms" -> allow clients to chat within their rooms
 */



#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"

#define BACKLOG 10
#define MAX_BUFF_SIZE 32

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "incorrect arguments provided\n");
        exit(1);
    }
    char *port = argv[1];
    int status;
    int yes = 1;
    struct addrinfo hints, *servinfo, *p; // sets up call for getaddrinfo
    struct sockaddr_storage inc_addr;
    socklen_t addr_size = sizeof inc_addr;
    int sockfd, new_fd;
    memset(&hints, 0, sizeof(hints)); // set hints to 0 before modifying fields
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    status = getaddrinfo(NULL, port, &hints, &servinfo);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }
    // logic for listening on socket and port
    for (p = servinfo; p != NULL; p = p->ai_next)  {
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
    while(1) {
        if ((new_fd = accept(sockfd, (struct sockaddr *) &inc_addr, &addr_size)) == -1) {
            perror("accept");
            close(sockfd);
            return 1;
        }
        fprintf(stdout, "Connected to client: %d\n", new_fd);
        int recv_len;
        char buffer[MAX_BUFF_SIZE] = {0};
        while(1) {
            int exit_flag = 0;
            if ((recv_len = recv(new_fd, buffer, MAX_BUFF_SIZE, 0)) == -1) {
                perror("recv");
                close(new_fd);
                continue;
            }
            buffer[recv_len] = '\0';
            fprintf(stdout, "recieved: %s\n", buffer);
            if (strcmp(buffer, "exit") == 0) {
                exit_flag = 1;
            }
            if (strcmp(buffer, "close") == 0) {
              exit_flag = 2;
            }
            char *msg = strdup(buffer);
            int len_msg = strlen(msg);
            if (send(new_fd, msg, len_msg, 0) == -1) {
                perror("send");
            }
            free(msg);
            msg = NULL;
            if (exit_flag == 1) {
                fprintf(stdout, "closing client connection from %d\n", new_fd);
                close(new_fd);
                break;
            } else if (exit_flag == 2) {
                fprintf(stdout, "closing server\n");
                close(new_fd);
                close(sockfd);
                return 0;
            }
        }
    }
    close(sockfd);
    return 0;
}

