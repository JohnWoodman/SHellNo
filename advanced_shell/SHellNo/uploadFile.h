#pragma once
#include <fstream>

int uploadFile(SOCKET ClientSocket, char* file_path) {
	printf("Uploading File %s\n", file_path);
	
	std::ofstream out(file_path, std::ios::binary);
	while (out.is_open()) {
		char* recvbuf = new char[1024];
		int bytes = recv(ClientSocket, recvbuf, 1024, 0);

		int len_recv = strlen(recvbuf);
		printf("Length of rcvbuf is %d\n", len_recv);
		if (len_recv > 0) {
			printf("Writing to file\n");
			out.write(recvbuf, len_recv);
			memset(recvbuf, '\0', 1024);
		}
		else {
			printf("Closing file error: %s\n", strerror(errno));
			out.close();
			break;
		}

		if (len_recv < 1024) {
			printf("Last section of file\n");
			out.close();
			break;
		}
	}
	printf("Finished upload");

	return 0;
}