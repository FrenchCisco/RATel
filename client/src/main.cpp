#include "../inc/Connexion.h"
#include "../inc/HandShake.h"



int main()
{
    
   // ShowWindow(GetConsoleWindow(),SW_HIDE);
    
    BOOL stat1 =SetConsoleOutputCP(CP_UTF8); //OK 

    int stat = _setmode(_fileno(stdout), 0x00020000);  //OK
    
    wcout << "stat setmod: " << stat << endl;
    WCHAR test[]= L"Ελληνικά";

    
   // BOOL stat2 =SetConsoleCP(CP_UTF8); //NO FOUND 

    
    wcout << "SetConsoleOutputCP: " << GetConsoleOutputCP() << endl;
    wcout << "stat: " << stat1 << endl;

    wcout << "SetConsoleCP: " << GetConsoleCP() << endl;
    //wcout << "stat2: " << stat2 << endl;

    wcout << "text: " << test << endl;
    wcout << "getlasterror"<< GetLastError() << endl;
    
    Connexion Client;  
    HandShake Handshake;

    Client.openConnexion();

    Handshake.setSock(Client.getSocket());
    Handshake.beforeHandShake();
    Handshake.startHandShake(); 

    Client.setToken(Handshake.getToken());
    Client.setIsAdmin(Handshake.getIsAdmin());
    Client.setPathProg(Handshake.getPathProg());
    Client.main();

    Client.closeConnexion();
    
    return 0;
}
