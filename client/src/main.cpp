#include "../inc/Connexion.h"
#include "../inc/HandShake.h"


int main()
{   
    ShowWindow(GetConsoleWindow(),SW_HIDE);

    SetConsoleOutputCP(CP_UTF8); 
    _setmode(_fileno(stdout), 0x00020000);  
    _setmode(_fileno(stdin), 0x00020000);

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
