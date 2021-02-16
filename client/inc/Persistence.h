#ifndef PERSI_H
#define PERSI_H

#include "../inc/master.h"

using namespace std;

class Persistence
{//Change the registry key according to the user's permission.
    public:
    
        Persistence();
        Persistence(bool is_admin, wstring path_prog);
        INT customPersi(); //Comming soon
        
        VOID main();
               
    private:
        BOOL a_is_admin; //Declaration attribut of constuctor.
        wstring a_path_prog;
};
#endif