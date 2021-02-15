#include <iostream>
#include <vector>
#include <string> 

#include <windows.h>


#include <fcntl.h>
#include <io.h>
#include <stdio.h>
using namespace std;

wstring ConvertUtf8ToWide(const string& str)
{
    int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
    wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
    return wstr;
}

int main()
{
    char buff[4096] = {0};
    const char name_of_file[] = "test.txt";
    HANDLE hFile; 
    OVERLAPPED ol = {0};
    DWORD  dwBytesRead = 0;

    hFile = CreateFileA(name_of_file, FILE_APPEND_DATA | FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);

     cout << "get last " << GetLastError() << endl;

    ZeroMemory(&buff, sizeof(buff));
    ReadFile(hFile, buff, 4096 , &dwBytesRead,NULL);

    cout << "Buff: " << buff << endl;
    cout << "get last " << GetLastError() << endl;

    wstring result = ConvertUtf8ToWide((string) buff);
    _setmode(_fileno(stdout), 0x00020000); //Si cette fonction est called alors il est impossible d'utilisee cout
    wcout << result << endl;
    return 0;
}
