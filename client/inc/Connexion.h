#ifndef CONNEXION_H
#define CONNEXION_H

#include <iostream>


using namespace std;
//PROTOTYPES 

class Connexion
{
public:
    Connexion(); //Constructor
    int openConnexion(); //Connect serveur or re connect to server. #1
    //int statrHandShake(); //Start handshake class and send multiple information.#2
    int main(bool is_admin, string path_prog); //Main function of client. #3

    int sendSafe(string &command); //Sends the data and manages possible errors. 
    int recvSafe(string &result,int i); //receives the data and manages possible errors. 
    void checkSend(int &iResult);
    int closeConnexion(); //Close connexion. 
    void reConnect(); //re connection to the server in case of problem.
    
    int getSocket(); //Ghetter of socket.
    int setAutoPeristence();

private:
    int sock_client=0; //Recupere le descripteur de fichier.
    bool a_auto_persitence;
};
#endif

