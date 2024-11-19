// c file for client
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define BACKLOG 10
#define MAX_BUFF_SIZE 32


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "incorrect arguments provided\n");
        exit(1);
    }
    char *port = argv[1];
    int status;
    struct addrinfo hints, *servinfo, *p; // sets up call for getaddrinfo
    int sockfd;
    memset(&hints, 0, sizeof(hints)); // set hints to 0 before modifying fields
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    status = getaddrinfo(NULL, port, &hints, &servinfo);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket\n");
            continue;
        }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client connect:");
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "Could not connect to server\n");
        close(sockfd);
        exit(0);
    }
    char buffer[MAX_BUFF_SIZE];
    int bytes_read;
    while (1) {
        int exit_flag = 0;
        fscanf(stdin, "%31s", buffer); // 31 to ensure that buffer is NUL termianted
        buffer[MAX_BUFF_SIZE - 1] = '\0';
        if ((bytes_read = send(sockfd, buffer, strlen(buffer), 0)) < 0) {
            perror("send");
        }
        if (strcmp(buffer, "exit") == 0 || strcmp(buffer, "close") == 0) {
            exit_flag = 1;
        }
        if ((bytes_read = recv(sockfd, buffer, MAX_BUFF_SIZE - 1, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        buffer[bytes_read] = '\0';
        fprintf(stdout, "recieved: %s\n", buffer);
        if (exit_flag) {
            break;
        }
    }
    fprintf(stdout, "closing client\n");
    close(sockfd);
    return 0;
}
