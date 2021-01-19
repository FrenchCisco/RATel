
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
    string char_xor;

    for(int i=0;i<data.size(); i++)
    {
        char_xor = data.at(i)^key[i % (sizeof(key) / sizeof(char))];
        
        cout <<"KEY: "<< key[i % (sizeof(key) / sizeof(char))] << endl;
        cout << "result calcul: "  << i % (sizeof(key) / sizeof(char)) << endl;
        cout << "char: " << data.at(i) << " char_xor: " << char_xor << " CMPT: " << i << endl;
        result += char_xor;
        if(char_xor.at(0) == ' ')
        {cout <<"erro"<<endl;}
        
        cout <<" \n" << endl;

    }

    cout << "-------------___>"<< result <<"<-------" << endl;
    system("PAUSE");
    system("cls");
    
    return result;
}
s
/*
Source: 
- https://www.cprogramming.com/tutorial/xor.html
- https://github.com/KyleBanks/XOREncryption/blob/master/C%2B%2B/main.cpp
*/



int main()
{   
    while (true)
    {
        string input="";

        cout <<"data: ";getline(cin, input);
        string data_encrypted = XORData(input);
        
       

        ///cout << XORData(data_encrypted) << endl;
    }
    return 0;
}
/*
Source:
https://stackoverflow.com/questions/1135186/whats-wrong-with-xor-encryption

*/