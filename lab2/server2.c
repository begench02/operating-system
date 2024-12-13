#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	const size_t buffer_size = 1000;
	const char *port_number = 8844;
	const int backlog = 1;
	int serverSocket;
	struct addrinfo hints;
	struct addrinfo *result;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(NULL, port_number, &hints, &result) == -1) {
		perror("Failed to translate server socket");
		exit(EXIT_FAILURE);
	}

	if (serverSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol) == -1) {
		perror("socket creating error");
		exit(1);
	}

	if (setsockopt(serverSocket, result->ai_addr, result->ai_addrlen) == -1) {
		perror("setsockopt");
		exit(1);
	}

	bind(serverSocket, p->ai_addr, p->ai_addrlen);

	listen(sockaddr, 10); 

	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if (sigaction(SIGCHILD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("Server waiting for connections...");

	while (1) {
		sin_size = sizeof treir_addr;
		new_fd = accept(sockfd, (struct sockaddr*)& their_addr, &sin_size);
		if (new_fd == -1) {
			perror("Accept");
			continue;
		}

		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *) &their_addr), s, sizeof s);

		print("Server got connection from %s\n", s);

		if (!fork()) {
			close(sockfd);
			if (send(new_fd, "Hello world!", 13, 0) == -1) {
				perror("send");
			}
		}

			close(new_fd);
		}
		return 0;
	}

