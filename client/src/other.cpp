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


void sendUltraSafe(int sock, wstring data) //Just for HandShake or reconnect | !!! Use XOREncryption !!!
{
    int len_send=0;
    int len_recv=0;
    WCHAR buffer[BUFFER_LEN];
    wstring result,tmp;
    timeval timeout;

   
    if(wcslen(buffer) > 0)
    {
        //clean buffer
        //cout << "Clean buffer in sendUltrasafe " << endl;
        //memset(buffer, 0, sizeof(buffer));
        ZeroMemory(&buffer, wcslen(buffer));
    }

    len_send = send(sock, (char *)data.c_str(), data.length() * sizeof(WCHAR), 0); /// !!!warning !! 
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
            len_recv = recv(sock, (char *)buffer, sizeof(buffer), 0);
            tmp = buffer;
            result = XOREncryption(tmp);

            if(result == L"\r\n")
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


wstring XOREncryption(wstring data) //Do not use strlen on XOREncryption
{
    wstring result;
    wstring char_xor;
    wstring key = L"" XOR_KEY;

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


string to_utf8(const wstring &s)
{
    /*
    wstring_convert<codecvt_utf8_utf16<wchar_t>> utf16conv;
    return utf16conv.to_bytes(s);
    */

    string utf8;
    int len = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), s.length(), NULL, 0, NULL, NULL);
    if (len > 0)
    {
        utf8.resize(len);
        WideCharToMultiByte(CP_UTF8, 0, s.c_str(), s.length(), &utf8[0], len, NULL, NULL);
    }

    return utf8;
}