#ifndef PERSI_H
#define PERSI_H

#include "../inc/master.h"

using namespace std;

class Persistence
{//Change the registry key according to the user's permission.
    public:
    
        Persistence();
        Persistence(bool is_admin,string path_prog);
        int customPersi(); //Comming soon
        
        void main();
               
    private:
        bool a_is_admin; //Declaration attribut of constuctor.
        string a_path_prog;
};
#endif