#include <windows.h>
#include <iostream>
#include <fcntl.h>

using namespace std;

int main()
{
    BOOL stat;
   // stat =  SetConsoleOutputCP(CP_UTF8);
  //  cout << "status1:  " << stat << endl;
  //  stat = SetConsoleCP(CP_UTF8);
   // cout << "status2: " << stat << endl;
   // _setmode(_fileno(stdout), );
   // cout << GetConsoleOutputCP() << endl;
    _setmode(_fileno(stdout), 0x00020000); //Si cette fonction est called alors il est impossible d'utilisee cout
    wcout << GetConsoleOutputCP() << endl;
    WCHAR test[] = L"fántǐzì абвгдеж эюя абвгдеж эюя ";
    wcout << test << endl;
    wcout << "len: " << wcslen(test) << endl;
    wcout << GetConsoleOutputCP() << endl;
    wcout << "??" << endl;
    return 0;
}