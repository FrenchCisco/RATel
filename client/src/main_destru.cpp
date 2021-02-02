#include "../inc/Destruction.h"

int main()
{
    Destruction destruction(L"destruction.exe"); 

    int status = destruction.main();
    cout << status << endl;
    return 0;
}
