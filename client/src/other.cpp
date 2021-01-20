#include "../inc/other.h"
#include "../inc/common.h"

#include <iostream>
#include <stdio.h>
#include <string>
#include <windows.h>
#include <direct.h> //_chdir




int changeDirectory(string path)
{
//    cout << path.substr(3,path.length()) << endl;
    if(_chdir((path.substr(3,path.length())).c_str())!=0)
    {
        return 1;
    }
    return 0;
}

string getPath()
{
    char* buffer;
   // Get the current working directory:
    if ( (buffer = _getcwd( NULL, 0 )) == NULL )
        ;
    string path = buffer;
    return path;
}


void sendUltraSafe(int sock, string data) //Just for HandShake or reconnect | !!! Use XOREncryption !!!
{
    int len_send=0;
    int len_recv=0;
    char buffer[BUFFER_LEN];
    string result,tmp;
    timeval timeout;

   
    if(strlen(buffer) > 0)
    {
        //clean buffer
        cout << "Clean buffer in sendUltrasafe " << endl;
        memset(buffer, 0, sizeof(buffer));
    }
    cout << "strlen in send ultra safe: "<< strlen(data.c_str()) << endl; // //les deux sont de la meme taille 
    cout << "size:  send ultra safe" << data.size() << endl; //les deux sont de la meme taille  sauf path prog et is admin

    len_send = send(sock, data.c_str(), data.size(), 0); /// !!!warning !! 
    cout << "SEND IN ULTRASAE \n" << endl;
    if(len_send == SOCKET_ERROR)
    {
        //error
        cout << "error " << endl;
    }
    else
    {   
        struct fd_set fds;

        FD_ZERO(&fds);
        FD_SET(sock, &fds);

        timeout.tv_sec = TIMEOUT_SOCK;
        timeout.tv_usec = 0;

        int selectSock = select(0, &fds, 0, 0, &timeout);
        if(selectSock > 0)
        {
            len_recv = recv(sock, buffer, sizeof(buffer), 0);
            tmp = buffer;
            result = XOREncryption(tmp);
            //cout << "buffer in sendultrasafe: " << result << endl;
            //cout << "Buffer len sendultrasafe: " << result.length() << "|"<< strlen("confirmation") << endl;

            if(result == "\r\n")
            {
                //;
                cout << "\n\n[++]confirmation ok ! in sendUltraSafe" << endl;
            }
            
            else
            {
                cout << "ERROR in sendUltraSafe confirmation: "  << endl;
                cout << "data corompue: " << result << endl;
                cout << "size data cormp: " << result.size() << endl;
                cout << "len_recv "<< len_recv << endl; //ok len recv = 2
            }
        }
        else if (selectSock == 0)
        {
            //timeout
            ;        
        }
    }
    result.erase(); //TESTTTTT

    cout << "\n\n\n" << endl;
}

string XOREncryption(string data) //Do not use strlen on XOREncryption
{
    string result;
    string char_xor;
    string key = XOR_KEY;

   // cout << "Before encrypt: "<< data << " <-----" << endl;
    //cout << "in XOREncryption !" << endl;
    if(data.empty())
    {
        return result;
    }
    //cout << key << endl;
    //cout << "size key: " << key.size() << endl;
   // cout << "size data: " << data.size() << endl;
    for(int i=0;i<data.size(); i++)
    {
        char_xor = data.at(i) ^ key[i % key.size()];
        result += char_xor;   
    }
    //cout << "size result: " << result.size() << endl;
    return result;

}//Source: https://www.cprogramming.com/tutorial/xor.html

