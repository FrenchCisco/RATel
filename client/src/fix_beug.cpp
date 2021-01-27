#include "../inc/Exec.h"

int main()
{
    Exec exec;
    vector<string> result;
    int i=0;
    
    string input;
    while (true)
    {
        /* code */
        input.erase();
        getline(cin,input);
        cout << "commande execute: " << input << endl;
        result = exec.executeCommand(input);
        for(i=0;i<result.size();i++)
        {
            cout << result[i];
        }
        
    }
    return 0;
}