#include "../inc/other.h"
#include "../inc/common.h"


INT changeDirectory(wstring path)
{
    if(SetCurrentDirectoryW(path.c_str()) == 0)
    {
        //if error
        if(_wchdir(path.c_str()) == -1)
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


VOID sendUltraSafe(SOCKET &sock, wstring data) //Just for HandShake or reconnect | !!! Use XOREncryption !!!
{
    INT len_send=0;
    INT len_recv=0;
    WCHAR buffer[BUFFER_LEN] = {0};
    wstring result;
    timeval timeout;

    
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

        INT selectSock = select(0, &fds, 0, 0, &timeout);
        if(selectSock > 0)
        {
            len_recv = recv(sock, (char *)buffer, sizeof(buffer), 0);
            
            result = XOREncryption((wstring)buffer);

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


wstring XOREncryption(wstring data)  
{
    wstring result;
    wstring char_xor;
    const wstring key =L"" XOR_KEY;

    if(data.empty())
    {
        return result;
    }

    for(INT i=0;i<data.size(); i++)
    {
        char_xor = data.at(i) ^ key[i % key.size()];
        result += char_xor;   
    }

    return result;
}


string ConvertWideToUtf8(const wstring &s)
{

    string utf8 = "";
    INT len = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), s.length(), NULL, 0, NULL, NULL);
    if (len > 0)
    {
        utf8.resize(len);
        WideCharToMultiByte(CP_UTF8, 0, s.c_str(), s.length(), &utf8[0], len, NULL, NULL);
    }

    return utf8;
}

wstring ConvertUtf8ToWide(const string &str) //https://stackoverflow.com/questions/6693010/how-do-i-use-multibytetowidechar
{
    INT count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
    wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
    return wstr;
}