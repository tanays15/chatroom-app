#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include "client.h"
#include "util.h"

#define BACKLOG 10
#define MAX_BUFF_SIZE ((2<<7) - 1)
#define TIMEOUT 2500

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stdout, "Incorrect arguments provided\n");
        return 1;
    }
    char *port = argv[1];
    struct pollfd *pfds = malloc(sizeof(struct pollfd) * 2);
    int client_socket = create_socket(port);
    if (client_socket == -1) {
        fprintf(stdout, "Unable to connect to server\n");
        return -1;
    }
    pfds[0].fd = client_socket;
    pfds[0].events = POLLIN;
    pfds[1].fd = STDIN_FILENO;
    pfds[1].events = POLLIN;
    char buf[MAX_BUFF_SIZE];
    while (1) {
        int polled = poll(pfds, 2, TIMEOUT);
        if (polled == -1) {
            perror("poll");
            free(pfds);
            close_socket(client_socket);
            return 1;
        }
        for (int i = 0; i < 2; i++) {
            if (pfds[i].revents & POLLIN) {
                if (pfds[i].fd == STDIN_FILENO) {
                    // logic for reading in user input and sending to socket
                    if (fgets(buf, MAX_BUFF_SIZE, stdin) == NULL) {
                        fprintf(stdout, "error: reading user input\n");
                        continue;
                    }
                    int buf_size = strlen(buf) - 1;
                    if (buf_size <= 0) {
                        continue;
                    }
                    buf[buf_size] = '\0';
                    if (strcmp(buf, "exit") == 0) {
                        send_all(client_socket, "", 0);
                        fprintf(stdout, "closing client\n");
                        close_socket(client_socket);
                        return 1;
                    }
                    // read user input into buf, now write this to the server socket
                    unsigned char *packet = create_packet(buf);
                    if (packet == NULL) {
                        fprintf(stdout, "error: invalid packet format\n");
                        continue;
                    }
                    int packet_size = buf_size + 1; // add first length byte
                    fprintf(stdout, "client sending: %s\n", packet);
                    if (send_all(client_socket, (char *) packet, packet_size) == -1) {
                        fprintf(stdout, "error: couldn't send packet\n");
                        continue;
                    }
                    free(packet);
                    packet = NULL;
                } else {
                    // logic for recieving from server
                    unsigned char *serv_pack;
                    if (recv_all(client_socket, &serv_pack) == -1) {
                        fprintf(stdout, "error: couldn't recieve packet\n");
                        continue;
                    }
                    if (serv_pack == NULL) {
                        fprintf(stdout, "error: bad packet recieved\n");
                        continue;
                    }
                    fprintf(stdout, "recieved: %s\n", serv_pack);
                    free(serv_pack);
                    serv_pack = NULL;
                }
            }
        }
        /*
        if (fgets(buf, MAX_BUFF_SIZE, stdin) == NULL) {
            fprintf(stdout, "error: invalid input\n");
            continue;
        }
        if (strcmp(buf, "exit") == 0) {
            fprintf(stdout, "closing client\n");
            close_socket(client_socket);
            return 1;
        }
        int buf_size = strlen(buf) - 1;
        if (buf_size <= 0) {
            continue;
        }
        buf[buf_size] = '\0';
        unsigned char *packet = create_packet(buf);
        if (packet == NULL) {
            fprintf(stdout, "error: invalid packet format\n");
            continue;
        }
        int packet_size = buf_size + 1; // add first length byte
        fprintf(stdout, "client sending: %s\n", packet);
        if (send_all(client_socket, (char *) packet, packet_size) == -1) {
            fprintf(stdout, "error: couldn't send packet\n");
            continue;
        }
        free(packet);
        packet = NULL;
        unsigned char *serv_pack;
        if (recv_all(client_socket, &serv_pack) == -1) {
            fprintf(stdout, "error: couldn't recieve packet\n");
            continue;
        }
        if (serv_pack == NULL) {
            fprintf(stdout, "error: bad packet recieved\n");
            continue;
        }
        fprintf(stdout, "client recieved: %s\n", serv_pack);
        free(serv_pack);
        serv_pack = NULL;*/
    }
    return 0;

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
