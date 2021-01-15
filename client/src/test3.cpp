#include <iostream> 
#include <windows.h>
#include <string>
#include <winsock2.h> 

using namespace std;
int main()
{
    
    STARTUPINFOA siStartInfo;
    PROCESS_INFORMATION piProcInfo;
    
   // siStartInfo.cb = sizeof(siStartInfo);
    //siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
    //siStartInfo.hStdInput= (HANDLE) sock;
    //siStartInfo.hStdOutput= (HANDLE) sock;
   // siStartInfo.hStdError=(HANDLE) sock;
    
    char cmd[] = "cmd.exe";
    char argument[] = "/C  dir";

    cout << "GO to create proc" << endl;
    ZeroMemory(&siStartInfo, sizeof(siStartInfo)); //Remplit un bloc de mémoire avec des zéros.
    CreateProcessA(NULL,  //command line 
        cmd,     // argv of cmd
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        TRUE,          // handles are inherited 
        0,             // creation flags 
        NULL,          // use parent's environment 
        NULL,          // use parent's current directory 
        &siStartInfo,  // STARTUPINFO pointer 
        &piProcInfo);  // receives PROCESS_INFORMATION
    cout << "CREATPROC ERR" << GetLastError() << endl;
    BOOL status = WaitForSingleObject(piProcInfo.hProcess, INFINITE);
    cout << "WAITFORSING ERR" << GetLastError() << endl;

    cout << "STATUS: " << status << endl;
    cout << "CREATE PROC: " << status << endl;

    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
}