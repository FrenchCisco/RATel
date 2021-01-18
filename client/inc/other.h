#ifndef OTHER_H
#define OTHER_H
#include <iostream>
#include <windows.h>
#include <string>

using namespace  std;



void sendUltraSafe(int sock, string data); //for HandShake or reconnect

string generateToken(int length);

string XORData(string data);

string getPath();
int changeDirectory(string path);

#endif