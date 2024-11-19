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


#include "parser.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BACKLOG 10
#define MAX_BUFF_SIZE 1024

int main(int argc, char *argv[]) {
    char *port = argc != 2 ? "8080" : argv[1];
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
    while (1) {
        if ((new_fd = accept(sockfd, (struct sockaddr *)&inc_addr, &addr_size)) ==
            -1) {
            perror("accept");
            close(sockfd);
            return 1;
        }
        fprintf(stdout, "Connected to client: %d\n", new_fd);
        while (1) {
            int recv_len;
            char buffer[MAX_BUFF_SIZE] = {0};
            if ((recv_len = recv(new_fd, buffer, MAX_BUFF_SIZE, 0)) == -1) {
                perror("recv");
                close(new_fd);
                continue;
            }
            buffer[recv_len] = '\0';
            fprintf(stdout, "LOG: %s", buffer);
            msg_t resp = parseMessage(buffer);
            if (resp.auth == INVALID) {
                char *error_msg = "ERROR\n";
                int error_len = strlen(error_msg);
                fprintf(stdout, "Could not read message\n");
                if (send(new_fd, error_msg, error_len, 0) == -1) {
                    perror("send");
                }
            } else {
                fprintf(stdout, "auth: %d\n", resp.auth);
                if (send(new_fd, buffer, recv_len, 0) == -1) {
                    perror("send");
                }
            }
        }
    }
    close(sockfd);
    return 0;
}
