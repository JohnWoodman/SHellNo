#pragma once
#include <fstream>

int downloadFile(SOCKET ClientSocket, char* file_path) {
	printf("Downloading File %s\n", file_path);
	std::streampos filesize = 0;
	std::ifstream in(file_path, std::ios::binary);
	char* sendbuf = new char[1024];
	int sendbuflen = 1024;
	ZeroMemory(sendbuf, sendbuflen);

	if (!in.is_open()) {
		printf("Error Opening File\n");
		send(ClientSocket, "1: Error Opening File\n", sendbuflen, 0);
		ZeroMemory(sendbuf, sendbuflen);
		return 1;
	}
	else {
		send(ClientSocket, "0", sendbuflen, 0);
		ZeroMemory(sendbuf, sendbuflen);
	}

	while (in.is_open()) {
		printf("File is open\n");
		in.read(sendbuf, sendbuflen);
		if (in.eof()) {
			printf("End of file\n");
			send(ClientSocket, sendbuf, sendbuflen, 0);
			ZeroMemory(sendbuf, sendbuflen);
			in.close();
		}
		else {
			printf("Sending file\n");
			send(ClientSocket, sendbuf, sendbuflen, 0);
			ZeroMemory(sendbuf, sendbuflen);
		}
	}
	return 0;
}