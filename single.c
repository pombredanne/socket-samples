#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "socket.h"

#define PORT 8000

int main(int argc, char *argv[])
{
	struct sockaddr_in6 addr;
        int n;
	socklen_t len = sizeof(addr);
	char buffer[256];
	int fd, server_fd;

	server_fd = setup_socket_server(PORT);
	if (server_fd < 0) {
		perror("ERROR binding the server");
		exit(1);
	}

	printf("listening on port %d\n", PORT);

	while (1) {
		fd = accept(server_fd, (struct sockaddr *)&addr, &len);
		if (fd < 0) {
			perror("ERROR on accept");
			exit(1);
		}

		const char *client_addr = inet_ntop(AF_INET6, &addr.sin6_addr, buffer, sizeof(buffer));
		printf("got connection from %s\n", client_addr);

		n = read(fd, buffer, sizeof(buffer) - 1);
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}
		buffer[n] = 0;
		printf("%s\n", buffer);

		n = write_full(fd, buffer, n);
		if (n < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}
		close(fd);
	}

	return 0;
}
