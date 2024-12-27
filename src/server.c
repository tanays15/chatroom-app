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
#define TIMEOUT 2500

int main(int argc, char *argv[]) {
    char *port = argc != 2 ? "8080" : argv[1];
    int new_fd;
    struct sockaddr_storage inc_addr;
    socklen_t addr_size = sizeof (struct sockaddr);
    int fd_count = 0;
    int fd_cap = INIT_SIZE;
    struct pollfd *pfds = malloc(sizeof (struct pollfd) * fd_cap);
    user_t *users = malloc(sizeof(user_t) * INIT_SIZE);
    unsigned char *buf;
    int listener_socket = create_listener(port);
    if (listener_socket == -1) {
        fprintf(stdout, "Couldn't listen on socket\n");
        return 1;
    }
    pfds[0].fd = listener_socket;
    pfds[0].events = POLLIN;
    fd_count++;
    while (1) {
        int polled = poll(pfds, fd_count, TIMEOUT);
        if (polled == -1) {
            perror("poll");
            free(pfds);
            close_socket(listener_socket);
            return 1;
        }
        for (int i = 0; i < fd_count; ++i) {
            if (pfds[i].revents & POLLIN) {
                if (pfds[i].fd == listener_socket) { // if curr is a listener, try adding a new connection
                    new_fd = accept(listener_socket, (struct sockaddr *) &inc_addr, (socklen_t *) &addr_size);
                    if (new_fd == -1) {
                        perror("accept");
                    }
                    add_connection(&pfds, new_fd, &fd_count, &fd_cap);
                    fprintf(stdout, "Connected to %d\n", new_fd);
                } else { // else read from a client
                    int bytes = recv_all(pfds[i].fd, &buf);
                    int sender_fd = pfds[i].fd;
                    if (bytes <= 0) {
                        if (bytes == 0) {
                            fprintf(stdout, "client %d closed connection\n", sender_fd);
                        } else {
                            perror("recv");
                        }
                        close(pfds[i].fd);
                        delete_connection(pfds, i, &fd_count);
                        continue;
                    }
                    fprintf(stdout, "server recieved: %s from client %d\n", buf, sender_fd);
                    unsigned char *resp = create_packet((char *)buf);
                    free(buf);
                    buf = NULL;
                    if (resp == NULL) {
                        fprintf(stdout, "error: bad packet\n");
                        continue;
                    }
                    int resp_len = strlen((char *)resp);
                    for (int j = 0; j < fd_count; ++j) {
                        int curr_client = pfds[j].fd;
                        if (curr_client != sender_fd && curr_client != listener_socket) {
                            fprintf(stdout, "server sending: %s to client %d\n", resp, curr_client);
                            if (send_all(curr_client, (char *)resp, resp_len) == -1) {
                                fprintf(stdout, "error: couldn't send response\n");
                            }
                        }
                    }
                    free(resp);
                    resp = NULL;
                }
            }
        }
    }
    return 0;
}

int create_listener(char *port) {
    struct addrinfo hints, *servinfo; // sets up call for getaddrinfo
    int sockfd, status;
    memset(&hints, 0, sizeof(hints)); // set hints to 0 before modifying fields
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if ((status = getaddrinfo(NULL, port, &hints, &servinfo)) < 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return -1;
    }
    if ((sockfd = bind_socket(servinfo)) == -1) {
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

int bind_socket(struct addrinfo *serv_info) {
    int sockfd;
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
    return sockfd;
}

void add_connection(struct pollfd *pfds[], int new_fd, int *fd_count, int *fd_cap) {
    if (*fd_cap == *fd_count) {
        *fd_cap *= 2;
        *pfds = realloc(*pfds, sizeof(**pfds) * (*fd_cap));
    }
    (*pfds)[(*fd_count)].fd = new_fd;
    (*pfds)[(*fd_count)].events = POLLIN;
    (*fd_count)++;
}

void delete_connection(struct pollfd pfds[], int index, int *fd_count) {
    pfds[index] = pfds[(*fd_count)];
    (*fd_count)--;
}

void add_user(user_t *users[], int new_fd, int *user_count, int *user_cap) {
    if (*user_cap == *user_count) {
        *user_cap *= 2;
        *users = realloc(*users, sizeof(**users) * (*user_cap));
    }
    user_t *new_user = malloc(sizeof(user_t));
    (*users)[(*user_count)].room = 0;
    (*users)[(*user_count)].socket = new_fd;
    (*user_count)++;
}

void delete_user(user_t users[], int index, int *user_count) {
    users[index] = users[(*user_count)];
    (*user_count)--;
}
