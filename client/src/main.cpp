#include "../inc/Connexion.h"
#include "../inc/HandShake.h"


int main()
{
    
   // ShowWindow(GetConsoleWindow(),SW_HIDE);

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
    
}
