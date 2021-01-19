
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
        result += data.at(i)^key[i % strlen(key)];
    }
    cout << "result: " << result << endl;
    cout << result.size() << endl;

    return result;
    
/*
Source: 
- https://www.cprogramming.com/tutorial/xor.html
- https://github.com/KyleBanks/XOREncryption/blob/master/C%2B%2B/main.cpp
*/
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
        string data_encrypted = XORData(input);
        cout << data_encrypted << endl;
        cout << XORData(data_encrypted) << endl;
    }
    return 0;
}