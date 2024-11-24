#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int send_all(int sockfd, const char *resp, int resp_len) {
  int total_bytes_sent = 0;
  int sent;
  while (total_bytes_sent < resp_len) {
    if ((sent = send(sockfd, resp + total_bytes_sent, resp_len, 0)) == -1) {
      perror("send");
      break;
    }
    total_bytes_sent += sent;
    resp_len -= sent;
  }
  return sent == -1 ? -1 : 0;
}

int recv_all(int sockfd, char *req, int req_len) {
  int total_bytes_read = 0;
  int bytes_read;
  while (total_bytes_read < req_len) {
    if ((bytes_read = recv(sockfd, req + total_bytes_read, req_len, 0)) == -1) {
      perror("recv");
      break;
    }
    total_bytes_read += bytes_read;
    req_len -= bytes_read;
  }
  return bytes_read == -1 ? -1 : 0;
}

void close_socket(int sockfd) {
    close(sockfd);
    fprintf(stdout, "socket closed\n");
}
