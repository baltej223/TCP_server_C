#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8000

int main() {
  int tcp_socket = socket(AF_INET, SOCK_STREAM, 0); /* socket syscall */

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT); // htons means: host to network store
  server.sin_addr.s_addr = INADDR_ANY;

  if (!(bind(tcp_socket, (struct sockaddr *)&server, sizeof(server)) ==
        0)) /*bind syscall!*/ {
    // bind not succeeded.
    perror("Bind Error");
  }

  if (listen(tcp_socket, 100) != 0) {
    perror("TCP LISTEN ERROR");
  }

  for (int i = 0; i < 10000; i++) {
    int client_fd = accept(tcp_socket, NULL, NULL);
    if (client_fd < 0) {
      perror("accept");
    }
    char *data = "Just a TCP server sending some data!";

    write(client_fd, data, strlen(data));
    printf("Handling %dth request\n", i);
    int shut = shutdown(client_fd, SHUT_WR);
    fflush(stdout);
    close(client_fd);
  }
  close(tcp_socket);
}
