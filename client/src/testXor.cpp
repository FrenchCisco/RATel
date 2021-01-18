
#include <iostream>
#include <stdio.h>
#include <string>
#include <windows.h>
#include <direct.h> //_chdir

using namespace std;


const char key[] = "juanleboss";

string XORData(string data)
{
    
    string result;
    //cout << "Key: " << key << endl;
    //cout << "strlen: " << strlen(key) << endl;
    //cout << "size: " << sizeof(key) << endl;
    for(int i=0;i<data.size(); i++)
    {
        result += data.at(i)^key[i];

    }
    cout << "result: " << result << endl;
    cout << result.size() << endl;

    return result;
//Source: https://www.cprogramming.com/tutorial/xor.html
}

string decryptData(string data)
{
    string result;
    return result;
    ;
}

int main()
{   
    while (true)
    {
        string input="";

        cout <<"data: ";getline(cin, input);
        string data_encrypted = encryptData(input);
        cout << data_encrypted << endl;
        encryptData(data_encrypted);
    }
    return 0;
}