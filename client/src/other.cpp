#include "../inc/other.h"
#include "../inc/common.h"


int changeDirectory(wstring path)
{

    if(SetCurrentDirectoryW(path.c_str()) != 0)
    {
        //if error
        if(_wchdir(path.c_str()) != 0)
        {
            //if error
            ; // TO change
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}


wstring getPath()
{
    WCHAR path[MAX_PATH];

    if(GetCurrentDirectoryW(MAX_PATH, path) == 0)
    {
        //error TO CHANGE
        return (wstring) L"";
    } 
    else
    
    {return (wstring) path;}

}


void sendUltraSafe(int sock, string data) //Just for HandShake or reconnect | !!! Use XOREncryption !!!
{
    int len_send=0;
    int len_recv=0;
    CHAR buffer[BUFFER_LEN];
    string result;
    timeval timeout;

    ZeroMemory(&buffer, strlen(buffer)); //Clean buff
    
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
            
            result = XOREncryption((string)buffer);

            if(result == "\r\n")
            {
                ;//confirmation ok !
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
    //wstring key = L"" XOR_KEY;
    string key = XOR_KEY;

    wcout << "\n\n--------------" << endl;
    wcout << data.length() << endl;
    if(data.empty())
    {
        return result;
    }

    for(int i=0;i<data.size(); i++)
    {
        char_xor = data.at(i) ^ key[i % key.size()];
        cout << "i: "<< i << " "<< char_xor << endl;
        result += char_xor;   
    }

    return result;

}//Source: https://www.cprogramming.com/tutorial/xor.html


string ConvertWideToUtf8(const wstring &s)
{

    string utf8;
    int len = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), s.length(), NULL, 0, NULL, NULL);
    if (len > 0)
    {
        utf8.resize(len);
        WideCharToMultiByte(CP_UTF8, 0, s.c_str(), s.length(), &utf8[0], len, NULL, NULL);
    }

    return utf8;
}

wstring ConvertUtf8ToWide(const string& str) //https://stackoverflow.com/questions/6693010/how-do-i-use-multibytetowidechar
{
    int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
    wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
    return wstr;
}