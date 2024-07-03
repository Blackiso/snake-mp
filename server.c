#include <windows.h>
#include <stdio.h>
#include "types.h"

#pragma comment(lib, "Ws2_32.lib")

const int PORT = 5252;
const int BUFFER_SIZE = sizeof(char) * 4;

int startServer(GameState* state) {

	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (result != 0) {
		printf("Error: %d\n", result);
		return 1;
	}

	SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
	if (server == INVALID_SOCKET) {
		printf("Error: %d\n", WSAGetLastError());
		return 1;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = 0;
	addr.sin_port = htons(PORT);

	bind(server, &addr, sizeof(addr));
	listen(server, SOMAXCONN);

	SOCKET client = accept(server, 0, 0);

	int byteRead = 0;
	char* buffer = malloc(BUFFER_SIZE);

	if (buffer == NULL) return 1;

	while (byteRead = recv(client, buffer, BUFFER_SIZE, 0)) {

		if (byteRead <= 0) break;

		for (int i = 0; i < byteRead; i++)
		{
			printf("%c\n", buffer[i]);
		}
	}

	return 0;
}

//buffer format
//[which player invoked the action 0 for server 1 for client][the action][the value]