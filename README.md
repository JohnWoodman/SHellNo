# SHellNo
This is the preliminary work for the SHellNo C2 framework

## ToDo

### Framework

### Shellcode Injection
- ~~Write code that reads syscall number from ntdll~~

- Create class object that can be used for any syscall

- Write download and upload file functions

- ~~Create header file for retrieving syscall from ntdll (modularization)~~

- ~~Write program that injects shellcode given shellcode file and PID (or option to start own process)~~

- *Write reverse shell that uses only syscalls (no shellcode) [having issue with RTL_USER_PROCESS_PARAMETERS attributes]

- Look into converting existing enumeration tools into shellcode (may not be possible, might have to rewrite certain tools to use direct syscalls)

- Look into bypassing Kernel land hooking (Sysmon)
