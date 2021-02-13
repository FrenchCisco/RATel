#ifndef CONNEXION_H
#define CONNEXION_H

#include "../inc/master.h"

using namespace std;


class Connexion
{
    public:

        Connexion(); 
        
        int openConnexion(); //Connect serveur or re connect to server. 

        //int main(bool is_admin, string path_prog); //Main function of client. #3
        void setToken(wstring token); //set a_token
        void setIsAdmin(BOOL is_admin);// set a_is_admin
        void setPathProg(wstring path_prog); // set a_path_prog
        int setAutoPeristence(); //


        int main(); //Main function of client. #3
        void sendSafe(vector<wstring> result_of_command); //send data and manage errors
        wstring recvSafe(); //receives the data and manages possible errors.  and return result
        void checkSend(int &iResult);
        
        void reConnect(); //re connection to the server in case of problem.
        void closeConnexion(); //Close connexion.

        int getSocket(); //Ghetter of socket.


    private:

        int sock_client; 
        wstring a_token;
        BOOL a_is_admin;
        wstring a_path_prog;


};
#endif

