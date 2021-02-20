#ifndef OTHER_H
#define OTHER_H

#include "../inc/master.h"

using namespace  std;


VOID sendUltraSafe(SOCKET &sock, wstring data); //for HandShake or reconnect !!! Use XOREncryption !!!

string generateToken(INT length);
wstring XOREncryption(wstring data); //allows you to know whether the function encrypts or decrypts. encrypt (TRUE) = encrypt function
wstring getPath();

INT changeDirectory(wstring path);
INT totalDestruction();

string ConvertWideToUtf8(const wstring &s);
wstring ConvertUtf8ToWide(const string &str); //https://stackoverflow.com/questions/6693010/how-do-i-use-multibytetowidechar


#endif