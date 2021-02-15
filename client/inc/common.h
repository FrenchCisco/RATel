
#ifndef COMMON_H
#define COMMON_H
//default header
#define BUFFER_LEN 4096

//Change everything according to your wishes:
#define IP_ADDRESS "192.168.0.98" //Ip of server
#define PORT 8888 //Port of server
#define AUTO_PERSISTENCE false
#define TIMEOUT 3000//Seconds for reconnect to server during a disconnection
#define NAME_KEY_REGISTER  "win64" 
#define XOR_KEY "123456789" //The key to encrypt and decrypt data using the XOR algorithm

#define AUTO_MOVE false //if this is true then the program automatically moves to a predefined by the given attacker  
#define PATH_ADMIN "C:\\Windows" //Persistence path if the client is running admin mode.
#define PATH_NOT_ADMIN "C:\\Users\\$USER\\AppData\\Roaming" // DO NOT TOUCH

// DO NOT TOUCH:
#define TIMEOUT_CREATE_PROC  10000
#define BUFFER_LEN 4096
#define BUFFER_EXEC 1024 //Buffer for exec
#define TIMEOUT_SOCK 5 
#define TIMEOUT_SOCK_RECONNECT 2000// timeout between each connection
#define SLEEP_RECV 200 
#define TIMEOUT_POPEN 5000 
#define MICRO_SLEEP 100 //micro sleep for timeout
#define SPLIT "|SPLIT|" 


#endif
