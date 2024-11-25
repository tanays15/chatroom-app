#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

int send_all(int sockfd, const char *resp, int resp_len) {
  int total_bytes_sent = 0;
  int bytes_sent;
  while (total_bytes_sent < resp_len) {
    if ((bytes_sent = send(sockfd, resp + total_bytes_sent, resp_len, 0)) == -1) {
      perror("send");
      break;
    }
    total_bytes_sent += bytes_sent;
    resp_len -= bytes_sent;
  }
  return bytes_sent == -1 ? -1 : 0;
}

int recv_all(int sockfd, char *req, char **buf) {
    int total_bytes_read = 0;
    int bytes_read;
    uint8_t len;
    // read first byte for length of packet
    *buf = malloc(len);
    if (recv(sockfd, &len, sizeof(uint8_t), 0) == -1) {
        fprintf(stdout, "Invalid Header\n");
        return -1;
    }
    while (total_bytes_read < len) {
        if ((bytes_read = recv(sockfd, buf + total_bytes_read, len ,0)) == -1) {
            perror("recv");
            break;
        }
        total_bytes_read += bytes_read;
        len -= bytes_read;
    }
    return bytes_read == -1 ? -1 : 0;
}

void close_socket(int sockfd) {
    close(sockfd);
    fprintf(stdout, "socket closed\n");
}
