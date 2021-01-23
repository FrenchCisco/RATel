#ifndef OTHER_H
#define OTHER_H

#include "../inc/master.h"

using namespace  std;



void sendUltraSafe(int sock, string data); //for HandShake or reconnect !!! Use XOREncryption !!!

string generateToken(int length);

string XOREncryption(string data);

string getPath();
int changeDirectory(string path);

#endif