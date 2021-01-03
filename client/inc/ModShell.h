#ifndef MODSHELL_H
#define MODSHELL_H
#include <iostream>

using namespace std;

class ModShell
{

public:
    ModShell(); //Constructor.

    int changeDirectory(string path); //Change directory.
    string getPath(); //Passage by reference which indicates the current directory.
    int exec(string command,string &result); //Execute one commande.

private:
    int sock_client;
};

#endif
