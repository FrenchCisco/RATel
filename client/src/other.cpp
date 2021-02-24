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

BOOL checkIfFileExist(wstring file_name)
{
    
    if(GetFileAttributesW(file_name.c_str()) == INVALID_FILE_ATTRIBUTES)
    {
        //File not found
        return FALSE;
    }
    else{return TRUE;}
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


wstring XOREncryption(wstring data)  
{
    wstring result;
    wstring char_xor;
   // const wstring key =L"" XOR_KEY;
    const wstring key =L"ba38fab1b592c8f4b1fbbe8d915ee009a840efe504e3a833d0d8afae3b9ea148c859a8e8441877a0721d54f92fccbf12241007253cd6468194484e310798c01d8874dab3c4daa2c3ad612b7ba79ddc5b7ae80de0190f6842d6540da4b5b66c08987f8a15b3f5b80c588e42d1b6c9f149e99cc3c145cd31edaa25b095a6d509df93743a4861929533582d418aa3a766de439e938b7302001e17a6b4691480c4520a5e3a3b31012a003f7ced0ae59936953356f04b92b0e81b9f5b3703130d98118df5bdcc57e0f632b01b443a02bd23fc2f83ca27f9be70cdc657e8f4d74095f5dd88a4c88519db28a45e2e9e4ff714715d6ba53f1a88579f7f47e421b4a3526d40ead58521fbc0b56d373dce956fbb5a7d85f44e24085289b02e4782c3ed1154277fdc56467c1fe08a89d43e96ad2e73f4246c4e9238d660cf85dbc034543a13830dafb383ba61999de8eaee11ac9fa69b170461b241be69e0213e2a3f860edb29d3a1baf0f8a31b577d5893f13ca7377f0aa1bc433ff212e1e7c11cb2ad210df750cdcb6b3202655b215f7abca0f7f85803444e4fb06766a668975e7aa216abc8fdcda3bc7e2797a819b4a1eeccba75bdb49e2e0832ab171c73911362f1384480ca3152399c52b14deefc20a7ddb5e7cc9379688b37af6a42d54a32a85c20b86c4aa86361757e34e66ab0c13e7270788dc097d3db859b0474e4305dc5ffb51d";
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


