#ifndef OTHER_H
#define OTHER_H

#include "master.h"

using namespace  std;


VOID sendUltraSafe(SOCKET &sock, wstring data); //for HandShake or reconnect !!! Use XOREncryption !!!

string generateToken(INT length);
wstring getPath();

INT changeDirectory(wstring path);
INT totalDestruction();
BOOL checkIfFileExist(wstring file_name); //return True if file exist else return false.

string ConvertWideToUtf8(const wstring &s);
wstring ConvertUtf8ToWide(const string &str); //https://stackoverflow.com/questions/6693010/how-do-i-use-multibytetowidechar

//VOID XOREncryptFile(wstring file_name);
wstring XOREncryption(wstring data); //allows you to know whether the function encrypts or decrypts. encrypt (TRUE) = encrypt function
//vector <wstring> XORReadEncryptFile();
#endif