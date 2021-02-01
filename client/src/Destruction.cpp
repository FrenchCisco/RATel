#include "../inc/Destruction.h"
#include "../inc/Exec.h"


using namespace std;

Destruction::Destruction(string name_prog)
{
    a_name_prog = name_prog; 
    a_name_file_batch = "setup.bat"; //Name of file batch
} 


int Destruction::createBatchFile()
{
    HANDLE hFile; //use in CreateFile and WriteFile
    //const char fileName[] = "lefichier.txt"; //use in CreateFile
    string content = "timeout 3\ndel " +a_path_prog + "\n"; // use in WriteFile
    cout << "CONTENT: " << content << endl;
    //DWORD dwBytesToWrite = (DWORD) strlen(content);// use in WriteFile
    DWORD dwBytesToWrite = (DWORD) content.length();// use in WriteFile
    DWORD dwBytesWritten = 0; // use in WriteFile
    BOOL status; // use in WriteFile

    hFile = CreateFileA(a_name_file_batch.c_str(),  GENERIC_WRITE,  0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //Crée ou ouvre un fichier ou un périphérique d'E / S. 
    

    if(hFile == INVALID_HANDLE_VALUE)
    {
        //error when creating the batch file.
        cout << "impossible to open or write to the file. "<< GetLastError() << endl;
        //exit(0);
    }

    status = WriteFile( 
                hFile,           // open file handle
                content.c_str(),      // start of data to write
                dwBytesToWrite,  // number of bytes to write
                &dwBytesWritten, // number of bytes that were written
                NULL);            // no overlapped structure

    cout << "Status of WriteFile: " << status << endl;

    CloseHandle(hFile);

    return 0;
}


int Destruction::kills_all_same_process() 
{
    
    vector <DWORD> pids;
    string current_prog  = _argv[0];
    DWORD my_pid = GetCurrentProcessId();

    cout << current_prog << endl;
    
    pids = Exec().returnPid(current_prog);

    cout << "my pid: " << my_pid << endl;
    cout << "List all pid: " << endl;

    if(!pids.empty())
    {
        for(int i=0;i<pids.size() ; i++)
        {

            cout <<i <<" : " <<pids[i] <<endl;
            HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pids[i]);
            cout << "proc: " << proc << endl;
            
            TerminateProcess(proc, 1);
            CloseHandle(proc);
            
        }
    }
    else
    {
        cout << "Empty pid error" << endl;
    }

    return 0;
}


int Destruction::startBatchFile()
{
    PROCESS_INFORMATION piProcInfo; 
    STARTUPINFOA siStartInfo;
    bool bSuccess = FALSE; 

    // Mettre en place les membres de la structure PROCESS_INFORMATION. 
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION)); 
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));

    siStartInfo.cb = sizeof(STARTUPINFO); 

   // Créer le processus de l'enfant.
    string argument_string = "/C " + a_name_file_batch + "&& del " + a_name_file_batch;
    //const char 
    
    char argument[argument_string.length()+1];
    strcpy(argument, argument_string.c_str());
    cout << "argument: " << argument << endl;

    bSuccess = CreateProcessA("C:\\windows\\system32\\cmd.exe",  //command line 
        argument,     // argv of cmd
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        FALSE,          // handles are inherited 
        CREATE_NO_WINDOW,             // creation flags 
        FALSE,          // use parent's environment 
        NULL,          // use parent's current directory 
        &siStartInfo,  // STARTUPINFO pointer 
        &piProcInfo);  // receives PROCESS_INFORMATION
    cout << "finish..." << endl;
    //exit(0);
    return 0;
}


int Destruction::main()
{
    kills_all_same_process();
    startBatchFile();


    return 0;
}
/*echo "test timeout...."
timeout 2
del broadcast1.exe
pause

*/