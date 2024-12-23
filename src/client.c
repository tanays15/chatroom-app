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
#define MAX_BUFF_SIZE ((2<<7) - 1)

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stdout, "Incorrect arguments provided\n");
        return 1;
    }
    char *port = argv[1];
    int client_socket = create_socket(port);
    if (client_socket == -1) {
        fprintf(stdout, "Unable to connect to server\n");
        return -1;
    }
    char buf[MAX_BUFF_SIZE];
    while (1) {
        if (fgets(buf, MAX_BUFF_SIZE, stdin) == NULL) {
            fprintf(stdout, "error: invalid input\n");
            continue;
        }
        if (strcmp(buf, "exit") == 0) {
            fprintf(stdout, "closing client\n");
            close_socket(client_socket);
            return 1;
        }
        unsigned char *packet = create_packet(buf);
        if (packet == NULL) {
            fprintf(stdout, "error: invalid packet format\n");
            continue;
        }
        int packet_size = strlen((char *) packet);
        fprintf(stdout, "sending: %s\n", packet);
        if (send_all(client_socket, (char *) packet, packet_size) == -1) {
            fprintf(stdout, "error: couldn't send packet\n");
            continue;
        }
        free(packet);
        packet = NULL;
        char *serv_pack;
        if (recv_all(client_socket, &serv_pack) == -1) {
            fprintf(stdout, "error: couldn't recieve packet\n");
            continue;
        }
        char *response = unpack_packet((unsigned char *)serv_pack);
        free(serv_pack);
        serv_pack = NULL;
        if (response == NULL) {
            fprintf(stdout, "error: bad packet recieved\n");
            continue;
        }
        fprintf(stdout, "recieved: %s\n", response);
        free(response);
        response = NULL;
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
