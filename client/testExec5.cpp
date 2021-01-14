#include <iostream>
#include <string>
#include <windows.h>

#include "Exec.h"

using namespace std;

int main()
{
    while (true)
    {  
        string input;
        cout << "SHELL: ";getline(cin, input);

        string result = Exec().executeCommand(input);
        //Exec exec;
        //string result = exec.executeCommand(input);
        cout << "\n\nRESULT : " << result << endl;
    }
}