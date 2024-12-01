#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#define PORT 8844
#define BACKLOG 5

volatile sig_atomic_t sighupReceived = 0;

void sighupHandler(int sigNumber) {
	sighupReceived = 1;
}

int main() {
	int serverFD;
	int incomingSocketFD = 0;
	struct sockaddr_in socketAddress;
	int addressLength = sizeof(socketAddress);
	fd_set readfds;
	struct sigaction sa;
	sigset_t blockedMask, origMask;
	char buffer[4000] = {0};
	int readBytes;
	int maxSd;
	int signalOrConnectionCount = 0;

	if ((serverFD = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("create error");
		exit(EXIT_FAILURE);
	}

	socketAddress.sin_family = AF_INET;
	socketAddress.sin_addr.s_addr = INADDR_ANY;
	socketAddress.sin_port = htons(PORT);

	if (bind(serverFD, (struct sockaddr*)& socketAddress, sizeof(socketAddress)) < 0) {
		perror("bind error");
		exit(EXIT_FAILURE);
	}

	if (listen(serverFD, BACKLOG) < 0) {
		perror("listen error");
		exit(EXIT_FAILURE);
	}

	printf("Server running on port: %d\n", PORT);

	sigaction(SIGHUP, NULL, &sa);
	sa.sa_handler = sighupHandler;
	sa.sa_flags |= SA_RESTART;
	sigaction(SIGHUP, &sa, NULL);

	sigemptyset(&blockedMask);
	sigemptyset(&origMask);
	sigaddset(&blockedMask, SIGHUP);
	sigprocmask(SIG_BLOCK, &blockedMask, &origMask);

	while (signalOrConnectionCount < 3) {
		FD_ZERO(&readfds);
		FD_SET(serverFD, &readfds);

		if (incomingSocketFD > 0) {
			FD_SET(incomingSocketFD, &readfds);
		}

		maxSd = (incomingSocketFD > serverFD) ? incomingSocketFD : serverFD;

		if (pselect(maxSd + 1, &readfds, NULL, NULL, NULL, &origMask) < 0 && errno != EINTR) {
			perror("Pselect error");
			exit(EXIT_FAILURE);
		}

		if (sighupReceived) {
			printf("SIGHUP received!");
			sighupReceived = 0;
			signalOrConnectionCount++;
			continue;
		}

		if (incomingSocketFD > 0 && FD_ISSET(incomingSocketFD, &readfds)) {
			readBytes = read(incomingSocketFD, buffer, 1024);

			if (readBytes > 0) {
				printf("Received data: %d bytes\n", readBytes);
			} else {
				if (readBytes == 0) {
					close(incomingSocketFD);
					incomingSocketFD = 0;
				} else {
					perror("read error");
				}

				signalOrConnectionCount++;
			}
			continue;
		}

		if (FD_ISSET(serverFD, &readfds)) {
			if ((incomingSocketFD = accept(serverFD, (struct sockaddr*)&socketAddress, (socklen_t*)&addressLength)) < 0) {
				perror("accept error");
				exit(EXIT_FAILURE);
			}

			printf("New connection \n");
		}
	}

	close(serverFD);

	return 0;
}