#include "../inc/other.h"
#include "../inc/common.h"


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
        //cout << "Clean buffer in sendUltrasafe " << endl;
        //memset(buffer, 0, sizeof(buffer));
        ZeroMemory(&buffer, strlen(buffer));
    }

    len_send = send(sock, data.c_str(), data.length(), 0); /// !!!warning !! 
    if(len_send == SOCKET_ERROR)
    {
        //error
        ;
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

            if(result == "\r\n")
            {
                ;
                //cout << "\n\n[++]confirmation ok ! in sendUltraSafe" << endl;
            }
            
        }
        else if (selectSock == 0)
        {
            //timeout
            ;        
        }
    }
    result.erase(); 
}


string XOREncryption(string data) //Do not use strlen on XOREncryption
{
    string result;
    string char_xor;
    string key = XOR_KEY;

    if(data.empty())
    {
        return result;
    }

    for(int i=0;i<data.size(); i++)
    {
        char_xor = data.at(i) ^ key[i % key.size()];
        result += char_xor;   
    }

    return result;

}//Source: https://www.cprogramming.com/tutorial/xor.html

