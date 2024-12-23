#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_LEN ((2<<7) - 1)

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

int recv_all(int sockfd, char **buf) {
    int total_bytes_read = 0;
    int bytes_read;
    uint8_t len;
    // read first byte for length of packet
    *buf = malloc(len);
    // up to caller to free this allocated memory
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

unsigned char *create_packet(unsigned char *data) {
    int len = strlen((char *) data);
    if (len > MAX_LEN) {
        return NULL;
    }
    unsigned char *packet = malloc(len + 1); // + 1 for length byte at header of packet
    // up to caller to free this pointer
    if (packet == NULL) {
        return NULL;
    }
    unsigned char *p = packet;
    *p++ = (uint8_t) len;
    memcpy(p, data, len);
    return packet;
}

char *unpack_packet(unsigned char *packet) {
    if (packet == NULL) {
        return NULL;
    }
    uint8_t len = packet[0];
    char *message = malloc(len + 1); // + 1 for the NUL
    // up to caller to free this memory
    if (message == NULL) {
        return NULL;
    }
    memcpy(message, (char *) (packet + 1), len);
    message[len] = '\0';
    return message;
}
