#include <windows.h>
#include <iostream>

#include <fcntl.h>
#include <io.h>
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
    SetConsoleOutputCP(CP_UTF8); 
    _setmode(_fileno(stdout), 0x00020000);  
    _setmode(_fileno(stdin), 0x00020000);


    WCHAR test[] = L"fántǐzì абвгдеж эюя абвгдеж эюя ";
    wcout << test << endl;
    cout << GetConsoleCP() << endl;
    wcout << GetConsoleOutputCP() << endl; // 0x00040000 not found 
    wstring input;
    getline(wcin, input);

    wcout << "--> " << input << endl;
    


    system("PAUSE");
    return 0;
    
}

/*
Note:
 _setmode(_fileno(stdout), 0x00040000) = utf16 ne fonctionne pas.


*/