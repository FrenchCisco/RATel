#ifndef CONNEXION_H
#define CONNEXION_H

#include <iostream>
#include "../inc/common.h"
using namespace std;


class Connexion
{
public:
    Connexion(); 
    int openConnexion(); //Connect serveur or re connect to server. 

    int main(bool is_admin, string path_prog); //Main function of client. #3
    void sendSafe(string command); //send data and manage errors
    string recvSafe(int i); //receives the data and manages possible errors.  and return result
    void checkSend(int &iResult);
     
    void reConnect(); //re connection to the server in case of problem.
    void closeConnexion(); //Close connexion.

    int getSocket(); //Ghetter of socket.

    int setAutoPeristence(); //



private:
    int sock_client; //Recupere le descripteur de fichier.
    
};
#endif

