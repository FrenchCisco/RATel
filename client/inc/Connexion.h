#ifndef CONNEXION_H
#define CONNEXION_H

#include <iostream>
#include "../inc/common.h"
using namespace std;
//PROTOTYPES 

class Connexion
{
public:
    Connexion(); //Constructor
    int openConnexion(); //Connect serveur or re connect to server. 

    int main(bool is_admin, string path_prog); //Main function of client. #3

    int sendSafe(string command); //Sends the data and manages possible errors. 
    int recvSafe(string &result,int i); //receives the data and manages possible errors. 
    void checkSend(int &iResult);
    int closeConnexion(); //Close connexion. 
    void reConnect(); //re connection to the server in case of problem.
    
    int getSocket(); //Ghetter of socket.

    int setAutoPeristence(); //



private:
    int sock_client; //Recupere le descripteur de fichier.
    
};
#endif

