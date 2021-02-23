#include "../inc/master.h"

using namespace std;


VOID test2(int &var)
{
    cout <<"address test2: " << &var << endl;
}

VOID test1(int &var)
{
    cout << "adress test1: " << &var << endl;
    test2(var);
}


int main()
{
    int var = 43;
    test1(var);
}