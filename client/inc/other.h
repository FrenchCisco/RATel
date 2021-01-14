#ifndef OTHER_H
#define OTHER_H
#include <iostream>
#include <windows.h>
#include <string>

using namespace  std;


string recvUltraSafe(int sock);  //Ensures that data is not corrupted and handles errors.
void sendUltraSafe(int sock, string data);
string generateToken(int length);


string exec(string command);
string getPath();
int changeDirectory(string path);


DWORD WINAPI popenThread(void *param);

struct THREAD_POPEN //the
{
    string command;
    string result;
    bool timeout =false;
    FILE *test;
};


#endif