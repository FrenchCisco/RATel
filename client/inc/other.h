#ifndef OTHER_H
#define OTHER_H

#include "../inc/master.h"

using namespace  std;


void sendUltraSafe(int sock, wstring data); //for HandShake or reconnect !!! Use XOREncryption !!!

wstring generateToken(int length);
wstring XOREncryption(wstring data);
wstring getPath();

int changeDirectory(wstring path);
int totalDestruction();

string to_utf8(const wstring &s);

#endif