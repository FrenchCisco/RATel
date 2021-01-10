
#include <windows.h>
#include <Winerror.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc,char** argv)
{
    string cmd ="ls";

    WCHAR command[] = L"C:\\Windows\\System32\\cmd.exe /c dir";
    WCHAR cmdPath[] = L"C:\\Windows\\System32\\cmd.exe";

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);

    ZeroMemory( &pi, sizeof(pi) );
    //CreateProcessAutilise des chaînes de caractères ANSI char*etSTARTUPINFOA
    // Start the child process. 

    if(CreateProcessW(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) //https://stackoverflow.com/questions/46831863/how-do-i-call-the-unicode-function-createprocessw-in-c-to-launch-a-windows-exe
    {
        WaitForSingleObject(pi.hProcess, INFINITE); 
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

        //https://docs.microsoft.com/en-us/windows/win32/procthread/creating-processes
        //https://stackoverflow.com/questions/39702655/how-to-execute-a-command-in-cmd-using-createprocess
    }