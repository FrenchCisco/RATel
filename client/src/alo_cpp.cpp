#include "../inc/Exec.h"


using namespace std;


int main()
{
    string input;
    while(true)
    {
        
        cout << ">"; getline(cin,input);
        Exec().executeCommand(input);
        input.erase();
    }
}