#ifndef CONNEXION_H
#define CONNEXION_H

#include "../inc/master.h"

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
        VOID sendSafe(vector<string> result_of_command); //send data and manage errors
        wstring recvSafe(); //receives the data and manages possible errors.  and return result
        VOID checkSend(INT &iResult);
        
        VOID reConnect(); //re connection to the server in case of problem.
        VOID closeConnexion(); //Close connexion.

        SOCKET getSocket(); //Ghetter of socket.


    private:

        SOCKET sock_client; 
        wstring a_token;
        BOOL a_is_admin;
        wstring a_path_prog;

};
#endif

