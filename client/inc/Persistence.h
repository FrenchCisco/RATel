#ifndef PERSI_H
#define PERSI_H

#include <iostream>
#include <string>


using namespace std;

class Persistence
{//Change the registry key according to the user's permission.
    public:
        Persistence();
        Persistence(bool is_admin,string path_prog);
        int defaultPersi(); //Create persistence with the default directories.
        int customPersi(); //FUTUR
        
               
    private:
        bool a_is_admin; //Declaration attribut of constuctor.
        string a_path_prog;
};
#endif