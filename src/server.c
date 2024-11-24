#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
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
    int listener_socket = create_listener(port);
    if (listener_socket == -1) {
        fprintf(stdout, "Couldn't listen on socket\n");
        return 1;
    }
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

void add_connection(struct pollfd *pfds[], int new_fd, int *fd_count, int *fd_cap) {
    if (fd_cap == fd_count) {
        *fd_count *= 2;
        *pfds = realloc(*pfds, sizeof(**pfds) * (*fd_cap));
    }
    (*pfds)[(*fd_count)].fd = new_fd;
    (*pfds)[(*fd_count)].events = POLL_IN;
    (*fd_count)++;
}

void delete_connection(struct pollfd pfds[], int index, int *fd_count) {
    pfds[index] = pfds[(*fd_count - 1)];
    (*fd_count)--;
}
