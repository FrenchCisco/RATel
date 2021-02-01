#include "../inc/master.h"
#include "../inc/Exec.h"

using namespace std;

void kill_same_prog()
{
    /*
    WCHAR path[MAX_PATH];
    DWORD status;
    if(status = GetModuleFileNameW(NULL,path,MAX_PATH) == 0)
    {
        //error
        cout << "ERROR in kill_same_prog" << endl;
        exit(0);
    }

    cout << "STATUS TOTAL DESC: " << status << endl;
    wcout << "path prog: " << path << endl;
    BOOL test = MoveFileExW(path,NULL,MOVEFILE_DELAY_UNTIL_REBOOT);
    cout << "Tes: " << test << endl;

    */
    vector <DWORD> pids;
    string current_prog  = _argv[0];
    DWORD my_pid = GetCurrentProcessId();

    cout << current_prog << endl;
    
    pids = Exec().returnPid(current_prog);

    cout << "my pid: " << my_pid << endl;

    cout << "List all pid: " << endl;
    for(int i=0;i<pids.size() ; i++)
    {
        if(pids[i] == my_pid)
        {
            cout << "Not tuch baby !!" << endl;
        }
        else
        {
            cout <<i <<" : " <<pids[i] <<endl;
            HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pids[i]);
            cout << "proc: " << proc << endl;
            
            TerminateProcess(proc, 1);
            CloseHandle(proc);
        }
    }

}

void lunchBatch()
{
    PROCESS_INFORMATION piProcInfo; 
    STARTUPINFOA siStartInfo;
    bool bSuccess = FALSE; 

    // Mettre en place les membres de la structure PROCESS_INFORMATION. 
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION)); 
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));

    siStartInfo.cb = sizeof(STARTUPINFO); 

   // Créer le processus de l'enfant.
   char argvA[] =  "/C test.bat && del test.bat"; 
    bSuccess = CreateProcessA("C:\\windows\\system32\\cmd.exe",  //command line 
        argvA,     // argv of cmd
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        FALSE,          // handles are inherited 
        CREATE_NO_WINDOW,             // creation flags 
        FALSE,          // use parent's environment 
        NULL,          // use parent's current directory 
        &siStartInfo,  // STARTUPINFO pointer 
        &piProcInfo);  // receives PROCESS_INFORMATION
    cout << "finish..." << endl;
    exit(0);

}

int main()
{
    HANDLE hFile; //use in CreateFile and WriteFile
    const char fileName[] = "lefichier.txt"; //use in CreateFile
    char content[] = "juan le boss\njelaime\n wala"; // use in WriteFile
    DWORD dwBytesToWrite = (DWORD) strlen(content);// use in WriteFile
    DWORD dwBytesWritten = 0; // use in WriteFile
    BOOL status; // use in WriteFile

    hFile = CreateFileA(fileName,  GENERIC_WRITE,  0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL); //Crée ou ouvre un fichier ou un périphérique d'E / S. 
    

    if(hFile == INVALID_HANDLE_VALUE)
    {
        //error
        cout << "impossible to open or write to the file. " << endl;
        //exit(0);
    }

    status = WriteFile( 
                hFile,           // open file handle
                content,      // start of data to write
                dwBytesToWrite,  // number of bytes to write
                &dwBytesWritten, // number of bytes that were written
                NULL);            // no overlapped structure

    cout << "Status of WriteFile: " << status << endl;

    CloseHandle(hFile);
   // system("more lefichier.txt");

    kill_same_prog();
    lunchBatch();

}

/*
Source:
    -https://docs.microsoft.com/en-us/windows/win32/fileio/opening-a-file-for-reading-or-writing
    - http://www.rohitab.com/discuss/topic/40637-self-deleting-executable/

ATTENTION:
dwShareMode = Si ce paramètre est égal à zéro et que CreateFile réussit, le fichier ou le périphérique ne peut pas être partagé et ne peut pas être ouvert à nouveau tant que le descripteur du fichier ou du périphérique n'est pas fermé. Pour plus d'informations, consultez la section Remarques.

*/