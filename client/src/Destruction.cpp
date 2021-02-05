#include "../inc/Destruction.h"
#include "../inc/Exec.h"
#include "../inc/other.h"


using namespace std;


Destruction::Destruction(wstring name_prog, wstring path_prog)
{
    a_name_prog = name_prog; 
    a_path_prog = path_prog;

}


void Destruction::createBatchFile()
{
    wcout << "CREATEBATCH FILE: \n\n" <<endl;
    HANDLE hFile; //use in CreateFile and WriteFile
    //const char fileName[] = "lefichier.txt"; //use in CreateFile
    wstring content = L"@echo off\ntimeout 5\ndel " + a_path_prog + L"\n"; // use in WriteFile
    wstring command_attrib = L"attrib +h " + a_name_file_batch; ////command for hide the file batch

    DWORD dwBytesToWrite = (DWORD) content.length();// use in WriteFile
    DWORD dwBytesWritten = 0; // use in WriteFile
    BOOL status; // use in WriteFile

    hFile = CreateFileW(a_name_file_batch.c_str(),  GENERIC_WRITE,  0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL); //Crée ou ouvre un fichier ou un périphérique d'E / S. 
    cout << "\n\n\nfile Create !" << endl;
  
    cout << "\n\n" << endl;

   
    if (GetLastError() == ERROR_FILE_EXISTS)
    {
        cout << "ERROR_FILE_EXISTS" << endl;
        delete_batch_file();
        hFile = CreateFileW(a_name_file_batch.c_str(),  GENERIC_WRITE,  0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL); //Crée ou ouvre un fichier ou un périphérique d'E / S. 
        cout << "EST LA: " << GetLastError() << "    " << hFile << endl ;
        cout << "a_error:" << a_error << endl;

    }
    
    if(hFile == INVALID_HANDLE_VALUE)
    {
        //error when creating the batch file.
        cout << "impossible to open or write to the file. "<< GetLastError() << endl;
        a_error = TRUE;
    }//ERROR_ALREADY_EXISTS


    status = WriteFile(          //https://stackoverflow.com/questions/28618715/c-writefile-unicode-characters
                hFile,           // open file handle
                to_utf8(content).c_str(), // start of data to write
                dwBytesToWrite,  // number of bytes to write
                &dwBytesWritten, // number of bytes that were written
                NULL);           // no overlapped structure

    cout << "Status of WriteFile: " << status << endl;
    if(status == 0)
    {
        //If error
        a_error = TRUE;
    }
    CloseHandle(hFile);
    
    _wsystem(command_attrib.c_str()); //hide the file batch

}   


void Destruction::kills_all_same_process() 
{
    wcout << "kills_all_same_process : \n\n" <<endl;
    vector <DWORD> pids;
    string current_prog  = NAME_PROG; //To change for unicode 
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
        cout << "Empty pid" << endl;
        ;
    }
}


void Destruction::startBatchFile()
{
    wcout << "startBatchFile : \n\n" <<endl;

    PROCESS_INFORMATION piProcInfo; 
    STARTUPINFOW siStartInfo;
    BOOL bSuccess = FALSE; 

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION)); 
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));

    siStartInfo.cb = sizeof(STARTUPINFO); 


    wstring argument_string = L"/C " + a_name_file_batch + L" && del " + a_name_file_batch + L"/A";
    
    WCHAR argument[argument_string.length()+1];

    wcscpy(argument, argument_string.c_str());
    
    wcout << "argument: " << argument << endl;

    bSuccess = CreateProcessW(L"C:\\windows\\system32\\cmd.exe",  //command line 
        argument,     // argv of cmd
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        FALSE,          // handles are inherited 
        CREATE_NO_WINDOW,             // creation flags 
        FALSE,          // use parent's environment 
        NULL,          // use parent's current directory 
        &siStartInfo,  // STARTUPINFO pointer 
        &piProcInfo);  // receives PROCESS_INFORMATION
    cout << "finish: "<< bSuccess << endl;
    cout << GetLastError() << endl;

}


int Destruction::main()
{
    //-----------------------
    createBatchFile();
    if(testIfError())
    {
        return 1;
    }
    //-----------------------
    kills_all_same_process();
    if(testIfError())
    {
        return 1;
    }
    //system("pause");

    //-----------------------
    startBatchFile();
    if(testIfError())
    {
        return 1;
    }

    //-----------------------

    if(testIfError()) //test final
    {
        return 1;
    }
    else
    {return 0;}
}


int Destruction::testIfError()
{
    if(a_error)
    {
        cout << "error detect in testIfError (Destruction)" << endl;
        delete_batch_file();
        return 1;
    }
    else
    {return 0;}
}


void Destruction::delete_batch_file()
{
    //Test if file batch exist:
    cout << "Test if file batch exist: " << endl;

    DWORD status = GetFileAttributesW(a_name_file_batch.c_str());
    
    if(status == INVALID_FILE_ATTRIBUTES)
    {
        //THE FILE DOESN'T EXIST ANYMORE
        cout << "LE FICHIER N'EXISTE PLUS. " << endl; 
    }
    else
    {
        //THE FILE EXISTS 
        a_error = TRUE;
        //error (file not exist or file with higher rights)
        //system("pause");
        DeleteFileW(a_name_file_batch.c_str());
        cout << "file exist wala: " <<GetLastError()  << endl;
    }

}//https://stackoverflow.com/questions/4403986/c-which-is-the-best-method-of-checking-for-file-existence-on-windows-platform

Destruction::~Destruction()
{
    cout << "Destructor !!!!" << endl;
}