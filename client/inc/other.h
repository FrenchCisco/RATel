#ifndef OTHER_H
#define OTHER_H

#include "../inc/master.h"

using namespace  std;


void sendUltraSafe(int sock, string data); //for HandShake or reconnect !!! Use XOREncryption !!!

string generateToken(int length);
string XOREncryption(string data);
wstring getPath();

int changeDirectory(wstring path);
int totalDestruction();

string ConvertWideToUtf8(const wstring &s);
wstring ConvertUtf8ToWide(const string& str); //https://stackoverflow.com/questions/6693010/how-do-i-use-multibytetowidechar


#endif